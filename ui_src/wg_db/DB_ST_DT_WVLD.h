#ifndef __DB_ST_DT_WVLD_H__
#define __DB_ST_DT_WVLD_H__

#define MIDAS_SDK

#pragma pack(push, 1)

enum CRESTPOS
{
	EN_CRPS_MXM,   // Maximum Moment About Bottom W.R.T. Wave Direction 
	EN_CRPS_MNM,   // Minimum Moment About Bottom W.R.T. Wave Direction
	EN_CRPS_MXS,   // Maximum Shear About Bottom W.R.T. Wave Direction
	EN_CRPS_MNS,   // Maximum Shear About Bottom W.R.T. Wave Direction
	EN_CRPS_MUP,   // Maximum Force Upward
	EN_CRPS_MDN,   // Maximum Force Downward
	EN_CRPS_ALL    // All
};

enum WAVETH
{
	EN_WVTH_AIRY,  // Airy wave
	EN_WVTH_AIRM,  // Airy wave(mean position)
	EN_WVTH_STKV,  // Stokes V wave
	EN_WVTH_STRC,  // Stream Function with current effect
	EN_WVTH_STRN,  // Stream Function without current effect
	EN_WVTH_CNDL,  // Cnoidal wave
	EN_WVTH_SLTR,  // Solitary wave
	EN_WVTH_USER   // user defined(grid)
};

///// Drag & Inertia Coefficient
struct T_WVCF_D
{
	T_GRUP_K GrupK;      // Structure Group Key
	double   dDiameter;  // Diameter
	 
	double   dDragCoefX; // Drag Coefficient(x dir)=>tangential drag coeff.
	double   dDragCoefY; // Drag Coefficient(y dir)
	double   dDragCoefZ; // Drag Coefficient(X dir)
	double   dInerCoefX; // Inertia Coefficient(x dir)=>tangential intertia drag coeff.
	double   dInerCoefY; // Inertia Coefficient(x dir)
	double   dInerCoefZ; // Inertia Coefficient(x dir)

	T_WVCF_D() {}
	T_WVCF_D(const T_WVCF_D& src) { *this = src; }
	T_WVCF_D& operator=(const T_WVCF_D& src)
	{
		GrupK      = src.GrupK;
		dDiameter  = src.dDiameter;
		dDragCoefX = src.dDragCoefX;
		dDragCoefY = src.dDragCoefY;
		dDragCoefZ = src.dDragCoefZ;
		dInerCoefX = src.dInerCoefX;
		dInerCoefY = src.dInerCoefY;
		dInerCoefZ = src.dInerCoefZ;
		return *this;
	}
	void Initialize()
	{
		GrupK = 0;
		dDiameter = 0.;
		dDragCoefY = dDragCoefZ = 0.;
		dInerCoefY = dInerCoefZ = 0.;
		dDragCoefX = dInerCoefX = 0.;
	}
};

///// Current Profile
struct T_WVCP_D	
{	
	double	dD; // depth
	double	dV; // velocity

	T_WVCP_D() {}
	T_WVCP_D(const T_WVCP_D& src) { *this = src; }
	T_WVCP_D& operator=(const T_WVCP_D& src)
	{
		dD  = src.dD;
		dV  = src.dV;
		return *this;
	}

	void Initialize()
	{
		dD = dV = 0.;
	}
};

///// Marine Growth
struct T_WVMG_D	
{	
	double	dZ;
	double	dThick;
	T_WVMG_D() {}
	T_WVMG_D(const T_WVMG_D& src) { *this = src; }
	T_WVMG_D& operator=(const T_WVMG_D& src)
	{
		dZ = src.dZ;
		dThick = src.dThick;
		return *this;
	}

	void Initialize()
	{
		dZ = dThick = 0.;
	}
};	


///// User Defined wave characteristics
struct T_WVUS_D	
{	
	double	dX;
	double	dZ;
	double	dElevation;
	double	dVx;
	double	dVcx;
	double  dVt;
	double	dVz;
	double	dAx;
	double	dAz;

