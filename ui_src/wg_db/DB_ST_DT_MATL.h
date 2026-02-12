#ifndef __DB_ST_DT_MATL_H__
#define __DB_ST_DT_MATL_H__

#include <map>
#define MIDAS_SDK

#pragma pack(push, 1)

#define T_MATL_K unsigned int


// 변수 추가시 CDBLib::FillMatdFromMatl 함수 수정
struct T_MATL_ALUMINUM
{
	double Elast;
	double Poisson;
	double Thermal;
	double Density;
	double MassDensity;
	double A_Ftu; // in AA : tensile ultimate strength. (Aluminum Design Manual)
	double A_Fty; // in AA : tensile yield strength. (Aluminum Design Manual)
	double A_Fcy; // in AA : compressive yield strength. (Aluminum Design Manual)
	double A_Fsu; // in AA : shear ultimate strength. (Aluminum Design Manual)
	double A_Fsy; // in AA : shear yield strength. (Aluminum Design Manual)
	double A_Fbu; // in AA : bearing ultimate strength. (Aluminum Construction Manual)
	double A_Fby; // in AA : bearing yield strength. (Aluminum Construction Manual)
	
	double dT_From;
	double dT_To;

	double A_Fu;
	double A_Fy1;
	double A_Fy2;
	
	void Initialize()
	{
		Elast = 0.0;
		Poisson = 0.0;
		Thermal = 0.0;
		Density = 0.0;
		MassDensity = 0.0;
		A_Ftu = 0.0;
		A_Fty = 0.0;
		A_Fcy = 0.0;
		A_Fsu = 0.0;
		A_Fsy = 0.0;
		A_Fbu = 0.0;
		A_Fby = 0.0;
		dT_From = 0.0;
		dT_To   = 0.0;

		A_Fu  = 0.0;
		A_Fy1 = 0.0;
		A_Fy2 = 0.0;
	}
};

struct T_MATL_UNIT_ALUMINUM
{
	T_MATL_UNIT_ALUMINUM()
	{
		Elast  =D_UNITSYS_BASE_ELAST;
		Poisson=D_UNITSYS_NONE;
		Thermal=D_UNITSYS_BASE_THERMAL;
		Density=D_UNITSYS_BASE_DENSITY;
		MassDensity=D_UNITSYS_BASE_MASSDENSITY;
		A_Ftu =D_UNITSYS_BASE_STRESS;
		A_Fty =D_UNITSYS_BASE_STRESS;
		A_Fcy =D_UNITSYS_BASE_STRESS;
		A_Fsu =D_UNITSYS_BASE_STRESS;
		A_Fsy =D_UNITSYS_BASE_STRESS;
		A_Fbu =D_UNITSYS_BASE_STRESS;
		A_Fby =D_UNITSYS_BASE_STRESS;
		dT_From=D_UNITSYS_BASE_LENGTH;
		dT_To  =D_UNITSYS_BASE_LENGTH;
		A_Fu  = D_UNITSYS_BASE_STRESS;
		A_Fy1 = D_UNITSYS_BASE_STRESS;
		A_Fy2 = D_UNITSYS_BASE_STRESS;
	}
	int Elast;
	int Poisson;
	int Thermal;
	int Density;
	int MassDensity;
	int A_Ftu;
	int A_Fty;
	int A_Fcy;
	int A_Fsu;
	int A_Fsy;
	int A_Fbu;
	int A_Fby;
	int dT_From;
	int dT_To;
	int A_Fu;
	int A_Fy1;
	int A_Fy2;
};

// 변수 추가시 CDBLib::FillMatdFromMatl 함수 수정
struct T_MATL_TIMBER
{
	double fmk;		// Strength Property (Bending)
	double ft0k;	// Strength Property (Tension Parallel to the Grain)
	double ft90k;	// Strength Property (Tension Perpendicular to the Grain)
	double fc0k;	// Strength Property (Compression Parallel to the Grain)
	double fc90k;	// Strength Property (Compression Perpendicular to the Grain)
	double fvk;		// Strength Property (Shear)

	double E0mean;	// Stiffness Property (Mean modulus of elasticity parallel)
	double E0_05;		// Stiffness Property (5% modulus of elasticity parallel)
	double E90mean;	// Stiffness Property (Mean modulus of elasticity perpendicular)
	double Gmean;		// Stiffness Property (Mean shear modulus)
	double nu;			// Poisson ratio
	double Wd;			// Design value of unit weight
	double Density_char;	// characteristic density
	double Density_mean;	// Mean density
	double dGamma_m;
	double dBeta_c;
	double Thermal;	// Coefficient of thermal expansion
	double dBeta_n;

	double Elast_x; /**< 탄성 계수( elastic modulus(E) ) */
	double Poisson_x; /**< 프와송 비( poisson's ratio ) */
	double Thermal_x; /**< 열 계수( thermal coefficient(α) ) */
	double Density; /**< 재질의 밀도 */
	double MassDensity; /**< 재질의 질량 밀도 */     // 600에서 추가
	double Elast_y; /**< Y축에 대한 탄성 계수(elastic modulus(Ey) about y-axis) */
	double Poisson_y; /**< Y축에 대한 프와송 비(poisson's ratio(νy) about y-axis) */
	double Thermal_y; /**< Y축에 대한 열 계수 */
	double Elast_z; /**< Z축에 대한 탄성 계수(elastic modulus(Ez) about z-axis) */
	double Poisson_z; /**< Z축에 대한 프와송 비(poisson's ratio(νz) about z-axis) */
	double Thermal_z; /**< Z축에 대한 열 계수 */
	double ShearModulus_x; /**< 재질의 전단 탄성 계수( shear modulus(G) ) */
	double ShearModulus_y; /**< Y축에 대한 전단 탄성 계수(shear modulus(Gy) about y-axis) */
	double ShearModulus_z; /**< Z축에 대한 전단 탄성 계수(shear modulus(Gz) about z-axis) */

	void Initialize()
	{
		fmk = 0.;
		ft0k = 0.;
		ft90k = 0.;
		fc0k = 0.;
		fc90k = 0.;
		fvk = 0.;

		E0mean = 0.;
		E0_05 = 0.;
		E90mean = 0.;
		Gmean = 0.;
		Density_char = 0.;
		Density_mean = 0.;
		Wd = 0.;
		Thermal = 0.;
		nu = 0.;
		dGamma_m = 0.;
		dBeta_c = 0.;
		dBeta_n = 0.;

		Elast_x = 0.;
		Poisson_x = 0.;
		Thermal_x = 0.;
		Density = 0.;
		MassDensity = 0.;
		Elast_y = 0.;
		Poisson_y = 0.;
		Thermal_y = 0.;
		Elast_z = 0.;
		Poisson_z = 0.;
		Thermal_z = 0.;
		ShearModulus_x = 0.;
		ShearModulus_y = 0.;
		ShearModulus_z = 0.;
	}
	T_MATL_TIMBER()
	{
		Initialize();
	}
};

struct T_MATL_UNIT_TIMBER
{
	T_MATL_UNIT_TIMBER()
	{
		fmk = D_UNITSYS_BASE_STRESS;
		ft0k = D_UNITSYS_BASE_STRESS;
		ft90k = D_UNITSYS_BASE_STRESS;
		fc0k = D_UNITSYS_BASE_STRESS;
		fc90k = D_UNITSYS_BASE_STRESS;
		fvk = D_UNITSYS_BASE_STRESS;

		E0mean = D_UNITSYS_BASE_ELAST;
		E0_05 = D_UNITSYS_BASE_ELAST;
		E90mean = D_UNITSYS_BASE_ELAST;
		Gmean = D_UNITSYS_BASE_ELAST;
		Density_char = D_UNITSYS_BASE_MASSDENSITY;
		Density_mean = D_UNITSYS_BASE_MASSDENSITY;
		Wd = D_UNITSYS_BASE_DENSITY;
		Thermal = D_UNITSYS_BASE_THERMAL;

		Elast_x = D_UNITSYS_BASE_ELAST;
		Thermal_x = D_UNITSYS_BASE_THERMAL;
		Density = D_UNITSYS_BASE_DENSITY;
		MassDensity = D_UNITSYS_BASE_MASSDENSITY;
		Elast_y = D_UNITSYS_BASE_ELAST;
		Thermal_y = D_UNITSYS_BASE_THERMAL;
		Elast_z = D_UNITSYS_BASE_ELAST;
		Thermal_z = D_UNITSYS_BASE_THERMAL;
		ShearModulus_x = D_UNITSYS_BASE_ELAST;
		ShearModulus_y = D_UNITSYS_BASE_ELAST;
		ShearModulus_z = D_UNITSYS_BASE_ELAST;
	}
	int fmk;		// Strength Property (Bending)
	int ft0k;	// Strength Property (Tension Parallel to the Grain)
	int ft90k;	// Strength Property (Tension Perpendicular to the Grain)
	int fc0k;	// Strength Property (Compression Parallel to the Grain)
	int fc90k;	// Strength Property (Compression Perpendicular to the Grain)
	int fvk;		// Strength Property (Shear)

	int E0mean;	// Stiffness Property (Mean modulus of elasticity parallel)
	int E0_05;		// Stiffness Property (5% modulus of elasticity parallel)
	int E90mean;	// Stiffness Property (Mean modulus of elasticity perpendicular)
	int Gmean;		// Stiffness Property (Mean shear modulus)
	int Density_char;	// characteristic density
	int Density_mean;	// Mean density
	int Wd;			// Design value of unit weight
	int Thermal;	// Coefficient of thermal expansion

	int Elast_x; /**< 탄성 계수( elastic modulus(E) ) */
	int Thermal_x; /**< 열 계수( thermal coefficient(α) ) */
	int Density; /**< 재질의 밀도 */
	int MassDensity; /**< 재질의 질량 밀도 */     // 600에서 추가
	int Elast_y; /**< Y축에 대한 탄성 계수(elastic modulus(Ey) about y-axis) */
	int Thermal_y; /**< Y축에 대한 열 계수 */
	int Elast_z; /**< Z축에 대한 탄성 계수(elastic modulus(Ez) about z-axis) */
	int Thermal_z; /**< Z축에 대한 열 계수 */
	int ShearModulus_x; /**< 재질의 전단 탄성 계수( shear modulus(G) ) */
	int ShearModulus_y; /**< Y축에 대한 전단 탄성 계수(shear modulus(Gy) about y-axis) */
	int ShearModulus_z; /**< Z축에 대한 전단 탄성 계수(shear modulus(Gz) about z-axis) */
};

// 변수 추가시 CDBLib::FillMatdFromMatl 함수 수정
struct T_MATL_STEEL
{
	double Elast;
	double Poisson;
	double Thermal;
	double Density;
	double MassDensity;
	double S_Fu;
	double S_Fy1;
	double S_Fy2;
	double S_Fy3;
	double S_Fy4;
	double S_Fy5; // add by Seungjun (`06.05.29) at 710
	double S_Fy6; // add by Seungjun (`06.05.29) at 710
	double S_Fy7;
	double S_Fy8;
	double S_Fy9;
	double S_Fy10;
	double S_Aft;
	double S_Aft2; // Add, Jaeoh.('07.04.12) at 730
	double S_Aft3; // Add, Jaeoh.('07.04.12) at 730
	double S_Fy;
	double S_Afv;
	double S_Afv2; // Add, Jaeoh.('07.04.12) at 730
	double S_Afv3; // Add, Jaeoh.('07.04.12) at 730
	void Initialize()
	{
		Elast = 0.;
		Poisson = 0.;
		Thermal = 0.;
		Density = 0.;
		MassDensity = 0.;
		S_Fu = 0.;
		S_Fy1 = 0.;
		S_Fy2 = 0.;
		S_Fy3 = 0.;
		S_Fy4 = 0.;
		S_Fy5 = 0.; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = 0.; // add by Seungjun (`06.05.29) at 710
		S_Fy7 = 0.0;
		S_Fy8 = 0.0;
		S_Fy9 = 0.0;
		S_Fy10 = 0.0;
		S_Aft = 0.;
		S_Aft2 = 0.; // Add, Jaeoh.('07.04.12) at 730
		S_Aft3 = 0.; // Add, Jaeoh.('07.04.12) at 730
		S_Fy = 0.;
		S_Afv = 0.;
		S_Afv2 = 0.; // Add, Jaeoh.('07.04.12) at 730
		S_Afv3 = 0.; // Add, Jaeoh.('07.04.12) at 730
	}
	T_MATL_STEEL()
	{
		Initialize();
	}
};

struct T_MATL_UNIT_STEEL
{
	T_MATL_UNIT_STEEL()
	{
		Elast  =D_UNITSYS_BASE_ELAST;
		Poisson=D_UNITSYS_NONE;
		Thermal=D_UNITSYS_BASE_THERMAL;
		Density=D_UNITSYS_BASE_DENSITY;
		MassDensity=D_UNITSYS_BASE_MASSDENSITY;
		S_Fu   =D_UNITSYS_BASE_STRESS;
		S_Fy1  =D_UNITSYS_BASE_STRESS;
		S_Fy2  =D_UNITSYS_BASE_STRESS;
		S_Fy3  =D_UNITSYS_BASE_STRESS;
		S_Fy4  =D_UNITSYS_BASE_STRESS;
		S_Fy5  =D_UNITSYS_BASE_STRESS; // add by Seungjun (`06.05.29) at 710
		S_Fy6  =D_UNITSYS_BASE_STRESS; // add by Seungjun (`06.05.29) at 710
		S_Fy7  =D_UNITSYS_BASE_STRESS;
		S_Fy8  =D_UNITSYS_BASE_STRESS;
		S_Fy9  =D_UNITSYS_BASE_STRESS;
		S_Fy10 =D_UNITSYS_BASE_STRESS;
		S_Aft  =D_UNITSYS_BASE_STRESS;
		S_Aft2 =D_UNITSYS_BASE_STRESS;
		S_Aft3 =D_UNITSYS_BASE_STRESS;
		S_Fy   =D_UNITSYS_BASE_STRESS;
		S_Afv  =D_UNITSYS_BASE_STRESS;
		S_Afv2 =D_UNITSYS_BASE_STRESS;
		S_Afv3 =D_UNITSYS_BASE_STRESS;
	}
	int Elast;
	int Poisson;
	int Thermal;
	int Density;
	int MassDensity;
	int S_Fu;
	int S_Fy1;
	int S_Fy2;
	int S_Fy3;
	int S_Fy4;
	int S_Fy5; // add by Seungjun (`06.05.29) at 710
	int S_Fy6; // add by Seungjun (`06.05.29) at 710
	int S_Fy7;
	int S_Fy8;
	int S_Fy9;
	int S_Fy10;
	int S_Aft;
	int S_Aft2;
	int S_Aft3;
	int S_Fy;
	int S_Afv;
	int S_Afv2;
	int S_Afv3;
};
// 변수 추가시 CDBLib::FillMatdFromMatl 함수 수정
struct T_MATL_CONCRETE
{
	double Elast;
	double Poisson;
	double Thermal;
	double Density;
	double MassDensity;
	double C_fc; 
	double C_fci;
	void Initialize()
	{
		Elast=0.;
		Poisson=0.;
		Thermal=0.;
		Density=0.;
		MassDensity= 0.;
		C_fc=0.;
		C_fci=0.;
	}
	
};

struct T_MATL_UNIT_CONCRETE
{
	T_MATL_UNIT_CONCRETE()
	{
		Elast  =D_UNITSYS_BASE_ELAST;
		Poisson=D_UNITSYS_NONE;
		Thermal=D_UNITSYS_BASE_THERMAL;
		Density=D_UNITSYS_BASE_DENSITY;
		MassDensity=D_UNITSYS_BASE_MASSDENSITY;
		C_fc   =D_UNITSYS_BASE_STRESS;     
		C_fci  =D_UNITSYS_BASE_STRESS;     
	}
	int Elast;
	int Poisson;
	int Thermal;
	int Density;
	int MassDensity;
	int C_fc;
	int C_fci;
};
struct T_MATL_REBAR
{
	double B_fy;   
	double B_Elast;
	double B_Density;
	void Initialize()
	{
		B_fy=0.;
		B_Elast=0.;
		B_Density=0.;
	}
};
struct T_MATL_UNIT_REBAR
{
	T_MATL_UNIT_REBAR()
	{
		B_fy     =D_UNITSYS_BASE_STRESS;
		B_Elast  =D_UNITSYS_BASE_ELAST;
		B_Density=D_UNITSYS_BASE_DENSITY;
	}
	int B_fy;
	int B_Elast;
	int B_Density;
};
struct T_MATL_ANALYSIS_550
{
	double Elast;
	double Poisson;
	double Thermal;
	double Density;
	void Initialize()
	{
		Elast=0.;
		Poisson=0.;
		Thermal=0.;
		Density=0.;
	}
};
struct T_MATL_ANALYSIS_580
{
	double Elast;
	double Poisson;
	double Thermal;
	double Density;
	double Elast_y;
	double Poisson_y;
	double Thermal_y;
	double Elast_z;
	double Poisson_z;
	double Thermal_z;
	double ShearModulus;
	double ShearModulus_y;
	double ShearModulus_z;
	int    nIsoOrtho;   // 0 : Isotropic, 1 : Orthotropic  
	void Initialize()
	{
		Elast=0.;
		Poisson=0.;
		Thermal=0.;
		Density=0.;
		Elast_y=0.;
		Poisson_y=0.;
		Thermal_y=0.;   
		Elast_z=0.;
		Poisson_z=0.;
		Thermal_z=0.;
		ShearModulus=0.;
		ShearModulus_y=0.;
		ShearModulus_z=0.;
		nIsoOrtho = 0;
	}
};

struct T_MATL_ANALYSIS_845
{
	double Elast;
	double Poisson;
	double Thermal;
	double Density;
	double MassDensity;     // 600에서 추가
	double Elast_y;
	double Poisson_y;
	double Thermal_y;
	double Elast_z;
	double Poisson_z;
	double Thermal_z;
	double ShearModulus;
	double ShearModulus_y;
	double ShearModulus_z;
	int    nIsoOrtho;   // 0 : Isotropic, 1 : Orthotropic
	void Initialize()
	{
		Elast=0.;
		Poisson=0.;
		Thermal=0.;
		Density=0.;
		MassDensity=0.0;
		Elast_y=0.;
		Poisson_y=0.;
		Thermal_y=0.;   
		Elast_z=0.;
		Poisson_z=0.;
		Thermal_z=0.;
		ShearModulus=0.;
		ShearModulus_y=0.;
		ShearModulus_z=0.;
		nIsoOrtho = 0;
	}
};

//////////////////////////////////////////////////////////////////////////
struct T_MATL_LIST_REBAR
{
	CString CodeName;
	CString MatlName;
	T_MATL_REBAR Rebar;

	void Initialize()
	{
		CodeName = _T("");
		MatlName = _T("");
		Rebar.Initialize();
	}

	T_MATL_LIST_REBAR() { Initialize(); }
	T_MATL_LIST_REBAR(const T_MATL_LIST_REBAR& src) { *this = src; }
	T_MATL_LIST_REBAR& operator=(const T_MATL_LIST_REBAR& src)
	{
		CodeName = src.CodeName;
		MatlName = src.MatlName;
		Rebar = src.Rebar;
		return *this;
	}

	T_MATL_LIST_REBAR(const CString& strCodeName, const CString& strMatlName, double dfy, double dElast, double dDensity)
	{
		CodeName = strCodeName;
		MatlName = strMatlName;
		Rebar.B_fy = dfy;
		Rebar.B_Elast = dElast;
		Rebar.B_Density = dDensity;
	}
};

typedef CArray<T_MATL_LIST_REBAR, T_MATL_LIST_REBAR&> T_MATL_LIST_REBAR_LIST;

struct T_MATL_LIST_REBAR_CH
{
	char    CodeName[40];
	char    MatlName[40];

	double B_fy;
	double B_Elast;
	double B_Density;

	void ConvertToString(T_MATL_LIST_REBAR& rData)
	{
		ConvertCharStr(CodeName, rData.CodeName, sizeof(CodeName));
		ConvertCharStr(MatlName, rData.MatlName, sizeof(MatlName));

		rData.Rebar.B_fy      = B_fy;
		rData.Rebar.B_Elast   = B_Elast;
		rData.Rebar.B_Density = B_Density;
	}
	void ConvertToChar(T_MATL_LIST_REBAR& rData)
	{
		ConvertStrChar(rData.CodeName, CodeName, sizeof(CodeName));
		ConvertStrChar(rData.MatlName, MatlName, sizeof(MatlName));

		B_fy      = rData.Rebar.B_fy;
		B_Elast   = rData.Rebar.B_Elast;
		B_Density = rData.Rebar.B_Density;
	}
};

struct _DGN_REBAR_DATA
{
	CString strName;
	int iOrb;
	double dWeight;
	double dDia;
	double dArea;
	double dOutDia;  // Add by GAY. MQC:1395. ('07.10.08). 철근의 최외경.
	double dNameDia; // Add by GAY. PMS:4215-145. ('12.01.04). 철근의 호칭직경.

	void Initialize()
	{
		strName = _T("");
		iOrb = 0;
		dWeight = 0.0;
		dDia = 0.0;
		dArea = 0.0;
		dOutDia = 0.0;
		dNameDia = 0.0;
	}

	_DGN_REBAR_DATA()
	{
		Initialize();
	}

	_DGN_REBAR_DATA(const CString& sName, int nO, double dD, double dA, double dW, double dOD = 0.0, double dND = 0.0)
	{
		strName = sName;
		iOrb = nO;
		dWeight = dW;
		dDia = dD;
		dArea = dA;
		dOutDia = (dOD > 0.0) ? dOD : dD;
		dNameDia = (dND > 0.0) ? dND : dD;
	}
};

typedef std::map<int, _DGN_REBAR_DATA> T_DGN_REBAR_DATA_MAP;
typedef CArray<_DGN_REBAR_DATA, _DGN_REBAR_DATA&> T_DGN_REBAR_DATA_LIST;

struct _DGN_REBAR_DATA_CH
{
	char    strName[40];
	int    iOrb;
	double dWeight;
	double dDia;
	double dArea;
	double dOutDia;  // Add by GAY. MQC:1395. ('07.10.08). 철근의 최외경.
	double dNameDia; // Add by GAY. PMS:4215-145. ('12.01.04). 철근의 호칭직경.
	void ConvertToString(_DGN_REBAR_DATA& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));

		rData.iOrb     = iOrb;
		rData.dWeight  = dWeight;
		rData.dDia     = dDia;
		rData.dArea    = dArea;
		rData.dOutDia  = dOutDia;
		rData.dNameDia = dNameDia;
	}
	void ConvertToChar(_DGN_REBAR_DATA& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));

		iOrb     = rData.iOrb;
		dWeight  = rData.dWeight;
		dDia     = rData.dDia;
		dArea    = rData.dArea;
		dOutDia  = rData.dOutDia;
		dNameDia = rData.dNameDia;
	}
};
//////////////////////////////////////////////////////////////////////////

struct T_TENDON_DATA
{
	CString csDiaName;
	double  dArea;
	double  dW_Factor;
	double  dC_Factor;
	double  dR_Factor;
	double  dY_Strength;
	double  dU_Strength;
	double  dAnchorageSlip;

	void Initialize()
	{
		csDiaName.Empty();
		dArea = 0.0;
		dW_Factor = 0.0;
		dC_Factor = 0.0;
		dR_Factor = 0.0;
		dY_Strength = 0.0;
		dU_Strength = 0.0;
		dAnchorageSlip = 0.0;
	}

	T_TENDON_DATA()
	{

	}

	T_TENDON_DATA(const CString& DiaName, double Area)
	{
		Initialize();
		csDiaName = DiaName;
		dArea = Area;
	}
};

typedef CArray<T_TENDON_DATA, T_TENDON_DATA&> T_TENDON_DATA_LIST;

