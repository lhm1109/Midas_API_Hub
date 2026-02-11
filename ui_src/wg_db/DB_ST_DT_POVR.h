#ifndef __DB_ST_DT_POVR_H__
#define __DB_ST_DT_POVR_H__

#define MIDAS_SDK

#pragma pack(push, 1)

///// Renewal2007 Pushover Analysis Data ///////////////////////////////////////////////////////////////
// MNET:2762-GSJI-200704~
// nCalcMethod --------------------------------------
#define D_PHGT_CALC_AUTO    0 // Auto-Calculation
#define D_PHGT_CALC_USER    1 // User Defined
// nSkcvType ----------------------------------------
#define D_SKCV_STD_BI    0  // Standard Bilinear
#define D_SKCV_STD_TRI   1  // Standard Trilinear
#define D_SKCV_SLP_BI    2  // Slip Bilinear
#define D_SKCV_SLP_TRI   3  // Slip Trilinear
#define D_SKCV_FEMA      4  // FEMA RC/Steel
#define D_SKCV_EC8       5  // Eurocode 8 : 2004
#define D_SKCV_COMP      6  // Compression Only
#define D_SKCV_FEMA_IFST 7  // FEMA Infill Strut Type
#define D_SKCV_FEMA_TRIL 8  // FEMA Trilinear
#define D_SKCV_COUNT D_SKCV_FEMA_TRIL

// FEMA Max Count
#define D_FEMA_MAX_SIZE 20

struct T_PHGT_YLDP    // Yield Surface Property
{
	BOOL   bSymmetricYZ;
	BOOL   bSymmetricPlusMnus;

	double dAxialForce[2];   // [PC0, Pmax]
	double dMomentY[2][2];  // [+,-][M1,M2]  crack / yield(ultimate)
	double dMomentZ[2][2];  // Bilinear, FEMA일 경우 dMomentZ[][0]에 저장할 값이 없는 경우
													// dMomentZ[][1]과 동일한 값 저장해둘 것!!!

	int    nInterpolationMethod; // 0=Ellipse, 1=Linear, 2=User, 3=H-Sect
	double dAlpha;            // nInterpolationMethod==0 -> dAlpha=2.0
														// nInterpolationMethod==1 -> dAlpha=1.0
	BOOL   bSymmetricY;        // dXXXSurface_RatioXXY[]  [1~5] == [7~11]
	BOOL   bSymmetricZ;        // dXXXSurface_RatioXXZ[]  [1~5] == [7~11]
	double d1stSurface_RatioPY[2][11]; // [+,-][E(c), D(c), ...,O,D(t),E(t)]
	double d1stSurface_RatioMY[2][11]; // [+,-][E(c), D(c), ...,O,D(t),E(t)]
	double d1stSurface_RatioPZ[2][11]; // [+,-][E(c), D(c), ...,O,D(t),E(t)]
	double d1stSurface_RatioMZ[2][11]; // [+,-][E(c), D(c), ...,O,D(t),E(t)]

	double d2ndSurface_RatioPY[2][11]; // [+,-][E(c), D(c), ...,O,D(t),E(t)]
	double d2ndSurface_RatioMY[2][11]; // [+,-][E(c), D(c), ...,O,D(t),E(t)]
	double d2ndSurface_RatioPZ[2][11]; // [+,-][E(c), D(c), ...,O,D(t),E(t)]
	double d2ndSurface_RatioMZ[2][11]; // [+,-][E(c), D(c), ...,O,D(t),E(t)]
	
	void Initialize()
	{
		bSymmetricYZ = FALSE; // 20070904 mylee
		bSymmetricPlusMnus = TRUE;
		dAxialForce[0] = 0.5;
		dAxialForce[1] = 1.0;
		nInterpolationMethod=0;
		dAlpha = 2.0;             // <= when nInterpolationMethod==0
		bSymmetricY = TRUE;
		bSymmetricZ = TRUE;    
		double dValueP[11] = {1.0,0.8,0.6,0.4,0.2,0.0,-0.2,-0.4,-0.6,-0.8,-1.0};
		double dValueM[11] = {0.0,0.3,0.6,0.8,0.9,1.0, 0.9,0.8,0.6,0.3,0.0};    
		double dRatio[3] = {0.5, 0.1, 0.667};
		for(int k=0; k<2; ++k)
		{
			dMomentY[k][0] = 0.5;
			dMomentY[k][1] = 1.0;
			dMomentZ[k][0] = 0.5;
			dMomentZ[k][1] = 1.0;
					 
			for(int i=0; i<11; ++i)
			{
				d1stSurface_RatioPY[k][i] = dValueP[i];        
				d1stSurface_RatioPZ[k][i] = dValueP[i];
				d2ndSurface_RatioPY[k][i] = dValueP[i];
				d2ndSurface_RatioPZ[k][i] = dValueP[i];
			}     
		}
		for(int i=0; i<11; ++i)
		{
			d1stSurface_RatioMY[0][i] = dValueM[i];
			d1stSurface_RatioMZ[0][i] = dValueM[i];
			d2ndSurface_RatioMY[0][i] = dValueM[i];
			d2ndSurface_RatioMZ[0][i] = dValueM[i];
			d1stSurface_RatioMY[1][i] = -dValueM[i];
			d1stSurface_RatioMZ[1][i] = -dValueM[i];
			d2ndSurface_RatioMY[1][i] = -dValueM[i];
			d2ndSurface_RatioMZ[1][i] = -dValueM[i];
		}    
	}
	T_PHGT_YLDP() {}
	T_PHGT_YLDP(T_PHGT_YLDP& src)
	{
		*this = src;
	}
	T_PHGT_YLDP& operator=(T_PHGT_YLDP& src)
	{
		bSymmetricYZ       = src.bSymmetricYZ      ;
		bSymmetricPlusMnus = src.bSymmetricPlusMnus;
		dAxialForce[0]     = src.dAxialForce[0];
		dAxialForce[1]     = src.dAxialForce[1];
		nInterpolationMethod=src.nInterpolationMethod;
		dAlpha = src.dAlpha;
		bSymmetricY = src.bSymmetricY;
		bSymmetricZ = src.bSymmetricZ;
		for(int k=0; k<2; ++k)
		{
			for(int i=0;i<2;++i)
			{
				dMomentY[k][i] = src.dMomentY[k][i];
				dMomentZ[k][i] = src.dMomentZ[k][i];
			}      
			for(int i = 0; i<11; ++i)
			{
				d1stSurface_RatioPY[k][i] = src.d1stSurface_RatioPY[k][i];        
				d1stSurface_RatioPZ[k][i] = src.d1stSurface_RatioPZ[k][i];
				d2ndSurface_RatioPY[k][i] = src.d2ndSurface_RatioPY[k][i];
				d2ndSurface_RatioPZ[k][i] = src.d2ndSurface_RatioPZ[k][i];
			}
			for(int i = 0; i<11; ++i)
			{
				d1stSurface_RatioMY[k][i] = src.d1stSurface_RatioMY[k][i];
				d1stSurface_RatioMZ[k][i] = src.d1stSurface_RatioMZ[k][i];
				d2ndSurface_RatioMY[k][i] = src.d2ndSurface_RatioMY[k][i];
				d2ndSurface_RatioMZ[k][i] = src.d2ndSurface_RatioMZ[k][i];
			}
		}
		return *this;
	}
	BOOL operator==(const T_PHGT_YLDP& src) const
	{
		if(bSymmetricYZ       != src.bSymmetricYZ      ) return FALSE;
		if(bSymmetricPlusMnus != src.bSymmetricPlusMnus) return FALSE;
		if(dAxialForce[0]     != src.dAxialForce[0]) return FALSE;
		if(dAxialForce[1]     != src.dAxialForce[1]) return FALSE;
		if(nInterpolationMethod!=src.nInterpolationMethod) return FALSE;
		if(dAlpha != src.dAlpha) return FALSE;
		if(bSymmetricY != src.bSymmetricY) return FALSE;
		if(bSymmetricZ != src.bSymmetricZ) return FALSE;
		for(int k=0; k<2; ++k)
		{
			for(int i=0;i<2;++i)
			{
				if(dMomentY[k][i] != src.dMomentY[k][i]) return FALSE;
				if(dMomentZ[k][i] != src.dMomentZ[k][i]) return FALSE;
			}      
			for(int i = 0; i<11; ++i)
			{
				if(d1stSurface_RatioPY[k][i] != src.d1stSurface_RatioPY[k][i]) return FALSE;        
				if(d1stSurface_RatioPZ[k][i] != src.d1stSurface_RatioPZ[k][i]) return FALSE;
				if(d2ndSurface_RatioPY[k][i] != src.d2ndSurface_RatioPY[k][i]) return FALSE;
				if(d2ndSurface_RatioPZ[k][i] != src.d2ndSurface_RatioPZ[k][i]) return FALSE;
			}
			for(int i = 0; i<11; ++i)
			{
				if(d1stSurface_RatioMY[k][i] != src.d1stSurface_RatioMY[k][i]) return FALSE;
				if(d1stSurface_RatioMZ[k][i] != src.d1stSurface_RatioMZ[k][i]) return FALSE;
				if(d2ndSurface_RatioMY[k][i] != src.d2ndSurface_RatioMY[k][i]) return FALSE;
				if(d2ndSurface_RatioMZ[k][i] != src.d2ndSurface_RatioMZ[k][i]) return FALSE;
			}
		}
		return TRUE;
	}  
	BOOL operator!=(const T_PHGT_YLDP& src) const
	{
		return !( *this == src );
	}
};
struct T_PHGT_YLDP_UNIT
{
	T_PHGT_YLDP_UNIT()
	{
		dAxialForce = D_UNITSYS_BASE_FORCE;
		dMoment     = D_UNITSYS_BASE_MOMENT;
	}
	int dAxialForce;
	int dMoment;
};

struct T_PHGT_YLDS    // Yield Surface  // (before: T_HNGT_INTERACTION)
{
	int         nCalcMethod;     // D_PHGT_CALC_XXXX: 0=Auto, 1=User  
	BOOL        bSymmetricIJ;    // Symmetric for i-end/j-end (T_PHGT_D's nLoc[4]==I&J)

	T_PHGT_YLDP YieldSurfaceProp;  
	T_PHGT_YLDP YieldSurfacePropSub;  //T_PHGT_D's nLoc[4]==I&J 일 경우 J-end 값 저장
	void Initialize()
	{    
		nCalcMethod  =D_PHGT_CALC_AUTO;
		bSymmetricIJ =TRUE;
		YieldSurfaceProp.Initialize();
		YieldSurfacePropSub.Initialize();
	}

	T_PHGT_YLDS(){}
	T_PHGT_YLDS(T_PHGT_YLDS& src)
	{
		*this = src;
	}
	T_PHGT_YLDS& operator=(T_PHGT_YLDS& src)
	{
		nCalcMethod        =src.nCalcMethod;    
		bSymmetricIJ       =src.bSymmetricIJ;
		YieldSurfaceProp   =src.YieldSurfaceProp;
		YieldSurfacePropSub=src.YieldSurfacePropSub;
		return *this;
	}
	BOOL operator!=(const T_PHGT_YLDS& src) const
	{
		if(nCalcMethod != src.nCalcMethod) return TRUE;
		if(bSymmetricIJ != src.bSymmetricIJ) return TRUE;
		if(YieldSurfaceProp != src.YieldSurfaceProp) return TRUE;
		if(YieldSurfacePropSub != src.YieldSurfacePropSub) return TRUE;
		return FALSE;
	}
};

struct T_PHGT_YLDS_UNIT
{
	T_PHGT_YLDP_UNIT Yldp;
};
enum EN_PHGT_MSNR_SPANDREL_TYPE
{
	EN_PHGT_MSNR_REGULAR,
	EN_PHGT_MSNR_IRREGULAR
};

struct T_PHGT_MSNR_885
{
	// Pier
	double dCompStrength ;
	double dShearStrength;    
	double dDistCoeffK   ; // only for Pier
	int    nBuildingType ; // only for Pier 0==New, 1=Exist
	// Spandrel
	double dCompStrength_s ;
	double dShearStrength_s;  
	double dForceHp      ; // only for Spandrel
	void Initialize()
	{
		dCompStrength    = 6400000;
		dShearStrength   = 200000;
		dDistCoeffK      = 0.85;
		nBuildingType    = 0;
		dCompStrength_s  = 4500000;
		dShearStrength_s = 200000;
		dForceHp         = 100000;
	}
	T_PHGT_MSNR_885() {}
	T_PHGT_MSNR_885& operator=(T_PHGT_MSNR_885& src)
	{
		dCompStrength    = src.dCompStrength;
		dShearStrength   = src.dShearStrength;
		nBuildingType    = src.nBuildingType ;
		dDistCoeffK      = src.dDistCoeffK   ;
		dCompStrength_s  = src.dCompStrength_s;
		dShearStrength_s = src.dShearStrength_s;
		dForceHp         = src.dForceHp      ;
		return *this;
	}
	BOOL operator!=(const T_PHGT_MSNR_885& src) const
	{
		if(dCompStrength    != src.dCompStrength   ) return TRUE;
		if(dShearStrength   != src.dShearStrength  ) return TRUE;
		if(dDistCoeffK      != src.dDistCoeffK     ) return TRUE;
		if(nBuildingType	!= src.nBuildingType   ) return TRUE;
		if(dCompStrength_s  != src.dCompStrength_s ) return TRUE;
		if(dShearStrength_s != src.dShearStrength_s) return TRUE;   
		if(dForceHp         != src.dForceHp        ) return TRUE;
		return FALSE;
	}
};

struct T_PHGT_MSNR
{
	// Pier
	double dCompStrength ;
	double dShearStrength;    
	double dDistCoeffK   ; // only for Pier
	int    nBuildingType ; // only for Pier 0==New, 1=Exist
	// Spandrel
	double dCompStrength_s ;
	double dShearStrength_s;  
	double dForceHp      ; // only for Spandrel
	int nSpandrelType; // 0:Regular, 1:Irregular
	void Initialize()
	{
		dCompStrength    = 6400000;
		dShearStrength   = 200000;
		dDistCoeffK      = 0.85;
		nBuildingType    = 0;
		dCompStrength_s  = 4500000;
		dShearStrength_s = 200000;
		dForceHp         = 100000;
		nSpandrelType = EN_PHGT_MSNR_REGULAR;
	}
	T_PHGT_MSNR() {}
	T_PHGT_MSNR& operator=(T_PHGT_MSNR& src)
	{
		dCompStrength    = src.dCompStrength;
		dShearStrength   = src.dShearStrength;
		nBuildingType    = src.nBuildingType ;
		dDistCoeffK      = src.dDistCoeffK   ;
		dCompStrength_s  = src.dCompStrength_s;
		dShearStrength_s = src.dShearStrength_s;
		dForceHp         = src.dForceHp      ;
		nSpandrelType	 = src.nSpandrelType;
		return *this;
	}
	BOOL operator!=(const T_PHGT_MSNR& src) const
	{
		if(dCompStrength    != src.dCompStrength   ) return TRUE;
		if(dShearStrength   != src.dShearStrength  ) return TRUE;
		if(dDistCoeffK      != src.dDistCoeffK     ) return TRUE;
		if(nBuildingType    != src.nBuildingType   ) return TRUE;
		if(dCompStrength_s  != src.dCompStrength_s ) return TRUE;
		if(dShearStrength_s != src.dShearStrength_s) return TRUE;   
		if(dForceHp         != src.dForceHp        ) return TRUE;
		if(nSpandrelType    != src.nSpandrelType	) return TRUE;
		return FALSE;
	}

	void Convert885(T_PHGT_MSNR_885& src)
	{
		dCompStrength    = src.dCompStrength;
		dShearStrength   = src.dShearStrength;
		nBuildingType    = src.nBuildingType ;
		dDistCoeffK      = src.dDistCoeffK   ;
		dCompStrength_s  = src.dCompStrength_s;
		dShearStrength_s = src.dShearStrength_s;
		dForceHp         = src.dForceHp      ;
		//기존모델은 default값 REGULAR으로!
		nSpandrelType	 = EN_PHGT_MSNR_REGULAR;
	}
};

struct T_PHGT_MSNR_UNIT
{
	int dCompStrength; // Stress
	int dShearStrength;
	int dForceHp;
	T_PHGT_MSNR_UNIT()
	{
		dCompStrength = D_UNITSYS_BASE_STRESS;
		dShearStrength= D_UNITSYS_BASE_STRESS;
		dForceHp      = D_UNITSYS_BASE_FORCE;
	}
};
struct T_PHGT_FEMA_730      // FEMA는 M-Theta 
{  
	BOOL   bSymmetric;
	BOOL   bUserDefineRatio;
	double dForceMomentRatio[2][9];    // [Displacement, Force][-E,.., A, B, ..,E]
	
	// Yield Strength
	//BOOL   bUseCalcStrength;
	double dYieldForce[2];     // [+, -]
	double dYieldMoment[2];    // [+, -]
	
	// Deformation
	BOOL   bUserDefineYieldDeform;
	double dDisplacement[2];  // [+, -]
	double dStrain[2];        // [+, -]  
	double dRotation[2];      // [+, -]
	
//   int    nAcceptanceCritera;  // 0=Current/Yield(Total/Yield 기존방법) , 1=Plastic/Yield  
	double dIO[2];            // [+, -] Immediate Occupancy
	double dLS[2];            // [+, -] Life Safety
	double dCP[2];            // [+, -] Collapse Prevention
};

struct T_PHGT_FEMA_815      // FEMA는 M-Theta 
{  
	BOOL   bSymmetric;
	BOOL   bUserDefineRatio;
	double dForceMomentRatio[2][9];    // [Displacement, Force][-E,.., A, B, ..,E]
	
	// Yield Strength
	double dYieldForce[2];     // [+, -]
	double dYieldMoment[2];    // [+, -]
	
	// Deformation
	BOOL   bUserDefineYieldDeform;
	double dDisplacement[2];  // [+, -]
	double dStrain[2];        // [+, -]  
	double dRotation[2];      // [+, -]
	double dRotationU[2];     // [+, -] // 7.4.0 추가 : EC8 에서의 dU 값
	
	double dIO[2];            // [+, -] Immediate Occupancy // EC8 - DL
	double dLS[2];            // [+, -] Life Safety         // EC8 - SD
	double dCP[2];            // [+, -] Collapse Prevention // EC8 - NC
	
	int nClass; // 7.4.0 추가 0:Auto, 1:Class 1, 2:Class 2, 3:Class 3,4
};

struct T_PHGT_FEMA_SUB_885      // FEMA는 M-Theta
{ 
	double dAxialForce;      // 8.2.0 에 추가  // MNET:XXXX-JWKWON-20130614

	double dForceMomentRatio[2][9];    // [Displacement, Force][-E,.., A, B, ..,E]

	double dIO[2];            // [+, -] Immediate Occupancy // EC8 - DL
	double dLS[2];            // [+, -] Life Safety         // EC8 - SD
	double dCP[2];            // [+, -] Collapse Prevention // EC8 - NC
};

struct T_PHGT_FEMA_SUB      // FEMA는 M-Theta
{ 
	double dAxialForce;      // 8.2.0 에 추가  // MNET:XXXX-JWKWON-20130614

	double dForceMomentRatio[2][9];    // [Displacement, Force][-E,.., A, B, ..,E]

	double dIO[2];            // [+, -] Immediate Occupancy // EC8 - DL
	double dLS[2];            // [+, -] Life Safety         // EC8 - SD
	double dCP[2];            // [+, -] Collapse Prevention // EC8 - NC

	double dYieldMomentId[2]; // [+, -] 각각의 Idealized curve에 대한 항복모멘트
	double dRotationId[2];    

	T_PHGT_FEMA_SUB() { Initialize(); }
	T_PHGT_FEMA_SUB(const T_PHGT_FEMA_SUB& src) { *this = src; }
	void Initialize()
	{
		double dDeformRatio[] ={ 1.0, 6.0 , 6.3, 8.0};
		double dForceRatio[]  ={ 1.0, 1.25, 0.2, 0.2};    
		int i = 0;
		for(i=0; i<4; ++i)
		{
			dForceMomentRatio[0][i]=-dDeformRatio[3-i];
			dForceMomentRatio[1][i]=-dForceRatio[3-i];
		}
		dForceMomentRatio[0][i]=0.;
		dForceMomentRatio[1][i]=0.;
		for(i=5; i<9; ++i)
		{
			dForceMomentRatio[0][i]=dDeformRatio[i-5];
			dForceMomentRatio[1][i]=dForceRatio[i-5];
		}
		for(int k=0; k<2; ++k)
		{
			dIO[k]=2.;
			dLS[k]=4.;
			dCP[k]=6.;
			dYieldMomentId[k]=0.0;
			dRotationId[k]=0.0;
		}
		dAxialForce = 0.0;
	}

	void Copy(double dForceMomentRatio_src[2][9], double dIO_src[2], double dLS_src[2], double dCP_src[2])
	{
		for (int k=0; k<2; k++) memcpy(dForceMomentRatio[k], dForceMomentRatio_src[k], sizeof(dForceMomentRatio[k]));
		memcpy(dIO, dIO_src, sizeof(dIO));    
		memcpy(dLS, dLS_src, sizeof(dLS));    
		memcpy(dCP, dCP_src, sizeof(dCP));    
	}

	T_PHGT_FEMA_SUB& operator=(const T_PHGT_FEMA_SUB& src)
	{
		dAxialForce = src.dAxialForce;
		for(int k=0; k<2; ++k)
		{
			for(int i=0; i<9; ++i) dForceMomentRatio[k][i]=src.dForceMomentRatio[k][i];

			dIO[k]=src.dIO[k];
			dLS[k]=src.dLS[k];
			dCP[k]=src.dCP[k];
			dYieldMomentId[k]=src.dYieldMomentId[k];
			dRotationId[k]   =src.dRotationId[k];
		}
		return *this;
	}
	BOOL operator==(const T_PHGT_FEMA_SUB& src) const
	{
		for(int k=0 ; k<2 ; ++k)
		{
			for(int i=0 ; i<9 ; ++i) if(dForceMomentRatio[k][i]!=src.dForceMomentRatio[k][i]) return FALSE;

			if(dIO[k]!=src.dIO[k]) return FALSE;
			if(dLS[k]!=src.dLS[k]) return FALSE;
			if(dCP[k]!=src.dCP[k]) return FALSE;
			if(dYieldMomentId[k]!=src.dYieldMomentId[k]) return FALSE;
			if(dRotationId[k]   !=src.dRotationId[k]) return FALSE;
		}
		if(dAxialForce != src.dAxialForce) return FALSE;
		return TRUE;
	}

	BOOL operator!=(const T_PHGT_FEMA_SUB& src) const
	{
		for(int k=0 ; k<2 ; ++k)
		{
			for(int i=0 ; i<9 ; ++i)
			{
				if(dForceMomentRatio[k][i]!=src.dForceMomentRatio[k][i]) return TRUE;
			}
			if(dIO[k]!=src.dIO[k]) return TRUE;
			if(dLS[k]!=src.dLS[k]) return TRUE;
			if(dCP[k]!=src.dCP[k]) return TRUE;
			if(dYieldMomentId[k]!=src.dYieldMomentId[k]) return TRUE;
			if(dRotationId[k]   !=src.dRotationId[k]) return TRUE;
		}
		if(dAxialForce != src.dAxialForce) return TRUE;
		return FALSE;
	}


	void Convert885(T_PHGT_FEMA_SUB_885& src, double dYieldMoment[2], double dRotation[2])
	{
		dAxialForce = src.dAxialForce;
		for(int k=0; k<2; ++k)
		{
			for(int i=0; i<9; ++i) dForceMomentRatio[k][i]=src.dForceMomentRatio[k][i];

			dIO[k]=src.dIO[k];
			dLS[k]=src.dLS[k];
			dCP[k]=src.dCP[k];
// 여기서 아래 부분은 제외시킴.ㅜㅜ 
// CPO_HingeYieldPropDlgNew, CPO_HingeSkcvTabFemaDlg 에서 관련 데이터가 재계산되기 때문..
// 			dYieldMomentId[k]=dYieldMoment[k];
// 			dRotationId[k]   =dRotation[k];
		}
	}
};

struct T_PHGT_FEMA_ENRG_DISSIPATION
{
	BOOL   bCyclicDegration		;
	double dAB_p				;
	double dC_p					;
	double dD_p					;
	double dE_p					;
	double dAB_m				;
	double dC_m					;
	double dD_m					;
	double dE_m					;
	BOOL   bUnloadingStiffFactor;
	double dUnloadingStiffFactor;

	void Initialize()
	{
		bCyclicDegration = TRUE;
		dAB_p = 1.0;
		dC_p = 0.4;
		dD_p = 0.1;
		dE_p = 0.1;
		dAB_m = 1.0;
		dC_m = 0.4;
		dD_m = 0.1;
		dE_m = 0.1;
		bUnloadingStiffFactor = TRUE;
		dUnloadingStiffFactor = 1.0;
	}

	void Convert(const T_PHGT_FEMA_ENRG_DISSIPATION& rData)
	{
		bCyclicDegration      = rData.bCyclicDegration     ;
		dAB_p                 = rData.dAB_p                ;
		dC_p                  = rData.dC_p                 ;
		dD_p                  = rData.dD_p                 ;
		dE_p                  = rData.dE_p                 ;
		dAB_m                 = rData.dAB_m                ;
		dC_m                  = rData.dC_m                 ;
		dD_m                  = rData.dD_m                 ;
		dE_m                  = rData.dE_m                 ;
		bUnloadingStiffFactor = rData.bUnloadingStiffFactor;
		dUnloadingStiffFactor = rData.dUnloadingStiffFactor;
	}

	BOOL operator!=(const T_PHGT_FEMA_ENRG_DISSIPATION& src) const
	{
		if (bCyclicDegration	 !=src.bCyclicDegration		) return TRUE;
		if (dAB_p				 !=src.dAB_p				) return TRUE;
		if (dC_p				 !=src.dC_p					) return TRUE;
		if (dD_p				 !=src.dD_p					) return TRUE;
		if (dE_p				 !=src.dE_p					) return TRUE;
		if (dAB_m				 !=src.dAB_m				) return TRUE;
		if (dC_m				 !=src.dC_m					) return TRUE;
		if (dD_m				 !=src.dD_m					) return TRUE;
		if (dE_m				 !=src.dE_m					) return TRUE;
		if (bUnloadingStiffFactor!=src.bUnloadingStiffFactor) return TRUE;
		if (dUnloadingStiffFactor!=src.dUnloadingStiffFactor) return TRUE;

		return FALSE;
	}
};

struct T_PHGT_FEMA_865
{  
	BOOL   bSymmetric;
	BOOL   bUserDefineRatio;
	double dForceMomentRatio[2][9];    // [Displacement, Force][-E,.., A, B, ..,E]

	// Yield Strength
	double dYieldForce[2];     // [+, -]
	double dYieldMoment[2];    // [+, -]

	// Deformation
	BOOL   bUserDefineYieldDeform;
	double dDisplacement[2];  // [+, -]
	double dStrain[2];        // [+, -]  
	double dRotation[2];      // [+, -]
	double dRotationU[2];     // [+, -] // 7.4.0 추가 : EC8 에서의 dU 값

	double dIO[2];            // [+, -] Immediate Occupancy // EC8 - DL
	double dLS[2];            // [+, -] Life Safety         // EC8 - SD
	double dCP[2];            // [+, -] Collapse Prevention // EC8 - NC

	int nClass; // 7.4.0 추가 0:Auto, 1:Class 1, 2:Class 2, 3:Class 3,4

	BOOL bMultiCurve;
	int nUseSubCount;
	T_PHGT_FEMA_SUB_885 FEMASub[D_FEMA_MAX_SIZE];
};

struct T_PHGT_FEMA_875
{  
	BOOL   bSymmetric;
	BOOL   bUserDefineRatio;
	double dForceMomentRatio[2][9];    // [Displacement, Force][-E,.., A, B, ..,E]

	// Yield Strength
	double dYieldForce[2];     // [+, -]
	double dYieldMoment[2];    // [+, -]

	// Deformation
	BOOL   bUserDefineYieldDeform;
	double dDisplacement[2];  // [+, -]
	double dStrain[2];        // [+, -]  
	double dRotation[2];      // [+, -]
	double dRotationU[2];     // [+, -] // 7.4.0 추가 : EC8 에서의 dU 값

	double dIO[2];            // [+, -] Immediate Occupancy // EC8 - DL
	double dLS[2];            // [+, -] Life Safety         // EC8 - SD
	double dCP[2];            // [+, -] Collapse Prevention // EC8 - NC

	int nClass; // 7.4.0 추가 0:Auto, 1:Class 1, 2:Class 2, 3:Class 3,4

	BOOL bMultiCurve;
	int nUseSubCount;
	T_PHGT_FEMA_SUB_885 FEMASub[D_FEMA_MAX_SIZE];
	int nDeformType; // 0:D/Dy, 1:Plastic Rotation
};
struct T_PHGT_FEMA_880
{  
	BOOL   bSymmetric;
	BOOL   bUserDefineRatio;
	double dForceMomentRatio[2][9];    // [Displacement, Force][-E,.., A, B, ..,E]

	// Yield Strength
	double dYieldForce[2];     // [+, -]
	double dYieldMoment[2];    // [+, -]

	// Deformation
	BOOL   bUserDefineYieldDeform;
	double dDisplacement[2];  // [+, -]
	double dStrain[2];        // [+, -]  
	double dRotation[2];      // [+, -]
	double dRotationU[2];     // [+, -] // 7.4.0 추가 : EC8 에서의 dU 값

	double dIO[2];            // [+, -] Immediate Occupancy // EC8 - DL
	double dLS[2];            // [+, -] Life Safety         // EC8 - SD
	double dCP[2];            // [+, -] Collapse Prevention // EC8 - NC

	int nClass; // 7.4.0 추가 0:Auto, 1:Class 1, 2:Class 2, 3:Class 3,4

	BOOL bMultiCurve;
	int nUseSubCount;
	T_PHGT_FEMA_SUB_885 FEMASub[D_FEMA_MAX_SIZE];
	int nDeformType; // 0:D/Dy, 1:Plastic Rotation 
	BOOL   bUseShapeCurve         ; // v880~
	int    nShapeCurve            ; // D_PHGT_FEMA_SHAPE
	BOOL   bUseStrengthLoss       ;
	BOOL   bStrengthLoss          ; // Yes,No
	int    nInfillStrutType       ; // D_PHGT_FEMA_IFST
	BOOL   bUseTotalStrLossAtE    ; // Check on/off 
	BOOL   bTotalStrLossAtE       ; // Yes,No
	BOOL   bUseUnloadStiffnessType;
	int    nUnloadStiffnessType   ; // D_PHGT_FEMA_UNLD_TYPE
	double dExpUnloadStiff_Exp    ;
	int    nInfillStiff           ; // 0:User, 1:Elastic Stiffness
	double dInfillStiff_KmiRop    ;
	double dInfillStiff_KmsRop    ;
	double dInfillStiff_Hm        ;
	double dInfillStiff_ThetaM    ;
};

struct T_PHGT_FEMA_881      // FEMA는 M-Theta
{  
	BOOL   bSymmetric;
	BOOL   bUserDefineRatio;
	double dForceMomentRatio[2][9];    // [Displacement, Force][-E,.., A, B, ..,E]

	// Yield Strength
	double dYieldForce[2];     // [+, -]
	double dYieldMoment[2];    // [+, -]

	// Deformation
	BOOL   bUserDefineYieldDeform;
	double dDisplacement[2];  // [+, -]
	double dStrain[2];        // [+, -]  
	double dRotation[2];      // [+, -]
	double dRotationU[2];     // [+, -] // 7.4.0 추가 : EC8 에서의 dU 값

	double dIO[2];            // [+, -] Immediate Occupancy // EC8 - DL
	double dLS[2];            // [+, -] Life Safety         // EC8 - SD
	double dCP[2];            // [+, -] Collapse Prevention // EC8 - NC

	int nClass; // 7.4.0 추가 0:Auto, 1:Class 1, 2:Class 2, 3:Class 3,4

	BOOL bMultiCurve;
	int nUseSubCount;
	T_PHGT_FEMA_SUB_885 FEMASub[D_FEMA_MAX_SIZE];
	int nDeformType; // 0:D/Dy, 1:Plastic Rotation 
	BOOL   bUseShapeCurve         ; // v880~
	int    nShapeCurve            ; // D_PHGT_FEMA_SHAPE
	BOOL   bUseStrengthLoss       ;
	BOOL   bStrengthLoss          ; // Yes,No
	int    nInfillStrutType       ; // D_PHGT_FEMA_IFST
	BOOL   bUseTotalStrLossAtE    ; // Check on/off 
	BOOL   bTotalStrLossAtE       ; // Yes,No
	BOOL   bUseUnloadStiffnessType;
	int    nUnloadStiffnessType   ; // D_PHGT_FEMA_UNLD_TYPE
	double dExpUnloadStiff_Exp    ;
	double dExpUnloadStiff_PRFact ;
	int    nInfillStiff           ; // 0:User, 1:Elastic Stiffness
	double dInfillStiff_KmiRop    ;
	double dInfillStiff_KmsRop    ;
	double dInfillStiff_Hm        ;
	double dInfillStiff_ThetaM    ;
};

struct T_PHGT_FEMA_905 
{
	BOOL   bSymmetric;
	BOOL   bUserDefineRatio;
	double dForceMomentRatio[2][9];    // [Displacement, Force][-E,.., A, B, ..,E]

	// Yield Strength
	double dYieldForce[2];     // [+, -]
	double dYieldMoment[2];    // [+, -]

	// Deformation
	BOOL   bUserDefineYieldDeform;
	double dDisplacement[2];  // [+, -]
	double dStrain[2];        // [+, -]  
	double dRotation[2];      // [+, -]
	double dRotationU[2];     // [+, -] // 7.4.0 추가 : EC8 에서의 dU 값

	double dIO[2];            // [+, -] Immediate Occupancy // EC8 - DL
	double dLS[2];            // [+, -] Life Safety         // EC8 - SD
	double dCP[2];            // [+, -] Collapse Prevention // EC8 - NC

	int nClass; // 7.4.0 추가 0:Auto, 1:Class 1, 2:Class 2, 3:Class 3,4

	BOOL bMultiCurve;
	int nUseSubCount;
	CArray<T_PHGT_FEMA_SUB, T_PHGT_FEMA_SUB&> aFEMASub;
	int nDeformType; // 0:D/Dy, 1:Plastic Rotation 
	BOOL   bUseShapeCurve; // v880~
	int    nShapeCurve; // D_PHGT_FEMA_SHAPE
	BOOL   bUseStrengthLoss;
	BOOL   bStrengthLoss; // Yes,No
	int    nInfillStrutType; // D_PHGT_FEMA_IFST
	BOOL   bUseTotalStrLossAtE; // Check on/off 
	BOOL   bTotalStrLossAtE; // Yes,No
	BOOL   bUseUnloadStiffnessType;
	int    nUnloadStiffnessType; // D_PHGT_FEMA_UNLD_TYPE
	double dExpUnloadStiff_Exp;
	double dExpUnloadStiff_PRFact; // v880
	int    nInfillStiff; // 0:User, 1:Elastic Stiffness
	double dInfillStiff_KmiRop;
	double dInfillStiff_KmsRop;
	double dInfillStiff_Hm;
	double dInfillStiff_ThetaM;

	int nYieldAnalOption; // D_PHGT_FEMA_YIELD_ANAL
};

enum D_PHGT_FEMA_YIELD_ANAL { D_PHGT_FEMA_YIELD_ANAL_OLD, D_PHGT_FEMA_YIELD_ANAL_NEW };
enum D_PHGT_FEMA_SHAPE { D_PHGT_FEMA_SHAPE_GENERAL, D_PHGT_FEMA_SHAPE_PLASTIC };
enum D_PHGT_FEMA_IFST  { D_PHGT_FEMA_IFST_WAIST, D_PHGT_FEMA_IFST_INFILL };
enum D_PHGT_FEMA_UNLD_TYPE  { 
	D_PHGT_FEMA_UNLD_DUMMY          =0,
	D_PHGT_FEMA_UNLD_NORMAL_BILINEAR  ,
	D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED  ,
	D_PHGT_FEMA_UNLD_STIFF_DEGRADE     ,
	D_PHGT_FEMA_UNLD_NONLIN_ELASTIC   ,
	D_PHGT_FEMA_UNLD_COMPRESSION_ONLY ,
	D_PHGT_FEMA_UNLD_STIFF_DEGRADE_TAKEDA,
	D_PHGT_FEMA_UNLD_STIFF_DEGRADE_SLIP,
	D_PHGT_FEMA_UNLD_ENERGY_DISSIPATION, // [ GEN-3447] 이 옵션은 콤보박스에 들어가는건 아니고 라디오로 선택 가능
};
enum D_PHGT_SIGN { D_PHGT_PLUS = 0, D_PHGT_MINUS = 1 };
enum D_PHGT_IJ { D_PHGT_I = 0, D_PHGT_J = 1 };
enum D_PHGT_FEMA_UNLD_OPTION { D_PHGT_FEMA_UNLD_OPTION_SELECT_HYST_MODEL, D_PHGT_FEMA_UNLD_OPTION_ENRG_DISSIPATION };
// Eurocode 8 : 2004 도 이 구조체 사용
struct T_PHGT_FEMA      // FEMA는 M-Theta
{  
	BOOL   bSymmetric;
	BOOL   bUserDefineRatio;
	double dForceMomentRatio[2][9];    // [Displacement, Force][-E,.., A, B, ..,E]
	
	// Yield Strength
	double dYieldForce[2];     // [+, -]
	double dYieldMoment[2];    // [+, -]
	
	// Deformation
	BOOL   bUserDefineYieldDeform;
	double dDisplacement[2];  // [+, -]
	double dStrain[2];        // [+, -]  
	double dRotation[2];      // [+, -]
	double dRotationU[2];     // [+, -] // 7.4.0 추가 : EC8 에서의 dU 값
	
	double dIO[2];            // [+, -] Immediate Occupancy // EC8 - DL
	double dLS[2];            // [+, -] Life Safety         // EC8 - SD
	double dCP[2];            // [+, -] Collapse Prevention // EC8 - NC

	int nClass; // 7.4.0 추가 0:Auto, 1:Class 1, 2:Class 2, 3:Class 3,4

	BOOL bMultiCurve;
	int nUseSubCount;
	CArray<T_PHGT_FEMA_SUB,T_PHGT_FEMA_SUB&> aFEMASub;
	int nDeformType; // 0:D/Dy, 1:Plastic Rotation 
	BOOL   bUseShapeCurve         ; // v880~
	int    nShapeCurve            ; // D_PHGT_FEMA_SHAPE
	BOOL   bUseStrengthLoss       ;
	BOOL   bStrengthLoss          ; // Yes,No
	int    nInfillStrutType       ; // D_PHGT_FEMA_IFST
	BOOL   bUseTotalStrLossAtE    ; // Check on/off 
	BOOL   bTotalStrLossAtE       ; // Yes,No
	BOOL   bUseUnloadStiffnessType;
	int    nUnloadStiffnessType   ; // D_PHGT_FEMA_UNLD_TYPE
	double dExpUnloadStiff_Exp    ;
	double dExpUnloadStiff_PRFact ; // v880
	int    nInfillStiff           ; // 0:User, 1:Elastic Stiffness
	double dInfillStiff_KmiRop    ;
	double dInfillStiff_KmsRop    ;
	double dInfillStiff_Hm        ;
	double dInfillStiff_ThetaM    ;

	int nYieldAnalOption          ; // D_PHGT_FEMA_YIELD_ANAL
	int nUnloadStiffOption; // D_PHGT_FEMA_UNLD_OPTION
	T_PHGT_FEMA_ENRG_DISSIPATION enrgDissipation;

	T_PHGT_FEMA() { Initialize(); }
	T_PHGT_FEMA(const T_PHGT_FEMA& src) { *this = src; }
		
	void Initialize()
	{
		bSymmetric = TRUE;
		bUserDefineRatio      =FALSE;
		bUserDefineYieldDeform=FALSE;
		double dDeformRatio[] ={ 1.0, 6.0 , 6.3, 8.0};
		double dForceRatio[]  ={ 1.0, 1.25, 0.2, 0.2};    
		int i = 0;
		for(i=0; i<4; ++i)
		{
			dForceMomentRatio[0][i]=-dDeformRatio[3-i];
			dForceMomentRatio[1][i]=-dForceRatio[3-i];
		}
		dForceMomentRatio[0][i]=0.;
		dForceMomentRatio[1][i]=0.;
		for(i=5; i<9; ++i)
		{
			dForceMomentRatio[0][i]=dDeformRatio[i-5];
			dForceMomentRatio[1][i]=dForceRatio[i-5];
		}
		for(int k=0; k<2; ++k)
		{
			dYieldForce[k] =1.0;
			dYieldMoment[k]=1.0;
			dDisplacement[k]=0.;
			dStrain[k]  =0.;
			dRotation[k]=0.;
			dRotationU[k]=0.;
			dIO[k]=2.;
			dLS[k]=4.;
			dCP[k]=6.;
		}
		nClass = 1;

		bMultiCurve = FALSE;
		nUseSubCount=1;
		T_PHGT_FEMA_SUB sub;
		aFEMASub.RemoveAll();
		aFEMASub.Add(sub);

		nDeformType = 0;
		bUseShapeCurve          = FALSE;
		nShapeCurve             = D_PHGT_FEMA_SHAPE_GENERAL;
		bUseStrengthLoss        = FALSE;
		bStrengthLoss           = TRUE;
		nInfillStrutType        = 1;
		bUseTotalStrLossAtE     = FALSE;
		bTotalStrLossAtE        = FALSE;
		bUseUnloadStiffnessType = FALSE;
		nUnloadStiffnessType    = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
		dExpUnloadStiff_Exp     = 0.4;
		dExpUnloadStiff_PRFact  = 0.5;
		nInfillStiff            = 1;
		dInfillStiff_KmiRop     = 2000.0;
		dInfillStiff_KmsRop     = 500.0;
		dInfillStiff_Hm         = 1.0;
		dInfillStiff_ThetaM     = 1.0;
		nYieldAnalOption        = D_PHGT_FEMA_YIELD_ANAL_NEW;
		nUnloadStiffOption      = D_PHGT_FEMA_UNLD_OPTION_SELECT_HYST_MODEL;
		enrgDissipation.Initialize();
	}
	void InitializeRatio_Infill()
	{
		Initialize();
		nUnloadStiffnessType = D_PHGT_FEMA_UNLD_NORMAL_BILINEAR;
		dForceMomentRatio[0][0] = -0.005;  dForceMomentRatio[1][0] = -0;     
		dForceMomentRatio[0][1] = -0.005;  dForceMomentRatio[1][1] = -0;     
		dForceMomentRatio[0][2] = -6;      dForceMomentRatio[1][2] = -1;     
		dForceMomentRatio[0][3] = -1;      dForceMomentRatio[1][3] = -0.7;   
		dForceMomentRatio[0][4] = 0;       dForceMomentRatio[1][4] = 0.0;    
	}
	void InitializeFEMADDyValue()
	{
		nDeformType = 0;
		dForceMomentRatio[0][0] = -8;
		dForceMomentRatio[0][1] = -6.3;
		dForceMomentRatio[0][2] = -6;
		dForceMomentRatio[0][3] = -1;
		dForceMomentRatio[0][4] = 0.0;
		dForceMomentRatio[0][5] = 1;
		dForceMomentRatio[0][6] = 6;
		dForceMomentRatio[0][7] = 6.3;
		dForceMomentRatio[0][8] = 8;
		for(int k=0; k<2; ++k)
		{
			dIO[k]=2.;
			dLS[k]=4.;
			dCP[k]=6.;
		}
	}
	void InitializeFEMAPlasticDeformValue()
	{
		nDeformType = 1;
		dForceMomentRatio[0][0] = -0.03;
		dForceMomentRatio[0][1] = -0.025;
		dForceMomentRatio[0][2] = -0.02;
		dForceMomentRatio[0][3] = 0.0;
		dForceMomentRatio[0][4] = 0.0;
		dForceMomentRatio[0][5] = 0.0;
		dForceMomentRatio[0][6] = 0.02;
		dForceMomentRatio[0][7] = 0.025;
		dForceMomentRatio[0][8] = 0.03;
		for(int k=0; k<2; ++k)
		{
			dIO[k]=0.005;
			dLS[k]=0.01;
			dCP[k]=0.02;
		}
	}
	void InitializeMasonry(int nDof, BOOL bExist=FALSE) // 20071207 mylee - Masonry 초기화시 쓰임
	{
		bSymmetric = TRUE;
		bUserDefineRatio      =TRUE;
		bUserDefineYieldDeform=FALSE;
		for(int k=0; k<2; ++k)
		{
			dYieldForce[k] =1.0;
			dYieldMoment[k]=1.0;
			dDisplacement[k]=0.;
			dStrain[k]  =0.;
			dRotation[k]=0.;
			dRotationU[k]=0.;
			dIO[k]=2.;
			dLS[k]=4.;
			dCP[k]=6.;
		}
		nClass = 1;

		if(nDof==0)
		{
			bSymmetric = FALSE;
			double dForceRatio[] ={ -0.2, -0.2, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0};
			double dDeformRatio[]  ={ -1.75, -1.75, -1.75, -1.0, 0.0, 5.0, 5.0, 5.0, 5.0};
			for(int i=0; i<9; ++i)
			{
				dForceMomentRatio[0][i]=dDeformRatio[i];
				dForceMomentRatio[1][i]=dForceRatio[i];
			}
		}
		else if(nDof==2)
		{
			double dForceRatio[]  ={ -0.3, -0.3, -1.001, -1.0, 0.0, 1.0, 1.001, 0.3, 0.3};
			double dDeformRatio[] ={ -0.006, -0.004, -0.004, -0.001, 0.0, 0.001, 0.004, 0.004, 0.006};
			for(int i=0; i<9; ++i)
			{
				dForceMomentRatio[0][i]=dDeformRatio[i];
				dForceMomentRatio[1][i]=dForceRatio[i];
			}
		}
		else if(nDof==4)
		{
			double dForceRatio[]  ={ -0.3, -0.3, -1.001, -1.0, 0.0, 1.0, 1.001, 0.3, 0.3};
			double dDeformRatio[] ={ -0.012, -0.008, -0.008, -0.001, 0.0, 0.001, 0.008, 0.008, 0.012};
			for(int i=0; i<9; ++i)
			{
				dForceMomentRatio[0][i]=dDeformRatio[i];
				dForceMomentRatio[1][i]=dForceRatio[i];
			}
			if(bExist)
			{
				double dDeformRatio[] ={ -0.009, -0.006, -0.006, -0.001, 0.0, 0.001, 0.006, 0.006, 0.009};
				for(int i=0; i<9; ++i)
					dForceMomentRatio[0][i]=dDeformRatio[i];
			}
		}
		else ASSERT(0);

		bMultiCurve = FALSE;
		nUseSubCount=1;
		T_PHGT_FEMA_SUB sub;
		sub.Copy(dForceMomentRatio, dIO, dLS, dCP);
		aFEMASub.RemoveAll();
		aFEMASub.Add(sub);
		nDeformType = 0;
		bUseShapeCurve          = FALSE;
		nShapeCurve             = D_PHGT_FEMA_SHAPE_GENERAL;
		bUseStrengthLoss        = FALSE;
		bStrengthLoss           = TRUE;
		nInfillStrutType        = 1;
		bUseTotalStrLossAtE     = FALSE;
		bTotalStrLossAtE        = FALSE;
		bUseUnloadStiffnessType = FALSE;
		nUnloadStiffnessType    = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
		dExpUnloadStiff_Exp     = 0.4;
		dExpUnloadStiff_PRFact  = 0.5;
		nInfillStiff            = 1;
		dInfillStiff_KmiRop     = 2000.0;
		dInfillStiff_KmsRop     = 500.0;
		dInfillStiff_Hm         = 1.0;
		dInfillStiff_ThetaM     = 1.0;
		nYieldAnalOption        = D_PHGT_FEMA_YIELD_ANAL_NEW;
		nUnloadStiffOption      = D_PHGT_FEMA_UNLD_OPTION_SELECT_HYST_MODEL;
		enrgDissipation.Initialize();
	}
	void InitializeEC8(int nDof, int nElem, int nMatl, int nClassInput=1) // 20071018 mylee - EC8 로 초기화할때 쓰임
	{
		bSymmetric = FALSE;
		bUserDefineRatio      =FALSE;
		bUserDefineYieldDeform=FALSE;
		for(int k=0; k<2; ++k)
		{
			dYieldForce[k] =1.0;
			dYieldMoment[k]=1.0;
			dDisplacement[k]=1.;
			dStrain[k]  =1.;
			dRotation[k]=1.;
			dRotationU[k]=9.;
		}
		nClass = nClassInput;

		if(nMatl==1) // Steel
		{
			if(nDof==0 && (nElem==0 || nElem==1)) // Fx, Beam/Col
			{
				double dDeformRatio[] ={ -8.0, -1.01, -1.01, -1.0, 0.0, 1.0, 6.0, 6.0, 8.0};
				double dForceRatio[]  ={ -0.2, -0.2, -1.0, -1.0, 0.0, 1.0, 1.001, 0.2, 0.2};    
				for(int i=0; i<9; ++i)
				{
					dForceMomentRatio[0][i]=dDeformRatio[i];
					dForceMomentRatio[1][i]=dForceRatio[i];
				}
				dIO[0]=1.25; dIO[1]=1.0;
				dLS[0]=4.0; dLS[1]=1.0;
				dCP[0]=6.0; dCP[1]=1.0;
			}
			else if(nDof==0 && nElem==2) // Fx, Truss
			{
				if(nClass==0 || nClass==1)
				{
					double dDeformRatio[] ={ -12.0, -1.01, -1.01, -1.0, 0.0, 1.0, 10.0, 10.0, 12.0};
					double dForceRatio[]  ={ -0.2, -0.2, -1.001, -1.0, 0.0, 1.0, 1.001, 0.2, 0.2};    
					for(int i=0; i<9; ++i)
					{
						dForceMomentRatio[0][i]=dDeformRatio[i];
						dForceMomentRatio[1][i]=dForceRatio[i];
					}
					dIO[0]=1.25; dIO[1]=0.25;
					dLS[0]=8.0; dLS[1]=4.0;
					dCP[0]=10.0; dCP[1]=6.0;
				}
				else if(nClass==2)
				{
					double dDeformRatio[] ={ -12.0, -1.01, -1.01, -1.0, 0.0, 1.0, 10.0, 10.0, 12.0};
					double dForceRatio[]  ={ -0.2, -0.2, -1.001, -1.0, 0.0, 1.0, 1.001, 0.2, 0.2};    
					for(int i=0; i<9; ++i)
					{
						dForceMomentRatio[0][i]=dDeformRatio[i];
						dForceMomentRatio[1][i]=dForceRatio[i];
					}
					dIO[0]=1.25; dIO[1]=0.25;
					dLS[0]=8.0; dLS[1]=1.0;
					dCP[0]=10.0; dCP[1]=2.0;
				}
				else if(nClass==3)
				{
					double dDeformRatio[] ={ -8.0, -6.0, -1.0, -1.0, 0.0, 1.0, 1.0, 6.0, 8.0};
					double dForceRatio[]  ={ -0.2, -0.2, -0.2, -1.0, 0.0, 1.0, 0.2, 0.2, 0.2};    
					for(int i=0; i<9; ++i)
					{
						dForceMomentRatio[0][i]=dDeformRatio[i];
						dForceMomentRatio[1][i]=dForceRatio[i];
					}
					dIO[0]=1.0; dIO[1]=1.0;
					dLS[0]=1.0; dLS[1]=1.0;
					dCP[0]=1.0; dCP[1]=1.0;
				}
			}
			else if(nDof==1 || nDof==2) // Fy, Fz
			{
				double dDeformRatio[] ={ -8.0, -1.01, -1.01, -1.0, 0.0, 1.0, 1.01, 1.01, 8.0};
				double dForceRatio[]  ={ -0.2, -0.2, -1.0, -1.0, 0.0, 1.0, 1.0, 0.2, 0.2};    
				for(int i=0; i<9; ++i)
				{
					dForceMomentRatio[0][i]=dDeformRatio[i];
					dForceMomentRatio[1][i]=dForceRatio[i];
				}
				dIO[0]=1.0; dIO[1]=1.0;
				dLS[0]=1.0; dLS[1]=1.0;
				dCP[0]=1.0; dCP[1]=1.0;
			}
			else if(nDof==4 || nDof==5) // My, Mz
			{
				if(nClass==0 || nClass==1)
				{
					double dDeformRatio[] ={ -12.0, -9.0, -9.0, -1.0, 0.0, 1.0, 9.0, 9.0, 12.0};
					double dForceRatio[]  ={ -0.2, -0.2, -1.001, -1.0, 0.0, 1.0, 1.001, 0.2, 0.2};    
					for(int i=0; i<9; ++i)
					{
						dForceMomentRatio[0][i]=dDeformRatio[i];
						dForceMomentRatio[1][i]=dForceRatio[i];
					}
					dIO[0]=2.0; dIO[1]=2.0;
					dLS[0]=7.0; dLS[1]=7.0;
					dCP[0]=9.0; dCP[1]=9.0;
				}
				else if(nClass==2)
				{
					double dDeformRatio[] ={ -8.0, -4.0, -4.0, -1.0, 0.0, 1.0, 4.0, 4.0, 8.0};
					double dForceRatio[]  ={ -0.2, -0.2, -1.001, -1.0, 0.0, 1.0, 1.001, 0.2, 0.2};    
					for(int i=0; i<9; ++i)
					{
						dForceMomentRatio[0][i]=dDeformRatio[i];
						dForceMomentRatio[1][i]=dForceRatio[i];
					}
					dIO[0]=1.25; dIO[1]=1.25;
					dLS[0]=3.0; dLS[1]=3.0;
					dCP[0]=4.0; dCP[1]=4.0;
				}
				else if(nClass==3)
				{
					double dDeformRatio[] ={ -8.0, -6.0, -1.0, -1.0, 0.0, 1.0, 1.0, 6.0, 8.0};
					double dForceRatio[]  ={ -0.2, -0.2, -0.2, -1.0, 0.0, 1.0, 0.2, 0.2, 0.2};    
					for(int i=0; i<9; ++i)
					{
						dForceMomentRatio[0][i]=dDeformRatio[i];
						dForceMomentRatio[1][i]=dForceRatio[i];
					}
					dIO[0]=1.0; dIO[1]=1.0;
					dLS[0]=1.0; dLS[1]=1.0;
					dCP[0]=1.0; dCP[1]=1.0;
				}
			}
		}
		if(nMatl==0) // RC
		{
			if(nDof==0) // Fx
			{
				double dDeformRatio[] ={ -8.0, -1.01, -1.01, -1.0, 0.0, 1.0, 6.0, 6.0, 8.0};
				double dForceRatio[]  ={ -0.2, -0.2, -1.0, -1.0, 0.0, 1.0, 1.001, 0.2, 0.2};    
				for(int i=0; i<9; ++i)
				{
					dForceMomentRatio[0][i]=dDeformRatio[i];
					dForceMomentRatio[1][i]=dForceRatio[i];
				}
				dIO[0]=1.25; dIO[1]=1.0;
				dLS[0]=4.0; dLS[1]=1.0;
				dCP[0]=6.0; dCP[1]=1.0;
			}
			else if(nDof==1 || nDof==2) // Fy, Fz
			{
				double dDeformRatio[] ={ -8.0, -1.01, -1.01, -1.0, 0.0, 1.0, 1.01, 1.01, 8.0};
				double dForceRatio[]  ={ -0.2, -0.2, -1.0, -1.0, 0.0, 1.0, 1.0, 0.2, 0.2};    
				for(int i=0; i<9; ++i)
				{
					dForceMomentRatio[0][i]=dDeformRatio[i];
					dForceMomentRatio[1][i]=dForceRatio[i];
				}
				dIO[0]=1.0; dIO[1]=1.0;
				dLS[0]=1.0; dLS[1]=1.0;
				dCP[0]=1.0; dCP[1]=1.0;
			}
			else if(nDof==4 || nDof==5) // My, Mz
			{
				double dDeformRatio[] ={ -12.0, -9.0, -9.0, -1.0, 0.0, 1.0, 9.0, 9.0, 12.0};
				double dForceRatio[]  ={ -0.2, -0.2, -1.001, -1.0, 0.0, 1.0, 1.001, 0.2, 0.2};
				for(int i=0; i<9; ++i)
				{
					dForceMomentRatio[0][i]=dDeformRatio[i];
					dForceMomentRatio[1][i]=dForceRatio[i];
				}
				dIO[0]=1.0; dIO[1]=1.0;
				dLS[0]=0.75; dLS[1]=0.75;
				dCP[0]=1.0; dCP[1]=1.0;
			}
		}
		
		bMultiCurve = FALSE;
		nUseSubCount = 1;
		T_PHGT_FEMA_SUB sub;
		sub.Copy(dForceMomentRatio, dIO, dLS, dCP);
		aFEMASub.RemoveAll();
		aFEMASub.Add(sub);
		nDeformType = 0;
		bUseShapeCurve          = FALSE;
		nShapeCurve             = D_PHGT_FEMA_SHAPE_GENERAL;
		bUseStrengthLoss        = FALSE;
		bStrengthLoss           = TRUE;
		nInfillStrutType        = 1;
		bUseTotalStrLossAtE     = FALSE;
		bTotalStrLossAtE        = FALSE;
		bUseUnloadStiffnessType = FALSE;
		nUnloadStiffnessType    = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
		dExpUnloadStiff_Exp     = 0.4;
		dExpUnloadStiff_PRFact  = 0.5;
		nInfillStiff            = 1;
		dInfillStiff_KmiRop     = 2000.0;
		dInfillStiff_KmsRop     = 500.0;
		dInfillStiff_Hm         = 1.0;
		dInfillStiff_ThetaM     = 1.0;
		nYieldAnalOption        = D_PHGT_FEMA_YIELD_ANAL_NEW;
		nUnloadStiffOption      = D_PHGT_FEMA_UNLD_OPTION_SELECT_HYST_MODEL;
		enrgDissipation.Initialize();
	}
	T_PHGT_FEMA& operator=(const T_PHGT_FEMA& src)
	{
		nClass      = src.nClass;
		bSymmetric  = src.bSymmetric;
		bUserDefineRatio      =src.bUserDefineRatio;
		bUserDefineYieldDeform=src.bUserDefineYieldDeform;
		for(int k=0; k<2; ++k)
		{
			for(int i=0; i<9; ++i)
				dForceMomentRatio[k][i]=src.dForceMomentRatio[k][i];
			dYieldForce[k]  =src.dYieldForce[k]  ;
			dYieldMoment[k] =src.dYieldMoment[k] ;
			dDisplacement[k]=src.dDisplacement[k];
			dStrain[k]      =src.dStrain[k];
			dRotation[k]    =src.dRotation[k];
			dRotationU[k]   =src.dRotationU[k];
			dIO[k]=src.dIO[k];
			dLS[k]=src.dLS[k];
			dCP[k]=src.dCP[k];
		}
		
		bMultiCurve  = src.bMultiCurve; 
		nUseSubCount = src.nUseSubCount;
		aFEMASub.Copy(src.aFEMASub);
		if(aFEMASub.GetSize() > nUseSubCount)
			aFEMASub.SetSize(nUseSubCount);
		nDeformType = src.nDeformType;
		bUseShapeCurve          = src.bUseShapeCurve         ;
		nShapeCurve             = src.nShapeCurve            ;
		bUseStrengthLoss        = src.bUseStrengthLoss       ;
		bStrengthLoss           = src.bStrengthLoss          ;
		nInfillStrutType        = src.nInfillStrutType       ;
		bUseTotalStrLossAtE     = src.bUseTotalStrLossAtE    ;
		bTotalStrLossAtE        = src.bTotalStrLossAtE       ;
		bUseUnloadStiffnessType = src.bUseUnloadStiffnessType;
		nUnloadStiffnessType    = src.nUnloadStiffnessType   ;
		dExpUnloadStiff_Exp     = src.dExpUnloadStiff_Exp    ;
		dExpUnloadStiff_PRFact  = src.dExpUnloadStiff_PRFact ;
		nInfillStiff            = src.nInfillStiff           ;
		dInfillStiff_KmiRop     = src.dInfillStiff_KmiRop    ;
		dInfillStiff_KmsRop     = src.dInfillStiff_KmsRop    ;
		dInfillStiff_Hm         = src.dInfillStiff_Hm        ;
		dInfillStiff_ThetaM     = src.dInfillStiff_ThetaM    ;
		nYieldAnalOption        = src.nYieldAnalOption       ;
		nUnloadStiffOption      = src.nUnloadStiffOption     ;
		enrgDissipation			= src.enrgDissipation		 ;

		return *this;
	}
	BOOL operator==(const T_PHGT_FEMA& src) const
	{
		if(nClass != src.nClass) return FALSE;
		if(bSymmetric != src.bSymmetric) return FALSE;
		if(bUserDefineRatio      !=src.bUserDefineRatio) return FALSE;
		if(bUserDefineYieldDeform !=src.bUserDefineYieldDeform) return FALSE;
		for(int k=0 ; k<2 ; ++k)
		{
			for(int i=0 ; i<9 ; ++i)
				if(dForceMomentRatio[k][i]!=src.dForceMomentRatio[k][i]) return FALSE;
			if(dYieldForce[k]  !=src.dYieldForce[k])   return FALSE;
			if(dYieldMoment[k] !=src.dYieldMoment[k])  return FALSE;
			if(dDisplacement[k]!=src.dDisplacement[k]) return FALSE;
			if(dStrain[k]      !=src.dStrain[k]) return FALSE;
			if(dRotation[k]    !=src.dRotation[k]) return FALSE;
			if(dRotationU[k]   !=src.dRotationU[k]) return FALSE;
			if(dIO[k]!=src.dIO[k]) return FALSE;
			if(dLS[k]!=src.dLS[k]) return FALSE;
			if(dCP[k]!=src.dCP[k]) return FALSE;
		}
		if(bMultiCurve !=src.bMultiCurve)  return FALSE;
		if(nUseSubCount!=src.nUseSubCount) return FALSE;
		if(aFEMASub.GetSize() != src.aFEMASub.GetSize()) return FALSE;
		for(int i=0; i<aFEMASub.GetSize(); i++)
		{
			if(aFEMASub[i] != src.aFEMASub[i]) return FALSE;
		}
		if(nDeformType != src.nDeformType) return FALSE;
		if(bUseShapeCurve          != src.bUseShapeCurve         ) return FALSE;
		if(nShapeCurve             != src.nShapeCurve            ) return FALSE;
		if(bUseStrengthLoss        != src.bUseStrengthLoss       ) return FALSE;
		if(bStrengthLoss           != src.bStrengthLoss          ) return FALSE;
		if(nInfillStrutType        != src.nInfillStrutType       ) return FALSE;
		if(bUseTotalStrLossAtE     != src.bUseTotalStrLossAtE    ) return FALSE;
		if(bTotalStrLossAtE        != src.bTotalStrLossAtE       ) return FALSE;
		if(bUseUnloadStiffnessType != src.bUseUnloadStiffnessType) return FALSE;
		if(nUnloadStiffnessType    != src.nUnloadStiffnessType   ) return FALSE;
		if(dExpUnloadStiff_Exp     != src.dExpUnloadStiff_Exp    ) return FALSE;
		if(dExpUnloadStiff_PRFact  != src.dExpUnloadStiff_PRFact ) return FALSE;
		if(nInfillStiff            != src.nInfillStiff           ) return FALSE;
		if(dInfillStiff_KmiRop     != src.dInfillStiff_KmiRop    ) return FALSE;
		if(dInfillStiff_KmsRop     != src.dInfillStiff_KmsRop    ) return FALSE;
		if(dInfillStiff_Hm         != src.dInfillStiff_Hm        ) return FALSE;
		if(dInfillStiff_ThetaM     != src.dInfillStiff_ThetaM    ) return FALSE;
		if(nYieldAnalOption        != src.nYieldAnalOption       ) return FALSE;
		if (nUnloadStiffOption     != src.nUnloadStiffOption) return FALSE;
		if (enrgDissipation        != src.enrgDissipation) return FALSE;

		return TRUE;
	}
	BOOL operator!=(const T_PHGT_FEMA& src) const
	{
		return !( *this == src );
	}
	
	void InsertFEMASub(int idx, T_PHGT_FEMA_SUB& src)
	{
		aFEMASub.InsertAt(idx, src);
		nUseSubCount++;
	}
	void DeleteFEMASub(int idx)
	{
		aFEMASub.RemoveAt(idx);
		nUseSubCount--;
	}
};

struct T_PHGT_FEMA_RW_896
{  
	BOOL   bSymmetric;
	BOOL   bUserDefineRatio;
	double dForceMomentRatio[2][9];    // [Displacement, Force][-E,.., A, B, ..,E]

	// Yield Strength
	double dYieldForce[2];     // [+, -]
	double dYieldMoment[2];    // [+, -]

	// Deformation
	BOOL   bUserDefineYieldDeform;
	double dDisplacement[2];  // [+, -]
	double dStrain[2];        // [+, -]  
	double dRotation[2];      // [+, -]
	double dRotationU[2];     // [+, -] // 7.4.0 추가 : EC8 에서의 dU 값

	double dIO[2];            // [+, -] Immediate Occupancy // EC8 - DL
	double dLS[2];            // [+, -] Life Safety         // EC8 - SD
	double dCP[2];            // [+, -] Collapse Prevention // EC8 - NC

	int nClass; // 7.4.0 추가 0:Auto, 1:Class 1, 2:Class 2, 3:Class 3,4

	BOOL bMultiCurve;
	int nUseSubCount;
	int nDeformType; // 0:D/Dy, 1:Plastic Rotation 
	BOOL   bUseShapeCurve         ; // v880~
	int    nShapeCurve            ; // D_PHGT_FEMA_SHAPE
	BOOL   bUseStrengthLoss       ;
	BOOL   bStrengthLoss          ; // Yes,No
	int    nInfillStrutType       ; // D_PHGT_FEMA_IFST
	BOOL   bUseTotalStrLossAtE    ; // Check on/off 
	BOOL   bTotalStrLossAtE       ; // Yes,No
	BOOL   bUseUnloadStiffnessType;
	int    nUnloadStiffnessType   ; // D_PHGT_FEMA_UNLD_TYPE
	double dExpUnloadStiff_Exp    ;
	double dExpUnloadStiff_PRFact ; // v880
	int    nInfillStiff           ; // 0:User, 1:Elastic Stiffness
	double dInfillStiff_KmiRop    ;
	double dInfillStiff_KmsRop    ;
	double dInfillStiff_Hm        ;
	double dInfillStiff_ThetaM    ;
};

struct T_PHGT_FEMA_RW_905
{
	BOOL   bSymmetric;
	BOOL   bUserDefineRatio;
	double dForceMomentRatio[2][9];    // [Displacement, Force][-E,.., A, B, ..,E]

	// Yield Strength
	double dYieldForce[2];     // [+, -]
	double dYieldMoment[2];    // [+, -]

	// Deformation
	BOOL   bUserDefineYieldDeform;
	double dDisplacement[2];  // [+, -]
	double dStrain[2];        // [+, -]  
	double dRotation[2];      // [+, -]
	double dRotationU[2];     // [+, -] // 7.4.0 추가 : EC8 에서의 dU 값

	double dIO[2];            // [+, -] Immediate Occupancy // EC8 - DL
	double dLS[2];            // [+, -] Life Safety         // EC8 - SD
	double dCP[2];            // [+, -] Collapse Prevention // EC8 - NC

	int nClass; // 7.4.0 추가 0:Auto, 1:Class 1, 2:Class 2, 3:Class 3,4

	BOOL bMultiCurve;
	int nUseSubCount;
	int nDeformType; // 0:D/Dy, 1:Plastic Rotation 
	BOOL   bUseShapeCurve; // v880~
	int    nShapeCurve; // D_PHGT_FEMA_SHAPE
	BOOL   bUseStrengthLoss;
	BOOL   bStrengthLoss; // Yes,No
	int    nInfillStrutType; // D_PHGT_FEMA_IFST
	BOOL   bUseTotalStrLossAtE; // Check on/off 
	BOOL   bTotalStrLossAtE; // Yes,No
	BOOL   bUseUnloadStiffnessType;
	int    nUnloadStiffnessType; // D_PHGT_FEMA_UNLD_TYPE
	double dExpUnloadStiff_Exp;
	double dExpUnloadStiff_PRFact; // v880
	int    nInfillStiff; // 0:User, 1:Elastic Stiffness
	double dInfillStiff_KmiRop;
	double dInfillStiff_KmsRop;
	double dInfillStiff_Hm;
	double dInfillStiff_ThetaM;

	int nYieldAnalOption; // D_PHGT_FEMA_YIELD_ANAL
};

struct T_PHGT_FEMA_RW
{  
	BOOL   bSymmetric;
	BOOL   bUserDefineRatio;
	double dForceMomentRatio[2][9];    // [Displacement, Force][-E,.., A, B, ..,E]

	// Yield Strength
	double dYieldForce[2];     // [+, -]
	double dYieldMoment[2];    // [+, -]

	// Deformation
	BOOL   bUserDefineYieldDeform;
	double dDisplacement[2];  // [+, -]
	double dStrain[2];        // [+, -]  
	double dRotation[2];      // [+, -]
	double dRotationU[2];     // [+, -] // 7.4.0 추가 : EC8 에서의 dU 값

	double dIO[2];            // [+, -] Immediate Occupancy // EC8 - DL
	double dLS[2];            // [+, -] Life Safety         // EC8 - SD
	double dCP[2];            // [+, -] Collapse Prevention // EC8 - NC

	int nClass; // 7.4.0 추가 0:Auto, 1:Class 1, 2:Class 2, 3:Class 3,4

	BOOL bMultiCurve;
	int nUseSubCount;
	int nDeformType; // 0:D/Dy, 1:Plastic Rotation 
	BOOL   bUseShapeCurve         ; // v880~
	int    nShapeCurve            ; // D_PHGT_FEMA_SHAPE
	BOOL   bUseStrengthLoss       ;
	BOOL   bStrengthLoss          ; // Yes,No
	int    nInfillStrutType       ; // D_PHGT_FEMA_IFST
	BOOL   bUseTotalStrLossAtE    ; // Check on/off 
	BOOL   bTotalStrLossAtE       ; // Yes,No
	BOOL   bUseUnloadStiffnessType;
	int    nUnloadStiffnessType   ; // D_PHGT_FEMA_UNLD_TYPE
	double dExpUnloadStiff_Exp    ;
	double dExpUnloadStiff_PRFact ; // v880
	int    nInfillStiff           ; // 0:User, 1:Elastic Stiffness
	double dInfillStiff_KmiRop    ;
	double dInfillStiff_KmsRop    ;
	double dInfillStiff_Hm        ;
	double dInfillStiff_ThetaM    ;

	int nYieldAnalOption          ; // D_PHGT_FEMA_YIELD_ANAL
	int nUnloadStiffOption; // D_PHGT_FEMA_UNLD_OPTION
	T_PHGT_FEMA_ENRG_DISSIPATION enrgDissipation;

	void ConvertToData(T_PHGT_FEMA& data)
	{
		data.nClass      = nClass;
		data.bSymmetric  = bSymmetric;
		data.bUserDefineRatio      =bUserDefineRatio;
		data.bUserDefineYieldDeform=bUserDefineYieldDeform;
		for(int k=0; k<2; ++k)
		{
			for(int i=0; i<9; ++i)
				data.dForceMomentRatio[k][i]=dForceMomentRatio[k][i];
			data.dYieldForce[k]  =dYieldForce[k]  ;
			data.dYieldMoment[k] =dYieldMoment[k] ;
			data.dDisplacement[k]=dDisplacement[k];
			data.dStrain[k]      =dStrain[k];
			data.dRotation[k]    =dRotation[k];
			data.dRotationU[k]   =dRotationU[k];
			data.dIO[k]=dIO[k];
			data.dLS[k]=dLS[k];
			data.dCP[k]=dCP[k];
		}

		data.bMultiCurve  = bMultiCurve; 
		data.nUseSubCount = nUseSubCount;
		data.nDeformType = nDeformType;
		data.bUseShapeCurve          = bUseShapeCurve         ;
		data.nShapeCurve             = nShapeCurve            ;
		data.bUseStrengthLoss        = bUseStrengthLoss       ;
		data.bStrengthLoss           = bStrengthLoss          ;
		data.nInfillStrutType        = nInfillStrutType       ;
		data.bUseTotalStrLossAtE     = bUseTotalStrLossAtE    ;
		data.bTotalStrLossAtE        = bTotalStrLossAtE       ;
		data.bUseUnloadStiffnessType = bUseUnloadStiffnessType;
		data.nUnloadStiffnessType    = nUnloadStiffnessType   ;
		data.dExpUnloadStiff_Exp     = dExpUnloadStiff_Exp    ;
		data.dExpUnloadStiff_PRFact  = dExpUnloadStiff_PRFact ;
		data.nInfillStiff            = nInfillStiff           ;
		data.dInfillStiff_KmiRop     = dInfillStiff_KmiRop    ;
		data.dInfillStiff_KmsRop     = dInfillStiff_KmsRop    ;
		data.dInfillStiff_Hm         = dInfillStiff_Hm        ;
		data.dInfillStiff_ThetaM     = dInfillStiff_ThetaM    ;

		data.nYieldAnalOption = nYieldAnalOption;
		data.nUnloadStiffOption = nUnloadStiffOption;
		data.enrgDissipation = enrgDissipation;
	}
	void ConvertFromData(const T_PHGT_FEMA& data)
	{
		nClass      = data.nClass;
		bSymmetric  = data.bSymmetric;
		bUserDefineRatio      =data.bUserDefineRatio;
		bUserDefineYieldDeform=data.bUserDefineYieldDeform;
		for(int k=0; k<2; ++k)
		{
			for(int i=0; i<9; ++i)
				dForceMomentRatio[k][i]=data.dForceMomentRatio[k][i];
			dYieldForce[k]  =data.dYieldForce[k]  ;
			dYieldMoment[k] =data.dYieldMoment[k] ;
			dDisplacement[k]=data.dDisplacement[k];
			dStrain[k]      =data.dStrain[k];
			dRotation[k]    =data.dRotation[k];
			dRotationU[k]   =data.dRotationU[k];
			dIO[k]=data.dIO[k];
			dLS[k]=data.dLS[k];
			dCP[k]=data.dCP[k];
		}

		bMultiCurve  = data.bMultiCurve; 
		nUseSubCount = data.nUseSubCount;
		nDeformType = data.nDeformType;
		bUseShapeCurve          = data.bUseShapeCurve         ;
		nShapeCurve             = data.nShapeCurve            ;
		bUseStrengthLoss        = data.bUseStrengthLoss       ;
		bStrengthLoss           = data.bStrengthLoss          ;
		nInfillStrutType        = data.nInfillStrutType       ;
		bUseTotalStrLossAtE     = data.bUseTotalStrLossAtE    ;
		bTotalStrLossAtE        = data.bTotalStrLossAtE       ;
		bUseUnloadStiffnessType = data.bUseUnloadStiffnessType;
		nUnloadStiffnessType    = data.nUnloadStiffnessType   ;
		dExpUnloadStiff_Exp     = data.dExpUnloadStiff_Exp    ;
		dExpUnloadStiff_PRFact  = data.dExpUnloadStiff_PRFact ;
		nInfillStiff            = data.nInfillStiff           ;
		dInfillStiff_KmiRop     = data.dInfillStiff_KmiRop    ;
		dInfillStiff_KmsRop     = data.dInfillStiff_KmsRop    ;
		dInfillStiff_Hm         = data.dInfillStiff_Hm        ;
		dInfillStiff_ThetaM     = data.dInfillStiff_ThetaM    ;
		nYieldAnalOption        = data.nYieldAnalOption       ;
		nUnloadStiffOption      = data.nUnloadStiffOption     ;
		enrgDissipation         = data.enrgDissipation		  ;
	}
	void Convert905(T_PHGT_FEMA_RW_905& src)
	{
		nClass      = src.nClass;
		bSymmetric  = src.bSymmetric;
		bUserDefineRatio      =src.bUserDefineRatio;
		bUserDefineYieldDeform=src.bUserDefineYieldDeform;
		for (int k=0; k<2; ++k)
		{
			for (int i=0; i<9; ++i)
				dForceMomentRatio[k][i]=src.dForceMomentRatio[k][i];
			dYieldForce[k]  =src.dYieldForce[k];
			dYieldMoment[k] =src.dYieldMoment[k];
			dDisplacement[k]=src.dDisplacement[k];
			dStrain[k]      =src.dStrain[k];
			dRotation[k]    =src.dRotation[k];
			dRotationU[k]   =src.dRotationU[k];
			dIO[k]=src.dIO[k];
			dLS[k]=src.dLS[k];
			dCP[k]=src.dCP[k];
		}

		bMultiCurve  = src.bMultiCurve;
		nUseSubCount = src.nUseSubCount;
		nDeformType = src.nDeformType;
		bUseShapeCurve          = src.bUseShapeCurve;
		nShapeCurve             = src.nShapeCurve;
		bUseStrengthLoss        = src.bUseStrengthLoss;
		bStrengthLoss           = src.bStrengthLoss;
		nInfillStrutType        = src.nInfillStrutType;
		bUseTotalStrLossAtE     = src.bUseTotalStrLossAtE;
		bTotalStrLossAtE        = src.bTotalStrLossAtE;
		bUseUnloadStiffnessType = src.bUseUnloadStiffnessType;
		nUnloadStiffnessType    = src.nUnloadStiffnessType;
		dExpUnloadStiff_Exp     = src.dExpUnloadStiff_Exp;
		dExpUnloadStiff_PRFact  = src.dExpUnloadStiff_PRFact;
		nInfillStiff            = src.nInfillStiff;
		dInfillStiff_KmiRop     = src.dInfillStiff_KmiRop;
		dInfillStiff_KmsRop     = src.dInfillStiff_KmsRop;
		dInfillStiff_Hm         = src.dInfillStiff_Hm;
		dInfillStiff_ThetaM     = src.dInfillStiff_ThetaM;
		nYieldAnalOption        = src.nYieldAnalOption;
		nUnloadStiffOption      = D_PHGT_FEMA_UNLD_OPTION_SELECT_HYST_MODEL;
		enrgDissipation.Initialize();
	}
	void Convert896(T_PHGT_FEMA_RW_896& src)
	{
		nClass      = src.nClass;
		bSymmetric  = src.bSymmetric;
		bUserDefineRatio      =src.bUserDefineRatio;
		bUserDefineYieldDeform=src.bUserDefineYieldDeform;
		for(int k=0; k<2; ++k)
		{
			for(int i=0; i<9; ++i)
				dForceMomentRatio[k][i]=src.dForceMomentRatio[k][i];
			dYieldForce[k]  =src.dYieldForce[k]  ;
			dYieldMoment[k] =src.dYieldMoment[k] ;
			dDisplacement[k]=src.dDisplacement[k];
			dStrain[k]      =src.dStrain[k];
			dRotation[k]    =src.dRotation[k];
			dRotationU[k]   =src.dRotationU[k];
			dIO[k]=src.dIO[k];
			dLS[k]=src.dLS[k];
			dCP[k]=src.dCP[k];
		}

		bMultiCurve  = src.bMultiCurve; 
		nUseSubCount = src.nUseSubCount;
		nDeformType = src.nDeformType;
		bUseShapeCurve          = src.bUseShapeCurve         ;
		nShapeCurve             = src.nShapeCurve            ;
		bUseStrengthLoss        = src.bUseStrengthLoss       ;
		bStrengthLoss           = src.bStrengthLoss          ;
		nInfillStrutType        = src.nInfillStrutType       ;
		bUseTotalStrLossAtE     = src.bUseTotalStrLossAtE    ;
		bTotalStrLossAtE        = src.bTotalStrLossAtE       ;
		bUseUnloadStiffnessType = src.bUseUnloadStiffnessType;
		nUnloadStiffnessType    = src.nUnloadStiffnessType   ;
		dExpUnloadStiff_Exp     = src.dExpUnloadStiff_Exp    ;
		dExpUnloadStiff_PRFact  = src.dExpUnloadStiff_PRFact ;
		nInfillStiff            = src.nInfillStiff           ;
		dInfillStiff_KmiRop     = src.dInfillStiff_KmiRop    ;
		dInfillStiff_KmsRop     = src.dInfillStiff_KmsRop    ;
		dInfillStiff_Hm         = src.dInfillStiff_Hm        ;
		dInfillStiff_ThetaM     = src.dInfillStiff_ThetaM    ;
		nYieldAnalOption        = D_PHGT_FEMA_YIELD_ANAL_OLD ;
		nUnloadStiffOption      = D_PHGT_FEMA_UNLD_OPTION_SELECT_HYST_MODEL;
		enrgDissipation.Initialize();
	}
	// 190122 jslee : 앞으로는 FEMA_RW 하위버전을 convert합시다용

	void Convert881(T_PHGT_FEMA_881& src)
	{
		nClass      = src.nClass;
		bSymmetric  = src.bSymmetric;
		bUserDefineRatio      =src.bUserDefineRatio;
		bUserDefineYieldDeform=src.bUserDefineYieldDeform;
		for(int k=0; k<2; ++k)
		{
			for(int i=0; i<9; ++i)
				dForceMomentRatio[k][i]=src.dForceMomentRatio[k][i];
			dYieldForce[k]  =src.dYieldForce[k]  ;
			dYieldMoment[k] =src.dYieldMoment[k] ;
			dDisplacement[k]=src.dDisplacement[k];
			dStrain[k]      =src.dStrain[k];
			dRotation[k]    =src.dRotation[k];
			dRotationU[k]   =src.dRotationU[k];
			dIO[k]=src.dIO[k];
			dLS[k]=src.dLS[k];
			dCP[k]=src.dCP[k];
		}

		bMultiCurve  = src.bMultiCurve; 
		nUseSubCount = src.nUseSubCount;
		nDeformType = src.nDeformType;
		bUseShapeCurve          = src.bUseShapeCurve         ;
		nShapeCurve             = src.nShapeCurve            ;
		bUseStrengthLoss        = src.bUseStrengthLoss       ;
		bStrengthLoss           = src.bStrengthLoss          ;
		nInfillStrutType        = src.nInfillStrutType       ;
		bUseTotalStrLossAtE     = src.bUseTotalStrLossAtE    ;
		bTotalStrLossAtE        = src.bTotalStrLossAtE       ;
		bUseUnloadStiffnessType = src.bUseUnloadStiffnessType;
		nUnloadStiffnessType    = src.nUnloadStiffnessType   ;
		// [GEN-359] 881버전에선 이 값이 잘못 저장되어 있었으므로 변환
		if(nUnloadStiffnessType==4) nUnloadStiffnessType=D_PHGT_FEMA_UNLD_STIFF_DEGRADE_SLIP;
	    else if(nUnloadStiffnessType==5) nUnloadStiffnessType=D_PHGT_FEMA_UNLD_NONLIN_ELASTIC;
		dExpUnloadStiff_Exp     = src.dExpUnloadStiff_Exp    ;
		dExpUnloadStiff_PRFact  = src.dExpUnloadStiff_PRFact ;
		nInfillStiff            = src.nInfillStiff           ;
		dInfillStiff_KmiRop     = src.dInfillStiff_KmiRop    ;
		dInfillStiff_KmsRop     = src.dInfillStiff_KmsRop    ;
		dInfillStiff_Hm         = src.dInfillStiff_Hm        ;
		dInfillStiff_ThetaM     = src.dInfillStiff_ThetaM    ;
		nYieldAnalOption        = D_PHGT_FEMA_YIELD_ANAL_OLD ;
		nUnloadStiffOption      = D_PHGT_FEMA_UNLD_OPTION_SELECT_HYST_MODEL;
		enrgDissipation.Initialize();
	}
	void Convert880(T_PHGT_FEMA_880& src)
	{
		nClass      = src.nClass;
		bSymmetric  = src.bSymmetric;
		bUserDefineRatio      =src.bUserDefineRatio;
		bUserDefineYieldDeform=src.bUserDefineYieldDeform;
		for(int k=0; k<2; ++k)
		{
			for(int i=0; i<9; ++i)
				dForceMomentRatio[k][i]=src.dForceMomentRatio[k][i];
			dYieldForce[k]  =src.dYieldForce[k]  ;
			dYieldMoment[k] =src.dYieldMoment[k] ;
			dDisplacement[k]=src.dDisplacement[k];
			dStrain[k]      =src.dStrain[k];
			dRotation[k]    =src.dRotation[k];
			dRotationU[k]   =src.dRotationU[k];
			dIO[k]=src.dIO[k];
			dLS[k]=src.dLS[k];
			dCP[k]=src.dCP[k];
		}

		bMultiCurve  = src.bMultiCurve; 
		nUseSubCount = src.nUseSubCount;
		nDeformType = src.nDeformType;
		bUseShapeCurve          = src.bUseShapeCurve         ;
		nShapeCurve             = src.nShapeCurve            ;
		bUseStrengthLoss        = src.bUseStrengthLoss       ;
		bStrengthLoss           = src.bStrengthLoss          ;
		nInfillStrutType        = src.nInfillStrutType       ;
		bUseTotalStrLossAtE     = src.bUseTotalStrLossAtE    ;
		bTotalStrLossAtE        = src.bTotalStrLossAtE       ;
		bUseUnloadStiffnessType = src.bUseUnloadStiffnessType;
		nUnloadStiffnessType    = src.nUnloadStiffnessType   ;
		dExpUnloadStiff_Exp     = src.dExpUnloadStiff_Exp    ;
		dExpUnloadStiff_PRFact  = 0.5;
		nInfillStiff            = src.nInfillStiff           ;
		dInfillStiff_KmiRop     = src.dInfillStiff_KmiRop    ;
		dInfillStiff_KmsRop     = src.dInfillStiff_KmsRop    ;
		dInfillStiff_Hm         = src.dInfillStiff_Hm        ;
		dInfillStiff_ThetaM     = src.dInfillStiff_ThetaM    ;
		nYieldAnalOption        = D_PHGT_FEMA_YIELD_ANAL_OLD ;
		nUnloadStiffOption      = D_PHGT_FEMA_UNLD_OPTION_SELECT_HYST_MODEL;
		enrgDissipation.Initialize();
	}
	void Convert875(T_PHGT_FEMA_875& src)
	{
		nClass      = src.nClass;
		bSymmetric  = src.bSymmetric;
		bUserDefineRatio      =src.bUserDefineRatio;
		bUserDefineYieldDeform=src.bUserDefineYieldDeform;
		for(int k=0; k<2; ++k)
		{
			for(int i=0; i<9; ++i)
				dForceMomentRatio[k][i]=src.dForceMomentRatio[k][i];
			dYieldForce[k]  =src.dYieldForce[k]  ;
			dYieldMoment[k] =src.dYieldMoment[k] ;
			dDisplacement[k]=src.dDisplacement[k];
			dStrain[k]      =src.dStrain[k];
			dRotation[k]    =src.dRotation[k];
			dRotationU[k]   =src.dRotationU[k];
			dIO[k]=src.dIO[k];
			dLS[k]=src.dLS[k];
			dCP[k]=src.dCP[k];
		}

		bMultiCurve  = src.bMultiCurve; 
		nUseSubCount = src.nUseSubCount;
		nDeformType = src.nDeformType;
		bUseShapeCurve          = TRUE;
		nShapeCurve             = D_PHGT_FEMA_SHAPE_GENERAL;
		bUseStrengthLoss        = TRUE;
		bStrengthLoss           = TRUE;
		nInfillStrutType        = 1;
		bUseTotalStrLossAtE     = TRUE;
		bTotalStrLossAtE        = TRUE;
		bUseUnloadStiffnessType = TRUE;
		nUnloadStiffnessType    = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
		dExpUnloadStiff_Exp     = 0.4;
		dExpUnloadStiff_PRFact  = 0.5;
		nInfillStiff            = 1;
		dInfillStiff_KmiRop     = 2000.0;
		dInfillStiff_KmsRop     = 500.0;
		dInfillStiff_Hm         = 1.0;
		dInfillStiff_ThetaM     = 1.0;
		nYieldAnalOption        = D_PHGT_FEMA_YIELD_ANAL_OLD;
		nUnloadStiffOption      = D_PHGT_FEMA_UNLD_OPTION_SELECT_HYST_MODEL;
		enrgDissipation.Initialize();
	}
	void Convert865(T_PHGT_FEMA_865& src)
	{
		nClass      = src.nClass;
		bSymmetric  = src.bSymmetric;
		bUserDefineRatio      =src.bUserDefineRatio;
		bUserDefineYieldDeform=src.bUserDefineYieldDeform;
		for(int k=0; k<2; ++k)
		{
			for(int i=0; i<9; ++i)
				dForceMomentRatio[k][i]=src.dForceMomentRatio[k][i];
			dYieldForce[k]  =src.dYieldForce[k]  ;
			dYieldMoment[k] =src.dYieldMoment[k] ;
			dDisplacement[k]=src.dDisplacement[k];
			dStrain[k]      =src.dStrain[k];
			dRotation[k]    =src.dRotation[k];
			dRotationU[k]   =src.dRotationU[k];
			dIO[k]=src.dIO[k];
			dLS[k]=src.dLS[k];
			dCP[k]=src.dCP[k];
		}

		bMultiCurve  = src.bMultiCurve; 
		nUseSubCount = src.nUseSubCount;
		nDeformType = 0;
		bUseShapeCurve          = TRUE;
		nShapeCurve             = D_PHGT_FEMA_SHAPE_GENERAL;
		bUseStrengthLoss        = TRUE;
		bStrengthLoss           = TRUE;
		nInfillStrutType        = 1;
		bUseTotalStrLossAtE     = TRUE;
		bTotalStrLossAtE        = TRUE;
		bUseUnloadStiffnessType = TRUE;
		nUnloadStiffnessType    = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
		dExpUnloadStiff_Exp     = 0.4;
		dExpUnloadStiff_PRFact  = 0.5;
		nInfillStiff            = 1;
		dInfillStiff_KmiRop     = 2000.0;
		dInfillStiff_KmsRop     = 500.0;
		dInfillStiff_Hm         = 1.0;
		dInfillStiff_ThetaM     = 1.0;
		nYieldAnalOption        = D_PHGT_FEMA_YIELD_ANAL_OLD;
		nUnloadStiffOption      = D_PHGT_FEMA_UNLD_OPTION_SELECT_HYST_MODEL;
		enrgDissipation.Initialize();
	}
	void Convert815(T_PHGT_FEMA_815& src)
	{
		bSymmetric = src.bSymmetric;
		bUserDefineRatio      =src.bUserDefineRatio;
		bUserDefineYieldDeform=src.bUserDefineYieldDeform;
		for(int k=0; k<2; ++k)
		{
			for(int i=0; i<9; ++i)
				dForceMomentRatio[k][i]=src.dForceMomentRatio[k][i];
			dYieldForce[k]  =src.dYieldForce[k]  ;
			dYieldMoment[k] =src.dYieldMoment[k] ;
			dDisplacement[k]=src.dDisplacement[k];
			dStrain[k]      =src.dStrain[k];
			dRotation[k]    =src.dRotation[k];
			dRotationU[k]   =src.dRotationU[k];
			dIO[k]=src.dIO[k];
			dLS[k]=src.dLS[k];
			dCP[k]=src.dCP[k];
		}
		nClass      = src.nClass;

		bMultiCurve = FALSE;
		nUseSubCount = 1;
		nDeformType = 0;
		bUseShapeCurve          = TRUE;
		nShapeCurve             = D_PHGT_FEMA_SHAPE_GENERAL;
		bUseStrengthLoss        = TRUE;
		bStrengthLoss           = TRUE;
		nInfillStrutType        = 1;
		bUseTotalStrLossAtE     = TRUE;
		bTotalStrLossAtE        = TRUE;
		bUseUnloadStiffnessType = TRUE;
		nUnloadStiffnessType    = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
		dExpUnloadStiff_Exp     = 0.4;
		dExpUnloadStiff_PRFact  = 0.5;
		nInfillStiff            = 1;
		dInfillStiff_KmiRop     = 2000.0;
		dInfillStiff_KmsRop     = 500.0;
		dInfillStiff_Hm         = 1.0;
		dInfillStiff_ThetaM     = 1.0;
		nYieldAnalOption        = D_PHGT_FEMA_YIELD_ANAL_OLD;
		nUnloadStiffOption      = D_PHGT_FEMA_UNLD_OPTION_SELECT_HYST_MODEL;
		enrgDissipation.Initialize();
	} 
	void Convert730(T_PHGT_FEMA_730& src)
	{
		bSymmetric = src.bSymmetric;
		bUserDefineRatio      =src.bUserDefineRatio;
		bUserDefineYieldDeform=src.bUserDefineYieldDeform;
		for(int k=0; k<2; ++k)
		{
			for(int i=0; i<9; ++i)
				dForceMomentRatio[k][i]=src.dForceMomentRatio[k][i];
			dYieldForce[k]  =src.dYieldForce[k]  ;
			dYieldMoment[k] =src.dYieldMoment[k] ;
			dDisplacement[k]=src.dDisplacement[k];
			dStrain[k]      =src.dStrain[k];
			dRotation[k]    =src.dRotation[k];
			dRotationU[k]   =0.;
			dIO[k]=src.dIO[k];
			dLS[k]=src.dLS[k];
			dCP[k]=src.dCP[k];
		}
		nClass = 1;

		bMultiCurve = FALSE;
		nUseSubCount = 1;
		nDeformType = 0;
		bUseShapeCurve          = TRUE;
		nShapeCurve             = D_PHGT_FEMA_SHAPE_GENERAL;
		bUseStrengthLoss        = TRUE;
		bStrengthLoss           = TRUE;
		nInfillStrutType        = 1;
		bUseTotalStrLossAtE     = TRUE;
		bTotalStrLossAtE        = TRUE;
		bUseUnloadStiffnessType = TRUE;
		nUnloadStiffnessType    = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
		dExpUnloadStiff_Exp     = 0.4;
		dExpUnloadStiff_PRFact  = 0.5;
		nInfillStiff            = 1;
		dInfillStiff_KmiRop     = 2000.0;
		dInfillStiff_KmsRop     = 500.0;
		dInfillStiff_Hm         = 1.0;
		dInfillStiff_ThetaM     = 1.0;
		nYieldAnalOption        = D_PHGT_FEMA_YIELD_ANAL_OLD;
		nUnloadStiffOption      = D_PHGT_FEMA_UNLD_OPTION_SELECT_HYST_MODEL;
		enrgDissipation.Initialize();
	}  
};

struct T_PHGT_MULT
{
	int    nInputType;      // 0=Strength-Ratio, 1=Strength-Yield Rot.  
	int    nPlusMnustype;   // Tension/compression type
													// nSkcvType!=D_SKCV_SLP_XXX: 0=Symmetric, 1=Asymmetric
													// nSkcvType==D_SKCV_SLP_XXX: 0=Both, 1=Tension, 2=Compression
													// nSkcvType==D_SKCV_COMP   : 0=Fixd, 1=Deformed, 2=Fixed & Yield, 3=Deformed & Yield
	int    nStiffReductRatioOption;   // 0=Global Ctrl  + AIJ CheckOff
																		// 1=User Defined + AIJ CheckOff
																		// 2=User Defined + AIJ CheckOn
																		// 3=Global Ctrl  + AIJ CheckOn
	double dStiffReductRatio[2][3];   // [+, -] [a1, a2, ay] // 저장시 주의할 것
	
	//GSJI_AIJ
	BOOL   bAutoSpan;
	double dSpanRatio;

	//=> (+) Tension (-) Comp
	// DOF:Fx,Fy,Fz
	double dForce[2][3];           // unit:tonf   [+, -] [crack, yield, ultimate] P1,P2,P3
	double dDisplacement[2][3];    // unit:m      T_PHGT_D>nDefinition==D_PHGT_DEFINITION_MTHETA_LUMP
	double dStrain[2][3];          // unit:No!    T_PHGT_D>nDefinition!=D_PHGT_DEFINITION_MTHETA_LUMP
																 //             [+, -] [D1, D2, D2]        
	// DOF:Mx,My,Mz
	double dMoment[2][3];          // unit:tonf.m [+, -] [crack, yield, ultimate] P1,P2,P3
	double dRotation[2][3];        // unit:rad    T_PHGT_D>nDefinition==D_PHGT_DEFINITION_MTHETA_LUMP
	double dCurvature[2][3];       // unit:rad/m  T_PHGT_D>nDefinition!=D_PHGT_DEFINITION_MTHETA_LUMP
																 //             [+, -] [D1, D2, D2]
	 
	// Only used when nSkcvType==D_SKCV_SLP_BI, D_SKCV_SLP_TRI
	double dInitGapDisplacement[2];// unit:m   [+, -]
	double dInitGapRotation[2];    // unit:rad [+, -]
	// --------------------------------------------------------------------------------------------------
	void Initialize()
	{
		nInputType   =0;
		nPlusMnustype=0;
		nStiffReductRatioOption =0;
		bAutoSpan  = TRUE;
		dSpanRatio = 3.5;
		for(int k=0; k<2; ++k)//[+, -]
		{
			dStiffReductRatio[k][0] = 0.5;
			dStiffReductRatio[k][1] = 0.1;
			dStiffReductRatio[k][2] = 0.667; // ay = a1/(1+(a1-1)P1/P2)

			dForce[k][0] = dMoment[k][0] = 0.5;
			dForce[k][1] = dMoment[k][1] = 1.0;
			dForce[k][2] = dMoment[k][2] = 1.0;
			
			dDisplacement[k][0] = dStrain[k][0]   =0.1;
			dDisplacement[k][1] = dStrain[k][1]   =0.2;
			dDisplacement[k][2] = dStrain[k][2]   =0.3;
			dCurvature[k][0]    = dRotation[k][0] =0.1;
			dCurvature[k][1]    = dRotation[k][1] =0.2;
			dCurvature[k][2]    = dRotation[k][2] =0.3;    

			dInitGapDisplacement[k]=0.0;
			dInitGapRotation[k]    =0.0;
		}
	}
	T_PHGT_MULT& operator=(T_PHGT_MULT& src)
	{
		nInputType              = src.nInputType;
		nPlusMnustype           = src.nPlusMnustype;
		nStiffReductRatioOption = src.nStiffReductRatioOption;
		bAutoSpan               = src.bAutoSpan;
		dSpanRatio              = src.dSpanRatio;
		for(int k=0; k<2; ++k)//[+, -]
		{
			for(int i=0; i<3; ++i)
			{
				dStiffReductRatio[k][i] = src.dStiffReductRatio[k][i];
				dForce[k][i]            = src.dForce[k][i] ;
				dMoment[k][i]           = src.dMoment[k][i];
				dDisplacement[k][i]     = src.dDisplacement[k][i];
				dStrain[k][i]           = src.dStrain[k][i];
				dCurvature[k][i]        = src.dCurvature[k][i]   ;
				dRotation[k][i]         = src.dRotation[k][i]    ;
			}
			dInitGapDisplacement[k]=src.dInitGapDisplacement[k];
			dInitGapRotation[k]    =src.dInitGapRotation[k]    ;
		}
		return *this;
	}
	BOOL operator==(const T_PHGT_MULT& src) const
	{
		if(nInputType              != src.nInputType) return FALSE;
		if(nPlusMnustype           != src.nPlusMnustype) return FALSE;
		if(nStiffReductRatioOption != src.nStiffReductRatioOption) return FALSE;
		if(bAutoSpan               != src.bAutoSpan) return FALSE;
		if(dSpanRatio              != src.dSpanRatio) return FALSE;
		for(int k=0; k<2; ++k)//[+, -]
		{
			for(int i=0; i<3; ++i)
			{
				if(dStiffReductRatio[k][i] != src.dStiffReductRatio[k][i]) return FALSE;
				if(dForce[k][i]            != src.dForce[k][i] ) return FALSE;
				if(dMoment[k][i]           != src.dMoment[k][i]) return FALSE;
				if(dDisplacement[k][i]     != src.dDisplacement[k][i]) return FALSE;
				if(dStrain[k][i]           != src.dStrain[k][i]) return FALSE;
				if(dCurvature[k][i]        != src.dCurvature[k][i]   ) return FALSE;
				if(dRotation[k][i]         != src.dRotation[k][i]    ) return FALSE;
			}
			if(dInitGapDisplacement[k]!=src.dInitGapDisplacement[k]) return FALSE;
			if(dInitGapRotation[k]    !=src.dInitGapRotation[k]    ) return FALSE;
		}
		return TRUE;
	}
};

#define T_PHGT_SKCV_K unsigned int
struct T_PHGT_SKCV_730      // Skeleton Curve Properties  // (before: T_HNGT_PROP)
{ 
	BOOL         nCalcMethod;     // D_PHGT_CALC_XXXXX                        
	BOOL         bSymmetricIJ;    // Symmetric for i-end/j-end (only used DOF:My,Mz && nLoc=I&J)

	T_PHGT_MULT  Multi;   // used when nSkcvType==D_SKCV_STD_BI,D_SKCV_STD_TRI,D_SKCV_SLP_BI,D_SKCV_SLP_TRI
	T_PHGT_FEMA_730  FEMA;    // Only used when nSkcvType==D_SKCV_FEMA

	// Initial Stiffness <= DOF, nDefinition
	int    nInitStiffMethod;          // 0=6EI/L, 1=3EI/L, 2=2EL/L, 3=User, 4=Elastic, 5=Skeleton(P1/D1)  
	// Only when used  nInitStiffMethod==3  
	double dInitStiffUserTheta[3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N    N.m    // Beam/Column, Wall
	double dInitStiffUserPhi  [3];  //[Fx, Fy/Fz, Mx~Mz]  N    N    N.m2   // Beam/Column, Wall  
	double dInitStiffUserSprg [3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N/m  N.m/rad : // Truss, General Link
};
struct T_PHGT_SKCV_740      // Skeleton Curve Properties  // (before: T_HNGT_PROP)
{ 
	BOOL         nCalcMethod;     // D_PHGT_CALC_XXXXX                        
	BOOL         bSymmetricIJ;    // Symmetric for i-end/j-end (only used DOF:My,Mz && nLoc=I&J)

	T_PHGT_MULT  Multi;   // used when nSkcvType==D_SKCV_STD_BI,D_SKCV_STD_TRI,D_SKCV_SLP_BI,D_SKCV_SLP_TRI
	T_PHGT_FEMA_815  FEMA;    // Only used when nSkcvType==D_SKCV_FEMA

	// Initial Stiffness <= DOF, nDefinition
	int    nInitStiffMethod;          // 0=6EI/L, 1=3EI/L, 2=2EL/L, 3=User, 4=Elastic, 5=Skeleton(P1/D1)  
	// Only when used  nInitStiffMethod==3  
	double dInitStiffUserTheta[3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N    N.m    // Beam/Column, Wall
	double dInitStiffUserPhi  [3];  //[Fx, Fy/Fz, Mx~Mz]  N    N    N.m2   // Beam/Column, Wall  
	double dInitStiffUserSprg [3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N/m  N.m/rad : // Truss, General Link
};

struct T_PHGT_SKCV_815      // Skeleton Curve Properties  // (before: T_HNGT_PROP)
{ 
	BOOL         nCalcMethod;     // D_PHGT_CALC_XXXXX                        
	BOOL         bSymmetricIJ;    // Symmetric for i-end/j-end (only used DOF:My,Mz && nLoc=I&J)
	
	T_PHGT_MULT  Multi;   // used when nSkcvType==D_SKCV_STD_BI,D_SKCV_STD_TRI,D_SKCV_SLP_BI,D_SKCV_SLP_TRI
	T_PHGT_FEMA_815  FEMA;    // Only used when nSkcvType==D_SKCV_FEMA
	
	// Initial Stiffness <= DOF, nDefinition
	int    nInitStiffMethod;          // 0=6EI/L, 1=3EI/L, 2=2EL/L, 3=User, 4=Elastic, 5=Skeleton(P1/D1)  
	// Only when used  nInitStiffMethod==3  
	double dInitStiffUserTheta[3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N    N.m    // Beam/Column, Wall
	double dInitStiffUserPhi  [3];  //[Fx, Fy/Fz, Mx~Mz]  N    N    N.m2   // Beam/Column, Wall  
	double dInitStiffUserSprg [3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N/m  N.m/rad : // Truss, General Link
	
	double dInitStiffUserTheta_m[3]; // 741 추가
	double dInitStiffUserPhi_m  [3];
	double dInitStiffUserSprg_m [3];
};

struct T_PHGT_SKCV_865      // Skeleton Curve Properties  // (before: T_HNGT_PROP)
{ 
	BOOL         nCalcMethod;     // D_PHGT_CALC_XXXXX                        
	BOOL         bSymmetricIJ;    // Symmetric for i-end/j-end (only used DOF:My,Mz && nLoc=I&J)

	T_PHGT_MULT  Multi;   // used when nSkcvType==D_SKCV_STD_BI,D_SKCV_STD_TRI,D_SKCV_SLP_BI,D_SKCV_SLP_TRI
	T_PHGT_FEMA_865  FEMA;    // Only used when nSkcvType==D_SKCV_FEMA

	// Initial Stiffness <= DOF, nDefinition
	int    nInitStiffMethod;          // 0=6EI/L, 1=3EI/L, 2=2EL/L, 3=User, 4=Elastic, 5=Skeleton(P1/D1)  
	// Only when used  nInitStiffMethod==3  
	double dInitStiffUserTheta[3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N    N.m    // Beam/Column, Wall
	double dInitStiffUserPhi  [3];  //[Fx, Fy/Fz, Mx~Mz]  N    N    N.m2   // Beam/Column, Wall  
	double dInitStiffUserSprg [3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N/m  N.m/rad : // Truss, General Link

	double dInitStiffUserTheta_m[3]; // 741 추가
	double dInitStiffUserPhi_m  [3];
	double dInitStiffUserSprg_m [3];
};

struct T_PHGT_SKCV_870_minor // 출시 전 추가됨(161026). 별도 mgt처리 등은 하지 않음
{ 
	BOOL         nCalcMethod;     // D_PHGT_CALC_XXXXX                        
	BOOL         bSymmetricIJ;    // Symmetric for i-end/j-end (only used DOF:My,Mz && nLoc=I&J)

	T_PHGT_MULT  Multi;   // used when nSkcvType==D_SKCV_STD_BI,D_SKCV_STD_TRI,D_SKCV_SLP_BI,D_SKCV_SLP_TRI
	T_PHGT_FEMA_875  FEMA;    // Only used when nSkcvType==D_SKCV_FEMA

	// Initial Stiffness <= DOF, nDefinition
	int    nInitStiffMethod;          // 0=6EI/L, 1=3EI/L, 2=2EL/L, 3=User, 4=Elastic, 5=Skeleton(P1/D1)  
	// Only when used  nInitStiffMethod==3  
	double dInitStiffUserTheta[3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N    N.m    // Beam/Column, Wall
	double dInitStiffUserPhi  [3];  //[Fx, Fy/Fz, Mx~Mz]  N    N    N.m2   // Beam/Column, Wall  
	double dInitStiffUserSprg [3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N/m  N.m/rad : // Truss, General Link

	double dInitStiffUserTheta_m[3]; // 741 추가
	double dInitStiffUserPhi_m  [3];
	double dInitStiffUserSprg_m [3];
};

struct T_PHGT_SKCV_875
{ 
	BOOL         nCalcMethod;     // D_PHGT_CALC_XXXXX                        
	BOOL         bSymmetricIJ;    // Symmetric for i-end/j-end (only used DOF:My,Mz && nLoc=I&J)

	T_PHGT_MULT  Multi;   // used when nSkcvType==D_SKCV_STD_BI,D_SKCV_STD_TRI,D_SKCV_SLP_BI,D_SKCV_SLP_TRI
	T_PHGT_FEMA_875  FEMA;    // Only used when nSkcvType==D_SKCV_FEMA,D_SKCV_FEMA_IFST

	// Initial Stiffness <= DOF, nDefinition
	int    nInitStiffMethod;          // 0=6EI/L, 1=3EI/L, 2=2EL/L, 3=User, 4=Elastic, 5=Skeleton(P1/D1)  
	// Only when used  nInitStiffMethod==3  
	double dInitStiffUserTheta[3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N    N.m    // Beam/Column, Wall
	double dInitStiffUserPhi  [3];  //[Fx, Fy/Fz, Mx~Mz]  N    N    N.m2   // Beam/Column, Wall  
	double dInitStiffUserSprg [3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N/m  N.m/rad : // Truss, General Link

	double dInitStiffUserTheta_m[3]; // 741 추가
	double dInitStiffUserPhi_m  [3];
	double dInitStiffUserSprg_m [3];
	BOOL bInitStiffAssymetric; // PO General Link CLT 초기강성 비대칭 대응
};
struct T_PHGT_SKCV_880
{ 
	BOOL         nCalcMethod;     // D_PHGT_CALC_XXXXX                        
	BOOL         bSymmetricIJ;    // Symmetric for i-end/j-end (only used DOF:My,Mz && nLoc=I&J)

	T_PHGT_MULT  Multi;   // used when nSkcvType==D_SKCV_STD_BI,D_SKCV_STD_TRI,D_SKCV_SLP_BI,D_SKCV_SLP_TRI
	T_PHGT_FEMA_880  FEMA;    // Only used when nSkcvType==D_SKCV_FEMA,D_SKCV_FEMA_IFST

	// Initial Stiffness <= DOF, nDefinition
	int    nInitStiffMethod;          // 0=6EI/L, 1=3EI/L, 2=2EL/L, 3=User, 4=Elastic, 5=Skeleton(P1/D1)  
	// Only when used  nInitStiffMethod==3  
	double dInitStiffUserTheta[3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N    N.m    // Beam/Column, Wall
	double dInitStiffUserPhi  [3];  //[Fx, Fy/Fz, Mx~Mz]  N    N    N.m2   // Beam/Column, Wall  
	double dInitStiffUserSprg [3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N/m  N.m/rad : // Truss, General Link

	double dInitStiffUserTheta_m[3]; // 741 추가
	double dInitStiffUserPhi_m  [3];
	double dInitStiffUserSprg_m [3];
	BOOL bInitStiffAssymetric; // PO General Link CLT 초기강성 비대칭 대응
};
struct T_PHGT_SKCV_881
{ 
	BOOL         nCalcMethod;     // D_PHGT_CALC_XXXXX                        
	BOOL         bSymmetricIJ;    // Symmetric for i-end/j-end (only used DOF:My,Mz && nLoc=I&J)

	T_PHGT_MULT  Multi;   // used when nSkcvType==D_SKCV_STD_BI,D_SKCV_STD_TRI,D_SKCV_SLP_BI,D_SKCV_SLP_TRI
	T_PHGT_FEMA_881  FEMA;    // Only used when nSkcvType==D_SKCV_FEMA,D_SKCV_FEMA_IFST

	// Initial Stiffness <= DOF, nDefinition
	int    nInitStiffMethod;          // 0=6EI/L, 1=3EI/L, 2=2EL/L, 3=User, 4=Elastic, 5=Skeleton(P1/D1)  
	// Only when used  nInitStiffMethod==3  
	double dInitStiffUserTheta[3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N    N.m    // Beam/Column, Wall
	double dInitStiffUserPhi  [3];  //[Fx, Fy/Fz, Mx~Mz]  N    N    N.m2   // Beam/Column, Wall  
	double dInitStiffUserSprg [3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N/m  N.m/rad : // Truss, General Link

	double dInitStiffUserTheta_m[3]; // 741 추가
	double dInitStiffUserPhi_m  [3];
	double dInitStiffUserSprg_m [3];
	BOOL bInitStiffAssymetric; // PO General Link CLT 초기강성 비대칭 대응
};

struct T_PHGT_SKCV      // Skeleton Curve Properties  // (before: T_HNGT_PROP)
{ 
	BOOL         nCalcMethod;     // D_PHGT_CALC_XXXXX                        
	BOOL         bSymmetricIJ;    // Symmetric for i-end/j-end (only used DOF:My,Mz && nLoc=I&J)

	T_PHGT_MULT  Multi;   // used when nSkcvType==D_SKCV_STD_BI,D_SKCV_STD_TRI,D_SKCV_SLP_BI,D_SKCV_SLP_TRI
	T_PHGT_FEMA  FEMA;    // Only used when nSkcvType==D_SKCV_FEMA,D_SKCV_FEMA_IFST

	// Initial Stiffness <= DOF, nDefinition
	int    nInitStiffMethod;          // 0=6EI/L, 1=3EI/L, 2=2EL/L, 3=User, 4=Elastic, 5=Skeleton(P1/D1)  
	// Only when used  nInitStiffMethod==3  
	double dInitStiffUserTheta[3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N    N.m    // Beam/Column, Wall
	double dInitStiffUserPhi  [3];  //[Fx, Fy/Fz, Mx~Mz]  N    N    N.m2   // Beam/Column, Wall  
	double dInitStiffUserSprg [3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N/m  N.m/rad : // Truss, General Link

	double dInitStiffUserTheta_m[3]; // 741 추가
	double dInitStiffUserPhi_m  [3];
	double dInitStiffUserSprg_m [3];
	BOOL bInitStiffAssymetric; // PO General Link CLT 초기강성 비대칭 대응
	int nConfineEffect;        // 0:Yes, 1:No

	void Initialize()
	{    
		nCalcMethod  =D_PHGT_CALC_AUTO;
		bSymmetricIJ =TRUE;
		Multi.Initialize();
		FEMA.Initialize();    
		nInitStiffMethod = 4;
		for(int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=0.;
			dInitStiffUserPhi[k]=0.;
			dInitStiffUserSprg[k]=0.;
			dInitStiffUserTheta_m[k]=0.;
			dInitStiffUserPhi_m[k]=0.;
			dInitStiffUserSprg_m[k]=0.;
		}
		bInitStiffAssymetric = FALSE;
		nConfineEffect = 0;
	}
	// 20071018 mylee - 콤보박스에서 EC8 선택시 - 해당 자유도의 Skcv 만 초기화해준다
	void InitializeEC8(int nDof, int nElem, int nMatl, int nClass=1, int nInitStiff=4)
	{
		nCalcMethod  =D_PHGT_CALC_AUTO;
		bSymmetricIJ =TRUE;
		Multi.Initialize();
		FEMA.InitializeEC8(nDof, nElem, nMatl, nClass);
		nInitStiffMethod = nInitStiff;
		for(int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=1.;
			dInitStiffUserPhi[k]=1.;
			dInitStiffUserSprg[k]=1.;
			dInitStiffUserTheta_m[k]=1.;
			dInitStiffUserPhi_m[k]=1.;
			dInitStiffUserSprg_m[k]=1.;
		}
		bInitStiffAssymetric = FALSE;
		nConfineEffect = 0;
	}
	// 20071207 mylee - 대화창 에서 Material, Type 라디오 버튼 클릭시 Masonry 타입으로 초기화
	void InitializeMasonry(int nDof, BOOL bExist=FALSE)
	{
		nCalcMethod  =D_PHGT_CALC_USER;
		bSymmetricIJ =TRUE;
		Multi.Initialize();
		FEMA.InitializeMasonry(nDof, bExist);
		nInitStiffMethod = 4;
		for(int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=0.;
			dInitStiffUserPhi[k]=0.;
			dInitStiffUserSprg[k]=0.;
			dInitStiffUserTheta_m[k]=0.;
			dInitStiffUserPhi_m[k]=0.;
			dInitStiffUserSprg_m[k]=0.;
		}
		bInitStiffAssymetric = FALSE;
		nConfineEffect = 0;
	}
	void Initialize_InfillStrut()
	{    
		nCalcMethod  =D_PHGT_CALC_USER;
		bSymmetricIJ =FALSE;
		Multi.Initialize();
		FEMA.InitializeRatio_Infill();
		nInitStiffMethod = 4;
		for(int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=0.;
			dInitStiffUserPhi[k]=0.;
			dInitStiffUserSprg[k]=0.;
			dInitStiffUserTheta_m[k]=0.;
			dInitStiffUserPhi_m[k]=0.;
			dInitStiffUserSprg_m[k]=0.;
		}
		bInitStiffAssymetric = FALSE;
		nConfineEffect = 0;
	}
	T_PHGT_SKCV& operator=(T_PHGT_SKCV& src)
	{    
		nCalcMethod  = src.nCalcMethod; 
		bSymmetricIJ = src.bSymmetricIJ; 
		Multi        = src.Multi;
		FEMA         = src.FEMA;
		nInitStiffMethod = src.nInitStiffMethod;
		for(int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=src.dInitStiffUserTheta[k];
			dInitStiffUserPhi[k]  =src.dInitStiffUserPhi[k];
			dInitStiffUserSprg[k] =src.dInitStiffUserSprg[k];
			dInitStiffUserTheta_m[k]=src.dInitStiffUserTheta_m[k];
			dInitStiffUserPhi_m[k]  =src.dInitStiffUserPhi_m[k];
			dInitStiffUserSprg_m[k] =src.dInitStiffUserSprg_m[k];
		}   
		bInitStiffAssymetric = src.bInitStiffAssymetric;
		nConfineEffect = src.nConfineEffect;

		return *this;
	}

	BOOL operator==(const T_PHGT_SKCV& src) const
	{    
		if(nCalcMethod  != src.nCalcMethod) return FALSE; 
		if(bSymmetricIJ != src.bSymmetricIJ) return FALSE; 
		if(!(Multi        == src.Multi)) return FALSE;
		if(!(FEMA         == src.FEMA)) return FALSE;
		if(nInitStiffMethod != src.nInitStiffMethod) return FALSE;
		for(int k=0; k<3; ++k)
		{
			if(dInitStiffUserTheta[k]!=src.dInitStiffUserTheta[k]) return FALSE;
			if(dInitStiffUserPhi[k]  !=src.dInitStiffUserPhi[k]) return FALSE;
			if(dInitStiffUserSprg[k] !=src.dInitStiffUserSprg[k]) return FALSE;
			if(dInitStiffUserTheta_m[k]!=src.dInitStiffUserTheta_m[k]) return FALSE;
			if(dInitStiffUserPhi_m[k]  !=src.dInitStiffUserPhi_m[k]) return FALSE;
			if(dInitStiffUserSprg_m[k] !=src.dInitStiffUserSprg_m[k]) return FALSE;
		}   
		if(bInitStiffAssymetric != src.bInitStiffAssymetric) return FALSE;
		if(nConfineEffect != src.nConfineEffect) return FALSE;

		return TRUE;
	}  
};

struct T_PHGT_SKCV_UNIT
{
	T_PHGT_SKCV_UNIT()
	{               
		dForce                = D_UNITSYS_BASE_FORCE;
		dMoment               = D_UNITSYS_BASE_MOMENT;    
		dDisplacement         = D_UNITSYS_BASE_LENGTH;
		dStrain               = D_UNITSYS_NONE;
		dRotation             = D_UNITSYS_BASE_RADIAN;
		dCurvature            = D_UNITSYS_BASE_RADIAN_L;  //rad/m
		dInitGapDisplacement  = D_UNITSYS_BASE_LENGTH;
		dInitGapRotation      = D_UNITSYS_BASE_RADIAN;
		dInitStiffUserTheta[0]= D_UNITSYS_BASE_UNITFORCE;
		dInitStiffUserTheta[1]= D_UNITSYS_BASE_FORCE     ;
		dInitStiffUserTheta[2]= D_UNITSYS_BASE_MOMENT;
		dInitStiffUserPhi[0]  = D_UNITSYS_BASE_FORCE;
		dInitStiffUserPhi[1]  = D_UNITSYS_BASE_FORCE;
		dInitStiffUserPhi[2]  = D_UNITSYS_BASE_DISTSTIFF;
		dInitStiffUserSprg[0] = D_UNITSYS_BASE_SPRING;
		dInitStiffUserSprg[1] = D_UNITSYS_BASE_SPRING;
		dInitStiffUserSprg[2] = D_UNITSYS_BASE_ROSPRING;
		dInfillStiff_KmiRop   = D_UNITSYS_BASE_UNITFORCE;
		dInfillStiff_KmsRop   = D_UNITSYS_BASE_UNITFORCE;
		dInfillStiff_Hm       = D_UNITSYS_BASE_LENGTH;
	}
	int dForce;
	int dMoment;
	int dDisplacement;  
	int dStrain;
	int dRotation;
	int dCurvature;
	int dInitGapDisplacement;
	int dInitGapRotation;
	int dInitStiffUserTheta[3];  //[ N/m  N    N.m     ]
	int dInitStiffUserPhi  [3];  //[ N    N    N.m2    ]
	int dInitStiffUserSprg[3];   //[ N/m  N/m  N.m/rad ]
	int dInfillStiff_KmiRop;
	int dInfillStiff_KmsRop;
	int dInfillStiff_Hm; 
	// Displacement, Strain, Rotation, Curvature
	// ---------------------------------------------------------------
	// |        |         Beam/Column, Wall            |Truss, G-Link |
	// ---------------------------------------------------------------
	// | DOF    |    M-theta      |      M-phi         |              |
	// ---------------------------------------------------------------
	// | Fx     |  m   (Displ)    |  none  (Strain)    |      m       |
	// | Fy,z   | none (Strain)   |  none  (Strain)    |      m       |
	// | Mx,y,z | rad  (Rotation) |  rad/m (Curvature) |     rad      |
	// ---------------------------------------------------------------
	// T_PHGT_D의 nDefinition == D_PHGT_DEFINITION_MTHETA_LUMP -> M-theta 
	// T_PHGT_D의 nDefinition == D_PHGT_DEFINITION_MPHI_LUMP   -> M-phi 
	// T_PHGT_D의 nDefinition == D_PHGT_DEFINITION_MPHI_DIST   -> M-phi
	// dInitStiffUserXXXX 에서 XXXX(Theta, Phi) 타입 중에 적절히 골라서 저장
	// Truss, Spring은 dInitStiffUserSprg에 저장
};

struct T_PHGT_SKCV_RW_896
{ 
	BOOL         nCalcMethod;     // D_PHGT_CALC_XXXXX                        
	BOOL         bSymmetricIJ;    // Symmetric for i-end/j-end (only used DOF:My,Mz && nLoc=I&J)

	T_PHGT_MULT  Multi;   // used when nSkcvType==D_SKCV_STD_BI,D_SKCV_STD_TRI,D_SKCV_SLP_BI,D_SKCV_SLP_TRI
	T_PHGT_FEMA_RW_896  FEMA;    // Only used when nSkcvType==D_SKCV_FEMA,D_SKCV_FEMA_IFST

	// Initial Stiffness <= DOF, nDefinition
	int    nInitStiffMethod;          // 0=6EI/L, 1=3EI/L, 2=2EL/L, 3=User, 4=Elastic, 5=Skeleton(P1/D1)  
	// Only when used  nInitStiffMethod==3  
	double dInitStiffUserTheta[3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N    N.m    // Beam/Column, Wall
	double dInitStiffUserPhi  [3];  //[Fx, Fy/Fz, Mx~Mz]  N    N    N.m2   // Beam/Column, Wall  
	double dInitStiffUserSprg [3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N/m  N.m/rad : // Truss, General Link

	double dInitStiffUserTheta_m[3]; // 741 추가
	double dInitStiffUserPhi_m  [3];
	double dInitStiffUserSprg_m [3];
	BOOL bInitStiffAssymetric; // PO General Link CLT 초기강성 비대칭 대응
};

struct T_PHGT_SKCV_RW_905
{
	BOOL         nCalcMethod;     // D_PHGT_CALC_XXXXX                        
	BOOL         bSymmetricIJ;    // Symmetric for i-end/j-end (only used DOF:My,Mz && nLoc=I&J)

	T_PHGT_MULT  Multi;   // used when nSkcvType==D_SKCV_STD_BI,D_SKCV_STD_TRI,D_SKCV_SLP_BI,D_SKCV_SLP_TRI
	T_PHGT_FEMA_RW_905  FEMA;    // Only used when nSkcvType==D_SKCV_FEMA,D_SKCV_FEMA_IFST

	// Initial Stiffness <= DOF, nDefinition
	int    nInitStiffMethod;          // 0=6EI/L, 1=3EI/L, 2=2EL/L, 3=User, 4=Elastic, 5=Skeleton(P1/D1)  
	// Only when used  nInitStiffMethod==3  
	double dInitStiffUserTheta[3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N    N.m    // Beam/Column, Wall
	double dInitStiffUserPhi[3];  //[Fx, Fy/Fz, Mx~Mz]  N    N    N.m2   // Beam/Column, Wall  
	double dInitStiffUserSprg[3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N/m  N.m/rad : // Truss, General Link

	double dInitStiffUserTheta_m[3]; // 741 추가
	double dInitStiffUserPhi_m[3];
	double dInitStiffUserSprg_m[3];
	BOOL bInitStiffAssymetric; // PO General Link CLT 초기강성 비대칭 대응
};

struct T_PHGT_SKCV_RW_955      // Skeleton Curve Properties  // (before: T_HNGT_PROP)
{
	BOOL         nCalcMethod;     // D_PHGT_CALC_XXXXX                        
	BOOL         bSymmetricIJ;    // Symmetric for i-end/j-end (only used DOF:My,Mz && nLoc=I&J)

	T_PHGT_MULT  Multi;   // used when nSkcvType==D_SKCV_STD_BI,D_SKCV_STD_TRI,D_SKCV_SLP_BI,D_SKCV_SLP_TRI
	T_PHGT_FEMA_RW  FEMA;    // Only used when nSkcvType==D_SKCV_FEMA,D_SKCV_FEMA_IFST

	// Initial Stiffness <= DOF, nDefinition
	int    nInitStiffMethod;          // 0=6EI/L, 1=3EI/L, 2=2EL/L, 3=User, 4=Elastic, 5=Skeleton(P1/D1)  
	// Only when used  nInitStiffMethod==3  
	double dInitStiffUserTheta[3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N    N.m    // Beam/Column, Wall
	double dInitStiffUserPhi[3];  //[Fx, Fy/Fz, Mx~Mz]  N    N    N.m2   // Beam/Column, Wall  
	double dInitStiffUserSprg[3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N/m  N.m/rad : // Truss, General Link

	double dInitStiffUserTheta_m[3]; // 741 추가
	double dInitStiffUserPhi_m[3];
	double dInitStiffUserSprg_m[3];
	BOOL bInitStiffAssymetric; // PO General Link CLT 초기강성 비대칭 대응
};

struct T_PHGT_SKCV_RW      // Skeleton Curve Properties  // (before: T_HNGT_PROP)
{ 
	BOOL         nCalcMethod;     // D_PHGT_CALC_XXXXX                        
	BOOL         bSymmetricIJ;    // Symmetric for i-end/j-end (only used DOF:My,Mz && nLoc=I&J)

	T_PHGT_MULT  Multi;   // used when nSkcvType==D_SKCV_STD_BI,D_SKCV_STD_TRI,D_SKCV_SLP_BI,D_SKCV_SLP_TRI
	T_PHGT_FEMA_RW  FEMA;    // Only used when nSkcvType==D_SKCV_FEMA,D_SKCV_FEMA_IFST

	// Initial Stiffness <= DOF, nDefinition
	int    nInitStiffMethod;          // 0=6EI/L, 1=3EI/L, 2=2EL/L, 3=User, 4=Elastic, 5=Skeleton(P1/D1)  
	// Only when used  nInitStiffMethod==3  
	double dInitStiffUserTheta[3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N    N.m    // Beam/Column, Wall
	double dInitStiffUserPhi  [3];  //[Fx, Fy/Fz, Mx~Mz]  N    N    N.m2   // Beam/Column, Wall  
	double dInitStiffUserSprg [3];  //[Fx, Fy/Fz, Mx~Mz]  N/m  N/m  N.m/rad : // Truss, General Link

	double dInitStiffUserTheta_m[3]; // 741 추가
	double dInitStiffUserPhi_m  [3];
	double dInitStiffUserSprg_m [3];
	BOOL bInitStiffAssymetric; // PO General Link CLT 초기강성 비대칭 대응
	int nConfineEffect;        // 0:Yes, 1:No

	void Convert730(T_PHGT_SKCV_730& src)
	{    
		nCalcMethod  = src.nCalcMethod; 
		bSymmetricIJ = src.bSymmetricIJ; 
		Multi        = src.Multi;
		FEMA.Convert730(src.FEMA);
		nInitStiffMethod = src.nInitStiffMethod;
		for(int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=src.dInitStiffUserTheta[k];
			dInitStiffUserPhi[k]  =src.dInitStiffUserPhi[k];
			dInitStiffUserSprg[k] =src.dInitStiffUserSprg[k];
			dInitStiffUserTheta_m[k]=src.dInitStiffUserTheta[k]; // 일단 같은것으로 채움
			dInitStiffUserPhi_m[k]  =src.dInitStiffUserPhi[k];
			dInitStiffUserSprg_m[k] =src.dInitStiffUserSprg[k];
		}    
		bInitStiffAssymetric = FALSE;
		nConfineEffect = 0;
	}
	void Convert740(T_PHGT_SKCV_740& src)
	{    
		nCalcMethod  = src.nCalcMethod; 
		bSymmetricIJ = src.bSymmetricIJ; 
		Multi        = src.Multi;
		FEMA.Convert815(src.FEMA);
		nInitStiffMethod = src.nInitStiffMethod;
		for(int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=src.dInitStiffUserTheta[k];
			dInitStiffUserPhi[k]  =src.dInitStiffUserPhi[k];
			dInitStiffUserSprg[k] =src.dInitStiffUserSprg[k];
			dInitStiffUserTheta_m[k]=src.dInitStiffUserTheta[k]; // 일단 같은것으로 채움
			dInitStiffUserPhi_m[k]  =src.dInitStiffUserPhi[k];
			dInitStiffUserSprg_m[k] =src.dInitStiffUserSprg[k];
		}    
		bInitStiffAssymetric = FALSE;
		nConfineEffect = 0;
	}
	void Convert815(T_PHGT_SKCV_815& src)
	{    
		nCalcMethod  = src.nCalcMethod; 
		bSymmetricIJ = src.bSymmetricIJ; 
		Multi        = src.Multi;
		FEMA.Convert815(src.FEMA);
		nInitStiffMethod = src.nInitStiffMethod;
		for(int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=src.dInitStiffUserTheta[k];
			dInitStiffUserPhi[k]  =src.dInitStiffUserPhi[k];
			dInitStiffUserSprg[k] =src.dInitStiffUserSprg[k];
			dInitStiffUserTheta_m[k]=src.dInitStiffUserTheta_m[k];
			dInitStiffUserPhi_m[k]  =src.dInitStiffUserPhi_m[k];
			dInitStiffUserSprg_m[k] =src.dInitStiffUserSprg_m[k];
		}     
		bInitStiffAssymetric = FALSE;
		nConfineEffect = 0;
	}
	void Convert865(T_PHGT_SKCV_865& src)
	{    
		nCalcMethod  = src.nCalcMethod; 
		bSymmetricIJ = src.bSymmetricIJ; 
		Multi        = src.Multi;
		FEMA.Convert865(src.FEMA);
		nInitStiffMethod = src.nInitStiffMethod;
		for(int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=src.dInitStiffUserTheta[k];
			dInitStiffUserPhi[k]  =src.dInitStiffUserPhi[k];
			dInitStiffUserSprg[k] =src.dInitStiffUserSprg[k];
			dInitStiffUserTheta_m[k]=src.dInitStiffUserTheta_m[k];
			dInitStiffUserPhi_m[k]  =src.dInitStiffUserPhi_m[k];
			dInitStiffUserSprg_m[k] =src.dInitStiffUserSprg_m[k];
		}     
		bInitStiffAssymetric = FALSE;
		nConfineEffect = 0;
	}
	void Convert870_minor(T_PHGT_SKCV_870_minor& src)
	{    
		nCalcMethod  = src.nCalcMethod; 
		bSymmetricIJ = src.bSymmetricIJ; 
		Multi        = src.Multi;
		FEMA.Convert875(src.FEMA);
		nInitStiffMethod = src.nInitStiffMethod;
		for(int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=src.dInitStiffUserTheta[k];
			dInitStiffUserPhi[k]  =src.dInitStiffUserPhi[k];
			dInitStiffUserSprg[k] =src.dInitStiffUserSprg[k];
			dInitStiffUserTheta_m[k]=src.dInitStiffUserTheta_m[k];
			dInitStiffUserPhi_m[k]  =src.dInitStiffUserPhi_m[k];
			dInitStiffUserSprg_m[k] =src.dInitStiffUserSprg_m[k];
		}     
		bInitStiffAssymetric = FALSE;
		nConfineEffect = 0;
	}
	void Convert875(T_PHGT_SKCV_875& src)
	{    
		nCalcMethod  = src.nCalcMethod; 
		bSymmetricIJ = src.bSymmetricIJ; 
		Multi        = src.Multi;
		FEMA.Convert875(src.FEMA);
		nInitStiffMethod = src.nInitStiffMethod;
		for(int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=src.dInitStiffUserTheta[k];
			dInitStiffUserPhi[k]  =src.dInitStiffUserPhi[k];
			dInitStiffUserSprg[k] =src.dInitStiffUserSprg[k];
			dInitStiffUserTheta_m[k]=src.dInitStiffUserTheta_m[k];
			dInitStiffUserPhi_m[k]  =src.dInitStiffUserPhi_m[k];
			dInitStiffUserSprg_m[k] =src.dInitStiffUserSprg_m[k];
		}     
		bInitStiffAssymetric = src.bInitStiffAssymetric;
		nConfineEffect = 0;
	}
	void Convert880(T_PHGT_SKCV_880& src)
	{    
		nCalcMethod  = src.nCalcMethod; 
		bSymmetricIJ = src.bSymmetricIJ; 
		Multi        = src.Multi;
		FEMA.Convert880(src.FEMA);
		nInitStiffMethod = src.nInitStiffMethod;
		for(int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=src.dInitStiffUserTheta[k];
			dInitStiffUserPhi[k]  =src.dInitStiffUserPhi[k];
			dInitStiffUserSprg[k] =src.dInitStiffUserSprg[k];
			dInitStiffUserTheta_m[k]=src.dInitStiffUserTheta_m[k];
			dInitStiffUserPhi_m[k]  =src.dInitStiffUserPhi_m[k];
			dInitStiffUserSprg_m[k] =src.dInitStiffUserSprg_m[k];
		}     
		bInitStiffAssymetric = src.bInitStiffAssymetric;
		nConfineEffect = 0;
	}
	void Convert881(T_PHGT_SKCV_881& src)
	{    
		nCalcMethod  = src.nCalcMethod; 
		bSymmetricIJ = src.bSymmetricIJ; 
		Multi        = src.Multi;
		FEMA.Convert881(src.FEMA);
		nInitStiffMethod = src.nInitStiffMethod;
		for(int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=src.dInitStiffUserTheta[k];
			dInitStiffUserPhi[k]  =src.dInitStiffUserPhi[k];
			dInitStiffUserSprg[k] =src.dInitStiffUserSprg[k];
			dInitStiffUserTheta_m[k]=src.dInitStiffUserTheta_m[k];
			dInitStiffUserPhi_m[k]  =src.dInitStiffUserPhi_m[k];
			dInitStiffUserSprg_m[k] =src.dInitStiffUserSprg_m[k];
		}     
		bInitStiffAssymetric = src.bInitStiffAssymetric;
		nConfineEffect = 0;
	}
	void Convert896(T_PHGT_SKCV_RW_896& src)
	{
		nCalcMethod  = src.nCalcMethod; 
		bSymmetricIJ = src.bSymmetricIJ; 
		Multi        = src.Multi;
		FEMA.Convert896(src.FEMA);
		nInitStiffMethod = src.nInitStiffMethod;
		for(int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=src.dInitStiffUserTheta[k];
			dInitStiffUserPhi[k]  =src.dInitStiffUserPhi[k];
			dInitStiffUserSprg[k] =src.dInitStiffUserSprg[k];
			dInitStiffUserTheta_m[k]=src.dInitStiffUserTheta_m[k];
			dInitStiffUserPhi_m[k]  =src.dInitStiffUserPhi_m[k];
			dInitStiffUserSprg_m[k] =src.dInitStiffUserSprg_m[k];
		}     
		bInitStiffAssymetric = src.bInitStiffAssymetric;
		nConfineEffect = 0;
	}
	void Convert905(T_PHGT_SKCV_RW_905& src)
	{
		nCalcMethod  = src.nCalcMethod;
		bSymmetricIJ = src.bSymmetricIJ;
		Multi        = src.Multi;
		FEMA.Convert905(src.FEMA);
		nInitStiffMethod = src.nInitStiffMethod;
		for (int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=src.dInitStiffUserTheta[k];
			dInitStiffUserPhi[k]  =src.dInitStiffUserPhi[k];
			dInitStiffUserSprg[k] =src.dInitStiffUserSprg[k];
			dInitStiffUserTheta_m[k]=src.dInitStiffUserTheta_m[k];
			dInitStiffUserPhi_m[k]  =src.dInitStiffUserPhi_m[k];
			dInitStiffUserSprg_m[k] =src.dInitStiffUserSprg_m[k];
		}
		bInitStiffAssymetric = src.bInitStiffAssymetric;
		nConfineEffect = 0;
	}
	void Convert955(T_PHGT_SKCV_RW_955& src)
	{
		nCalcMethod = src.nCalcMethod;
		bSymmetricIJ = src.bSymmetricIJ;
		Multi = src.Multi;
		FEMA = src.FEMA;
		nInitStiffMethod = src.nInitStiffMethod;
		for (int k = 0; k < 3; ++k)
		{
			dInitStiffUserTheta[k] = src.dInitStiffUserTheta[k];
			dInitStiffUserPhi[k] = src.dInitStiffUserPhi[k];
			dInitStiffUserSprg[k] = src.dInitStiffUserSprg[k];
			dInitStiffUserTheta_m[k] = src.dInitStiffUserTheta_m[k];
			dInitStiffUserPhi_m[k] = src.dInitStiffUserPhi_m[k];
			dInitStiffUserSprg_m[k] = src.dInitStiffUserSprg_m[k];
		}
		bInitStiffAssymetric = src.bInitStiffAssymetric;
		nConfineEffect = 0;
	}
	void ConvertToData(T_PHGT_SKCV& src)
	{    
		src.nCalcMethod  = nCalcMethod; 
		src.bSymmetricIJ = bSymmetricIJ; 
		src.Multi        = Multi;
		FEMA.ConvertToData(src.FEMA);
		src.nInitStiffMethod = nInitStiffMethod;
		for(int k=0; k<3; ++k)
		{
			src.dInitStiffUserTheta[k]  = dInitStiffUserTheta[k];
			src.dInitStiffUserPhi[k]    = dInitStiffUserPhi[k];
			src.dInitStiffUserSprg[k]   = dInitStiffUserSprg[k];
			src.dInitStiffUserTheta_m[k]= dInitStiffUserTheta_m[k];
			src.dInitStiffUserPhi_m[k]  = dInitStiffUserPhi_m[k];
			src.dInitStiffUserSprg_m[k] = dInitStiffUserSprg_m[k];
		}     
		src.bInitStiffAssymetric = bInitStiffAssymetric;
		src.nConfineEffect = nConfineEffect;
	}
	void ConvertFromData(T_PHGT_SKCV& src)
	{    
		nCalcMethod  = src.nCalcMethod; 
		bSymmetricIJ = src.bSymmetricIJ; 
		Multi        = src.Multi;
		FEMA.ConvertFromData(src.FEMA);
		nInitStiffMethod = src.nInitStiffMethod;
		for(int k=0; k<3; ++k)
		{
			dInitStiffUserTheta[k]=src.dInitStiffUserTheta[k];
			dInitStiffUserPhi[k]  =src.dInitStiffUserPhi[k];
			dInitStiffUserSprg[k] =src.dInitStiffUserSprg[k];
			dInitStiffUserTheta_m[k]=src.dInitStiffUserTheta_m[k];
			dInitStiffUserPhi_m[k]  =src.dInitStiffUserPhi_m[k];
			dInitStiffUserSprg_m[k] =src.dInitStiffUserSprg_m[k];
		}     
		bInitStiffAssymetric = src.bInitStiffAssymetric;
		nConfineEffect = src.nConfineEffect;
	}
};

#define T_PHGT_K unsigned int

#define D_PHGT_DEFINITION_MTHETA_LUMP 0  // M-Theta(Rotation) Lumped
#define D_PHGT_DEFINITION_MPHI_LUMP   1  // M-Phi(Curvature)  Lumped
#define D_PHGT_DEFINITION_MPHI_DIST   2  // M-Phi(Curvature)  Distributed

// nMaterial !=2 && nDefinition !=D_PHGT_DEFINITION_MPHI_DIST 일 경우 Hinge Location
#define D_PHGT_LOCATION_CENTER  0   // center
#define D_PHGT_LOCATION_I       1   // I-end
#define D_PHGT_LOCATION_J       2   // J-end
#define D_PHGT_LOCATION_IJ      3   // I-end & J-end
#define D_PHGT_LOCATION_NONE    100

#define D_PHGT_DISTRIBUTION_ENTIRETY  0   // Entirety
#define D_PHGT_DISTRIBUTION_I       1   // I-end
#define D_PHGT_DISTRIBUTION_J       2   // J-end
#define D_PHGT_DISTRIBUTION_IJ      3   // I-end & J-end
//nDefinition ==D_PHGT_DEFINITION_MPHI_DIST 일 경우 nLoc[] 은 1~20 까지 허용됨.(갯수로 입력)

enum D_PHGT_INTERCTION { D_PHGT_INTERCTION_NONE, D_PHGT_INTERCTION_PMM, D_PHGT_INTERCTION_PM };
enum D_PHGT_ELEM { D_PHGT_ELEM_BEAM, D_PHGT_ELEM_WALL, D_PHGT_ELEM_TRUSS, D_PHGT_ELEM_GLINK, D_PHGT_ELEM_NSPR };
enum D_PHGT_MATL { D_PHGT_MATL_CON, D_PHGT_MATL_STL, D_PHGT_MATL_MAS }; // 0=RC/SRC(encased), 1=Steel/SRC(filled), 2=Masonry
struct T_PHGT_D     // (before: T_HNGT_D)
{
	int     nSeq;
	CString strName;
	CString strDescription;
	int     nElemType;        // 0=Beam-Col, 1=Wall, 2=Truss, 3=GeneralLink(Spring), 4=Point Spring Support ->PHGE와 다르므로 주의
	int     nWallType;        // 0=Membrane, 1=Plate
	int     nMaterial;        // 0=RC/SRC(encased), 1=Steel/SRC(filled), 2=Masonry
	int     nDefinition;      // nMaterial !=2 : D_PHGT_DEFINITION_XXXXX
														// nMaterial ==2 : 0=Pier, 1=Spandrel
														// nElemType==2or3 일 경우 반드시 D_PHGT_DEFINITION_MTHETA_LUMP 로 저장할 것
														// 단위변환때문임. 
	int     nInteractionType; // Axial-Moment Interaction Type. 0=None, 1=N-M-M(축력변동), 2=N-M
 
	BOOL    bConsiderHingeLength; // nDefinition이 D_PHGT_DEFINITION_MPHI_LUMP일때만 값이 의미가 있음.. // MNET:XXXX-JWKWON-20130611 : v820부터
	int     nIntegrationPoint;    // 0=End, 1=Center

	BOOL    bDOF[6];          // [자유도] 해당 자유도의 힌지 존재여부 TRUE==Use, FALSE==DoNotUse
	int     nLoc[6];          // 힌지 위치 또는 갯수
														// nDefinition==D_PHGT_DEFINITION_MTHETA_LUMP || D_PHGT_DEFINITION_MPHI_LUMP
														// -> D_PHGT_LOCATION_XXXX 참고
														// nDefinition==D_PHGT_DEFINITION_MPHI_DIST
														//           ->  갯수(1~20)저장
	int     nDistribution[6]; // v800추가 nDefinition==D_PHGT_DEFINITION_MPHI_DIST
														// 0 : Entirety,    1 : I,    2: J,   3: I & J
	int     nSkcvType[6];     // D_SKCV_XXXXX   해당 자유도의 힌지 모델종류
	double  dLengthRatio_I[6];  // v800추가 Hinge Length Ratio(Lp/L),    0.0 < Lp/L < 1.0
	double  dLengthRatio_J[6];  // 

	double  dHingeLength_I[2];	// v831 추가 // MNET:XXXX-HUKIM-20140318(My, Mz에만 사용 : Moment - Rotation 일때만..)
	double  dHingeLength_J[2];  // dHingeLength_I , dHingeLength_J는 Ele Type : Beam/Column , Wall Mat Type : RC / SRC(encased) , Steel/SRC(filled) Definition : Moment Rotation Skeleton Curve : Eurocode 8 일때 활성화.
	BOOL bRelativeLength; //디폴트 체크.
	int     nHingeType        ; // v865 추가. 0=Skeleton, 1=Fiber
	UINT    PropKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_SECT_K, nElemType==D_PHGT_ELEM_WALL => T_THIK_K
	UINT    FibrKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_FIBR_K, nElemType==D_IEHP_HTYP_WALL => T_FIBW_K
	int     nFiberAutoUser    ; // 0:Auto Generation, 1:User Defined
	int     nUsePogdShearR    ; // 0:Use option of POGD, 1:User Defined
	double  dShearR           ; // Shear Spring Location : r
	int     nUsePogdConsOutpl ; // 0:Use option of POGD, 1:User Defined
	BOOL    bConsOutpl        ; // Out-of-plane Nonlinearity of Fiber Wall
	int     nASInteractionType; // Axial-Shear Interaction Type. 0=None, 1=P-Q Interaction

	T_PHGT_SKCV  Skcv[6]   ;     // I only, J only, Center, 개수입력할 경우 여기 저장
	T_PHGT_SKCV  SkcvSub[6];     // I&J가 필요할 경우 J단의 값에 대한 값 저장 [4]와 [5] 사용
															 // CAttrCtrl::NeedIJproperties(T_PHGT_K Key, BOOL bNeedBothEnd[6])로 판단가능
	T_PHGT_YLDS  YieldSurface;   // nInteractionType==1 일 경우 저장
	T_PHGT_MSNR  Masonry;

	BOOL IsSectionalLocation()
	{
		return (nDefinition == D_PHGT_DEFINITION_MPHI_DIST);
	}
	void Initialize()
	{
		nSeq =0;
		strName.Empty();
		strDescription.Empty();
		nElemType        = 0;
		nWallType        = 0;
		nMaterial        = 0;
		nDefinition      = D_PHGT_DEFINITION_MTHETA_LUMP;
		nInteractionType = D_PHGT_INTERCTION_NONE;
		bConsiderHingeLength=FALSE; 
		nIntegrationPoint=0;    

		for(int i=0; i<6; ++i)
		{
			bDOF[i] = FALSE;
			//nDefinition에 따라 아래와 같이 정의
			//if(i<4) nLoc[i] = D_PHGT_LOCATION_CENTER;
			//else    nLoc[i] = D_PHGT_LOCATION_IJ;      
			nLoc[i] = D_PHGT_LOCATION_IJ; // MQC:XXXX mylee 20110117
			nDistribution[i] = D_PHGT_DISTRIBUTION_ENTIRETY;
			dLengthRatio_I[i] = 0.1;
			dLengthRatio_J[i] = 0.1;

			nSkcvType[i]    = D_SKCV_STD_TRI;
			Skcv[i].Initialize();
			SkcvSub[i].Initialize();
		}
		for (int j=0; j<2; j++)
		{
			dHingeLength_I[j] = 0.5;
			dHingeLength_J[j] = 0.5;
		}

		bRelativeLength = TRUE;
		nHingeType        = 0;
		PropKey           = 0;
		FibrKey           = 0;
		nFiberAutoUser    = 1;
		nUsePogdShearR    = 0;
		dShearR           = 0.4;
		nUsePogdConsOutpl = 0;
		bConsOutpl        = FALSE;
		nASInteractionType = 0;

		YieldSurface.Initialize();
		Masonry.Initialize();
	}
	T_PHGT_D() {}
	T_PHGT_D& operator=(T_PHGT_D& src)
	{
		nSeq            = src.nSeq            ;
		strName         = src.strName         ;
		strDescription  = src.strDescription  ;
		nElemType       = src.nElemType       ;
		nWallType       = src.nWallType       ;
		nMaterial       = src.nMaterial       ;
		nDefinition     = src.nDefinition     ;                
		nInteractionType= src.nInteractionType;
		bConsiderHingeLength = src.bConsiderHingeLength;
		nIntegrationPoint    = src.nIntegrationPoint;
		for(int k=0; k<6; ++k)
		{
			bDOF[k]= src.bDOF[k];
			nLoc[k]= src.nLoc[k];   
			nDistribution[k] = src.nDistribution[k];
			dLengthRatio_I[k] = src.dLengthRatio_I[k];
			dLengthRatio_J[k] = src.dLengthRatio_J[k];
			nSkcvType[k] = src.nSkcvType[k];
			Skcv[k]      = src.Skcv[k];
			SkcvSub[k]   = src.SkcvSub[k];
		}

		for (int j=0; j<2; j++)
		{
			dHingeLength_I[j] = src.dHingeLength_I[j];
			dHingeLength_J[j] = src.dHingeLength_J[j];
		}
		
		bRelativeLength = src.bRelativeLength;
		nHingeType        = src.nHingeType       ;
		PropKey           = src.PropKey          ;
		FibrKey           = src.FibrKey          ;
		nFiberAutoUser    = src.nFiberAutoUser   ;
		nUsePogdShearR    = src.nUsePogdShearR   ;
		dShearR           = src.dShearR          ;
		nUsePogdConsOutpl = src.nUsePogdConsOutpl;
		bConsOutpl        = src.bConsOutpl       ;
		nASInteractionType = src.nASInteractionType;

		YieldSurface = src.YieldSurface;
		Masonry      = src.Masonry;
		return *this;
	}
	BOOL IsSameData(T_PHGT_D& src)
	{
		if(nElemType != src.nElemType) return FALSE;
		if(nWallType != src.nWallType) return FALSE;
		if(nMaterial != src.nMaterial) return FALSE;
		if(nDefinition != src.nDefinition) return FALSE;
		if(nInteractionType != src.nInteractionType) return FALSE;
		if(bConsiderHingeLength != src.bConsiderHingeLength) return FALSE;
		if(nIntegrationPoint != src.nIntegrationPoint) return FALSE;
		for(int i=0; i<6; i++)
		{
			if(bDOF[i] != src.bDOF[i]) return FALSE;
			if(nLoc[i] != src.nLoc[i]) return FALSE;
			if(nDistribution[i] != src.nDistribution[i]) return FALSE;
			if(nSkcvType[i] != src.nSkcvType[i]) return FALSE;
			if(dLengthRatio_I[i] != src.dLengthRatio_I[i]) return FALSE;
			if(dLengthRatio_J[i] != src.dLengthRatio_J[i]) return FALSE;   

			if(!(Skcv[i] == src.Skcv[i])) return FALSE;   
			if(!(SkcvSub[i] == src.SkcvSub[i])) return FALSE;   
		}  
		
		for (int j=0; j<2; j++)
		{
			if(dHingeLength_I[j] != src.dHingeLength_I[j]) return FALSE;
			if(dHingeLength_J[j] != src.dHingeLength_J[j]) return FALSE;
		}
		
		if (bRelativeLength != src.bRelativeLength) return FALSE;
		if (nHingeType        != src.nHingeType       ) return FALSE;
		if (PropKey           != src.PropKey          ) return FALSE;
		if (FibrKey           != src.FibrKey          ) return FALSE;
		if (nFiberAutoUser    != src.nFiberAutoUser   ) return FALSE;
		if (nUsePogdShearR    != src.nUsePogdShearR   ) return FALSE;
		if (dShearR           != src.dShearR          ) return FALSE;
		if (nUsePogdConsOutpl != src.nUsePogdConsOutpl) return FALSE;
		if (bConsOutpl        != src.bConsOutpl       ) return FALSE;
		if (nASInteractionType != src.nASInteractionType) return FALSE;

		if(YieldSurface != src.YieldSurface) return FALSE;
		if(Masonry != src.Masonry) return FALSE;   
		return TRUE;
	}
	BOOL DeformTypeEnabled(int nDof)
	{
		if(nElemType==D_PHGT_ELEM_BEAM)
		{
			if(nDof!=4 && nDof!=5) return FALSE; // My,Mz
		}
		else if(nElemType==D_PHGT_ELEM_WALL)
		{
			if(nDof!=1 && nDof!=2 && nDof!=4 && nDof!=5) return FALSE; // Fy,Fz,My,Mz
		}
		else
		{
			return FALSE;
		}

		if(nDefinition!=D_PHGT_DEFINITION_MTHETA_LUMP) return FALSE; // M-Theta
		if(nMaterial!=0) return FALSE; // RC
		if(nSkcvType[nDof]!=D_SKCV_FEMA && nSkcvType[nDof]!=D_SKCV_FEMA_IFST) return FALSE; // FEMA

		return TRUE;
	}
};

struct T_PHGT_D_CH_881
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;   
	int  nWallType;
	int  nMaterial;       
	int  nDefinition;      
	int  nInteractionType;
	BOOL    bConsiderHingeLength; 
	int     nIntegrationPoint;    
	BOOL bDOF[6];
	int  nLoc[6];
	int  nDistribution[6];
	int  nSkcvType[6];
	double dLengthRatio_I[6];
	double dLengthRatio_J[6];
	double  dHingeLength_I[2];	
	double  dHingeLength_J[2];
	BOOL bRelativeLength;
	int     nHingeType        ; // v865 추가. 0=Skeleton, 1=Fiber
	UINT    PropKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_SECT_K, nElemType==D_PHGT_ELEM_WALL => T_THIK_K
	UINT    FibrKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_FIBR_K, nElemType==D_IEHP_HTYP_WALL => T_FIBW_K
	int     nFiberAutoUser    ; // 0:Auto Generation, 1:User Defined
	int     nUsePogdShearR    ; // 0:Use option of POGD, 1:User Defined
	double  dShearR           ; // Shear Spring Location : r
	int     nUsePogdConsOutpl ; // 0:Use option of POGD, 1:User Defined
	BOOL    bConsOutpl        ; // Out-of-plane Nonlinearity of Fiber Wall
	int     nASInteractionType; // Axial-Shear Interaction Type. 0=None, 1=P-Q Interaction

	T_PHGT_SKCV_881  Skcv[6];
	T_PHGT_SKCV_881  SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR_885  Masonry;
};
struct T_PHGT_D_CH_880
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;   
	int  nWallType;
	int  nMaterial;       
	int  nDefinition;      
	int  nInteractionType;
	BOOL    bConsiderHingeLength; 
	int     nIntegrationPoint;    
	BOOL bDOF[6];
	int  nLoc[6];
	int  nDistribution[6];
	int  nSkcvType[6];
	double dLengthRatio_I[6];
	double dLengthRatio_J[6];
	double  dHingeLength_I[2];	
	double  dHingeLength_J[2];
	BOOL bRelativeLength;
	int     nHingeType        ; // v865 추가. 0=Skeleton, 1=Fiber
	UINT    PropKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_SECT_K, nElemType==D_PHGT_ELEM_WALL => T_THIK_K
	UINT    FibrKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_FIBR_K, nElemType==D_IEHP_HTYP_WALL => T_FIBW_K
	int     nFiberAutoUser    ; // 0:Auto Generation, 1:User Defined
	int     nUsePogdShearR    ; // 0:Use option of POGD, 1:User Defined
	double  dShearR           ; // Shear Spring Location : r
	int     nUsePogdConsOutpl ; // 0:Use option of POGD, 1:User Defined
	BOOL    bConsOutpl        ; // Out-of-plane Nonlinearity of Fiber Wall
	int     nASInteractionType; // Axial-Shear Interaction Type. 0=None, 1=P-Q Interaction

	T_PHGT_SKCV_880  Skcv[6];
	T_PHGT_SKCV_880  SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR_885  Masonry;
};

struct T_PHGT_D_CH_875
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;   
	int  nWallType;
	int  nMaterial;       
	int  nDefinition;      
	int  nInteractionType;
	BOOL    bConsiderHingeLength; 
	int     nIntegrationPoint;    
	BOOL bDOF[6];
	int  nLoc[6];
	int  nDistribution[6];
	int  nSkcvType[6];
	double dLengthRatio_I[6];
	double dLengthRatio_J[6];
	double  dHingeLength_I[2];	
	double  dHingeLength_J[2];
	BOOL bRelativeLength;
	int     nHingeType        ; // v865 추가. 0=Skeleton, 1=Fiber
	UINT    PropKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_SECT_K, nElemType==D_PHGT_ELEM_WALL => T_THIK_K
	UINT    FibrKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_FIBR_K, nElemType==D_IEHP_HTYP_WALL => T_FIBW_K
	int     nFiberAutoUser    ; // 0:Auto Generation, 1:User Defined
	int     nUsePogdShearR    ; // 0:Use option of POGD, 1:User Defined
	double  dShearR           ; // Shear Spring Location : r
	int     nUsePogdConsOutpl ; // 0:Use option of POGD, 1:User Defined
	BOOL    bConsOutpl        ; // Out-of-plane Nonlinearity of Fiber Wall
	int     nASInteractionType; // Axial-Shear Interaction Type. 0=None, 1=P-Q Interaction

	T_PHGT_SKCV_875  Skcv[6];
	T_PHGT_SKCV_875  SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR_885  Masonry;
};

struct T_PHGT_D_CH_865_v3
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;   
	int  nWallType;
	int  nMaterial;       
	int  nDefinition;      
	int  nInteractionType;
	BOOL    bConsiderHingeLength; 
	int     nIntegrationPoint;    
	BOOL bDOF[6];
	int  nLoc[6];
	int  nDistribution[6];
	int  nSkcvType[6];
	double dLengthRatio_I[6];
	double dLengthRatio_J[6];
	double  dHingeLength_I[2];	
	double  dHingeLength_J[2];
	BOOL bRelativeLength;
	int     nHingeType        ; // v865 추가. 0=Skeleton, 1=Fiber
	UINT    PropKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_SECT_K, nElemType==D_PHGT_ELEM_WALL => T_THIK_K
	UINT    FibrKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_FIBR_K, nElemType==D_IEHP_HTYP_WALL => T_FIBW_K
	int     nFiberAutoUser    ; // 0:Auto Generation, 1:User Defined
	int     nUsePogdShearR    ; // 0:Use option of POGD, 1:User Defined
	double  dShearR           ; // Shear Spring Location : r
	int     nUsePogdConsOutpl ; // 0:Use option of POGD, 1:User Defined
	BOOL    bConsOutpl        ; // Out-of-plane Nonlinearity of Fiber Wall
	int     nASInteractionType; // Axial-Shear Interaction Type. 0=None, 1=P-Q Interaction
	T_PHGT_SKCV_870_minor  Skcv[6];
	T_PHGT_SKCV_870_minor  SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR_885  Masonry;
};

// ASInteractionType 추가. 기존 만들어놓은 모델들이 많을 것 같아서 호환처리함
struct T_PHGT_D_CH_865_v2
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;   
	int  nWallType;
	int  nMaterial;       
	int  nDefinition;      
	int  nInteractionType;
	BOOL    bConsiderHingeLength; 
	int     nIntegrationPoint;    
	BOOL bDOF[6];
	int  nLoc[6];
	int  nDistribution[6];
	int  nSkcvType[6];
	double dLengthRatio_I[6];
	double dLengthRatio_J[6];
	double  dHingeLength_I[2];	
	double  dHingeLength_J[2];
	BOOL bRelativeLength;
	int     nHingeType        ; // v865 추가. 0=Skeleton, 1=Fiber
	UINT    PropKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_SECT_K, nElemType==D_PHGT_ELEM_WALL => T_THIK_K
	UINT    FibrKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_FIBR_K, nElemType==D_IEHP_HTYP_WALL => T_FIBW_K
	int     nFiberAutoUser    ; // 0:Auto Generation, 1:User Defined
	int     nUsePogdShearR    ; // 0:Use option of POGD, 1:User Defined
	double  dShearR           ; // Shear Spring Location : r
	int     nUsePogdConsOutpl ; // 0:Use option of POGD, 1:User Defined
	BOOL    bConsOutpl        ; // Out-of-plane Nonlinearity of Fiber Wall
	T_PHGT_SKCV_870_minor  Skcv[6];
	T_PHGT_SKCV_870_minor  SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR_885  Masonry;
};

struct T_PHGT_D_CH_865
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;   
	int  nWallType;
	int  nMaterial;       
	int  nDefinition;      
	int  nInteractionType;
	BOOL    bConsiderHingeLength; 
	int     nIntegrationPoint;    
	BOOL bDOF[6];
	int  nLoc[6];
	int  nDistribution[6];
	int  nSkcvType[6];
	double dLengthRatio_I[6];
	double dLengthRatio_J[6];
	double  dHingeLength_I[2];	
	double  dHingeLength_J[2];
	BOOL bRelativeLength;
	int     nHingeType        ; // v865 추가. 0=Skeleton, 1=Fiber
	UINT    PropKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_SECT_K, nElemType==D_PHGT_ELEM_WALL => T_THIK_K
	UINT    FibrKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_FIBR_K, nElemType==D_IEHP_HTYP_WALL => T_FIBW_K
	int     nFiberAutoUser    ; // 0:Auto Generation, 1:User Defined
	int     nUsePogdShearR    ; // 0:Use option of POGD, 1:User Defined
	double  dShearR           ; // Shear Spring Location : r
	int     nUsePogdConsOutpl ; // 0:Use option of POGD, 1:User Defined
	BOOL    bConsOutpl        ; // Out-of-plane Nonlinearity of Fiber Wall
	T_PHGT_SKCV_865  Skcv[6];
	T_PHGT_SKCV_865  SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR_885  Masonry;
};

struct T_PHGT_D_CH_861
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;   
	int  nWallType;
	int  nMaterial;       
	int  nDefinition;      
	int  nInteractionType;
	BOOL    bConsiderHingeLength; 
	int     nIntegrationPoint;    
	BOOL bDOF[6];
	int  nLoc[6];
	int  nDistribution[6];
	int  nSkcvType[6];
	double dLengthRatio_I[6];
	double dLengthRatio_J[6];
	double  dHingeLength_I[2];	
	double  dHingeLength_J[2];
	BOOL bRelativeLength;
	T_PHGT_SKCV_865  Skcv[6];
	T_PHGT_SKCV_865  SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR_885  Masonry;
};

struct T_PHGT_D_CH_830
{
	int  nSeq;
	char strName[80];
	char strDescription[80];

	int  nElemType;   
	int  nWallType;
	int  nMaterial;       
	int  nDefinition;      
	int  nInteractionType;
	BOOL bConsiderHingeLength; // nDefinition이 D_PHGT_DEFINITION_MPHI_LUMP일때만 값이 의미가 있음.. // MNET:XXXX-JWKWON-20130611 : v820부터
	int  nIntegrationPoint;    // 0=End, 1=Center

	BOOL bDOF[6];
	int  nLoc[6];     
	int  nDistribution[6];
	int  nSkcvType[6];
	double  dLengthRatio_I[6];  
	double  dLengthRatio_J[6];  
	T_PHGT_SKCV_865  Skcv[6];
	T_PHGT_SKCV_865  SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR_885  Masonry;
};

struct T_PHGT_D_CH_815
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;   
	int  nWallType;
	int  nMaterial;       
	int  nDefinition;      
	int  nInteractionType;
	BOOL bDOF[6];
	int  nLoc[6];     
	int  nDistribution[6];
	int  nSkcvType[6];
	double  dLengthRatio_I[6];  
	double  dLengthRatio_J[6];  
	T_PHGT_SKCV_815  Skcv[6];
	T_PHGT_SKCV_815  SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR_885  Masonry;
};

struct T_PHGT_D_CH_796
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;   
	int  nWallType;
	int  nMaterial;       
	int  nDefinition;      
	int  nInteractionType;
	BOOL bDOF[6];
	int  nLoc[6];                           
	int  nSkcvType[6];
	T_PHGT_SKCV_815  Skcv[6];
	T_PHGT_SKCV_815  SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR_885  Masonry;
};
struct T_PHGT_D_CH_740
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;   
	int  nWallType;
	int  nMaterial;       
	int  nDefinition;      
	int  nInteractionType;
	BOOL bDOF[6];
	int  nLoc[6];                           
	int  nSkcvType[6];
	T_PHGT_SKCV_740  Skcv[6];
	T_PHGT_SKCV_740  SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR_885  Masonry;
};
struct T_PHGT_D_CH_730
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;   
	int  nWallType;
	int  nMaterial;       
	int  nDefinition;      
	int  nInteractionType;
	BOOL bDOF[6];
	int  nLoc[6];                           
	int  nSkcvType[6];
	T_PHGT_SKCV_730  Skcv[6];
	T_PHGT_SKCV_730  SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR_885  Masonry;
};
struct T_PHGT_D_RW_885
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;   
	int  nWallType;
	int  nMaterial;       
	int  nDefinition;      
	int  nInteractionType;
	BOOL    bConsiderHingeLength; 
	int     nIntegrationPoint;    
	BOOL bDOF[6];
	int  nLoc[6];
	int  nDistribution[6];
	int  nSkcvType[6];
	double dLengthRatio_I[6];
	double dLengthRatio_J[6];
	double  dHingeLength_I[2];	
	double  dHingeLength_J[2];
	BOOL bRelativeLength;
	int     nHingeType        ; // v865 추가. 0=Skeleton, 1=Fiber
	UINT    PropKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_SECT_K, nElemType==D_PHGT_ELEM_WALL => T_THIK_K
	UINT    FibrKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_FIBR_K, nElemType==D_IEHP_HTYP_WALL => T_FIBW_K
	int     nFiberAutoUser    ; // 0:Auto Generation, 1:User Defined
	int     nUsePogdShearR    ; // 0:Use option of POGD, 1:User Defined
	double  dShearR           ; // Shear Spring Location : r
	int     nUsePogdConsOutpl ; // 0:Use option of POGD, 1:User Defined
	BOOL    bConsOutpl        ; // Out-of-plane Nonlinearity of Fiber Wall
	int     nASInteractionType; // Axial-Shear Interaction Type. 0=None, 1=P-Q Interaction

	T_PHGT_SKCV_RW_896 Skcv[6];
	T_PHGT_SKCV_RW_896 SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR_885  Masonry;
};
struct T_PHGT_D_RW_896
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;   
	int  nWallType;
	int  nMaterial;       
	int  nDefinition;      
	int  nInteractionType;
	BOOL    bConsiderHingeLength; 
	int     nIntegrationPoint;    
	BOOL bDOF[6];
	int  nLoc[6];
	int  nDistribution[6];
	int  nSkcvType[6];
	double dLengthRatio_I[6];
	double dLengthRatio_J[6];
	double  dHingeLength_I[2];	
	double  dHingeLength_J[2];
	BOOL bRelativeLength;
	int     nHingeType        ; // v865 추가. 0=Skeleton, 1=Fiber
	UINT    PropKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_SECT_K, nElemType==D_PHGT_ELEM_WALL => T_THIK_K
	UINT    FibrKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_FIBR_K, nElemType==D_IEHP_HTYP_WALL => T_FIBW_K
	int     nFiberAutoUser    ; // 0:Auto Generation, 1:User Defined
	int     nUsePogdShearR    ; // 0:Use option of POGD, 1:User Defined
	double  dShearR           ; // Shear Spring Location : r
	int     nUsePogdConsOutpl ; // 0:Use option of POGD, 1:User Defined
	BOOL    bConsOutpl        ; // Out-of-plane Nonlinearity of Fiber Wall
	int     nASInteractionType; // Axial-Shear Interaction Type. 0=None, 1=P-Q Interaction

	T_PHGT_SKCV_RW_896 Skcv[6];
	T_PHGT_SKCV_RW_896 SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR  Masonry;
};
struct T_PHGT_D_RW_905
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;
	int  nWallType;
	int  nMaterial;
	int  nDefinition;
	int  nInteractionType;
	BOOL    bConsiderHingeLength;
	int     nIntegrationPoint;
	BOOL bDOF[6];
	int  nLoc[6];
	int  nDistribution[6];
	int  nSkcvType[6];
	double dLengthRatio_I[6];
	double dLengthRatio_J[6];
	double  dHingeLength_I[2];
	double  dHingeLength_J[2];
	BOOL bRelativeLength;
	int     nHingeType; // v865 추가. 0=Skeleton, 1=Fiber
	UINT    PropKey; // nElemType==D_PHGT_ELEM_BEAM => T_SECT_K, nElemType==D_PHGT_ELEM_WALL => T_THIK_K
	UINT    FibrKey; // nElemType==D_PHGT_ELEM_BEAM => T_FIBR_K, nElemType==D_IEHP_HTYP_WALL => T_FIBW_K
	int     nFiberAutoUser; // 0:Auto Generation, 1:User Defined
	int     nUsePogdShearR; // 0:Use option of POGD, 1:User Defined
	double  dShearR; // Shear Spring Location : r
	int     nUsePogdConsOutpl; // 0:Use option of POGD, 1:User Defined
	BOOL    bConsOutpl; // Out-of-plane Nonlinearity of Fiber Wall
	int     nASInteractionType; // Axial-Shear Interaction Type. 0=None, 1=P-Q Interaction

	T_PHGT_SKCV_RW_905 Skcv[6];
	T_PHGT_SKCV_RW_905 SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR  Masonry;
};
struct T_PHGT_D_RW_955
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;
	int  nWallType;
	int  nMaterial;
	int  nDefinition;
	int  nInteractionType;
	BOOL    bConsiderHingeLength;
	int     nIntegrationPoint;
	BOOL bDOF[6];
	int  nLoc[6];
	int  nDistribution[6];
	int  nSkcvType[6];
	double dLengthRatio_I[6];
	double dLengthRatio_J[6];
	double  dHingeLength_I[2];
	double  dHingeLength_J[2];
	BOOL bRelativeLength;
	int     nHingeType; // v865 추가. 0=Skeleton, 1=Fiber
	UINT    PropKey; // nElemType==D_PHGT_ELEM_BEAM => T_SECT_K, nElemType==D_PHGT_ELEM_WALL => T_THIK_K
	UINT    FibrKey; // nElemType==D_PHGT_ELEM_BEAM => T_FIBR_K, nElemType==D_IEHP_HTYP_WALL => T_FIBW_K
	int     nFiberAutoUser; // 0:Auto Generation, 1:User Defined
	int     nUsePogdShearR; // 0:Use option of POGD, 1:User Defined
	double  dShearR; // Shear Spring Location : r
	int     nUsePogdConsOutpl; // 0:Use option of POGD, 1:User Defined
	BOOL    bConsOutpl; // Out-of-plane Nonlinearity of Fiber Wall
	int     nASInteractionType; // Axial-Shear Interaction Type. 0=None, 1=P-Q Interaction

	T_PHGT_SKCV_RW_955 Skcv[6];
	T_PHGT_SKCV_RW_955 SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR  Masonry;
};
struct T_PHGT_D_RW
{
	int  nSeq;
	char strName[80];
	char strDescription[80];
	int  nElemType;   
	int  nWallType;
	int  nMaterial;       
	int  nDefinition;      
	int  nInteractionType;
	BOOL    bConsiderHingeLength; 
	int     nIntegrationPoint;    
	BOOL bDOF[6];
	int  nLoc[6];
	int  nDistribution[6];
	int  nSkcvType[6];
	double dLengthRatio_I[6];
	double dLengthRatio_J[6];
	double  dHingeLength_I[2];	
	double  dHingeLength_J[2];
	BOOL bRelativeLength;
	int     nHingeType        ; // v865 추가. 0=Skeleton, 1=Fiber
	UINT    PropKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_SECT_K, nElemType==D_PHGT_ELEM_WALL => T_THIK_K
	UINT    FibrKey           ; // nElemType==D_PHGT_ELEM_BEAM => T_FIBR_K, nElemType==D_IEHP_HTYP_WALL => T_FIBW_K
	int     nFiberAutoUser    ; // 0:Auto Generation, 1:User Defined
	int     nUsePogdShearR    ; // 0:Use option of POGD, 1:User Defined
	double  dShearR           ; // Shear Spring Location : r
	int     nUsePogdConsOutpl ; // 0:Use option of POGD, 1:User Defined
	BOOL    bConsOutpl        ; // Out-of-plane Nonlinearity of Fiber Wall
	int     nASInteractionType; // Axial-Shear Interaction Type. 0=None, 1=P-Q Interaction

	T_PHGT_SKCV_RW Skcv[6];
	T_PHGT_SKCV_RW SkcvSub[6];
	T_PHGT_YLDS  YieldSurface;
	T_PHGT_MSNR  Masonry;

	void Convert730(T_PHGT_D_CH_730& rData)
	{
		nSeq             = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType       ;       
		nWallType        = rData.nWallType       ;
		nMaterial        = rData.nMaterial       ;      
		nDefinition      = rData.nDefinition     ;     
		nInteractionType = rData.nInteractionType;
		bConsiderHingeLength=FALSE;
		nIntegrationPoint=0; 
		for(int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert730(rData.Skcv[i]);
			SkcvSub[i].Convert730(rData.SkcvSub[i]);
			nDistribution[i] = D_PHGT_DISTRIBUTION_ENTIRETY;
			dLengthRatio_I[i] = 0.1;
			dLengthRatio_J[i] = 0.1;
		}  
		for(int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = 0.5;
			dHingeLength_J[j] = 0.5;
		}
		bRelativeLength = TRUE;
		nHingeType        = 0;
		PropKey           = 0;
		FibrKey           = 0;
		nFiberAutoUser    = 1;
		nUsePogdShearR    = 0;
		dShearR           = 0.4;
		nUsePogdConsOutpl = 0;
		bConsOutpl        = FALSE;
		nASInteractionType = 0;
		YieldSurface = rData.YieldSurface;
		Masonry.Convert885(rData.Masonry);
	}
	void Convert740(T_PHGT_D_CH_740& rData)
	{
		nSeq             = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType       ;       
		nWallType        = rData.nWallType       ;
		nMaterial        = rData.nMaterial       ;      
		nDefinition      = rData.nDefinition     ;     
		nInteractionType = rData.nInteractionType;
		bConsiderHingeLength=FALSE;
		nIntegrationPoint=0; 
		for(int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert740(rData.Skcv[i]);
			SkcvSub[i].Convert740(rData.SkcvSub[i]);
			nDistribution[i] = D_PHGT_DISTRIBUTION_ENTIRETY;
			dLengthRatio_I[i] = 0.1;
			dLengthRatio_J[i] = 0.1;
		}   
		for(int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = 0.5;
			dHingeLength_J[j] = 0.5;
		}
		bRelativeLength = TRUE;
		nHingeType        = 0;
		PropKey           = 0;
		FibrKey           = 0;
		nFiberAutoUser    = 1;
		nUsePogdShearR    = 0;
		dShearR           = 0.4;
		nUsePogdConsOutpl = 0;
		bConsOutpl        = FALSE;
		nASInteractionType = 0;

		YieldSurface = rData.YieldSurface;
		Masonry.Convert885(rData.Masonry);
	}
	void Convert796(T_PHGT_D_CH_796& rData)
	{
		nSeq             = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType       ;       
		nWallType        = rData.nWallType       ;
		nMaterial        = rData.nMaterial       ;      
		nDefinition      = rData.nDefinition     ;     
		nInteractionType = rData.nInteractionType;
		bConsiderHingeLength=FALSE;
		nIntegrationPoint=0; 
		for(int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert815(rData.Skcv[i]);
			SkcvSub[i].Convert815(rData.SkcvSub[i]);
			nDistribution[i] = D_PHGT_DISTRIBUTION_ENTIRETY;
			dLengthRatio_I[i] = 0.1;
			dLengthRatio_J[i] = 0.1;
		}    
		for(int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = 0.5;
			dHingeLength_J[j] = 0.5;
		}
		bRelativeLength = TRUE;
		nHingeType        = 0;
		PropKey           = 0;
		FibrKey           = 0;
		nFiberAutoUser    = 1;
		nUsePogdShearR    = 0;
		dShearR           = 0.4;
		nUsePogdConsOutpl = 0;
		bConsOutpl        = FALSE;
		nASInteractionType = 0;

		YieldSurface = rData.YieldSurface;
		Masonry.Convert885(rData.Masonry);
	}
	void Convert815(T_PHGT_D_CH_815& rData)
	{
		nSeq             = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType       ;       
		nWallType        = rData.nWallType       ;
		nMaterial        = rData.nMaterial       ;      
		nDefinition      = rData.nDefinition     ;     
		nInteractionType = rData.nInteractionType;

		BOOL bConvert=FALSE;
		for(int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert815(rData.Skcv[i]);
			SkcvSub[i].Convert815(rData.SkcvSub[i]);
			nDistribution[i] = rData.nDistribution[i];
			dLengthRatio_I[i] = rData.dLengthRatio_I[i];
			dLengthRatio_J[i] = rData.dLengthRatio_J[i];
			if(nDistribution[i] != D_PHGT_DISTRIBUTION_ENTIRETY) bConvert = TRUE;
		}   

		for(int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = 0.5;
			dHingeLength_J[j] = 0.5;
		}

		bRelativeLength = TRUE;
		nHingeType        = 0;
		PropKey           = 0;
		FibrKey           = 0;
		nFiberAutoUser    = 1;
		nUsePogdShearR    = 0;
		dShearR           = 0.4;
		nUsePogdConsOutpl = 0;
		bConsOutpl        = FALSE;
		nASInteractionType = 0;

		if(nDefinition == D_PHGT_DEFINITION_MPHI_DIST && bConvert)
		{
			nDefinition = D_PHGT_DEFINITION_MPHI_LUMP;
			bConsiderHingeLength = TRUE;

			for(int j=0; j<6; ++j)
			{
				if(nDistribution[j]==D_PHGT_DISTRIBUTION_ENTIRETY)
				{
					dLengthRatio_I[j]=0.5;
					dLengthRatio_J[j]=0.5;
					nLoc[j] = D_PHGT_DISTRIBUTION_IJ; 
				}
				else 
				{
					nLoc[j] = nDistribution[j];
					nDistribution[j] = D_PHGT_DISTRIBUTION_ENTIRETY;
				}
			}
		}
		else
		{
			bConsiderHingeLength=FALSE; 
			for(int j=0; j<6; ++j)
			{
				if(nDistribution[j]!=D_PHGT_DISTRIBUTION_ENTIRETY) 
				{
					nDistribution[j] = D_PHGT_DISTRIBUTION_ENTIRETY;
					dLengthRatio_I[j] = 0.1;
					dLengthRatio_J[j] = 0.1;
				}
			}
		}
		nIntegrationPoint=0;    
		YieldSurface = rData.YieldSurface;
		Masonry.Convert885(rData.Masonry);
	}

	void Convert830(T_PHGT_D_CH_830& rData)
	{
		nSeq             = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType       ;       
		nWallType        = rData.nWallType       ;
		nMaterial        = rData.nMaterial       ;      
		nDefinition      = rData.nDefinition     ;     
		nInteractionType = rData.nInteractionType;

		BOOL bConvert=FALSE;
		for(int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert865(rData.Skcv[i]);
			SkcvSub[i].Convert865(rData.SkcvSub[i]);
			nDistribution[i]  = rData.nDistribution[i];
			dLengthRatio_I[i] = rData.dLengthRatio_I[i];
			dLengthRatio_J[i] = rData.dLengthRatio_J[i];
		}    

		for(int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = 0.5;
			dHingeLength_J[j] = 0.5;
		}

		bRelativeLength = TRUE;
		nHingeType        = 0;
		PropKey           = 0;
		FibrKey           = 0;
		nFiberAutoUser    = 1;
		nUsePogdShearR    = 0;
		dShearR           = 0.4;
		nUsePogdConsOutpl = 0;
		bConsOutpl        = FALSE;
		nASInteractionType = 0;

		nIntegrationPoint=rData.nIntegrationPoint;    
		bConsiderHingeLength = rData.bConsiderHingeLength;
		YieldSurface = rData.YieldSurface;
		Masonry.Convert885(rData.Masonry);
	}

	void Convert861(T_PHGT_D_CH_861& rData)
	{
		nSeq             = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType       ;       
		nWallType        = rData.nWallType       ;
		nMaterial        = rData.nMaterial       ;      
		nDefinition      = rData.nDefinition     ;     
		nInteractionType = rData.nInteractionType;

		BOOL bConvert=FALSE;
		for(int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert865(rData.Skcv[i]);
			SkcvSub[i].Convert865(rData.SkcvSub[i]);
			nDistribution[i]  = rData.nDistribution[i];
			dLengthRatio_I[i] = rData.dLengthRatio_I[i];
			dLengthRatio_J[i] = rData.dLengthRatio_J[i];
		}    

		for(int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = rData.dHingeLength_I[j];
			dHingeLength_J[j] = rData.dHingeLength_J[j];
		}

		bRelativeLength   = rData.bRelativeLength;
		nHingeType        = 0;
		PropKey           = 0;
		FibrKey           = 0;
		nFiberAutoUser    = 1;
		nUsePogdShearR    = 0;
		dShearR           = 0.4;
		nUsePogdConsOutpl = 0;
		bConsOutpl        = FALSE;
		nASInteractionType = 0;

		nIntegrationPoint=rData.nIntegrationPoint;    
		bConsiderHingeLength = rData.bConsiderHingeLength;
		YieldSurface = rData.YieldSurface;
		Masonry.Convert885(rData.Masonry);
	}

	void Convert865(T_PHGT_D_CH_865& rData)
	{
		nSeq             = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType       ;       
		nWallType        = rData.nWallType       ;
		nMaterial        = rData.nMaterial       ;      
		nDefinition      = rData.nDefinition     ;     
		nInteractionType = rData.nInteractionType;

		BOOL bConvert=FALSE;
		for(int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert865(rData.Skcv[i]);
			SkcvSub[i].Convert865(rData.SkcvSub[i]);
			nDistribution[i]  = rData.nDistribution[i];
			dLengthRatio_I[i] = rData.dLengthRatio_I[i];
			dLengthRatio_J[i] = rData.dLengthRatio_J[i];
		}    

		for(int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = rData.dHingeLength_I[j];
			dHingeLength_J[j] = rData.dHingeLength_J[j];
		}

		bRelativeLength   = rData.bRelativeLength;
		nHingeType        = rData.nHingeType       ;
		PropKey           = rData.PropKey          ;
		FibrKey           = rData.FibrKey          ;
		nFiberAutoUser    = rData.nFiberAutoUser   ;
		nUsePogdShearR    = rData.nUsePogdShearR   ;
		dShearR           = rData.dShearR          ;
		nUsePogdConsOutpl = rData.nUsePogdConsOutpl;
		bConsOutpl        = rData.bConsOutpl       ;
		nASInteractionType = 0;

		nIntegrationPoint=rData.nIntegrationPoint;    
		bConsiderHingeLength = rData.bConsiderHingeLength;
		YieldSurface = rData.YieldSurface;
		Masonry.Convert885(rData.Masonry);
	}
	void Convert865_v2(T_PHGT_D_CH_865_v2& rData)
	{
		nSeq             = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType       ;       
		nWallType        = rData.nWallType       ;
		nMaterial        = rData.nMaterial       ;      
		nDefinition      = rData.nDefinition     ;     
		nInteractionType = rData.nInteractionType;

		BOOL bConvert=FALSE;
		for(int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert870_minor(rData.Skcv[i]);
			SkcvSub[i].Convert870_minor(rData.SkcvSub[i]);
			nDistribution[i]  = rData.nDistribution[i];
			dLengthRatio_I[i] = rData.dLengthRatio_I[i];
			dLengthRatio_J[i] = rData.dLengthRatio_J[i];
		}    

		for(int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = rData.dHingeLength_I[j];
			dHingeLength_J[j] = rData.dHingeLength_J[j];
		}

		bRelativeLength   = rData.bRelativeLength;
		nHingeType        = rData.nHingeType       ;
		PropKey           = rData.PropKey          ;
		FibrKey           = rData.FibrKey          ;
		nFiberAutoUser    = rData.nFiberAutoUser   ;
		nUsePogdShearR    = rData.nUsePogdShearR   ;
		dShearR           = rData.dShearR          ;
		nUsePogdConsOutpl = rData.nUsePogdConsOutpl;
		bConsOutpl        = rData.bConsOutpl       ;
		nASInteractionType = 0;

		nIntegrationPoint=rData.nIntegrationPoint;    
		bConsiderHingeLength = rData.bConsiderHingeLength;
		YieldSurface = rData.YieldSurface;
		Masonry.Convert885(rData.Masonry);
	}
	void Convert865_v3(T_PHGT_D_CH_865_v3& rData)
	{
		nSeq             = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType       ;       
		nWallType        = rData.nWallType       ;
		nMaterial        = rData.nMaterial       ;      
		nDefinition      = rData.nDefinition     ;     
		nInteractionType = rData.nInteractionType;

		BOOL bConvert=FALSE;
		for(int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert870_minor(rData.Skcv[i]);
			SkcvSub[i].Convert870_minor(rData.SkcvSub[i]);
			nDistribution[i]  = rData.nDistribution[i];
			dLengthRatio_I[i] = rData.dLengthRatio_I[i];
			dLengthRatio_J[i] = rData.dLengthRatio_J[i];
		}    

		for(int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = rData.dHingeLength_I[j];
			dHingeLength_J[j] = rData.dHingeLength_J[j];
		}

		bRelativeLength   = rData.bRelativeLength;
		nHingeType        = rData.nHingeType       ;
		PropKey           = rData.PropKey          ;
		FibrKey           = rData.FibrKey          ;
		nFiberAutoUser    = rData.nFiberAutoUser   ;
		nUsePogdShearR    = rData.nUsePogdShearR   ;
		dShearR           = rData.dShearR          ;
		nUsePogdConsOutpl = rData.nUsePogdConsOutpl;
		bConsOutpl        = rData.bConsOutpl       ;
		nASInteractionType = rData.nASInteractionType;

		nIntegrationPoint=rData.nIntegrationPoint;    
		bConsiderHingeLength = rData.bConsiderHingeLength;
		YieldSurface = rData.YieldSurface;
		Masonry.Convert885(rData.Masonry);
	}
	void Convert875(T_PHGT_D_CH_875& rData)
	{
		nSeq             = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType       ;       
		nWallType        = rData.nWallType       ;
		nMaterial        = rData.nMaterial       ;      
		nDefinition      = rData.nDefinition     ;     
		nInteractionType = rData.nInteractionType;

		for(int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert875(rData.Skcv[i]);
			SkcvSub[i].Convert875(rData.SkcvSub[i]);
			nDistribution[i]  = rData.nDistribution[i];
			dLengthRatio_I[i] = rData.dLengthRatio_I[i];
			dLengthRatio_J[i] = rData.dLengthRatio_J[i];
		}    

		for(int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = rData.dHingeLength_I[j];
			dHingeLength_J[j] = rData.dHingeLength_J[j];
		}

		bRelativeLength   = rData.bRelativeLength;
		nHingeType        = rData.nHingeType       ;
		PropKey           = rData.PropKey          ;
		FibrKey           = rData.FibrKey          ;
		nFiberAutoUser    = rData.nFiberAutoUser   ;
		nUsePogdShearR    = rData.nUsePogdShearR   ;
		dShearR           = rData.dShearR          ;
		nUsePogdConsOutpl = rData.nUsePogdConsOutpl;
		bConsOutpl        = rData.bConsOutpl       ;
		nASInteractionType = rData.nASInteractionType;

		nIntegrationPoint=rData.nIntegrationPoint;    
		bConsiderHingeLength = rData.bConsiderHingeLength;
		YieldSurface = rData.YieldSurface;
		Masonry.Convert885(rData.Masonry);
	}
	void Convert880(T_PHGT_D_CH_880& rData)
	{
		nSeq             = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType       ;       
		nWallType        = rData.nWallType       ;
		nMaterial        = rData.nMaterial       ;      
		nDefinition      = rData.nDefinition     ;     
		nInteractionType = rData.nInteractionType;

		for(int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert880(rData.Skcv[i]);
			SkcvSub[i].Convert880(rData.SkcvSub[i]);
			nDistribution[i]  = rData.nDistribution[i];
			dLengthRatio_I[i] = rData.dLengthRatio_I[i];
			dLengthRatio_J[i] = rData.dLengthRatio_J[i];
		}    

		for(int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = rData.dHingeLength_I[j];
			dHingeLength_J[j] = rData.dHingeLength_J[j];
		}

		bRelativeLength   = rData.bRelativeLength;
		nHingeType        = rData.nHingeType       ;
		PropKey           = rData.PropKey          ;
		FibrKey           = rData.FibrKey          ;
		nFiberAutoUser    = rData.nFiberAutoUser   ;
		nUsePogdShearR    = rData.nUsePogdShearR   ;
		dShearR           = rData.dShearR          ;
		nUsePogdConsOutpl = rData.nUsePogdConsOutpl;
		bConsOutpl        = rData.bConsOutpl       ;
		nASInteractionType = rData.nASInteractionType;

		nIntegrationPoint=rData.nIntegrationPoint;    
		bConsiderHingeLength = rData.bConsiderHingeLength;
		YieldSurface = rData.YieldSurface;
		Masonry.Convert885(rData.Masonry);
	}
	void Convert881(T_PHGT_D_CH_881& rData)
	{
		nSeq             = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType       ;       
		nWallType        = rData.nWallType       ;
		nMaterial        = rData.nMaterial       ;      
		nDefinition      = rData.nDefinition     ;     
		nInteractionType = rData.nInteractionType;

		for(int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert881(rData.Skcv[i]);
			SkcvSub[i].Convert881(rData.SkcvSub[i]);
			nDistribution[i]  = rData.nDistribution[i];
			dLengthRatio_I[i] = rData.dLengthRatio_I[i];
			dLengthRatio_J[i] = rData.dLengthRatio_J[i];
		}    

		for(int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = rData.dHingeLength_I[j];
			dHingeLength_J[j] = rData.dHingeLength_J[j];
		}

		bRelativeLength   = rData.bRelativeLength;
		nHingeType        = rData.nHingeType       ;
		PropKey           = rData.PropKey          ;
		FibrKey           = rData.FibrKey          ;
		nFiberAutoUser    = rData.nFiberAutoUser   ;
		nUsePogdShearR    = rData.nUsePogdShearR   ;
		dShearR           = rData.dShearR          ;
		nUsePogdConsOutpl = rData.nUsePogdConsOutpl;
		bConsOutpl        = rData.bConsOutpl       ;
		nASInteractionType = rData.nASInteractionType;

		nIntegrationPoint=rData.nIntegrationPoint;    
		bConsiderHingeLength = rData.bConsiderHingeLength;
		YieldSurface = rData.YieldSurface;
		Masonry.Convert885(rData.Masonry);
	}
	void Convert885(T_PHGT_D_RW_885& rData)
	{
		nSeq             = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType       ;       
		nWallType        = rData.nWallType       ;
		nMaterial        = rData.nMaterial       ;      
		nDefinition      = rData.nDefinition     ;     
		nInteractionType = rData.nInteractionType;

		for(int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert896(rData.Skcv[i]);
			SkcvSub[i].Convert896(rData.SkcvSub[i]);
			nDistribution[i]  = rData.nDistribution[i];
			dLengthRatio_I[i] = rData.dLengthRatio_I[i];
			dLengthRatio_J[i] = rData.dLengthRatio_J[i];
		}    

		for(int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = rData.dHingeLength_I[j];
			dHingeLength_J[j] = rData.dHingeLength_J[j];
		}

		bRelativeLength   = rData.bRelativeLength;
		nHingeType        = rData.nHingeType       ;
		PropKey           = rData.PropKey          ;
		FibrKey           = rData.FibrKey          ;
		nFiberAutoUser    = rData.nFiberAutoUser   ;
		nUsePogdShearR    = rData.nUsePogdShearR   ;
		dShearR           = rData.dShearR          ;
		nUsePogdConsOutpl = rData.nUsePogdConsOutpl;
		bConsOutpl        = rData.bConsOutpl       ;
		nASInteractionType = rData.nASInteractionType;

		nIntegrationPoint=rData.nIntegrationPoint;    
		bConsiderHingeLength = rData.bConsiderHingeLength;
		YieldSurface = rData.YieldSurface;
		Masonry.Convert885(rData.Masonry);
	}
	void Convert896(T_PHGT_D_RW_896& rData)
	{
		nSeq             = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType       ;       
		nWallType        = rData.nWallType       ;
		nMaterial        = rData.nMaterial       ;      
		nDefinition      = rData.nDefinition     ;     
		nInteractionType = rData.nInteractionType;

		for(int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert896(rData.Skcv[i]);
			SkcvSub[i].Convert896(rData.SkcvSub[i]);
			nDistribution[i]  = rData.nDistribution[i];
			dLengthRatio_I[i] = rData.dLengthRatio_I[i];
			dLengthRatio_J[i] = rData.dLengthRatio_J[i];
		}    

		for(int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = rData.dHingeLength_I[j];
			dHingeLength_J[j] = rData.dHingeLength_J[j];
		}

		bRelativeLength   = rData.bRelativeLength;
		nHingeType        = rData.nHingeType       ;
		PropKey           = rData.PropKey          ;
		FibrKey           = rData.FibrKey          ;
		nFiberAutoUser    = rData.nFiberAutoUser   ;
		nUsePogdShearR    = rData.nUsePogdShearR   ;
		dShearR           = rData.dShearR          ;
		nUsePogdConsOutpl = rData.nUsePogdConsOutpl;
		bConsOutpl        = rData.bConsOutpl       ;
		nASInteractionType = rData.nASInteractionType;

		nIntegrationPoint=rData.nIntegrationPoint;    
		bConsiderHingeLength = rData.bConsiderHingeLength;
		YieldSurface = rData.YieldSurface;
		Masonry = rData.Masonry;
	}
	void Convert905(T_PHGT_D_RW_905& rData)
	{
		nSeq             = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType;
		nWallType        = rData.nWallType;
		nMaterial        = rData.nMaterial;
		nDefinition      = rData.nDefinition;
		nInteractionType = rData.nInteractionType;

		for (int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert905(rData.Skcv[i]);
			SkcvSub[i].Convert905(rData.SkcvSub[i]);
			nDistribution[i]  = rData.nDistribution[i];
			dLengthRatio_I[i] = rData.dLengthRatio_I[i];
			dLengthRatio_J[i] = rData.dLengthRatio_J[i];
		}

		for (int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = rData.dHingeLength_I[j];
			dHingeLength_J[j] = rData.dHingeLength_J[j];
		}

		bRelativeLength   = rData.bRelativeLength;
		nHingeType        = rData.nHingeType;
		PropKey           = rData.PropKey;
		FibrKey           = rData.FibrKey;
		nFiberAutoUser    = rData.nFiberAutoUser;
		nUsePogdShearR    = rData.nUsePogdShearR;
		dShearR           = rData.dShearR;
		nUsePogdConsOutpl = rData.nUsePogdConsOutpl;
		bConsOutpl        = rData.bConsOutpl;
		nASInteractionType = rData.nASInteractionType;

		nIntegrationPoint=rData.nIntegrationPoint;
		bConsiderHingeLength = rData.bConsiderHingeLength;
		YieldSurface = rData.YieldSurface;
		Masonry = rData.Masonry;
	}
	void Convert955(T_PHGT_D_RW_955& rData)
	{
		nSeq = rData.nSeq;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nElemType = rData.nElemType;
		nWallType = rData.nWallType;
		nMaterial = rData.nMaterial;
		nDefinition = rData.nDefinition;
		nInteractionType = rData.nInteractionType;

		for (int i = 0; i < 6; ++i)
		{
			bDOF[i] = rData.bDOF[i];
			nLoc[i] = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].Convert955(rData.Skcv[i]);
			SkcvSub[i].Convert955(rData.SkcvSub[i]);
			nDistribution[i] = rData.nDistribution[i];
			dLengthRatio_I[i] = rData.dLengthRatio_I[i];
			dLengthRatio_J[i] = rData.dLengthRatio_J[i];
		}

		for (int j = 0; j < 2; ++j)
		{
			dHingeLength_I[j] = rData.dHingeLength_I[j];
			dHingeLength_J[j] = rData.dHingeLength_J[j];
		}

		bRelativeLength = rData.bRelativeLength;
		nHingeType = rData.nHingeType;
		PropKey = rData.PropKey;
		FibrKey = rData.FibrKey;
		nFiberAutoUser = rData.nFiberAutoUser;
		nUsePogdShearR = rData.nUsePogdShearR;
		dShearR = rData.dShearR;
		nUsePogdConsOutpl = rData.nUsePogdConsOutpl;
		bConsOutpl = rData.bConsOutpl;
		nASInteractionType = rData.nASInteractionType;

		nIntegrationPoint = rData.nIntegrationPoint;
		bConsiderHingeLength = rData.bConsiderHingeLength;
		YieldSurface = rData.YieldSurface;
		Masonry = rData.Masonry;
	}
	void ConvertToData(T_PHGT_D& rData)
	{ 
		rData.nSeq             = nSeq;
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDescription, rData.strDescription, sizeof(strDescription));
		rData.nElemType        = nElemType       ;       
		rData.nWallType        = nWallType       ;
		rData.nMaterial        = nMaterial       ;      
		rData.nDefinition      = nDefinition     ;     
		rData.nInteractionType = nInteractionType;
		rData.bConsiderHingeLength = bConsiderHingeLength;
		rData.nIntegrationPoint    = nIntegrationPoint;
		for(int i=0; i<6; ++i)
		{
			rData.bDOF[i]      = bDOF[i];
			rData.nLoc[i]      = nLoc[i];
			rData.nSkcvType[i] = nSkcvType[i];
			Skcv[i].ConvertToData(rData.Skcv[i]);
			SkcvSub[i].ConvertToData(rData.SkcvSub[i]);
			rData.nDistribution[i] = nDistribution[i];
			rData.dLengthRatio_I[i] = dLengthRatio_I[i];
			rData.dLengthRatio_J[i] = dLengthRatio_J[i];
		}
		for(int j=0; j<2; ++j)
		{
			rData.dHingeLength_I[j] = dHingeLength_I[j];
			rData.dHingeLength_J[j] = dHingeLength_J[j];
		}

		rData.bRelativeLength   = bRelativeLength;
		rData.nHingeType        = nHingeType       ;
		rData.PropKey           = PropKey          ;
		rData.FibrKey           = FibrKey          ;
		rData.nFiberAutoUser    = nFiberAutoUser   ;
		rData.nUsePogdShearR    = nUsePogdShearR   ;
		rData.dShearR           = dShearR          ;
		rData.nUsePogdConsOutpl = nUsePogdConsOutpl;
		rData.bConsOutpl        = bConsOutpl       ;
		rData.nASInteractionType = nASInteractionType;

		rData.YieldSurface = YieldSurface;
		rData.Masonry      = Masonry;
	}
	void ConvertFromData(T_PHGT_D& rData)
	{
		nSeq             = rData.nSeq;
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDescription, strDescription, sizeof(strDescription));
		nElemType        = rData.nElemType       ;       
		nWallType        = rData.nWallType       ;
		nMaterial        = rData.nMaterial       ;      
		nDefinition      = rData.nDefinition     ;     
		nInteractionType = rData.nInteractionType;
		bConsiderHingeLength = rData.bConsiderHingeLength;
		nIntegrationPoint    = rData.nIntegrationPoint;
		for(int i=0; i<6; ++i)
		{
			bDOF[i]      = rData.bDOF[i];
			nLoc[i]      = rData.nLoc[i];
			nSkcvType[i] = rData.nSkcvType[i];
			Skcv[i].ConvertFromData(rData.Skcv[i]);
			SkcvSub[i].ConvertFromData(rData.SkcvSub[i]);
			nDistribution[i] = rData.nDistribution[i];
			dLengthRatio_I[i] = rData.dLengthRatio_I[i];
			dLengthRatio_J[i] = rData.dLengthRatio_J[i];
		}   
		for(int j=0; j<2; ++j)
		{
			dHingeLength_I[j] = rData.dHingeLength_I[j];
			dHingeLength_J[j] = rData.dHingeLength_J[j];
		}

		bRelativeLength   = rData.bRelativeLength;
		nHingeType        = rData.nHingeType       ;
		PropKey           = rData.PropKey          ;
		FibrKey           = rData.FibrKey          ;
		nFiberAutoUser    = rData.nFiberAutoUser   ;
		nUsePogdShearR    = rData.nUsePogdShearR   ;
		dShearR           = rData.dShearR          ;
		nUsePogdConsOutpl = rData.nUsePogdConsOutpl;
		bConsOutpl        = rData.bConsOutpl       ;
		nASInteractionType = rData.nASInteractionType;

		YieldSurface = rData.YieldSurface;
		Masonry      = rData.Masonry;
	}
};
struct T_PHGT_UNIT
{
	int dHingeLength;
	T_PHGT_SKCV_UNIT Skcv;
	T_PHGT_YLDS_UNIT Ylds;
	T_PHGT_MSNR_UNIT Msnr;
	
	T_PHGT_UNIT()
	{
		dHingeLength = D_UNITSYS_BASE_LENGTH;
	}
};

struct T_PHGT_UDRD_D
{
	T_PHGT_K key;
	T_PHGT_D data;
};
struct T_PHGT_UDRD_D_CH_730
{
	T_PHGT_K key;
	T_PHGT_D_CH_730 data;
};
struct T_PHGT_UDRD_D_CH_740
{
	T_PHGT_K key;
	T_PHGT_D_CH_740 data;
};
struct T_PHGT_UDRD_D_CH_796
{
	T_PHGT_K key;
	T_PHGT_D_CH_796 data;
};
struct T_PHGT_UDRD_D_CH_815
{
	T_PHGT_K key;
	T_PHGT_D_CH_815 data;
};

struct T_PHGT_UDRD_D_CH_830
{
	T_PHGT_K key;
	T_PHGT_D_CH_830 data;
};

struct T_PHGT_UDRD_D_CH_861
{
	T_PHGT_K key;
	T_PHGT_D_CH_861 data;
};

struct T_PHGT_UDRD_D_CH_865
{
	T_PHGT_K key;
	T_PHGT_D_CH_865 data;
};

struct T_PHGT_UDRD_D_CH_865_v2
{
	T_PHGT_K key;
	T_PHGT_D_CH_865_v2 data;
};

struct T_PHGT_UDRD_D_CH_865_v3
{
	T_PHGT_K key;
	T_PHGT_D_CH_865_v3 data;
};

struct T_PHGT_UDRD_D_CH_875
{
	T_PHGT_K key;
	T_PHGT_D_CH_875 data;
};
struct T_PHGT_UDRD_D_CH_880
{
	T_PHGT_K key;
	T_PHGT_D_CH_880 data;
};
struct T_PHGT_UDRD_D_CH_881
{
	T_PHGT_K key;
	T_PHGT_D_CH_881 data;
};
struct T_PHGT_UDRD_D_RW_885
{
	T_PHGT_K key;
	T_PHGT_D_RW_885 data;
};
struct T_PHGT_UDRD_D_RW_896
{
	T_PHGT_K key;
	T_PHGT_D_RW_896 data;
};
struct T_PHGT_UDRD_D_RW_905
{
	T_PHGT_K key;
	T_PHGT_D_RW_905 data;
};
struct T_PHGT_UDRD_D_RW_955
{
	T_PHGT_K key;
	T_PHGT_D_RW_955 data;
};
struct T_PHGT_UDRD_D_RW
{
	T_PHGT_K key;
	T_PHGT_D_RW data;
};
#define HASHSIZEPHGT 11

// Assign Pushover Hinge Type/Properties
#define D_PHGE_BEAMCOLUMN 1
#define D_PHGE_WALL       2
#define D_PHGE_TRUSS      3
#define D_PHGE_GLINK      4
#define D_PHGE_PSPR       5 // Point Spring Support

#define T_PHGE_KEY unsigned int
union T_PHGE_K_730A
{
	T_PHGE_KEY keymap;
	struct
	{
		unsigned int type : 20; // 1:Beam/Column, 2:Wall, 3:Truss, 4:GeneralLink(NLNK)
		unsigned int objK : 12; // type==1,3 ->ElemK, type==2 -> WALL_K, type==4 -> NLNK_K
	} key;
};
union T_PHGE_K
{
	T_PHGE_KEY keymap;
	struct
	{
		unsigned int type : 4; // 1:Beam/Column, 2:Wall, 3:Truss, 4:GeneralLink(NLNK), 5:Point Spring Support
		unsigned int objK : 28; // type==1,3 ->ElemK, type==2 -> WALL_K, type==4 -> NLNK_K
	} key;
};
struct T_PHGE_D_730B     // (before: T_HNGE_D)
{
	T_PHGT_K HingeTypeK;
	BOOL bUpdateProperties;   // TRUE:  PhgtD가 변경되면 PhgtD.Skcv, SkcvSub, Yield를 변경해준다.
														// FALSE: PhgtD가 변경되어도 데이터를 보존한다.
	T_PHGT_SKCV_730 Skcv[6];        // 디폴트 저장공간(J-end only일 경우에는 여기 저장)
	T_PHGT_SKCV_730 SkcvSub[6];     // My, Mz 자유도에서 I&J 일 경우 J단의 값 저장
	T_PHGT_YLDS YieldSurface;
	T_PHGT_MSNR_885 Masonry;
};
struct T_PHGE_D_865
{
	T_PHGT_K HingeTypeK;
};
struct T_PHGE_D     // (before: T_HNGE_D)
{
	T_PHGT_K HingeTypeK;
	T_KEY FiberKey;   // Iehp가 Fiber&Auto 일 때는 이 값을 사용
	T_PHGE_D() : HingeTypeK(0), FiberKey(0) {}
	void Initialize()
	{
		HingeTypeK=0;
		FiberKey  =0;
	}
	T_PHGE_D& operator=(T_PHGE_D& src)
	{
		HingeTypeK = src.HingeTypeK;
		FiberKey = src.FiberKey;
		return *this;
	}
	BOOL operator==(const T_PHGE_D& src) const
	{
		if(HingeTypeK != src.HingeTypeK) return FALSE;    
		if(FiberKey != src.FiberKey) return FALSE;    
		return TRUE;
	}
	BOOL operator!=(const T_PHGE_D& src) const
	{
		return !( *this == src );
	}
	void Convert730B(T_PHGE_D_730B& src)
	{
		HingeTypeK = src.HingeTypeK;
		FiberKey  =0;
	}
	void Convert865(T_PHGE_D_865& src)
	{
		HingeTypeK = src.HingeTypeK;
		FiberKey  =0;
	}
};

struct T_PHGE_UDRD_D_730A
{
	T_PHGE_K key;
	T_PHGE_D_730B data;
};
struct T_PHGE_UDRD_D_730B // 730A 에서 T_PHGE_K 의 비트수를 바꾼것
{
	T_PHGE_K key;
	T_PHGE_D_730B data;
	int nVer730B;
};
struct T_PHGE_UDRD_D_865 // 730B 에서 data를 대폭 정리한 것.
{
	T_PHGE_K key;
	T_PHGE_D_865 data;
};
struct T_PHGE_UDRD_D 
{
	T_PHGE_K key;
	T_PHGE_D data;
};

struct T_PHGE_UNIT
{
//   T_PHGE_UNIT()
//   {
//     HingeType = D_UNITSYS_NONE;
//   }
//   int HingeType;
	T_PHGT_SKCV_UNIT Skcv;
};

#define HASHSIZEPHGE      HASHSIZEELEM

struct T_POLC_BASE
{
	// T_POLC_D 와 T_POCT2_D 에서 사용
	T_KEY LoadCaseKey; // nLoadPatternType에 따라 Static Load Case Key,
										 // Acceleration(1=X, 2=Y, 3=Z), Mode Shape No
	double dScaleFactor;
	void Initialize()
	{
		LoadCaseKey  = 0;
		dScaleFactor = 1.0;
	}

	bool operator==(const T_POLC_BASE& src) const
	{
		if (this == &src) return true;
		if (dScaleFactor != src.dScaleFactor) return false;
		if (LoadCaseKey != src.LoadCaseKey) return false;
		return true;
	}
	bool operator!=(const T_POLC_BASE& src) const { return !(*this == src); }

};

enum D_POLC_GEOMNONLIN { D_POLC_GEOMNONLIN_NONE, D_POLC_GEOMNONLIN_LARGE_DISP, D_POLC_GEOMNONLIN_P_DELTA };

#define T_POLC_K unsigned int
struct T_POLC_D              // Pushover Load Case --------------------------------
{
	unsigned int LoadCaseId;
	CString strName;
	CString strDescription;
	int     nStepNum;
	BOOL    bUseInitial;
	BOOL    bConsiderPDelta;
	int     nIncrementMethod;    //0=Load, 1=Displ.
	// Load Control
	int     nStepCtrlOption;     //0=Auto, 1=Equal, 2=Inc.Function
	T_KEY   IncFuncK;
	// Displacement Control
	int      nDispCtrlOption ;   // 0=Global Control, 1=Master Node Control
	double   dOverallMaxDisp;    // nControlOption==1
	T_NODE_K MasterNode     ;    // nControlOption==2
	int      MasterDirection;    // nControlOption==2 : 1=DX, 2=DY, 3=DZ
	double   dMasterMaxDisp;     // nControlOption==2
	// Analysis Stopping Condition
	double   dStiffPercent;        // % 입력, 솔버엔 소수로 넘겨야 함.
	BOOL     bLimitDeformAngle;
	double   dIntStorDeformValue; // Max : 분모값이므로 솔버넘길때 주의
	// Load Pattern
	int      nLoadPattern;
	CArray<T_POLC_BASE, T_POLC_BASE&> aLoadPattern;
	// Initial Load
	BOOL      bReacOutput;      // 790
	BOOL      bDispOutput;      // 790
	
	BOOL   bDriftMax   ;  // 791
	BOOL   bDriftCenter;  // 791 
	BOOL   bDriftAver  ;  // 791
	BOOL   bAnalStop   ;  // 791
	int    nAnalStepNum;  // 791
	double dIntStorDeformValue_Cent; // 791
	double dIntStorDeformValue_Aver; // 791
	int  nGeomNonlinearType;  // D_POLC_GEOMNONLIN

	void Initialize()
	{
		LoadCaseId = 0;
		strName.Empty();
		strDescription.Empty();
		nStepNum        =20;
		bUseInitial     =FALSE;
		bConsiderPDelta =FALSE;
		nIncrementMethod=0;
		nStepCtrlOption =0;
		IncFuncK        =0;
		nDispCtrlOption =0;
		dOverallMaxDisp =0.0;
		MasterNode      =0;
		MasterDirection =0;
		dMasterMaxDisp  =0.0;
		dStiffPercent   =0.0;
		bLimitDeformAngle =(nIncrementMethod==0) ? TRUE: FALSE;// Default 세팅시 참고할 것
		nLoadPattern =0;
		dIntStorDeformValue =10.;
		aLoadPattern.RemoveAll();

		bReacOutput = FALSE;
		bDispOutput = FALSE;
			 
		bDriftMax                = bLimitDeformAngle  ;
		bDriftCenter             = FALSE              ;
		bDriftAver               = FALSE              ;    
		bAnalStop                = FALSE              ;  
		nAnalStepNum             = nStepNum           ;  
		dIntStorDeformValue_Cent = dIntStorDeformValue; 
		dIntStorDeformValue_Aver = dIntStorDeformValue; 
		nGeomNonlinearType = D_POLC_GEOMNONLIN_NONE;
	}
	T_POLC_D()
	{
	}
	T_POLC_D(T_POLC_D& src)
	{
		*this = src;
	}
	T_POLC_D& operator=(T_POLC_D& src)
	{
		LoadCaseId          =src.LoadCaseId;
		strName             =src.strName;
		strDescription      =src.strDescription;
		nStepNum            =src.nStepNum;
		bUseInitial         =src.bUseInitial;
		bConsiderPDelta     =src.bConsiderPDelta;
		nIncrementMethod    =src.nIncrementMethod;
		nStepCtrlOption     =src.nStepCtrlOption;
		IncFuncK            =src.IncFuncK;
		nDispCtrlOption     =src.nDispCtrlOption;
		dOverallMaxDisp     =src.dOverallMaxDisp;
		MasterNode          =src.MasterNode;
		MasterDirection     =src.MasterDirection;
		dMasterMaxDisp      =src.dMasterMaxDisp;
		dStiffPercent       =src.dStiffPercent;
		bLimitDeformAngle   =src.bLimitDeformAngle;
		dIntStorDeformValue =src.dIntStorDeformValue;
		nLoadPattern        =src.nLoadPattern;
		aLoadPattern.Copy(src.aLoadPattern);

		bReacOutput         = src.bReacOutput;
		bDispOutput         = src.bDispOutput;

		bDriftMax                = src.bDriftMax               ;
		bDriftCenter             = src.bDriftCenter            ;
		bDriftAver               = src.bDriftAver              ;
		bAnalStop                = src.bAnalStop               ;
		nAnalStepNum             = src.nAnalStepNum            ;
		dIntStorDeformValue_Cent = src.dIntStorDeformValue_Cent; 
		dIntStorDeformValue_Aver = src.dIntStorDeformValue_Aver; 
		nGeomNonlinearType = src.nGeomNonlinearType;
		return *this;
	}

	bool operator==(const T_POLC_D& src) const
	{
		if (this == &src) return true;
		if (LoadCaseId != src.LoadCaseId) return false;
		if (strName != src.strName) return false;
		if (strDescription != src.strDescription) return false;
		if (nStepNum != src.nStepNum) return false;
		if (bUseInitial != src.bUseInitial) return false;
		if (bConsiderPDelta != src.bConsiderPDelta) return false;
		if (nIncrementMethod != src.nIncrementMethod) return false;
		if (nStepCtrlOption != src.nStepCtrlOption) return false;
		if (IncFuncK != src.IncFuncK) return false;
		if (nDispCtrlOption != src.nDispCtrlOption) return false;
		if (dOverallMaxDisp != src.dOverallMaxDisp) return false;
		if (MasterNode != src.MasterNode) return false;
		if (MasterDirection != src.MasterDirection) return false;
		if (dMasterMaxDisp != src.dMasterMaxDisp) return false;
		if (dStiffPercent != src.dStiffPercent) return false;
		if (bLimitDeformAngle != src.bLimitDeformAngle) return false;
		if (dIntStorDeformValue != src.dIntStorDeformValue) return false;
		if (nLoadPattern != src.nLoadPattern) return false;

		if (bReacOutput != src.bReacOutput) return false;
		if (bDispOutput != src.bDispOutput) return false;

		if (bDriftMax != src.bDriftMax) return false;
		if (bDriftCenter != src.bDriftCenter) return false;
		if (bDriftAver != src.bDriftAver) return false;
		if (bAnalStop != src.bAnalStop) return false;
		if (nAnalStepNum != src.nAnalStepNum) return false;
		if (dIntStorDeformValue_Cent != src.dIntStorDeformValue_Cent) return false;
		if (dIntStorDeformValue_Aver != src.dIntStorDeformValue_Aver) return false;
		if (nGeomNonlinearType != src.nGeomNonlinearType) return false;

		if (aLoadPattern.GetSize() != src.aLoadPattern.GetSize()) {
			return false;
		}
		else
		{
			int nDataTgt = aLoadPattern.GetSize();
			int nDataSrc = src.aLoadPattern.GetSize();
			int i, j;
			for (i = 0; i < nDataTgt; ++i)
			{
				for (j = 0; j < nDataSrc; ++j)
				{
					if (aLoadPattern[i] == src.aLoadPattern[j]) break;
				}
				if (j == nDataSrc) return false;
			}
		}
		return true;
	}

	bool operator!=(const T_POLC_D& src) const { return !(*this == src); }

};
struct T_POLC_D_CH
{
	unsigned int LoadCaseId;
	char     strName[20];
	char     strDescription[80];
	int      nStepNum;
	BOOL     bUseInitial;
	BOOL     bConsiderPDelta;
	int      nIncrementMethod; 
	int      nStepCtrlOption;  
	T_KEY    IncFuncK;  
	int      nDispCtrlOption ;
	double   dOverallMaxDisp; 
	T_NODE_K MasterNode     ; 
	int      MasterDirection; 
	double   dMasterMaxDisp;
	double   dStiffPercent;     
	BOOL     bLimitDeformAngle;
	double   dIntStorDeformValue; 
	int      nLoadPattern;
	CArray<T_POLC_BASE, T_POLC_BASE&> aLoadPattern;

	BOOL      bReacOutput;
	BOOL      bDispOutput;

	BOOL   bDriftMax               ;
	BOOL   bDriftCenter            ;
	BOOL   bDriftAver              ;
	BOOL   bAnalStop               ; 
	int    nAnalStepNum            ; 
	double dIntStorDeformValue_Cent; 
	double dIntStorDeformValue_Aver; 
	int   nGeomNonlinearType;  // D_POLC_GEOMNONLIN
	void Initialize()
	{
		LoadCaseId = 0;
		memset(strName,         0, sizeof(strName));
		memset(strDescription,  0, sizeof(strDescription));
		nStepNum        =20;
		bUseInitial     =FALSE;
		bConsiderPDelta =FALSE;
		nIncrementMethod=0;
		nStepCtrlOption =0;
		IncFuncK        =0;
		nDispCtrlOption =0;
		dOverallMaxDisp =0.0;
		MasterNode      =0;
		MasterDirection =0;
		dMasterMaxDisp  =0.0;
		dStiffPercent   =0.0;
		bLimitDeformAngle =(nIncrementMethod==0) ? TRUE: FALSE;// Default 세팅시 참고할 것
		nLoadPattern =0;
		dIntStorDeformValue =10.;
		aLoadPattern.RemoveAll();
		bReacOutput = FALSE;
		bDispOutput = FALSE;
		bDriftMax                = bLimitDeformAngle  ;
		bDriftCenter             = FALSE              ;
		bDriftAver               = FALSE              ;    
		bAnalStop                = FALSE              ;  
		nAnalStepNum             = nStepNum           ;  
		dIntStorDeformValue_Cent = dIntStorDeformValue; 
		dIntStorDeformValue_Aver = dIntStorDeformValue; 
		nGeomNonlinearType = D_POLC_GEOMNONLIN_NONE;
	}
	void ConvertToString(T_POLC_D& rData)
	{
		rData.LoadCaseId         = LoadCaseId;
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDescription, rData.strDescription, sizeof(strDescription));    
		rData.nStepNum           = nStepNum           ;
		rData.bUseInitial        = bUseInitial        ;
		rData.bConsiderPDelta    = bConsiderPDelta    ;
		rData.nIncrementMethod   = nIncrementMethod   ;
		rData.nStepCtrlOption    = nStepCtrlOption    ;
		rData.IncFuncK           = IncFuncK           ;
		rData.nDispCtrlOption    = nDispCtrlOption    ;
		rData.dOverallMaxDisp    = dOverallMaxDisp    ;
		rData.MasterNode         = MasterNode         ;
		rData.MasterDirection    = MasterDirection    ;
		rData.dMasterMaxDisp     = dMasterMaxDisp     ;
		rData.dStiffPercent      = dStiffPercent      ;
		rData.bLimitDeformAngle  = bLimitDeformAngle  ;
		rData.dIntStorDeformValue= dIntStorDeformValue;
		rData.nLoadPattern       = nLoadPattern;
		rData.aLoadPattern.Copy(aLoadPattern);
		rData.bReacOutput         = bReacOutput;
		rData.bDispOutput         = bDispOutput;
		rData.bDriftMax                = bDriftMax               ;
		rData.bDriftCenter             = bDriftCenter            ;
		rData.bDriftAver               = bDriftAver              ;
		rData.bAnalStop                = bAnalStop               ;
		rData.nAnalStepNum             = nAnalStepNum            ;
		rData.dIntStorDeformValue_Cent = dIntStorDeformValue_Cent; 
		rData.dIntStorDeformValue_Aver = dIntStorDeformValue_Aver; 
		rData.nGeomNonlinearType = nGeomNonlinearType;

	}
	void ConvertToChar(T_POLC_D& rData)
	{
		LoadCaseId         = rData.LoadCaseId;
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDescription, strDescription, sizeof(strDescription));
		nStepNum           = rData.nStepNum           ;
		bUseInitial        = rData.bUseInitial        ;
		bConsiderPDelta    = rData.bConsiderPDelta    ;
		nIncrementMethod   = rData.nIncrementMethod   ;
		nStepCtrlOption    = rData.nStepCtrlOption    ;
		IncFuncK           = rData.IncFuncK           ;
		nDispCtrlOption    = rData.nDispCtrlOption    ;
		dOverallMaxDisp    = rData.dOverallMaxDisp    ;
		MasterNode         = rData.MasterNode         ;
		MasterDirection    = rData.MasterDirection    ;
		dMasterMaxDisp     = rData.dMasterMaxDisp     ;
		dStiffPercent      = rData.dStiffPercent      ;
		bLimitDeformAngle  = rData.bLimitDeformAngle  ;
		dIntStorDeformValue= rData.dIntStorDeformValue;
		nLoadPattern       = rData.nLoadPattern;
		aLoadPattern.Copy(rData.aLoadPattern);
		bReacOutput         = rData.bReacOutput;
		bDispOutput         = rData.bDispOutput;
		bDriftMax           = rData.bDriftMax   ;
		bDriftCenter        = rData.bDriftCenter;
		bDriftAver          = rData.bDriftAver  ;
		bAnalStop                = rData.bAnalStop               ;
		nAnalStepNum             = rData.nAnalStepNum            ;
		dIntStorDeformValue_Cent = rData.dIntStorDeformValue_Cent; 
		dIntStorDeformValue_Aver = rData.dIntStorDeformValue_Aver;
		nGeomNonlinearType = rData.nGeomNonlinearType;
	}
};

struct T_POLC_UDRD_D
{
	T_POLC_K key;
	T_POLC_D data;
};
struct T_POLC_UDRD_D_CH
{
	T_POLC_K    key;
	T_POLC_D_CH data;
};

struct T_POLC_UDRD_D_CH_RW_790
{
	T_POLC_K key;
	unsigned int LoadCaseId;
	char     strName[20];
	char     strDescription[80];
	int      nStepNum;
	BOOL     bUseInitial;
	BOOL     bConsiderPDelta;
	int      nIncrementMethod; 
	int      nStepCtrlOption;  
	T_KEY    IncFuncK;  
	int      nDispCtrlOption ;
	double   dOverallMaxDisp; 
	T_NODE_K MasterNode     ; 
	int      MasterDirection; 
	double   dMasterMaxDisp;
	double   dStiffPercent;     
	BOOL     bLimitDeformAngle;
	double   dIntStorDeformValue; 
	int      nLoadPattern;
	int      nCountLoadPattern;
	BOOL     bReacOutput;
	BOOL     bDispOutput;
};
struct T_POLC_UDRD_D_CH_RW_785
{
	T_POLC_K key;
	unsigned int LoadCaseId;
	char     strName[20];
	char     strDescription[80];
	int      nStepNum;
	BOOL     bUseInitial;
	BOOL     bConsiderPDelta;
	int      nIncrementMethod; 
	int      nStepCtrlOption;  
	T_KEY    IncFuncK;  
	int      nDispCtrlOption ;
	double   dOverallMaxDisp; 
	T_NODE_K MasterNode     ; 
	int      MasterDirection; 
	double   dMasterMaxDisp;
	double   dStiffPercent;     
	BOOL     bLimitDeformAngle;
	double   dIntStorDeformValue; 
	int      nLoadPattern;
	int      nCountLoadPattern;
};
struct T_POLC_UDRD_D_CH_RW_960
{
	T_POLC_K key;
	unsigned int LoadCaseId;
	char     strName[20];
	char     strDescription[80];
	int      nStepNum;
	BOOL     bUseInitial;
	BOOL     bConsiderPDelta;
	int      nIncrementMethod;
	int      nStepCtrlOption;
	T_KEY    IncFuncK;
	int      nDispCtrlOption;
	double   dOverallMaxDisp;
	T_NODE_K MasterNode;
	int      MasterDirection;
	double   dMasterMaxDisp;
	double   dStiffPercent;
	BOOL     bLimitDeformAngle;
	double   dIntStorDeformValue;
	int      nLoadPattern;
	int      nCountLoadPattern;
	BOOL     bReacOutput;
	BOOL     bDispOutput;
	BOOL     bDriftMax;
	BOOL     bDriftCenter;
	BOOL     bDriftAver;
	BOOL     bAnalStop;
	int      nAnalStepNum;
	double   dIntStorDeformValue_Cent;
	double   dIntStorDeformValue_Aver;
};

struct T_POLC_UDRD_D_CH_RW
{
	T_POLC_K key;
	unsigned int LoadCaseId;
	char     strName[20];
	char     strDescription[80];
	int      nStepNum;
	BOOL     bUseInitial;
	BOOL     bConsiderPDelta;
	int      nIncrementMethod; 
	int      nStepCtrlOption;  
	T_KEY    IncFuncK;  
	int      nDispCtrlOption ;
	double   dOverallMaxDisp; 
	T_NODE_K MasterNode     ; 
	int      MasterDirection; 
	double   dMasterMaxDisp;
	double   dStiffPercent;     
	BOOL     bLimitDeformAngle;
	double   dIntStorDeformValue; 
	int      nLoadPattern;
	int      nCountLoadPattern;
	BOOL     bReacOutput;
	BOOL     bDispOutput;
	BOOL     bDriftMax   ;
	BOOL     bDriftCenter;
	BOOL     bDriftAver  ;
	BOOL     bAnalStop               ; 
	int      nAnalStepNum            ; 
	double   dIntStorDeformValue_Cent; 
	double   dIntStorDeformValue_Aver; 
	int      nGeomNonlinearType;  // D_POLC_GEOMNONLIN

	void GetPolc(T_POLC_K& rKey, T_POLC_D_CH& rData)
	{
		rKey=key;
		rData.LoadCaseId         = LoadCaseId;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strDescription, strDescription, sizeof(strDescription));    
		rData.nStepNum           = nStepNum           ;
		rData.bUseInitial        = bUseInitial        ;
		rData.bConsiderPDelta    = bConsiderPDelta    ;
		rData.nIncrementMethod   = nIncrementMethod   ;
		rData.nStepCtrlOption    = nStepCtrlOption    ;
		rData.IncFuncK           = IncFuncK           ;
		rData.nDispCtrlOption    = nDispCtrlOption    ;
		rData.dOverallMaxDisp    = dOverallMaxDisp    ;
		rData.MasterNode         = MasterNode         ;
		rData.MasterDirection    = MasterDirection    ;
		rData.dMasterMaxDisp     = dMasterMaxDisp     ;
		rData.dStiffPercent      = dStiffPercent      ;
		rData.bLimitDeformAngle  = bLimitDeformAngle  ;
		rData.dIntStorDeformValue= dIntStorDeformValue;
		rData.nLoadPattern       = nLoadPattern;
		rData.bReacOutput        = bReacOutput;
		rData.bDispOutput        = bDispOutput;
		rData.bDriftMax          = bDriftMax   ;
		rData.bDriftCenter       = bDriftCenter;
		rData.bDriftAver         = bDriftAver  ;
		rData.bAnalStop                = bAnalStop               ; 
		rData.nAnalStepNum             = nAnalStepNum            ; 
		rData.dIntStorDeformValue_Cent = dIntStorDeformValue_Cent; 
		rData.dIntStorDeformValue_Aver = dIntStorDeformValue_Aver; 
		rData.nGeomNonlinearType = nGeomNonlinearType;
	}
	void SetPolc(const T_POLC_K Key, const T_POLC_D_CH& rData)
	{
		key=Key;
		LoadCaseId         = rData.LoadCaseId;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nStepNum           = rData.nStepNum           ;
		bUseInitial        = rData.bUseInitial        ;
		bConsiderPDelta    = rData.bConsiderPDelta    ;
		nIncrementMethod   = rData.nIncrementMethod   ;
		nStepCtrlOption    = rData.nStepCtrlOption    ;
		IncFuncK           = rData.IncFuncK           ;
		nDispCtrlOption    = rData.nDispCtrlOption    ;
		dOverallMaxDisp    = rData.dOverallMaxDisp    ;
		MasterNode         = rData.MasterNode         ;
		MasterDirection    = rData.MasterDirection    ;
		dMasterMaxDisp     = rData.dMasterMaxDisp     ;
		dStiffPercent      = rData.dStiffPercent      ;
		bLimitDeformAngle  = rData.bLimitDeformAngle  ;
		dIntStorDeformValue= rData.dIntStorDeformValue;
		nLoadPattern       = rData.nLoadPattern;
		nCountLoadPattern  = (int)rData.aLoadPattern.GetSize();
		bReacOutput        = rData.bReacOutput;
		bDispOutput        = rData.bDispOutput;
		bDriftMax          = rData.bDriftMax   ;
		bDriftCenter       = rData.bDriftCenter;
		bDriftAver         = rData.bDriftAver  ;
		bAnalStop                = rData.bAnalStop               ; 
		nAnalStepNum             = rData.nAnalStepNum            ; 
		dIntStorDeformValue_Cent = rData.dIntStorDeformValue_Cent; 
		dIntStorDeformValue_Aver = rData.dIntStorDeformValue_Aver; 
		nGeomNonlinearType = rData.nGeomNonlinearType;
	}
	void Convert790(T_POLC_UDRD_D_CH_RW_790& rData)
	{
		key                 = rData.key;
		LoadCaseId          = rData.LoadCaseId;
		memcpy(strName,         rData.strName,        sizeof(strName));
		memcpy(strDescription,  rData.strDescription, sizeof(strDescription));
		nStepNum            = rData.nStepNum           ;
		bUseInitial         = rData.bUseInitial        ;
		bConsiderPDelta     = rData.bConsiderPDelta    ;
		nIncrementMethod    = rData.nIncrementMethod   ;
		nStepCtrlOption     = rData.nStepCtrlOption    ;
		IncFuncK            = rData.IncFuncK           ;
		nDispCtrlOption     = rData.nDispCtrlOption    ;
		dOverallMaxDisp     = rData.dOverallMaxDisp    ;
		MasterNode          = rData.MasterNode         ;
		MasterDirection     = rData.MasterDirection    ;
		dMasterMaxDisp      = rData.dMasterMaxDisp     ;
		dStiffPercent       = rData.dStiffPercent      ;
		bLimitDeformAngle   = rData.bLimitDeformAngle  ;
		dIntStorDeformValue = rData.dIntStorDeformValue;
		nLoadPattern        = rData.nLoadPattern;
		nCountLoadPattern   = rData.nCountLoadPattern;
		bReacOutput         = rData.bReacOutput;
		bDispOutput         = rData.bDispOutput;
		bDriftMax           = rData.bLimitDeformAngle;
		bDriftCenter        = FALSE;
		bDriftAver          = FALSE;
		bAnalStop                = FALSE              ; 
		nAnalStepNum             = nStepNum           ; 
		dIntStorDeformValue_Cent = dIntStorDeformValue; 
		dIntStorDeformValue_Aver = dIntStorDeformValue; 
		nGeomNonlinearType = D_POLC_GEOMNONLIN_NONE;
	}
	void Convert785(T_POLC_UDRD_D_CH_RW_785& rData)
	{
		key                 = rData.key;
		LoadCaseId          = rData.LoadCaseId;
		memcpy(strName,         rData.strName,        sizeof(strName));
		memcpy(strDescription,  rData.strDescription, sizeof(strDescription));
		nStepNum            = rData.nStepNum           ;
		bUseInitial         = rData.bUseInitial        ;
		bConsiderPDelta     = rData.bConsiderPDelta    ;
		nIncrementMethod    = rData.nIncrementMethod   ;
		nStepCtrlOption     = rData.nStepCtrlOption    ;
		IncFuncK            = rData.IncFuncK           ;
		nDispCtrlOption     = rData.nDispCtrlOption    ;
		dOverallMaxDisp     = rData.dOverallMaxDisp    ;
		MasterNode          = rData.MasterNode         ;
		MasterDirection     = rData.MasterDirection    ;
		dMasterMaxDisp      = rData.dMasterMaxDisp     ;
		dStiffPercent       = rData.dStiffPercent      ;
		bLimitDeformAngle   = rData.bLimitDeformAngle  ;
		dIntStorDeformValue = rData.dIntStorDeformValue;
		nLoadPattern        = rData.nLoadPattern;
		nCountLoadPattern   = rData.nCountLoadPattern;
		bReacOutput         = FALSE;
		bDispOutput         = FALSE;
		bDriftMax           = rData.bLimitDeformAngle;
		bDriftCenter        = FALSE;
		bDriftAver          = FALSE;
		bAnalStop                = FALSE              ; 
		nAnalStepNum             = nStepNum           ; 
		dIntStorDeformValue_Cent = dIntStorDeformValue; 
		dIntStorDeformValue_Aver = dIntStorDeformValue; 
		nGeomNonlinearType = D_POLC_GEOMNONLIN_NONE;
	}
	void Converter960(T_POLC_UDRD_D_CH_RW_960& rData)
	{
		key					= rData.key;
		LoadCaseId          = rData.LoadCaseId;
		memcpy(strName,         rData.strName,        sizeof(strName));
		memcpy(strDescription,  rData.strDescription, sizeof(strDescription));
		nStepNum            = rData.nStepNum           ;
		bUseInitial         = rData.bUseInitial        ;
		bConsiderPDelta     = rData.bConsiderPDelta    ;
		nIncrementMethod    = rData.nIncrementMethod   ;
		nStepCtrlOption     = rData.nStepCtrlOption    ;
		IncFuncK            = rData.IncFuncK           ;
		nDispCtrlOption     = rData.nDispCtrlOption    ;
		dOverallMaxDisp     = rData.dOverallMaxDisp    ;
		MasterNode          = rData.MasterNode         ;
		MasterDirection     = rData.MasterDirection    ;
		dMasterMaxDisp      = rData.dMasterMaxDisp     ;
		dStiffPercent       = rData.dStiffPercent      ;
		bLimitDeformAngle   = rData.bLimitDeformAngle  ;
		dIntStorDeformValue = rData.dIntStorDeformValue;
		nLoadPattern        = rData.nLoadPattern;
		nCountLoadPattern   = rData.nCountLoadPattern;
		bReacOutput         = rData.bReacOutput;
		bDispOutput         = rData.bDispOutput;
		bDriftMax           = rData.bDriftMax   ;
		bDriftCenter        = rData.bDriftCenter;
		bDriftAver          = rData.bDriftAver  ;
		bAnalStop                = rData.bAnalStop               ;
		nAnalStepNum             = rData.nAnalStepNum            ;
		dIntStorDeformValue_Cent = rData.dIntStorDeformValue_Cent;
		dIntStorDeformValue_Aver = rData.dIntStorDeformValue_Aver;
		nGeomNonlinearType		 = D_POLC_GEOMNONLIN_NONE;
	}
};
struct T_POLC_UNIT
{
	T_POLC_UNIT()
	{
		dOverallMaxDisp = D_UNITSYS_BASE_LENGTH;
		dMasterMaxDisp  = D_UNITSYS_BASE_LENGTH;
	}
	int dOverallMaxDisp;
	int dMasterMaxDisp;
};
#define HASHSIZEPOLC 11
#define D_POLC_MINNUM       1   // 입력가능한 최소 하중조건번호
#define D_POLC_MAXNUM    9999   // 입력가능한 최대 하중조건번호

struct T_PVLC_BASE
{
	// T_POGD_D에서 사용
	UINT  LoadCaseType;   // D_LOADCASE_STATIC, D_LOADCASE_STAGE
	T_KEY LoadCaseKey;    // nLoadPatternType에 따라 Static Load Case Key,
												// Acceleration(1=X, 2=Y, 3=Z), Mode Shape No
	double dScaleFactor;
	void Initialize()
	{
		LoadCaseType = D_LOADCASE_STATIC;
		LoadCaseKey  = 0;
		dScaleFactor = 1.0;
	}

	bool operator==(const T_PVLC_BASE& src) const
	{
		if (this == &src) return true;
		if (dScaleFactor != src.dScaleFactor) return false;
		if (LoadCaseKey != src.LoadCaseKey) return false;
		if (LoadCaseType != src.LoadCaseType) return false;
		return true;
	}

	bool operator!=(const T_PVLC_BASE& src) const { return !(*this == src); }

};

#define T_POGD_K unsigned int
enum { D_POGD_FSIZE_AUTO, D_POGD_FSIZE_EQUAL };
enum { D_POGD_WSIZE_AUTO, D_POGD_WSIZE_EQUAL };
enum D_POGD_GEOMNONLIN { D_POGD_GEOMNONLIN_NONE, D_POGD_GEOMNONLIN_LARGE_DISP, D_POGD_GEOMNONLIN_P_DELTA };

struct T_POGD_D                   // Pushover Global Data --------------------------
{  
	// Initial Load
	CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad;  // MNET:4103-SHJUNG-20100719 : 사용 안함.
	// Analysis Control
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	// Design Option    // JP에서만 쓸수도 있으므로 Initialize 등이 변경될 수도 있다.
	BOOL  bCalcCrackMoment;
	// Stiffness Reduction Ratio
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;        // 0:I, 1:J, 2:M  
	int    nLocOfColumn;      // 0:I, 1:J
	// Reference Design Code Data (v740 추가)
	BOOL   bConsiderAxialForce;       // AIJ
	double dConfidenceFactor;         // Eurocode
	double dScaleFactorWall;          // Eurocode
	BOOL   bScaleFactorBrittle_Use;   // Eurocode
	double dScaleFactorBrittle;       // Eurocode
	BOOL   bScaleFactorEarth_Use;     // Eurocode
	double dScaleFactorEarth;         // Eurocode
	BOOL   bScaleFactorSmoothBar_Use; // Eurocode
	double dScaleFactorSmoothBar;     // Eurocode
	T_GRUP_K SecondaryGroupKey;       // Eurocode
	BOOL   bPermitFail; // 741
	int    nSubstep;    // 741
	BOOL   bBuckling;   // 760
	
	BOOL bShearYieldStop;       // 780
	BOOL bShearYieldStopBeam;
	BOOL bShearYieldStopWall;

	int   nInitLoadMethod;                        // 790   
	CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad;  // 790. MNET:4103-SHJUNG-20100719

	BOOL bAxialYieldStop     ;  // 791
	BOOL bAxialYieldStopBeam ;
	BOOL bAxialYieldStopWall ;
	BOOL bAxialYieldStopTruss;

	int  nPointSpringSupport;   // 805 // MNET:XXXX-JWKWON-20120413  
	BOOL bSupportDzDirStop;     // 805
	BOOL bSupportStopUplifting;
	BOOL bSupportStopCollapse;
	int  nNodeConnectivity;     // 0: Pinned 1: Fixed
	int   nGeomNonlinearType;   // 0 : None, 1 : Large Displacements 2:P-delta
	int    nBeamDivNumNy; // Pushover Fiber Model Option (from v865)
	int    nBeamDivNumNz;
	BOOL   bWallConsOut ;
	int    nWallDivNumZ ;
	int    nWallDivNumY ;
	double dR           ;
	int    nWAreaSize   ; // Fiber Wall Areas Core, D_POGD_FSIZE
	BOOL   bAssignByMember    ;
	int    nRCDgnCode         ; // D_SEME_METHOD
	BOOL   bUseAutoCalcReference;     // Reference Code/Manual for Auto-Calculation
	BOOL   bShowPOCurveAfterAnalysis; // show PO Curve Dialog when PO analysis is finished
	BOOL   bShowPOAnalGraph         ; // show Real-time PO graph during PO analysis
	int    nElasticLink; // 875 
	BOOL   bConsiderIgnoreElem; // 881
	BOOL   bConsiderRebarArea1D; // Beam-Column, Consider Rebar Area
	BOOL   bConsiderRebarAreaWall; // Wall, Consider Rebar Area
	int    nFAreaSizeCore; // Fiber Beam Areas Core, D_POGD_FSIZE
	int    nFAreaSizeCover; // Fiber Beam Areas Cover, D_POGD_FSIZE
	int    nWAreaSizeCover; // Fiber Wall Areas Cover, D_POGD_FSIZE
	int    nBeamDivNumNyCover; // Number of divisions(Beam-Column) - Ny, Cover
	int    nBeamDivNumNzCover; // Number of divisions(Beam-Column) - Nz, Cover
	int    nWallDivNumZCover ; // Number of divisions - z, Cover
	int    nWallDivNumYCover ; // Number of divisions - y, Cover
	BOOL bConvertWallStiffness; // v945 : GEN 7660 Wall Stiffness Scale Factor Convert

	T_NTIS_BOUNDARY BoundaryNL;
	BOOL   bConsiderPDelta;  // 사용안함.

	T_POGD_D() { Initialize(); }
	
	void Initialize()
	{
		aOldInitLoad.RemoveAll();    
		nMaxIterNum = 10;
		bDisplNorm = TRUE;
		bForceNorm = bEnergyNorm = FALSE;
		dDisplNorm = dForceNorm = dEnergyNorm = 0.001;
		bCalcCrackMoment = TRUE;
		bTri_Symmetric = TRUE;
#if defined(_CH)
		dTri_Tens_a1   = 0.5;
#else
		dTri_Tens_a1   = 0.1;
#endif
		dTri_Tens_a2   = 0.05;
		dTri_Comp_a1   = 0.1;
		dTri_Comp_a2   = 0.05;
		bBi_Symmetric  = TRUE;
		dBi_Tens_a1    = 0.05;
		dBi_Comp_a1    = 0.05;
		nLocOfBeam     = 0;
		nLocOfColumn   = 0;

		bConsiderAxialForce = TRUE; // v740
		dConfidenceFactor = 1.0;
		dScaleFactorWall = 1.6;
		bScaleFactorBrittle_Use = FALSE;
		dScaleFactorBrittle = 1.6;
		bScaleFactorEarth_Use = FALSE;
		dScaleFactorEarth = 0.85;
		bScaleFactorSmoothBar_Use = FALSE;
		dScaleFactorSmoothBar = 0.575;
		SecondaryGroupKey = 0;

		bPermitFail = TRUE; // v741
		nSubstep = 10;
		bBuckling = FALSE;

		bShearYieldStop = FALSE;
		bShearYieldStopBeam = TRUE;
		bShearYieldStopWall = FALSE;

		nInitLoadMethod = 0;   
		aInitLoad.RemoveAll();

		bAxialYieldStop      = FALSE;  
		bAxialYieldStopBeam  = TRUE ;
		bAxialYieldStopWall  = FALSE;
		bAxialYieldStopTruss = FALSE;
		nPointSpringSupport  = 0;
		bSupportDzDirStop    = FALSE;
		bSupportStopUplifting= FALSE;
		bSupportStopCollapse = FALSE;
		nNodeConnectivity    = 1;     // 20141105 shjung(By 김종민 부장님) 솔버내부에서 'Fixed"로 처리되었기 때문에 디폴트를 1로 설정
		nGeomNonlinearType   = D_POGD_GEOMNONLIN_NONE;
		nBeamDivNumNy = 15;
		nBeamDivNumNz = 15;
		bWallConsOut  = FALSE;
		nWallDivNumZ  = 8;
		nWallDivNumY  = 1;
		dR            = 0.4; 
		nWAreaSize    = D_POGD_WSIZE_AUTO;
		bAssignByMember = TRUE;
		nRCDgnCode      = 0;
		bUseAutoCalcReference = FALSE;
#if defined(_MGEN_JP)
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = FALSE;
#elif defined(_MGEN)
		bShowPOAnalGraph = TRUE;
		bShowPOCurveAfterAnalysis = TRUE;
#else
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = TRUE;
#endif

		nElasticLink = 0;
		bConsiderIgnoreElem=TRUE;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
		bConvertWallStiffness = TRUE;

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}

	bool operator==(const T_POGD_D& src) const
	{
		if (this == &src) return true;

		if (nMaxIterNum != src.nMaxIterNum) return false;
		if (bDisplNorm != src.bDisplNorm) return false;
		if (bForceNorm != src.bForceNorm) return false;
		if (bEnergyNorm != src.bEnergyNorm) return false;
		if (dDisplNorm != src.dDisplNorm) return false;
		if (dForceNorm != src.dForceNorm) return false;
		if (dEnergyNorm != src.dEnergyNorm) return false;
		if (bCalcCrackMoment != src.bCalcCrackMoment) return false;
		if (bTri_Symmetric != src.bTri_Symmetric) return false;
		if (dTri_Tens_a1 != src.dTri_Tens_a1) return false;
		if (dTri_Tens_a2 != src.dTri_Tens_a2) return false;
		if (dTri_Comp_a1 != src.dTri_Comp_a1) return false;
		if (dTri_Comp_a2 != src.dTri_Comp_a2) return false;
		if (bBi_Symmetric != src.bBi_Symmetric) return false;
		if (dBi_Tens_a1 != src.dBi_Tens_a1) return false;
		if (dBi_Comp_a1 != src.dBi_Comp_a1) return false;
		if (nLocOfBeam != src.nLocOfBeam) return false;
		if (nLocOfColumn != src.nLocOfColumn) return false;

		if (bConsiderAxialForce != src.bConsiderAxialForce) return false;
		if (dConfidenceFactor != src.dConfidenceFactor) return false;
		if (dScaleFactorWall != src.dScaleFactorWall) return false;
		if (bScaleFactorBrittle_Use != src.bScaleFactorBrittle_Use) return false;
		if (dScaleFactorBrittle != src.dScaleFactorBrittle) return false;
		if (bScaleFactorEarth_Use != src.bScaleFactorEarth_Use) return false;
		if (dScaleFactorEarth != src.dScaleFactorEarth) return false;
		if (bScaleFactorSmoothBar_Use != src.bScaleFactorSmoothBar_Use) return false;
		if (dScaleFactorSmoothBar != src.dScaleFactorSmoothBar) return false;
		if (SecondaryGroupKey != src.SecondaryGroupKey) return false;

		if (bPermitFail != src.bPermitFail) return false;
		if (nSubstep != src.nSubstep) return false;
		if (bBuckling != src.bBuckling) return false;

		if (bShearYieldStop != src.bShearYieldStop) return false;
		if (bShearYieldStopBeam != src.bShearYieldStopBeam) return false;
		if (bShearYieldStopWall != src.bShearYieldStopWall) return false;

		if (nInitLoadMethod != src.nInitLoadMethod) return false;

		if (bAxialYieldStop != src.bAxialYieldStop) return false;
		if (bAxialYieldStopBeam != src.bAxialYieldStopBeam) return false;
		if (bAxialYieldStopWall != src.bAxialYieldStopWall) return false;
		if (bAxialYieldStopTruss != src.bAxialYieldStopTruss) return false;
		if (nPointSpringSupport != src.nPointSpringSupport) return false;
		if (bSupportDzDirStop != src.bSupportDzDirStop) return false;
		if (bSupportStopUplifting != src.bSupportStopUplifting) return false;
		if (bSupportStopCollapse != src.bSupportStopCollapse) return false;
		if (nNodeConnectivity != src.nNodeConnectivity) return false;
		if (nGeomNonlinearType != src.nGeomNonlinearType) return false;
		if (nBeamDivNumNy != src.nBeamDivNumNy) return false;
		if (nBeamDivNumNz != src.nBeamDivNumNz) return false;
		if (bWallConsOut != src.bWallConsOut) return false;
		if (nWallDivNumZ != src.nWallDivNumZ) return false;
		if (nWallDivNumY != src.nWallDivNumY) return false;
		if (dR != src.dR) return false;
		if (nWAreaSize != src.nWAreaSize) return false;
		if (bAssignByMember != src.bAssignByMember) return false;
		if (nRCDgnCode != src.nRCDgnCode) return false;
		if (bUseAutoCalcReference != src.bUseAutoCalcReference) return false;
		if (bShowPOCurveAfterAnalysis != src.bShowPOCurveAfterAnalysis) return false;
		if (bShowPOAnalGraph != src.bShowPOAnalGraph) return false;
		if (nElasticLink != src.nElasticLink) return false;
		if (bConsiderIgnoreElem != src.bConsiderIgnoreElem) return false;
		if (bConsiderRebarArea1D != src.bConsiderRebarArea1D) return false;
		if (bConsiderRebarAreaWall != src.bConsiderRebarAreaWall) return false;
		if (nFAreaSizeCore != src.nFAreaSizeCore) return false;
		if (nFAreaSizeCover != src.nFAreaSizeCover) return false;
		if (nWAreaSizeCover != src.nWAreaSizeCover) return false;
		if (nBeamDivNumNyCover != src.nBeamDivNumNyCover) return false;
		if (nBeamDivNumNzCover != src.nBeamDivNumNzCover) return false;
		if (nWallDivNumZCover != src.nWallDivNumZCover) return false;
		if (nWallDivNumYCover != src.nWallDivNumYCover) return false;
		if (bConvertWallStiffness != src.bConvertWallStiffness) return false;

		if (BoundaryNL != src.BoundaryNL) return false;
		if (bConsiderPDelta != src.bConsiderPDelta) return false;


		if (aOldInitLoad.GetSize() != src.aOldInitLoad.GetSize()) {
			return false;
		}
		else
		{
			int nDataTgt = aOldInitLoad.GetSize();
			int nDataSrc = src.aOldInitLoad.GetSize();
			int i, j;
			for (i = 0; i < nDataTgt; ++i)
			{
				for (j = 0; j < nDataSrc; ++j)
				{
					if (aOldInitLoad[i] == src.aOldInitLoad[j]) break;
				}
				if (j == nDataSrc) return false;
			}
		}
		if (aInitLoad.GetSize() != src.aInitLoad.GetSize()) {
			return false;
		}
		else
		{
			int nDataTgt = aInitLoad.GetSize();
			int nDataSrc = src.aInitLoad.GetSize();
			int i, j;
			for (i = 0; i < nDataTgt; ++i)
			{
				for (j = 0; j < nDataSrc; ++j)
				{
					if (aInitLoad[i] == src.aInitLoad[j]) break;
				}
				if (j == nDataSrc) return false;
			}
		}

		return true;
	}

	bool operator!=(const T_POGD_D& src) const { return !(*this == src); }

	T_POGD_D& operator=(T_POGD_D& src)
	{
		aOldInitLoad.Copy(src.aOldInitLoad);      
		nMaxIterNum = src.nMaxIterNum;
		bDisplNorm  = src.bDisplNorm ;
		bForceNorm  = src.bForceNorm ;
		bEnergyNorm = src.bEnergyNorm;
		dDisplNorm  = src.dDisplNorm ;
		dForceNorm  = src.dForceNorm ;
		dEnergyNorm = src.dEnergyNorm;
		bCalcCrackMoment = src.bCalcCrackMoment;
		bTri_Symmetric = src.bTri_Symmetric;
		dTri_Tens_a1   = src.dTri_Tens_a1  ;
		dTri_Tens_a2   = src.dTri_Tens_a2  ;
		dTri_Comp_a1   = src.dTri_Comp_a1  ;
		dTri_Comp_a2   = src.dTri_Comp_a2  ;
		bBi_Symmetric  = src.bBi_Symmetric ;
		dBi_Tens_a1    = src.dBi_Tens_a1   ;
		dBi_Comp_a1    = src.dBi_Comp_a1   ;
		nLocOfBeam     = src.nLocOfBeam;  
		nLocOfColumn   = src.nLocOfColumn;

		bConsiderAxialForce       = src.bConsiderAxialForce;
		dConfidenceFactor         = src.dConfidenceFactor;
		dScaleFactorWall          = src.dScaleFactorWall;
		bScaleFactorBrittle_Use   = src.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = src.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = src.bScaleFactorEarth_Use;
		dScaleFactorEarth         = src.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = src.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = src.dScaleFactorSmoothBar;
		SecondaryGroupKey         = src.SecondaryGroupKey;

		bPermitFail = src.bPermitFail;
		nSubstep = src.nSubstep;
		bBuckling = src.bBuckling;

		bShearYieldStop = src.bShearYieldStop;
		bShearYieldStopBeam = src.bShearYieldStopBeam;
		bShearYieldStopWall = src.bShearYieldStopWall;

		nInitLoadMethod = src.nInitLoadMethod;   
		aInitLoad.Copy(src.aInitLoad);

		bAxialYieldStop      = src.bAxialYieldStop     ;
		bAxialYieldStopBeam  = src.bAxialYieldStopBeam ;
		bAxialYieldStopWall  = src.bAxialYieldStopWall ;
		bAxialYieldStopTruss = src.bAxialYieldStopTruss;
		nPointSpringSupport  = src.nPointSpringSupport;
		bSupportDzDirStop    = src.bSupportDzDirStop;
		bSupportStopUplifting= src.bSupportStopUplifting;
		bSupportStopCollapse = src.bSupportStopCollapse;
		nNodeConnectivity    = src.nNodeConnectivity;
		nGeomNonlinearType   = src.nGeomNonlinearType;
		nBeamDivNumNy = src.nBeamDivNumNy;
		nBeamDivNumNz = src.nBeamDivNumNz;
		bWallConsOut  = src.bWallConsOut ;
		nWallDivNumZ  = src.nWallDivNumZ ;
		nWallDivNumY  = src.nWallDivNumY ;
		dR            = src.dR           ;
		nWAreaSize    = src.nWAreaSize   ;
		bAssignByMember = src.bAssignByMember;
		nRCDgnCode      = src.nRCDgnCode     ;
		bUseAutoCalcReference   = src.bUseAutoCalcReference  ;
		bShowPOCurveAfterAnalysis = src.bShowPOCurveAfterAnalysis;
		bShowPOAnalGraph          = src.bShowPOAnalGraph         ;
		nElasticLink = src.nElasticLink;
		bConsiderIgnoreElem = src.bConsiderIgnoreElem;
		bConsiderRebarArea1D = src.bConsiderRebarArea1D;
		bConsiderRebarAreaWall = src.bConsiderRebarAreaWall;
		nFAreaSizeCore  = src.nFAreaSizeCore ;
		nFAreaSizeCover = src.nFAreaSizeCover;
		nWAreaSizeCover = src.nWAreaSizeCover;
		nBeamDivNumNyCover = src.nBeamDivNumNyCover;
		nBeamDivNumNzCover = src.nBeamDivNumNzCover;
		nWallDivNumZCover  = src.nWallDivNumZCover;
		nWallDivNumYCover  = src.nWallDivNumYCover;
		bConvertWallStiffness = src.bConvertWallStiffness;

		BoundaryNL = src.BoundaryNL;
		bConsiderPDelta = src.bConsiderPDelta;
		return *this;
	}
};
struct T_POGD_UDRD_D
{
	T_POGD_D data;
};
struct T_POGD_UDRD_D_RW_730
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;
	void SetPogd(T_POGD_D& rData)
	{
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = (int)rData.aOldInitLoad.GetSize();
	}
};
struct T_POGD_UDRD_D_RW_740
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;
	BOOL   bConsiderAxialForce; // 740
	double dConfidenceFactor;
	double dScaleFactorWall;
	BOOL   bScaleFactorBrittle_Use;
	double dScaleFactorBrittle;
	BOOL   bScaleFactorEarth_Use;
	double dScaleFactorEarth;
	BOOL   bScaleFactorSmoothBar_Use;
	double dScaleFactorSmoothBar;
	T_GRUP_K SecondaryGroupKey;
};
struct T_POGD_UDRD_D_RW_750
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int    nMaxIterNum;
	BOOL   bDisplNorm;
	BOOL   bForceNorm;
	BOOL   bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL   bCalcCrackMoment;
	BOOL   bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;
	BOOL   bConsiderAxialForce; // 740
	double dConfidenceFactor;
	double dScaleFactorWall;
	BOOL   bScaleFactorBrittle_Use;
	double dScaleFactorBrittle;
	BOOL   bScaleFactorEarth_Use;
	double dScaleFactorEarth;
	BOOL   bScaleFactorSmoothBar_Use;
	double dScaleFactorSmoothBar;
	T_GRUP_K SecondaryGroupKey;
	BOOL   bPermitFail; // 741
	int    nSubstep;    // 741
};
struct T_POGD_UDRD_D_RW_770
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int    nMaxIterNum;
	BOOL   bDisplNorm;
	BOOL   bForceNorm;
	BOOL   bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL   bCalcCrackMoment;
	BOOL   bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;
	BOOL   bConsiderAxialForce; // 740
	double dConfidenceFactor;
	double dScaleFactorWall;
	BOOL   bScaleFactorBrittle_Use;
	double dScaleFactorBrittle;
	BOOL   bScaleFactorEarth_Use;
	double dScaleFactorEarth;
	BOOL   bScaleFactorSmoothBar_Use;
	double dScaleFactorSmoothBar;
	T_GRUP_K SecondaryGroupKey;
	BOOL   bPermitFail; // 741
	int    nSubstep;    // 741
	BOOL   bBuckling;   // 760
};

struct T_POGD_UDRD_D_RW_785
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;
	
	// Reference Design Code Data (v740 추가)
	BOOL   bConsiderAxialForce;       // AIJ
	double dConfidenceFactor;         // Eurocode
	double dScaleFactorWall;          // Eurocode
	BOOL   bScaleFactorBrittle_Use;   // Eurocode
	double dScaleFactorBrittle;       // Eurocode
	BOOL   bScaleFactorEarth_Use;     // Eurocode
	double dScaleFactorEarth;         // Eurocode
	BOOL   bScaleFactorSmoothBar_Use; // Eurocode
	double dScaleFactorSmoothBar;     // Eurocode
	T_GRUP_K SecondaryGroupKey;       // Eurocode
	
	BOOL    bPermitFail; // 741
	int     nSubstep;    // 741
	BOOL    bBuckling;   // 760

	BOOL    bShearYieldStop;       // 780
	BOOL    bShearYieldStopBeam;
	BOOL    bShearYieldStopWall;
};
struct T_POGD_UDRD_D_RW_790
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;
	
	// Reference Design Code Data (v740 추가)
	BOOL   bConsiderAxialForce;       // AIJ
	double dConfidenceFactor;         // Eurocode
	double dScaleFactorWall;          // Eurocode
	BOOL   bScaleFactorBrittle_Use;   // Eurocode
	double dScaleFactorBrittle;       // Eurocode
	BOOL   bScaleFactorEarth_Use;     // Eurocode
	double dScaleFactorEarth;         // Eurocode
	BOOL   bScaleFactorSmoothBar_Use; // Eurocode
	double dScaleFactorSmoothBar;     // Eurocode
	T_GRUP_K SecondaryGroupKey;       // Eurocode
	
	BOOL    bPermitFail; // 741
	int     nSubstep;    // 741
	BOOL    bBuckling;   // 760
	
	BOOL    bShearYieldStop;       // 780
	BOOL    bShearYieldStopBeam;
	BOOL    bShearYieldStopWall;

	int     nInitLoadMethod;                      // 790
	char aInitLoad[20];  // CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad;  // 790. MNET:4103-SHJUNG-20100719 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int     nCountInitLoad;                       // 790
};

struct T_POGD_UDRD_D_RW_800
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;
	
	// Reference Design Code Data (v740 추가)
	BOOL   bConsiderAxialForce;       // AIJ
	double dConfidenceFactor;         // Eurocode
	double dScaleFactorWall;          // Eurocode
	BOOL   bScaleFactorBrittle_Use;   // Eurocode
	double dScaleFactorBrittle;       // Eurocode
	BOOL   bScaleFactorEarth_Use;     // Eurocode
	double dScaleFactorEarth;         // Eurocode
	BOOL   bScaleFactorSmoothBar_Use; // Eurocode
	double dScaleFactorSmoothBar;     // Eurocode
	T_GRUP_K SecondaryGroupKey;       // Eurocode
	
	BOOL    bPermitFail; // 741
	int     nSubstep;    // 741
	BOOL    bBuckling;   // 760
	
	BOOL    bShearYieldStop;       // 780
	BOOL    bShearYieldStopBeam;
	BOOL    bShearYieldStopWall;
	
	int     nInitLoadMethod;                      // 790
	char aInitLoad[20];  // CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad;  // 790. MNET:4103-SHJUNG-20100719 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int     nCountInitLoad;                       // 790

	BOOL bAxialYieldStop     ;  // 791  
	BOOL bAxialYieldStopBeam ;
	BOOL bAxialYieldStopWall ;
	BOOL bAxialYieldStopTruss;

};

struct T_POGD_UDRD_D_RW_820
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;

	// Reference Design Code Data (v740 추가)
	BOOL   bConsiderAxialForce;       // AIJ
	double dConfidenceFactor;         // Eurocode
	double dScaleFactorWall;          // Eurocode
	BOOL   bScaleFactorBrittle_Use;   // Eurocode
	double dScaleFactorBrittle;       // Eurocode
	BOOL   bScaleFactorEarth_Use;     // Eurocode
	double dScaleFactorEarth;         // Eurocode
	BOOL   bScaleFactorSmoothBar_Use; // Eurocode
	double dScaleFactorSmoothBar;     // Eurocode
	T_GRUP_K SecondaryGroupKey;       // Eurocode

	BOOL   bPermitFail; // 741
	int    nSubstep;    // 741
	BOOL   bBuckling;   // 760
	BOOL bShearYieldStop;       // 780
	BOOL bShearYieldStopBeam;
	BOOL bShearYieldStopWall;

	int nInitLoadMethod;                         // 790  
	char aInitLoad[20];	// CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad; // 790 // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int nCountInitLoad;                          // 790
	
	BOOL bAxialYieldStop     ;  // 791  
	BOOL bAxialYieldStopBeam ;
	BOOL bAxialYieldStopWall ;
	BOOL bAxialYieldStopTruss;
	
	int  nPointSpringSupport;   // 805
	BOOL bSupportDzDirStop;
	BOOL bSupportStopUplifting;
	BOOL bSupportStopCollapse;
	
};

struct T_POGD_UDRD_D_RW_841
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;

	// Reference Design Code Data (v740 추가)
	BOOL   bConsiderAxialForce;       // AIJ
	double dConfidenceFactor;         // Eurocode
	double dScaleFactorWall;          // Eurocode
	BOOL   bScaleFactorBrittle_Use;   // Eurocode
	double dScaleFactorBrittle;       // Eurocode
	BOOL   bScaleFactorEarth_Use;     // Eurocode
	double dScaleFactorEarth;         // Eurocode
	BOOL   bScaleFactorSmoothBar_Use; // Eurocode
	double dScaleFactorSmoothBar;     // Eurocode
	T_GRUP_K SecondaryGroupKey;       // Eurocode

	BOOL   bPermitFail; // 741
	int    nSubstep;    // 741
	BOOL   bBuckling;   // 760
	BOOL bShearYieldStop;       // 780
	BOOL bShearYieldStopBeam;
	BOOL bShearYieldStopWall;

	int nInitLoadMethod;                         // 790  
	char aInitLoad[20];	// CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad; // 790 // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)  
	int nCountInitLoad;                          // 790

	BOOL bAxialYieldStop     ;  // 791  
	BOOL bAxialYieldStopBeam ;
	BOOL bAxialYieldStopWall ;
	BOOL bAxialYieldStopTruss;

	int  nPointSpringSupport;   // 805
	BOOL bSupportDzDirStop;
	BOOL bSupportStopUplifting;
	BOOL bSupportStopCollapse;

	int  nNodeConnectivity;    // 820
};

struct T_POGD_UDRD_D_RW_861
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;

	// Reference Design Code Data (v740 추가)
	BOOL   bConsiderAxialForce;       // AIJ
	double dConfidenceFactor;         // Eurocode
	double dScaleFactorWall;          // Eurocode
	BOOL   bScaleFactorBrittle_Use;   // Eurocode
	double dScaleFactorBrittle;       // Eurocode
	BOOL   bScaleFactorEarth_Use;     // Eurocode
	double dScaleFactorEarth;         // Eurocode
	BOOL   bScaleFactorSmoothBar_Use; // Eurocode
	double dScaleFactorSmoothBar;     // Eurocode
	T_GRUP_K SecondaryGroupKey;       // Eurocode

	BOOL   bPermitFail; // 741
	int    nSubstep;    // 741
	BOOL   bBuckling;   // 760
	BOOL bShearYieldStop;       // 780
	BOOL bShearYieldStopBeam;
	BOOL bShearYieldStopWall;

	int nInitLoadMethod;                         // 790  
	char aInitLoad[20];	// CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad; // 790 // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)  
	int nCountInitLoad;                          // 790

	BOOL bAxialYieldStop     ;  // 791  
	BOOL bAxialYieldStopBeam ;
	BOOL bAxialYieldStopWall ;
	BOOL bAxialYieldStopTruss;

	int  nPointSpringSupport;   // 805
	BOOL bSupportDzDirStop;
	BOOL bSupportStopUplifting;
	BOOL bSupportStopCollapse;

	int  nNodeConnectivity;    // 820

	int   nGeomNonlinearType;   // 842
};

struct T_POGD_UDRD_D_RW_865
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;

	// Reference Design Code Data (v740 추가)
	BOOL   bConsiderAxialForce;       // AIJ
	double dConfidenceFactor;         // Eurocode
	double dScaleFactorWall;          // Eurocode
	BOOL   bScaleFactorBrittle_Use;   // Eurocode
	double dScaleFactorBrittle;       // Eurocode
	BOOL   bScaleFactorEarth_Use;     // Eurocode
	double dScaleFactorEarth;         // Eurocode
	BOOL   bScaleFactorSmoothBar_Use; // Eurocode
	double dScaleFactorSmoothBar;     // Eurocode
	T_GRUP_K SecondaryGroupKey;       // Eurocode

	BOOL   bPermitFail; // 741
	int    nSubstep;    // 741
	BOOL   bBuckling;   // 760
	BOOL bShearYieldStop;       // 780
	BOOL bShearYieldStopBeam;
	BOOL bShearYieldStopWall;

	int nInitLoadMethod;                         // 790  
	char aInitLoad[20];	// CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad; // 790 // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)  
	int nCountInitLoad;                          // 790

	BOOL bAxialYieldStop     ;  // 791  
	BOOL bAxialYieldStopBeam ;
	BOOL bAxialYieldStopWall ;
	BOOL bAxialYieldStopTruss;

	int  nPointSpringSupport;   // 805
	BOOL bSupportDzDirStop;
	BOOL bSupportStopUplifting;
	BOOL bSupportStopCollapse;

	int  nNodeConnectivity;    // 820

	int    nGeomNonlinearType ;// 842
	int    nBeamDivNumNy      ;// 865
	int    nBeamDivNumNz      ; 
	BOOL   bWallConsOut       ; 
	int    nWallDivNumZ       ; 
	int    nWallDivNumY       ; 
	double dR                 ; 
};

struct T_POGD_UDRD_D_RW_870minor
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;

	// Reference Design Code Data (v740 추가)
	BOOL   bConsiderAxialForce;       // AIJ
	double dConfidenceFactor;         // Eurocode
	double dScaleFactorWall;          // Eurocode
	BOOL   bScaleFactorBrittle_Use;   // Eurocode
	double dScaleFactorBrittle;       // Eurocode
	BOOL   bScaleFactorEarth_Use;     // Eurocode
	double dScaleFactorEarth;         // Eurocode
	BOOL   bScaleFactorSmoothBar_Use; // Eurocode
	double dScaleFactorSmoothBar;     // Eurocode
	T_GRUP_K SecondaryGroupKey;       // Eurocode

	BOOL   bPermitFail; // 741
	int    nSubstep;    // 741
	BOOL   bBuckling;   // 760
	BOOL bShearYieldStop;       // 780
	BOOL bShearYieldStopBeam;
	BOOL bShearYieldStopWall;

	int nInitLoadMethod;                         // 790  
	char aInitLoad[20];	// CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad; // 790 // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)  
	int nCountInitLoad;                          // 790

	BOOL bAxialYieldStop     ;  // 791  
	BOOL bAxialYieldStopBeam ;
	BOOL bAxialYieldStopWall ;
	BOOL bAxialYieldStopTruss;

	int  nPointSpringSupport;   // 805
	BOOL bSupportDzDirStop;
	BOOL bSupportStopUplifting;
	BOOL bSupportStopCollapse;

	int  nNodeConnectivity;    // 820

	int    nGeomNonlinearType ;// 842
	int    nBeamDivNumNy      ;// 865
	int    nBeamDivNumNz      ; 
	BOOL   bWallConsOut       ; 
	int    nWallDivNumZ       ; 
	int    nWallDivNumY       ; 
	double dR                 ; 
	int    nWAreaSize         ; // 870
	BOOL   bAssignByMember    ;
	int    nRCDgnCode         ;
	int    bUseAutoCalcReference;
};

struct T_POGD_UDRD_D_RW_871
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;

	// Reference Design Code Data (v740 추가)
	BOOL   bConsiderAxialForce;       // AIJ
	double dConfidenceFactor;         // Eurocode
	double dScaleFactorWall;          // Eurocode
	BOOL   bScaleFactorBrittle_Use;   // Eurocode
	double dScaleFactorBrittle;       // Eurocode
	BOOL   bScaleFactorEarth_Use;     // Eurocode
	double dScaleFactorEarth;         // Eurocode
	BOOL   bScaleFactorSmoothBar_Use; // Eurocode
	double dScaleFactorSmoothBar;     // Eurocode
	T_GRUP_K SecondaryGroupKey;       // Eurocode

	BOOL   bPermitFail; // 741
	int    nSubstep;    // 741
	BOOL   bBuckling;   // 760
	BOOL bShearYieldStop;       // 780
	BOOL bShearYieldStopBeam;
	BOOL bShearYieldStopWall;

	int nInitLoadMethod;                         // 790  
	char aInitLoad[20];	// CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad; // 790 // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)  
	int nCountInitLoad;                          // 790

	BOOL bAxialYieldStop     ;  // 791  
	BOOL bAxialYieldStopBeam ;
	BOOL bAxialYieldStopWall ;
	BOOL bAxialYieldStopTruss;

	int  nPointSpringSupport;   // 805
	BOOL bSupportDzDirStop;
	BOOL bSupportStopUplifting;
	BOOL bSupportStopCollapse;

	int  nNodeConnectivity;    // 820

	int    nGeomNonlinearType ;// 842
	int    nBeamDivNumNy      ;// 865
	int    nBeamDivNumNz      ; 
	BOOL   bWallConsOut       ; 
	int    nWallDivNumZ       ; 
	int    nWallDivNumY       ; 
	double dR                 ; 
	int    nWAreaSize         ; // 870
	BOOL   bAssignByMember    ;
	int    nRCDgnCode         ;
	int    bUseAutoCalcReference      ;
	BOOL   bShowPOCurveAfterAnalysis; // show PO Curve Dialog when PO analysis is finished
	BOOL   bShowPOAnalGraph;          // show Real-time PO graph during PO analysis
};

struct T_POGD_UDRD_D_RW_880
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;

	// Reference Design Code Data (v740 추가)
	BOOL   bConsiderAxialForce;       // AIJ
	double dConfidenceFactor;         // Eurocode
	double dScaleFactorWall;          // Eurocode
	BOOL   bScaleFactorBrittle_Use;   // Eurocode
	double dScaleFactorBrittle;       // Eurocode
	BOOL   bScaleFactorEarth_Use;     // Eurocode
	double dScaleFactorEarth;         // Eurocode
	BOOL   bScaleFactorSmoothBar_Use; // Eurocode
	double dScaleFactorSmoothBar;     // Eurocode
	T_GRUP_K SecondaryGroupKey;       // Eurocode

	BOOL   bPermitFail; // 741
	int    nSubstep;    // 741
	BOOL   bBuckling;   // 760
	BOOL bShearYieldStop;       // 780
	BOOL bShearYieldStopBeam;
	BOOL bShearYieldStopWall;

	int nInitLoadMethod;                         // 790  
	char aInitLoad[20];	// CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad; // 790 // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)  
	int nCountInitLoad;                          // 790

	BOOL bAxialYieldStop     ;  // 791  
	BOOL bAxialYieldStopBeam ;
	BOOL bAxialYieldStopWall ;
	BOOL bAxialYieldStopTruss;

	int  nPointSpringSupport;   // 805
	BOOL bSupportDzDirStop;
	BOOL bSupportStopUplifting;
	BOOL bSupportStopCollapse;

	int  nNodeConnectivity;    // 820

	int    nGeomNonlinearType ;// 842
	int    nBeamDivNumNy      ;// 865
	int    nBeamDivNumNz      ; 
	BOOL   bWallConsOut       ; 
	int    nWallDivNumZ       ; 
	int    nWallDivNumY       ; 
	double dR                 ; 
	int    nWAreaSize         ; // 870
	BOOL   bAssignByMember    ;
	int    nRCDgnCode         ;
	int    bUseAutoCalcReference      ;
	BOOL   bShowPOCurveAfterAnalysis; // show PO Curve Dialog when PO analysis is finished
	BOOL   bShowPOAnalGraph;          // show Real-time PO graph during PO analysis
	int    nElasticLink; // 875
};

struct T_POGD_UDRD_D_RW_885
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;

	// Reference Design Code Data (v740 추가)
	BOOL   bConsiderAxialForce;       // AIJ
	double dConfidenceFactor;         // Eurocode
	double dScaleFactorWall;          // Eurocode
	BOOL   bScaleFactorBrittle_Use;   // Eurocode
	double dScaleFactorBrittle;       // Eurocode
	BOOL   bScaleFactorEarth_Use;     // Eurocode
	double dScaleFactorEarth;         // Eurocode
	BOOL   bScaleFactorSmoothBar_Use; // Eurocode
	double dScaleFactorSmoothBar;     // Eurocode
	T_GRUP_K SecondaryGroupKey;       // Eurocode

	BOOL   bPermitFail; // 741
	int    nSubstep;    // 741
	BOOL   bBuckling;   // 760
	BOOL bShearYieldStop;       // 780
	BOOL bShearYieldStopBeam;
	BOOL bShearYieldStopWall;

	int nInitLoadMethod;                         // 790  
	char aInitLoad[20];	// CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad; // 790 // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)  
	int nCountInitLoad;                          // 790

	BOOL bAxialYieldStop     ;  // 791  
	BOOL bAxialYieldStopBeam ;
	BOOL bAxialYieldStopWall ;
	BOOL bAxialYieldStopTruss;

	int  nPointSpringSupport;   // 805
	BOOL bSupportDzDirStop;
	BOOL bSupportStopUplifting;
	BOOL bSupportStopCollapse;

	int  nNodeConnectivity;    // 820

	int    nGeomNonlinearType ;// 842
	int    nBeamDivNumNy      ;// 865
	int    nBeamDivNumNz      ; 
	BOOL   bWallConsOut       ; 
	int    nWallDivNumZ       ; 
	int    nWallDivNumY       ; 
	double dR                 ; 
	int    nWAreaSize         ; // 870
	BOOL   bAssignByMember    ;
	int    nRCDgnCode         ;
	int    bUseAutoCalcReference      ;
	BOOL   bShowPOCurveAfterAnalysis; // show PO Curve Dialog when PO analysis is finished
	BOOL   bShowPOAnalGraph;          // show Real-time PO graph during PO analysis
	int    nElasticLink; // 875
	BOOL   bConsiderIgnoreElem; //881
};

struct T_POGD_UDRD_D_RW_940
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;

	// Reference Design Code Data (v740 추가)
	BOOL   bConsiderAxialForce;       // AIJ
	double dConfidenceFactor;         // Eurocode
	double dScaleFactorWall;          // Eurocode
	BOOL   bScaleFactorBrittle_Use;   // Eurocode
	double dScaleFactorBrittle;       // Eurocode
	BOOL   bScaleFactorEarth_Use;     // Eurocode
	double dScaleFactorEarth;         // Eurocode
	BOOL   bScaleFactorSmoothBar_Use; // Eurocode
	double dScaleFactorSmoothBar;     // Eurocode
	T_GRUP_K SecondaryGroupKey;       // Eurocode

	BOOL   bPermitFail; // 741
	int    nSubstep;    // 741
	BOOL   bBuckling;   // 760
	BOOL bShearYieldStop;       // 780
	BOOL bShearYieldStopBeam;
	BOOL bShearYieldStopWall;

	int nInitLoadMethod;                         // 790  
	char aInitLoad[20];	// CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad; // 790 // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)  
	int nCountInitLoad;                          // 790

	BOOL bAxialYieldStop;  // 791  
	BOOL bAxialYieldStopBeam;
	BOOL bAxialYieldStopWall;
	BOOL bAxialYieldStopTruss;

	int  nPointSpringSupport;   // 805
	BOOL bSupportDzDirStop;
	BOOL bSupportStopUplifting;
	BOOL bSupportStopCollapse;

	int  nNodeConnectivity;    // 820

	int    nGeomNonlinearType;// 842
	int    nBeamDivNumNy;// 865
	int    nBeamDivNumNz;
	BOOL   bWallConsOut;
	int    nWallDivNumZ;
	int    nWallDivNumY;
	double dR;
	int    nWAreaSize; // 870
	BOOL   bAssignByMember;
	int    nRCDgnCode; // D_SEME_METHOD
	int    bUseAutoCalcReference;
	BOOL   bShowPOCurveAfterAnalysis; // show PO Curve Dialog when PO analysis is finished
	BOOL   bShowPOAnalGraph;          // show Real-time PO graph during PO analysis
	int    nElasticLink; // 875
	BOOL   bConsiderIgnoreElem; //881
	BOOL   bConsiderRebarArea1D;
	BOOL   bConsiderRebarAreaWall;
	int    nFAreaSizeCore; // Fiber Beam Areas Core, D_POGD_FSIZE
	int    nFAreaSizeCover; // Fiber Beam Areas Cover, D_POGD_FSIZE
	int    nWAreaSizeCover; // Fiber Wall Areas Cover, D_POGD_FSIZE
	int    nBeamDivNumNyCover; // Number of divisions(Beam-Column) - Ny, Cover
	int    nBeamDivNumNzCover; // Number of divisions(Beam-Column) - Nz, Cover
	int    nWallDivNumZCover; // Number of divisions - z, Cover
	int    nWallDivNumYCover; // Number of divisions - y, Cover
};

struct T_POGD_UDRD_D_RW_960
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;

	// Reference Design Code Data (v740 추가)
	BOOL   bConsiderAxialForce;       // AIJ
	double dConfidenceFactor;         // Eurocode
	double dScaleFactorWall;          // Eurocode
	BOOL   bScaleFactorBrittle_Use;   // Eurocode
	double dScaleFactorBrittle;       // Eurocode
	BOOL   bScaleFactorEarth_Use;     // Eurocode
	double dScaleFactorEarth;         // Eurocode
	BOOL   bScaleFactorSmoothBar_Use; // Eurocode
	double dScaleFactorSmoothBar;     // Eurocode
	T_GRUP_K SecondaryGroupKey;       // Eurocode

	BOOL   bPermitFail; // 741
	int    nSubstep;    // 741
	BOOL   bBuckling;   // 760
	BOOL bShearYieldStop;       // 780
	BOOL bShearYieldStopBeam;
	BOOL bShearYieldStopWall;

	int nInitLoadMethod;                         // 790  
	char aInitLoad[20];	// CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad; // 790 // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)  
	int nCountInitLoad;                          // 790

	BOOL bAxialYieldStop;  // 791  
	BOOL bAxialYieldStopBeam;
	BOOL bAxialYieldStopWall;
	BOOL bAxialYieldStopTruss;

	int  nPointSpringSupport;   // 805
	BOOL bSupportDzDirStop;
	BOOL bSupportStopUplifting;
	BOOL bSupportStopCollapse;

	int  nNodeConnectivity;    // 820

	int    nGeomNonlinearType;// 842
	int    nBeamDivNumNy;// 865
	int    nBeamDivNumNz;
	BOOL   bWallConsOut;
	int    nWallDivNumZ;
	int    nWallDivNumY;
	double dR;
	int    nWAreaSize; // 870
	BOOL   bAssignByMember;
	int    nRCDgnCode; // D_SEME_METHOD
	int    bUseAutoCalcReference;
	BOOL   bShowPOCurveAfterAnalysis; // show PO Curve Dialog when PO analysis is finished
	BOOL   bShowPOAnalGraph;          // show Real-time PO graph during PO analysis
	int    nElasticLink; // 875
	BOOL   bConsiderIgnoreElem; //881
	BOOL   bConsiderRebarArea1D;
	BOOL   bConsiderRebarAreaWall;
	int    nFAreaSizeCore; // Fiber Beam Areas Core, D_POGD_FSIZE
	int    nFAreaSizeCover; // Fiber Beam Areas Cover, D_POGD_FSIZE
	int    nWAreaSizeCover; // Fiber Wall Areas Cover, D_POGD_FSIZE
	int    nBeamDivNumNyCover; // Number of divisions(Beam-Column) - Ny, Cover
	int    nBeamDivNumNzCover; // Number of divisions(Beam-Column) - Nz, Cover
	int    nWallDivNumZCover; // Number of divisions - z, Cover
	int    nWallDivNumYCover; // Number of divisions - y, Cover
	BOOL   bConvertWallStiffness;
};

struct T_POGD_UDRD_D_RW
{
	char aOldInitLoad[20];	//CArray<T_POLC_BASE, T_POLC_BASE&> aOldInitLoad; // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	BOOL  bCalcCrackMoment;
	BOOL  bTri_Symmetric;
	double dTri_Tens_a1;
	double dTri_Tens_a2;
	double dTri_Comp_a1;
	double dTri_Comp_a2;
	BOOL   bBi_Symmetric;
	double dBi_Tens_a1;
	double dBi_Comp_a1;
	int    nLocOfBeam;
	int    nLocOfColumn;
	int    nCountOldInitLoad;

	// Reference Design Code Data (v740 추가)
	BOOL   bConsiderAxialForce;       // AIJ
	double dConfidenceFactor;         // Eurocode
	double dScaleFactorWall;          // Eurocode
	BOOL   bScaleFactorBrittle_Use;   // Eurocode
	double dScaleFactorBrittle;       // Eurocode
	BOOL   bScaleFactorEarth_Use;     // Eurocode
	double dScaleFactorEarth;         // Eurocode
	BOOL   bScaleFactorSmoothBar_Use; // Eurocode
	double dScaleFactorSmoothBar;     // Eurocode
	T_GRUP_K SecondaryGroupKey;       // Eurocode

	BOOL   bPermitFail; // 741
	int    nSubstep;    // 741
	BOOL   bBuckling;   // 760
	BOOL bShearYieldStop;       // 780
	BOOL bShearYieldStopBeam;
	BOOL bShearYieldStopWall;

	int nInitLoadMethod;                         // 790  
	char aInitLoad[20];	// CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad; // 790 // 32bit 버전 호환용 처리. 32bit에서 CArray size는 20byte (64bit : 40byte)  
	int nCountInitLoad;                          // 790
	
	BOOL bAxialYieldStop     ;  // 791  
	BOOL bAxialYieldStopBeam ;
	BOOL bAxialYieldStopWall ;
	BOOL bAxialYieldStopTruss;

	int  nPointSpringSupport;   // 805
	BOOL bSupportDzDirStop;
	BOOL bSupportStopUplifting;
	BOOL bSupportStopCollapse;
	
	int  nNodeConnectivity;    // 820

	int    nGeomNonlinearType ;// 842
	int    nBeamDivNumNy      ;// 865
	int    nBeamDivNumNz      ; 
	BOOL   bWallConsOut       ; 
	int    nWallDivNumZ       ; 
	int    nWallDivNumY       ; 
	double dR                 ; 
	int    nWAreaSize         ; // 870
	BOOL   bAssignByMember    ;
	int    nRCDgnCode         ; // D_SEME_METHOD
	int    bUseAutoCalcReference      ;
	BOOL   bShowPOCurveAfterAnalysis; // show PO Curve Dialog when PO analysis is finished
	BOOL   bShowPOAnalGraph;          // show Real-time PO graph during PO analysis
	int    nElasticLink; // 875
	BOOL   bConsiderIgnoreElem; //881
	BOOL   bConsiderRebarArea1D;
	BOOL   bConsiderRebarAreaWall;
	int    nFAreaSizeCore; // Fiber Beam Areas Core, D_POGD_FSIZE
	int    nFAreaSizeCover; // Fiber Beam Areas Cover, D_POGD_FSIZE
	int    nWAreaSizeCover; // Fiber Wall Areas Cover, D_POGD_FSIZE
	int    nBeamDivNumNyCover; // Number of divisions(Beam-Column) - Ny, Cover
	int    nBeamDivNumNzCover; // Number of divisions(Beam-Column) - Nz, Cover
	int    nWallDivNumZCover ; // Number of divisions - z, Cover
	int    nWallDivNumYCover ; // Number of divisions - y, Cover
	BOOL   bConvertWallStiffness;

	T_NTIS_BOUNDARY BoundaryNL;
	BOOL   bConsiderPDelta;

	void Convert730(T_POGD_UDRD_D_RW_730& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;

		bConsiderAxialForce = TRUE; // v740
		dConfidenceFactor = 1.0;
		dScaleFactorWall = 1.6;
		bScaleFactorBrittle_Use = FALSE;
		dScaleFactorBrittle = 1.6;
		bScaleFactorEarth_Use = FALSE;
		dScaleFactorEarth = 0.85;
		bScaleFactorSmoothBar_Use = FALSE;
		dScaleFactorSmoothBar = 0.575;
		SecondaryGroupKey = 0;

		bPermitFail = TRUE;
		nSubstep = 10;

		bBuckling = FALSE;

		bShearYieldStop = FALSE;
		bShearYieldStopBeam = TRUE;
		bShearYieldStopWall = FALSE;

		nInitLoadMethod = 0;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.RemoveAll();
		nCountInitLoad  = 0;      
		
		bAxialYieldStop      = FALSE;  
		bAxialYieldStopBeam  = TRUE ;
		bAxialYieldStopWall  = FALSE;
		bAxialYieldStopTruss = FALSE;
		nPointSpringSupport  = 0;
		bSupportDzDirStop    = FALSE;
		bSupportStopUplifting= FALSE;
		bSupportStopCollapse = FALSE;
		nNodeConnectivity = 1; // 20141105 shjung(By 김종민 부장님) 솔버내부에서 'Fixed"로 처리되었기 때문에 디폴트를 1로 설정
		nGeomNonlinearType = 0;
		nBeamDivNumNy = 15;
		nBeamDivNumNz = 15;
		bWallConsOut  = FALSE;
		nWallDivNumZ  = 20;
		nWallDivNumY  = 10;
		dR            = 0.4;
		nWAreaSize    = D_POGD_WSIZE_EQUAL;
		bAssignByMember = FALSE;
		nRCDgnCode      = 0     ;
		bUseAutoCalcReference = FALSE;
#if defined(_MGEN_JP)
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = FALSE;
#elif defined(_MGEN)
		bShowPOAnalGraph = TRUE;
		bShowPOCurveAfterAnalysis = TRUE;
#else
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = TRUE;
#endif

		nElasticLink = 1;
		bConsiderIgnoreElem = TRUE;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}
	void Convert740(T_POGD_UDRD_D_RW_740& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;

		bConsiderAxialForce       = rData.bConsiderAxialForce; // v740
		dConfidenceFactor         = rData.dConfidenceFactor;
		dScaleFactorWall          = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use   = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth         = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey         = rData.SecondaryGroupKey;

		bPermitFail = TRUE; // v741
		nSubstep = 10;

		bBuckling = FALSE;

		bShearYieldStop = FALSE;
		bShearYieldStopBeam = TRUE;
		bShearYieldStopWall = FALSE;

		nInitLoadMethod = 0;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.RemoveAll();
		nCountInitLoad  = 0;      
		
		bAxialYieldStop      = FALSE;  
		bAxialYieldStopBeam  = TRUE ;
		bAxialYieldStopWall  = FALSE;
		bAxialYieldStopTruss = FALSE;
		nPointSpringSupport  = 0;
		bSupportDzDirStop    = FALSE;
		bSupportStopUplifting= FALSE;
		bSupportStopCollapse = FALSE;
		nNodeConnectivity = 1; // 20141105 shjung(By 김종민 부장님) 솔버내부에서 'Fixed"로 처리되었기 때문에 디폴트를 1로 설정
		nGeomNonlinearType = 0;
		nBeamDivNumNy = 15;
		nBeamDivNumNz = 15;
		bWallConsOut  = FALSE;
		nWallDivNumZ  = 20;
		nWallDivNumY  = 10;
		dR            = 0.4;
		nWAreaSize    = D_POGD_WSIZE_EQUAL;
		bAssignByMember = FALSE;
		nRCDgnCode      = 0     ;
		bUseAutoCalcReference = FALSE;
#if defined(_MGEN_JP)
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = FALSE;
#elif defined(_MGEN)
		bShowPOAnalGraph = TRUE;
		bShowPOCurveAfterAnalysis = TRUE;
#else
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = TRUE;
#endif
		nElasticLink = 1;
		bConsiderIgnoreElem = TRUE;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}
	void Convert750(T_POGD_UDRD_D_RW_750& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;
		
		bConsiderAxialForce       = rData.bConsiderAxialForce; // v740
		dConfidenceFactor         = rData.dConfidenceFactor;
		dScaleFactorWall          = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use   = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth         = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey         = rData.SecondaryGroupKey;
		
		bPermitFail = rData.bPermitFail; // v741
		nSubstep = rData.nSubstep;
		
		bBuckling = FALSE;

		bShearYieldStop = FALSE;
		bShearYieldStopBeam = TRUE;
		bShearYieldStopWall = FALSE;

		nInitLoadMethod = 0;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.RemoveAll();
		nCountInitLoad  = 0;      
		
		bAxialYieldStop      = FALSE;  
		bAxialYieldStopBeam  = TRUE ;
		bAxialYieldStopWall  = FALSE;
		bAxialYieldStopTruss = FALSE;
		nPointSpringSupport  = 0;
		bSupportDzDirStop    = FALSE;
		bSupportStopUplifting= FALSE;
		bSupportStopCollapse = FALSE;
		nNodeConnectivity = 1; // 20141105 shjung(By 김종민 부장님) 솔버내부에서 'Fixed"로 처리되었기 때문에 디폴트를 1로 설정
		nGeomNonlinearType = 0;
		nBeamDivNumNy = 15;
		nBeamDivNumNz = 15;
		bWallConsOut  = FALSE;
		nWallDivNumZ  = 20;
		nWallDivNumY  = 10;
		dR            = 0.4;
		nWAreaSize    = D_POGD_WSIZE_EQUAL;
		bAssignByMember = FALSE;
		nRCDgnCode      = 0;
		bUseAutoCalcReference = FALSE;
#if defined(_MGEN_JP)
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = FALSE;
#elif defined(_MGEN)
		bShowPOAnalGraph = TRUE;
		bShowPOCurveAfterAnalysis = TRUE;
#else
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = TRUE;
#endif
		nElasticLink = 1;
		bConsiderIgnoreElem = TRUE;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}
	void Convert770(T_POGD_UDRD_D_RW_770& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;
		
		bConsiderAxialForce       = rData.bConsiderAxialForce; // v740
		dConfidenceFactor         = rData.dConfidenceFactor;
		dScaleFactorWall          = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use   = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth         = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey         = rData.SecondaryGroupKey;
		
		bPermitFail = rData.bPermitFail; // v741
		nSubstep = rData.nSubstep;
		
		bBuckling = rData.bBuckling;
		
		bShearYieldStop = FALSE;
		bShearYieldStopBeam = TRUE;
		bShearYieldStopWall = FALSE;

		nInitLoadMethod = 0;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.RemoveAll();
		nCountInitLoad  = 0;      
		
		bAxialYieldStop      = FALSE;  
		bAxialYieldStopBeam  = TRUE ;
		bAxialYieldStopWall  = FALSE;
		bAxialYieldStopTruss = FALSE;
		nPointSpringSupport  = 0;
		bSupportDzDirStop    = FALSE;
		bSupportStopUplifting= FALSE;
		bSupportStopCollapse = FALSE;
		nNodeConnectivity = 1; // 20141105 shjung(By 김종민 부장님) 솔버내부에서 'Fixed"로 처리되었기 때문에 디폴트를 1로 설정
		nGeomNonlinearType = 0;
		nBeamDivNumNy = 15;
		nBeamDivNumNz = 15;
		bWallConsOut  = FALSE;
		nWallDivNumZ  = 20;
		nWallDivNumY  = 10;
		dR            = 0.4;
		nWAreaSize    = D_POGD_WSIZE_EQUAL;
		bAssignByMember = FALSE;
		nRCDgnCode      = 0     ;
		bUseAutoCalcReference = FALSE;
#if defined(_MGEN_JP)
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = FALSE;
#elif defined(_MGEN)
		bShowPOAnalGraph = TRUE;
		bShowPOCurveAfterAnalysis = TRUE;
#else
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = TRUE;
#endif
		nElasticLink = 1;
		bConsiderIgnoreElem = TRUE;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}
	void Convert785(T_POGD_UDRD_D_RW_785& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;
		
		bConsiderAxialForce       = rData.bConsiderAxialForce; // v740
		dConfidenceFactor         = rData.dConfidenceFactor;
		dScaleFactorWall          = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use   = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth         = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey         = rData.SecondaryGroupKey;
		
		bPermitFail = rData.bPermitFail; // v741
		nSubstep = rData.nSubstep;
		
		bBuckling = rData.bBuckling;
		
		bShearYieldStop = rData.bShearYieldStop;
		bShearYieldStopBeam = rData.bShearYieldStopBeam;
		bShearYieldStopWall = rData.bShearYieldStopWall;
		
		nInitLoadMethod = 0;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.RemoveAll();
		nCountInitLoad  = 0;    

		bAxialYieldStop      = FALSE;  
		bAxialYieldStopBeam  = TRUE ;
		bAxialYieldStopWall  = FALSE;
		bAxialYieldStopTruss = FALSE;
		nPointSpringSupport  = 0;
		bSupportDzDirStop    = FALSE;
		bSupportStopUplifting= FALSE;
		bSupportStopCollapse = FALSE;
		nNodeConnectivity = 1; // 20141105 shjung(By 김종민 부장님) 솔버내부에서 'Fixed"로 처리되었기 때문에 디폴트를 1로 설정
		nGeomNonlinearType = 0;
		nBeamDivNumNy = 15;
		nBeamDivNumNz = 15;
		bWallConsOut  = FALSE;
		nWallDivNumZ  = 20;
		nWallDivNumY  = 10;
		dR            = 0.4;
		nWAreaSize    = D_POGD_WSIZE_EQUAL;
		bAssignByMember = FALSE;
		nRCDgnCode      = 0     ;
		bUseAutoCalcReference = FALSE;
#if defined(_MGEN_JP)
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = FALSE;
#elif defined(_MGEN)
		bShowPOAnalGraph = TRUE;
		bShowPOCurveAfterAnalysis = TRUE;
#else
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = TRUE;
#endif
		nElasticLink = 1;
		bConsiderIgnoreElem = TRUE;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}
	void Convert790(T_POGD_UDRD_D_RW_790& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;
		
		bConsiderAxialForce       = rData.bConsiderAxialForce; // v740
		dConfidenceFactor         = rData.dConfidenceFactor;
		dScaleFactorWall          = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use   = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth         = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey         = rData.SecondaryGroupKey;
		
		bPermitFail = rData.bPermitFail; // v741
		nSubstep = rData.nSubstep;
		
		bBuckling = rData.bBuckling;
		
		bShearYieldStop = rData.bShearYieldStop;
		bShearYieldStopBeam = rData.bShearYieldStopBeam;
		bShearYieldStopWall = rData.bShearYieldStopWall;
		
		nInitLoadMethod = rData.nInitLoadMethod;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.Copy(rData.aInitLoad);
		nCountInitLoad  = rData.nCountInitLoad;    
		
		bAxialYieldStop      = FALSE;  
		bAxialYieldStopBeam  = TRUE ;
		bAxialYieldStopWall  = FALSE;
		bAxialYieldStopTruss = FALSE;
		nPointSpringSupport  = 0;
		bSupportDzDirStop    = FALSE;
		bSupportStopUplifting= FALSE;
		bSupportStopCollapse = FALSE;
		nNodeConnectivity = 1; // 20141105 shjung(By 김종민 부장님) 솔버내부에서 'Fixed"로 처리되었기 때문에 디폴트를 1로 설정
		nGeomNonlinearType = 0;
		nBeamDivNumNy = 15;
		nBeamDivNumNz = 15;
		bWallConsOut  = FALSE;
		nWallDivNumZ  = 20;
		nWallDivNumY  = 10;
		dR            = 0.4;
		nWAreaSize    = D_POGD_WSIZE_EQUAL;
		bAssignByMember = FALSE;
		nRCDgnCode      = 0;
		bUseAutoCalcReference = FALSE;
#if defined(_MGEN_JP)
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = FALSE;
#elif defined(_MGEN)
		bShowPOAnalGraph = TRUE;
		bShowPOCurveAfterAnalysis = TRUE;
#else
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = TRUE;
#endif
		nElasticLink = 1;
		bConsiderIgnoreElem = TRUE;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}
	void Convert800(T_POGD_UDRD_D_RW_800& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;
		
		bConsiderAxialForce       = rData.bConsiderAxialForce; // v740
		dConfidenceFactor         = rData.dConfidenceFactor;
		dScaleFactorWall          = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use   = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth         = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey         = rData.SecondaryGroupKey;
		
		bPermitFail = rData.bPermitFail; // v741
		nSubstep = rData.nSubstep;
		
		bBuckling = rData.bBuckling;
		
		bShearYieldStop = rData.bShearYieldStop;
		bShearYieldStopBeam = rData.bShearYieldStopBeam;
		bShearYieldStopWall = rData.bShearYieldStopWall;
		
		nInitLoadMethod = rData.nInitLoadMethod;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.Copy(rData.aInitLoad);
		nCountInitLoad  = rData.nCountInitLoad;    
		
		bAxialYieldStop      = rData.bAxialYieldStop;  
		bAxialYieldStopBeam  = rData.bAxialYieldStopBeam;
		bAxialYieldStopWall  = rData.bAxialYieldStopWall;
		bAxialYieldStopTruss = rData.bAxialYieldStopTruss;
		nPointSpringSupport  = 0;
		bSupportDzDirStop    = FALSE;
		bSupportStopUplifting= FALSE;
		bSupportStopCollapse = FALSE;
		nNodeConnectivity = 1; // 20141105 shjung(By 김종민 부장님) 솔버내부에서 'Fixed"로 처리되었기 때문에 디폴트를 1로 설정
		nGeomNonlinearType = 0;
		nBeamDivNumNy = 15;
		nBeamDivNumNz = 15;
		bWallConsOut  = FALSE;
		nWallDivNumZ  = 20;
		nWallDivNumY  = 10;
		dR            = 0.4;
		nWAreaSize    = D_POGD_WSIZE_EQUAL;
		bAssignByMember = FALSE;
		nRCDgnCode      = 0     ;
		bUseAutoCalcReference = FALSE;
#if defined(_MGEN_JP)
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = FALSE;
#elif defined(_MGEN)
		bShowPOAnalGraph = TRUE;
		bShowPOCurveAfterAnalysis = TRUE;
#else
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = TRUE;
#endif
		nElasticLink = 1;
		bConsiderIgnoreElem = TRUE;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}
	void Convert820(T_POGD_UDRD_D_RW_820& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;
		
		bConsiderAxialForce       = rData.bConsiderAxialForce; // v740
		dConfidenceFactor         = rData.dConfidenceFactor;
		dScaleFactorWall          = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use   = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth         = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey         = rData.SecondaryGroupKey;
		
		bPermitFail = rData.bPermitFail; // v741
		nSubstep = rData.nSubstep;
		
		bBuckling = rData.bBuckling;
		
		bShearYieldStop = rData.bShearYieldStop;
		bShearYieldStopBeam = rData.bShearYieldStopBeam;
		bShearYieldStopWall = rData.bShearYieldStopWall;
		
		nInitLoadMethod = rData.nInitLoadMethod;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.Copy(rData.aInitLoad);
		nCountInitLoad  = rData.nCountInitLoad;    
		
		bAxialYieldStop      = rData.bAxialYieldStop;  
		bAxialYieldStopBeam  = rData.bAxialYieldStopBeam;
		bAxialYieldStopWall  = rData.bAxialYieldStopWall;
		bAxialYieldStopTruss = rData.bAxialYieldStopTruss;
		nPointSpringSupport  = rData.nPointSpringSupport;
		bSupportDzDirStop    = rData.bSupportDzDirStop;
		bSupportStopUplifting= rData.bSupportStopUplifting;
		bSupportStopCollapse = rData.bSupportStopCollapse;
		nNodeConnectivity = 1; // 20141105 shjung(By 김종민 부장님) 솔버내부에서 'Fixed"로 처리되었기 때문에 디폴트를 1로 설정
		nGeomNonlinearType = 0;
		nBeamDivNumNy = 15;
		nBeamDivNumNz = 15;
		bWallConsOut  = FALSE;
		nWallDivNumZ  = 20;
		nWallDivNumY  = 10;
		dR            = 0.4;
		nWAreaSize    = D_POGD_WSIZE_EQUAL;
		bAssignByMember = FALSE;
		nRCDgnCode      = 0     ;
		bUseAutoCalcReference = FALSE;
#if defined(_MGEN_JP)
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = FALSE;
#elif defined(_MGEN)
		bShowPOAnalGraph = TRUE;
		bShowPOCurveAfterAnalysis = TRUE;
#else
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = TRUE;
#endif
		nElasticLink = 1;
		bConsiderIgnoreElem = TRUE;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}
	void Convert841(T_POGD_UDRD_D_RW_841& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;

		bConsiderAxialForce       = rData.bConsiderAxialForce; // v740
		dConfidenceFactor         = rData.dConfidenceFactor;
		dScaleFactorWall          = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use   = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth         = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey         = rData.SecondaryGroupKey;

		bPermitFail = rData.bPermitFail; // v741
		nSubstep = rData.nSubstep;

		bBuckling = rData.bBuckling;

		bShearYieldStop = rData.bShearYieldStop;
		bShearYieldStopBeam = rData.bShearYieldStopBeam;
		bShearYieldStopWall = rData.bShearYieldStopWall;

		nInitLoadMethod = rData.nInitLoadMethod;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.Copy(rData.aInitLoad);
		nCountInitLoad  = rData.nCountInitLoad;    

		bAxialYieldStop      = rData.bAxialYieldStop;  
		bAxialYieldStopBeam  = rData.bAxialYieldStopBeam;
		bAxialYieldStopWall  = rData.bAxialYieldStopWall;
		bAxialYieldStopTruss = rData.bAxialYieldStopTruss;
		nPointSpringSupport  = rData.nPointSpringSupport;
		bSupportDzDirStop    = rData.bSupportDzDirStop;
		bSupportStopUplifting= rData.bSupportStopUplifting;
		bSupportStopCollapse = rData.bSupportStopCollapse;
		nNodeConnectivity     = rData.nNodeConnectivity;
		nGeomNonlinearType    = 0;
		nBeamDivNumNy = 15;
		nBeamDivNumNz = 15;
		bWallConsOut  = FALSE;
		nWallDivNumZ  = 20;
		nWallDivNumY  = 10;
		dR            = 0.4;
		nWAreaSize    = D_POGD_WSIZE_EQUAL;
		bAssignByMember = FALSE;
		nRCDgnCode      = 0     ;
		bUseAutoCalcReference = FALSE;
#if defined(_MGEN_JP)
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = FALSE;
#elif defined(_MGEN)
		bShowPOAnalGraph = TRUE;
		bShowPOCurveAfterAnalysis = TRUE;
#else
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = TRUE;
#endif
		nElasticLink = 1;
		bConsiderIgnoreElem = TRUE;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}
	void Convert861(T_POGD_UDRD_D_RW_861& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;

		bConsiderAxialForce       = rData.bConsiderAxialForce; // v740
		dConfidenceFactor         = rData.dConfidenceFactor;
		dScaleFactorWall          = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use   = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth         = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey         = rData.SecondaryGroupKey;

		bPermitFail = rData.bPermitFail; // v741
		nSubstep = rData.nSubstep;

		bBuckling = rData.bBuckling;

		bShearYieldStop = rData.bShearYieldStop;
		bShearYieldStopBeam = rData.bShearYieldStopBeam;
		bShearYieldStopWall = rData.bShearYieldStopWall;

		nInitLoadMethod = rData.nInitLoadMethod;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.Copy(rData.aInitLoad);
		nCountInitLoad  = rData.nCountInitLoad;    

		bAxialYieldStop      = rData.bAxialYieldStop;  
		bAxialYieldStopBeam  = rData.bAxialYieldStopBeam;
		bAxialYieldStopWall  = rData.bAxialYieldStopWall;
		bAxialYieldStopTruss = rData.bAxialYieldStopTruss;
		nPointSpringSupport  = rData.nPointSpringSupport;
		bSupportDzDirStop    = rData.bSupportDzDirStop;
		bSupportStopUplifting= rData.bSupportStopUplifting;
		bSupportStopCollapse = rData.bSupportStopCollapse;
		nNodeConnectivity     = rData.nNodeConnectivity;
		nGeomNonlinearType    = rData.nGeomNonlinearType;
		nBeamDivNumNy = 15;
		nBeamDivNumNz = 15;
		bWallConsOut  = FALSE;
		nWallDivNumZ  = 20;
		nWallDivNumY  = 10;
		dR            = 0.4;
		nWAreaSize    = D_POGD_WSIZE_EQUAL;
		bAssignByMember = FALSE;
		nRCDgnCode      = 0     ;
		bUseAutoCalcReference = FALSE;
#if defined(_MGEN_JP)
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = FALSE;
#elif defined(_MGEN)
		bShowPOAnalGraph = TRUE;
		bShowPOCurveAfterAnalysis = TRUE;
#else
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = TRUE;
#endif
		nElasticLink = 1;
		bConsiderIgnoreElem = TRUE;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}
	void Convert865(T_POGD_UDRD_D_RW_865& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;

		bConsiderAxialForce       = rData.bConsiderAxialForce; // v740
		dConfidenceFactor         = rData.dConfidenceFactor;
		dScaleFactorWall          = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use   = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth         = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey         = rData.SecondaryGroupKey;

		bPermitFail = rData.bPermitFail; // v741
		nSubstep = rData.nSubstep;

		bBuckling = rData.bBuckling;

		bShearYieldStop = rData.bShearYieldStop;
		bShearYieldStopBeam = rData.bShearYieldStopBeam;
		bShearYieldStopWall = rData.bShearYieldStopWall;

		nInitLoadMethod = rData.nInitLoadMethod;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.Copy(rData.aInitLoad);
		nCountInitLoad  = rData.nCountInitLoad;    

		bAxialYieldStop      = rData.bAxialYieldStop;  
		bAxialYieldStopBeam  = rData.bAxialYieldStopBeam;
		bAxialYieldStopWall  = rData.bAxialYieldStopWall;
		bAxialYieldStopTruss = rData.bAxialYieldStopTruss;
		nPointSpringSupport  = rData.nPointSpringSupport;
		bSupportDzDirStop    = rData.bSupportDzDirStop;
		bSupportStopUplifting= rData.bSupportStopUplifting;
		bSupportStopCollapse = rData.bSupportStopCollapse;
		nNodeConnectivity     = rData.nNodeConnectivity;
		nGeomNonlinearType    = rData.nGeomNonlinearType;
		nBeamDivNumNy         = rData.nBeamDivNumNy ;
		nBeamDivNumNz         = rData.nBeamDivNumNz ;
		bWallConsOut          = rData.bWallConsOut  ;
		nWallDivNumZ          = rData.nWallDivNumZ  ;
		nWallDivNumY          = rData.nWallDivNumY  ;
		dR                    = rData.dR            ;
		nWAreaSize            = D_POGD_WSIZE_EQUAL;
		bAssignByMember = FALSE;
		nRCDgnCode      = 0     ;
		bUseAutoCalcReference = FALSE;
#if defined(_MGEN_JP)
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = FALSE;
#elif defined(_MGEN)
		bShowPOAnalGraph = TRUE;
		bShowPOCurveAfterAnalysis = TRUE;
#else
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = TRUE;
#endif
		nElasticLink = 1;
		bConsiderIgnoreElem = TRUE;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}
	void Convert870minor(T_POGD_UDRD_D_RW_870minor& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;

		bConsiderAxialForce       = rData.bConsiderAxialForce; // v740
		dConfidenceFactor         = rData.dConfidenceFactor;
		dScaleFactorWall          = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use   = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth         = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey         = rData.SecondaryGroupKey;

		bPermitFail = rData.bPermitFail; // v741
		nSubstep = rData.nSubstep;

		bBuckling = rData.bBuckling;

		bShearYieldStop = rData.bShearYieldStop;
		bShearYieldStopBeam = rData.bShearYieldStopBeam;
		bShearYieldStopWall = rData.bShearYieldStopWall;

		nInitLoadMethod = rData.nInitLoadMethod;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.Copy(rData.aInitLoad);
		nCountInitLoad  = rData.nCountInitLoad;    

		bAxialYieldStop      = rData.bAxialYieldStop;  
		bAxialYieldStopBeam  = rData.bAxialYieldStopBeam;
		bAxialYieldStopWall  = rData.bAxialYieldStopWall;
		bAxialYieldStopTruss = rData.bAxialYieldStopTruss;
		nPointSpringSupport  = rData.nPointSpringSupport;
		bSupportDzDirStop    = rData.bSupportDzDirStop;
		bSupportStopUplifting= rData.bSupportStopUplifting;
		bSupportStopCollapse = rData.bSupportStopCollapse;
		nNodeConnectivity     = rData.nNodeConnectivity;
		nGeomNonlinearType    = rData.nGeomNonlinearType;
		nBeamDivNumNy         = rData.nBeamDivNumNy ;
		nBeamDivNumNz         = rData.nBeamDivNumNz ;
		bWallConsOut          = rData.bWallConsOut  ;
		nWallDivNumZ          = rData.nWallDivNumZ  ;
		nWallDivNumY          = rData.nWallDivNumY  ;
		dR                    = rData.dR            ;
		nWAreaSize            = rData.nWAreaSize     ;
		bAssignByMember       = rData.bAssignByMember;
		nRCDgnCode            = rData.nRCDgnCode     ;
		bUseAutoCalcReference = rData.bUseAutoCalcReference  ;
#if defined(_MGEN_JP)
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = FALSE;
#elif defined(_MGEN)
		bShowPOAnalGraph = TRUE;
		bShowPOCurveAfterAnalysis = TRUE;
#else
		bShowPOAnalGraph = FALSE;
		bShowPOCurveAfterAnalysis = TRUE;
#endif
		nElasticLink = 1;
		bConsiderIgnoreElem = TRUE;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}
	void Convert871(T_POGD_UDRD_D_RW_871& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;

		bConsiderAxialForce       = rData.bConsiderAxialForce; // v740
		dConfidenceFactor         = rData.dConfidenceFactor;
		dScaleFactorWall          = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use   = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth         = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey         = rData.SecondaryGroupKey;

		bPermitFail = rData.bPermitFail; // v741
		nSubstep = rData.nSubstep;

		bBuckling = rData.bBuckling;

		bShearYieldStop = rData.bShearYieldStop;
		bShearYieldStopBeam = rData.bShearYieldStopBeam;
		bShearYieldStopWall = rData.bShearYieldStopWall;

		nInitLoadMethod = rData.nInitLoadMethod;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.Copy(rData.aInitLoad);
		nCountInitLoad  = rData.nCountInitLoad;    

		bAxialYieldStop      = rData.bAxialYieldStop;  
		bAxialYieldStopBeam  = rData.bAxialYieldStopBeam;
		bAxialYieldStopWall  = rData.bAxialYieldStopWall;
		bAxialYieldStopTruss = rData.bAxialYieldStopTruss;
		nPointSpringSupport  = rData.nPointSpringSupport;
		bSupportDzDirStop    = rData.bSupportDzDirStop;
		bSupportStopUplifting= rData.bSupportStopUplifting;
		bSupportStopCollapse = rData.bSupportStopCollapse;
		nNodeConnectivity     = rData.nNodeConnectivity;
		nGeomNonlinearType    = rData.nGeomNonlinearType;
		nBeamDivNumNy         = rData.nBeamDivNumNy ;
		nBeamDivNumNz         = rData.nBeamDivNumNz ;
		bWallConsOut          = rData.bWallConsOut  ;
		nWallDivNumZ          = rData.nWallDivNumZ  ;
		nWallDivNumY          = rData.nWallDivNumY  ;
		dR                    = rData.dR            ;
		nWAreaSize            = rData.nWAreaSize     ;
		bAssignByMember       = rData.bAssignByMember;
		nRCDgnCode            = rData.nRCDgnCode     ;
		bUseAutoCalcReference = rData.bUseAutoCalcReference  ;
		bShowPOCurveAfterAnalysis = rData.bShowPOCurveAfterAnalysis;
		bShowPOAnalGraph          = rData.bShowPOAnalGraph;;
		nElasticLink = 1;
		bConsiderIgnoreElem = TRUE;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}
	void Convert880(T_POGD_UDRD_D_RW_880& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;

		bConsiderAxialForce       = rData.bConsiderAxialForce; // v740
		dConfidenceFactor         = rData.dConfidenceFactor;
		dScaleFactorWall          = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use   = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth         = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey         = rData.SecondaryGroupKey;

		bPermitFail = rData.bPermitFail; // v741
		nSubstep = rData.nSubstep;

		bBuckling = rData.bBuckling;

		bShearYieldStop = rData.bShearYieldStop;
		bShearYieldStopBeam = rData.bShearYieldStopBeam;
		bShearYieldStopWall = rData.bShearYieldStopWall;

		nInitLoadMethod = rData.nInitLoadMethod;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.Copy(rData.aInitLoad);
		nCountInitLoad  = rData.nCountInitLoad;    

		bAxialYieldStop      = rData.bAxialYieldStop;  
		bAxialYieldStopBeam  = rData.bAxialYieldStopBeam;
		bAxialYieldStopWall  = rData.bAxialYieldStopWall;
		bAxialYieldStopTruss = rData.bAxialYieldStopTruss;
		nPointSpringSupport  = rData.nPointSpringSupport;
		bSupportDzDirStop    = rData.bSupportDzDirStop;
		bSupportStopUplifting= rData.bSupportStopUplifting;
		bSupportStopCollapse = rData.bSupportStopCollapse;
		nNodeConnectivity     = rData.nNodeConnectivity;
		nGeomNonlinearType    = rData.nGeomNonlinearType;
		nBeamDivNumNy         = rData.nBeamDivNumNy ;
		nBeamDivNumNz         = rData.nBeamDivNumNz ;
		bWallConsOut          = rData.bWallConsOut  ;
		nWallDivNumZ          = rData.nWallDivNumZ  ;
		nWallDivNumY          = rData.nWallDivNumY  ;
		dR                    = rData.dR            ;
		nWAreaSize            = rData.nWAreaSize     ;
		bAssignByMember       = rData.bAssignByMember;
		nRCDgnCode            = rData.nRCDgnCode     ;
		bUseAutoCalcReference = rData.bUseAutoCalcReference  ;
		bShowPOCurveAfterAnalysis = rData.bShowPOCurveAfterAnalysis;
		bShowPOAnalGraph          = rData.bShowPOAnalGraph;;
		nElasticLink = rData.nElasticLink;
		bConsiderIgnoreElem = TRUE;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}
	void Convert885(T_POGD_UDRD_D_RW_885& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;

		bConsiderAxialForce       = rData.bConsiderAxialForce; // v740
		dConfidenceFactor         = rData.dConfidenceFactor;
		dScaleFactorWall          = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use   = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth         = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey         = rData.SecondaryGroupKey;

		bPermitFail = rData.bPermitFail; // v741
		nSubstep = rData.nSubstep;

		bBuckling = rData.bBuckling;

		bShearYieldStop = rData.bShearYieldStop;
		bShearYieldStopBeam = rData.bShearYieldStopBeam;
		bShearYieldStopWall = rData.bShearYieldStopWall;

		nInitLoadMethod = rData.nInitLoadMethod;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.Copy(rData.aInitLoad);
		nCountInitLoad  = rData.nCountInitLoad;    

		bAxialYieldStop      = rData.bAxialYieldStop;  
		bAxialYieldStopBeam  = rData.bAxialYieldStopBeam;
		bAxialYieldStopWall  = rData.bAxialYieldStopWall;
		bAxialYieldStopTruss = rData.bAxialYieldStopTruss;
		nPointSpringSupport  = rData.nPointSpringSupport;
		bSupportDzDirStop    = rData.bSupportDzDirStop;
		bSupportStopUplifting= rData.bSupportStopUplifting;
		bSupportStopCollapse = rData.bSupportStopCollapse;
		nNodeConnectivity     = rData.nNodeConnectivity;
		nGeomNonlinearType    = rData.nGeomNonlinearType;
		nBeamDivNumNy         = rData.nBeamDivNumNy ;
		nBeamDivNumNz         = rData.nBeamDivNumNz ;
		bWallConsOut          = rData.bWallConsOut  ;
		nWallDivNumZ          = rData.nWallDivNumZ  ;
		nWallDivNumY          = rData.nWallDivNumY  ;
		dR                    = rData.dR            ;
		nWAreaSize            = rData.nWAreaSize     ;
		bAssignByMember       = rData.bAssignByMember;
		nRCDgnCode            = rData.nRCDgnCode     ;
		bUseAutoCalcReference = rData.bUseAutoCalcReference  ;
		bShowPOCurveAfterAnalysis = rData.bShowPOCurveAfterAnalysis;
		bShowPOAnalGraph          = rData.bShowPOAnalGraph;;
		nElasticLink = rData.nElasticLink;
		bConsiderIgnoreElem = rData.bConsiderIgnoreElem;;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_POGD_FSIZE_EQUAL;
		nFAreaSizeCover = nFAreaSizeCore;
		nWAreaSizeCover = nWAreaSize;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover  = nWallDivNumZ;
		nWallDivNumYCover  = nWallDivNumY;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}
	void Convert940(T_POGD_UDRD_D_RW_940& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm = rData.bDisplNorm;
		bForceNorm = rData.bForceNorm;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm = rData.dDisplNorm;
		dForceNorm = rData.dForceNorm;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1 = rData.dTri_Tens_a1;
		dTri_Tens_a2 = rData.dTri_Tens_a2;
		dTri_Comp_a1 = rData.dTri_Comp_a1;
		dTri_Comp_a2 = rData.dTri_Comp_a2;
		bBi_Symmetric = rData.bBi_Symmetric;
		dBi_Tens_a1 = rData.dBi_Tens_a1;
		dBi_Comp_a1 = rData.dBi_Comp_a1;
		nLocOfBeam = rData.nLocOfBeam;
		nLocOfColumn = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;

		bConsiderAxialForce = rData.bConsiderAxialForce; // v740
		dConfidenceFactor = rData.dConfidenceFactor;
		dScaleFactorWall = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey = rData.SecondaryGroupKey;

		bPermitFail = rData.bPermitFail; // v741
		nSubstep = rData.nSubstep;

		bBuckling = rData.bBuckling;

		bShearYieldStop = rData.bShearYieldStop;
		bShearYieldStopBeam = rData.bShearYieldStopBeam;
		bShearYieldStopWall = rData.bShearYieldStopWall;

		nInitLoadMethod = rData.nInitLoadMethod;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.Copy(rData.aInitLoad);
		nCountInitLoad = rData.nCountInitLoad;

		bAxialYieldStop = rData.bAxialYieldStop;
		bAxialYieldStopBeam = rData.bAxialYieldStopBeam;
		bAxialYieldStopWall = rData.bAxialYieldStopWall;
		bAxialYieldStopTruss = rData.bAxialYieldStopTruss;
		nPointSpringSupport = rData.nPointSpringSupport;
		bSupportDzDirStop = rData.bSupportDzDirStop;
		bSupportStopUplifting = rData.bSupportStopUplifting;
		bSupportStopCollapse = rData.bSupportStopCollapse;
		nNodeConnectivity = rData.nNodeConnectivity;
		nGeomNonlinearType = rData.nGeomNonlinearType;
		nBeamDivNumNy = rData.nBeamDivNumNy;
		nBeamDivNumNz = rData.nBeamDivNumNz;
		bWallConsOut = rData.bWallConsOut;
		nWallDivNumZ = rData.nWallDivNumZ;
		nWallDivNumY = rData.nWallDivNumY;
		dR = rData.dR;
		nWAreaSize = rData.nWAreaSize;
		bAssignByMember = rData.bAssignByMember;
		nRCDgnCode = rData.nRCDgnCode;
		bUseAutoCalcReference = rData.bUseAutoCalcReference;
		bShowPOCurveAfterAnalysis = rData.bShowPOCurveAfterAnalysis;
		bShowPOAnalGraph = rData.bShowPOAnalGraph;
		nElasticLink = rData.nElasticLink;
		bConsiderIgnoreElem = rData.bConsiderIgnoreElem;;
		bConsiderRebarArea1D = rData.bConsiderRebarArea1D;
		bConsiderRebarAreaWall = rData.bConsiderRebarAreaWall;
		nFAreaSizeCore = rData.nFAreaSizeCore;
		nFAreaSizeCover = rData.nFAreaSizeCover;
		nWAreaSizeCover = rData.nWAreaSizeCover;
		nBeamDivNumNyCover = rData.nBeamDivNumNyCover;
		nBeamDivNumNzCover = rData.nBeamDivNumNzCover;
		nWallDivNumZCover = rData.nWallDivNumZCover;
		nWallDivNumYCover = rData.nWallDivNumYCover;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}

	void Convert960(T_POGD_UDRD_D_RW_960& rData)
	{
		memset(aOldInitLoad, 0, sizeof(aOldInitLoad));		//aOldInitLoad.Copy(rData.aOldInitLoad);
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm = rData.bDisplNorm;
		bForceNorm = rData.bForceNorm;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm = rData.dDisplNorm;
		dForceNorm = rData.dForceNorm;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1 = rData.dTri_Tens_a1;
		dTri_Tens_a2 = rData.dTri_Tens_a2;
		dTri_Comp_a1 = rData.dTri_Comp_a1;
		dTri_Comp_a2 = rData.dTri_Comp_a2;
		bBi_Symmetric = rData.bBi_Symmetric;
		dBi_Tens_a1 = rData.dBi_Tens_a1;
		dBi_Comp_a1 = rData.dBi_Comp_a1;
		nLocOfBeam = rData.nLocOfBeam;
		nLocOfColumn = rData.nLocOfColumn;
		nCountOldInitLoad = rData.nCountOldInitLoad;

		bConsiderAxialForce = rData.bConsiderAxialForce; // v740
		dConfidenceFactor = rData.dConfidenceFactor;
		dScaleFactorWall = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey = rData.SecondaryGroupKey;

		bPermitFail = rData.bPermitFail; // v741
		nSubstep = rData.nSubstep;

		bBuckling = rData.bBuckling;

		bShearYieldStop = rData.bShearYieldStop;
		bShearYieldStopBeam = rData.bShearYieldStopBeam;
		bShearYieldStopWall = rData.bShearYieldStopWall;

		nInitLoadMethod = rData.nInitLoadMethod;
		memset(aInitLoad, 0, sizeof(aInitLoad));		//aInitLoad.Copy(rData.aInitLoad);
		nCountInitLoad = rData.nCountInitLoad;

		bAxialYieldStop = rData.bAxialYieldStop;
		bAxialYieldStopBeam = rData.bAxialYieldStopBeam;
		bAxialYieldStopWall = rData.bAxialYieldStopWall;
		bAxialYieldStopTruss = rData.bAxialYieldStopTruss;
		nPointSpringSupport = rData.nPointSpringSupport;
		bSupportDzDirStop = rData.bSupportDzDirStop;
		bSupportStopUplifting = rData.bSupportStopUplifting;
		bSupportStopCollapse = rData.bSupportStopCollapse;
		nNodeConnectivity = rData.nNodeConnectivity;
		nGeomNonlinearType = rData.nGeomNonlinearType;
		nBeamDivNumNy = rData.nBeamDivNumNy;
		nBeamDivNumNz = rData.nBeamDivNumNz;
		bWallConsOut = rData.bWallConsOut;
		nWallDivNumZ = rData.nWallDivNumZ;
		nWallDivNumY = rData.nWallDivNumY;
		dR = rData.dR;
		nWAreaSize = rData.nWAreaSize;
		bAssignByMember = rData.bAssignByMember;
		nRCDgnCode = rData.nRCDgnCode;
		bUseAutoCalcReference = rData.bUseAutoCalcReference;
		bShowPOCurveAfterAnalysis = rData.bShowPOCurveAfterAnalysis;
		bShowPOAnalGraph = rData.bShowPOAnalGraph;;
		nElasticLink = rData.nElasticLink;
		bConsiderIgnoreElem = rData.bConsiderIgnoreElem;;
		bConsiderRebarArea1D = rData.bConsiderRebarArea1D;
		bConsiderRebarAreaWall = rData.bConsiderRebarAreaWall;
		nFAreaSizeCore = rData.nFAreaSizeCore;
		nFAreaSizeCover = rData.nFAreaSizeCover;
		nWAreaSizeCover = rData.nWAreaSizeCover;
		nBeamDivNumNyCover = rData.nBeamDivNumNyCover;
		nBeamDivNumNzCover = rData.nBeamDivNumNzCover;
		nWallDivNumZCover = rData.nWallDivNumZCover;
		nWallDivNumYCover = rData.nWallDivNumYCover;
		bConvertWallStiffness = rData.bConvertWallStiffness;

		BoundaryNL.Initialize();
		bConsiderPDelta = FALSE;
	}

	void GetPogd(T_POGD_D& rData)
	{
		rData.aOldInitLoad.RemoveAll();		//aOldInitLoad.Copy(rData.aOldInitLoad);
		rData.nMaxIterNum = nMaxIterNum;
		rData.bDisplNorm  = bDisplNorm ;
		rData.bForceNorm  = bForceNorm ;
		rData.bEnergyNorm = bEnergyNorm;
		rData.dDisplNorm  = dDisplNorm ;
		rData.dForceNorm  = dForceNorm ;
		rData.dEnergyNorm = dEnergyNorm;
		rData.bCalcCrackMoment = bCalcCrackMoment;
		rData.bTri_Symmetric = bTri_Symmetric;
		rData.dTri_Tens_a1   = dTri_Tens_a1  ;
		rData.dTri_Tens_a2   = dTri_Tens_a2  ;
		rData.dTri_Comp_a1   = dTri_Comp_a1  ;
		rData.dTri_Comp_a2   = dTri_Comp_a2  ;
		rData.bBi_Symmetric  = bBi_Symmetric ;
		rData.dBi_Tens_a1    = dBi_Tens_a1   ;
		rData.dBi_Comp_a1    = dBi_Comp_a1   ;
		rData.nLocOfBeam     = nLocOfBeam;  
		rData.nLocOfColumn   = nLocOfColumn;

		rData.bConsiderAxialForce       = bConsiderAxialForce;
		rData.dConfidenceFactor         = dConfidenceFactor;
		rData.dScaleFactorWall          = dScaleFactorWall;
		rData.bScaleFactorBrittle_Use   = bScaleFactorBrittle_Use;
		rData.dScaleFactorBrittle       = dScaleFactorBrittle;
		rData.bScaleFactorEarth_Use     = bScaleFactorEarth_Use;
		rData.dScaleFactorEarth         = dScaleFactorEarth;
		rData.bScaleFactorSmoothBar_Use = bScaleFactorSmoothBar_Use;
		rData.dScaleFactorSmoothBar     = dScaleFactorSmoothBar;
		rData.SecondaryGroupKey         = SecondaryGroupKey;

		rData.bPermitFail = bPermitFail; // v741
		rData.nSubstep    = nSubstep;

		rData.bBuckling = bBuckling;

		rData.bShearYieldStop     = bShearYieldStop;
		rData.bShearYieldStopBeam = bShearYieldStopBeam;
		rData.bShearYieldStopWall = bShearYieldStopWall;

		rData.nInitLoadMethod = nInitLoadMethod;
		rData.aInitLoad.RemoveAll();  //rData.aInitLoad.Copy(aInitLoad);

		rData.bAxialYieldStop      = bAxialYieldStop     ;
		rData.bAxialYieldStopBeam  = bAxialYieldStopBeam ;
		rData.bAxialYieldStopWall  = bAxialYieldStopWall ;
		rData.bAxialYieldStopTruss = bAxialYieldStopTruss;
		rData.nPointSpringSupport  = nPointSpringSupport;
		rData.bSupportDzDirStop    = bSupportDzDirStop;
		rData.bSupportStopUplifting= bSupportStopUplifting;
		rData.bSupportStopCollapse = bSupportStopCollapse;
		rData.nNodeConnectivity    = nNodeConnectivity;
		rData.nGeomNonlinearType   = nGeomNonlinearType;
		rData.nBeamDivNumNy = nBeamDivNumNy;
		rData.nBeamDivNumNz = nBeamDivNumNz;
		rData.bWallConsOut  = bWallConsOut ;
		rData.nWallDivNumZ  = nWallDivNumZ ;
		rData.nWallDivNumY  = nWallDivNumY ;
		rData.dR            = dR           ;
		rData.nWAreaSize    = nWAreaSize   ;
		rData.bAssignByMember = bAssignByMember;
		rData.nRCDgnCode      = nRCDgnCode     ;
		rData.bUseAutoCalcReference   = bUseAutoCalcReference  ;
		rData.bShowPOCurveAfterAnalysis = bShowPOCurveAfterAnalysis;
		rData.bShowPOAnalGraph          = bShowPOAnalGraph         ;
		rData.nElasticLink = nElasticLink;
		rData.bConsiderIgnoreElem = bConsiderIgnoreElem;
		rData.bConsiderRebarArea1D = bConsiderRebarArea1D;
		rData.bConsiderRebarAreaWall = bConsiderRebarAreaWall;		
		rData.nFAreaSizeCore  = nFAreaSizeCore ;
		rData.nFAreaSizeCover = nFAreaSizeCover;
		rData.nWAreaSizeCover = nWAreaSizeCover;
		rData.nBeamDivNumNyCover = nBeamDivNumNyCover; 
		rData.nBeamDivNumNzCover = nBeamDivNumNzCover;
		rData.nWallDivNumZCover  = nWallDivNumZCover;  
		rData.nWallDivNumYCover  = nWallDivNumYCover;
		rData.bConvertWallStiffness = bConvertWallStiffness;

		rData.BoundaryNL = BoundaryNL;
		rData.bConsiderPDelta = bConsiderPDelta;
	}
	void SetPogd(T_POGD_D& rData)
	{
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm  = rData.bDisplNorm ;
		bForceNorm  = rData.bForceNorm ;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm  = rData.dDisplNorm ;
		dForceNorm  = rData.dForceNorm ;
		dEnergyNorm = rData.dEnergyNorm;
		bCalcCrackMoment = rData.bCalcCrackMoment;
		bTri_Symmetric = rData.bTri_Symmetric;
		dTri_Tens_a1   = rData.dTri_Tens_a1  ;
		dTri_Tens_a2   = rData.dTri_Tens_a2  ;
		dTri_Comp_a1   = rData.dTri_Comp_a1  ;
		dTri_Comp_a2   = rData.dTri_Comp_a2  ;
		bBi_Symmetric  = rData.bBi_Symmetric ;
		dBi_Tens_a1    = rData.dBi_Tens_a1   ;
		dBi_Comp_a1    = rData.dBi_Comp_a1   ;
		nLocOfBeam     = rData.nLocOfBeam;  
		nLocOfColumn   = rData.nLocOfColumn;
		nCountOldInitLoad = (int)rData.aOldInitLoad.GetSize();

		bConsiderAxialForce       = rData.bConsiderAxialForce;
		dConfidenceFactor         = rData.dConfidenceFactor;
		dScaleFactorWall          = rData.dScaleFactorWall;
		bScaleFactorBrittle_Use   = rData.bScaleFactorBrittle_Use;
		dScaleFactorBrittle       = rData.dScaleFactorBrittle;
		bScaleFactorEarth_Use     = rData.bScaleFactorEarth_Use;
		dScaleFactorEarth         = rData.dScaleFactorEarth;
		bScaleFactorSmoothBar_Use = rData.bScaleFactorSmoothBar_Use;
		dScaleFactorSmoothBar     = rData.dScaleFactorSmoothBar;
		SecondaryGroupKey         = rData.SecondaryGroupKey;

		bPermitFail = rData.bPermitFail; // v741
		nSubstep    = rData.nSubstep;

		bBuckling = rData.bBuckling;

		bShearYieldStop = rData.bShearYieldStop;
		bShearYieldStopBeam = rData.bShearYieldStopBeam;
		bShearYieldStopWall = rData.bShearYieldStopWall;

		nInitLoadMethod = rData.nInitLoadMethod;
		nCountInitLoad  = (int)rData.aInitLoad.GetSize();

		bAxialYieldStop      = rData.bAxialYieldStop     ;
		bAxialYieldStopBeam  = rData.bAxialYieldStopBeam ;
		bAxialYieldStopWall  = rData.bAxialYieldStopWall ;
		bAxialYieldStopTruss = rData.bAxialYieldStopTruss;
		nPointSpringSupport  = rData.nPointSpringSupport;
		bSupportDzDirStop    = rData.bSupportDzDirStop;
		bSupportStopUplifting= rData.bSupportStopUplifting;
		bSupportStopCollapse = rData.bSupportStopCollapse;
		nNodeConnectivity    = rData.nNodeConnectivity;
		nGeomNonlinearType   = rData.nGeomNonlinearType;
		nBeamDivNumNy = rData.nBeamDivNumNy;
		nBeamDivNumNz = rData.nBeamDivNumNz;
		bWallConsOut  = rData.bWallConsOut ;
		nWallDivNumZ  = rData.nWallDivNumZ ;
		nWallDivNumY  = rData.nWallDivNumY ;
		dR            = rData.dR           ;
		nWAreaSize    = rData.nWAreaSize   ;
		bAssignByMember = rData.bAssignByMember;
		nRCDgnCode      = rData.nRCDgnCode     ;
		bUseAutoCalcReference   = rData.bUseAutoCalcReference  ;
		bShowPOCurveAfterAnalysis = rData.bShowPOCurveAfterAnalysis;
		bShowPOAnalGraph          = rData.bShowPOAnalGraph         ;
		nElasticLink = rData.nElasticLink;
		bConsiderIgnoreElem = rData.bConsiderIgnoreElem;
		bConsiderRebarArea1D = rData.bConsiderRebarArea1D;
		bConsiderRebarAreaWall = rData.bConsiderRebarAreaWall;
		nFAreaSizeCore  = rData.nFAreaSizeCore ;
		nFAreaSizeCover = rData.nFAreaSizeCover;
		nWAreaSizeCover = rData.nWAreaSizeCover;
		nBeamDivNumNyCover = rData.nBeamDivNumNyCover;
		nBeamDivNumNzCover = rData.nBeamDivNumNzCover;
		nWallDivNumZCover  = rData.nWallDivNumZCover; 
		nWallDivNumYCover  = rData.nWallDivNumYCover;
		bConvertWallStiffness = rData.bConvertWallStiffness;

		BoundaryNL = rData.BoundaryNL;
		bConsiderPDelta = rData.bConsiderPDelta;
	}
};
struct T_POGD_UNIT
{
	T_POGD_UNIT()
	{
		nMaxIterNum = D_UNITSYS_NONE;
		// 그외 다른 변수들도 모두 단위없음.
	}
	int nMaxIterNum;
};
#define HASHSIZEPOGD 1

struct T_POFC_BASE                        // Increment Function --------------------
{  
	double dStep;				// Step
	double dLambda;			// Scale Factor  
};
#define T_POFC_K unsigned int
struct T_POFC_D
{
	unsigned int FunctionId;
	CString strName;          // Increment Control Function Name  
	CString strDescription;
	CArray<T_POFC_BASE, T_POFC_BASE&> aIncFuncData;
	
	void Initialize()
	{
		FunctionId = 0;
		strName.Empty();
		strDescription.Empty();
		aIncFuncData.RemoveAll();
	}
	T_POFC_D() {}
	T_POFC_D(T_POFC_D& src) { *this = src; }
	T_POFC_D& operator=(T_POFC_D& rData)
	{
		FunctionId = rData.FunctionId;
		strName = rData.strName;
		strDescription = rData.strDescription;    
		aIncFuncData.Copy(rData.aIncFuncData);
		return *this;
	}
};
struct T_POFC_D_CH
{
	UINT     FunctionId;
	char     strName[20];
	char     strDescription[80];
	CArray<T_POFC_BASE, T_POFC_BASE&> aIncFuncData;
	void ConvertToString(T_POFC_D& rData)
	{
		rData.FunctionId = FunctionId;
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDescription, rData.strDescription, sizeof(strDescription));
		rData.aIncFuncData.Copy(aIncFuncData);
	}
	void ConvertToChar(T_POFC_D& rData)
	{
		FunctionId = rData.FunctionId;
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDescription, strDescription, sizeof(strDescription));
		aIncFuncData.Copy(rData.aIncFuncData);
	}
};

struct T_POFC_UDRD_D
{
	T_POFC_K key;
	T_POFC_D data;
};
struct T_POFC_UDRD_D_CH
{
	T_POFC_K    key;
	T_POFC_D_CH data;
};
struct T_POFC_UDRD_D_CH_RW
{
	T_POFC_K key;
	UINT FunctionId;
	char strName[20];
	char strDescription[80];
	int  nIncFuncCount;
	void GetPofc(T_POFC_K& rKey, T_POFC_D_CH& rData)
	{
		rKey=key;
		rData.FunctionId = FunctionId;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strDescription, strDescription, sizeof(strDescription));
	}
	void SetPofc(const T_POFC_K Key, const T_POFC_D_CH& rData)
	{
		key=Key;
		FunctionId = rData.FunctionId;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nIncFuncCount = (int)rData.aIncFuncData.GetSize();
	}
};
// T_POFC_UNIT 없음.
//struct T_POFC_UNIT  // D_UNITSYS_NONE
#define HASHSIZEPOFC 11
#define D_POFC_MINNUM       1   // 입력가능한 최소 하중조건번호
#define D_POFC_MAXNUM    9999   // 입력가능한 최대 하중조건번호

///// Ver.7.2.1 이전의 데이터 --------------------------------------------------------------------------

// Hinge Data Type
#define T_HNGT_K unsigned int

struct T_HNGT_INTERACTION_590
{
	int nInterpolationType;   // 0:Ellipse, 1:Linear, 2:User
	int bSymmetric;
	double dAlpha;
	double dP[11];
	double dMy[2][11];
	double dMz[2][11];
	double dPmax;
	double dMmax;
	void Initialize()
	{
		nInterpolationType = 0;
		bSymmetric = FALSE;
		dAlpha = 1.0;
		for (int i = 0; i < 11; i++)
			dP[i] = 1.0 - i*0.2;
		for (int i = 0; i < 2; i++)
			for(int j=0; j<5; j++)
				dMy[i][j] = dMy[i][10-j] = dMz[i][j] = dMz[i][10-j] = (i==0)? 0.2*j:(-0.2*j);
		dMy[0][5] = dMz[0][5] = 1.0;
		dMy[1][5] = dMz[1][5] = -1.0;
		dPmax = 1.0;
		dMmax = 1.0;    
	}
};
struct T_HNGT_INTERACTION
{
	int nInterpolationType;   // 0:Ellipse, 1:Linear, 2:User
	int bSymmetric;
	double dAlpha;
	double dPy[2][11];     // 6.0.0 추가
	double dPz[2][11];     // 6.0.0 추가
	double dMy[2][11];     
	double dMz[2][11];
	double dPmax;
	double dMmax;
	double dMmaxcr;
	void Initialize()
	{
		nInterpolationType = 0;
		bSymmetric = FALSE;
		dAlpha = 1.0;
		for(int i = 0; i < 2; i++)
			for(int j = 0; j < 11; j++)
			{
				dPy[i][j] = 1.0 - j * 0.2;
				dPz[i][j] = 1.0 - j * 0.2;
			}
		for(int i = 0; i < 2; i++)
			for(int j = 0; j < 5; j++)
				dMy[i][j] = dMy[i][10-j] = dMz[i][j] = dMz[i][10-j] = (i==0)? 0.2*j:(-0.2*j);
		dMy[0][5] = dMz[0][5] = 1.0;
		dMy[1][5] = dMz[1][5] = -1.0;
		dPmax = 1.0;
		dMmax = 1.0;
		dMmaxcr = 1.0;
	}
	void Convert590(T_HNGT_INTERACTION_590& rData)
	{
		nInterpolationType = rData.nInterpolationType;
		bSymmetric = rData.bSymmetric;
		dAlpha = rData.dAlpha;
		for(int i = 0; i < 2; i++)
			for(int j = 0; j < 11; j++)
			{
				dPy[i][j] = rData.dP[j];
				dPz[i][j] = rData.dP[j];
			}
		for(int i = 0; i < 2; i++)
			for(int j = 0; j < 11; j++)
			{
				dMy[i][j] = rData.dMy[i][j];        
				dMz[i][j] = rData.dMz[i][j];        
			}
		dPmax = rData.dPmax;
		dMmax = rData.dMmax;
		dMmaxcr = 1.0;
	}
};
struct T_HNGT_INTERACTION_UNIT
{
	T_HNGT_INTERACTION_UNIT()
	{
		dPmax = D_UNITSYS_BASE_FORCE;
		dMmax = D_UNITSYS_BASE_MOMENT;
		dMmaxcr = D_UNITSYS_BASE_MOMENT;
	}
	int dPmax;
	int dMmax;
	int dMmaxcr;
};
struct T_HNGT_PROP_590
{
	double dForceMoment[2][9];    // 0:Y, 1:Z   (P-type, T-type은 0만 사용)
	double dDisplRotation[2][9];  // 0:Y, 1:Z   (P-type, T-type은 0만 사용)
	BOOL bSymmetric;
	BOOL bUseCalcValue;
	double dYieldMoment[4];   // Moment-y,z, P-My-Mz, Torsion-T
	double dYieldRotation[4]; // Moment-y,z, P-My-Mz, Torsion-T, Shear-y,z
	double dYieldForce[4];    // Axial-P, Shear-y,z
	double dYieldLength[4];   // Axial-P
	double dIO[2];            // Immediate Occupancy
	double dLS[2];            // Life Safety
	double dCP[2];            // Collapse Prevention
	T_HNGT_INTERACTION_590 IS;    // Interaction Surface
};
struct T_HNGT_PROP_711
{
	double dForceMoment[2][9];    // 0:Y, 1:Z   (P-type, T-type은 0만 사용)
	double dDisplRotation[2][9];  // 0:Y, 1:Z   (P-type, T-type은 0만 사용)
	BOOL bSymmetric;
	BOOL bUseCalcValue;
	double dYieldMoment[4];   // Moment-y,z, P-My-Mz, Torsion-T
	double dYieldRotation[4]; // Moment-y,z, P-My-Mz, Torsion-T, Shear-y,z
	double dYieldForce[4];    // Axial-P, Shear-y,z
	double dYieldLength[4];   // Axial-P
	double dIO[2];            // Immediate Occupancy
	double dLS[2];            // Life Safety
	double dCP[2];            // Collapse Prevention
	T_HNGT_INTERACTION IS;    // Interaction Surface  
	int    nMultiLinearType;      // 6.0.0 추가  0: Bilinear 1:Trilinear
	BOOL   bHysUseCalcValue;
	BOOL   bSymmetricMoment;      // 6.0.0 추가
	BOOL   bSymmetricAlpha;       // 6.0.0 추가
	double dHysYieldMoment[4];    // 6.0.0 추가
	double dHysCrackMoment[4];    // 6.0.0 추가
	double dHysYieldForce[4];     // 6.0.0 추가
	double dHysCrackForce[4];     // 6.0.0 추가
	double dHysStifAlpha[4];      // 6.0.0 추가
};
struct T_HNGT_PROP
{
	double dForceMoment[2][9];    // 0:Y, 1:Z   (P-type, T-type은 0만 사용)
	double dDisplRotation[2][9];  // 0:Y, 1:Z   (P-type, T-type은 0만 사용)
	BOOL bSymmetric;
	BOOL bUseCalcValue;
	double dYieldMoment[4];   // Moment-y,z, P-My-Mz, Torsion-T
	double dYieldRotation[4]; // Moment-y,z, P-My-Mz, Torsion-T, Shear-y,z
	double dYieldForce[4];    // Axial-P, Shear-y,z
	double dYieldLength[4];   // Axial-P
	double dIO[2];            // Immediate Occupancy
	double dLS[2];            // Life Safety
	double dCP[2];            // Collapse Prevention
	T_HNGT_INTERACTION IS;    // Interaction Surface  
	int    nMultiLinearType;   // 6.0.0 추가  0: Bilinear 1:Trilinear
	BOOL   bHysUseCalcValue;
	BOOL   bSymmetricMoment;   // 6.0.0 추가
	BOOL   bSymmetricAlpha;    // 6.0.0 추가
	double dHysYieldMoment[4]; // 6.0.0 추가
	double dHysCrackMoment[4]; // 6.0.0 추가
	double dHysYieldForce[4];  // 6.0.0 추가
	double dHysCrackForce[4];  // 6.0.0 추가
	double dHysStifAlpha[4];   // 6.0.0 추가
	int    nInitStiff;         // 7.1.2 추가 (0: 6EI/L, 1: 3EI/L, 2: 2EI/L)
	void Initialize()
	{
		double FM[] = {-0.2, -0.2, -1.25, -1, 0, 1, 1.25, 0.2, 0.2};
		double DR[] = {-8, -6, -6, -1, 0, 1, 6, 6, 8 };
		for (int n = 0; n < 2; n++)
		{
			for (int i = 0; i < 9; i++)
			{
				dForceMoment[n][i] = FM[i];
				dDisplRotation[n][i] = DR[i];
			}
		}
		bSymmetric = TRUE;
		bUseCalcValue = TRUE;
		for (int i = 0; i < 4; i= i+2)
			dYieldMoment[i] = dYieldRotation[i] = dYieldForce[i] = dYieldLength[i] = 1.0;
		for (int i = 1; i < 4; i= i+2)
			dYieldMoment[i] = dYieldRotation[i] = dYieldForce[i] = dYieldLength[i] = -1.0;
		dIO[0] = 2.0; dIO[1] = -2.0;
		dLS[0] = 4.0; dLS[1] = -4.0;
		dCP[0] = 6.0; dCP[1] = -6.0;
		IS.Initialize();    
		nMultiLinearType = 0;
		bHysUseCalcValue = TRUE;
		bSymmetricMoment = FALSE;
		bSymmetricAlpha = TRUE;
		dHysYieldMoment[0] = dHysCrackMoment[0] = dHysYieldMoment[2] = dHysCrackMoment[2] = 1.0;
		dHysYieldMoment[1] = dHysCrackMoment[1] = dHysYieldMoment[3] = dHysCrackMoment[3] = -1.0;
		dHysYieldForce[0] = dHysCrackForce[0] = dHysYieldForce[2] = dHysCrackForce[2] = 1.0;
		dHysYieldForce[1] = dHysCrackForce[1] = dHysYieldForce[3] = dHysCrackForce[3] = -1.0;
		dHysStifAlpha[0] = dHysStifAlpha[1] = 0.3;
		dHysStifAlpha[2] = dHysStifAlpha[3] = 0.1;
		nInitStiff = 0;
	}
	void Convert590(T_HNGT_PROP_590& rData)
	{
		for(int i = 0; i < 2; i++)
		{
			for(int j = 0; j < 9; j++)
			{
				dForceMoment[i][j] = rData.dForceMoment[i][j];
				dDisplRotation[i][j] = rData.dDisplRotation[i][j];  
			}
		}
		bSymmetric = rData.bSymmetric;
		bUseCalcValue = rData.bUseCalcValue;
		for(int i = 0; i < 4; i++)
		{
			dYieldMoment[i]   = rData.dYieldMoment[i];   
			dYieldRotation[i] = rData.dYieldRotation[i]; 
			dYieldForce[i]    = rData.dYieldForce[i];    
			dYieldLength[i]   = rData.dYieldLength[i];   
		}
		for(int i = 0; i < 2; i++)
		{
			dIO[i] = rData.dIO[i];            
			dLS[i] = rData.dLS[i];            
			dCP[i] = rData.dCP[i];            
		}
		IS.Convert590(rData.IS);        
		nMultiLinearType = 0;
		bHysUseCalcValue = TRUE;
		bSymmetricMoment = FALSE;
		bSymmetricAlpha = TRUE;
		dHysYieldMoment[0] = dHysCrackMoment[0] = dHysYieldMoment[2] = dHysCrackMoment[2] = 1.0;
		dHysYieldMoment[1] = dHysCrackMoment[1] = dHysYieldMoment[3] = dHysCrackMoment[3] = -1.0;
		dHysYieldForce[0] = dHysCrackForce[0] = dHysYieldForce[2] = dHysCrackForce[2] = 1.0;
		dHysYieldForce[1] = dHysCrackForce[1] = dHysYieldForce[3] = dHysCrackForce[3] = -1.0;
		dHysStifAlpha[0] = dHysStifAlpha[1] = 0.3;
		dHysStifAlpha[2] = dHysStifAlpha[3] = 0.1;
		nInitStiff = 0;
	}
	void Convert711(T_HNGT_PROP_711& rData)
	{
		memcpy(dForceMoment,   rData.dForceMoment,   sizeof(dForceMoment));
		memcpy(dDisplRotation, rData.dDisplRotation, sizeof(dDisplRotation));
		
		bSymmetric = rData.bSymmetric;
		bUseCalcValue = rData.bUseCalcValue;

		memcpy(dYieldMoment,   rData.dYieldMoment,   sizeof(dYieldMoment));
		memcpy(dYieldRotation, rData.dYieldRotation, sizeof(dYieldRotation));
		memcpy(dYieldForce,    rData.dYieldForce,    sizeof(dYieldForce));
		memcpy(dYieldLength,   rData.dYieldLength,   sizeof(dYieldLength));

		memcpy(dIO, rData.dIO, sizeof(dIO));
		memcpy(dLS, rData.dLS, sizeof(dLS));
		memcpy(dCP, rData.dCP, sizeof(dCP));
		
		IS = rData.IS;

		nMultiLinearType = rData.nMultiLinearType;
		bHysUseCalcValue = rData.bHysUseCalcValue;
		bSymmetricMoment = rData.bSymmetricMoment;
		bSymmetricAlpha  = rData.bSymmetricAlpha;

		memcpy(dHysYieldMoment, rData.dHysYieldMoment, sizeof(dHysYieldMoment));
		memcpy(dHysCrackMoment, rData.dHysCrackMoment, sizeof(dHysCrackMoment));
		memcpy(dHysYieldForce, rData.dHysYieldForce, sizeof(dHysYieldForce));
		memcpy(dHysCrackForce, rData.dHysCrackForce, sizeof(dHysCrackForce));
		memcpy(dHysStifAlpha, rData.dHysStifAlpha, sizeof(dHysStifAlpha));

		nInitStiff = 0;
	}
};
struct T_HNGT_PROP_UNIT
{
	T_HNGT_PROP_UNIT()
	{
		dYieldMoment = D_UNITSYS_BASE_MOMENT;
		dYieldRotation = D_UNITSYS_BASE_RADIAN;
		dYieldForce = D_UNITSYS_BASE_FORCE;
		dYieldLength = D_UNITSYS_BASE_LENGTH;
		dHysYieldMoment = D_UNITSYS_BASE_MOMENT;
		dHysCrackMoment = D_UNITSYS_BASE_MOMENT;
		dHysYieldForce = D_UNITSYS_BASE_FORCE;
		dHysCrackForce = D_UNITSYS_BASE_FORCE;
	}
	int dYieldMoment;
	int dYieldRotation;
	int dYieldForce;
	int dYieldLength;
	int dHysYieldMoment;
	int dHysCrackMoment;
	int dHysYieldForce;
	int dHysCrackForce;
	T_HNGT_INTERACTION_UNIT IS;
};
struct T_HNGT_D_590
{
	CString strTypeName;
	int     nDataType;    // 0:P, 1:My-Mz, 2:Vy-Vz, 3:PMM, 4:User
	int     nHingeType;   // 0:Axial-P, 1:Moment-y,z, 2:Shear-y,z, 3:P-My-Mz, 4:Torsion-T
	T_HNGT_PROP_590 Prop;
	int     nSeq;
};
struct T_HNGT_D_711
{
	CString strTypeName;
	int     nDataType;    // 0:P, 1:My-Mz, 2:Vy-Vz, 3:PMM, 4:User
	int     nHingeType;   // 0:Axial-P, 1:Moment-y,z, 2:Shear-y,z, 3:P-My-Mz, 4:Torsion-T
	T_HNGT_PROP_711 Prop;
	int     nSeq;
	int     nMultiATCType;    // 6.0.0 추가  0: Multi-Linear, 1:ATC
};
struct T_HNGT_D
{
	CString strTypeName;
	int     nDataType;    // 0:P, 1:My-Mz, 2:Vy-Vz, 3:PMM, 4:User
	int     nHingeType;   // 0:Axial-P, 1:Moment-y,z, 2:Shear-y,z, 3:P-My-Mz, 4:Torsion-T
	T_HNGT_PROP Prop;
	int     nSeq;
	int     nMultiATCType;    // 6.0.0 추가  0: Multi-Linear, 1:ATC
	void Initialize()
	{
		strTypeName.Empty();
		nDataType = 0;
		nHingeType = 0;
		Prop.Initialize();
		nSeq = 0;
		nMultiATCType = 1;
	}
};
struct T_HNGT_D_CH_590
{
	char    strTypeName[80];
	int     nDataType;
	int     nHingeType;
	T_HNGT_PROP_590 Prop;
	int     nSeq;
};
struct T_HNGT_D_CH_711
{
	char    strTypeName[80];
	int     nDataType;
	int     nHingeType;
	T_HNGT_PROP_711 Prop;
	int     nSeq;
	int     nMultiATCType;
};
struct T_HNGT_D_CH
{
	char    strTypeName[80];
	int     nDataType;
	int     nHingeType;
	T_HNGT_PROP Prop;
	int     nSeq;
	int     nMultiATCType;    // 6.0.0 추가  0: Multi-Linear, 1:ATC
	void ConvertToString(T_HNGT_D& rData)
	{
		ConvertCharStr(strTypeName, rData.strTypeName, sizeof(strTypeName));
		rData.nDataType  = nDataType;
		rData.nHingeType = nHingeType;
		rData.Prop = Prop;
		rData.nSeq = nSeq;
		rData.nMultiATCType = nMultiATCType;
	}
	void ConvertToChar(T_HNGT_D& rData)
	{
		ConvertStrChar(rData.strTypeName, strTypeName, sizeof(strTypeName));
		nDataType = rData.nDataType;
		nHingeType = rData.nHingeType;
		Prop = rData.Prop;
		nSeq = rData.nSeq;
		nMultiATCType = rData.nMultiATCType;
	}
	void Convert590(T_HNGT_D_CH_590& rData)
	{    
		memcpy(strTypeName, rData.strTypeName, sizeof(strTypeName));    
		nDataType = rData.nDataType;
		nHingeType = rData.nHingeType;
		Prop.Convert590(rData.Prop);
		nSeq = rData.nSeq;
		nMultiATCType = 1;
	}
	void Convert711(T_HNGT_D_CH_711& rData)
	{    
		memcpy(strTypeName, rData.strTypeName, sizeof(strTypeName));    
		nDataType  = rData.nDataType;
		nHingeType = rData.nHingeType;
		Prop.Convert711(rData.Prop);
		nSeq = rData.nSeq;
		nMultiATCType = rData.nMultiATCType;
	}
};
struct T_HNGT_UNIT
{
	T_HNGT_PROP_UNIT Prop;
};
struct T_HNGT_UDRD_D
{
	T_HNGT_K key;
	T_HNGT_D data;
};
struct T_HNGT_UDRD_D_CH_590
{
	T_HNGT_K key;
	T_HNGT_D_CH_590 data;
};
struct T_HNGT_UDRD_D_CH_711
{
	T_HNGT_K key;
	T_HNGT_D_CH_711 data;
};
struct T_HNGT_UDRD_D_CH
{
	T_HNGT_K key;
	T_HNGT_D_CH data;
};
#define HASHSIZEHNGT 11

// Pushover Load Case
struct T_POLD_BASE
{
	T_STLD_K LoadCaseKey; // nLoadPatternType에 따라 Static Load Case Key, 
												// Acceleration(1=X, 2=Y, 3=Z), Mode Shape No
	double dScaleFactor;

	void Initialize()
	{
		LoadCaseKey = 0;
		dScaleFactor = 1.0;
	}
};
#define T_POLD_DT CArray<T_POLD_BASE, T_POLD_BASE&>
#define T_POLD_K unsigned int
struct T_POLD_D_590
{
	CString LoadCaseName;
	CString Description;
	int nControlOption;   // 1=Global Control, 2=Master Node Control
	double dOverallMaxDisp; // nControlOption==1
	T_NODE_K MasterNode;    // nControlOption==2
	int MasterDirection;    // nControlOption==2 : 1=DX, 2=DY, 3=DZ
	double dMasterMaxDisp;  // nControlOption==2
	int nAnalysisOption;    // 0=None, 1=P-Delta, 2=Non-Linear
	BOOL bUsePrevPold;
	int nLoadPatternType; // 0=Static Load, 1=Acceleration, 2=Mode Shape
	T_POLD_DT aLoadPattern;
	unsigned int LoadCaseId;
	void Initialize()
	{
		LoadCaseName.Empty();
		Description.Empty();
		nControlOption = 1;
		dOverallMaxDisp = 0.0;
		MasterNode = 1;
		MasterDirection = 1;
		dMasterMaxDisp = 0.0;
		nAnalysisOption = 1;
		bUsePrevPold = FALSE;
		nLoadPatternType = 0;
		aLoadPattern.RemoveAll();
		LoadCaseId=0;
	}  
};
struct T_POLD_D_600
{
	CString LoadCaseName;
	CString Description;
	int nControlOption;   // 1=Global Control, 2=Master Node Control
	double dOverallMaxDisp; // nControlOption==1
	T_NODE_K MasterNode;    // nControlOption==2
	int MasterDirection;    // nControlOption==2 : 1=DX, 2=DY, 3=DZ
	double dMasterMaxDisp;  // nControlOption==2
	int nAnalysisOption;    // 0=None, 1=P-Delta, 2=Non-Linear
	BOOL bUsePrevPold;
	int nLoadPatternType; // 0=Static Load, 1=Acceleration, 2=Mode Shape
	T_POLD_DT aLoadPattern;
	unsigned int LoadCaseId;
	int    nIncrementMethod;      // 6.0.0 추가  0: Force Control 1:Displacement Control
	double dCollapseLoad;         // 6.0.0 추가
	int    nNumofStep;            // 6.0.0 추가
	double dInterStoryDeformAngle; // 6.0.0 추가		//-> 이건 이제 안쓴다...From v691... By HP
	double dHorizontalCapacity;    // 6.0.0 추가
	void Initialize()
	{
		LoadCaseName.Empty();
		Description.Empty();
		nControlOption = 1;
		dOverallMaxDisp = 0.0;
		MasterNode = 1;
		MasterDirection = 1;
		dMasterMaxDisp = 0.0;
		nAnalysisOption = 1;
		bUsePrevPold = FALSE;
		nLoadPatternType = 0;
		aLoadPattern.RemoveAll();
		LoadCaseId=0;
		nIncrementMethod = 1;
		dCollapseLoad = 0.4;      
		nNumofStep = 5;           
		dInterStoryDeformAngle = 0.1;
		dHorizontalCapacity = 1.0;   
	}
};
struct T_POLD_D
{
	CString LoadCaseName;
	CString Description;
	int nControlOption;   // 1=Global Control, 2=Master Node Control
	double dOverallMaxDisp; // nControlOption==1
	T_NODE_K MasterNode;    // nControlOption==2
	int MasterDirection;    // nControlOption==2 : 1=DX, 2=DY, 3=DZ
	double dMasterMaxDisp;  // nControlOption==2
	int nAnalysisOption;    // 0=None, 1=P-Delta, 2=Non-Linear
	BOOL bUsePrevPold;
	int nLoadPatternType; // 0=Static Load, 1=Acceleration, 2=Mode Shape, 3=Mode Shape * Story Mass
	T_POLD_DT aLoadPattern;
	unsigned int LoadCaseId;
	int    nIncrementMethod;      // 6.0.0 추가  0: Force Control 1:Displacement Control
	double dCollapseLoad;         // 6.0.0 추가
	int    nNumofStep;            // 6.0.0 추가
	double dInterStoryDeformAngle;		// 6.0.0 추가		//-> 이건 이제 안쓴다...From v691... By HP	
	double dHorizontalCapacity;    // 6.0.0 추가
	double dNewInterStoryDeformAngle; // 6.9.1 에서 새로 추가.....MNET1514-HP 2006.06.24
	void Initialize()
	{
		LoadCaseName.Empty();
		Description.Empty();
		nControlOption = 1;
		dOverallMaxDisp = 0.0;
		MasterNode = 1;
		MasterDirection = 1;
		dMasterMaxDisp = 0.0;
		nAnalysisOption = 0;    // 1에서 0으로 변경 2005.08.05 by 강은경 요청
		bUsePrevPold = FALSE;
		nLoadPatternType = 0;
		aLoadPattern.RemoveAll();
		LoadCaseId=0;
		nIncrementMethod = 1;
		dCollapseLoad = 0.4;      
		nNumofStep = 5;           
		dInterStoryDeformAngle = 0.1;
		dHorizontalCapacity = 1.0;   
		dNewInterStoryDeformAngle = 10;
	}
	T_POLD_D()
	{
	}
	T_POLD_D(T_POLD_D& src)
	{
		*this = src;
	}
	T_POLD_D& operator=(T_POLD_D& src)
	{
		LoadCaseName=src.LoadCaseName;
		Description=src.Description;
		nControlOption = src.nControlOption;
		dOverallMaxDisp = src.dOverallMaxDisp;
		MasterNode = src.MasterNode;
		MasterDirection = src.MasterDirection;
		dMasterMaxDisp = src.dMasterMaxDisp;
		nAnalysisOption = src.nAnalysisOption;
		bUsePrevPold = src.bUsePrevPold;
		nLoadPatternType = src.nLoadPatternType;
		aLoadPattern.Copy(src.aLoadPattern);
		LoadCaseId=src.LoadCaseId;
		nIncrementMethod = src.nIncrementMethod;
		dCollapseLoad = src.dCollapseLoad;      
		nNumofStep = src.nNumofStep;           
		dInterStoryDeformAngle = src.dInterStoryDeformAngle;
		dHorizontalCapacity = src.dHorizontalCapacity;   
		dNewInterStoryDeformAngle = src.dNewInterStoryDeformAngle;
		return *this;
	}
};
struct T_POLD_D_CH
{
	char LoadCaseName[20];
	char Description[80];
	int nControlOption;   // 1=Global Control, 2=Master Node Control
	double dOverallMaxDisp; // nControlOption==1
	T_NODE_K MasterNode;    // nControlOption==2
	int MasterDirection;    // nControlOption==2 : 1=DX, 2=DY, 3=DZ, 4=RX, 5=RY, 6=RZ
	double dMasterMaxDisp;  // nControlOption==2
	int nAnalysisOption;
	BOOL bUsePrevPold;
	int nLoadPatternType;   // 0=stld, 1=acceleration, 2=mode shape, 3=mode shape * story mass
	T_POLD_DT aLoadPattern;
	unsigned int LoadCaseId;
	int    nIncrementMethod;       // 6.0.0 추가  0: Force Control 1:Displacement Control
	double dCollapseLoad;          // 6.0.0 추가
	int    nNumofStep;             // 6.0.0 추가
	double dInterStoryDeformAngle; // 6.0.0 추가	
	double dHorizontalCapacity;    // 6.0.0 추가
	double dNewInterStoryDeformAngle; // 6.9.1 에서 새로 추가.....MNET1514-HP 2006.06.24
	void ConvertToString(T_POLD_D& rData)
	{
		ConvertCharStr(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.nControlOption = nControlOption;
		rData.dOverallMaxDisp = dOverallMaxDisp;
		rData.MasterNode = MasterNode;
		rData.MasterDirection = MasterDirection;
		rData.dMasterMaxDisp = dMasterMaxDisp;
		rData.nAnalysisOption = nAnalysisOption;
		rData.bUsePrevPold = bUsePrevPold;
		rData.nLoadPatternType = nLoadPatternType;
		rData.aLoadPattern.Copy(aLoadPattern);
		rData.LoadCaseId=LoadCaseId;
		rData.nIncrementMethod = nIncrementMethod;
		rData.dCollapseLoad = dCollapseLoad;
		rData.nNumofStep = nNumofStep;
		rData.dInterStoryDeformAngle = dInterStoryDeformAngle;
		rData.dHorizontalCapacity = dHorizontalCapacity;
		rData.dNewInterStoryDeformAngle = dNewInterStoryDeformAngle;
	}
	void ConvertToChar(T_POLD_D& rData)
	{
		ConvertStrChar(rData.LoadCaseName, LoadCaseName, sizeof(LoadCaseName));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		nControlOption = rData.nControlOption;
		dOverallMaxDisp = rData.dOverallMaxDisp;
		MasterNode = rData.MasterNode;
		MasterDirection = rData.MasterDirection;
		dMasterMaxDisp = rData.dMasterMaxDisp;
		nAnalysisOption = rData.nAnalysisOption;
		bUsePrevPold = rData.bUsePrevPold;
		nLoadPatternType = rData.nLoadPatternType;
		aLoadPattern.Copy(rData.aLoadPattern);
		LoadCaseId=rData.LoadCaseId;
		nIncrementMethod = rData.nIncrementMethod;
		dCollapseLoad = rData.dCollapseLoad;
		nNumofStep = rData.nNumofStep;
		dInterStoryDeformAngle = rData.dInterStoryDeformAngle;
		dHorizontalCapacity = rData.dHorizontalCapacity;
		dNewInterStoryDeformAngle = rData.dNewInterStoryDeformAngle;
	}
};
struct T_POLD_UDRD_D
{
	T_POLD_K key;
	T_POLD_D data;
};
struct T_POLD_UDRD_D_CH
{
	T_POLD_K key;
	T_POLD_D_CH data;
};
struct T_POLD_UDRD_D_CH_RW_590
{
	T_POLD_K key;
	char LoadCaseName[20];
	char Description[80];
	int nControlOption;
	double dOverallMaxDisp;
	T_NODE_K MasterNode;
	int MasterDirection;
	double dMasterMaxDisp;
	int nAnalysisOption;
	BOOL bUsePrevPold;
	int nLoadPatternType;
	unsigned int LoadCaseId;
	int nCountLoadPattern;
};
struct T_POLD_UDRD_D_CH_RW_600
{
	T_POLD_K key;
	char LoadCaseName[20];
	char Description[80];
	int nControlOption;
	double dOverallMaxDisp;
	T_NODE_K MasterNode;
	int MasterDirection;
	double dMasterMaxDisp;
	int nAnalysisOption;
	BOOL bUsePrevPold;
	int nLoadPatternType;
	unsigned int LoadCaseId;
	int nCountLoadPattern;
	int    nIncrementMethod;       // 6.0.0 추가  0: Force Control 1:Displacement Control
	double dCollapseLoad;          // 6.0.0 추가
	int    nNumofStep;             // 6.0.0 추가
	double dInterStoryDeformAngle; // 6.0.0 추가
	double dHorizontalCapacity;    // 6.0.0 추가
};
struct T_POLD_UDRD_D_CH_RW
{
	T_POLD_K key;
	char LoadCaseName[20];
	char Description[80];
	int nControlOption;
	double dOverallMaxDisp;
	T_NODE_K MasterNode;
	int MasterDirection;
	double dMasterMaxDisp;
	int nAnalysisOption;
	BOOL bUsePrevPold;
	int nLoadPatternType;
	unsigned int LoadCaseId;
	int nCountLoadPattern;
	int    nIncrementMethod;       // 6.0.0 추가  0: Force Control 1:Displacement Control
	double dCollapseLoad;          // 6.0.0 추가
	int    nNumofStep;             // 6.0.0 추가
	double dInterStoryDeformAngle; // 6.0.0 추가
	double dHorizontalCapacity;    // 6.0.0 추가
	double dNewInterStoryDeformAngle; // 6.9.1 에서 새로 추가.....MNET1514-HP 2006.06.24
	void GetPold(T_POLD_K& rKey, T_POLD_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.LoadCaseName, LoadCaseName, sizeof(LoadCaseName));
		memcpy(rData.Description, Description, sizeof(Description));
		rData.nControlOption = nControlOption;
		rData.dOverallMaxDisp = dOverallMaxDisp;
		rData.MasterNode = MasterNode;
		rData.MasterDirection = MasterDirection;
		rData.dMasterMaxDisp = dMasterMaxDisp;
		rData.nAnalysisOption = nAnalysisOption;
		rData.bUsePrevPold = bUsePrevPold;
		rData.nLoadPatternType = nLoadPatternType;
		rData.LoadCaseId=LoadCaseId;
		rData.nIncrementMethod = nIncrementMethod;
		rData.dCollapseLoad = dCollapseLoad;
		rData.nNumofStep = nNumofStep;
		rData.dInterStoryDeformAngle = dInterStoryDeformAngle;
		rData.dHorizontalCapacity = dHorizontalCapacity;
		rData.dNewInterStoryDeformAngle = dNewInterStoryDeformAngle;
	}
	void SetPold(const T_POLD_K Key, const T_POLD_D_CH& rData)
	{
		key=Key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		nControlOption = rData.nControlOption;
		dOverallMaxDisp = rData.dOverallMaxDisp;
		MasterNode = rData.MasterNode;
		MasterDirection = rData.MasterDirection;
		dMasterMaxDisp = rData.dMasterMaxDisp;
		nAnalysisOption = rData.nAnalysisOption;
		bUsePrevPold = rData.bUsePrevPold;
		nLoadPatternType = rData.nLoadPatternType;
		LoadCaseId=rData.LoadCaseId;
		nCountLoadPattern=(int)rData.aLoadPattern.GetSize();
		nIncrementMethod = rData.nIncrementMethod;
		dCollapseLoad = rData.dCollapseLoad;
		nNumofStep = rData.nNumofStep;
		dInterStoryDeformAngle = rData.dInterStoryDeformAngle;
		dHorizontalCapacity = rData.dHorizontalCapacity;
		dNewInterStoryDeformAngle = rData.dNewInterStoryDeformAngle;
	}
	void Convert590(T_POLD_UDRD_D_CH_RW_590& rData)
	{
		key=rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		nControlOption = rData.nControlOption;
		dOverallMaxDisp = rData.dOverallMaxDisp;
		MasterNode = rData.MasterNode;
		MasterDirection = rData.MasterDirection;
		dMasterMaxDisp = rData.dMasterMaxDisp;
		nAnalysisOption = rData.nAnalysisOption;
		bUsePrevPold = rData.bUsePrevPold;
		nLoadPatternType = rData.nLoadPatternType;
		LoadCaseId=rData.LoadCaseId;
		nCountLoadPattern=rData.nCountLoadPattern;
		nIncrementMethod = 1;
		dCollapseLoad = 0.4;
		nNumofStep = 5;
		dInterStoryDeformAngle = 0.1;
		dHorizontalCapacity = 1.0;
		dNewInterStoryDeformAngle = 10;
	}
	void Convert600(T_POLD_UDRD_D_CH_RW_600& rData)
	{
		key=rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		nControlOption = rData.nControlOption;
		dOverallMaxDisp = rData.dOverallMaxDisp;
		MasterNode = rData.MasterNode;
		MasterDirection = rData.MasterDirection;
		dMasterMaxDisp = rData.dMasterMaxDisp;
		nAnalysisOption = rData.nAnalysisOption;
		bUsePrevPold = rData.bUsePrevPold;
		nLoadPatternType = rData.nLoadPatternType;
		LoadCaseId=rData.LoadCaseId;
		nCountLoadPattern=rData.nCountLoadPattern;
		nIncrementMethod = rData.nIncrementMethod;
		dCollapseLoad = rData.dCollapseLoad;
		nNumofStep = rData.nNumofStep;
		dInterStoryDeformAngle = rData.dInterStoryDeformAngle;
		dHorizontalCapacity = rData.dHorizontalCapacity;
		dNewInterStoryDeformAngle = 1/rData.dInterStoryDeformAngle;   // 6.9.1 에서 새로 추가.....MNET1514-HP 2006.06.24
	}
};
struct T_POLD_UNIT
{
	T_POLD_UNIT()
	{
		dOverallMaxDisp = D_UNITSYS_BASE_LENGTH;
		dMasterMaxDisp = D_UNITSYS_BASE_LENGTH;
	}
	int dOverallMaxDisp;
	int dMasterMaxDisp;
};
#define HASHSIZEPOLD 11
#define D_POLD_MINNUM       1   // 입력가능한 최소 하중조건번호
#define D_POLD_MAXNUM    9999   // 입력가능한 최대 하중조건번호

#define T_HNGE_KEY unsigned int
union T_HNGE_K
{
	T_HNGE_KEY keymap;
	struct
	{
		unsigned int entity : 20;
		unsigned int pos    : 12; // 1:I, 2:J, 3:CP, 4:CV, 5:CT
	} key;
};
struct T_HNGE_D
{
	T_HNGT_K HingeType;
	int nHingeLocation; // 1:I, 2:Center, 3:J
};
struct T_HNGE_UDRD_D
{
	T_HNGE_K key;
	T_HNGE_D data;
};
struct T_HNGE_UNIT
{
	T_HNGE_UNIT()
	{
		HingeType = D_UNITSYS_NONE;
	}
	int HingeType;
};
#define HASHSIZEHNGE      HASHSIZEELEM

// Pushover analysis control
#define T_POCT_K unsigned int
struct T_POCT_D_711
{
	int nPushStepNum;
	int nMaxIterNum;
	double dConvTol;
};
struct T_POCT_D
{
	int nPushStepNum;
	int nMaxIterNum;
	double dConvTol;    // 7.1.2부터 사용안함
	BOOL bDisplNorm;    // 7.1.2추가
	BOOL bForceNorm;    // 7.1.2추가
	BOOL bEnergyNorm;   // 7.1.2추가
	double dDisplNorm;  // 7.1.2추가
	double dForceNorm;  // 7.1.2추가
	double dEnergyNorm; // 7.1.2추가
	void Initialize()
	{
		nPushStepNum = 10;
		nMaxIterNum = 10;
		dConvTol = 1.0e-3;
		bDisplNorm = TRUE;
		bForceNorm = bEnergyNorm = FALSE;
		dDisplNorm = dForceNorm = dEnergyNorm = 0.001;
	}
	void Convert711(T_POCT_D_711& rData)
	{
		nPushStepNum = rData.nPushStepNum;
		nMaxIterNum  = rData.nMaxIterNum;
		dConvTol     = rData.dConvTol;
		bDisplNorm = TRUE;
		bForceNorm = bEnergyNorm = FALSE;
		dDisplNorm = dForceNorm = dEnergyNorm = 0.001;
	}
};
struct T_POCT_UDRD_D_711
{
	T_POCT_D_711 data;
};
struct T_POCT_UDRD_D
{
	T_POCT_D data;
};
struct T_POCT_UNIT
{
	T_POCT_UNIT()
	{
		nPushStepNum = D_UNITSYS_NONE;
	}
	int nPushStepNum;
};
#define HASHSIZEPOCT 1

// previous pushover load case
#define T_PPLC_K unsigned int
struct T_PPLC_D
{
	T_POLD_DT aLoadPattern; // always static
	void Initialize()
	{
		aLoadPattern.RemoveAll();
	}
	T_PPLC_D()
	{
	}
	T_PPLC_D(T_PPLC_D& src)
	{
		*this = src;
	}
	T_PPLC_D& operator=(T_PPLC_D& src)
	{
		aLoadPattern.Copy(src.aLoadPattern);
		return *this;
	}
};
struct T_PPLC_UDRD_D
{
	T_PPLC_D data;
};
struct T_PPLC_UDRD_D_RW
{
	int nCountLoadPattern;
	void GetPplc(T_PPLC_D& rData)
	{
	}
	void SetPplc(T_PPLC_D& rData)
	{
		nCountLoadPattern = (int)rData.aLoadPattern.GetSize();
	}
};
struct T_PPLC_UNIT
{
	T_PPLC_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEPPLC 1

// Additional Step for Pushover analysis
#define T_POAS_K unsigned int
#define D_POAS_NUMPOAS  15
struct T_POAS_BASE_690
{
	int  nRefStep;
	double dDistRatio;
};
struct T_POAS_BASE
{
	int  nRefStep;
	double dDistRatio;
	int  nType;         // 6.9.1에서 추가 0:PP, 1:MD, 2:BS, 3:User
	double dMonDispl;   // 6.9.1에서 추가 (MD인 경우)
	double dBaseShear;  // 6.9.1에서 추가 (BS인 경우)
	CString csName;     // 6.9.1에서 추가 (User인 경우)
	T_KEY PushoverKey;  // 6.9.1에서 추가 Pushover Loadcase Key
	void Initialize()
	{
		nRefStep=-1;
		dDistRatio=0;
		nType = 0;
		dMonDispl = 0.0;
		dBaseShear = 0.0;
		csName.Empty();
		PushoverKey = 0;
	}
};
struct T_POAS_BASE_CH
{
	int  nRefStep;
	double dDistRatio;
	int  nType;
	double dMonDispl;
	double dBaseShear;
	char csName[20];
	T_KEY PushoverKey;
	void ConvertToString(T_POAS_BASE& rData)
	{
		rData.nRefStep = nRefStep;
		rData.dDistRatio = dDistRatio;
		rData.nType = nType;
		rData.dMonDispl = dMonDispl;
		rData.dBaseShear = dBaseShear;
		ConvertCharStr(csName, rData.csName, sizeof(csName));
		rData.PushoverKey = PushoverKey;
	}
	void ConvertToChar(T_POAS_BASE& rData)
	{
		nRefStep = rData.nRefStep;
		dDistRatio = rData.dDistRatio;
		nType = rData.nType;
		dMonDispl = rData.dMonDispl;
		dBaseShear = rData.dBaseShear;
		ConvertStrChar(rData.csName, csName, sizeof(csName));
		PushoverKey = rData.PushoverKey;
	}
};
struct T_POAS_D_690
{
	T_POAS_BASE_690 aStep[15];
};
struct T_POAS_D
{
	T_POAS_BASE aStep[D_POAS_NUMPOAS];
	T_POAS_D() {}
	T_POAS_D(const T_POAS_D& src) { *this = src; }
	T_POAS_D& operator=(const T_POAS_D& rData)
	{
		for (int i = 0; i < D_POAS_NUMPOAS; i++)
			aStep[i] = rData.aStep[i];
		return *this;
	}
	void Initialize()
	{
		for(int i = 0; i < D_POAS_NUMPOAS ; i++) aStep[i].Initialize();    
	}
	void Convert690(T_POAS_D_690& rData)
	{
		Initialize();
		for (int i = 0; i < 15; i++)
		{
			aStep[i].Initialize();
			aStep[i].nRefStep = rData.aStep[i].nRefStep;
			aStep[i].dDistRatio = rData.aStep[i].dDistRatio;
		}
	}
};
struct T_POAS_D_CH
{
	T_POAS_BASE_CH aStep[D_POAS_NUMPOAS];
	void ConvertToString(T_POAS_D& rData)
	{
		for (int i = 0; i < D_POAS_NUMPOAS; i++)
			aStep[i].ConvertToString(rData.aStep[i]);
	}
	void ConvertToChar(T_POAS_D& rData)
	{
		for (int i = 0; i < D_POAS_NUMPOAS; i++)
			aStep[i].ConvertToChar(rData.aStep[i]);
	}
};
struct T_POAS_UDRD_D
{
	T_POAS_D data;
};
struct T_POAS_UNIT
{
	T_POAS_UNIT()
	{
		dMonDispl = D_UNITSYS_BASE_LENGTH;
		dBaseShear = D_UNITSYS_BASE_FORCE;
	}
	int dMonDispl;
	int dBaseShear;
};
#define HASHSIZEPOAS 1

// Result Data for Pushover analysis
#define T_PORD_K unsigned int
struct T_PORD_D
{
	CString Name;
	unsigned int nEntity;	// Node(nFunctionType=1,2), Elem(nFunctionType=3,4,5,6,7), Nlnk(nFunctionType=8), Story(nFunctionType=12), Elnk(nFunctionType=14)
	int nFunctionType;		// 1=Reaction  2=Displacement  3=Truss  4=Beam  5=PlaneStress  6=Plate  7=Solid  8=General Link
							// 9=Inelastic Hinge, 10=PlaneStrain, 11=Load Increment, 12=Displ by Story,		14=Elastic Link
	int nSubType;			// if nFuncType == 1                ; ?
							// if nFuncType == 2                ; 1=Displ.  2=Vel.    3=Accel.
							// if nFuncType == 3,4,5,7,10       ; 1=Force   2=Stress
							// if nFuncType == 6                ; 1=Force   2=Stress  3=UnitForce
							// if nFuncType == 8,14             ; 1=Deformation, 2=I-Node Force, 3=J-Node Force
							// if nFuncType == 9                ; 1=Deform  2=Force
							// if nFuncType == 11               ; 사용안함
							// if nFuncType == 12               : 1=Displ.  2=Vel.    3=Accel

	int nPosition;			// if nFuncType == 1,2              ; 사용안함
							// if nFuncType == 3,4              ; 1=I 2=J
							// if nFuncType == 5, nSubType == 1 ; 1=I 2=J 3=K 4=L
							// if nFuncType == 5, nSubType == 2 ; 1=CENT 2=I 3=J 4=K 5=L
							// if nFuncType == 6, nSubType == 1 ; 1=I 2=J 3=K 4=L
							// if nFuncType == 6, nSubType == 2 ; 1=CENT 2=I 3=J 4=K 5=L
							// if nFuncType == 7, nSubType == 1 ; 1=I 2=J 3=K 4=L 5=M 6=N 7=O 8=P
							// if nFuncType == 7, nSubType == 2 ; 1=CENT 2=I 3=J 4=K 5=L 6=M 7=N 8=O 9=P
							// if nFuncType == 8, 14            ; 사용안함
							// if nFuncType == 9                ; Location으로 사용
							// if nFuncType == 10, nSubType == 1; 1=I 2=J 3=K 4=L
							// if nFuncType == 10, nSubType == 2; 1=CENT 2=I 3=J 4=K 5=L
							// if nFuncType == 11               ; 사용안함
							// if nFuncType == 12               : 사용안함                      

	int nComponent;			// if nFuncType == 1                    ; ?
							// if nFuncType == 2                    ; 1=DX 2=DY 3=DZ 4=RX 5=RY 6=RZ
							// if nFuncType == 3, nSubType == 1     ; FX
							// if nFuncType == 3, nSubType == 2     ; SX
							// if nFuncType == 4, nSubType == 1     ; Fx Fy Fz Mm My Mz
							// if nFuncType == 4, nSubType == 2     ; Sx Sy Sz ByP ByM BzP BzM
							// if nFuncType == 5, nSubType == 1     ; Fx Fy
							// if nFuncType == 5, nSubType == 2     ; Sx Sy Sxy
							// if nFuncType == 6, nSubType == 1     ; Fx Fy Fz Mx My Mz
							// if nFuncType == 6, nSubType == 2     ; SxT SyT SxyT SxB SyB SxyB
							// if nFuncType == 6, nSubType == 3     ; Fxx Fyy Fxy Mxx Myy Mzz Vxx Vyy
							// if nFuncType == 7, nSubType == 1     ; FX FY FZ
							// if nFuncType == 7, nSubType == 2     ; SXX SYY SZZ SXY SYZ SXZ
							// if nFuncType == 8,14 nSubType == 1   ; Axial, Tran-y, Tran-z, Torsional, Rotation-y, Rotation-z
							// if nFuncType == 8,14 nSubType == 2,3 ; Axial, Shear-y, Shear-z, Torsion, Moment-y, Moment-z
							// if nFuncType == 10, nSubType == 1    ; Fx Fy Fz
							// if nFuncType == 10, nSubType == 2    ; Sx Sy Sz Sxy
							// if nFuncType == 11                   ; 사용안함
							// if nFuncType == 12                   : 1=DX, 2=DY

	BOOL bAllMode;
	int nSelectedMode;
	int nOption1;
	int nRefPoint;         // 1=Ground, 2=Add Ground Motion, 3=Another Node / 3=Story Drift : nFuncType==12

	T_NODE_K KeyRefNode;   
	T_POLC_K KeyPolc;

	// 아래 4 개의 변수들은 현재 쓰이지 않으나 추가 가능성 있음
	// Default 값으로 간주, 일단 CheckPord 에서 체크
	int    nGraphStep;        // [NOT USED:0] 0=Graph Function, 1=Step Function
	int    nStepFunction;     // [NOT USED:1] 1=Time Step, 2=Displ/Vel/Accel, 3=Truss Force/Stress, 4=Beam Force/Stress, 5=Designated DOF
	int    nValueType;        // [NOT USED:0] (nStepFunction=2, 3, 4, 5, 6 인 경우 사용) - 0=Min, 1=Max, 2=Abs Max
	int    nGlobalLocal;      // [NOT USED:0] 0=Global, 1=Local : nFunctionType == 7(Solid), 10(Plane Strain)인 경우 사용

	double dTimeStep;         
	int    nElemType4Iehg;    // 0=Beam, 1=General Link, 2=Truss, 3=Wall, 4=Point Spring
	BOOL   bCumulDisplByInitLoad;

	void Initialize()
	{
		Name.Empty();
		nEntity=0;
		nFunctionType=1;
		nSubType=0;
		nPosition=0;
		nComponent=0;
		bAllMode=FALSE;
		nSelectedMode=0;
		nOption1=0;
		nRefPoint = 1;
		KeyRefNode = 0;
		KeyPolc = 0;
		nGraphStep = 0;
		nStepFunction = 1;
		nValueType = 0;
		dTimeStep = 0.;
		nElemType4Iehg = 0;
		nGlobalLocal = 0;
		bCumulDisplByInitLoad = FALSE;
	}
};
struct T_PORD_D_CH_871
{
	char Name[20];
	unsigned int nEntity;
	int nFunctionType;
	int nSubType;
	int nPosition;
	int nComponent;
	BOOL bAllMode;
	int nSelectedMode;
	int nOption1;
	int nRefPoint; 
	T_NODE_K KeyRefNode;  
	T_POLC_K KeyPolc;
	int    nGraphStep;    // 0=Graph Function, 1=Step Function
	int    nStepFunction; // 2=Displ/Vel/Accel, 3=Truss Force/Stress, 4=Beam Force/Stress  
	int    nValueType;
	double dTimeStep;
	int    nElemType4Iehg;    // 0=Beam, 1=General Link, 2=Truss, 3=Wall
	int    nGlobalLocal;      // 0=Global, 1=Local : nFunctionType == 7(Solid), 10(Plane Strain)인 경우 사용
};
struct T_PORD_UDRD_D_CH_871
{
	T_PORD_K key;
	T_PORD_D_CH_871 data;
};
struct T_PORD_D_CH
{
	char Name[20];
	unsigned int nEntity;
	int nFunctionType;
	int nSubType;
	int nPosition;
	int nComponent;
	BOOL bAllMode;
	int nSelectedMode;
	int nOption1;
	int nRefPoint; 
	T_NODE_K KeyRefNode;  
	T_POLC_K KeyPolc;
	int    nGraphStep;    // 0=Graph Function, 1=Step Function
	int    nStepFunction; // 2=Displ/Vel/Accel, 3=Truss Force/Stress, 4=Beam Force/Stress  
	int    nValueType;
	double dTimeStep;
	int    nElemType4Iehg;    // 0=Beam, 1=General Link, 2=Truss, 3=Wall
	int    nGlobalLocal;      // 0=Global, 1=Local : nFunctionType == 7(Solid), 10(Plane Strain)인 경우 사용
	BOOL   bCumulDisplByInitLoad;
	void ConvertToString(T_PORD_D& rData)
	{
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.nEntity=nEntity;
		rData.nFunctionType=nFunctionType;
		rData.nSubType=nSubType;
		rData.nPosition=nPosition;
		rData.nComponent=nComponent;
		rData.bAllMode=bAllMode;
		rData.nSelectedMode=nSelectedMode;
		rData.nOption1=nOption1;
		rData.nRefPoint = nRefPoint;
		rData.KeyRefNode = KeyRefNode;
		rData.KeyPolc = KeyPolc;
		rData.nGraphStep = nGraphStep;
		rData.nStepFunction = nStepFunction;
		rData.nValueType = nValueType;
		rData.dTimeStep = dTimeStep;
		rData.nElemType4Iehg = nElemType4Iehg;
		rData.nGlobalLocal = nGlobalLocal;
		rData.bCumulDisplByInitLoad = bCumulDisplByInitLoad;
	}
	void ConvertToChar(T_PORD_D& rData)
	{
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		nEntity=rData.nEntity;
		nFunctionType=rData.nFunctionType;
		nSubType=rData.nSubType;
		nPosition=rData.nPosition;
		nComponent=rData.nComponent;
		bAllMode=rData.bAllMode;
		nSelectedMode=rData.nSelectedMode;
		nOption1=rData.nOption1;
		nRefPoint = rData.nRefPoint;
		KeyRefNode = rData.KeyRefNode;
		KeyPolc = rData.KeyPolc;
		nGraphStep = rData.nGraphStep;
		nStepFunction = rData.nStepFunction;
		nValueType = rData.nValueType;
		dTimeStep = rData.dTimeStep;
		nElemType4Iehg = rData.nElemType4Iehg;
		nGlobalLocal = rData.nGlobalLocal;
		bCumulDisplByInitLoad = rData.bCumulDisplByInitLoad;
	}
	void Convert871(const T_PORD_D_CH_871& src)
	{
		memcpy(Name, src.Name, sizeof(src.Name));
		nEntity        = src.nEntity;
		nFunctionType  = src.nFunctionType;
		nSubType       = src.nSubType;
		nPosition      = src.nPosition;
		nComponent     = src.nComponent;
		bAllMode       = src.bAllMode;
		nSelectedMode  = src.nSelectedMode;
		nOption1       = src.nOption1;
		nRefPoint      = src.nRefPoint;
		KeyRefNode     = src.KeyRefNode;
		KeyPolc        = src.KeyPolc;
		nGraphStep     = src.nGraphStep;
		nStepFunction  = src.nStepFunction;
		nValueType     = src.nValueType;
		dTimeStep      = src.dTimeStep;
		nElemType4Iehg = src.nElemType4Iehg;
		nGlobalLocal   = src.nGlobalLocal;
		bCumulDisplByInitLoad = FALSE;
	}
};
struct T_PORD_UDRD_D
{
	T_PORD_K key;
	T_PORD_D data;
};
struct T_PORD_UDRD_D_CH
{
	T_PORD_K key;
	T_PORD_D_CH data;
};
struct T_PORD_UNIT
{
	T_PORD_UNIT()
	{
		Name=D_UNITSYS_NONE;
		nEntity=D_UNITSYS_NONE;
		nFunctionType=D_UNITSYS_NONE;
		nSubType=D_UNITSYS_NONE;
		nPosition=D_UNITSYS_NONE;
		nComponent=D_UNITSYS_NONE;
		bAllMode=D_UNITSYS_NONE;
		nSelectedMode=D_UNITSYS_NONE;
		nRefPoint=D_UNITSYS_NONE;
		KeyRefNode=D_UNITSYS_NONE;
		KeyPolc=D_UNITSYS_NONE;
	}
	int Name;
	int nEntity;
	int nFunctionType;
	int nSubType;
	int nPosition;
	int nComponent;
	int bAllMode;
	int nSelectedMode;
	int nRefPoint;
	int KeyRefNode;
	int KeyPolc;
};

#define HASHSIZEPORD 1

// Pushover Result Graph - Element Force Result
enum D_POEF_TYPE { D_POEF_TYPE_FORCE };
enum D_POEF_ELEM { D_POEF_ELEM_BEAM=0, D_POEF_ELEM_TRUSS, D_POEF_ELEM_WALL };
#define T_POEF_K UINT
struct T_POEF_D
{
	int nTypeElem;  // D_POEF_ELEM
	T_KEY PropK;    // nTypeElem==0,1 : T_ELEM_K
									// nTypeElem==2   : User Wall ID
	T_STOR_K StorK; // nTypeElem==D_POEF_ELEM_WALL일 때만 사용
	int nTypeRes;   // D_POEF_TYPE
	int nLocation;  // Location. 0:I-Node, 1:J-Node
	int nComp;      // Component 0:Fx, ..., 5:Mz
	T_POLC_K PolcK;

	T_POEF_D() { Initialize(); }
	T_POEF_D(T_POEF_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		nTypeElem=D_POEF_ELEM_BEAM;
		PropK=0;
		StorK=0;
		nTypeRes=D_POEF_TYPE_FORCE;
		nLocation=0;
		nComp=0;
		PolcK=0;
	}
	T_POEF_D& operator=(T_POEF_D& src)
	{
		nTypeElem = src.nTypeElem;
		PropK     = src.PropK    ;
		StorK     = src.StorK    ;
		nTypeRes  = src.nTypeRes;
		nLocation = src.nLocation;
		nComp     = src.nComp;
		PolcK     = src.PolcK;
		return *this;
	}
	BOOL operator==(const T_POEF_D& src)
	{
		if(nTypeElem != src.nTypeElem) return FALSE;
		if(PropK     != src.PropK    ) return FALSE;
		if(StorK     != src.StorK    ) return FALSE;
		if(nTypeRes  != src.nTypeRes ) return FALSE;
		if(nLocation != src.nLocation) return FALSE;
		if(nComp     != src.nComp    ) return FALSE;
		if(PolcK     != src.PolcK    ) return FALSE;
		return TRUE;
	}
};
struct T_POEF_UDRD_D
{
	T_POEF_K key;
	T_POEF_D data;
};
struct T_POEF_UDRD_D_RW
{
	T_POEF_K key;

	int nTypeElem;  // Type of Element
	T_KEY    PropK;
	T_STOR_K StorK;
	int nTypeRes;   // Type of Result
	int nLocation;  
	int nComp;      // Component
	T_POLC_K PolcK;

	void GetPoef(T_POEF_K& rKey, T_POEF_D& rData)
	{
		rKey=key;
		rData.nTypeElem=nTypeElem;
		rData.PropK=PropK;
		rData.StorK=StorK;
		rData.nTypeRes=nTypeRes;
		rData.nLocation=nLocation;
		rData.nComp=nComp;
		rData.PolcK=PolcK;
	}
	void SetPoef(const T_POEF_K Key, const T_POEF_D& rData)
	{
		key=Key;
		nTypeElem = rData.nTypeElem;
		PropK = rData.PropK;
		StorK = rData.StorK;
		nTypeRes = rData.nTypeRes;
		nLocation = rData.nLocation;
		nComp = rData.nComp;
		PolcK = rData.PolcK;
	}
};
#define HASHSIZEPOEF 101

enum D_PORG_TYPE { D_PORG_TYPE_FORCE_DEFRM=0, D_PORG_TYPE_FORCE, D_PORG_TYPE_DEFRM };
enum D_PORG_SUB_TYPE { D_PORG_SUB_TYPE_DEFRM=1, D_PORG_SUB_TYPE_I_FORCE=2, D_PORG_SUB_TYPE_J_FORCE=3 };

// Pushover Result Graph - General Link
#define T_PORG_K UINT
struct T_PORG_D
{
	T_NLNK_K NlnkK;
	int nTypeRes;   // Type of Result. 0:Force-Deformation, 1:Force, 2:Deformation
	int nLocation;  // Location. 0:I-Node, 1:J-Node
	int nComp;      // Component 0:Fx, ..., 5:Mz
	T_POLC_K PolcK;

	T_PORG_D() { Initialize(); }
	T_PORG_D(T_PORG_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		NlnkK = 0;
		nTypeRes=0;
		nLocation=0;
		nComp=0;
		PolcK=0;
	}
	T_PORG_D& operator=(T_PORG_D& src)
	{
		NlnkK = src.NlnkK;
		nTypeRes  = src.nTypeRes;
		nLocation = src.nLocation;
		nComp     = src.nComp;
		PolcK     = src.PolcK;
		return *this;
	}
	BOOL operator==(const T_PORG_D& src)
	{
		if(NlnkK     != src.NlnkK    ) return FALSE;
		if(nTypeRes  != src.nTypeRes ) return FALSE;
		if(nLocation != src.nLocation) return FALSE;
		if(nComp     != src.nComp    ) return FALSE;
		if(PolcK     != src.PolcK    ) return FALSE;
		return TRUE;
	}
};
struct T_PORG_UDRD_D
{
	T_PORG_K key;
	T_PORG_D data;
};
struct T_PORG_UDRD_D_RW
{
	T_PORG_K key;

	T_NLNK_K NlnkK;
	int nTypeRes;   // Type of Result
	int nLocation;  // Location
	int nComp;      // Component
	T_POLC_K PolcK;

	void GetPorg(T_PORG_K& rKey, T_PORG_D& rData)
	{
		rKey=key;
		rData.NlnkK = NlnkK;
		rData.nTypeRes=nTypeRes;
		rData.nLocation=nLocation;
		rData.nComp=nComp;
		rData.PolcK=PolcK;
	}
	void SetPorg(const T_PORG_K Key, const T_PORG_D& rData)
	{
		key=Key;
		NlnkK = rData.NlnkK;
		nTypeRes = rData.nTypeRes;
		nLocation = rData.nLocation;
		nComp = rData.nComp;
		PolcK = rData.PolcK;
	}
};
#define HASHSIZEPORG 101

// Pushover Result Graph - Elastic Link
#define T_PORE_K UINT
struct T_PORE_D
{
	T_ELNK_K ElnkK;
	int nTypeRes;   // Type of Result. 0:Force-Deformation, 1:Force, 2:Deformation
	int nLocation;  // Location. 0:I-Node, 1:J-Node
	int nComp;      // Component 0:Fx, ..., 5:Mz
	T_POLC_K PolcK;

	T_PORE_D() { Initialize(); }
	T_PORE_D(T_PORE_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		ElnkK=0;
		nTypeRes=0;
		nLocation=0;
		nComp=0;
		PolcK=0;
	}
	T_PORE_D& operator=(T_PORE_D& src)
	{
		ElnkK = src.ElnkK;
		nTypeRes = src.nTypeRes;
		nLocation = src.nLocation;
		nComp = src.nComp;
		PolcK = src.PolcK;
		return *this;
	}
	BOOL operator==(const T_PORE_D& src)
	{
		if(ElnkK     != src.ElnkK    ) return FALSE;
		if(nTypeRes  != src.nTypeRes ) return FALSE;
		if(nLocation != src.nLocation) return FALSE;
		if(nComp     != src.nComp    ) return FALSE;
		if(PolcK     != src.PolcK    ) return FALSE;
		return TRUE;
	}
};
struct T_PORE_UDRD_D
{
	T_PORE_K key;
	T_PORE_D data;
};
struct T_PORE_UDRD_D_RW
{
	T_PORE_K key;

	T_ELNK_K ElnkK;
	int nTypeRes;   // Type of Result
	int nLocation;  // Location
	int nComp;      // Component
	T_POLC_K PolcK;

	void GetPore(T_PORE_K& rKey, T_PORE_D& rData)
	{
		rKey=key;
		rData.ElnkK=ElnkK;
		rData.nTypeRes=nTypeRes;
		rData.nLocation=nLocation;
		rData.nComp=nComp;
		rData.PolcK=PolcK;
	}
	void SetPore(const T_PORE_K Key, const T_PORE_D& rData)
	{
		key=Key;
		ElnkK=rData.ElnkK;
		nTypeRes=rData.nTypeRes;
		nLocation=rData.nLocation;
		nComp=rData.nComp;
		PolcK=rData.PolcK;
	}
};
#define HASHSIZEPORE 101

// Pushover Result Graph - Inelastic Hinge
enum D_PORI_TYPE { D_PORI_TYPE_FRCDFM,D_PORI_TYPE_FORCE,D_PORI_TYPE_DEFORM };
enum D_PORI_ELEM { D_PORI_ELEM_BEAM,D_PORI_ELEM_TRUSS,D_PORI_ELEM_WALL,D_PORI_ELEM_GLLINK,D_PORI_ELEM_PSPR };
#define T_PORI_K UINT
struct T_PORI_D
{
	int nTypeElem;  // Type of Element
	T_KEY PropK;    // nTypeElem==0,1 : T_ELEM_K
									// nTypeElem==2   : User Wall ID
									// nTypeElem==3   : T_NLNK_K
	T_STOR_K StorK; // nTypeElem==2일 때만 사용
	int nTypeRes;   // Type of Result. 0:Force-Deformation, 1:Force, 2:Deformation
	int nLocation;  // Location. 0:I-Node, 1:J-Node
	int nComp;      // Component 0:Fx, ..., 5:Mz
	T_POLC_K PolcK;

	T_PORI_D() { Initialize(); }
	T_PORI_D(T_PORI_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		nTypeElem=D_PORI_ELEM_BEAM;
		PropK=0;
		StorK=0;
		nTypeRes=D_PORI_TYPE_FRCDFM;
		nLocation=0;
		nComp=0;
		PolcK=0;
	}
	T_PORI_D& operator=(T_PORI_D& src)
	{
		nTypeElem = src.nTypeElem;
		PropK     = src.PropK    ;
		StorK     = src.StorK    ;
		nTypeRes  = src.nTypeRes;
		nLocation = src.nLocation;
		nComp     = src.nComp;
		PolcK     = src.PolcK;
		return *this;
	}
	BOOL operator==(const T_PORI_D& src)
	{
		if(nTypeElem != src.nTypeElem) return FALSE;
		if(PropK     != src.PropK    ) return FALSE;
		if(StorK     != src.StorK    ) return FALSE;
		if(nTypeRes  != src.nTypeRes ) return FALSE;
		if(nLocation != src.nLocation) return FALSE;
		if(nComp     != src.nComp    ) return FALSE;
		if(PolcK     != src.PolcK    ) return FALSE;
		return TRUE;
	}
};
struct T_PORI_UDRD_D
{
	T_PORI_K key;
	T_PORI_D data;
};
struct T_PORI_UDRD_D_RW
{
	T_PORI_K key;

	int nTypeElem;  // Type of Element
	T_KEY    PropK;
	T_STOR_K StorK;
	int nTypeRes;   // Type of Result
	int nLocation;  
	int nComp;      // Component
	T_POLC_K PolcK;

	void GetPori(T_PORI_K& rKey, T_PORI_D& rData)
	{
		rKey=key;
		rData.nTypeElem=nTypeElem;
		rData.PropK=PropK;
		rData.StorK=StorK;
		rData.nTypeRes=nTypeRes;
		rData.nLocation=nLocation;
		rData.nComp=nComp;
		rData.PolcK=PolcK;
	}
	void SetPori(const T_PORI_K Key, const T_PORI_D& rData)
	{
		key=Key;
		nTypeElem = rData.nTypeElem;
		PropK = rData.PropK;
		StorK = rData.StorK;
		nTypeRes = rData.nTypeRes;
		nLocation = rData.nLocation;
		nComp = rData.nComp;
		PolcK = rData.PolcK;
	}
};
#define HASHSIZEPORI 101

// Ignore Elements for NL. Analysis Initial Load 
#define T_IEPI_K T_ELEM_K
struct T_IEPI_D
{
	BOOL bIgnore;
	void Initialize()
	{
		bIgnore=FALSE;
	}
};
struct T_IEPI_UDRD_D
{
	T_IEPI_K key;
	T_IEPI_D data;
};
struct T_IEPI_UNIT
{
	T_IEPI_UNIT()
	{
		bIgnore=D_UNITSYS_NONE;
	}
	int bIgnore;
};
#define HASHSIZEIEPI HASHSIZEELEM

// Yield Strength Calculation Option 
#define T_YSCO_K unsigned int
enum D_YSCO_PQ	 { D_YSCO_PQ_CALCULATED=0, D_YSCO_PQ_3 };

// 출시 전 버전. 나중에 지워도 무방
struct T_YSCO_D_885_temp
{
	int  nPQOption			; // M/Vd value
	BOOL bSelectElemStep	; // Select Elem. & Step for Print PQ-Inter. Detail Report
	T_ELEM_K ElemK_Detail	;
	int      nStep_Detail	;
};

struct T_YSCO_D
{
	int  nPQOption_Comp		; // M/Vd value (Compressive)
	int  nPQOption_Tens		; // M/Vd value (0 or Tensile)
	BOOL bSelectElemStep	; // Select Elem. & Step for Print PQ-Inter. Detail Report
	T_ELEM_K ElemK_Detail	;
	int      nStep_Detail	;
	void Initialize()
	{
		nPQOption_Comp	= D_YSCO_PQ_CALCULATED;
		nPQOption_Tens	= D_YSCO_PQ_CALCULATED;
		bSelectElemStep	= FALSE;
		ElemK_Detail=0;
		nStep_Detail=1;
	}

	bool operator==(const T_YSCO_D& src) const
	{
		if (this == &src) return true;
		if (nPQOption_Comp != src.nPQOption_Comp) return false;
		if (nPQOption_Tens != src.nPQOption_Tens) return false;
		if (ElemK_Detail != src.ElemK_Detail) return false;
		if (nStep_Detail != src.nStep_Detail) return false;
		return true;
	}
	bool operator!=(const T_YSCO_D& src) const { return !(*this == src); }

	T_YSCO_D() { Initialize(); }
	void Convert885temp(T_YSCO_D_885_temp& rData)
	{
		nPQOption_Comp	= rData.nPQOption;
		nPQOption_Tens	= rData.nPQOption;
		bSelectElemStep	= rData.bSelectElemStep;
		ElemK_Detail	= rData.ElemK_Detail;
		nStep_Detail	= rData.nStep_Detail;
	}
};
struct T_YSCO_UDRD_D_885_temp
{
	T_YSCO_K key;
	T_YSCO_D_885_temp data;
};
struct T_YSCO_UDRD_D
{
	T_YSCO_K key;
	T_YSCO_D data;
};
struct T_YSCO_UNIT
{
	T_YSCO_UNIT()
	{
	}
};
#define HASHSIZEYSCO 1

// Select Pushover Hinge Result Output
#define T_SPHO_KEY unsigned int
#define T_SPHO_K T_PHGE_K
struct T_SPHO_D
{  
	BOOL bFiberResult; // Fiber Section Result
	void Initialize()
	{
		bFiberResult = FALSE;
	}
};
struct T_SPHO_UDRD_D
{
	T_SPHO_K key;
	T_SPHO_D data;
};
struct T_SPHO_UNIT
{
	T_SPHO_UNIT() {}
};
#define HASHSIZESPHO HASHSIZEPHGE

#pragma pack(pop)

#endif  // __DB_ST_DT_POVR_H__