	T_WVUS_D() {}
	T_WVUS_D(const T_WVUS_D& src) { *this = src; }
	T_WVUS_D& operator=(const T_WVUS_D& src)
	{
		dX = src.dX;
		dZ = src.dZ;
		dElevation = src.dElevation;
		dVx = src.dVx;
		dVcx = src.dVcx;
		dVt = src.dVt;
		dVz = src.dVz;
		dAx = src.dAx;
		dAz = src.dAz;
		return *this;
	}

	void Initialize()
	{
		dX = 0.;
		dZ = 0.;
		dElevation = 0.;
		dVx = 0.;
		dVcx = 0.;
		dVt = 0.;
		dVz = 0.;
		dAx = 0.;
		dAz = 0.;
	}
};	
	

///// Wave Load Data
#define T_WVLD_K unsigned int

struct T_WVLD_D
{
// General Data
	CString  strName;
	CString  strDesc;
	BOOL     bStld;          // static load
	UINT     nStldType;      // 0:Nodal Load, 1:Beam Load (default : 0) : 800 추가 
	BOOL     bThis;          // time history load
	T_THIS_K ThisK;
	int      nVertCoord;     // 0:Y, 1:Z
	double   dWaterDensity;
	double   dWaterDepth;
	double   dMudLine;
	double   dGroundLevel;
	BOOL     bSelfW;
	BOOL     bBuoyant;
	CRESTPOS nCrest;
	int      nUnit;          // 0:length[L], 1:period[deg], 2:time[sec]
	double   dInitPos;       // [L], [deg], [sec]
	double   dStepSize;      // [L], [deg], [sec]
	int      nNumberPos;
// Drag & Inertia Coefficient
	int      nCoefType;
	CArray<T_WVCF_D, T_WVCF_D&> aCoef_s; 
	CArray<T_WVCF_D, T_WVCF_D&> aCoef_r; 
	BOOL     bOverride;
	CArray<T_WVCF_D, T_WVCF_D&> aOver_s; 
	CArray<T_WVCF_D, T_WVCF_D&> aOver_r; 
// Wave Characteristic
	WAVETH   nTheory; 
	int      nFunc;          // STRC, STRN only
	double   dWaveDir;
	double   dWaveHeight;
	int      nLengthPeriod;  // 0:wave length, 1:wave period
	double   dLength;
	double   dPeriod;
	double   dKinematicF;
	double   dSurfaceV;
	double   dBottomV;
// Current Profile
	double   dCurDir;
	double	 dCurBlockageF;
	CArray<T_WVCP_D, T_WVCP_D&> aCurProfile;
// Flood Condition
	CArray<T_GRUP_K, T_GRUP_K> aFlood;
// Marine Growth
	CArray<T_WVMG_D, T_WVMG_D&> aMarineGrowth;
// User Defined Wave	
	int	      nGridX, nGridZ;
	T_WVUS_D	(*aUserGrid)[73]; // [20][73]
	T_WVUS_D	(*aTraj)[73];     // [10][73]