/**
@brief 재질이 갖고 있는 고유의 성질
*/
struct T_MATL_ANALYSIS
{
	double Elast; /**< 탄성 계수( elastic modulus(E) ) */
	double Poisson; /**< 프와송 비( poisson's ratio ) */
	double Thermal; /**< 열 계수( thermal coefficient(α) ) */
	double Density; /**< 재질의 밀도 */
	double MassDensity; /**< 재질의 질량 밀도 */     // 600에서 추가
	double Elast_y; /**< Y축에 대한 탄성 계수(elastic modulus(Ey) about y-axis) */
	double Poisson_y; /**< Y축에 대한 프와송 비(poisson's ratio(νy) about y-axis) */
	double Thermal_y; /**< Y축에 대한 열 계수 */
	double Elast_z; /**< Z축에 대한 탄성 계수(elastic modulus(Ez) about z-axis) */
	double Poisson_z; /**< Z축에 대한 프와송 비(poisson's ratio(νz) about z-axis) */
	double Thermal_z; /**< Z축에 대한 열 계수 */
	double ShearModulus; /**< 재질의 전단 탄성 계수( shear modulus(G) ) */
	double ShearModulus_y; /**< Y축에 대한 전단 탄성 계수(shear modulus(Gy) about y-axis) */
	double ShearModulus_z; /**< Z축에 대한 전단 탄성 계수(shear modulus(Gz) about z-axis) */
	int    nIsoOrtho;   // 0 : Isotropic, 1 : Orthotropic
	BOOL   bUserElast;
	void Initialize()
	{
		Elast=0.;
		Poisson=0.;
		Thermal=0.;
		Density=0.;
		MassDensity=0.0;
		Elast_y=0.;
		Poisson_y=0.;
		Thermal_y=0.;   
		Elast_z=0.;
		Poisson_z=0.;
		Thermal_z=0.;
		ShearModulus=0.;
		ShearModulus_y=0.;
		ShearModulus_z=0.;
		nIsoOrtho = 0;
		bUserElast = FALSE;
	}
	void Convert550(T_MATL_ANALYSIS_550& rData)
	{
		Elast = rData.Elast;
		Poisson = rData.Poisson;
		Thermal = rData.Thermal;
		Density = rData.Density;
		MassDensity = 0.0;
		Elast_y = 0.;
		Poisson_y = 0.;
		Thermal_y = 0.;   
		Elast_z = 0.;
		Poisson_z = 0.;
		Thermal_z = 0.;
		ShearModulus=0.;
		ShearModulus_y=0.;
		ShearModulus_z=0.;
		nIsoOrtho = 0;      
		bUserElast = FALSE;
	}
	void Convert580(T_MATL_ANALYSIS_580& rData)
	{
		Elast = rData.Elast;
		Poisson = rData.Poisson;
		Thermal = rData.Thermal;
		Density = rData.Density;
		MassDensity = 0.0;
		Elast_y = rData.Elast_y;
		Poisson_y = rData.Poisson_y;
		Thermal_y = rData.Thermal_y;
		Elast_z = rData.Elast_z;
		Poisson_z = rData.Poisson_z;
		Thermal_z = rData.Thermal_z;
		ShearModulus=rData.ShearModulus;
		ShearModulus_y=rData.ShearModulus_y;
		ShearModulus_z=rData.ShearModulus_z;
		nIsoOrtho = rData.nIsoOrtho;
		bUserElast = FALSE;
	}
	void Convert845(T_MATL_ANALYSIS_845& rData)
	{
		Elast = rData.Elast;
		Poisson = rData.Poisson;
		Thermal = rData.Thermal;
		Density = rData.Density;
		MassDensity = rData.MassDensity;
		Elast_y = rData.Elast_y;
		Poisson_y = rData.Poisson_y;
		Thermal_y = rData.Thermal_y;
		Elast_z = rData.Elast_z;
		Poisson_z = rData.Poisson_z;
		Thermal_z = rData.Thermal_z;
		ShearModulus=rData.ShearModulus;
		ShearModulus_y=rData.ShearModulus_y;
		ShearModulus_z=rData.ShearModulus_z;
		nIsoOrtho = rData.nIsoOrtho;
		bUserElast = FALSE;
	}
};
struct T_MATL_DESIGN_422
{
	double S_Fu;        // steel 인 경우 사용
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Aft;       // steel 인 경우 사용
	double S_Fy;        // steel 인 경우 사용
	double S_Afv;       // steel 인 경우 사용
	double C_fc;        // concrete 인 경우 사용
	void Initialize()
	{
		S_Fu=0.; 
		S_Fy1=0.;
		S_Fy2=0.;
		S_Aft=0.;
		S_Fy=0.; 
		S_Afv=0.;
		C_fc=0.; 
	}
};
struct T_MATL_DESIGN_550
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Aft;       // steel 인 경우 사용
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double C_fc;        // concrete 인 경우 사용
	void Initialize()
	{
		S_Fu=0.; 
		S_Fy1=0.;
		S_Fy2=0.;
		S_Fy3=0.;
		S_Aft=0.;
		S_Fy=0.; 
		S_Afv=0.;
		C_fc=0.; 
	}
};
struct T_MATL_DESIGN_560
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Aft;       // steel 인 경우 사용
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double C_fc;        // concrete 인 경우 사용
	void Initialize()
	{
		S_Fu=0.; 
		S_Fy1=0.;
		S_Fy2=0.;
		S_Fy3=0.;
		S_Fy4=0.;
		S_Aft=0.;
		S_Fy=0.; 
		S_Afv=0.;
		C_fc=0.; 
	}
};
struct T_MATL_DESIGN_702
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Aft;       // steel 인 경우 사용
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	void Initialize()
	{
		S_Fu=0.; 
		S_Fy1=0.;
		S_Fy2=0.;
		S_Fy3=0.;
		S_Fy4=0.;
		S_Aft=0.;
		S_Fy=0.; 
		S_Afv=0.;
		C_fc=0.; 
		C_fci=0.;
	}
};

struct T_MATL_DESIGN_721
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Aft;       // steel 인 경우 사용
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	void Initialize()
	{
		S_Fu=0.; 
		S_Fy1=0.;
		S_Fy2=0.;
		S_Fy3=0.;
		S_Fy4=0.;
		S_Fy5=0.;
		S_Fy6=0.;
		S_Aft=0.;
		S_Fy=0.; 
		S_Afv=0.;
		C_fc=0.; 
		C_fci=0.;
	}
};

struct T_MATL_DESIGN_KSCE05
{
	double dAlwS1;    // 허용축방향 인장응력 및 허용휨인장응력              (default=0, boundary>=0, unit : Stress)
	double dAlwS2;    // 국부좌굴을 고려하지 않은 허용축방향압축응력        (default=0, boundary>=0, unit : Stress)
	double dAlwS3;    // 허용휨압축응력(횡좌굴 방지)                        (default=0, boundary>=0, unit : Stress)
	double dAlwS4;    // 허용휨압축응력(그외경우)                           (default=0, boundary>=0, unit : Stress)
	double dAlwS5;    // 허용전단응력 및 허용지압응력                       (default=0, boundary>=0, unit : Stress)
	double dAlwS6;    // 양연지지판의 국부좌굴에 대한 허용응력              (default=0, boundary>=0, unit : Stress)
	double dAlwS7;    // 자유돌출판의 국부좌굴에 대한 허용응력              (default=0, boundary>=0, unit : Stress)
	double dAlwS8;    // 보강된 판의 국부좌굴에 대한 허용응력               (default=0, boundary>=0, unit : Stress)
	double dAlwS9;    // 국부좌굴을 고려하지 않은 허용축방향압축응력 상한값 (default=0, boundary>=0, unit : Stress) 
	double dAlwS10;   // 허용휨압축응력 상한값                              (default=0, boundary>=0, unit : Stress) 
	double dMin_t1;   // 압축응력을 받는 양연지지판의 최소두께              (default=0, boundary>=0, unit : Length)
	double dMin_t2;   // 압축응력을 받는 보강된 판의 최소두께               (default=0, boundary>=0, unit : Length)
	double dMin_t3;   // 압축응력을 받는 자유돌출 판의 최소두께             (default=0, boundary>=0, unit : Length)

	void Initialize()
	{
		dAlwS1 = 0.0;
		dAlwS2 = 0.0; 
		dAlwS3 = 0.0;   
		dAlwS4 = 0.0;   
		dAlwS5 = 0.0;   
		dAlwS6 = 0.0;   
		dAlwS7 = 0.0;   
		dAlwS8 = 0.0;
		dAlwS9 = 0.0;   
		dAlwS10= 0.0;
		dMin_t1 = 0.0;  
		dMin_t2 = 0.0;
		dMin_t3 = 0.0;
	}
};

struct T_MATL_DESIGN_810
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가

	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0
	void Initialize()
	{
		S_Fu=0.; 
		S_Fy1=0.;
		S_Fy2=0.;
		S_Fy3=0.;
		S_Fy4=0.;
		S_Fy5=0.; // add by Seungjun (`06.05.29) at 710
		S_Fy6=0.; // add by Seungjun (`06.05.29) at 710
		S_Aft=0.;
		S_Fy=0.; 
		S_Afv=0.;
		C_fc=0.; 
		C_fci=0.;
		S_Aft2=0.;
		S_Aft3=0.;
		S_Afv2=0.;
		S_Afv3=0.;
		Matl_KSCE05.Initialize();
	}
};

struct T_MATL_DESIGN_830
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	
	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0
	void Initialize()
	{
		S_Fu=0.; 
		S_Fy1=0.;
		S_Fy2=0.;
		S_Fy3=0.;
		S_Fy4=0.;
		S_Fy5=0.; // add by Seungjun (`06.05.29) at 710
		S_Fy6=0.; // add by Seungjun (`06.05.29) at 710
		S_Aft=0.;
		S_Fy=0.; 
		S_Afv=0.;
		C_fc=0.; 
		C_fci=0.;
		S_Aft2=0.;
		S_Aft3=0.;
		S_Afv2=0.;
		S_Afv3=0.;
		bLambda = FALSE;
		dLambda = 1.0;
		Matl_KSCE05.Initialize();
	}
};

struct T_MATL_DESIGN_835
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	CString strMatlName; // Composite Hybrid Factor를 사용할때 사용 
	
	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0

	void Initialize()
	{
		S_Fu=0.; 
		S_Fy1=0.;
		S_Fy2=0.;
		S_Fy3=0.;
		S_Fy4=0.;
		S_Fy5=0.; // add by Seungjun (`06.05.29) at 710
		S_Fy6=0.; // add by Seungjun (`06.05.29) at 710
		S_Aft=0.;
		S_Fy=0.; 
		S_Afv=0.;
		C_fc=0.; 
		C_fci=0.;
		S_Aft2=0.;
		S_Aft3=0.;
		S_Afv2=0.;
		S_Afv3=0.;
		bLambda = FALSE;
		dLambda = 1.0;
		strMatlName.Empty();
		Matl_KSCE05.Initialize();
	}
};
struct T_MATL_DESIGN_896
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	CString strMatlName; // Composite Hybrid Factor를 사용할때 사용 
	double dGamma_m;
	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0

	void Initialize()
	{
		S_Fu=0.; 
		S_Fy1=0.;
		S_Fy2=0.;
		S_Fy3=0.;
		S_Fy4=0.;
		S_Fy5=0.; // add by Seungjun (`06.05.29) at 710
		S_Fy6=0.; // add by Seungjun (`06.05.29) at 710
		S_Aft=0.;
		S_Fy=0.; 
		S_Afv=0.;
		C_fc=0.; 
		C_fci=0.;
		S_Aft2=0.;
		S_Aft3=0.;
		S_Afv2=0.;
		S_Afv3=0.;
		bLambda = FALSE;
		dLambda = 1.0;
		strMatlName.Empty();
		dGamma_m = 0.0;
		Matl_KSCE05.Initialize();
	}
};
struct T_MATL_DESIGN_900_TEMP
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	CString strMatlName; // Composite Hybrid Factor를 사용할때 사용 
	double dGamma_m;
	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0
	BOOL   bTransfer;   // 9.0.0  호주PSC
	void Initialize()
	{
		S_Fu=0.; 
		S_Fy1=0.;
		S_Fy2=0.;
		S_Fy3=0.;
		S_Fy4=0.;
		S_Fy5=0.; // add by Seungjun (`06.05.29) at 710
		S_Fy6=0.; // add by Seungjun (`06.05.29) at 710
		S_Aft=0.;
		S_Fy=0.; 
		S_Afv=0.;
		C_fc=0.; 
		C_fci=0.;
		S_Aft2=0.;
		S_Aft3=0.;
		S_Afv2=0.;
		S_Afv3=0.;
		bLambda = FALSE;
		dLambda = 1.0;
		strMatlName.Empty();
		dGamma_m = 0.0;
		Matl_KSCE05.Initialize();
		bTransfer=FALSE;
	}
};
// 변수 추가시 CDBLib::FillMatdFromMatl 함수 수정 // later
struct T_MATL_DESIGN_925
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	CString strMatlName; // Composite Hybrid Factor를 사용할때 사용 
	double dGamma_m;
	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0
	BOOL   bTransfer;   // 9.0.0  호주PSC
	double dTransferFci;   // 9.0.0  호주PSC

	void Initialize()
	{
		S_Fu = 0.;
		S_Fy1 = 0.;
		S_Fy2 = 0.;
		S_Fy3 = 0.;
		S_Fy4 = 0.;
		S_Fy5 = 0.; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = 0.; // add by Seungjun (`06.05.29) at 710
		S_Aft = 0.;
		S_Fy = 0.;
		S_Afv = 0.;
		C_fc = 0.;
		C_fci = 0.;
		S_Aft2 = 0.;
		S_Aft3 = 0.;
		S_Afv2 = 0.;
		S_Afv3 = 0.;
		bLambda = FALSE;
		dLambda = 1.0;
		strMatlName.Empty();
		dGamma_m = 0.0;
		Matl_KSCE05.Initialize();
		bTransfer = FALSE;
		dTransferFci = 0.0;
	}
}; // end

struct T_MATL_DESIGN_950
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	CString strMatlName; // Composite Hybrid Factor를 사용할때 사용 
	double dGamma_m;
	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0
	BOOL   bTransfer;   // 9.0.0  호주PSC
	double dTransferFci;   // 9.0.0  호주PSC
	BOOL bSeamlessP;

	void Initialize()
	{
		S_Fu = 0.;
		S_Fy1 = 0.;
		S_Fy2 = 0.;
		S_Fy3 = 0.;
		S_Fy4 = 0.;
		S_Fy5 = 0.; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = 0.; // add by Seungjun (`06.05.29) at 710		
		S_Aft = 0.;
		S_Fy = 0.;
		S_Afv = 0.;
		C_fc = 0.;
		C_fci = 0.;
		S_Aft2 = 0.;
		S_Aft3 = 0.;
		S_Afv2 = 0.;
		S_Afv3 = 0.;
		bLambda = FALSE;
		dLambda = 1.0;
		strMatlName.Empty();
		dGamma_m = 0.0;
		Matl_KSCE05.Initialize();
		bTransfer = FALSE;
		dTransferFci = 0.0;
		bSeamlessP = FALSE;
	}
};

struct T_MATL_DESIGN_955
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy7;
	double S_Fy8;
	double S_Fy9;
	double S_Fy10;
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	CString strMatlName; // Composite Hybrid Factor를 사용할때 사용 
	double dGamma_m;
	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0
	BOOL   bTransfer;   // 9.0.0  호주PSC
	double dTransferFci;   // 9.0.0  호주PSC
	BOOL bSeamlessP;

	void Initialize()
	{
		S_Fu = 0.;
		S_Fy1 = 0.;
		S_Fy2 = 0.;
		S_Fy3 = 0.;
		S_Fy4 = 0.;
		S_Fy5 = 0.; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = 0.; // add by Seungjun (`06.05.29) at 710
		S_Fy7 = 0.0;
		S_Fy8 = 0.0;
		S_Fy9 = 0.0;
		S_Fy10 = 0.0;
		S_Aft = 0.;
		S_Fy = 0.;
		S_Afv = 0.;
		C_fc = 0.;
		C_fci = 0.;
		S_Aft2 = 0.;
		S_Aft3 = 0.;
		S_Afv2 = 0.;
		S_Afv3 = 0.;
		bLambda = FALSE;
		dLambda = 1.0;
		strMatlName.Empty();
		dGamma_m = 0.0;
		Matl_KSCE05.Initialize();
		bTransfer = FALSE;
		dTransferFci = 0.0;
		bSeamlessP = FALSE;
	}
}; // end

struct T_MATL_DESIGN_960
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy7;
	double S_Fy8;
	double S_Fy9;
	double S_Fy10;
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	CString strMatlName; // Composite Hybrid Factor를 사용할때 사용 
	double dGamma_m;
	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0
	BOOL   bTransfer;   // 9.0.0  호주PSC
	double dTransferFci;   // 9.0.0  호주PSC
	BOOL bSeamlessP;
	double dEta;
	void Initialize()
	{
		S_Fu = 0.;
		S_Fy1 = 0.;
		S_Fy2 = 0.;
		S_Fy3 = 0.;
		S_Fy4 = 0.;
		S_Fy5 = 0.; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = 0.; // add by Seungjun (`06.05.29) at 710
		S_Fy7 = 0.0;
		S_Fy8 = 0.0;
		S_Fy9 = 0.0;
		S_Fy10 = 0.0;
		S_Aft = 0.;
		S_Fy = 0.;
		S_Afv = 0.;
		C_fc = 0.;
		C_fci = 0.;
		S_Aft2 = 0.;
		S_Aft3 = 0.;
		S_Afv2 = 0.;
		S_Afv3 = 0.;
		bLambda = FALSE;
		dLambda = 1.0;
		strMatlName.Empty();
		dGamma_m = 0.0;
		Matl_KSCE05.Initialize();
		bTransfer = FALSE;
		dTransferFci = 0.0;
		bSeamlessP = FALSE;
		dEta = 1.0;
	}
}; // end
// 변수 추가시 CDBLib::FillMatdFromMatl 함수 수정 // later
struct T_MATL_DESIGN
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy7;
	double S_Fy8;
	double S_Fy9;
	double S_Fy10;
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	CString strMatlName; // Composite Hybrid Factor를 사용할때 사용 
	double dGamma_m;
	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0
	BOOL   bTransfer;   // 9.0.0  호주PSC
	double dTransferFci;   // 9.0.0  호주PSC
	BOOL bSeamlessP;
	double dEta;
	BOOL bChkEta;
	
	void Initialize()
	{
		S_Fu=0.; 
		S_Fy1=0.;
		S_Fy2=0.;
		S_Fy3=0.;
		S_Fy4=0.;
		S_Fy5=0.; // add by Seungjun (`06.05.29) at 710
		S_Fy6=0.; // add by Seungjun (`06.05.29) at 710
		S_Fy7 = 0.0;
		S_Fy8 = 0.0;
		S_Fy9 = 0.0;
		S_Fy10 = 0.0;
		S_Aft=0.;
		S_Fy=0.; 
		S_Afv=0.;
		C_fc=0.; 
		C_fci=0.;
		S_Aft2=0.;
		S_Aft3=0.;
		S_Afv2=0.;
		S_Afv3=0.;
		bLambda = FALSE;
		dLambda = 1.0;
		strMatlName.Empty();
		dGamma_m = 0.0;
		Matl_KSCE05.Initialize();
		bTransfer = FALSE;
		dTransferFci = 0.0;
		bSeamlessP = FALSE;
		dEta = 1.0;
		bChkEta = TRUE;
	}
	void Convert960(T_MATL_DESIGN_960& rData)
	{
		Initialize();

		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = rData.S_Fy6; // add by Seungjun (`06.05.29) at 710
		S_Fy7 = rData.S_Fy7;
		S_Fy8 = rData.S_Fy8;
		S_Fy9 = rData.S_Fy9;
		S_Fy10 = rData.S_Fy10;
		S_Aft = rData.S_Aft;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		S_Aft2 = rData.S_Aft;
		S_Aft3 = rData.S_Aft;
		S_Afv2 = rData.S_Afv;
		S_Afv3 = rData.S_Afv;
		bLambda = rData.bLambda; 
		dLambda = rData.dLambda;
		strMatlName = rData.strMatlName;
		dGamma_m = rData.dGamma_m;
		Matl_KSCE05 = rData.Matl_KSCE05;
		bTransfer = rData.bTransfer;
		dTransferFci = rData.dTransferFci;
		bSeamlessP = rData.bSeamlessP;
		dEta = rData.dEta;
		bChkEta = TRUE;
	}
	void Convert955(T_MATL_DESIGN_955& rData)
	{
		Initialize();

		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = rData.S_Fy6; // add by Seungjun (`06.05.29) at 710
		S_Fy7 = rData.S_Fy7;
		S_Fy8 = rData.S_Fy8;
		S_Fy9 = rData.S_Fy9;
		S_Fy10 = rData.S_Fy10;
		S_Aft = rData.S_Aft;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		S_Aft2 = rData.S_Aft;
		S_Aft3 = rData.S_Aft;
		S_Afv2 = rData.S_Afv;
		S_Afv3 = rData.S_Afv;
		bLambda = rData.bLambda;
		dLambda = rData.dLambda;
		strMatlName = rData.strMatlName;
		dGamma_m = rData.dGamma_m;
		Matl_KSCE05 = rData.Matl_KSCE05;
		bTransfer = rData.bTransfer;
		dTransferFci = rData.dTransferFci;
		bSeamlessP = rData.bSeamlessP;
		dEta = 1.0;
	}
	void Convert950(T_MATL_DESIGN_950& rData)
	{
		Initialize();

		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5;
		S_Fy6 = rData.S_Fy6;
		S_Aft = rData.S_Aft;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		S_Aft2 = rData.S_Aft;
		S_Aft3 = rData.S_Aft;
		S_Afv2 = rData.S_Afv;
		S_Afv3 = rData.S_Afv;
		bLambda = rData.bLambda;
		dLambda = rData.dLambda;
		strMatlName = rData.strMatlName;
		dGamma_m = rData.dGamma_m;
		Matl_KSCE05 = rData.Matl_KSCE05;
		bTransfer = rData.bTransfer;
		dTransferFci = rData.dTransferFci;
		bSeamlessP = rData.bSeamlessP;
	}
	void Convert925(T_MATL_DESIGN_925& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = rData.S_Fy6; // add by Seungjun (`06.05.29) at 710  
		S_Aft = rData.S_Aft;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		S_Aft2 = rData.S_Aft;
		S_Aft3 = rData.S_Aft;
		S_Afv2 = rData.S_Afv;
		S_Afv3 = rData.S_Afv;
		bLambda = rData.bLambda;
		dLambda = rData.dLambda;
		strMatlName = rData.strMatlName;
		dGamma_m = rData.dGamma_m;
		Matl_KSCE05 = rData.Matl_KSCE05;
		bTransfer = rData.bTransfer;
		dTransferFci = 0.0;
		bSeamlessP = FALSE;
	}
	void Convert900_TEMP(T_MATL_DESIGN_900_TEMP& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = rData.S_Fy6; // add by Seungjun (`06.05.29) at 710  
		S_Aft = rData.S_Aft;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		S_Aft2 = rData.S_Aft;
		S_Aft3 = rData.S_Aft;
		S_Afv2 = rData.S_Afv;
		S_Afv3 = rData.S_Afv;
		bLambda = rData.bLambda;
		dLambda = rData.dLambda;
		strMatlName = rData.strMatlName;
		dGamma_m = rData.dGamma_m;
		Matl_KSCE05 = rData.Matl_KSCE05;
		bTransfer = rData.bTransfer;
		dTransferFci = 0.0;
	}
	void Convert896(T_MATL_DESIGN_896& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = rData.S_Fy6; // add by Seungjun (`06.05.29) at 710  
		S_Aft = rData.S_Aft;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		S_Aft2 = rData.S_Aft;
		S_Aft3 = rData.S_Aft;
		S_Afv2 = rData.S_Afv;
		S_Afv3 = rData.S_Afv;
		bLambda = rData.bLambda;
		dLambda = rData.dLambda;
		strMatlName = rData.strMatlName;
		dGamma_m = rData.dGamma_m;
		Matl_KSCE05 = rData.Matl_KSCE05;
		bTransfer = FALSE;
		dTransferFci = 0.0;
	}
	void Convert835(T_MATL_DESIGN_835& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = rData.S_Fy6; // add by Seungjun (`06.05.29) at 710  
		S_Aft = rData.S_Aft;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		S_Aft2 = rData.S_Aft;
		S_Aft3 = rData.S_Aft;
		S_Afv2 = rData.S_Afv;
		S_Afv3 = rData.S_Afv;
		bLambda = rData.bLambda;
		dLambda = rData.dLambda;
		strMatlName = rData.strMatlName;
		dGamma_m = 0.0;
		Matl_KSCE05 = rData.Matl_KSCE05;
		bTransfer = FALSE;
		dTransferFci = 0.0;
	}
	void Convert830(T_MATL_DESIGN_830& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = rData.S_Fy6; // add by Seungjun (`06.05.29) at 710  
		S_Aft = rData.S_Aft;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		S_Aft2 = rData.S_Aft;
		S_Aft3 = rData.S_Aft;
		S_Afv2 = rData.S_Afv;
		S_Afv3 = rData.S_Afv;
		bLambda = rData.bLambda;
		dLambda = rData.dLambda;
		strMatlName.Empty();
		dGamma_m = 0.0;
		Matl_KSCE05 = rData.Matl_KSCE05;
		bTransfer = FALSE;
		dTransferFci = 0.0;
	}
	void Convert810(T_MATL_DESIGN_810& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = rData.S_Fy6; // add by Seungjun (`06.05.29) at 710  
		S_Aft = rData.S_Aft;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		S_Aft2 = rData.S_Aft;
		S_Aft3 = rData.S_Aft;
		S_Afv2 = rData.S_Afv;
		S_Afv3 = rData.S_Afv;
		bLambda = FALSE;
		dLambda = 1.0;
		strMatlName.Empty();
		dGamma_m = 0.0;
		Matl_KSCE05 = rData.Matl_KSCE05;
		bTransfer = FALSE;
		dTransferFci = 0.0;
	}
	void Convert721(T_MATL_DESIGN_721& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = rData.S_Fy6; // add by Seungjun (`06.05.29) at 710  
		S_Aft = rData.S_Aft;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		S_Aft2 = rData.S_Aft;
		S_Aft3 = rData.S_Aft;
		S_Afv2 = rData.S_Afv;
		S_Afv3 = rData.S_Afv;
		bLambda = FALSE;
		dLambda = 1.0;
		strMatlName.Empty();
		dGamma_m = 0.0;
		Matl_KSCE05.Initialize();
		bTransfer = FALSE;
		dTransferFci = 0.0;
	}
	void Convert702(T_MATL_DESIGN_702& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = S_Fy4; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = S_Fy5; // add by Seungjun (`06.05.29) at 710  
		S_Aft = rData.S_Aft;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		S_Aft2 = rData.S_Aft;
		S_Aft3 = rData.S_Aft;
		S_Afv2 = rData.S_Afv;
		S_Afv3 = rData.S_Afv;
		bLambda = FALSE;
		dLambda = 1.0;
		strMatlName.Empty();
		dGamma_m = 0.0;
		Matl_KSCE05.Initialize();
		bTransfer = FALSE;
		dTransferFci = 0.0;
	}
	void Convert560(T_MATL_DESIGN_560& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = S_Fy4; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = S_Fy5; // add by Seungjun (`06.05.29) at 710  
		S_Aft = rData.S_Aft;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		C_fc = rData.C_fc;
		C_fci = 0.;
		S_Aft2 = rData.S_Aft;
		S_Aft3 = rData.S_Aft;
		S_Afv2 = rData.S_Afv;
		S_Afv3 = rData.S_Afv;
		bLambda = FALSE;
		dLambda = 1.0;
		strMatlName.Empty();
		dGamma_m = 0.0;
		Matl_KSCE05.Initialize();
		bTransfer = FALSE;
		dTransferFci = 0.0;
	}
	void Convert550(T_MATL_DESIGN_550& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = S_Fy3;
		S_Fy5 = S_Fy4; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = S_Fy5; // add by Seungjun (`06.05.29) at 710
		S_Aft = rData.S_Aft;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		C_fc = rData.C_fc;
		C_fci = 0.;
		S_Aft2 = rData.S_Aft;
		S_Aft3 = rData.S_Aft;
		S_Afv2 = rData.S_Afv;
		S_Afv3 = rData.S_Afv;
		bLambda = FALSE;
		dLambda = 1.0;
		strMatlName.Empty();
		dGamma_m = 0.0;
		Matl_KSCE05.Initialize();
		bTransfer = FALSE;
		dTransferFci = 0.0;
	}
	void Convert422(T_MATL_DESIGN_422& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = S_Fy2;
		S_Fy4 = S_Fy3;
		S_Fy5 = S_Fy4; // add by Seungjun (`06.05.29) at 710
		S_Fy6 = S_Fy5; // add by Seungjun (`06.05.29) at 710
		S_Aft = rData.S_Aft;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		C_fc = rData.C_fc;
		C_fci = 0.;
		S_Aft2 = rData.S_Aft;
		S_Aft3 = rData.S_Aft;
		S_Afv2 = rData.S_Afv;
		S_Afv3 = rData.S_Afv;
		bLambda = FALSE;
		dLambda = 1.0;
		strMatlName.Empty();
		dGamma_m = 0.0;
		Matl_KSCE05.Initialize();
		bTransfer = FALSE;
		dTransferFci = 0.0;
	}
}; // end