	T_WVLD_D() { Alloc(); }
	~T_WVLD_D() { delete []aUserGrid; delete []aTraj; }
	T_WVLD_D(const T_WVLD_D& src) { Alloc(); *this = src; }
	void Alloc() { aUserGrid = new T_WVUS_D[20][73]; aTraj = new T_WVUS_D[10][73]; }
	T_WVLD_D& operator=(const T_WVLD_D& src)
	{
		strName       = src.strName;
		strDesc       = src.strDesc;
		bStld         = src.bStld;
		nStldType     = src.nStldType;
		bThis         = src.bThis;
		ThisK         = src.ThisK;
		nVertCoord    = src.nVertCoord;
		dWaterDensity = src.dWaterDensity;
		dWaterDepth   = src.dWaterDepth;
		dMudLine      = src.dMudLine;
		dGroundLevel	= src.dGroundLevel;
		bSelfW        = src.bSelfW;
		bBuoyant      = src.bBuoyant;
		nCrest        = src.nCrest;
		nUnit         = src.nUnit;
		dInitPos      = src.dInitPos;
		dStepSize     = src.dStepSize;
		nNumberPos    = src.nNumberPos;
		nCoefType     = src.nCoefType;
		aCoef_s.Copy(src.aCoef_s);
		aCoef_r.Copy(src.aCoef_r);
		bOverride     = src.bOverride;
		aOver_s.Copy(src.aOver_s);
		aOver_r.Copy(src.aOver_r);
		nTheory       = src.nTheory;
		nFunc         = src.nFunc;
		dWaveDir      = src.dWaveDir;
		dWaveHeight   = src.dWaveHeight;
		nLengthPeriod = src.nLengthPeriod;
		dLength       = src.dLength;
		dPeriod       = src.dPeriod;
		dKinematicF   = src.dKinematicF;
		dSurfaceV     = src.dSurfaceV;
		dBottomV      = src.dBottomV;
		dCurDir       = src.dCurDir;
		dCurBlockageF = src.dCurBlockageF;
		aCurProfile.Copy(src.aCurProfile);
		aFlood.Copy(src.aFlood);
		aMarineGrowth.Copy(src.aMarineGrowth);
		nGridX        = src.nGridX;
		nGridZ        = src.nGridZ;
		for(int i=0; i<20; i++) for(int j=0; j<73; j++)
		{
			aUserGrid[i][j] = src.aUserGrid[i][j];
			if(i<10) aTraj[i][j] = src.aTraj[i][j];
		}
		return *this;
	}

	void Initialize()
	{
		strName       = _T("");
		strDesc       = _T("");
		bStld         = TRUE;
		nStldType     = 0;
		bThis         = FALSE;
		ThisK         = 0;
		nVertCoord    = 1;     
		dWaterDensity = 1.03;
		dWaterDepth   = 0.;
		dMudLine      = 0.;
		dGroundLevel	= 0.;
		bSelfW        = FALSE;
		bBuoyant      = TRUE;
		nCrest        = EN_CRPS_MXM;
		nUnit         = 1;
		dInitPos      = 0.;
		dStepSize     = 5.;
		nNumberPos    = 72;
		nCoefType     = 0;
		bOverride     = FALSE;
		nTheory       = EN_WVTH_AIRY; 
		nFunc         = 10;
		dWaveDir      = 0.;   
		dWaveHeight   = 2.;   
		nLengthPeriod = 1;    
		dLength       = 10.;
		dPeriod       = 0.;
		dKinematicF   = 0.85;
		dSurfaceV     = 3.;   
		dBottomV      = 0.;   
		dCurDir       = 0.;
		dCurBlockageF = 1.;
		aCurProfile.RemoveAll();
		aFlood.RemoveAll();
		aMarineGrowth.RemoveAll();
		nGridX        = 73;
		nGridZ        = 20;
		for(int i=0; i<20; i++) for(int j=0; j<73; j++)
		{
			aUserGrid[i][j].Initialize();
			if(i<10) aTraj[i][j].Initialize();
		}

		aCoef_s.RemoveAll();
		aCoef_r.RemoveAll();
		aOver_s.RemoveAll();
		aOver_r.RemoveAll();
	}
};


////////////////////////////////////////////////////////////////
struct T_WVLD_D_CH
{
	// General Data
	char		 strName[20];
	char		 strDesc[80];
	BOOL     bStld;          // static load
	UINT     nStldType;      // 0:Nodal Load, 1:Beam Load (default : 0) : 800 추가 
	BOOL     bThis;          // time history load
	T_THIS_K ThisK;
	int      nVertCoord;     // 0:Y, 1:Z
	double   dWaterDensity;
	double   dWaterDepth;
	double   dMudLine;
	double	 dGroundLevel;
	BOOL     bSelfW;
	BOOL     bBuoyant;
	CRESTPOS nCrest;
	int      nUnit;          // 0:length[L], 1:period[deg], 2:time[sec]
	double   dInitPos;       // [L], [deg], [sec]
	double   dStepSize;      // [L], [deg], [sec]
	int      nNumberPos;
// Drag & Inertia Coefficient
	int      nCoefType;
	CArray<T_WVCF_D, T_WVCF_D&> aCoef_s; 
	CArray<T_WVCF_D, T_WVCF_D&> aCoef_r; 
	BOOL     bOverride;
	CArray<T_WVCF_D, T_WVCF_D&> aOver_s; 
	CArray<T_WVCF_D, T_WVCF_D&> aOver_r; 
// Wave Characteristic
	WAVETH   nTheory; 
	int      nFunc;          // STRC, STRN only
	double   dWaveDir;
	double   dWaveHeight;
	int      nLengthPeriod;  // 0:wave length, 1:wave period
	double   dLength;
	double   dPeriod;
	double   dKinematicF;
	double   dSurfaceV;
	double   dBottomV;
// Current Profile
	double   dCurDir;
	double	 dCurBlockageF;
	CArray<T_WVCP_D, T_WVCP_D&> aCurProfile;
// Flood Condition
	CArray<T_GRUP_K, T_GRUP_K> aFlood;
///// Marine Growth
	CArray<T_WVMG_D, T_WVMG_D&> aMarineGrowth;
///// User Defined Wave	
	int	      nGridX, nGridZ;
	T_WVUS_D	aUserGrid[20][73];
	T_WVUS_D	aTraj[10][73];

	void ConvertToString(T_WVLD_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.bStld         = bStld;
		rData.nStldType     = nStldType;
		rData.bThis         = bThis;
		rData.ThisK         = ThisK;
		rData.nVertCoord    = nVertCoord;
		rData.dWaterDensity = dWaterDensity;
		rData.dWaterDepth   = dWaterDepth;
		rData.dMudLine      = dMudLine;
		rData.dGroundLevel	= dGroundLevel;
		rData.bSelfW        = bSelfW;
		rData.bBuoyant      = bBuoyant;
		rData.nCrest        = nCrest;
		rData.nUnit         = nUnit;
		rData.dInitPos      = dInitPos;
		rData.dStepSize     = dStepSize;
		rData.nNumberPos    = nNumberPos;
		rData.nCoefType     = nCoefType;
		rData.aCoef_s.Copy(aCoef_s);
		rData.aCoef_r.Copy(aCoef_r);
		rData.bOverride     = bOverride;
		rData.aOver_s.Copy(aOver_s);
		rData.aOver_r.Copy(aOver_r);
		rData.nTheory       = nTheory;
		rData.nFunc         = nFunc;
		rData.dWaveDir      = dWaveDir;
		rData.dWaveHeight   = dWaveHeight;
		rData.nLengthPeriod = nLengthPeriod;
		rData.dLength       = dLength;
		rData.dPeriod       = dPeriod;
		rData.dKinematicF   = dKinematicF;
		rData.dSurfaceV     = dSurfaceV;
		rData.dBottomV      = dBottomV;
		rData.dCurDir       = dCurDir;
		rData.dCurBlockageF = dCurBlockageF;
		rData.aCurProfile.Copy(aCurProfile);
		rData.aFlood.Copy(aFlood);
		rData.aMarineGrowth.Copy(aMarineGrowth);
		rData.nGridX        = nGridX;
		rData.nGridZ        = nGridZ;
		for(int i=0; i<20; i++) for(int j=0; j<73; j++)
		{
			rData.aUserGrid[i][j] = aUserGrid[i][j];
			if(i<10) rData.aTraj[i][j] = aTraj[i][j];
		}
	}
	void ConvertToChar(T_WVLD_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDesc, strDesc, sizeof(strDesc));
		bStld         = rData.bStld;
		nStldType     = rData.nStldType;
		bThis         = rData.bThis;
		ThisK         = rData.ThisK;
		nVertCoord    = rData.nVertCoord;
		dWaterDensity = rData.dWaterDensity;
		dWaterDepth   = rData.dWaterDepth;
		dMudLine      = rData.dMudLine;
		dGroundLevel	= rData.dGroundLevel;
		bSelfW        = rData.bSelfW;
		bBuoyant      = rData.bBuoyant;
		nCrest        = rData.nCrest;
		nUnit         = rData.nUnit;
		dInitPos      = rData.dInitPos;
		dStepSize     = rData.dStepSize;
		nNumberPos    = rData.nNumberPos;
		nCoefType     = rData.nCoefType;
		aCoef_s.Copy(rData.aCoef_s);
		aCoef_r.Copy(rData.aCoef_r);
		bOverride     = rData.bOverride;
		aOver_s.Copy(rData.aOver_s);
		aOver_r.Copy(rData.aOver_r);
		nTheory       = rData.nTheory;
		nFunc         = rData.nFunc;
		dWaveDir      = rData.dWaveDir;
		dWaveHeight   = rData.dWaveHeight;
		nLengthPeriod = rData.nLengthPeriod;
		dLength       = rData.dLength;
		dPeriod       = rData.dPeriod;
		dKinematicF   = rData.dKinematicF;
		dSurfaceV     = rData.dSurfaceV;
		dBottomV      = rData.dBottomV;
		dCurDir       = rData.dCurDir;
		dCurBlockageF = rData.dCurBlockageF;
		aCurProfile.Copy(rData.aCurProfile);
		aFlood.Copy(rData.aFlood);
		aMarineGrowth.Copy(rData.aMarineGrowth);
		nGridX        = rData.nGridX;
		nGridZ        = rData.nGridZ;
		for(int i=0; i<20; i++) for(int j=0; j<73; j++)
		{
			aUserGrid[i][j] = rData.aUserGrid[i][j];
			if(i<10) aTraj[i][j] = rData.aTraj[i][j];
		}
	}
};