struct T_MATL_ALL_550
{
	CString CodeName;       // 16자 이내
	CString CodeMatlName;       // 16자 이내
	T_MATL_ANALYSIS_550 Analysis;
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		Analysis.Initialize();
	}
};
struct T_MATL_ALL_580
{
	CString CodeName;       // 16자 이내
	CString CodeMatlName;   // 16자 이내
	T_MATL_ANALYSIS_580 Analysis;
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		Analysis.Initialize();
	}
};
struct T_MATL_ALL_730
{
	CString CodeName;       // 16자 이내
	CString CodeMatlName;   // 16자 이내
	T_MATL_ANALYSIS_845 Analysis;
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		Analysis.Initialize();
	}
};
struct T_MATL_ALL_845
{
	CString CodeName;       // 16자 이내
	CString CodeMatlName;   // 16자 이내
	T_MATL_ANALYSIS_845 Analysis;
	CString SubCodeName;    // 16자 이내
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		SubCodeName.Empty();
		Analysis.Initialize();
	}
};
/**
@brief T_MATL_D에서 사용되는 구조체로 재질의 DB명, 국가 코드 등의 정보를 갖고 있다.
*/
struct T_MATL_ALL
{
	CString CodeName;    /**< 재질의 국가 코드 */   // 16자 이내
	CString CodeMatlName; /**< 재질의 DB명 */  // 16자 이내
	T_MATL_ANALYSIS Analysis;
	CString SubCodeName; /**< 국가 코드를 더 세분화 시켜 구분하는데 사용된다 */   // 16자 이내
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		SubCodeName.Empty();
		Analysis.Initialize();
	}
	void Convert550(T_MATL_ALL_550 rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		Analysis.Convert550(rData.Analysis);
		SubCodeName.Empty();
	}
	void Convert580(T_MATL_ALL_580 rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		Analysis.Convert580(rData.Analysis);
		SubCodeName.Empty();
	}
	void Convert730(T_MATL_ALL_730 rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		Analysis.Convert845(rData.Analysis);
		SubCodeName.Empty();
	}
	void Convert845(T_MATL_ALL_845 rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		Analysis.Convert845(rData.Analysis);
		SubCodeName = rData.SubCodeName;
	}
};
struct T_MATL_ALL_CH_550
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	T_MATL_ANALYSIS_550 Analysis;
};
struct T_MATL_ALL_CH_580
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	T_MATL_ANALYSIS_580 Analysis;
};
struct T_MATL_ALL_CH_730
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	T_MATL_ANALYSIS_845 Analysis;
};
struct T_MATL_ALL_CH_845
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	char SubCodeName[16];       // 16자 이내
	T_MATL_ANALYSIS_845 Analysis;
};
struct T_MATL_ALL_CH
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	char SubCodeName[16];       // 16자 이내
	T_MATL_ANALYSIS Analysis;
	void ConvertToString(T_MATL_ALL& rData)
	{
		ConvertCharStr(CodeName, rData.CodeName, sizeof(CodeName));
		ConvertCharStr(CodeMatlName, rData.CodeMatlName, sizeof(CodeMatlName));
		ConvertCharStr(SubCodeName, rData.SubCodeName, sizeof(SubCodeName));
		rData.Analysis = Analysis;
	}
	void ConvertToChar(T_MATL_ALL& rData)
	{
		ConvertStrChar(rData.CodeName, CodeName, sizeof(CodeName));
		ConvertStrChar(rData.CodeMatlName, CodeMatlName, sizeof(CodeMatlName));
		ConvertStrChar(rData.SubCodeName, SubCodeName, sizeof(SubCodeName));
		Analysis = rData.Analysis;
	}
	void Convert550(T_MATL_ALL_CH_550& rData)
	{
		memcpy(CodeName, rData.CodeName, __min(sizeof(CodeName), sizeof(rData.CodeName)));
		memcpy(CodeMatlName, rData.CodeMatlName, __min(sizeof(CodeMatlName), sizeof(rData.CodeMatlName)));
		memset(SubCodeName, 0, sizeof(SubCodeName));
		Analysis.Convert550(rData.Analysis);
	}
	void Convert580(T_MATL_ALL_CH_580& rData)
	{
		memcpy(CodeName, rData.CodeName, __min(sizeof(CodeName), sizeof(rData.CodeName)));
		memcpy(CodeMatlName, rData.CodeMatlName, __min(sizeof(CodeMatlName), sizeof(rData.CodeMatlName)));
		memset(SubCodeName, 0, sizeof(SubCodeName));
		Analysis.Convert580(rData.Analysis);
	}
	void Convert730(T_MATL_ALL_CH_730& rData)
	{
		memcpy(CodeName, rData.CodeName, __min(sizeof(CodeName), sizeof(rData.CodeName)));
		memcpy(CodeMatlName, rData.CodeMatlName, __min(sizeof(CodeMatlName), sizeof(rData.CodeMatlName)));
		memset(SubCodeName, 0, sizeof(SubCodeName));
		Analysis.Convert845(rData.Analysis);
	}
	void Convert845(T_MATL_ALL_CH_845& rData)
	{
		memcpy(CodeName, rData.CodeName, __min(sizeof(CodeName), sizeof(rData.CodeName)));
		memcpy(CodeMatlName, rData.CodeMatlName, __min(sizeof(CodeMatlName), sizeof(rData.CodeMatlName)));
		memcpy(SubCodeName, rData.SubCodeName, __min(sizeof(SubCodeName), sizeof(rData.SubCodeName)));
		Analysis.Convert845(rData.Analysis);
	}
};

struct T_MATL_DESIGN_CH_835
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	char strMatlName[16]; // Composite Hybrid Factor를 사용할때 사용 

	void ConvertToString(T_MATL_DESIGN_835& rData)
	{
		rData.S_Fu =     S_Fu;
		rData.S_Fy1 =    S_Fy1;
		rData.S_Fy2 =    S_Fy2;
		rData.S_Fy3 =    S_Fy3;
		rData.S_Fy4 =    S_Fy4;
		rData.S_Fy5 =    S_Fy5;
		rData.S_Fy6 =    S_Fy6;
		rData.S_Aft =    S_Aft;
		rData.S_Aft2 =   S_Aft2;
		rData.S_Aft3 =   S_Aft3;
		rData.S_Fy   =   S_Fy; 
		rData.S_Afv =    S_Afv;
		rData.S_Afv2 =   S_Afv2;
		rData.S_Afv3 =   S_Afv3;
		rData.C_fc =     C_fc;
		rData.C_fci =    C_fci;
		rData.bLambda =  bLambda;
		rData.dLambda =  dLambda;
		ConvertCharStr(strMatlName, rData.strMatlName, sizeof(strMatlName));
	}
	void ConvertToChar(T_MATL_DESIGN_835& rData)
	{
		S_Fu    = rData.S_Fu;    
		S_Fy1   = rData.S_Fy1;   
		S_Fy2   = rData.S_Fy2;   
		S_Fy3   = rData.S_Fy3;   
		S_Fy4   = rData.S_Fy4;   
		S_Fy5   = rData.S_Fy5;   
		S_Fy6   = rData.S_Fy6;   
		S_Aft   = rData.S_Aft;   
		S_Aft2  = rData.S_Aft2;  
		S_Aft3  = rData.S_Aft3;  
		S_Fy    = rData.S_Fy;    
		S_Afv   = rData.S_Afv;   
		S_Afv2  = rData.S_Afv2;  
		S_Afv3  = rData.S_Afv3;  
		C_fc    = rData.C_fc;    
		C_fci   = rData.C_fci;   
		bLambda = rData.bLambda; 
		dLambda = rData.dLambda; 
		ConvertStrChar(rData.strMatlName, strMatlName, sizeof(strMatlName));      
	}
};

struct T_MATL_DESIGN_CH_896
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	char strMatlName[16]; // Composite Hybrid Factor를 사용할때 사용 
	double dGamma_m;

	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0 <- 835 버전에서 DB 추가할때 버그가 있었음 

	void ConvertToString(T_MATL_DESIGN_896& rData)
	{
		rData.S_Fu =     S_Fu;
		rData.S_Fy1 =    S_Fy1;
		rData.S_Fy2 =    S_Fy2;
		rData.S_Fy3 =    S_Fy3;
		rData.S_Fy4 =    S_Fy4;
		rData.S_Fy5 =    S_Fy5;
		rData.S_Fy6 =    S_Fy6;
		rData.S_Aft =    S_Aft;
		rData.S_Aft2 =   S_Aft2;
		rData.S_Aft3 =   S_Aft3;
		rData.S_Fy   =   S_Fy; 
		rData.S_Afv =    S_Afv;
		rData.S_Afv2 =   S_Afv2;
		rData.S_Afv3 =   S_Afv3;
		rData.C_fc =     C_fc;
		rData.C_fci =    C_fci;
		rData.bLambda =  bLambda;
		rData.dLambda =  dLambda;
		ConvertCharStr(strMatlName, rData.strMatlName, sizeof(strMatlName));
		rData.dGamma_m = dGamma_m;

		rData.Matl_KSCE05 = Matl_KSCE05;
	}
	void ConvertToChar(T_MATL_DESIGN_896& rData)
	{
		S_Fu    = rData.S_Fu;    
		S_Fy1   = rData.S_Fy1;   
		S_Fy2   = rData.S_Fy2;   
		S_Fy3   = rData.S_Fy3;   
		S_Fy4   = rData.S_Fy4;   
		S_Fy5   = rData.S_Fy5;   
		S_Fy6   = rData.S_Fy6;   
		S_Aft   = rData.S_Aft;   
		S_Aft2  = rData.S_Aft2;  
		S_Aft3  = rData.S_Aft3;  
		S_Fy    = rData.S_Fy;    
		S_Afv   = rData.S_Afv;   
		S_Afv2  = rData.S_Afv2;  
		S_Afv3  = rData.S_Afv3;  
		C_fc    = rData.C_fc;    
		C_fci   = rData.C_fci;   
		bLambda = rData.bLambda; 
		dLambda = rData.dLambda; 
		ConvertStrChar(rData.strMatlName, strMatlName, sizeof(strMatlName));      
		dGamma_m = rData.dGamma_m;

		Matl_KSCE05 = rData.Matl_KSCE05;
	}
};

struct T_MATL_DESIGN_CH_900_TEMP
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	char strMatlName[16]; // Composite Hybrid Factor를 사용할때 사용 
	double dGamma_m;

	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0 <- 835 버전에서 DB 추가할때 버그가 있었음 
	BOOL   bTransfer;   // 9.0.0  호주PSC

	void ConvertToString(T_MATL_DESIGN_900_TEMP& rData)
	{
		rData.S_Fu =     S_Fu;
		rData.S_Fy1 =    S_Fy1;
		rData.S_Fy2 =    S_Fy2;
		rData.S_Fy3 =    S_Fy3;
		rData.S_Fy4 =    S_Fy4;
		rData.S_Fy5 =    S_Fy5;
		rData.S_Fy6 =    S_Fy6;
		rData.S_Aft =    S_Aft;
		rData.S_Aft2 =   S_Aft2;
		rData.S_Aft3 =   S_Aft3;
		rData.S_Fy   =   S_Fy; 
		rData.S_Afv =    S_Afv;
		rData.S_Afv2 =   S_Afv2;
		rData.S_Afv3 =   S_Afv3;
		rData.C_fc =     C_fc;
		rData.C_fci =    C_fci;
		rData.bLambda =  bLambda;
		rData.dLambda =  dLambda;
		ConvertCharStr(strMatlName, rData.strMatlName, sizeof(strMatlName));
		rData.dGamma_m = dGamma_m;

		rData.Matl_KSCE05 = Matl_KSCE05;
		rData.bTransfer = bTransfer;
	}
	void ConvertToChar(T_MATL_DESIGN_900_TEMP& rData)
	{
		S_Fu    = rData.S_Fu;    
		S_Fy1   = rData.S_Fy1;   
		S_Fy2   = rData.S_Fy2;   
		S_Fy3   = rData.S_Fy3;   
		S_Fy4   = rData.S_Fy4;   
		S_Fy5   = rData.S_Fy5;   
		S_Fy6   = rData.S_Fy6;   
		S_Aft   = rData.S_Aft;   
		S_Aft2  = rData.S_Aft2;  
		S_Aft3  = rData.S_Aft3;  
		S_Fy    = rData.S_Fy;    
		S_Afv   = rData.S_Afv;   
		S_Afv2  = rData.S_Afv2;  
		S_Afv3  = rData.S_Afv3;  
		C_fc    = rData.C_fc;    
		C_fci   = rData.C_fci;   
		bLambda = rData.bLambda; 
		dLambda = rData.dLambda; 
		ConvertStrChar(rData.strMatlName, strMatlName, sizeof(strMatlName));      
		dGamma_m = rData.dGamma_m;

		Matl_KSCE05 = rData.Matl_KSCE05;
		bTransfer = rData.bTransfer;
	}
};

struct T_MATL_DESIGN_CH_925
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	char strMatlName[16]; // Composite Hybrid Factor를 사용할때 사용 
	double dGamma_m;

	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0 <- 835 버전에서 DB 추가할때 버그가 있었음 

	BOOL   bTransfer;   // 9.0.0  호주PSC
	double dTransferFci;   // 9.0.0  호주PSC

	void ConvertToString(T_MATL_DESIGN_925& rData)
	{
		rData.S_Fu = S_Fu;
		rData.S_Fy1 = S_Fy1;
		rData.S_Fy2 = S_Fy2;
		rData.S_Fy3 = S_Fy3;
		rData.S_Fy4 = S_Fy4;
		rData.S_Fy5 = S_Fy5;
		rData.S_Fy6 = S_Fy6;
		rData.S_Aft = S_Aft;
		rData.S_Aft2 = S_Aft2;
		rData.S_Aft3 = S_Aft3;
		rData.S_Fy = S_Fy;
		rData.S_Afv = S_Afv;
		rData.S_Afv2 = S_Afv2;
		rData.S_Afv3 = S_Afv3;
		rData.C_fc = C_fc;
		rData.C_fci = C_fci;
		rData.bLambda = bLambda;
		rData.dLambda = dLambda;
		ConvertCharStr(strMatlName, rData.strMatlName, sizeof(strMatlName));
		rData.dGamma_m = dGamma_m;

		rData.Matl_KSCE05 = Matl_KSCE05;

		rData.bTransfer = bTransfer;
		rData.dTransferFci = dTransferFci;
	}
	void ConvertToChar(T_MATL_DESIGN_925& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5;
		S_Fy6 = rData.S_Fy6;
		S_Aft = rData.S_Aft;
		S_Aft2 = rData.S_Aft2;
		S_Aft3 = rData.S_Aft3;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		S_Afv2 = rData.S_Afv2;
		S_Afv3 = rData.S_Afv3;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		bLambda = rData.bLambda;
		dLambda = rData.dLambda;
		ConvertStrChar(rData.strMatlName, strMatlName, sizeof(strMatlName));
		dGamma_m = rData.dGamma_m;

		Matl_KSCE05 = rData.Matl_KSCE05;

		bTransfer = rData.bTransfer;
		dTransferFci = rData.dTransferFci;
	}
};

struct T_MATL_DESIGN_CH_950
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	char strMatlName[16]; // Composite Hybrid Factor를 사용할때 사용 
	double dGamma_m;

	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0 <- 835 버전에서 DB 추가할때 버그가 있었음 

	BOOL   bTransfer;   // 9.0.0  호주PSC
	double dTransferFci;   // 9.0.0  호주PSC

	BOOL bSeamlessP;

	void ConvertToString(T_MATL_DESIGN_950& rData)
	{
		rData.S_Fu = S_Fu;
		rData.S_Fy1 = S_Fy1;
		rData.S_Fy2 = S_Fy2;
		rData.S_Fy3 = S_Fy3;
		rData.S_Fy4 = S_Fy4;
		rData.S_Fy5 = S_Fy5;
		rData.S_Fy6 = S_Fy6;
		rData.S_Aft = S_Aft;
		rData.S_Aft2 = S_Aft2;
		rData.S_Aft3 = S_Aft3;
		rData.S_Fy = S_Fy;
		rData.S_Afv = S_Afv;
		rData.S_Afv2 = S_Afv2;
		rData.S_Afv3 = S_Afv3;
		rData.C_fc = C_fc;
		rData.C_fci = C_fci;
		rData.bLambda = bLambda;
		rData.dLambda = dLambda;
		ConvertCharStr(strMatlName, rData.strMatlName, sizeof(strMatlName));
		rData.dGamma_m = dGamma_m;

		rData.Matl_KSCE05 = Matl_KSCE05;

		rData.bTransfer = bTransfer;
		rData.dTransferFci = dTransferFci;
		rData.bSeamlessP = bSeamlessP;
	}
	void ConvertToChar(T_MATL_DESIGN_950& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5;
		S_Fy6 = rData.S_Fy6;
		S_Aft = rData.S_Aft;
		S_Aft2 = rData.S_Aft2;
		S_Aft3 = rData.S_Aft3;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		S_Afv2 = rData.S_Afv2;
		S_Afv3 = rData.S_Afv3;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		bLambda = rData.bLambda;
		dLambda = rData.dLambda;
		ConvertStrChar(rData.strMatlName, strMatlName, sizeof(strMatlName));
		dGamma_m = rData.dGamma_m;

		Matl_KSCE05 = rData.Matl_KSCE05;

		bTransfer = rData.bTransfer;
		dTransferFci = rData.dTransferFci;
		bSeamlessP = rData.bSeamlessP;
	}
};

struct T_MATL_DESIGN_CH_955
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy7;
	double S_Fy8;
	double S_Fy9;
	double S_Fy10;
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	char strMatlName[16]; // Composite Hybrid Factor를 사용할때 사용 
	double dGamma_m;

	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0 <- 835 버전에서 DB 추가할때 버그가 있었음 

	BOOL   bTransfer;   // 9.0.0  호주PSC
	double dTransferFci;   // 9.0.0  호주PSC

	BOOL bSeamlessP;

	void ConvertToString(T_MATL_DESIGN_955& rData)
	{
		rData.S_Fu = S_Fu;
		rData.S_Fy1 = S_Fy1;
		rData.S_Fy2 = S_Fy2;
		rData.S_Fy3 = S_Fy3;
		rData.S_Fy4 = S_Fy4;
		rData.S_Fy5 = S_Fy5;
		rData.S_Fy6 = S_Fy6;
		rData.S_Fy7 = S_Fy7;
		rData.S_Fy8 = S_Fy8;
		rData.S_Fy9 = S_Fy9;
		rData.S_Fy10 = S_Fy10;
		rData.S_Aft = S_Aft;
		rData.S_Aft2 = S_Aft2;
		rData.S_Aft3 = S_Aft3;
		rData.S_Fy = S_Fy;
		rData.S_Afv = S_Afv;
		rData.S_Afv2 = S_Afv2;
		rData.S_Afv3 = S_Afv3;
		rData.C_fc = C_fc;
		rData.C_fci = C_fci;
		rData.bLambda = bLambda;
		rData.dLambda = dLambda;
		ConvertCharStr(strMatlName, rData.strMatlName, sizeof(strMatlName));
		rData.dGamma_m = dGamma_m;

		rData.Matl_KSCE05 = Matl_KSCE05;

		rData.bTransfer = bTransfer;
		rData.dTransferFci = dTransferFci;
		rData.bSeamlessP = bSeamlessP;
	}
	void ConvertToChar(T_MATL_DESIGN_955& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5;
		S_Fy6 = rData.S_Fy6;
		S_Fy7 = rData.S_Fy7;
		S_Fy8 = rData.S_Fy8;
		S_Fy9 = rData.S_Fy9;
		S_Fy10 = rData.S_Fy10;
		S_Aft = rData.S_Aft;
		S_Aft2 = rData.S_Aft2;
		S_Aft3 = rData.S_Aft3;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		S_Afv2 = rData.S_Afv2;
		S_Afv3 = rData.S_Afv3;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		bLambda = rData.bLambda;
		dLambda = rData.dLambda;
		ConvertStrChar(rData.strMatlName, strMatlName, sizeof(strMatlName));
		dGamma_m = rData.dGamma_m;

		Matl_KSCE05 = rData.Matl_KSCE05;

		bTransfer = rData.bTransfer;
		dTransferFci = rData.dTransferFci;

		bSeamlessP = rData.bSeamlessP;
	}
};