struct T_WVLD_UDRD_D
{
	T_WVLD_K key;
	T_WVLD_D data;
};
struct T_WVLD_UDRD_D_CH
{
	T_WVLD_K key;
	T_WVLD_D_CH data;
};
struct T_WVLD_UDRD_D_CH_RW_797
{
	T_WVLD_K key;
	
	// General Data
	char		 strName[20];
	char		 strDesc[80];
	BOOL     bStld;          // static load
	BOOL     bThis;          // time history load
	T_THIS_K ThisK;
	int      nVertCoord;     // 0:Y, 1:Z
	double   dWaterDensity;
	double   dWaterDepth;
	double   dMudLine;
	double	 dGroundLevel;
	BOOL     bSelfW;
	BOOL     bBuoyant;
	CRESTPOS nCrest;
	int      nUnit;          // 0:length[L], 1:period[deg], 2:time[sec]
	double   dInitPos;       // [L], [deg], [sec]
	double   dStepSize;      // [L], [deg], [sec]
	int      nNumberPos;
	
	// Drag & Inertia Coefficient
	int      nCoefType;
	int nCount_aCoef_s;
	int nCount_aCoef_r;
	
	BOOL     bOverride;  
	int nCount_aOver_s;
	int nCount_aOver_r;
	
	// Wave Characteristic
	WAVETH   nTheory; 
	int      nFunc;          // STRC, STRN only
	double   dWaveDir;
	double   dWaveHeight;
	int      nLengthPeriod;  // 0:wave length, 1:wave period
	double   dLength;
	double   dPeriod;
	double   dKinematicF;
	double   dSurfaceV;
	double   dBottomV;
	
	// Current Profile
	double   dCurDir;
	double	 dCurBlockageF;
	int nCount_aCurProfile;
	
	// Flood Condition
	int nCount_aFlood;
	
	// Marine Growth
	int nCount_aMarineGrowth;
	
	// User Defined Wave	
	int	      nGridX, nGridZ;
	T_WVUS_D	aUserGrid[20][73];
	T_WVUS_D	aTraj[10][73];
};
struct T_WVLD_UDRD_D_CH_RW
{
	T_WVLD_K key;
	
	// General Data
	char		 strName[20];
	char		 strDesc[80];
	BOOL     bStld;          // static load
	UINT     nStldType;      // 0:Nodal Load, 1:Beam Load (default : 0) : 800 추가 
	BOOL     bThis;          // time history load
	T_THIS_K ThisK;
	int      nVertCoord;     // 0:Y, 1:Z
	double   dWaterDensity;
	double   dWaterDepth;
	double   dMudLine;
	double	 dGroundLevel;
	BOOL     bSelfW;
	BOOL     bBuoyant;
	CRESTPOS nCrest;
	int      nUnit;          // 0:length[L], 1:period[deg], 2:time[sec]
	double   dInitPos;       // [L], [deg], [sec]
	double   dStepSize;      // [L], [deg], [sec]
	int      nNumberPos;
	