struct T_MATL_DESIGN_CH_960
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy7;
	double S_Fy8;
	double S_Fy9;
	double S_Fy10;
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	char strMatlName[16]; // Composite Hybrid Factor를 사용할때 사용 
	double dGamma_m;

	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0 <- 835 버전에서 DB 추가할때 버그가 있었음 

	BOOL   bTransfer;   // 9.0.0  호주PSC
	double dTransferFci;   // 9.0.0  호주PSC

	BOOL bSeamlessP;
	double dEta;
	void ConvertToString(T_MATL_DESIGN_960& rData)
	{
		rData.S_Fu = S_Fu;
		rData.S_Fy1 = S_Fy1;
		rData.S_Fy2 = S_Fy2;
		rData.S_Fy3 = S_Fy3;
		rData.S_Fy4 = S_Fy4;
		rData.S_Fy5 = S_Fy5;
		rData.S_Fy6 = S_Fy6;
		rData.S_Fy7 = S_Fy7;
		rData.S_Fy8 = S_Fy8;
		rData.S_Fy9 = S_Fy9;
		rData.S_Fy10 = S_Fy10;
		rData.S_Aft = S_Aft;
		rData.S_Aft2 = S_Aft2;
		rData.S_Aft3 = S_Aft3;
		rData.S_Fy = S_Fy;
		rData.S_Afv = S_Afv;
		rData.S_Afv2 = S_Afv2;
		rData.S_Afv3 = S_Afv3;
		rData.C_fc = C_fc;
		rData.C_fci = C_fci;
		rData.bLambda = bLambda;
		rData.dLambda = dLambda;
		ConvertCharStr(strMatlName, rData.strMatlName, sizeof(strMatlName));
		rData.dGamma_m = dGamma_m;

		rData.Matl_KSCE05 = Matl_KSCE05;

		rData.bTransfer = bTransfer;
		rData.dTransferFci = dTransferFci;
		rData.bSeamlessP = bSeamlessP;
		rData.dEta = dEta;
	}
	void ConvertToChar(T_MATL_DESIGN_960& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5;
		S_Fy6 = rData.S_Fy6;
		S_Fy7 = rData.S_Fy7;
		S_Fy8 = rData.S_Fy8;
		S_Fy9 = rData.S_Fy9;
		S_Fy10 = rData.S_Fy10;
		S_Aft = rData.S_Aft;
		S_Aft2 = rData.S_Aft2;
		S_Aft3 = rData.S_Aft3;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		S_Afv2 = rData.S_Afv2;
		S_Afv3 = rData.S_Afv3;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		bLambda = rData.bLambda;
		dLambda = rData.dLambda;
		ConvertStrChar(rData.strMatlName, strMatlName, sizeof(strMatlName));
		dGamma_m = rData.dGamma_m;

		Matl_KSCE05 = rData.Matl_KSCE05;

		bTransfer = rData.bTransfer;
		dTransferFci = rData.dTransferFci;

		bSeamlessP = rData.bSeamlessP;
		rData.dEta = dEta;
	}
};

struct T_MATL_DESIGN_CH
{
	double S_Fu;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fu)
	double S_Fy1;       // steel 인 경우 사용
	double S_Fy2;       // steel 인 경우 사용
	double S_Fy3;       // steel 인 경우 사용
	double S_Fy4;       // steel 인 경우 사용
	double S_Fy5;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy6;       // steel 인 경우 사용 // add by Seungjun (`06.05.29) at 710
	double S_Fy7;
	double S_Fy8;
	double S_Fy9;
	double S_Fy10;
	double S_Aft;       // steel 인 경우 사용
	double S_Aft2;      // steel 인 경우 사용 // 7.3.0 
	double S_Aft3;      // steel 인 경우 사용 // 7.3.0
	double S_Fy;        // steel 인 경우 사용(TOWER에서 Data2인 경우 Bolt의 Fy)
	double S_Afv;       // steel 인 경우 사용
	double S_Afv2;      // steel 인 경우 사용 // 7.3.0
	double S_Afv3;      // steel 인 경우 사용 // 7.3.0
	double C_fc;        // concrete 인 경우 사용
	double C_fci;       // concrete 인 경우 사용  : 5.6.0에서 추가
	BOOL   bLambda;     // Light Weight Concrete Factor // 8.1.1
	double dLambda;     // Light Weight Concrete Factor // 8.1.1
	char strMatlName[16]; // Composite Hybrid Factor를 사용할때 사용 
	double dGamma_m;

	T_MATL_DESIGN_KSCE05 Matl_KSCE05; // 7.3.0 <- 835 버전에서 DB 추가할때 버그가 있었음 

	BOOL   bTransfer;   // 9.0.0  호주PSC
	double dTransferFci;   // 9.0.0  호주PSC

	BOOL bSeamlessP; 
	double dEta;
	BOOL bChkEta;

	void ConvertToString(T_MATL_DESIGN& rData)
	{
		rData.S_Fu =     S_Fu;
		rData.S_Fy1 =    S_Fy1;
		rData.S_Fy2 =    S_Fy2;
		rData.S_Fy3 =    S_Fy3;
		rData.S_Fy4 =    S_Fy4;
		rData.S_Fy5 =    S_Fy5;
		rData.S_Fy6 =    S_Fy6;
		rData.S_Fy7 =    S_Fy7;
		rData.S_Fy8 =    S_Fy8;
		rData.S_Fy9 =    S_Fy9;
		rData.S_Fy10=    S_Fy10;
		rData.S_Aft =    S_Aft;
		rData.S_Aft2 =   S_Aft2;
		rData.S_Aft3 =   S_Aft3;
		rData.S_Fy   =   S_Fy; 
		rData.S_Afv =    S_Afv;
		rData.S_Afv2 =   S_Afv2;
		rData.S_Afv3 =   S_Afv3;
		rData.C_fc =     C_fc;
		rData.C_fci =    C_fci;
		rData.bLambda =  bLambda;
		rData.dLambda =  dLambda;
		ConvertCharStr(strMatlName, rData.strMatlName, sizeof(strMatlName));
		rData.dGamma_m = dGamma_m;

		rData.Matl_KSCE05 = Matl_KSCE05;

		rData.bTransfer = bTransfer;
		rData.dTransferFci = dTransferFci;
		rData.bSeamlessP = bSeamlessP;
		rData.dEta = dEta;
		rData.bChkEta = bChkEta;
	}
	void ConvertToChar(T_MATL_DESIGN& rData)
	{
		S_Fu    = rData.S_Fu;    
		S_Fy1   = rData.S_Fy1;   
		S_Fy2   = rData.S_Fy2;   
		S_Fy3   = rData.S_Fy3;   
		S_Fy4   = rData.S_Fy4;   
		S_Fy5   = rData.S_Fy5;   
		S_Fy6   = rData.S_Fy6;   
		S_Fy7   = rData.S_Fy7;
		S_Fy8   = rData.S_Fy8;
		S_Fy9   = rData.S_Fy9;
		S_Fy10  = rData.S_Fy10;
		S_Aft   = rData.S_Aft;   
		S_Aft2  = rData.S_Aft2;  
		S_Aft3  = rData.S_Aft3;  
		S_Fy    = rData.S_Fy;    
		S_Afv   = rData.S_Afv;   
		S_Afv2  = rData.S_Afv2;  
		S_Afv3  = rData.S_Afv3;  
		C_fc    = rData.C_fc;    
		C_fci   = rData.C_fci;   
		bLambda = rData.bLambda; 
		dLambda = rData.dLambda; 
		ConvertStrChar(rData.strMatlName, strMatlName, sizeof(strMatlName));      
		dGamma_m = rData.dGamma_m;

		Matl_KSCE05 = rData.Matl_KSCE05;

		bTransfer = rData.bTransfer;
		dTransferFci = rData.dTransferFci;

		bSeamlessP = rData.bSeamlessP;
		dEta = rData.dEta;
		bChkEta = rData.bChkEta;
	}
	void Convert960(T_MATL_DESIGN_CH_960& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5;
		S_Fy6 = rData.S_Fy6;
		S_Fy7 = rData.S_Fy7;
		S_Fy8 = rData.S_Fy8;
		S_Fy9 = rData.S_Fy9;
		S_Fy10 = rData.S_Fy10;
		S_Aft = rData.S_Aft;
		S_Aft2 = rData.S_Aft2;
		S_Aft3 = rData.S_Aft3;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		S_Afv2 = rData.S_Afv2;
		S_Afv3 = rData.S_Afv3;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		bLambda = rData.bLambda;
		dLambda = rData.dLambda;
		memcpy(strMatlName, rData.strMatlName, sizeof(strMatlName));
		dGamma_m = rData.dGamma_m;

		Matl_KSCE05 = rData.Matl_KSCE05;

		bTransfer = rData.bTransfer;
		dTransferFci = rData.dTransferFci;

		bSeamlessP = rData.bSeamlessP;
		dEta = rData.dEta;
		bChkEta = TRUE;
	}
	void Convert955(T_MATL_DESIGN_CH_955& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5;
		S_Fy6 = rData.S_Fy6;
		S_Fy7 = rData.S_Fy7;
		S_Fy8 = rData.S_Fy8;
		S_Fy9 = rData.S_Fy9;
		S_Fy10 = rData.S_Fy10;
		S_Aft = rData.S_Aft;
		S_Aft2 = rData.S_Aft2;
		S_Aft3 = rData.S_Aft3;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		S_Afv2 = rData.S_Afv2;
		S_Afv3 = rData.S_Afv3;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		bLambda = rData.bLambda;
		dLambda = rData.dLambda;
		memcpy(strMatlName, rData.strMatlName, sizeof(strMatlName));
		dGamma_m = rData.dGamma_m;

		Matl_KSCE05 = rData.Matl_KSCE05;

		bTransfer = rData.bTransfer;
		dTransferFci = rData.dTransferFci;

		bSeamlessP = rData.bSeamlessP;
		dEta = 1.0;
		bChkEta = TRUE;
	}
	void Convert950(T_MATL_DESIGN_CH_950& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5;
		S_Fy6 = rData.S_Fy6;
		S_Aft = rData.S_Aft;
		S_Aft2 = rData.S_Aft2;
		S_Aft3 = rData.S_Aft3;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		S_Afv2 = rData.S_Afv2;
		S_Afv3 = rData.S_Afv3;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		bLambda = rData.bLambda;
		dLambda = rData.dLambda;
		memcpy(strMatlName, rData.strMatlName, sizeof(strMatlName));
		dGamma_m = rData.dGamma_m;

		Matl_KSCE05 = rData.Matl_KSCE05;

		bTransfer = rData.bTransfer;
		dTransferFci = rData.dTransferFci;
		bSeamlessP = rData.bSeamlessP;
	}
	void Convert925(T_MATL_DESIGN_CH_925& rData)
	{
		S_Fu = rData.S_Fu;
		S_Fy1 = rData.S_Fy1;
		S_Fy2 = rData.S_Fy2;
		S_Fy3 = rData.S_Fy3;
		S_Fy4 = rData.S_Fy4;
		S_Fy5 = rData.S_Fy5;
		S_Fy6 = rData.S_Fy6;
		S_Aft = rData.S_Aft;
		S_Aft2 = rData.S_Aft2;
		S_Aft3 = rData.S_Aft3;
		S_Fy = rData.S_Fy;
		S_Afv = rData.S_Afv;
		S_Afv2 = rData.S_Afv2;
		S_Afv3 = rData.S_Afv3;
		C_fc = rData.C_fc;
		C_fci = rData.C_fci;
		bLambda = rData.bLambda;
		dLambda = rData.dLambda;
		memcpy(strMatlName, rData.strMatlName, sizeof(strMatlName));
		dGamma_m = rData.dGamma_m;

		Matl_KSCE05 = rData.Matl_KSCE05;

		bTransfer = rData.bTransfer;
		dTransferFci = rData.dTransferFci;
		bSeamlessP = FALSE;
	}
	void Convert896(T_MATL_DESIGN_CH_896& rData)
	{
		S_Fu    = rData.S_Fu;    
		S_Fy1   = rData.S_Fy1;   
		S_Fy2   = rData.S_Fy2;   
		S_Fy3   = rData.S_Fy3;   
		S_Fy4   = rData.S_Fy4;   
		S_Fy5   = rData.S_Fy5;   
		S_Fy6   = rData.S_Fy6;   
		S_Aft   = rData.S_Aft;   
		S_Aft2  = rData.S_Aft2;  
		S_Aft3  = rData.S_Aft3;  
		S_Fy    = rData.S_Fy;    
		S_Afv   = rData.S_Afv;   
		S_Afv2  = rData.S_Afv2;  
		S_Afv3  = rData.S_Afv3;  
		C_fc    = rData.C_fc;    
		C_fci   = rData.C_fci;   
		bLambda = rData.bLambda; 
		dLambda = rData.dLambda; 
		memcpy(strMatlName, rData.strMatlName, sizeof(strMatlName));      
		dGamma_m = rData.dGamma_m;

		Matl_KSCE05 = rData.Matl_KSCE05;

		bTransfer = FALSE;
		dTransferFci = 0.0;
		bSeamlessP = FALSE;
	}
	void Convert835(T_MATL_DESIGN_CH_835& rData)
	{
		S_Fu    = rData.S_Fu;    
		S_Fy1   = rData.S_Fy1;   
		S_Fy2   = rData.S_Fy2;   
		S_Fy3   = rData.S_Fy3;   
		S_Fy4   = rData.S_Fy4;   
		S_Fy5   = rData.S_Fy5;   
		S_Fy6   = rData.S_Fy6;   
		S_Aft   = rData.S_Aft;   
		S_Aft2  = rData.S_Aft2;  
		S_Aft3  = rData.S_Aft3;  
		S_Fy    = rData.S_Fy;    
		S_Afv   = rData.S_Afv;   
		S_Afv2  = rData.S_Afv2;  
		S_Afv3  = rData.S_Afv3;  
		C_fc    = rData.C_fc;    
		C_fci   = rData.C_fci;   
		bLambda = rData.bLambda; 
		dLambda = rData.dLambda; 
		memcpy(strMatlName, rData.strMatlName, sizeof(strMatlName));
		dGamma_m = 0.0;
		
		Matl_KSCE05.Initialize();	// 835 버전에서 DB 추가할때 버그가 있었음 

		bTransfer = FALSE;
		dTransferFci = 0.0;
		bSeamlessP = FALSE;
	}
};

struct T_MATL_D_550
{
	CString Type;       // 4자 이내
	CString Name;       // 16자 이내
	T_MATL_ALL_550 Data1;
	T_MATL_ALL_550 Data2;
	BOOL bChangeSteel;  // SRC인 경우 
	double dSpecificHeat;      
	double dHeatConduction;
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		bChangeSteel=TRUE;
		dSpecificHeat = 0.0;      
		dHeatConduction = 0.0;
	}
};
struct T_MATL_D_580
{
	CString Type;       // 4자 이내
	CString Name;       // 16자 이내
	T_MATL_ALL_580 Data1;
	T_MATL_ALL_580 Data2;
	BOOL bChangeSteel;  // SRC인 경우 
	double dSpecificHeat;      
	double dHeatConduction;
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		bChangeSteel=TRUE;
		dSpecificHeat = 0.0;      
		dHeatConduction = 0.0;
	}
};
struct T_MATL_D_630
{
	CString Type;       // 4자 이내
	CString Name;       // 16자 이내
	T_MATL_ALL_730 Data1;
	T_MATL_ALL_730 Data2;
	BOOL bChangeSteel;  // SRC인 경우 
	double dSpecificHeat;      
	double dHeatConduction;
	unsigned int KeyEpmt;     // 600 추가
	int    nThermalUnit;      // 0:Celsius, 1:Fahrenheit (600 추가)
	BOOL   bUseMassDensity;   // 600에서 추가
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		bChangeSteel=TRUE;
		dSpecificHeat = 0.0;      
		dHeatConduction = 0.0;
		KeyEpmt = 0;
		nThermalUnit = 0;
		bUseMassDensity = FALSE;
	}
};
struct T_MATL_D_730
{
	CString Type;       // 4자 이내
	CString Name;       // 16자 이내
	T_MATL_ALL_730 Data1;
	T_MATL_ALL_730 Data2;
	BOOL bChangeSteel;  // SRC인 경우 
	double dSpecificHeat;      
	double dHeatConduction;
	unsigned int KeyEpmt;     // 600 추가
	int    nThermalUnit;      // 0:Celsius, 1:Fahrenheit (600 추가)
	BOOL   bUseMassDensity;   // 600에서 추가
	double dDampingRatioD2E;  // 6.4.0에서 추가 : D2E => 사용하지 않음
	double dAlphaD2E;         // 6.4.0에서 추가 : D2E => 사용하지 않음 
	double dBetaD2E;          // 6.4.0에서 추가 : D2E => 사용하지 않음 
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		bChangeSteel=TRUE;
		dSpecificHeat = 0.0;      
		dHeatConduction = 0.0;
		KeyEpmt = 0;
		nThermalUnit = 0;
		bUseMassDensity = FALSE;
		dDampingRatioD2E = 0.05;
		dAlphaD2E = 0.;
		dBetaD2E = 0.;
	}
};
struct T_MATL_D_845
{
	CString Type;       // 4자 이내 
	//"A" 일 경우 알루미늄 재질.
	//기본적으로 유저타입과 거의 동일한 동작을 함.
	//Data1 안에 SubCodeName을 기존의 사용용도가 아닌 다른 용도로 사용함으로 주의 할 것. (sjhuh-2013-10-01)

	CString Name;       // 16자 이내
	T_MATL_ALL_845 Data1;   // steel, concrete, src인 경우 steel
	T_MATL_ALL_845 Data2;   // src인 경우 concrete
	BOOL bChangeSteel;  // SRC인 경우 
	double dSpecificHeat;      
	double dHeatConduction;
	unsigned int KeyEpmt;     // 600 추가
	int    nThermalUnit;      // 0:Celsius, 1:Fahrenheit (600 추가)
	BOOL   bUseMassDensity;   // 600에서 추가
	//  double dDampingRatioD2E;  // 6.4.0에서 추가 : D2E => 사용하지 않음
	double dDampingRatio;       // 790    사용하지 않는 dDampingRatioD2E 대신 사용
	double dAlphaD2E;         // 6.4.0에서 추가 : D2E => 사용하지 않음 
	double dBetaD2E;          // 6.4.0에서 추가 : D2E => 사용하지 않음  
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		bChangeSteel=TRUE;
		dSpecificHeat = 0.0;      
		dHeatConduction = 0.0;
		KeyEpmt = 0;
		nThermalUnit = 0;
		bUseMassDensity = FALSE;
		dDampingRatio = 0.05;
		dAlphaD2E = 0.;
		dBetaD2E = 0.;
	}
};
/**
@brief 재질 정보를 가지고 있는 구조체.
*/
struct T_MATL_D
{
	CString Type;       // 4자 이내 
	//"A" 일 경우 알루미늄 재질.
	//기본적으로 유저타입과 거의 동일한 동작을 함.
	//Data1 안에 SubCodeName을 기존의 사용용도가 아닌 다른 용도로 사용함으로 주의 할 것. (sjhuh-2013-10-01)
	/**< 재질의 종류
	사용 가능한 값은 0 = None, 1 = Steel, 2 = Concrete, 3 = User, 4 = SRC, 5 = Aluminum 이다.*/

	CString Name; /**< 재질의 이름 */      // 16자 이내
	T_MATL_ALL Data1;   // steel, concrete, src인 경우 steel
	T_MATL_ALL Data2;   // src인 경우 concrete
	BOOL bChangeSteel;  // SRC인 경우 
	double dSpecificHeat;	/**< 재질의 비열 */
	double dHeatConduction;	/**< 재질의 열 전도율 */
	unsigned int KeyEpmt;     // 600 추가
	int    nThermalUnit;      // 0:Celsius, 1:Fahrenheit (600 추가)
	BOOL   bUseMassDensity;   // 600에서 추가
//  double dDampingRatioD2E;  // 6.4.0에서 추가 : D2E => 사용하지 않음
	double dDampingRatio;      /**< 재질의 감쇠비 */ // 790    사용하지 않는 dDampingRatioD2E 대신 사용
	double dAlphaD2E;         // 6.4.0에서 추가 : D2E => 사용하지 않음 
	double dBetaD2E;          // 6.4.0에서 추가 : D2E => 사용하지 않음  
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		bChangeSteel=TRUE;
		dSpecificHeat = 0.0;      
		dHeatConduction = 0.0;
		KeyEpmt = 0;
		nThermalUnit = 0;
		bUseMassDensity = FALSE;
		dDampingRatio = 0.05;
		dAlphaD2E = 0.;
		dBetaD2E = 0.;
	}
};
struct T_MATL_D_CH_40
{
	char Type[4];
	char Name[16];
	char Code[16];
	T_MATL_ANALYSIS_550 Analysis;
};
struct T_MATL_D_CH_431
{
	char Type[4];
	char Name[16];
	T_MATL_ALL_CH_550 Data1;
	T_MATL_ALL_CH_550 Data2;
	BOOL bChangeSteel;
};
struct T_MATL_D_CH_550
{
	char Type[4];
	char Name[16];
	T_MATL_ALL_CH_550 Data1;
	T_MATL_ALL_CH_550 Data2;
	BOOL bChangeSteel;
	double dSpecificHeat;      
	double dHeatConduction;
};
struct T_MATL_D_CH_580
{
	char Type[4];
	char Name[16];
	T_MATL_ALL_CH_580 Data1;     // 다음 버전 변경시 주의! 
	T_MATL_ALL_CH_580 Data2;     // 다음 버전 변경시 주의! 
	BOOL bChangeSteel;
	double dSpecificHeat;      
	double dHeatConduction;  
};
struct T_MATL_D_CH_630
{
	char Type[4];
	char Name[16];
	T_MATL_ALL_CH_730 Data1;
	T_MATL_ALL_CH_730 Data2;
	BOOL bChangeSteel;
	double dSpecificHeat;      
	double dHeatConduction;
	unsigned int KeyEpmt;
	int nThermalUnit;
	int bUseMassDensity;
};
struct T_MATL_D_CH_730
{
	char Type[4];
	char Name[16];
	T_MATL_ALL_CH_730 Data1;
	T_MATL_ALL_CH_730 Data2;
	BOOL bChangeSteel;
	double dSpecificHeat;      
	double dHeatConduction;
	unsigned int KeyEpmt;
	int nThermalUnit;
	int bUseMassDensity;
	double dDampingRatioD2E;  // 6.4.0에서 추가 : D2E => 사용하지 않음   
	double dAlphaD2E;         // 6.4.0에서 추가 : D2E => 사용하지 않음  
	double dBetaD2E;          // 6.4.0에서 추가 : D2E => 사용하지 않음  
};
struct T_MATL_D_CH_845
{
	char Type[4];
	char Name[16];
	T_MATL_ALL_CH_845 Data1;
	T_MATL_ALL_CH_845 Data2;
	BOOL bChangeSteel;
	double dSpecificHeat;      
	double dHeatConduction;
	unsigned int KeyEpmt;
	int nThermalUnit;
	int bUseMassDensity;
	//  double dDampingRatioD2E;  // 6.4.0에서 추가 : D2E => 사용하지 않음   
	double dDampingRatio;       // 790    사용하지 않는 dDampingRatioD2E 대신 사용
	double dAlphaD2E;         // 6.4.0에서 추가 : D2E => 사용하지 않음  
	double dBetaD2E;          // 6.4.0에서 추가 : D2E => 사용하지 않음   
};
struct T_MATL_D_CH
{
	char Type[4];
	char Name[16];
	T_MATL_ALL_CH Data1;
	T_MATL_ALL_CH Data2;
	BOOL bChangeSteel;
	double dSpecificHeat;      
	double dHeatConduction;
	unsigned int KeyEpmt;
	int nThermalUnit;
	int bUseMassDensity;
//  double dDampingRatioD2E;  // 6.4.0에서 추가 : D2E => 사용하지 않음   
	double dDampingRatio;       // 790    사용하지 않는 dDampingRatioD2E 대신 사용
	double dAlphaD2E;         // 6.4.0에서 추가 : D2E => 사용하지 않음  
	double dBetaD2E;          // 6.4.0에서 추가 : D2E => 사용하지 않음   
	void ConvertToString(T_MATL_D& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertCharStr(Data1.SubCodeName, rData.Data1.SubCodeName, sizeof(Data1.SubCodeName));
		rData.Data2.Analysis=Data2.Analysis;
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Data2.SubCodeName, rData.Data2.SubCodeName, sizeof(Data2.SubCodeName));
		rData.bChangeSteel=bChangeSteel;
		rData.dSpecificHeat=dSpecificHeat;
		rData.dHeatConduction=dHeatConduction;
		rData.KeyEpmt = KeyEpmt;
		rData.nThermalUnit = nThermalUnit;
		rData.bUseMassDensity = bUseMassDensity;
		rData.dDampingRatio = dDampingRatio    ;
		rData.dAlphaD2E        = dAlphaD2E       ;       
		rData.dBetaD2E         = dBetaD2E        ;        
	}
	void ConvertToChar(T_MATL_D& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis=rData.Data1.Analysis;
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertStrChar(rData.Data1.SubCodeName, Data1.SubCodeName, sizeof(Data1.SubCodeName));
		Data2.Analysis=rData.Data2.Analysis;
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Data2.SubCodeName, Data2.SubCodeName, sizeof(Data2.SubCodeName));
		bChangeSteel=rData.bChangeSteel;
		dSpecificHeat=rData.dSpecificHeat;
		dHeatConduction=rData.dHeatConduction;
		KeyEpmt = rData.KeyEpmt;
		nThermalUnit = rData.nThermalUnit;
		bUseMassDensity = rData.bUseMassDensity;
		dDampingRatio = rData.dDampingRatio;
		dAlphaD2E        = rData.dAlphaD2E       ;       
		dBetaD2E         = rData.dBetaD2E        ;        
	}
	void Convert40(T_MATL_D_CH_40& rData)
	{
		memcpy(Type, rData.Type, __min(sizeof(Type), sizeof(rData.Type)));
		memcpy(Name, rData.Name, __min(sizeof(Name), sizeof(rData.Name)));
		memcpy(Data1.CodeName, rData.Code, __min(sizeof(Data1.CodeName), sizeof(rData.Code)));
		memset(&Data1, 0, sizeof(Data1));
		memset(&Data2, 0, sizeof(Data2));
		if(rData.Code[0] != 0)
			memcpy(Data1.CodeMatlName, rData.Name, __min(sizeof(Data1.CodeMatlName), sizeof(rData.Name)));
		Data1.Analysis.Elast=rData.Analysis.Elast;
		Data1.Analysis.Poisson=rData.Analysis.Poisson;
		Data1.Analysis.Thermal=rData.Analysis.Thermal;
		Data1.Analysis.Density=rData.Analysis.Density;
		Data1.Analysis.MassDensity = 0.0;
		bChangeSteel=TRUE;
		dSpecificHeat=0.0;
		dHeatConduction=0.0;
		KeyEpmt = 0;
		nThermalUnit = 0;
		bUseMassDensity = FALSE;
		dDampingRatio = 0.05;
		dAlphaD2E = 0.;
		dBetaD2E = 0.;
	}
	void Convert431(T_MATL_D_CH_431& rData)
	{
		memcpy(Type, rData.Type, __min(sizeof(Type), sizeof(rData.Type)));
		memcpy(Name, rData.Name, __min(sizeof(Name), sizeof(rData.Name)));
		Data1.Convert550(rData.Data1);
		Data2.Convert550(rData.Data2);
		bChangeSteel=rData.bChangeSteel;
		dSpecificHeat=0.0;
		dHeatConduction=0.0;
		KeyEpmt = 0;
		nThermalUnit = 0;
		bUseMassDensity = FALSE;
		dDampingRatio = 0.05;
		dAlphaD2E = 0.;
		dBetaD2E = 0.;
	}
	void Convert550(T_MATL_D_CH_550& rData)
	{
		memcpy(Type, rData.Type, __min(sizeof(Type), sizeof(rData.Type)));
		memcpy(Name, rData.Name, __min(sizeof(Name), sizeof(rData.Name)));
		Data1.Convert550(rData.Data1);
		Data2.Convert550(rData.Data2);
		bChangeSteel=rData.bChangeSteel;
		dSpecificHeat=rData.dSpecificHeat;
		dHeatConduction=rData.dHeatConduction;
		KeyEpmt = 0;
		nThermalUnit = 0;
		bUseMassDensity = FALSE;
		dDampingRatio = 0.05;
		dAlphaD2E = 0.;
		dBetaD2E = 0.;
	}
	void Convert580(T_MATL_D_CH_580& rData)
	{
		memcpy(Type, rData.Type, __min(sizeof(Type), sizeof(rData.Type)));
		memcpy(Name, rData.Name, __min(sizeof(Name), sizeof(rData.Name)));
		Data1.Convert580(rData.Data1);
		Data2.Convert580(rData.Data2);
		bChangeSteel=rData.bChangeSteel;
		dSpecificHeat=rData.dSpecificHeat;
		dHeatConduction=rData.dHeatConduction;
		KeyEpmt = 0;
		nThermalUnit = 0;
		bUseMassDensity = FALSE;
		dDampingRatio = 0.05;
		dAlphaD2E = 0.;
		dBetaD2E = 0.;
	}
	void Convert630(T_MATL_D_CH_630& rData)
	{
		memcpy(Type, rData.Type, __min(sizeof(Type), sizeof(rData.Type)));
		memcpy(Name, rData.Name, __min(sizeof(Name), sizeof(rData.Name)));
		Data1.Convert730(rData.Data1);
		Data2.Convert730(rData.Data2);
		bChangeSteel=rData.bChangeSteel;
		dSpecificHeat=rData.dSpecificHeat;
		dHeatConduction=rData.dHeatConduction;
		KeyEpmt = rData.KeyEpmt;
		nThermalUnit = rData.nThermalUnit;
		bUseMassDensity = rData.bUseMassDensity;
		dDampingRatio = 0.05;
		dAlphaD2E = 0.;
		dBetaD2E = 0.;
	}
	void Convert730(T_MATL_D_CH_730& rData)
	{
		memcpy(Type, rData.Type, __min(sizeof(Type), sizeof(rData.Type)));
		memcpy(Name, rData.Name, __min(sizeof(Name), sizeof(rData.Name)));
		Data1.Convert730(rData.Data1);
		Data2.Convert730(rData.Data2);
		bChangeSteel=rData.bChangeSteel;
		dSpecificHeat=rData.dSpecificHeat;
		dHeatConduction=rData.dHeatConduction;
		KeyEpmt = rData.KeyEpmt;
		nThermalUnit = rData.nThermalUnit;
		bUseMassDensity = rData.bUseMassDensity;
		dDampingRatio = 0.05;
		dAlphaD2E = rData.dAlphaD2E;
		dBetaD2E = rData.dBetaD2E;
	}
	void Convert845(T_MATL_D_CH_845& rData)
	{
		memcpy(Type, rData.Type, __min(sizeof(Type), sizeof(rData.Type)));
		memcpy(Name, rData.Name, __min(sizeof(Name), sizeof(rData.Name)));
		Data1.Convert845(rData.Data1);
		Data2.Convert845(rData.Data2);
		bChangeSteel=rData.bChangeSteel;
		dSpecificHeat=rData.dSpecificHeat;
		dHeatConduction=rData.dHeatConduction;
		KeyEpmt = rData.KeyEpmt;
		nThermalUnit = rData.nThermalUnit;
		bUseMassDensity = rData.bUseMassDensity;
		dDampingRatio = rData.dDampingRatio;
		dAlphaD2E = rData.dAlphaD2E;
		dBetaD2E = rData.dBetaD2E;
	}
};
struct T_MATL_UDRD_D_550
{
	T_MATL_K key;
	T_MATL_D_550 data;
};
struct T_MATL_UDRD_D_580
{
	T_MATL_K key;
	T_MATL_D_580 data;
};
struct T_MATL_UDRD_D_630
{
	T_MATL_K key;
	T_MATL_D_630 data;
};
struct T_MATL_UDRD_D_730
{
	T_MATL_K key;
	T_MATL_D_730 data;
};
struct T_MATL_UDRD_D_845
{
	T_MATL_K key;
	T_MATL_D_845 data;
};
struct T_MATL_UDRD_D
{
	T_MATL_K key;
	T_MATL_D data;
};
struct T_MATL_UDRD_D_CH_40
{
	T_MATL_K key;
	T_MATL_D_CH_40 data;
};
struct T_MATL_UDRD_D_CH_431
{
	T_MATL_K key;
	T_MATL_D_CH_431 data;
};
struct T_MATL_UDRD_D_CH_550
{
	T_MATL_K key;
	T_MATL_D_CH_550 data;
};
struct T_MATL_UDRD_D_CH_580
{
	T_MATL_K key;
	T_MATL_D_CH_580 data;
};
struct T_MATL_UDRD_D_CH_630
{
	T_MATL_K key;
	T_MATL_D_CH_630 data;
};
struct T_MATL_UDRD_D_CH_730
{
	T_MATL_K key;
	T_MATL_D_CH_730 data;
};
struct T_MATL_UDRD_D_CH_845
{
	T_MATL_K key;
	T_MATL_D_CH_845 data;
};
struct T_MATL_UDRD_D_CH
{
	T_MATL_K key;
	T_MATL_D_CH data;
};

struct T_MATL_UNIT
{
	T_MATL_UNIT()
	{
		Type=D_UNITSYS_NONE;
		Name=D_UNITSYS_NONE;
		CodeName=D_UNITSYS_NONE;
		CodeMatlName=D_UNITSYS_NONE;
		Elast=D_UNITSYS_BASE_ELAST;
		Poisson=D_UNITSYS_NONE;
		Thermal=D_UNITSYS_BASE_THERMAL;
		Density=D_UNITSYS_BASE_DENSITY;
		MassDensity=D_UNITSYS_BASE_MASSDENSITY;
		Elast_y=D_UNITSYS_BASE_ELAST;
		Poisson_y=D_UNITSYS_NONE;
		Thermal_y=D_UNITSYS_BASE_THERMAL;
		Elast_z=D_UNITSYS_BASE_ELAST;
		Poisson_z=D_UNITSYS_NONE;
		Thermal_z=D_UNITSYS_BASE_THERMAL;
		ShearModulus=D_UNITSYS_BASE_ELAST;
		ShearModulus_y=D_UNITSYS_BASE_ELAST;
		ShearModulus_z=D_UNITSYS_BASE_ELAST;
		bChangeSteel=D_UNITSYS_NONE;
		dSpecificHeat=D_UNITSYS_BASE_SPHEAT;
		dHeatConduction=D_UNITSYS_BASE_CONDUCTION;
		nIsoOrtho =D_UNITSYS_NONE;
	}
	int Type;
	int Name;
	int CodeName;
	int CodeMatlName;
	int Elast;
	int Poisson;
	int Thermal;
	int Density;
	int MassDensity;
	int Elast_y;
	int Poisson_y;
	int Thermal_y;
	int Elast_z;
	int Poisson_z;
	int Thermal_z;
	int ShearModulus;
	int ShearModulus_y;
	int ShearModulus_z;
	int bChangeSteel;
	int dSpecificHeat;
	int dHeatConduction;
	int nIsoOrtho;
};

struct T_MATD_ALL_422
{
	CString CodeName;          //
	CString CodeMatlName;
	T_MATL_ANALYSIS_550 Analysis;
	T_MATL_DESIGN_422 Design;
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		Analysis.Initialize();
		Design.Initialize();
	}
};
struct T_MATD_ALL_550
{
	CString CodeName;          
	CString CodeMatlName;
	T_MATL_ANALYSIS_550 Analysis;
	T_MATL_DESIGN_550 Design;
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		Analysis.Initialize();
		Design.Initialize();
	}
};
struct T_MATD_ALL_560
{
	CString CodeName;          
	CString CodeMatlName;
	T_MATL_ANALYSIS_580 Analysis;
	T_MATL_DESIGN_560 Design;
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		Analysis.Initialize();
		Design.Initialize();
	}
};
struct T_MATD_ALL_580
{
	CString CodeName;          //
	CString CodeMatlName;
	T_MATL_ANALYSIS_580 Analysis;
	T_MATL_DESIGN_702 Design;     
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		Analysis.Initialize();
		Design.Initialize();
	}
};
struct T_MATD_ALL_702
{
	CString CodeName;
	CString CodeMatlName;
	T_MATL_ANALYSIS_845 Analysis;  // 다음에 변경될 때 주의!  바뀌기 전의 버젼으로 바꾸어 줄 것
	T_MATL_DESIGN_702 Design;
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		Analysis.Initialize();
		Design.Initialize();
	}
};
struct T_MATD_ALL_721
{
	CString CodeName;
	CString CodeMatlName;
	T_MATL_ANALYSIS_845 Analysis;  // 다음에 변경될 때 주의!  바뀌기 전의 버젼으로 바꾸어 줄 것
	T_MATL_DESIGN_721 Design;
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		Analysis.Initialize();
		Design.Initialize();
	}
};

struct T_MATD_ALL_730
{
	CString CodeName;          //
	CString CodeMatlName;
	T_MATL_ANALYSIS_845 Analysis;
	T_MATL_DESIGN_810 Design;
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		Analysis.Initialize();
		Design.Initialize();
	}
};
struct T_MATD_ALL_810
{
	CString CodeName;
	CString CodeMatlName;
	CString SubCodeName;
	T_MATL_ANALYSIS_845 Analysis;
	T_MATL_DESIGN_810 Design;
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		SubCodeName.Empty();
		Analysis.Initialize();
		Design.Initialize();
	}
};

struct T_MATD_ALL_830
{
	CString CodeName;
	CString CodeMatlName;
	CString SubCodeName;
	T_MATL_ANALYSIS_845 Analysis;
	T_MATL_DESIGN_830 Design;
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		SubCodeName.Empty();
		Analysis.Initialize();
		Design.Initialize();
	}
};

struct T_MATD_ALL_835
{
	CString CodeName;
	CString CodeMatlName;
	CString SubCodeName;
	T_MATL_ANALYSIS_845 Analysis;
	T_MATL_DESIGN_835 Design;
	
	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS_845 AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS_845 AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS_845 AnalysisWeb;       // Composite Hybrid
	
	T_MATL_DESIGN_835 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_835 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_835 DesignWeb;       // Composite Hybrid
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		SubCodeName.Empty();
		Analysis.Initialize();
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();
		
		Design.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
	}
};
struct T_MATD_ALL_845
{
	CString CodeName;
	CString CodeMatlName;
	CString SubCodeName;
	T_MATL_ANALYSIS_845 Analysis;
	T_MATL_DESIGN_896 Design;

	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS_845 AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS_845 AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS_845 AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_896 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_896 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_896 DesignWeb;       // Composite Hybrid
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		SubCodeName.Empty();
		Analysis.Initialize();
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();

		Design.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
	}
};

struct T_MATD_ALL_896
{
	CString CodeName;
	CString CodeMatlName;
	CString SubCodeName;
	T_MATL_ANALYSIS Analysis;
	T_MATL_DESIGN_896 Design;

	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_896 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_896 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_896 DesignWeb;       // Composite Hybrid
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		SubCodeName.Empty();
		Analysis.Initialize();
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();

		Design.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
	}
};
struct T_MATD_ALL_900_TEMP
{
	CString CodeName;
	CString CodeMatlName;
	CString SubCodeName;
	T_MATL_ANALYSIS Analysis;
	T_MATL_DESIGN_900_TEMP Design;

	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_900_TEMP DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_900_TEMP DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_900_TEMP DesignWeb;       // Composite Hybrid
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		SubCodeName.Empty();
		Analysis.Initialize();
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();

		Design.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
	}
};
struct T_MATD_ALL_925
{
	CString CodeName;
	CString CodeMatlName;
	CString SubCodeName;
	T_MATL_ANALYSIS Analysis;
	T_MATL_DESIGN_925 Design;

	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_925 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_925 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_925 DesignWeb;       // Composite Hybrid
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		SubCodeName.Empty();
		Analysis.Initialize();
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();

		Design.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
	}
};
struct T_MATD_ALL_945
{
	CString CodeName;
	CString CodeMatlName;
	CString SubCodeName;
	T_MATL_ANALYSIS Analysis;
	T_MATL_DESIGN_950 Design;

	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_950 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_950 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_950 DesignWeb;       // Composite Hybrid
	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		SubCodeName.Empty();
		Analysis.Initialize();
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();

		Design.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
	}
};
struct T_MATD_ALL_950
{
	CString CodeName;
	CString CodeMatlName;
	CString SubCodeName;
	T_MATL_ANALYSIS Analysis;
	T_MATL_DESIGN_950 Design;

	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_950 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_950 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_950 DesignWeb;       // Composite Hybrid

	BOOL bWithoutPT;    // 9.5.0  일본 Steel

	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		SubCodeName.Empty();
		Analysis.Initialize();
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();

		Design.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
		bWithoutPT = FALSE;
	}
};
struct T_MATD_ALL_955
{
	CString CodeName;
	CString CodeMatlName;
	CString SubCodeName;
	T_MATL_ANALYSIS Analysis;
	T_MATL_DESIGN_955 Design;

	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_955 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_955 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_955 DesignWeb;       // Composite Hybrid

	BOOL bWithoutPT;    // 9.5.0  일본 Steel

	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		SubCodeName.Empty();
		Analysis.Initialize();
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();

		Design.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
		bWithoutPT = FALSE;
	}
};

struct T_MATD_ALL_960
{
	CString CodeName;
	CString CodeMatlName;
	CString SubCodeName;
	T_MATL_ANALYSIS Analysis;
	T_MATL_DESIGN_960 Design;

	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_960 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_960 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_960 DesignWeb;       // Composite Hybrid

	BOOL bWithoutPT;    // 9.5.0  일본 Steel

	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		SubCodeName.Empty();
		Analysis.Initialize();
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();

		Design.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
		bWithoutPT = FALSE;
	}
};
// 변수 추가시 CDBLib::FillMatdFromMatl 함수 수정
struct T_MATD_ALL
{
	CString CodeName;
	CString CodeMatlName;
	CString SubCodeName;
	T_MATL_ANALYSIS Analysis;
	T_MATL_DESIGN Design;
	
	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN DesignWeb;       // Composite Hybrid

	BOOL bWithoutPT;    // 9.5.0  일본 Steel

	void Initialize()
	{
		CodeName.Empty();
		CodeMatlName.Empty();
		SubCodeName.Empty();
		Analysis.Initialize();
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();

		Design.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
		bWithoutPT = FALSE;
	}
	void Convert422(T_MATD_ALL_422& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;  
		SubCodeName.Empty();
		Analysis.Convert550(rData.Analysis);
		Design.Convert422(rData.Design);
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
		bWithoutPT = FALSE;
	}
	void Convert550(T_MATD_ALL_550& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;    
		SubCodeName.Empty();
		Analysis.Convert550(rData.Analysis);
		Design.Convert550(rData.Design);    
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
		bWithoutPT = FALSE;
	}  
	void Convert560(T_MATD_ALL_560& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;    
		SubCodeName.Empty();
		Analysis.Convert580(rData.Analysis);
		Design.Convert560(rData.Design);    
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
		bWithoutPT = FALSE;
	}  
	void Convert580(T_MATD_ALL_580& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;  
		SubCodeName.Empty();
		Analysis.Convert580(rData.Analysis);
		Design.Convert702(rData.Design);           // 다음에 변경될 때 주의!  바뀌기 전의 버젼으로 바꾸어 줄 것
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
		bWithoutPT = FALSE;
	} 
	void Convert702(T_MATD_ALL_702& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;    
		SubCodeName.Empty();
		Analysis.Convert845(rData.Analysis); // 다음에 변경될 때 주의!  바뀌기 전의 버젼으로 바꾸어 줄 것
		Design.Convert702(rData.Design);          
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
		bWithoutPT = FALSE;
	} 
	void Convert721(T_MATD_ALL_721& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		SubCodeName.Empty();
		Analysis.Convert845(rData.Analysis); // 다음에 변경될 때 주의!  바뀌기 전의 버젼으로 바꾸어 줄 것
		Design.Convert721(rData.Design);          
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
		bWithoutPT = FALSE;
	}
	void Convert730(T_MATD_ALL_730& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		SubCodeName.Empty();
		Analysis.Convert845(rData.Analysis); // 다음에 변경될 때 주의!  바뀌기 전의 버젼으로 바꾸어 줄 것
		Design.Convert810(rData.Design);
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
		bWithoutPT = FALSE;
	}
	void Convert810(T_MATD_ALL_810& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		SubCodeName = rData.SubCodeName;
		Analysis.Convert845(rData.Analysis); // 다음에 변경될 때 주의!  바뀌기 전의 버젼으로 바꾸어 줄 것
		Design.Convert810(rData.Design);
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
		bWithoutPT = FALSE;
	}
	void Convert830(T_MATD_ALL_830& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		SubCodeName = rData.SubCodeName;
		Analysis.Convert845(rData.Analysis); // 다음에 변경될 때 주의!  바뀌기 전의 버젼으로 바꾸어 줄 것
		Design.Convert830(rData.Design);
		AnalysisTopFlange.Initialize();
		AnalysisBotFlange.Initialize();
		AnalysisWeb.Initialize();
		DesignTopFlange.Initialize();
		DesignBotFlange.Initialize();
		DesignWeb.Initialize();
		bHybridFactor = FALSE;
		bWithoutPT = FALSE;
	}
	void Convert835(T_MATD_ALL_835& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		SubCodeName = rData.SubCodeName;
		Analysis.Convert845(rData.Analysis); // 다음에 변경될 때 주의!  바뀌기 전의 버젼으로 바꾸어 줄 것
		Design.Convert835(rData.Design);
		AnalysisTopFlange.Convert845(rData.AnalysisTopFlange);
		AnalysisBotFlange.Convert845(rData.AnalysisBotFlange);
		AnalysisWeb.Convert845(rData.AnalysisWeb);
		DesignTopFlange.Convert835(rData.DesignTopFlange);
		DesignBotFlange.Convert835(rData.DesignBotFlange);
		DesignWeb.Convert835(rData.DesignWeb);
		bHybridFactor = rData.bHybridFactor;
		bWithoutPT = FALSE;
	}
	void Convert845(T_MATD_ALL_845& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		SubCodeName = rData.SubCodeName;
		Analysis.Convert845(rData.Analysis); // 다음에 변경될 때 주의!  바뀌기 전의 버젼으로 바꾸어 줄 것
		Design.Convert896(rData.Design);
		AnalysisTopFlange.Convert845(rData.AnalysisTopFlange);
		AnalysisBotFlange.Convert845(rData.AnalysisBotFlange);
		AnalysisWeb.Convert845(rData.AnalysisWeb);
		DesignTopFlange.Convert896(rData.DesignTopFlange);
		DesignBotFlange.Convert896(rData.DesignBotFlange);
		DesignWeb.Convert896(rData.DesignWeb);
		bHybridFactor = rData.bHybridFactor;
		bWithoutPT = FALSE;
	}
	void Convert896(T_MATD_ALL_896& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		SubCodeName = rData.SubCodeName;
		Analysis = rData.Analysis;
		AnalysisTopFlange = rData.AnalysisBotFlange;
		AnalysisBotFlange = rData.AnalysisBotFlange;
		AnalysisWeb = rData.AnalysisWeb;

		Design.Convert896(rData.Design);
		DesignTopFlange.Convert896(rData.DesignTopFlange);
		DesignBotFlange.Convert896(rData.DesignBotFlange);
		DesignWeb.Convert896(rData.DesignWeb);
		bHybridFactor = rData.bHybridFactor;
		bWithoutPT = FALSE;
	}
	void Convert900_TEMP(T_MATD_ALL_900_TEMP& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		SubCodeName = rData.SubCodeName;
		Analysis = rData.Analysis;
		AnalysisTopFlange = rData.AnalysisBotFlange;
		AnalysisBotFlange = rData.AnalysisBotFlange;
		AnalysisWeb = rData.AnalysisWeb;

		Design.Convert900_TEMP(rData.Design);
		DesignTopFlange.Convert900_TEMP(rData.DesignTopFlange);
		DesignBotFlange.Convert900_TEMP(rData.DesignBotFlange);
		DesignWeb.Convert900_TEMP(rData.DesignWeb);
		bHybridFactor = rData.bHybridFactor;
		bWithoutPT = FALSE;
	}
	void Convert925(T_MATD_ALL_925& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		SubCodeName = rData.SubCodeName;
		Analysis = rData.Analysis;
		AnalysisTopFlange = rData.AnalysisBotFlange;
		AnalysisBotFlange = rData.AnalysisBotFlange;
		AnalysisWeb = rData.AnalysisWeb;

		Design.Convert925(rData.Design);
		DesignTopFlange.Convert925(rData.DesignTopFlange);
		DesignBotFlange.Convert925(rData.DesignBotFlange);
		DesignWeb.Convert925(rData.DesignWeb);
		bHybridFactor = rData.bHybridFactor;
		bWithoutPT = FALSE;
	}
	void Convert945(T_MATD_ALL_945& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		SubCodeName = rData.SubCodeName;
		Analysis = rData.Analysis;
		AnalysisTopFlange = rData.AnalysisBotFlange;
		AnalysisBotFlange = rData.AnalysisBotFlange;
		AnalysisWeb = rData.AnalysisWeb;

		Design.Convert950(rData.Design);
		DesignTopFlange.Convert950(rData.DesignTopFlange);
		DesignBotFlange.Convert950(rData.DesignBotFlange);
		DesignWeb.Convert950(rData.DesignWeb);
		bHybridFactor = rData.bHybridFactor;
		bWithoutPT = FALSE;
	}
	void Convert950(T_MATD_ALL_950& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		SubCodeName = rData.SubCodeName;
		Analysis = rData.Analysis;
		AnalysisTopFlange = rData.AnalysisBotFlange;
		AnalysisBotFlange = rData.AnalysisBotFlange;
		AnalysisWeb = rData.AnalysisWeb;

		Design.Convert950(rData.Design);
		DesignTopFlange.Convert950(rData.DesignTopFlange);
		DesignBotFlange.Convert950(rData.DesignBotFlange);
		DesignWeb.Convert950(rData.DesignWeb);
		bHybridFactor = rData.bHybridFactor;
		bWithoutPT = rData.bWithoutPT;
	}	
	void Convert955(T_MATD_ALL_955& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		SubCodeName = rData.SubCodeName;
		Analysis = rData.Analysis;
		AnalysisTopFlange = rData.AnalysisBotFlange;
		AnalysisBotFlange = rData.AnalysisBotFlange;
		AnalysisWeb = rData.AnalysisWeb;

		Design.Convert955(rData.Design);
		DesignTopFlange.Convert955(rData.DesignTopFlange);
		DesignBotFlange.Convert955(rData.DesignBotFlange);
		DesignWeb.Convert955(rData.DesignWeb);
		bHybridFactor = rData.bHybridFactor;
		bWithoutPT = rData.bWithoutPT;
	}
	void Convert960(T_MATD_ALL_960& rData)
	{
		CodeName = rData.CodeName;
		CodeMatlName = rData.CodeMatlName;
		SubCodeName = rData.SubCodeName;
		Analysis = rData.Analysis;
		AnalysisTopFlange = rData.AnalysisBotFlange;
		AnalysisBotFlange = rData.AnalysisBotFlange;
		AnalysisWeb = rData.AnalysisWeb;

		Design.Convert960(rData.Design);
		DesignTopFlange.Convert960(rData.DesignTopFlange);
		DesignBotFlange.Convert960(rData.DesignBotFlange);
		DesignWeb.Convert960(rData.DesignWeb);
		bHybridFactor = rData.bHybridFactor;
		bWithoutPT = rData.bWithoutPT;
	}
};