	// Drag & Inertia Coefficient
	int      nCoefType;
	int nCount_aCoef_s;
	int nCount_aCoef_r;

	BOOL     bOverride;  
	int nCount_aOver_s;
	int nCount_aOver_r;

	// Wave Characteristic
	WAVETH   nTheory; 
	int      nFunc;          // STRC, STRN only
	double   dWaveDir;
	double   dWaveHeight;
	int      nLengthPeriod;  // 0:wave length, 1:wave period
	double   dLength;
	double   dPeriod;
	double   dKinematicF;
	double   dSurfaceV;
	double   dBottomV;
	
	// Current Profile
	double   dCurDir;
	double	 dCurBlockageF;
	int nCount_aCurProfile;
	
	// Flood Condition
	int nCount_aFlood;
	
	// Marine Growth
	int nCount_aMarineGrowth;
	
	// User Defined Wave	
	int	      nGridX, nGridZ;
	T_WVUS_D	aUserGrid[20][73];
	T_WVUS_D	aTraj[10][73];

	void GetWvld(T_WVLD_K& rKey, T_WVLD_D_CH& rData)
	{
		rKey=key;

		memcpy(rData.strName, strName, sizeof(strName));    
		memcpy(rData.strDesc, strDesc, sizeof(strDesc));    
		rData.bStld         = bStld;
		rData.nStldType     = nStldType;
		rData.bThis         = bThis;
		rData.ThisK         = ThisK;
		rData.nVertCoord    = nVertCoord;
		rData.dWaterDensity = dWaterDensity;
		rData.dWaterDepth   = dWaterDepth;
		rData.dMudLine      = dMudLine;
		rData.dGroundLevel	= dGroundLevel;
		rData.bSelfW        = bSelfW;
		rData.bBuoyant      = bBuoyant;
		rData.nCrest        = nCrest;
		rData.nUnit         = nUnit;
		rData.dInitPos      = dInitPos;
		rData.dStepSize     = dStepSize;
		rData.nNumberPos    = nNumberPos;
		rData.nCoefType     = nCoefType;
		rData.bOverride     = bOverride;
		rData.nTheory       = nTheory;
		rData.nFunc         = nFunc;
		rData.dWaveDir      = dWaveDir;
		rData.dWaveHeight   = dWaveHeight;
		rData.nLengthPeriod = nLengthPeriod;
		rData.dLength       = dLength;
		rData.dPeriod       = dPeriod;
		rData.dKinematicF   = dKinematicF;
		rData.dSurfaceV     = dSurfaceV;
		rData.dBottomV      = dBottomV;
		rData.dCurDir       = dCurDir;
		rData.dCurBlockageF = dCurBlockageF;
		rData.nGridX        = nGridX;
		rData.nGridZ        = nGridZ;
		for(int i=0; i<20; i++) for(int j=0; j<73; j++)
		{
			rData.aUserGrid[i][j] = aUserGrid[i][j];
			if(i<10) rData.aTraj[i][j] = aTraj[i][j];
		}
	}
	void SetWvld(const T_WVLD_K Key, const T_WVLD_D_CH& rData)
	{
		key=Key;    

		memcpy(strName, rData.strName, sizeof(strName));        
		memcpy(strDesc, rData.strDesc, sizeof(strDesc));        
		bStld         = rData.bStld;
		nStldType     = rData.nStldType;
		bThis         = rData.bThis;
		ThisK         = rData.ThisK;
		nVertCoord    = rData.nVertCoord;
		dWaterDensity = rData.dWaterDensity;
		dWaterDepth   = rData.dWaterDepth;
		dMudLine      = rData.dMudLine;
		dGroundLevel	= rData.dGroundLevel;
		bSelfW        = rData.bSelfW;
		bBuoyant      = rData.bBuoyant;
		nCrest        = rData.nCrest;
		nUnit         = rData.nUnit;
		dInitPos      = rData.dInitPos;
		dStepSize     = rData.dStepSize;
		nNumberPos    = rData.nNumberPos;
		nCoefType     = rData.nCoefType;
		bOverride     = rData.bOverride;
		nTheory       = rData.nTheory;
		nFunc         = rData.nFunc;
		dWaveDir      = rData.dWaveDir;
		dWaveHeight   = rData.dWaveHeight;
		nLengthPeriod = rData.nLengthPeriod;
		dLength       = rData.dLength;
		dPeriod       = rData.dPeriod;
		dKinematicF   = rData.dKinematicF;
		dSurfaceV     = rData.dSurfaceV;
		dBottomV      = rData.dBottomV;
		dCurDir       = rData.dCurDir;    
		dCurBlockageF = rData.dCurBlockageF;
		nGridX        = rData.nGridX;
		nGridZ        = rData.nGridZ;
		for(int i=0; i<20; i++) for(int j=0; j<73; j++)
		{
			aUserGrid[i][j] = rData.aUserGrid[i][j];
			if(i<10) aTraj[i][j] = rData.aTraj[i][j];
		}
		nCount_aCoef_s			= (int)rData.aCoef_s.GetSize()			;
		nCount_aCoef_r			= (int)rData.aCoef_r.GetSize()			;
		nCount_aOver_s			= (int)rData.aOver_s.GetSize()			;
		nCount_aOver_r			= (int)rData.aOver_r.GetSize()			;
		nCount_aCurProfile	= (int)rData.aCurProfile.GetSize()	;
		nCount_aFlood				= (int)rData.aFlood.GetSize()			;
		nCount_aMarineGrowth= (int)rData.aMarineGrowth.GetSize();
	}
	void Convert797(T_WVLD_UDRD_D_CH_RW_797& rData)
	{
		key=rData.key;
		
		memcpy(strName, rData.strName, sizeof(strName));        
		memcpy(strDesc, rData.strDesc, sizeof(strDesc));        
		bStld         = rData.bStld;
		nStldType     = 0;
		bThis         = rData.bThis;
		ThisK         = rData.ThisK;
		nVertCoord    = rData.nVertCoord;
		dWaterDensity = rData.dWaterDensity;
		dWaterDepth   = rData.dWaterDepth;
		dMudLine      = rData.dMudLine;
		dGroundLevel	= rData.dGroundLevel;
		bSelfW        = rData.bSelfW;
		bBuoyant      = rData.bBuoyant;
		nCrest        = rData.nCrest;
		nUnit         = rData.nUnit;
		dInitPos      = rData.dInitPos;
		dStepSize     = rData.dStepSize;
		nNumberPos    = rData.nNumberPos;
		nCoefType     = rData.nCoefType;
		bOverride     = rData.bOverride;
		nTheory       = rData.nTheory;
		nFunc         = rData.nFunc;
		dWaveDir      = rData.dWaveDir;
		dWaveHeight   = rData.dWaveHeight;
		nLengthPeriod = rData.nLengthPeriod;
		dLength       = rData.dLength;
		dPeriod       = rData.dPeriod;
		dKinematicF   = rData.dKinematicF;
		dSurfaceV     = rData.dSurfaceV;
		dBottomV      = rData.dBottomV;
		dCurDir       = rData.dCurDir;    
		dCurBlockageF = rData.dCurBlockageF;
		nGridX        = rData.nGridX;
		nGridZ        = rData.nGridZ;
		for(int i=0; i<20; i++) for(int j=0; j<73; j++)
		{
			aUserGrid[i][j] = rData.aUserGrid[i][j];
			if(i<10) aTraj[i][j] = rData.aTraj[i][j];
		}
		nCount_aCoef_s			= rData.nCount_aCoef_s		    ;
		nCount_aCoef_r			= rData.nCount_aCoef_r        ;
		nCount_aOver_s			= rData.nCount_aOver_s        ;
		nCount_aOver_r			= rData.nCount_aOver_r        ;
		nCount_aCurProfile	= rData.nCount_aCurProfile    ; 
		nCount_aFlood				= rData.nCount_aFlood         ;
		nCount_aMarineGrowth= rData.nCount_aMarineGrowth  ;
	}
};