struct T_MATD_ALL_CH_422
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	T_MATL_ANALYSIS_550 Analysis;
	T_MATL_DESIGN_422 Design;
};
struct T_MATD_ALL_CH_550
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	T_MATL_ANALYSIS_550 Analysis;
	T_MATL_DESIGN_550 Design;
};
struct T_MATD_ALL_CH_560
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	T_MATL_ANALYSIS_580 Analysis;     
	T_MATL_DESIGN_560 Design;
};
struct T_MATD_ALL_CH_580
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	T_MATL_ANALYSIS_580 Analysis;    
	T_MATL_DESIGN_702 Design;     // 다음에 변경될 때 주의! 바뀌기 전의 버젼으로 바꾸어 줄 것
};
struct T_MATD_ALL_CH_702
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	T_MATL_ANALYSIS_845 Analysis;    // 다음에 변경될 때 주의! 바뀌기 전의 버젼으로 바꾸어 줄 것
	T_MATL_DESIGN_702 Design;    
};
struct T_MATD_ALL_CH_721
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	T_MATL_ANALYSIS_845 Analysis;    // 다음에 변경될 때 주의! 바뀌기 전의 버젼으로 바꾸어 줄 것
	T_MATL_DESIGN_721 Design;    
};
struct T_MATD_ALL_CH_730
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	T_MATL_ANALYSIS_845 Analysis;    
	T_MATL_DESIGN_810 Design;
};
struct T_MATD_ALL_CH_810
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	char SubCodeName[16];
	T_MATL_ANALYSIS_845 Analysis;    
	T_MATL_DESIGN_810 Design;
};
struct T_MATD_ALL_CH_830
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	char SubCodeName[16];
	T_MATL_ANALYSIS_845 Analysis;    
	T_MATL_DESIGN_830 Design;
};

struct T_MATD_ALL_CH_835
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	char SubCodeName[16];
	T_MATL_ANALYSIS_845 Analysis;    
	T_MATL_DESIGN_CH_835 Design;
	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS_845 AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS_845 AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS_845 AnalysisWeb;       // Composite Hybrid
	
	T_MATL_DESIGN_CH_835 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_835 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_835 DesignWeb;       // Composite Hybrid
};

struct T_MATD_ALL_CH_845
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	char SubCodeName[16];
	T_MATL_ANALYSIS_845 Analysis;    
	T_MATL_DESIGN_CH_896 Design;
	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS_845 AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS_845 AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS_845 AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_CH_896 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_896 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_896 DesignWeb;       // Composite Hybrid
};

struct T_MATD_ALL_CH_896
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	char SubCodeName[16];
	T_MATL_ANALYSIS Analysis;    
	T_MATL_DESIGN_CH_896 Design;
	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_CH_896 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_896 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_896 DesignWeb;       // Composite Hybrid
};
struct T_MATD_ALL_CH_900_TEMP
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	char SubCodeName[16];
	T_MATL_ANALYSIS Analysis;    
	T_MATL_DESIGN_CH_900_TEMP Design;
	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_CH_900_TEMP DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_900_TEMP DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_900_TEMP DesignWeb;       // Composite Hybrid
};
struct T_MATD_ALL_CH_925
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	char SubCodeName[16];
	T_MATL_ANALYSIS Analysis;
	T_MATL_DESIGN_CH_925 Design;
	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_CH_925 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_925 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_925 DesignWeb;       // Composite Hybrid
};
struct T_MATD_ALL_CH_945
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	char SubCodeName[16];
	T_MATL_ANALYSIS Analysis;
	T_MATL_DESIGN_CH_950 Design;
	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_CH_950 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_950 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_950 DesignWeb;       // Composite Hybrid
};
struct T_MATD_ALL_CH_950
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	char SubCodeName[16];
	T_MATL_ANALYSIS Analysis;
	T_MATL_DESIGN_CH_950 Design;
	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_CH_950 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_950 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_950 DesignWeb;       // Composite Hybrid

	BOOL bWithoutPT;    // 9.5.0  일본 Steel
};
struct T_MATD_ALL_CH_955
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	char SubCodeName[16];
	T_MATL_ANALYSIS Analysis;
	T_MATL_DESIGN_CH_955 Design;
	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_CH_955 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_955 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_955 DesignWeb;       // Composite Hybrid

	BOOL bWithoutPT;    // 9.5.0  일본 Steel
};
struct T_MATD_ALL_CH_960
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	char SubCodeName[16];
	T_MATL_ANALYSIS Analysis;
	T_MATL_DESIGN_CH_960 Design;
	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid

	T_MATL_DESIGN_CH_960 DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_960 DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_CH_960 DesignWeb;       // Composite Hybrid

	BOOL bWithoutPT;    // 9.5.0  일본 Steel
};
struct T_MATD_ALL_CH
{
	char CodeName[16];       // 16자 이내
	char CodeMatlName[16];       // 16자 이내
	char SubCodeName[16];
	T_MATL_ANALYSIS Analysis;    
	T_MATL_DESIGN_CH Design;
	BOOL bHybridFactor; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisTopFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisBotFlange; // Composite Hybrid
	T_MATL_ANALYSIS AnalysisWeb;       // Composite Hybrid
	
	T_MATL_DESIGN_CH DesignTopFlange; // Composite Hybrid
	T_MATL_DESIGN_CH DesignBotFlange; // Composite Hybrid
	T_MATL_DESIGN_CH DesignWeb;       // Composite Hybrid

	BOOL bWithoutPT;    // 9.5.0  일본 Steel
};

struct CRC_T_MATD_RBAR
{
	double dRebar_AFu;    // 인장응력도
	double dRebar_AFt_D;  // 허용인장응력도(사하중시)
	double dRebar_AFt_T;  // 허용인장응력도(설계시,온도시)
	double dRebar_AFt_EQ; // 허용인장응력도(지진시)
	double dRebar_AFv;    // 전단 균열 상한치
	double dRebar_Es;     // 탄성 계수
	void Initialize()
	{
		dRebar_AFu    = 0.0;
		dRebar_AFt_D  = 0.0;
		dRebar_AFt_T  = 0.0;
		dRebar_AFt_EQ = 0.0;
		dRebar_AFv    = 0.0;
		dRebar_Es     = 0.0;
	}
};

struct CRC_T_MATD_RBAR_UNIT
{
	CRC_T_MATD_RBAR_UNIT()
	{
		dRebar_AFu    =  D_UNITSYS_BASE_STRESS;
		dRebar_AFt_D  =  D_UNITSYS_BASE_STRESS;
		dRebar_AFt_T  =  D_UNITSYS_BASE_STRESS;
		dRebar_AFt_EQ =  D_UNITSYS_BASE_STRESS;
		dRebar_AFv    =  D_UNITSYS_BASE_STRESS;
		dRebar_Es     =  D_UNITSYS_BASE_STRESS;
	}
	int dRebar_AFu;
	int dRebar_AFt_D;
	int dRebar_AFt_T;
	int dRebar_AFt_EQ;
	int dRebar_AFv;
	int dRebar_Es;
};

struct CRC_T_MATD_D           // 6.4.0에서 추가
{
	CRC_T_MATD_RBAR MbarMatl;
	CRC_T_MATD_RBAR SbarMatl;
	void Initialize()
	{
		MbarMatl.Initialize();
		SbarMatl.Initialize();
	}
};

struct T_MATD_D_422
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_422 Data1;
	T_MATD_ALL_422 Data2;   // SRC일 경우 Concrete
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;  // SRC인 경우 
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();
		bChangeSteel=TRUE;
	}
};
struct T_MATD_D_550
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_550 Data1;
	T_MATD_ALL_550 Data2;   // SRC일 경우 Concrete
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;  // SRC인 경우 
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();
		bChangeSteel=TRUE;
	}
};
struct T_MATD_D_560
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_560 Data1;
	T_MATD_ALL_560 Data2;   // SRC일 경우 Concrete
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;  // SRC인 경우 
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();
		bChangeSteel=TRUE;
	}
};
struct T_MATD_D_580
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_580 Data1;
	T_MATD_ALL_580 Data2;   // SRC일 경우 Concrete
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;  // SRC인 경우 
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();
		bChangeSteel=TRUE;
	}
};
struct T_MATD_D_630
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_702 Data1;
	T_MATD_ALL_702 Data2;   // SRC일 경우 Concrete
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;  // SRC인 경우 
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();
		bChangeSteel=TRUE;
	}
};
struct T_MATD_D_702
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_702 Data1;
	T_MATD_ALL_702 Data2;   // SRC일 경우 Concrete
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();
		bChangeSteel=TRUE;
		CRC.Initialize();
	}
};
struct T_MATD_D_712
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_721 Data1;
	T_MATD_ALL_721 Data2;   // SRC일 경우 Concrete
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();
		bChangeSteel=TRUE;
		CRC.Initialize();
	}
};
struct T_MATD_D_721
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_721 Data1;
	T_MATD_ALL_721 Data2;   // SRC일 경우 Concrete
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)

	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();
		bChangeSteel=TRUE;
		CRC.Initialize();

		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;
	}
};
struct T_MATD_D_730
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_730 Data1;
	T_MATD_ALL_730 Data2;   // SRC일 경우 Concrete
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)
	
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();
		bChangeSteel=TRUE;
		CRC.Initialize();
		
		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;
	}
};

struct T_MATD_D_810
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_810 Data1;
	T_MATD_ALL_810 Data2;   // SRC일 경우 Concrete
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)
	
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();
		bChangeSteel=TRUE;
		CRC.Initialize();
		
		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;
	}
};
struct T_MATD_D_820
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_830 Data1;
	T_MATD_ALL_830 Data2;   // SRC일 경우 Concrete
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)
	
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();
		bChangeSteel=TRUE;
		CRC.Initialize();
		
		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;
	}
};
struct T_MATD_D_830
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_830 Data1;
	T_MATD_ALL_830 Data2;   // SRC일 경우 Concrete, PSC Composite
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName; 
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;

	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)

	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check
	
	CString Rebar_CodeName2;      // v821추가 PSC Composite의 슬래브
	CString MainRebar_RebarName2; // v821추가 PSC Composite의 슬래브
	CString SubRebar_RebarName2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();
		bChangeSteel=TRUE;
		CRC.Initialize();
		
		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;

		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();
	}
};

struct T_MATD_D_835
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_835 Data1;
	T_MATD_ALL_835 Data2;   // SRC일 경우 Concrete, PSC Composite
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName; 
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	
	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)
	
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check
	
	CString Rebar_CodeName2;      // v821추가 PSC Composite의 슬래브
	CString MainRebar_RebarName2; // v821추가 PSC Composite의 슬래브
	CString SubRebar_RebarName2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브
	
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();
		
		bChangeSteel=TRUE;
		CRC.Initialize();
		
		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;
		
		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();
	}
};

struct T_MATD_D_845
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_845 Data1;
	T_MATD_ALL_845 Data2;   // SRC일 경우 Concrete, PSC Composite
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName; 
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;

	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)

	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	CString Rebar_CodeName2;      // v821추가 PSC Composite의 슬래브
	CString MainRebar_RebarName2; // v821추가 PSC Composite의 슬래브
	CString SubRebar_RebarName2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브

	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();

		bChangeSteel=TRUE;
		CRC.Initialize();

		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;

		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();
	}
};
struct T_MATD_D_896
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_896 Data1;
	T_MATD_ALL_896 Data2;   // SRC일 경우 Concrete, PSC Composite
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName; 
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;

	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)

	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	CString Rebar_CodeName2;      // v821추가 PSC Composite의 슬래브
	CString MainRebar_RebarName2; // v821추가 PSC Composite의 슬래브
	CString SubRebar_RebarName2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();

		bChangeSteel=TRUE;
		CRC.Initialize();

		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;

		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();
	}
};
struct T_MATD_D_900_TEMP
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_900_TEMP Data1;
	T_MATD_ALL_900_TEMP Data2;   // SRC일 경우 Concrete, PSC Composite
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName; 
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;

	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)

	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	CString Rebar_CodeName2;      // v821추가 PSC Composite의 슬래브
	CString MainRebar_RebarName2; // v821추가 PSC Composite의 슬래브
	CString SubRebar_RebarName2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();

		bChangeSteel=TRUE;
		CRC.Initialize();

		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;

		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();
	}
};
struct T_MATD_D_925
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_925 Data1;
	T_MATD_ALL_925 Data2;   // SRC일 경우 Concrete, PSC Composite
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;

	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)

	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	CString Rebar_CodeName2;      // v821추가 PSC Composite의 슬래브
	CString MainRebar_RebarName2; // v821추가 PSC Composite의 슬래브
	CString SubRebar_RebarName2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();

		bChangeSteel = TRUE;
		CRC.Initialize();

		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm = 0.0;

		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();
	}
};
struct T_MATD_D_945
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_945 Data1;
	T_MATD_ALL_945 Data2;   // SRC일 경우 Concrete, PSC Composite
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;

	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)

	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	CString Rebar_CodeName2;      // v821추가 PSC Composite의 슬래브
	CString MainRebar_RebarName2; // v821추가 PSC Composite의 슬래브
	CString SubRebar_RebarName2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();

		bChangeSteel = TRUE;
		CRC.Initialize();

		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm = 0.0;

		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();
	}
};

struct T_MATD_D_950
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_950 Data1;
	T_MATD_ALL_950 Data2;   // SRC일 경우 Concrete, PSC Composite
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;

	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)

	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	CString Rebar_CodeName2;      // v821추가 PSC Composite의 슬래브
	CString MainRebar_RebarName2; // v821추가 PSC Composite의 슬래브
	CString SubRebar_RebarName2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();

		bChangeSteel = TRUE;
		CRC.Initialize();

		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm = 0.0;

		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();
	}
};

struct T_MATD_D_955
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_955 Data1;
	T_MATD_ALL_955 Data2;   // SRC일 경우 Concrete, PSC Composite
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;

	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)

	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	CString Rebar_CodeName2;      // v821추가 PSC Composite의 슬래브
	CString MainRebar_RebarName2; // v821추가 PSC Composite의 슬래브
	CString SubRebar_RebarName2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();

		bChangeSteel = TRUE;
		CRC.Initialize();

		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm = 0.0;

		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();
	}
};
struct T_MATD_D_960
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL_960 Data1;
	T_MATD_ALL_960 Data2;   // SRC일 경우 Concrete, PSC Composite
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName;
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;

	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)

	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	CString Rebar_CodeName2;      // v821추가 PSC Composite의 슬래브
	CString MainRebar_RebarName2; // v821추가 PSC Composite의 슬래브
	CString SubRebar_RebarName2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();

		bChangeSteel = TRUE;
		CRC.Initialize();

		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm = 0.0;

		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();
	}
};
// 변수 추가시 CDBLib::FillMatdFromMatl 함수 수정
struct T_MATD_D
{
	CString Type;       //  "S", "C", "SRC", "U"  4자 이내
	CString Name;       // 16자 이내
	T_MATD_ALL Data1;
	T_MATD_ALL Data2;   // SRC일 경우 Concrete, PSC Composite
	CString Rebar_CodeName;
	CString MainRebar_RebarName;
	CString SubRebar_RebarName; 
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;

	BOOL bChangeSteel;  // SRC인 경우 
	CRC_T_MATD_D  CRC; // CRC의 PSC 관련 데이터 (6.4.0 추가)

	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check
 