struct T_WVLD_UNIT
{
	T_WVLD_UNIT()
	{
		nWaterDensity   =  D_UNITSYS_BASE_DENSITY;
		nWaterDepth			=  D_UNITSYS_BASE_LENGTH ;
		nMudLine				=  D_UNITSYS_BASE_LENGTH ;
		nGroundLevel		=  D_UNITSYS_BASE_LENGTH ;
		nInitPos[0]			=  D_UNITSYS_BASE_LENGTH ;      // [L], [deg], [sec]
		nInitPos[1]			=  D_UNITSYS_BASE_DEGREE ;      // [L], [deg], [sec]
		nInitPos[2]			=  D_UNITSYS_BASE_TIME ;				// [L], [deg], [sec]
		nStepSize[0]		=  D_UNITSYS_BASE_LENGTH ;			// [L], [deg], [sec]
		nStepSize[1]		=  D_UNITSYS_BASE_DEGREE ;			// [L], [deg], [sec]
		nStepSize[2]		=  D_UNITSYS_BASE_TIME ;				// [L], [deg], [sec]
		nWaveDir				=  D_UNITSYS_BASE_DEGREE ;
		nWaveHeight			=  D_UNITSYS_BASE_LENGTH ;
		nLength					=  D_UNITSYS_BASE_LENGTH ;
		nPeriod					=  D_UNITSYS_BASE_TIME ;
		nSurfaceV				=  D_UNITSYS_BASE_VELOCITY ;
		nBottomV				=  D_UNITSYS_BASE_VELOCITY ;
		nCurDir					=  D_UNITSYS_BASE_DEGREE ;
		nDiameter				=  D_UNITSYS_BASE_LENGTH ;				// Diameter
		nWVCPD					=  D_UNITSYS_BASE_LENGTH ;				// depth
		nWVCPV					=  D_UNITSYS_BASE_VELOCITY ;			// velocity
		nWVMGZ					=  D_UNITSYS_BASE_LENGTH ;
		nWVMGThick			=  D_UNITSYS_BASE_LENGTH ;
		nWVUSX					=  D_UNITSYS_BASE_LENGTH ;
		nWVUSZ					=  D_UNITSYS_BASE_LENGTH ;
		nWVUSElevation  =  D_UNITSYS_BASE_LENGTH ;
		nWVUSVx					=  D_UNITSYS_BASE_VELOCITY ;
		nWVUSVcx				=	 D_UNITSYS_BASE_VELOCITY ;
		nWVUSVt					=  D_UNITSYS_BASE_VELOCITY ;
		nWVUSVz					=  D_UNITSYS_BASE_ACCELERATION ;
		nWVUSAx					=  D_UNITSYS_BASE_ACCELERATION ;
		nWVUSAz					=  D_UNITSYS_BASE_ACCELERATION ;
	}

	int   nWaterDensity;
	int   nWaterDepth;
	int   nMudLine;
	int		nGroundLevel;
	int   nInitPos[3];       // [L], [deg], [sec]
	int   nStepSize[3];      // [L], [deg], [sec]
	int   nWaveDir;
	int   nWaveHeight;
	int   nLength;
	int   nPeriod;
	int   nSurfaceV;
	int   nBottomV;
	int   nCurDir;
	//WVCF
	int   nDiameter;  // Diameter
	// WVCP
	int	nWVCPD; // depth
	int	nWVCPV; // velocity
	//WVMG
	int	nWVMGZ;
	int	nWVMGThick;
	// WVUS
	int	nWVUSX;
	int	nWVUSZ;
	int	nWVUSElevation;
	int	nWVUSVx;
	int	nWVUSVcx;
	int	nWVUSVt;
	int	nWVUSVz;
	int	nWVUSAx;
	int	nWVUSAz;
};

#define HASHSIZEWVLD 11

#pragma pack(pop)

#endif  // __DB_ST_DT_WVLD_H__