	CString Rebar_CodeName2;      // v821추가 PSC Composite의 슬래브
	CString MainRebar_RebarName2; // v821추가 PSC Composite의 슬래브
	CString SubRebar_RebarName2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브
	void Initialize()
	{
		Type.Empty();
		Name.Empty();
		Data1.Initialize();
		Data2.Initialize();
		Rebar_CodeName.Empty();
		MainRebar_RebarName.Empty();
		SubRebar_RebarName.Empty();
		MainRebarData.Initialize();
		SubRebarData.Initialize();

		bChangeSteel=TRUE;
		CRC.Initialize();

		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;

		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();
	}
	void Convert422(T_MATD_D_422& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert422(rData.Data1);
		Data2.Convert422(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC.Initialize();
		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;
		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();

	}
	void Convert550(T_MATD_D_550& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert550(rData.Data1);
		Data2.Convert550(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC.Initialize();
		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;
		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();

	}
	void Convert560(T_MATD_D_560& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert560(rData.Data1);
		Data2.Convert560(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC.Initialize();
		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;
		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();

	}
	void Convert580(T_MATD_D_580& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert580(rData.Data1);
		Data2.Convert580(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC.Initialize();
		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;
		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();

	}
	void Convert630(T_MATD_D_630& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert702(rData.Data1);
		Data2.Convert702(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC.Initialize();
		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;
		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();

	}
	void Convert702(T_MATD_D_702& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert702(rData.Data1);
		Data2.Convert702(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;
		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();

	}
	void Convert712(T_MATD_D_712& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert721(rData.Data1);
		Data2.Convert721(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = FALSE;
		dShortTerm = 0.0;
		dLongTerm  = 0.0;
		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();

	}
	void Convert721(T_MATD_D_721& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert721(rData.Data1);
		Data2.Convert721(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm  = rData.dLongTerm;
		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();

	}
	void Convert730(T_MATD_D_730& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert730(rData.Data1);
		Data2.Convert730(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm  = rData.dLongTerm;
		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();

	}
	void Convert810(T_MATD_D_810& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert810(rData.Data1);
		Data2.Convert810(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm  = rData.dLongTerm;
		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();
	}
	void Convert820(T_MATD_D_820& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert830(rData.Data1);
		Data2.Convert830(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm  = rData.dLongTerm;
		Rebar_CodeName2.Empty();
		MainRebar_RebarName2.Empty();
		SubRebar_RebarName2.Empty();
		MainRebarData2.Initialize();
		SubRebarData2.Initialize();
	}
	void Convert830(T_MATD_D_830& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert830(rData.Data1);
		Data2.Convert830(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm  = rData.dLongTerm;
		Rebar_CodeName2 = rData.Rebar_CodeName2;
		MainRebar_RebarName2 = rData.MainRebar_RebarName2;
		SubRebar_RebarName2 = rData.SubRebar_RebarName2;
		MainRebarData2 = rData.MainRebarData2;
		SubRebarData2  = rData.SubRebarData2;
	}
	void Convert835(T_MATD_D_835& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert835(rData.Data1);
		Data2.Convert835(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm  = rData.dLongTerm;
		Rebar_CodeName2 = rData.Rebar_CodeName2;
		MainRebar_RebarName2 = rData.MainRebar_RebarName2;
		SubRebar_RebarName2 = rData.SubRebar_RebarName2;
		MainRebarData2 = rData.MainRebarData2;
		SubRebarData2  = rData.SubRebarData2;
	}
	void Convert845(T_MATD_D_845& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert845(rData.Data1);
		Data2.Convert845(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm  = rData.dLongTerm;
		Rebar_CodeName2 = rData.Rebar_CodeName2;
		MainRebar_RebarName2 = rData.MainRebar_RebarName2;
		SubRebar_RebarName2 = rData.SubRebar_RebarName2;
		MainRebarData2 = rData.MainRebarData2;
		SubRebarData2  = rData.SubRebarData2;
	}
	void Convert896(T_MATD_D_896& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert896(rData.Data1);
		Data2.Convert896(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm  = rData.dLongTerm;
		Rebar_CodeName2 = rData.Rebar_CodeName2;
		MainRebar_RebarName2 = rData.MainRebar_RebarName2;
		SubRebar_RebarName2 = rData.SubRebar_RebarName2;
		MainRebarData2 = rData.MainRebarData2;
		SubRebarData2  = rData.SubRebarData2;
	}
	void Convert900_TEMP(T_MATD_D_900_TEMP& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert900_TEMP(rData.Data1);
		Data2.Convert900_TEMP(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm  = rData.dLongTerm;
		Rebar_CodeName2 = rData.Rebar_CodeName2;
		MainRebar_RebarName2 = rData.MainRebar_RebarName2;
		SubRebar_RebarName2 = rData.SubRebar_RebarName2;
		MainRebarData2 = rData.MainRebarData2;
		SubRebarData2  = rData.SubRebarData2;
	}
	void Convert925(T_MATD_D_925& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert925(rData.Data1);
		Data2.Convert925(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm = rData.dLongTerm;
		Rebar_CodeName2 = rData.Rebar_CodeName2;
		MainRebar_RebarName2 = rData.MainRebar_RebarName2;
		SubRebar_RebarName2 = rData.SubRebar_RebarName2;
		MainRebarData2 = rData.MainRebarData2;
		SubRebarData2 = rData.SubRebarData2;
	}
	void Convert945(T_MATD_D_945& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert945(rData.Data1);
		Data2.Convert945(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm = rData.dLongTerm;
		Rebar_CodeName2 = rData.Rebar_CodeName2;
		MainRebar_RebarName2 = rData.MainRebar_RebarName2;
		SubRebar_RebarName2 = rData.SubRebar_RebarName2;
		MainRebarData2 = rData.MainRebarData2;
		SubRebarData2 = rData.SubRebarData2;
	}
	void Convert950(T_MATD_D_950& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert950(rData.Data1);
		Data2.Convert950(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm = rData.dLongTerm;
		Rebar_CodeName2 = rData.Rebar_CodeName2;
		MainRebar_RebarName2 = rData.MainRebar_RebarName2;
		SubRebar_RebarName2 = rData.SubRebar_RebarName2;
		MainRebarData2 = rData.MainRebarData2;
		SubRebarData2 = rData.SubRebarData2;
	}
	void Convert955(T_MATD_D_955& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert955(rData.Data1);
		Data2.Convert955(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm = rData.dLongTerm;
		Rebar_CodeName2 = rData.Rebar_CodeName2;
		MainRebar_RebarName2 = rData.MainRebar_RebarName2;
		SubRebar_RebarName2 = rData.SubRebar_RebarName2;
		MainRebarData2 = rData.MainRebarData2;
		SubRebarData2 = rData.SubRebarData2;
	}
	void Convert960(T_MATD_D_960& rData)
	{
		Type = rData.Type;
		Name = rData.Name;
		Data1.Convert960(rData.Data1);
		Data2.Convert960(rData.Data2);
		Rebar_CodeName = rData.Rebar_CodeName;
		MainRebar_RebarName = rData.MainRebar_RebarName;
		SubRebar_RebarName = rData.SubRebar_RebarName;
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm = rData.dLongTerm;
		Rebar_CodeName2 = rData.Rebar_CodeName2;
		MainRebar_RebarName2 = rData.MainRebar_RebarName2;
		SubRebar_RebarName2 = rData.SubRebar_RebarName2;
		MainRebarData2 = rData.MainRebarData2;
		SubRebarData2 = rData.SubRebarData2;
	}
};

struct T_MATD_D_CH_422
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_422 Data1;
	T_MATD_ALL_CH_422 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	void ConvertToString(T_MATD_D_422& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		rData.Data1.Design=Data1.Design;
		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		rData.Data2.Analysis=Data2.Analysis;
		rData.Data2.Design=Data2.Design;
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
	}
	/* 필요 없을 듯
	void ConvertToChar(T_MATD_D_422& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis=rData.Data1.Analysis;
		Data1.Design=rData.Data1.Design;
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		Data2.Analysis=rData.Data2.Analysis;
		Data2.Design=rData.Data2.Design;
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Rebar_CodeName, Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertStrChar(rData.MainRebar_RebarName, MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertStrChar(rData.SubRebar_RebarName, SubRebar_RebarName, sizeof(SubRebar_RebarName));
		MainRebarData=rData.MainRebarData;
		SubRebarData=rData.SubRebarData;
		bChangeSteel=rData.bChangeSteel;
	}
	*/
};
struct T_MATD_D_CH_550
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_550 Data1;
	T_MATD_ALL_CH_550 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	void ConvertToString(T_MATD_D_550& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		rData.Data1.Design=Data1.Design;
		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		rData.Data2.Analysis=Data2.Analysis;
		rData.Data2.Design=Data2.Design;
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
	}
	/* 필요 없을 듯
	void ConvertToChar(T_MATD_D_550& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis=rData.Data1.Analysis;
		Data1.Design=rData.Data1.Design;
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		Data2.Analysis=rData.Data2.Analysis;
		Data2.Design=rData.Data2.Design;
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Rebar_CodeName, Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertStrChar(rData.MainRebar_RebarName, MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertStrChar(rData.SubRebar_RebarName, SubRebar_RebarName, sizeof(SubRebar_RebarName));
		MainRebarData=rData.MainRebarData;
		SubRebarData=rData.SubRebarData;
		bChangeSteel=rData.bChangeSteel;
	}
	*/
};
struct T_MATD_D_CH_560
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_560 Data1;
	T_MATD_ALL_CH_560 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	void ConvertToString(T_MATD_D_560& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		rData.Data1.Design=Data1.Design;
		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		rData.Data2.Analysis=Data2.Analysis;
		rData.Data2.Design=Data2.Design;
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
	}
	/* 필요 없을 듯
	void ConvertToChar(T_MATD_D_560& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis=rData.Data1.Analysis;
		Data1.Design=rData.Data1.Design;
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		Data2.Analysis=rData.Data2.Analysis;
		Data2.Design=rData.Data2.Design;
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Rebar_CodeName, Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertStrChar(rData.MainRebar_RebarName, MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertStrChar(rData.SubRebar_RebarName, SubRebar_RebarName, sizeof(SubRebar_RebarName));
		MainRebarData=rData.MainRebarData;
		SubRebarData=rData.SubRebarData;
		bChangeSteel=rData.bChangeSteel;
	}
	*/
};
struct T_MATD_D_CH_580
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_580 Data1;
	T_MATD_ALL_CH_580 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	void ConvertToString(T_MATD_D_580& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		rData.Data1.Design=Data1.Design;
		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		rData.Data2.Analysis=Data2.Analysis;
		rData.Data2.Design=Data2.Design;
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
	}
	/* 필요 없을 듯
	void ConvertToChar(T_MATD_D_580& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis=rData.Data1.Analysis;
		Data1.Design=rData.Data1.Design;
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		Data2.Analysis=rData.Data2.Analysis;
		Data2.Design=rData.Data2.Design;
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Rebar_CodeName, Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertStrChar(rData.MainRebar_RebarName, MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertStrChar(rData.SubRebar_RebarName, SubRebar_RebarName, sizeof(SubRebar_RebarName));
		MainRebarData=rData.MainRebarData;
		SubRebarData=rData.SubRebarData;
		bChangeSteel=rData.bChangeSteel;
	}
	*/
};

struct T_MATD_D_CH_630
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_702 Data1;
	T_MATD_ALL_CH_702 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	void ConvertToString(T_MATD_D_630& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		rData.Data1.Design=Data1.Design;
		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		rData.Data2.Analysis=Data2.Analysis;
		rData.Data2.Design=Data2.Design;
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
	}
};
struct T_MATD_D_CH_702
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_702 Data1;
	T_MATD_ALL_CH_702 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	void ConvertToString(T_MATD_D_702& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		rData.Data1.Design=Data1.Design;
		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		rData.Data2.Analysis=Data2.Analysis;
		rData.Data2.Design=Data2.Design;
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
		rData.CRC = CRC;
	}
	void ConvertToChar(T_MATD_D_702& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis=rData.Data1.Analysis;
		Data1.Design=rData.Data1.Design;
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		Data2.Analysis=rData.Data2.Analysis;
		Data2.Design=rData.Data2.Design;
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Rebar_CodeName, Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertStrChar(rData.MainRebar_RebarName, MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertStrChar(rData.SubRebar_RebarName, SubRebar_RebarName, sizeof(SubRebar_RebarName));
		MainRebarData=rData.MainRebarData;
		SubRebarData=rData.SubRebarData;
		bChangeSteel=rData.bChangeSteel;
		CRC = rData.CRC;
	}
};
struct T_MATD_D_CH_712
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_721 Data1;
	T_MATD_ALL_CH_721 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	void ConvertToString(T_MATD_D_712& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		rData.Data1.Design=Data1.Design;
		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		rData.Data2.Analysis=Data2.Analysis;
		rData.Data2.Design=Data2.Design;
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
		rData.CRC = CRC;
	}
	void ConvertToChar(T_MATD_D_712& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis=rData.Data1.Analysis;
		Data1.Design=rData.Data1.Design;
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		Data2.Analysis=rData.Data2.Analysis;
		Data2.Design=rData.Data2.Design;
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Rebar_CodeName, Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertStrChar(rData.MainRebar_RebarName, MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertStrChar(rData.SubRebar_RebarName, SubRebar_RebarName, sizeof(SubRebar_RebarName));
		MainRebarData=rData.MainRebarData;
		SubRebarData=rData.SubRebarData;
		bChangeSteel=rData.bChangeSteel;
		CRC = rData.CRC;
	}
};
struct T_MATD_D_CH_721
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_721 Data1;
	T_MATD_ALL_CH_721 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check
	void ConvertToString(T_MATD_D_721& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		rData.Data1.Design=Data1.Design;
		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		rData.Data2.Analysis=Data2.Analysis;
		rData.Data2.Design=Data2.Design;
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
		rData.CRC = CRC;
		rData.bServCheck = bServCheck;
		rData.dShortTerm = dShortTerm;
		rData.dLongTerm  = dLongTerm;
	}
	/* 필요 없을 듯
	void ConvertToChar(T_MATD_D_721& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis=rData.Data1.Analysis;
		Data1.Design=rData.Data1.Design;
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		Data2.Analysis=rData.Data2.Analysis;
		Data2.Design=rData.Data2.Design;
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Rebar_CodeName, Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertStrChar(rData.MainRebar_RebarName, MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertStrChar(rData.SubRebar_RebarName, SubRebar_RebarName, sizeof(SubRebar_RebarName));
		MainRebarData=rData.MainRebarData;
		SubRebarData=rData.SubRebarData;
		bChangeSteel=rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm  = rData.dLongTerm;
	}*/
};
struct T_MATD_D_CH_730
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_730 Data1;
	T_MATD_ALL_CH_730 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check
	void ConvertToString(T_MATD_D_730& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		rData.Data1.Design=Data1.Design;
		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		rData.Data2.Analysis=Data2.Analysis;
		rData.Data2.Design=Data2.Design;
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
		rData.CRC = CRC;
		rData.bServCheck = bServCheck;
		rData.dShortTerm = dShortTerm;
		rData.dLongTerm  = dLongTerm;
	}
};

struct T_MATD_D_CH_810
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_810 Data1;
	T_MATD_ALL_CH_810 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check
	void ConvertToString(T_MATD_D_810& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		rData.Data1.Design=Data1.Design;
		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertCharStr(Data1.SubCodeName, rData.Data1.SubCodeName, sizeof(Data1.SubCodeName));
		rData.Data2.Analysis=Data2.Analysis;
		rData.Data2.Design=Data2.Design;
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Data2.SubCodeName, rData.Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
		rData.CRC = CRC;
		rData.bServCheck = bServCheck;
		rData.dShortTerm = dShortTerm;
		rData.dLongTerm  = dLongTerm;
	}
};

struct T_MATD_D_CH_820
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_830 Data1;
	T_MATD_ALL_CH_830 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check
	void ConvertToString(T_MATD_D_820& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		rData.Data1.Design=Data1.Design;
		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertCharStr(Data1.SubCodeName, rData.Data1.SubCodeName, sizeof(Data1.SubCodeName));
		rData.Data2.Analysis=Data2.Analysis;
		rData.Data2.Design=Data2.Design;
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Data2.SubCodeName, rData.Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
		rData.CRC = CRC;
		rData.bServCheck = bServCheck;
		rData.dShortTerm = dShortTerm;
		rData.dLongTerm  = dLongTerm;
	}
};

struct T_MATD_D_CH_830
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_830 Data1;
	T_MATD_ALL_CH_830 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check
	
	char Rebar_CodeName2[16];      // v821추가 PSC Composite의 슬래브
	char MainRebar_RebarName2[16]; // v821추가 PSC Composite의 슬래브
	char SubRebar_RebarName2[16];  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브

	void ConvertToString(T_MATD_D_830& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		rData.Data1.Design=Data1.Design;
		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertCharStr(Data1.SubCodeName, rData.Data1.SubCodeName, sizeof(Data1.SubCodeName));
		rData.Data2.Analysis=Data2.Analysis;
		rData.Data2.Design=Data2.Design;
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Data2.SubCodeName, rData.Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
		rData.CRC = CRC;
		rData.bServCheck = bServCheck;
		rData.dShortTerm = dShortTerm;
		rData.dLongTerm  = dLongTerm;


		ConvertCharStr(Rebar_CodeName2,      rData.Rebar_CodeName2,      sizeof(Rebar_CodeName2));      // v821추가 PSC Composite의 슬래브
		ConvertCharStr(MainRebar_RebarName2, rData.MainRebar_RebarName2, sizeof(MainRebar_RebarName2));      // v821추가 PSC Composite의 슬래브
		ConvertCharStr(SubRebar_RebarName2,  rData.SubRebar_RebarName2,  sizeof(SubRebar_RebarName2));      // v821추가 PSC Composite의 슬래브
		rData.MainRebarData2       = MainRebarData2;  // v821추가 PSC Composite의 슬래브
		rData.SubRebarData2        = SubRebarData2;   // v821추가 PSC Composite의 슬래브
	}
};

struct T_MATD_D_CH_835
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_835 Data1;
	T_MATD_ALL_CH_835 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check
	
	char Rebar_CodeName2[16];      // v821추가 PSC Composite의 슬래브
	char MainRebar_RebarName2[16]; // v821추가 PSC Composite의 슬래브
	char SubRebar_RebarName2[16];  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브
	
	void ConvertToString(T_MATD_D_835& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		Data1.Design.ConvertToString(rData.Data1.Design);
		
		rData.Data1.bHybridFactor = Data1.bHybridFactor;
		rData.Data1.AnalysisTopFlange = Data1.AnalysisTopFlange;
		rData.Data1.AnalysisBotFlange = Data1.AnalysisBotFlange;
		rData.Data1.AnalysisWeb       = Data1.AnalysisWeb;
		
		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertCharStr(Data1.SubCodeName, rData.Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToString(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToString(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToString(rData.Data1.DesignWeb);
		
		
		rData.Data2.Analysis=Data2.Analysis;
		Data2.Design.ConvertToString(rData.Data2.Design);
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Data2.SubCodeName, rData.Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
		rData.CRC = CRC;
		rData.bServCheck = bServCheck;
		rData.dShortTerm = dShortTerm;
		rData.dLongTerm  = dLongTerm;
		
		ConvertCharStr(Rebar_CodeName2, rData.Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertCharStr(MainRebar_RebarName2, rData.MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertCharStr(SubRebar_RebarName2, rData.SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		rData.MainRebarData2=MainRebarData2;
		rData.SubRebarData2=SubRebarData2;
	}
};

struct T_MATD_D_CH_845
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_845 Data1;
	T_MATD_ALL_CH_845 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	char Rebar_CodeName2[16];      // v821추가 PSC Composite의 슬래브
	char MainRebar_RebarName2[16]; // v821추가 PSC Composite의 슬래브
	char SubRebar_RebarName2[16];  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브

	void ConvertToString(T_MATD_D_845& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		Data1.Design.ConvertToString(rData.Data1.Design);

		rData.Data1.bHybridFactor = Data1.bHybridFactor;
		rData.Data1.AnalysisTopFlange = Data1.AnalysisTopFlange;
		rData.Data1.AnalysisBotFlange = Data1.AnalysisBotFlange;
		rData.Data1.AnalysisWeb       = Data1.AnalysisWeb;

		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertCharStr(Data1.SubCodeName, rData.Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToString(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToString(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToString(rData.Data1.DesignWeb);


		rData.Data2.Analysis=Data2.Analysis;
		Data2.Design.ConvertToString(rData.Data2.Design);
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Data2.SubCodeName, rData.Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
		rData.CRC = CRC;
		rData.bServCheck = bServCheck;
		rData.dShortTerm = dShortTerm;
		rData.dLongTerm  = dLongTerm;

		ConvertCharStr(Rebar_CodeName2, rData.Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertCharStr(MainRebar_RebarName2, rData.MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertCharStr(SubRebar_RebarName2, rData.SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		rData.MainRebarData2=MainRebarData2;
		rData.SubRebarData2=SubRebarData2;
	}
};

struct T_MATD_D_CH_896
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_896 Data1;
	T_MATD_ALL_CH_896 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	char Rebar_CodeName2[16];      // v821추가 PSC Composite의 슬래브
	char MainRebar_RebarName2[16]; // v821추가 PSC Composite의 슬래브
	char SubRebar_RebarName2[16];  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브

	void ConvertToString(T_MATD_D_896& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		Data1.Design.ConvertToString(rData.Data1.Design);

		rData.Data1.bHybridFactor = Data1.bHybridFactor;
		rData.Data1.AnalysisTopFlange = Data1.AnalysisTopFlange;
		rData.Data1.AnalysisBotFlange = Data1.AnalysisBotFlange;
		rData.Data1.AnalysisWeb       = Data1.AnalysisWeb;

		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertCharStr(Data1.SubCodeName, rData.Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToString(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToString(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToString(rData.Data1.DesignWeb);


		rData.Data2.Analysis=Data2.Analysis;
		Data2.Design.ConvertToString(rData.Data2.Design);
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Data2.SubCodeName, rData.Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
		rData.CRC = CRC;
		rData.bServCheck = bServCheck;
		rData.dShortTerm = dShortTerm;
		rData.dLongTerm  = dLongTerm;

		ConvertCharStr(Rebar_CodeName2, rData.Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertCharStr(MainRebar_RebarName2, rData.MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertCharStr(SubRebar_RebarName2, rData.SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		rData.MainRebarData2=MainRebarData2;
		rData.SubRebarData2=SubRebarData2;

	}
	void ConvertToChar(T_MATD_D_896& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis=rData.Data1.Analysis;
		Data1.Design.ConvertToChar(rData.Data1.Design);
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertStrChar(rData.Data1.SubCodeName, Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToChar(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToChar(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToChar(rData.Data1.DesignWeb);

		Data1.bHybridFactor     = rData.Data1.bHybridFactor;
		Data1.AnalysisTopFlange = rData.Data1.AnalysisTopFlange;
		Data1.AnalysisBotFlange = rData.Data1.AnalysisBotFlange;
		Data1.AnalysisWeb       = rData.Data1.AnalysisWeb;


		Data2.Analysis=rData.Data2.Analysis;
		Data2.Design.ConvertToChar(rData.Data2.Design);
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Data2.SubCodeName, Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertStrChar(rData.Rebar_CodeName, Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertStrChar(rData.MainRebar_RebarName, MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertStrChar(rData.SubRebar_RebarName, SubRebar_RebarName, sizeof(SubRebar_RebarName));
		MainRebarData=rData.MainRebarData;
		SubRebarData=rData.SubRebarData;
		bChangeSteel=rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm  = rData.dLongTerm;

		ConvertStrChar(rData.Rebar_CodeName2, Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertStrChar(rData.MainRebar_RebarName2, MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertStrChar(rData.SubRebar_RebarName2, SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		MainRebarData2=rData.MainRebarData2;
		SubRebarData2=rData.SubRebarData2;

	}
};
struct T_MATD_D_CH_900_TEMP
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_900_TEMP Data1;
	T_MATD_ALL_CH_900_TEMP Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	char Rebar_CodeName2[16];      // v821추가 PSC Composite의 슬래브
	char MainRebar_RebarName2[16]; // v821추가 PSC Composite의 슬래브
	char SubRebar_RebarName2[16];  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브

	void ConvertToString(T_MATD_D_900_TEMP& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		Data1.Design.ConvertToString(rData.Data1.Design);

		rData.Data1.bHybridFactor = Data1.bHybridFactor;
		rData.Data1.AnalysisTopFlange = Data1.AnalysisTopFlange;
		rData.Data1.AnalysisBotFlange = Data1.AnalysisBotFlange;
		rData.Data1.AnalysisWeb       = Data1.AnalysisWeb;

		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertCharStr(Data1.SubCodeName, rData.Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToString(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToString(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToString(rData.Data1.DesignWeb);


		rData.Data2.Analysis=Data2.Analysis;
		Data2.Design.ConvertToString(rData.Data2.Design);
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Data2.SubCodeName, rData.Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
		rData.CRC = CRC;
		rData.bServCheck = bServCheck;
		rData.dShortTerm = dShortTerm;
		rData.dLongTerm  = dLongTerm;

		ConvertCharStr(Rebar_CodeName2, rData.Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertCharStr(MainRebar_RebarName2, rData.MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertCharStr(SubRebar_RebarName2, rData.SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		rData.MainRebarData2=MainRebarData2;
		rData.SubRebarData2=SubRebarData2;

	}
	void ConvertToChar(T_MATD_D_900_TEMP& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis=rData.Data1.Analysis;
		Data1.Design.ConvertToChar(rData.Data1.Design);
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertStrChar(rData.Data1.SubCodeName, Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToChar(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToChar(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToChar(rData.Data1.DesignWeb);

		Data1.bHybridFactor     = rData.Data1.bHybridFactor;
		Data1.AnalysisTopFlange = rData.Data1.AnalysisTopFlange;
		Data1.AnalysisBotFlange = rData.Data1.AnalysisBotFlange;
		Data1.AnalysisWeb       = rData.Data1.AnalysisWeb;


		Data2.Analysis=rData.Data2.Analysis;
		Data2.Design.ConvertToChar(rData.Data2.Design);
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Data2.SubCodeName, Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertStrChar(rData.Rebar_CodeName, Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertStrChar(rData.MainRebar_RebarName, MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertStrChar(rData.SubRebar_RebarName, SubRebar_RebarName, sizeof(SubRebar_RebarName));
		MainRebarData=rData.MainRebarData;
		SubRebarData=rData.SubRebarData;
		bChangeSteel=rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm  = rData.dLongTerm;

		ConvertStrChar(rData.Rebar_CodeName2, Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertStrChar(rData.MainRebar_RebarName2, MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertStrChar(rData.SubRebar_RebarName2, SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		MainRebarData2=rData.MainRebarData2;
		SubRebarData2=rData.SubRebarData2;

	}
};
struct T_MATD_D_CH_925
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_925 Data1;
	T_MATD_ALL_CH_925 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	char Rebar_CodeName2[16];      // v821추가 PSC Composite의 슬래브
	char MainRebar_RebarName2[16]; // v821추가 PSC Composite의 슬래브
	char SubRebar_RebarName2[16];  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브

	void ConvertToString(T_MATD_D_925& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis = Data1.Analysis;
		Data1.Design.ConvertToString(rData.Data1.Design);

		rData.Data1.bHybridFactor = Data1.bHybridFactor;
		rData.Data1.AnalysisTopFlange = Data1.AnalysisTopFlange;
		rData.Data1.AnalysisBotFlange = Data1.AnalysisBotFlange;
		rData.Data1.AnalysisWeb = Data1.AnalysisWeb;

		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertCharStr(Data1.SubCodeName, rData.Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToString(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToString(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToString(rData.Data1.DesignWeb);


		rData.Data2.Analysis = Data2.Analysis;
		Data2.Design.ConvertToString(rData.Data2.Design);
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Data2.SubCodeName, rData.Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData = MainRebarData;
		rData.SubRebarData = SubRebarData;
		rData.bChangeSteel = bChangeSteel;
		rData.CRC = CRC;
		rData.bServCheck = bServCheck;
		rData.dShortTerm = dShortTerm;
		rData.dLongTerm = dLongTerm;

		ConvertCharStr(Rebar_CodeName2, rData.Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertCharStr(MainRebar_RebarName2, rData.MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertCharStr(SubRebar_RebarName2, rData.SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		rData.MainRebarData2 = MainRebarData2;
		rData.SubRebarData2 = SubRebarData2;

	}
	void ConvertToChar(T_MATD_D_925& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis = rData.Data1.Analysis;
		Data1.Design.ConvertToChar(rData.Data1.Design);
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertStrChar(rData.Data1.SubCodeName, Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToChar(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToChar(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToChar(rData.Data1.DesignWeb);

		Data1.bHybridFactor = rData.Data1.bHybridFactor;
		Data1.AnalysisTopFlange = rData.Data1.AnalysisTopFlange;
		Data1.AnalysisBotFlange = rData.Data1.AnalysisBotFlange;
		Data1.AnalysisWeb = rData.Data1.AnalysisWeb;


		Data2.Analysis = rData.Data2.Analysis;
		Data2.Design.ConvertToChar(rData.Data2.Design);
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Data2.SubCodeName, Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertStrChar(rData.Rebar_CodeName, Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertStrChar(rData.MainRebar_RebarName, MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertStrChar(rData.SubRebar_RebarName, SubRebar_RebarName, sizeof(SubRebar_RebarName));
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm = rData.dLongTerm;

		ConvertStrChar(rData.Rebar_CodeName2, Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertStrChar(rData.MainRebar_RebarName2, MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertStrChar(rData.SubRebar_RebarName2, SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		MainRebarData2 = rData.MainRebarData2;
		SubRebarData2 = rData.SubRebarData2;

	}
};
struct T_MATD_D_CH_945
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_945 Data1;
	T_MATD_ALL_CH_945 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	char Rebar_CodeName2[16];      // v821추가 PSC Composite의 슬래브
	char MainRebar_RebarName2[16]; // v821추가 PSC Composite의 슬래브
	char SubRebar_RebarName2[16];  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브

	void ConvertToString(T_MATD_D_945& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis = Data1.Analysis;
		Data1.Design.ConvertToString(rData.Data1.Design);

		rData.Data1.bHybridFactor = Data1.bHybridFactor;
		rData.Data1.AnalysisTopFlange = Data1.AnalysisTopFlange;
		rData.Data1.AnalysisBotFlange = Data1.AnalysisBotFlange;
		rData.Data1.AnalysisWeb = Data1.AnalysisWeb;

		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertCharStr(Data1.SubCodeName, rData.Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToString(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToString(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToString(rData.Data1.DesignWeb);


		rData.Data2.Analysis = Data2.Analysis;
		Data2.Design.ConvertToString(rData.Data2.Design);
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Data2.SubCodeName, rData.Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData = MainRebarData;
		rData.SubRebarData = SubRebarData;
		rData.bChangeSteel = bChangeSteel;
		rData.CRC = CRC;
		rData.bServCheck = bServCheck;
		rData.dShortTerm = dShortTerm;
		rData.dLongTerm = dLongTerm;

		ConvertCharStr(Rebar_CodeName2, rData.Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertCharStr(MainRebar_RebarName2, rData.MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertCharStr(SubRebar_RebarName2, rData.SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		rData.MainRebarData2 = MainRebarData2;
		rData.SubRebarData2 = SubRebarData2;

	}
	void ConvertToChar(T_MATD_D_945& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis = rData.Data1.Analysis;
		Data1.Design.ConvertToChar(rData.Data1.Design);
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertStrChar(rData.Data1.SubCodeName, Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToChar(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToChar(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToChar(rData.Data1.DesignWeb);

		Data1.bHybridFactor = rData.Data1.bHybridFactor;
		Data1.AnalysisTopFlange = rData.Data1.AnalysisTopFlange;
		Data1.AnalysisBotFlange = rData.Data1.AnalysisBotFlange;
		Data1.AnalysisWeb = rData.Data1.AnalysisWeb;


		Data2.Analysis = rData.Data2.Analysis;
		Data2.Design.ConvertToChar(rData.Data2.Design);
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Data2.SubCodeName, Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertStrChar(rData.Rebar_CodeName, Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertStrChar(rData.MainRebar_RebarName, MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertStrChar(rData.SubRebar_RebarName, SubRebar_RebarName, sizeof(SubRebar_RebarName));
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm = rData.dLongTerm;

		ConvertStrChar(rData.Rebar_CodeName2, Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertStrChar(rData.MainRebar_RebarName2, MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertStrChar(rData.SubRebar_RebarName2, SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		MainRebarData2 = rData.MainRebarData2;
		SubRebarData2 = rData.SubRebarData2;

	}
};
struct T_MATD_D_CH_950
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_950 Data1;
	T_MATD_ALL_CH_950 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	char Rebar_CodeName2[16];      // v821추가 PSC Composite의 슬래브
	char MainRebar_RebarName2[16]; // v821추가 PSC Composite의 슬래브
	char SubRebar_RebarName2[16];  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브

	void ConvertToString(T_MATD_D_950& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis = Data1.Analysis;
		Data1.Design.ConvertToString(rData.Data1.Design);

		rData.Data1.bHybridFactor = Data1.bHybridFactor;
		rData.Data1.AnalysisTopFlange = Data1.AnalysisTopFlange;
		rData.Data1.AnalysisBotFlange = Data1.AnalysisBotFlange;
		rData.Data1.AnalysisWeb = Data1.AnalysisWeb;

		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertCharStr(Data1.SubCodeName, rData.Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToString(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToString(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToString(rData.Data1.DesignWeb);


		rData.Data2.Analysis = Data2.Analysis;
		Data2.Design.ConvertToString(rData.Data2.Design);
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Data2.SubCodeName, rData.Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData = MainRebarData;
		rData.SubRebarData = SubRebarData;
		rData.bChangeSteel = bChangeSteel;
		rData.CRC = CRC;
		rData.bServCheck = bServCheck;
		rData.dShortTerm = dShortTerm;
		rData.dLongTerm = dLongTerm;

		ConvertCharStr(Rebar_CodeName2, rData.Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertCharStr(MainRebar_RebarName2, rData.MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertCharStr(SubRebar_RebarName2, rData.SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		rData.MainRebarData2 = MainRebarData2;
		rData.SubRebarData2 = SubRebarData2;

	}
	void ConvertToChar(T_MATD_D_950& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis = rData.Data1.Analysis;
		Data1.Design.ConvertToChar(rData.Data1.Design);
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertStrChar(rData.Data1.SubCodeName, Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToChar(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToChar(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToChar(rData.Data1.DesignWeb);

		Data1.bHybridFactor = rData.Data1.bHybridFactor;
		Data1.AnalysisTopFlange = rData.Data1.AnalysisTopFlange;
		Data1.AnalysisBotFlange = rData.Data1.AnalysisBotFlange;
		Data1.AnalysisWeb = rData.Data1.AnalysisWeb;


		Data2.Analysis = rData.Data2.Analysis;
		Data2.Design.ConvertToChar(rData.Data2.Design);
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Data2.SubCodeName, Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertStrChar(rData.Rebar_CodeName, Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertStrChar(rData.MainRebar_RebarName, MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertStrChar(rData.SubRebar_RebarName, SubRebar_RebarName, sizeof(SubRebar_RebarName));
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm = rData.dLongTerm;

		ConvertStrChar(rData.Rebar_CodeName2, Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertStrChar(rData.MainRebar_RebarName2, MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertStrChar(rData.SubRebar_RebarName2, SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		MainRebarData2 = rData.MainRebarData2;
		SubRebarData2 = rData.SubRebarData2;

	}
};
struct T_MATD_D_CH_955
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_955 Data1;
	T_MATD_ALL_CH_955 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	char Rebar_CodeName2[16];      // v821추가 PSC Composite의 슬래브
	char MainRebar_RebarName2[16]; // v821추가 PSC Composite의 슬래브
	char SubRebar_RebarName2[16];  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브

	void ConvertToString(T_MATD_D_955& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis = Data1.Analysis;
		Data1.Design.ConvertToString(rData.Data1.Design);

		rData.Data1.bHybridFactor = Data1.bHybridFactor;
		rData.Data1.AnalysisTopFlange = Data1.AnalysisTopFlange;
		rData.Data1.AnalysisBotFlange = Data1.AnalysisBotFlange;
		rData.Data1.AnalysisWeb = Data1.AnalysisWeb;

		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertCharStr(Data1.SubCodeName, rData.Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToString(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToString(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToString(rData.Data1.DesignWeb);
		rData.Data1.bWithoutPT = Data1.bWithoutPT;

		rData.Data2.Analysis = Data2.Analysis;
		Data2.Design.ConvertToString(rData.Data2.Design);
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Data2.SubCodeName, rData.Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData = MainRebarData;
		rData.SubRebarData = SubRebarData;
		rData.bChangeSteel = bChangeSteel;
		rData.CRC = CRC;
		rData.bServCheck = bServCheck;
		rData.dShortTerm = dShortTerm;
		rData.dLongTerm = dLongTerm;

		ConvertCharStr(Rebar_CodeName2, rData.Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertCharStr(MainRebar_RebarName2, rData.MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertCharStr(SubRebar_RebarName2, rData.SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		rData.MainRebarData2 = MainRebarData2;
		rData.SubRebarData2 = SubRebarData2;

	}
	void ConvertToChar(T_MATD_D_955& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis = rData.Data1.Analysis;
		Data1.Design.ConvertToChar(rData.Data1.Design);
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertStrChar(rData.Data1.SubCodeName, Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToChar(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToChar(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToChar(rData.Data1.DesignWeb);

		Data1.bHybridFactor = rData.Data1.bHybridFactor;
		Data1.AnalysisTopFlange = rData.Data1.AnalysisTopFlange;
		Data1.AnalysisBotFlange = rData.Data1.AnalysisBotFlange;
		Data1.AnalysisWeb = rData.Data1.AnalysisWeb;
		Data1.bWithoutPT = rData.Data1.bWithoutPT;

		Data2.Analysis = rData.Data2.Analysis;
		Data2.Design.ConvertToChar(rData.Data2.Design);
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Data2.SubCodeName, Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertStrChar(rData.Rebar_CodeName, Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertStrChar(rData.MainRebar_RebarName, MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertStrChar(rData.SubRebar_RebarName, SubRebar_RebarName, sizeof(SubRebar_RebarName));
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm = rData.dLongTerm;

		ConvertStrChar(rData.Rebar_CodeName2, Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertStrChar(rData.MainRebar_RebarName2, MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertStrChar(rData.SubRebar_RebarName2, SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		MainRebarData2 = rData.MainRebarData2;
		SubRebarData2 = rData.SubRebarData2;

	}
};
struct T_MATD_D_CH_960
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH_960 Data1;
	T_MATD_ALL_CH_960 Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	char Rebar_CodeName2[16];      // v821추가 PSC Composite의 슬래브
	char MainRebar_RebarName2[16]; // v821추가 PSC Composite의 슬래브
	char SubRebar_RebarName2[16];  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브

	void ConvertToString(T_MATD_D_960& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis = Data1.Analysis;
		Data1.Design.ConvertToString(rData.Data1.Design);

		rData.Data1.bHybridFactor = Data1.bHybridFactor;
		rData.Data1.AnalysisTopFlange = Data1.AnalysisTopFlange;
		rData.Data1.AnalysisBotFlange = Data1.AnalysisBotFlange;
		rData.Data1.AnalysisWeb = Data1.AnalysisWeb;

		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertCharStr(Data1.SubCodeName, rData.Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToString(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToString(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToString(rData.Data1.DesignWeb);
		rData.Data1.bWithoutPT = Data1.bWithoutPT;

		rData.Data2.Analysis = Data2.Analysis;
		Data2.Design.ConvertToString(rData.Data2.Design);
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Data2.SubCodeName, rData.Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData = MainRebarData;
		rData.SubRebarData = SubRebarData;
		rData.bChangeSteel = bChangeSteel;
		rData.CRC = CRC;
		rData.bServCheck = bServCheck;
		rData.dShortTerm = dShortTerm;
		rData.dLongTerm = dLongTerm;

		ConvertCharStr(Rebar_CodeName2, rData.Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertCharStr(MainRebar_RebarName2, rData.MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertCharStr(SubRebar_RebarName2, rData.SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		rData.MainRebarData2 = MainRebarData2;
		rData.SubRebarData2 = SubRebarData2;
	}
	void ConvertToChar(T_MATD_D_960& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis = rData.Data1.Analysis;
		Data1.Design.ConvertToChar(rData.Data1.Design);
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertStrChar(rData.Data1.SubCodeName, Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToChar(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToChar(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToChar(rData.Data1.DesignWeb);

		Data1.bHybridFactor = rData.Data1.bHybridFactor;
		Data1.AnalysisTopFlange = rData.Data1.AnalysisTopFlange;
		Data1.AnalysisBotFlange = rData.Data1.AnalysisBotFlange;
		Data1.AnalysisWeb = rData.Data1.AnalysisWeb;
		Data1.bWithoutPT = rData.Data1.bWithoutPT;

		Data2.Analysis = rData.Data2.Analysis;
		Data2.Design.ConvertToChar(rData.Data2.Design);
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Data2.SubCodeName, Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertStrChar(rData.Rebar_CodeName, Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertStrChar(rData.MainRebar_RebarName, MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertStrChar(rData.SubRebar_RebarName, SubRebar_RebarName, sizeof(SubRebar_RebarName));
		MainRebarData = rData.MainRebarData;
		SubRebarData = rData.SubRebarData;
		bChangeSteel = rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm = rData.dLongTerm;

		ConvertStrChar(rData.Rebar_CodeName2, Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertStrChar(rData.MainRebar_RebarName2, MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertStrChar(rData.SubRebar_RebarName2, SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		MainRebarData2 = rData.MainRebarData2;
		SubRebarData2 = rData.SubRebarData2;

	}
};
struct T_MATD_D_CH
{
	char Type[4];
	char Name[16];
	T_MATD_ALL_CH Data1;
	T_MATD_ALL_CH Data2;
	char Rebar_CodeName[16];
	char MainRebar_RebarName[16];
	char SubRebar_RebarName[16];
	T_MATL_REBAR MainRebarData;
	T_MATL_REBAR SubRebarData;
	BOOL bChangeSteel;
	CRC_T_MATD_D CRC;
	BOOL   bServCheck; // v720추가, EC2 사용성 평가, Stress check
	double dShortTerm; // v720추가, EC2 사용성 평가, Stress check
	double dLongTerm;  // v720추가, EC2 사용성 평가, Stress check

	char Rebar_CodeName2[16];      // v821추가 PSC Composite의 슬래브
	char MainRebar_RebarName2[16]; // v821추가 PSC Composite의 슬래브
	char SubRebar_RebarName2[16];  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR MainRebarData2;  // v821추가 PSC Composite의 슬래브
	T_MATL_REBAR SubRebarData2;   // v821추가 PSC Composite의 슬래브

	void ConvertToString(T_MATD_D& rData)
	{
		ConvertCharStr(Type, rData.Type, sizeof(Type));
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.Data1.Analysis=Data1.Analysis;
		Data1.Design.ConvertToString(rData.Data1.Design);

		rData.Data1.bHybridFactor = Data1.bHybridFactor;
		rData.Data1.AnalysisTopFlange = Data1.AnalysisTopFlange;
		rData.Data1.AnalysisBotFlange = Data1.AnalysisBotFlange;
		rData.Data1.AnalysisWeb       = Data1.AnalysisWeb;

		ConvertCharStr(Data1.CodeName, rData.Data1.CodeName, sizeof(Data1.CodeName));
		ConvertCharStr(Data1.CodeMatlName, rData.Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertCharStr(Data1.SubCodeName, rData.Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToString(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToString(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToString(rData.Data1.DesignWeb);
		rData.Data1.bWithoutPT = Data1.bWithoutPT;

		rData.Data2.Analysis=Data2.Analysis;
		Data2.Design.ConvertToString(rData.Data2.Design);
		ConvertCharStr(Data2.CodeName, rData.Data2.CodeName, sizeof(Data2.CodeName));
		ConvertCharStr(Data2.CodeMatlName, rData.Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertCharStr(Data2.SubCodeName, rData.Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertCharStr(Rebar_CodeName, rData.Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertCharStr(MainRebar_RebarName, rData.MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertCharStr(SubRebar_RebarName, rData.SubRebar_RebarName, sizeof(SubRebar_RebarName));
		rData.MainRebarData=MainRebarData;
		rData.SubRebarData=SubRebarData;
		rData.bChangeSteel=bChangeSteel;
		rData.CRC = CRC;
		rData.bServCheck = bServCheck;
		rData.dShortTerm = dShortTerm;
		rData.dLongTerm  = dLongTerm;

		ConvertCharStr(Rebar_CodeName2, rData.Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertCharStr(MainRebar_RebarName2, rData.MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertCharStr(SubRebar_RebarName2, rData.SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		rData.MainRebarData2=MainRebarData2;
		rData.SubRebarData2=SubRebarData2;

	}
	void ConvertToChar(T_MATD_D& rData)
	{
		ConvertStrChar(rData.Type, Type, sizeof(Type));
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		Data1.Analysis=rData.Data1.Analysis;
		Data1.Design.ConvertToChar(rData.Data1.Design);
		ConvertStrChar(rData.Data1.CodeName, Data1.CodeName, sizeof(Data1.CodeName));
		ConvertStrChar(rData.Data1.CodeMatlName, Data1.CodeMatlName, sizeof(Data1.CodeMatlName));
		ConvertStrChar(rData.Data1.SubCodeName, Data1.SubCodeName, sizeof(Data1.SubCodeName));
		//PMS:LRFD12 BSC-20131224
		Data1.DesignBotFlange.ConvertToChar(rData.Data1.DesignBotFlange);
		Data1.DesignTopFlange.ConvertToChar(rData.Data1.DesignTopFlange);
		Data1.DesignWeb.ConvertToChar(rData.Data1.DesignWeb);

		Data1.bHybridFactor     = rData.Data1.bHybridFactor;
		Data1.AnalysisTopFlange = rData.Data1.AnalysisTopFlange;
		Data1.AnalysisBotFlange = rData.Data1.AnalysisBotFlange;
		Data1.AnalysisWeb       = rData.Data1.AnalysisWeb;
		Data1.bWithoutPT		= rData.Data1.bWithoutPT;
		
		Data2.Analysis=rData.Data2.Analysis;
		Data2.Design.ConvertToChar(rData.Data2.Design);
		ConvertStrChar(rData.Data2.CodeName, Data2.CodeName, sizeof(Data2.CodeName));
		ConvertStrChar(rData.Data2.CodeMatlName, Data2.CodeMatlName, sizeof(Data2.CodeMatlName));
		ConvertStrChar(rData.Data2.SubCodeName, Data2.SubCodeName, sizeof(Data2.SubCodeName));
		ConvertStrChar(rData.Rebar_CodeName, Rebar_CodeName, sizeof(Rebar_CodeName));
		ConvertStrChar(rData.MainRebar_RebarName, MainRebar_RebarName, sizeof(MainRebar_RebarName));
		ConvertStrChar(rData.SubRebar_RebarName, SubRebar_RebarName, sizeof(SubRebar_RebarName));
		MainRebarData=rData.MainRebarData;
		SubRebarData=rData.SubRebarData;
		bChangeSteel=rData.bChangeSteel;
		CRC = rData.CRC;
		bServCheck = rData.bServCheck;
		dShortTerm = rData.dShortTerm;
		dLongTerm  = rData.dLongTerm;

		ConvertStrChar(rData.Rebar_CodeName2, Rebar_CodeName2, sizeof(Rebar_CodeName2));
		ConvertStrChar(rData.MainRebar_RebarName2, MainRebar_RebarName2, sizeof(MainRebar_RebarName2));
		ConvertStrChar(rData.SubRebar_RebarName2, SubRebar_RebarName2, sizeof(SubRebar_RebarName2));
		MainRebarData2=rData.MainRebarData2;
		SubRebarData2=rData.SubRebarData2;

	}
};

/* 필요 없을 듯
struct T_MATD_UDRD_D_422
{
	T_MATL_K key;
	T_MATD_D_422 data;
};
struct T_MATD_UDRD_D_550
{
	T_MATL_K key;
	T_MATD_D_550 data;
};
struct T_MATD_UDRD_D_560
{
	T_MATL_K key;
	T_MATD_D_560 data;
};
struct T_MATD_UDRD_D_580
{
	T_MATL_K key;
	T_MATD_D_580 data;
};
*/

struct T_MATD_UDRD_D
{
	T_MATL_K key;
	T_MATD_D data;
};

struct T_MATD_UDRD_D_CH_422
{
	T_MATL_K key;
	T_MATD_D_CH_422 data;
};
struct T_MATD_UDRD_D_CH_550
{
	T_MATL_K key;
	T_MATD_D_CH_550 data;
};
struct T_MATD_UDRD_D_CH_560
{
	T_MATL_K key;
	T_MATD_D_CH_560 data;
};
struct T_MATD_UDRD_D_CH_580
{
	T_MATL_K key;
	T_MATD_D_CH_580 data;
};
struct T_MATD_UDRD_D_CH_630
{
	T_MATL_K key;
	T_MATD_D_CH_630 data;
};
struct T_MATD_UDRD_D_CH_702
{
	T_MATL_K key;
	T_MATD_D_CH_702 data;
};
struct T_MATD_UDRD_D_CH_712
{
	T_MATL_K key;
	T_MATD_D_CH_712 data;
};
struct T_MATD_UDRD_D_CH_721
{
	T_MATL_K key;
	T_MATD_D_CH_721 data;
};
struct T_MATD_UDRD_D_CH_730
{
	T_MATL_K key;
	T_MATD_D_CH_730 data;
};
struct T_MATD_UDRD_D_CH_810
{
	T_MATL_K key;
	T_MATD_D_CH_810 data;
};
struct T_MATD_UDRD_D_CH_820
{
	T_MATL_K key;
	T_MATD_D_CH_820 data;
};
struct T_MATD_UDRD_D_CH_830
{
	T_MATL_K key;
	T_MATD_D_CH_830 data;
};
struct T_MATD_UDRD_D_CH_835
{
	T_MATL_K key;
	T_MATD_D_CH_835 data;
};
struct T_MATD_UDRD_D_CH_845
{
	T_MATL_K key;
	T_MATD_D_CH_845 data;
};
struct T_MATD_UDRD_D_CH_896
{
	T_MATL_K key;
	T_MATD_D_CH_896 data;
};
struct T_MATD_UDRD_D_CH_900_TEMP
{
	T_MATL_K key;
	T_MATD_D_CH_900_TEMP data;
};
struct T_MATD_UDRD_D_CH_925
{
	T_MATL_K key;
	T_MATD_D_CH_925 data;
};
struct T_MATD_UDRD_D_CH_945
{
	T_MATL_K key;
	T_MATD_D_CH_945 data;
};
struct T_MATD_UDRD_D_CH_950
{
	T_MATL_K key;
	T_MATD_D_CH_950 data;
};
struct T_MATD_UDRD_D_CH_955
{
	T_MATL_K key;
	T_MATD_D_CH_955 data;
};
struct T_MATD_UDRD_D_CH_960
{
	T_MATL_K key;
	T_MATD_D_CH_960 data;
};
struct T_MATD_UDRD_D_CH
{
	T_MATL_K key;
	T_MATD_D_CH data;
};
struct T_MATD_UNIT
{
	T_MATD_UNIT()
	{
		Type=D_UNITSYS_NONE;
		Name=D_UNITSYS_NONE;
		CodeName=D_UNITSYS_NONE;
		CodeMatlName=D_UNITSYS_NONE;
		Elast=D_UNITSYS_BASE_ELAST;
		Poisson=D_UNITSYS_NONE;
		Thermal=D_UNITSYS_BASE_THERMAL;
		Density=D_UNITSYS_BASE_DENSITY;
		MassDensity=D_UNITSYS_BASE_MASSDENSITY;
		S_Fu   =D_UNITSYS_BASE_STRESS;
		S_Fy1  =D_UNITSYS_BASE_STRESS;
		S_Fy2  =D_UNITSYS_BASE_STRESS;
		S_Fy3  =D_UNITSYS_BASE_STRESS;
		S_Fy4  =D_UNITSYS_BASE_STRESS;
		S_Fy5  =D_UNITSYS_BASE_STRESS;  // add by Seungjun (`06.05.29).
		S_Fy6  =D_UNITSYS_BASE_STRESS;
		S_Fy7  =D_UNITSYS_BASE_STRESS;
		S_Fy8  =D_UNITSYS_BASE_STRESS;
		S_Fy9  =D_UNITSYS_BASE_STRESS;
		S_Fy10 =D_UNITSYS_BASE_STRESS;		
		S_Aft  =D_UNITSYS_BASE_STRESS;
		S_Aft2 =D_UNITSYS_BASE_STRESS; // 7.3.0
		S_Aft3 =D_UNITSYS_BASE_STRESS; // 7.3.0
		S_Fy   =D_UNITSYS_BASE_STRESS;
		S_Afv  =D_UNITSYS_BASE_STRESS;
		S_Afv2 =D_UNITSYS_BASE_STRESS; // 7.3.0
		S_Afv3 =D_UNITSYS_BASE_STRESS; // 7.3.0
		C_fc   =D_UNITSYS_BASE_STRESS;
		C_fci  =D_UNITSYS_BASE_STRESS;
		dTransferFci = D_UNITSYS_BASE_STRESS;
		Rebar_CodeName=D_UNITSYS_NONE;
		MainRebar_RebarName=D_UNITSYS_NONE;
		SubRebar_RebarName=D_UNITSYS_NONE;
		B_fy     =D_UNITSYS_BASE_STRESS;
		B_Elast  =D_UNITSYS_BASE_ELAST;
		B_Density=D_UNITSYS_BASE_DENSITY;
		bChangeSteel=D_UNITSYS_NONE;
		dShortTerm = D_UNITSYS_NONE;
		dLongTerm  = D_UNITSYS_NONE;
		Stress = D_UNITSYS_BASE_STRESS;
		Length = D_UNITSYS_BASE_LENGTH;
		// CRC Unit : 자체 초기화
	}
	int Type;
	int Name;
	int CodeName;
	int CodeMatlName;
	int Elast;
	int Poisson;
	int Thermal;
	int Density;
	int MassDensity;
	int S_Fu;
	int S_Fy1;
	int S_Fy2;
	int S_Fy3;
	int S_Fy4;
	int S_Fy5;  // add by Seungjun (`06.05.29).
	int S_Fy6;
	int S_Fy7;
	int S_Fy8;
	int S_Fy9;
	int S_Fy10;
	int S_Aft;
	int S_Aft2; // 7.3.0
	int S_Aft3; // 7.3.0
	int S_Fy;
	int S_Afv;
	int S_Afv2; // 7.3.0
	int S_Afv3; // 7.3.0
	int C_fc;
	int C_fci;
	int dTransferFci;
	int Rebar_CodeName;
	int MainRebar_RebarName;
	int SubRebar_RebarName;
	int B_fy;
	int B_Elast;
	int B_Density;
	int bChangeSteel;
	int dShortTerm;
	int dLongTerm;
	CRC_T_MATD_RBAR_UNIT CRC;
	int Stress;
	int Length;
};
#define HASHSIZEMATL 11 
#define D_MATL_MINNUM       1       // 입력가능한 최소 재질번호
//#define D_MATL_MAXNUM    9999       // 입력가능한 최대 재질번호
#define D_MATL_MAXNUM    999999       // 입력가능한 최대 재질번호 (680부터)

#pragma pack(pop)

#endif