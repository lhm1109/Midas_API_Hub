#ifndef __DB_ST_DT_ITHA_H__
#define __DB_ST_DT_ITHA_H__

#define MIDAS_SDK

#pragma pack(push, 1)
#include "DB_ST_DT_POVR.h"

////////////////////////////////////////////////
////////////////////////////////////////////////

// Fiber Material Property
#define T_FIMP_K unsigned int

struct T_FIMP_CONC  // 사용되지 않음
{
	double dFc;
	double dE0;
	double dK;
	double dZ;
	double dEu;
	void Initialize()
	{
		dFc = 0.;
		dE0 = 0.;
		dK  = 1.;
		dZ  = 0.;
		dEu = 0.;
	}
};
struct T_FIMP_STL    // 사용되지 않음
{
	double dFy;
	double dR0;
	double dE;
	double dA1;
	double dA2;
	double dB;
	void Initialize()
	{
		dFy = 0.;
		dR0 = 20.;
		dE  = 0.;
		dA1 = 18.5;
		dA2 = 0.15;
		dB  = 0.;
	}
};
struct T_FIMP_CON_KENTPK_890
{
	double dFc;
	double dE0;
	double dK;
	double dZ;
	double dEu;
};
struct T_FIMP_CON_KENTPK_905
{
	double dFc;
	double dE0;
	double dK;
	double dZ;
	double dEu;
	double dPartialFact;
};
struct T_FIMP_CON_KENTPK
{
	double dFc;
	double dE0;
	double dK;
	double dZ;
	double dEu;
	double dPartialFact;
	int nEc1Method;	// 0: ec1, 1: Z
	double dEc1;
	int nStrAfter; // Strength after Ctirical Strain Property, 0=Assuem As Zero 1=Keep the Strength

	void Initialize()
	{
		dFc = 0.0;
		dE0 = 0.0;
		dK = 1.0;
		dZ = 0.0;
		dEu = 0.0;
		dPartialFact = 1.0;
		nEc1Method = 0;
		dEc1 = 0.0;
		nStrAfter = 1;
	}
	void Convert890(const T_FIMP_CON_KENTPK_890& src)
	{
		Initialize();
		dFc	= src.dFc;
		dE0	= src.dE0;
		dK	= src.dK;
		dZ	= src.dZ;
		dEu	= src.dEu;
		if (fabs(dZ) > 1.0E-12)
			dEc1 = 0.8/dZ + dE0;
	}
	void Convert905(const T_FIMP_CON_KENTPK_905& src)
	{
		Initialize();
		dFc = src.dFc;
		dE0 = src.dE0;
		dK = src.dK;
		dZ = src.dZ;
		dEu = src.dEu;
		dPartialFact = src.dPartialFact;
		if (fabs(dZ) > 1.0E-12)
			dEc1 = 0.8 / dZ + dE0;
	}
};
struct T_FIMP_CON_STANJP_890
{
	double dFc;
	double dEpeak;
};
struct T_FIMP_CON_STANJP_970
{
	double dFc;
	double dEpeak;
	double dPartialFact;
};
struct T_FIMP_CON_STANJP
{
	double dFc;
	double dEpeak;
	double dPartialFact;
	double dEcu;
	void Initialize()
	{
		dFc = 0.0;
		dEpeak = 0.002;
		dPartialFact = 1.0;
		dEcu = 0.0035;
	}
	void Convert970(const T_FIMP_CON_STANJP_970& src)
	{
		Initialize();
		dFc		= src.dFc;
		dEpeak	= src.dEpeak;
		dPartialFact	= src.dPartialFact;
	}
	void Convert890(const T_FIMP_CON_STANJP_890& src)
	{
		Initialize();
		dFc		= src.dFc;
		dEpeak	= src.dEpeak;
	}
};
struct T_FIMP_CON_GB1010
{
	double dFcr;
	double dFtr;
	double dEcr;  // epsilon c
	double dEtr;  // epsilon t
	double dEc;
	void Initialize()
	{
		dFcr = 0.;
		dFtr = 0.;
		dEcr = dEtr = 0.0007;    
		dEc  = 0.;
	}
};

struct T_FIMP_CON_GB1002
{
	double dFck;
	double dFtk;
	double dEc;  // epsilon c
	double dEt;  // epsilon t
	void Initialize()
	{
		dFck = 1.;
		dFtk = 0.;
		dEc = dEt = 0.002;    
	}
};
struct T_FIMP_CON_ROADJP
{
	int nEarthquakeType;   // 0=Type 1, 1=Type2  - H24 일때 사용하지 않는다.
	double dEc;
	double dSigmack;
	double dSigmasy;
	double dAlpha;
	double dSigmabt;
	double dAh;
	double ds;
	double dd;
	double dBeta;    
	int    nStrenthLimit; // 0=하강강성유지 1=강성 0으로 처리
	void Initialize()
	{
		nEarthquakeType = 0;
		dEc = 0.;
		dSigmack = 0.;
		dSigmasy = 0.;
		dAlpha = 1.0;
		dSigmabt = 0.;
		dAh = 0.;
		ds = 0.;
		dd = 0.;
		dBeta = 1.0;    
		nStrenthLimit = 0;
	}
};

struct T_FIMP_CON_NAGOYA
{
	double dSigmack;  
	double dEpsiloncc;  // Epsilon0
	double dEpsiloncu;  // Epsilinu
	double dK;
	double dEpsilont0;
	double dEpsilont1;
	double dEpsilontu;
	int nStrAfter; // Strength after Ctirical Strain Property, 0=Assuem As Zero 1=Keep the Strength

	void Initialize()
	{
		dSigmack = 0.;
		dEpsiloncc = 0.0023;
		dEpsiloncu = 0.011; 
		dK = 1.;
		dEpsilont0 = 1.0e-6;
		dEpsilont1 = 0.05;
		dEpsilontu = 0.1;
		nStrAfter = 0;
	}
};
struct T_FIMP_CON_TRILIN
{
	int    nMethod;    // 0:sigma-epsilin, 1:sigma-alpha
	double dSigmay[3];      // Sigmac
	double dEpsilont[3];    
	double dEpsilony[3];    // Epsilonc
	double dK[3];
	double dEpsiloncu;
	void Initialize()
	{
		nMethod = 0;
		for(int i = 0; i < 3; i++) dSigmay[i] = dEpsilony[i] = dK[i] = 0.;    
		dEpsilont[0] = 1.0e-6; 
		dEpsilont[1] = 0.05; 
		dEpsilont[2] = 0.1; 
		dEpsiloncu = 0.;
	}
};
struct T_FIMP_STL_MENEGO_890
{
	double dFy;
	double dR0;
	double dE;
	double dA1;
	double dA2;
	double dB;
};
struct T_FIMP_STL_MENEGO
{
	double dFy;
	double dR0;
	double dE;
	double dA1;
	double dA2;
	double dB;
	double dPartialFact;
	void Initialize()
	{
		dFy = 0.0;
		dR0 = 20.0;
		dE = 0.0;
		dA1 = 18.5;
		dA2 = 0.15;
		dB = 0.0;
		dPartialFact = 1.0;
	}
	void Convert890(const T_FIMP_STL_MENEGO_890& src)
	{
		Initialize();
		dFy	= src.dFy;
		dR0	= src.dR0;
		dE	= src.dE;
		dA1	= src.dA1;
		dA2	= src.dA2;
		dB	= src.dB;
	}
};
struct T_FIMP_STL_BILINE_890
{
	double dFy;
	double dE1;
	double dE2E1;
};
struct T_FIMP_STL_BILINE
{
	double dFy;
	double dE1;
	double dE2E1;
	double dPartialFact;
	void Initialize()
	{
		dFy = 0.0;
		dE1 = 0.0;
		dE2E1 = 0.01;
		dPartialFact = 1.0;
	}
	void Convert890(const T_FIMP_STL_BILINE_890& src)
	{
		Initialize();
		dFy		= src.dFy;
		dE1		= src.dE1;
		dE2E1	= src.dE2E1;
	}
};
struct T_FIMP_STL_GENBIL
{
	double dSigmay;
	double dSigmacy;
	double dEpsilon1;
	double dEpsilon2;
	double dE[5];
	void Initialize()
	{
		dSigmay = 0.;
		dSigmacy = 0.;
		dEpsilon1 = 0.;
		dEpsilon2 = 0.;
		dE[0] = dE[1] = dE[2] = dE[3] = dE[4] = 0.;    
	}
};
struct T_FIMP_STL_TRILIN
{
	int    nMethod;    // 0:sigma-epsilin, 1:sigma-alpha
	double dSigmay[6];
	double dEpsilony[6];
	double dK[5];
	void Initialize()
	{
		nMethod = 0;
		for(int i = 0; i < 6; i++) dSigmay[i] = dEpsilony[i] = 0.;
		for(int i = 0; i < 5; i++) dK[i] = 0.;    
	}
};
struct T_FIMP_STL_PARK
{
	double dFy;  // fy : 철근의 항복강도 [F]/[L^2]
	double dFu;  // fu : 철근의 극한강도 [F]/[L^2]
	double dEs;  // Es : 철근의 탄성계수 [F]/[L^2]
	double dEsh; // εsh : 철근의 경화시 변형률
	double dEsu; // εsu : 철근의 극한시 변형률
	void Initialize()
	{
		dFy = 0.0; //M_InitValueCurUnit(300.0, N, MM, D_UNITSYS_BASE_STRESS); // MPa
		dFu = 0.0; //M_InitValueCurUnit(420.0, N, MM, D_UNITSYS_BASE_STRESS); // MPa
		dEs = 0.0; //M_InitValueCurUnit(2.1E5, N, MM, D_UNITSYS_BASE_STRESS); // MPa
		dEsh = 0.0;
		dEsu = 0.0;
	}
};
struct T_FIMP_STL_ROADJP
{
	double dSigsy;  // Sigmasy : Yield Stress of Steel
	double dEs;     // Es : Elastic Modulus of Steel
	double dest;    // est : 축방향철근 허용인장 변형도
	
	void Initialize()
	{
		dSigsy = 0.0;
		dEs = 0.0;
		dest = 0.0;
	}
};

struct T_FIMP_STL_GB2010
{
	double dFyr;
	double dFstr;
	double dEs;  // epsilon c
	double dEy;  // epsilon t
	double dEuy;
	double dEu;
	double dK;
	void Initialize()
	{
		dFyr  = 0.;
		dFstr = 0.;
		dEs   = 0.;    
		dEy   = 0.;
		dEuy  = 0.;
		dEu   = 0.;    
		dK    = 0.;
	}
};

struct T_FIMP_CON_MANDER_800
{
	int     nConcType;                 // Concrete Type - 0:Unconfined 1:Confined
	BOOL    bConcDatafco;              // Unconfined Concrete Data - Concrete Strength (fco')
	double  dConcDatafco;              // Unconfined Concrete Data - Concrete Strength (fco')
	BOOL    bConcDataeco;              // Unconfined Concrete Data - Concrete Strain (eco)
	double  dConcDataeco;              // Unconfined Concrete Data - Concrete Strain (eco)
	int     nConcDataEc;               // Concrete Data - Elastic Modulus of Concrete (Ec)
	double  dConcDataEc;               // Concrete Data - Elastic Modulus of Concrete (Ec)
	int     nTensConcDataft;           // Tensile Strength of Concrete
	double  dTensConcDataft;           // Tensile Strength of Concrete
	double  dTensConcDataet;           // Tensile Strain of COncrete
	int     nSectRebarType;            // Section & Confinement Rebar Type - 0:Rectangular 1:Circular(Hoop Confinement Steel) 2:User Defined 3:Circular(Spiral Confinement Steel
	BOOL    bCoefficientAcc;           // Area of Effective Concrete Core (Acc)
	double  dCoefficientAcc;           // Area of Effective Concrete Core (Acc)
	BOOL    bCoefficientAe;            // Total area of ineffectively confined Core Concrete at the level of hoops (Ae)
	double  dCoefficientAe;            // Total area of ineffectively confined Core Concrete at the level of hoops (Ae)
	BOOL    bCoefficientke;            // Ae/Acc
	double  dCoefficientke;            // Ae/Acc
	BOOL    bfl;                       // The Effective Lateral confining stress on the Concrete (Circular) (f'l)
	double  dfl;                       // The Effective Lateral confining stress on the Concrete (Circular) (f'l)
	BOOL    bConfinedConcStrengthfcc;  // Strength of Confined Concrete (fcc)
	double  dConfinedConcStrengthfcc;  // Strength of Confined Concrete (fcc)
	BOOL    bConfinedConcStrainecc;    // Strain for Confined Concrete corresponding to fcc (ecc)
	double  dConfinedConcStrainecc;    // Strain for Confined Concrete corresponding to fcc (ecc)

	int     nRectSectAutoorUser;       // Section Data - 0:Auto 1:User (Rectangular)
	int     nCircSectAutoorUser;       // Section Data - 0:Auto 1:User (circular)
	double  dXdirectionbc;             // Section Data - Concrete Core Dimensions to Center lines of Perimeter Hoop in X-dir (Rectangular) (bc)
	double  dXdirectionwxi;            // Section Data - i-th Clear Transverse Spacing between adjacent Longitudinal Rebars in X-dir (Rectangular) (w'xi)
	int     nXdirectionn;              // Section Data - n (Rectangular)
	double  dYdirectiondc;             // Section Data - Concrete Core Dimensions to Center lines of Perimeter Hoop in Y-dir (Rectangular) (dc)
	double  dYdirectionwyj;            // Section Data - j-th Clear Transverse Spacing between adjacent Longitudinal Rebars in Y-dir (Rectangular) (w'yi)
	int     nYdirectionm;              // Section Data - m (Rectangular)
	double  dds;                       // Diameter of Spiral (Circular) (ds)

	int     nLongiRebarType;           // Longitudinal Rebar Type - 0:Rebar Number 1:Total Area of Rebars
	int     nLongiRebarNum;            // Longitudinal Rebar Data - Rebar Number
	char    strLongiRebarSize[16];     // Longitudinal Rebar Data - Rebar Size
	double  dLongiRebarArea;           // Longitudinal Rebar Data - Total Area of Rebars
	BOOL    bLongiRebarRatio;          // Longitudinal Rebar Data - Longitudinal Reinforcement Ratio (pcc)
	double  dLongiRebarRatio;          // Longitudinal Rebar Data - Longitudinal Reinforcement Ratio (pcc)
	int     nGeneralRebarType;         // General Confinement Rebar Type - 0:Rebar Size 1:Asp Rebar Area
	char    strGeneralRebarSize[16];   // General Confinement Rebar Data - Rebar Size
	double  dGeneralRebarArea;         // General Confinement Rebar Data - Rebar Area
	double  dHoopsSpaceS1;             // General Confinement Rebar Data - Hoops Space (s)
	double  dClearHoopsSpaceS2;        // General Confinement Rebar Data - Clear Hoops Space (s')
	int     nGeneralRebarNumX;         // General Confinement Rebar Data - Confinement Rebar Number X-dir
	int     nGeneralRebarNumY;         // General Confinement Rebar Data - Confinement Rebar Number Y-dir
	double  dGeneralAsx;               // General Confinement Rebar Data - Confinement Rebar Area X-dir (Asx)
	double  dGeneralAsy;               // General Confinement Rebar Data - Confinement Rebar Area Y-dir (Asy)

	BOOL    bRectStrengthfyh;          // Confinement Rebar Yield Strength (Rectangular) (fyh)
	double  dRectStrengthfyh;          // Confinement Rebar Yield Strength (Rectangular) (fyh)
	BOOL    bGeneralAsx;               // Total Area of Confinement Rebar (calculated in "Rebar Data") 
	BOOL    bGeneralAsy;               // Total Area of Confinement Rebar (calculated in "Rebar Data")    
	BOOL    bRectRatiopx;              // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Rectangular) (px)
	double  dRectRatiopx;              // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Rectangular) (px)
	BOOL    bRectRatiopy;              // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Rectangular) (py)
	double  dRectRatiopy;              // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Rectangular) (py)
	BOOL    bRectStressflx;            // The Effective lateral confining stress on the Concrete in the x,y Direction (Rectangular) (flx') 
	double  dRectStressflx;            // The Effective lateral confining stress on the Concrete in the x,y Direction (Rectangular) (flx') 
	BOOL    bRectStressfly;            // The Effective lateral confining stress on the Concrete in the x,y Direction (Rectangular) (fly') 
	double  dRectStressfly;            // The Effective lateral confining stress on the Concrete in the x,y Direction (Rectangular) (fly') 

	BOOL    bCircularStrengthfyh;      // Confinement Rebar Yield Strength (Circular) (fyh)
	double  dCircularStrengthfyh;      // Confinement Rebar Yield Strength (Circular) (fyh)
	BOOL    bGeneralRebarArea;         // Area of Confinement Rebar
	BOOL    bCircularRatiops;          // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Circular) (ps) 
	double  dCircularRatiops;          // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Circular) (ps) 
};

struct T_FIMP_CON_MANDER_881
{
	int     nConcType;                 // Concrete Type - 0:Unconfined 1:Confined
	BOOL    bConcDatafco;              // Unconfined Concrete Data - Concrete Strength (fco')
	double  dConcDatafco;              // Unconfined Concrete Data - Concrete Strength (fco')
	BOOL    bConcDataeco;              // Unconfined Concrete Data - Concrete Strain (eco)
	double  dConcDataeco;              // Unconfined Concrete Data - Concrete Strain (eco)
	int     nConcDataEc;               // Concrete Data - Elastic Modulus of Concrete (Ec)
	double  dConcDataEc;               // Concrete Data - Elastic Modulus of Concrete (Ec)
	int     nTensConcDataft;           // Tensile Strength of Concrete
	double  dTensConcDataft;           // Tensile Strength of Concrete
	double  dTensConcDataet;           // Tensile Strain of COncrete
	int     nSectRebarType;            // Section & Confinement Rebar Type - 0:Rectangular 1:Circular(Hoop Confinement Steel) 2:User Defined 3:Circular(Spiral Confinement Steel
	BOOL    bCoefficientAcc;           // Area of Effective Concrete Core (Acc)
	double  dCoefficientAcc;           // Area of Effective Concrete Core (Acc)
	BOOL    bCoefficientAe;            // Total area of ineffectively confined Core Concrete at the level of hoops (Ae)
	double  dCoefficientAe;            // Total area of ineffectively confined Core Concrete at the level of hoops (Ae)
	BOOL    bCoefficientke;            // Ae/Acc
	double  dCoefficientke;            // Ae/Acc
	BOOL    bfl;                       // The Effective Lateral confining stress on the Concrete (Circular) (f'l)
	double  dfl;                       // The Effective Lateral confining stress on the Concrete (Circular) (f'l)
	BOOL    bConfinedConcStrengthfcc;  // Strength of Confined Concrete (fcc)
	double  dConfinedConcStrengthfcc;  // Strength of Confined Concrete (fcc)
	BOOL    bConfinedConcStrainecc;    // Strain for Confined Concrete corresponding to fcc (ecc)
	double  dConfinedConcStrainecc;    // Strain for Confined Concrete corresponding to fcc (ecc)

	BOOL   bUseUltStrUlt             ; // TRUE: Enable Ultimate Strain for Concrete Group, FALSE: Disable
	BOOL   bConfinedConcStrUltPs     ; // Ultimate Strain for Confined Concrete - ps 
	double dConfinedConcStrUltPs     ; // Ultimate Strain for Confined Concrete - ps
	T_FIMP_K nSteelModelKey;           // Ultimate Strain for Confined Concrete - Steel Matrial
	BOOL   bConfinedConcStrUltEcu    ; // Ultimate Strain for Confined Concrete - eCU
	double dConfinedConcStrUltEcu    ; // Ultimate Strain for Confined Concrete - eCU

	int     nRectSectAutoorUser;       // Section Data - 0:Auto 1:User (Rectangular)
	int     nCircSectAutoorUser;       // Section Data - 0:Auto 1:User (circular)
	double  dXdirectionbc;             // Section Data - Concrete Core Dimensions to Center lines of Perimeter Hoop in X-dir (Rectangular) (bc)
	double  dXdirectionwxi;            // Section Data - i-th Clear Transverse Spacing between adjacent Longitudinal Rebars in X-dir (Rectangular) (w'xi)
	int     nXdirectionn;              // Section Data - n (Rectangular)
	double  dYdirectiondc;             // Section Data - Concrete Core Dimensions to Center lines of Perimeter Hoop in Y-dir (Rectangular) (dc)
	double  dYdirectionwyj;            // Section Data - j-th Clear Transverse Spacing between adjacent Longitudinal Rebars in Y-dir (Rectangular) (w'yi)
	int     nYdirectionm;              // Section Data - m (Rectangular)
	double  dds;                       // Diameter of Spiral (Circular) (ds)

	int     nLongiRebarType;           // Longitudinal Rebar Type - 0:Rebar Number 1:Total Area of Rebars
	int     nLongiRebarNum;            // Longitudinal Rebar Data - Rebar Number
	char    strLongiRebarSize[16];     // Longitudinal Rebar Data - Rebar Size
	double  dLongiRebarArea;           // Longitudinal Rebar Data - Total Area of Rebars
	BOOL    bLongiRebarRatio;          // Longitudinal Rebar Data - Longitudinal Reinforcement Ratio (pcc)
	double  dLongiRebarRatio;          // Longitudinal Rebar Data - Longitudinal Reinforcement Ratio (pcc)
	int     nGeneralRebarType;         // General Confinement Rebar Type - 0:Rebar Size 1:Asp Rebar Area
	char    strGeneralRebarSize[16];   // General Confinement Rebar Data - Rebar Size
	double  dGeneralRebarArea;         // General Confinement Rebar Data - Rebar Area
	double  dHoopsSpaceS1;             // General Confinement Rebar Data - Hoops Space (s)
	double  dClearHoopsSpaceS2;        // General Confinement Rebar Data - Clear Hoops Space (s')
	int     nGeneralRebarNumX;         // General Confinement Rebar Data - Confinement Rebar Number X-dir
	int     nGeneralRebarNumY;         // General Confinement Rebar Data - Confinement Rebar Number Y-dir
	double  dGeneralAsx;               // General Confinement Rebar Data - Confinement Rebar Area X-dir (Asx)
	double  dGeneralAsy;               // General Confinement Rebar Data - Confinement Rebar Area Y-dir (Asy)

	BOOL    bRectStrengthfyh;          // Confinement Rebar Yield Strength (Rectangular) (fyh)
	double  dRectStrengthfyh;          // Confinement Rebar Yield Strength (Rectangular) (fyh)
	BOOL    bGeneralAsx;               // Total Area of Confinement Rebar (calculated in "Rebar Data") 
	BOOL    bGeneralAsy;               // Total Area of Confinement Rebar (calculated in "Rebar Data")    
	BOOL    bRectRatiopx;              // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Rectangular) (px)
	double  dRectRatiopx;              // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Rectangular) (px)
	BOOL    bRectRatiopy;              // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Rectangular) (py)
	double  dRectRatiopy;              // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Rectangular) (py)
	BOOL    bRectStressflx;            // The Effective lateral confining stress on the Concrete in the x,y Direction (Rectangular) (flx') 
	double  dRectStressflx;            // The Effective lateral confining stress on the Concrete in the x,y Direction (Rectangular) (flx') 
	BOOL    bRectStressfly;            // The Effective lateral confining stress on the Concrete in the x,y Direction (Rectangular) (fly') 
	double  dRectStressfly;            // The Effective lateral confining stress on the Concrete in the x,y Direction (Rectangular) (fly') 

	BOOL    bCircularStrengthfyh;      // Confinement Rebar Yield Strength (Circular) (fyh)
	double  dCircularStrengthfyh;      // Confinement Rebar Yield Strength (Circular) (fyh)
	BOOL    bGeneralRebarArea;         // Area of Confinement Rebar
	BOOL    bCircularRatiops;          // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Circular) (ps) 
	double  dCircularRatiops;          // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Circular) (ps) 
};

struct T_FIMP_CON_MANDER
{
	int     nConcType;                 // Concrete Type - 0:Unconfined 1:Confined
	BOOL    bConcDatafco;              // Unconfined Concrete Data - Concrete Strength (fco')
	double  dConcDatafco;              // Unconfined Concrete Data - Concrete Strength (fco')
	BOOL    bConcDataeco;              // Unconfined Concrete Data - Concrete Strain (eco)
	double  dConcDataeco;              // Unconfined Concrete Data - Concrete Strain (eco)
	int     nConcDataEc;               // Concrete Data - Elastic Modulus of Concrete (Ec)
	double  dConcDataEc;               // Concrete Data - Elastic Modulus of Concrete (Ec)
	int     nTensConcDataft;           // Tensile Strength of Concrete
	double  dTensConcDataft;           // Tensile Strength of Concrete
	double  dTensConcDataet;           // Tensile Strain of COncrete
	int     nSectRebarType;            // Section & Confinement Rebar Type - 0:Rectangular 1:Circular(Hoop Confinement Steel) 2:User Defined 3:Circular(Spiral Confinement Steel
	BOOL    bCoefficientAcc;           // Area of Effective Concrete Core (Acc)
	double  dCoefficientAcc;           // Area of Effective Concrete Core (Acc)
	BOOL    bCoefficientAe;            // Total area of ineffectively confined Core Concrete at the level of hoops (Ae)
	double  dCoefficientAe;            // Total area of ineffectively confined Core Concrete at the level of hoops (Ae)
	BOOL    bCoefficientke;            // Ae/Acc
	double  dCoefficientke;            // Ae/Acc
	BOOL    bfl;                       // The Effective Lateral confining stress on the Concrete (Circular) (f'l)
	double  dfl;                       // The Effective Lateral confining stress on the Concrete (Circular) (f'l)
	BOOL    bConfinedConcStrengthfcc;  // Strength of Confined Concrete (fcc)
	double  dConfinedConcStrengthfcc;  // Strength of Confined Concrete (fcc)
	BOOL    bConfinedConcStrainecc;    // Strain for Confined Concrete corresponding to fcc (ecc)
	double  dConfinedConcStrainecc;    // Strain for Confined Concrete corresponding to fcc (ecc)

	BOOL   bUseUltStrUlt             ; // TRUE: Enable Ultimate Strain for Concrete Group, FALSE: Disable
	BOOL   bConfinedConcStrUltPs     ; // Ultimate Strain for Confined Concrete - ps 
	double dConfinedConcStrUltPs     ; // Ultimate Strain for Confined Concrete - ps
	T_FIMP_K nSteelModelKey;           // Ultimate Strain for Confined Concrete - Steel Matrial
	BOOL   bConfinedConcStrUltEcu    ; // Ultimate Strain for Confined Concrete - eCU
	double dConfinedConcStrUltEcu    ; // Ultimate Strain for Confined Concrete - eCU

	int     nRectSectAutoorUser;       // Section Data - 0:Auto 1:User (Rectangular)
	int     nCircSectAutoorUser;       // Section Data - 0:Auto 1:User (circular)
	double  dXdirectionbc;             // Section Data - Concrete Core Dimensions to Center lines of Perimeter Hoop in X-dir (Rectangular) (bc)
	double  dXdirectionwxi;            // Section Data - i-th Clear Transverse Spacing between adjacent Longitudinal Rebars in X-dir (Rectangular) (w'xi)
	int     nXdirectionn;              // Section Data - n (Rectangular)
	double  dYdirectiondc;             // Section Data - Concrete Core Dimensions to Center lines of Perimeter Hoop in Y-dir (Rectangular) (dc)
	double  dYdirectionwyj;            // Section Data - j-th Clear Transverse Spacing between adjacent Longitudinal Rebars in Y-dir (Rectangular) (w'yi)
	int     nYdirectionm;              // Section Data - m (Rectangular)
	double  dds;                       // Diameter of Spiral (Circular) (ds)

	int     nLongiRebarType;           // Longitudinal Rebar Type - 0:Rebar Number 1:Total Area of Rebars
	int     nLongiRebarNum;            // Longitudinal Rebar Data - Rebar Number
	char    strLongiRebarSize[16];     // Longitudinal Rebar Data - Rebar Size
	double  dLongiRebarArea;           // Longitudinal Rebar Data - Total Area of Rebars
	BOOL    bLongiRebarRatio;          // Longitudinal Rebar Data - Longitudinal Reinforcement Ratio (pcc)
	double  dLongiRebarRatio;          // Longitudinal Rebar Data - Longitudinal Reinforcement Ratio (pcc)
	int     nGeneralRebarType;         // General Confinement Rebar Type - 0:Rebar Size 1:Asp Rebar Area
	char    strGeneralRebarSize[16];   // General Confinement Rebar Data - Rebar Size
	double  dGeneralRebarArea;         // General Confinement Rebar Data - Rebar Area
	double  dHoopsSpaceS1;             // General Confinement Rebar Data - Hoops Space (s)
	double  dClearHoopsSpaceS2;        // General Confinement Rebar Data - Clear Hoops Space (s')
	int     nGeneralRebarNumX;         // General Confinement Rebar Data - Confinement Rebar Number X-dir
	int     nGeneralRebarNumY;         // General Confinement Rebar Data - Confinement Rebar Number Y-dir
	double  dGeneralAsx;               // General Confinement Rebar Data - Confinement Rebar Area X-dir (Asx)
	double  dGeneralAsy;               // General Confinement Rebar Data - Confinement Rebar Area Y-dir (Asy)

	BOOL    bRectStrengthfyh;          // Confinement Rebar Yield Strength (Rectangular) (fyh)
	double  dRectStrengthfyh;          // Confinement Rebar Yield Strength (Rectangular) (fyh)
	BOOL    bGeneralAsx;               // Total Area of Confinement Rebar (calculated in "Rebar Data") 
	BOOL    bGeneralAsy;               // Total Area of Confinement Rebar (calculated in "Rebar Data")    
	BOOL    bRectRatiopx;              // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Rectangular) (px)
	double  dRectRatiopx;              // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Rectangular) (px)
	BOOL    bRectRatiopy;              // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Rectangular) (py)
	double  dRectRatiopy;              // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Rectangular) (py)
	BOOL    bRectStressflx;            // The Effective lateral confining stress on the Concrete in the x,y Direction (Rectangular) (flx') 
	double  dRectStressflx;            // The Effective lateral confining stress on the Concrete in the x,y Direction (Rectangular) (flx') 
	BOOL    bRectStressfly;            // The Effective lateral confining stress on the Concrete in the x,y Direction (Rectangular) (fly') 
	double  dRectStressfly;            // The Effective lateral confining stress on the Concrete in the x,y Direction (Rectangular) (fly') 

	BOOL    bCircularStrengthfyh;      // Confinement Rebar Yield Strength (Circular) (fyh)
	double  dCircularStrengthfyh;      // Confinement Rebar Yield Strength (Circular) (fyh)
	BOOL    bGeneralRebarArea;         // Area of Confinement Rebar
	BOOL    bCircularRatiops;          // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Circular) (ps) 
	double  dCircularRatiops;          // Ratio of the volume of transverse confining Steel to the volume of confined concrete core (Circular) (ps) 

	BOOL    bConfinedConcStrainecy;    // Yield Strain for Confined Concrete (ecy)
	double  dConfinedConcStrainecy;    // Yield Strain for Confined Concrete (ecy)
	BOOL    bConcDataecy;              // Unconfined Concrete Data - Concrete Strain (ecy)
	double  dConcDataecy;              // Unconfined Concrete Data - Concrete Strain (ecy)
	BOOL    bConcDataesp;              // Unconfined Concrete Data - Concrete Strain (esp)
	double  dConcDataesp;              // Unconfined Concrete Data - Concrete Strain (esp)

	void Initialize()
	{
		nConcType                 = 1;
		bConcDatafco              = FALSE;
		dConcDatafco              = 0.;
		bConcDataeco              = FALSE;
		dConcDataeco              = 0.002;
		nConcDataEc               = 0;
		dConcDataEc               = 0.;
		nTensConcDataft           = 2;
		dTensConcDataft           = 0.;
		dTensConcDataet           = 0.;
		nSectRebarType            = 0;
		bCoefficientAcc           = FALSE;
		dCoefficientAcc           = 0.;
		bCoefficientAe            = FALSE;
		dCoefficientAe            = 0.;
		bCoefficientke            = FALSE;
		dCoefficientke            = 0.;
		bfl                       = FALSE;
		dfl                       = 0.;
		bConfinedConcStrengthfcc  = FALSE;
		dConfinedConcStrengthfcc  = 0.;
		bConfinedConcStrainecc    = FALSE;
		dConfinedConcStrainecc    = 0.;

		bUseUltStrUlt              = FALSE;
		bConfinedConcStrUltPs      = FALSE;
		dConfinedConcStrUltPs      = 0.;
		nSteelModelKey = 0;
		bConfinedConcStrUltEcu     = FALSE;
		dConfinedConcStrUltEcu     = 0.;

		nRectSectAutoorUser = 1;
		nCircSectAutoorUser = 1;
		dXdirectionbc     = 0.;
		dXdirectionwxi    = 0.;
		nXdirectionn      = 0;
		dYdirectiondc     = 0.;
		dYdirectionwyj    = 0.;
		nYdirectionm      = 0;
		dds               = 0.;

		nLongiRebarType    = 0;
		nLongiRebarNum     = 0;
		memset(strLongiRebarSize,' ',sizeof(strLongiRebarSize));
		dLongiRebarArea    = 0.;
		bLongiRebarRatio   = FALSE;          
		dLongiRebarRatio   = 0.;
		nGeneralRebarType  = 0;
		memset(strGeneralRebarSize,' ',sizeof(strGeneralRebarSize));
		dGeneralRebarArea  = 0.;
		dHoopsSpaceS1      = 0.;
		dClearHoopsSpaceS2 = 0.;
		nGeneralRebarNumX  = 0;
		nGeneralRebarNumY  = 0;
		dGeneralAsx        = 0.;
		dGeneralAsy        = 0.;

		bRectStrengthfyh = FALSE;    
		dRectStrengthfyh = 0.;
		bGeneralAsx      = FALSE;
		bGeneralAsy      = FALSE;
		bRectRatiopx     = FALSE;                  
		dRectRatiopx     = 0.;
		bRectRatiopy     = FALSE;
		dRectRatiopy     = 0.;
		bRectStressflx   = FALSE;
		dRectStressflx   = 0.;
		bRectStressfly   = FALSE;
		dRectStressfly   = 0.;

		bCircularStrengthfyh = FALSE;
		dCircularStrengthfyh = 0.;
		bGeneralRebarArea    = FALSE;
		bCircularRatiops     = FALSE;
		dCircularRatiops     = 0.;

		bConfinedConcStrainecy     = FALSE;
		dConfinedConcStrainecy     = dConfinedConcStrainecc;
		bConcDataecy               = FALSE;
		dConcDataecy               = dConcDataeco * 0.7;
		bConcDataesp               = FALSE;
		dConcDataesp               = dConcDataeco * 10.0;
	}

	void Convert881(const T_FIMP_CON_MANDER_881& src)
	{
		nConcType                 = src.nConcType                 ;
		bConcDatafco              = src.bConcDatafco              ;
		dConcDatafco              = src.dConcDatafco              ;
		bConcDataeco              = src.bConcDataeco              ;
		dConcDataeco              = src.dConcDataeco              ;
		nConcDataEc               = src.nConcDataEc               ;
		dConcDataEc               = src.dConcDataEc               ;
		nTensConcDataft           = src.nTensConcDataft           ;
		dTensConcDataft           = src.dTensConcDataft           ;
		dTensConcDataet           = src.dTensConcDataet           ;
		nSectRebarType            = src.nSectRebarType            ;
		bCoefficientAcc           = src.bCoefficientAcc           ;
		dCoefficientAcc           = src.dCoefficientAcc           ;
		bCoefficientAe            = src.bCoefficientAe            ;
		dCoefficientAe            = src.dCoefficientAe            ;
		bCoefficientke            = src.bCoefficientke            ;
		dCoefficientke            = src.dCoefficientke            ;
		bfl                       = src.bfl                       ;
		dfl                       = src.dfl                       ;
		bConfinedConcStrengthfcc  = src.bConfinedConcStrengthfcc  ;
		dConfinedConcStrengthfcc  = src.dConfinedConcStrengthfcc  ;
		bConfinedConcStrainecc    = src.bConfinedConcStrainecc    ;
		dConfinedConcStrainecc    = src.dConfinedConcStrainecc    ;

		bUseUltStrUlt             = src.bUseUltStrUlt             ;
		bConfinedConcStrUltPs     = src.bConfinedConcStrUltPs     ;
		dConfinedConcStrUltPs     = src.dConfinedConcStrUltPs     ;
		nSteelModelKey            = src.nSteelModelKey            ;
		bConfinedConcStrUltEcu    = src.bConfinedConcStrUltEcu    ;
		dConfinedConcStrUltEcu    = src.dConfinedConcStrUltEcu    ;

		nRectSectAutoorUser = src.nRectSectAutoorUser;
		nCircSectAutoorUser = src.nCircSectAutoorUser;
		dXdirectionbc       = src.dXdirectionbc      ;
		dXdirectionwxi      = src.dXdirectionwxi     ;
		nXdirectionn        = src.nXdirectionn       ;
		dYdirectiondc       = src.dYdirectiondc      ;
		dYdirectionwyj      = src.dYdirectionwyj     ;  
		nYdirectionm        = src.nYdirectionm       ;
		dds                 = src.dds                ;

		nLongiRebarType    = src.nLongiRebarType;
		nLongiRebarNum     = src.nLongiRebarNum ;
		memcpy(strLongiRebarSize, src.strLongiRebarSize, sizeof(src.strLongiRebarSize));
		dLongiRebarArea    = src.dLongiRebarArea ; 
		bLongiRebarRatio   = src.bLongiRebarRatio ;          
		dLongiRebarRatio   = src.dLongiRebarRatio ;
		nGeneralRebarType  = src.nGeneralRebarType;
		memcpy(strGeneralRebarSize, src.strGeneralRebarSize, sizeof(src.strGeneralRebarSize));
		dGeneralRebarArea  = src.dGeneralRebarArea;
		dHoopsSpaceS1      = src.dHoopsSpaceS1    ;
		dClearHoopsSpaceS2 = src.dClearHoopsSpaceS2;
		nGeneralRebarNumX  = src.nGeneralRebarNumX;
		nGeneralRebarNumY  = src.nGeneralRebarNumY;
		dGeneralAsx        = src.dGeneralAsx      ;
		dGeneralAsy        = src.dGeneralAsy      ;

		bRectStrengthfyh = src.bRectStrengthfyh  ;
		dRectStrengthfyh = src.dRectStrengthfyh;
		bGeneralAsx      = src.bGeneralAsx     ;
		bGeneralAsy      = src.bGeneralAsy     ;
		bRectRatiopx     = src.bRectRatiopx     ;               
		dRectRatiopx     = src.dRectRatiopx    ;
		bRectRatiopy     = src.bRectRatiopy    ;
		dRectRatiopy     = src.dRectRatiopy    ;
		bRectStressflx   = src.bRectStressflx  ;
		dRectStressflx   = src.dRectStressflx  ;
		bRectStressfly   = src.bRectStressfly  ;
		dRectStressfly   = src.dRectStressfly  ;

		bCircularStrengthfyh = src.bCircularStrengthfyh ;
		dCircularStrengthfyh = src.dCircularStrengthfyh;
		bGeneralRebarArea    = src.bGeneralRebarArea   ;
		bCircularRatiops     = src.bCircularRatiops    ;
		dCircularRatiops     = src.dCircularRatiops    ;

		bConfinedConcStrainecy     = FALSE;
		dConfinedConcStrainecy     = dConfinedConcStrainecc;
		bConcDataecy               = FALSE;
		dConcDataecy               = dConcDataeco * 0.7;
		bConcDataesp               = FALSE;
		dConcDataesp               = dConcDataeco * 10.0;
	};

	void Convert800(const T_FIMP_CON_MANDER_800& src)
	{
		nConcType                 = src.nConcType                 ;
		bConcDatafco              = src.bConcDatafco              ;
		dConcDatafco              = src.dConcDatafco              ;
		bConcDataeco              = src.bConcDataeco              ;
		dConcDataeco              = src.dConcDataeco              ;
		nConcDataEc               = src.nConcDataEc               ;
		dConcDataEc               = src.dConcDataEc               ;
		nTensConcDataft           = src.nTensConcDataft           ;
		dTensConcDataft           = src.dTensConcDataft           ;
		dTensConcDataet           = src.dTensConcDataet           ;
		nSectRebarType            = src.nSectRebarType            ;
		bCoefficientAcc           = src.bCoefficientAcc           ;
		dCoefficientAcc           = src.dCoefficientAcc           ;
		bCoefficientAe            = src.bCoefficientAe            ;
		dCoefficientAe            = src.dCoefficientAe            ;
		bCoefficientke            = src.bCoefficientke            ;
		dCoefficientke            = src.dCoefficientke            ;
		bfl                       = src.bfl                       ;
		dfl                       = src.dfl                       ;
		bConfinedConcStrengthfcc  = src.bConfinedConcStrengthfcc  ;
		dConfinedConcStrengthfcc  = src.dConfinedConcStrengthfcc  ;
		bConfinedConcStrainecc    = src.bConfinedConcStrainecc    ;
		dConfinedConcStrainecc    = src.dConfinedConcStrainecc    ;

		bUseUltStrUlt              = FALSE;
		bConfinedConcStrUltPs      = FALSE;
		dConfinedConcStrUltPs      = 0.;
		nSteelModelKey = 0;
		bConfinedConcStrUltEcu     = FALSE;
		dConfinedConcStrUltEcu     = 0.;
		
		nRectSectAutoorUser = src.nRectSectAutoorUser;
		nCircSectAutoorUser = src.nCircSectAutoorUser;
		dXdirectionbc       = src.dXdirectionbc      ;
		dXdirectionwxi      = src.dXdirectionwxi     ;
		nXdirectionn        = src.nXdirectionn       ;
		dYdirectiondc       = src.dYdirectiondc      ;
		dYdirectionwyj      = src.dYdirectionwyj     ;  
		nYdirectionm        = src.nYdirectionm       ;
		dds                 = src.dds                ;
		
		nLongiRebarType    = src.nLongiRebarType;
		nLongiRebarNum     = src.nLongiRebarNum ;
		memcpy(strLongiRebarSize, src.strLongiRebarSize, sizeof(src.strLongiRebarSize));
		dLongiRebarArea    = src.dLongiRebarArea ; 
		bLongiRebarRatio   = src.bLongiRebarRatio ;          
		dLongiRebarRatio   = src.dLongiRebarRatio ;
		nGeneralRebarType  = src.nGeneralRebarType;
		memcpy(strGeneralRebarSize, src.strGeneralRebarSize, sizeof(src.strGeneralRebarSize));
		dGeneralRebarArea  = src.dGeneralRebarArea;
		dHoopsSpaceS1      = src.dHoopsSpaceS1    ;
		dClearHoopsSpaceS2 = src.dClearHoopsSpaceS2;
		nGeneralRebarNumX  = src.nGeneralRebarNumX;
		nGeneralRebarNumY  = src.nGeneralRebarNumY;
		dGeneralAsx        = src.dGeneralAsx      ;
		dGeneralAsy        = src.dGeneralAsy      ;
		
		bRectStrengthfyh = src.bRectStrengthfyh  ;
		dRectStrengthfyh = src.dRectStrengthfyh;
		bGeneralAsx      = src.bGeneralAsx     ;
		bGeneralAsy      = src.bGeneralAsy     ;
		bRectRatiopx     = src.bRectRatiopx     ;               
		dRectRatiopx     = src.dRectRatiopx    ;
		bRectRatiopy     = src.bRectRatiopy    ;
		dRectRatiopy     = src.dRectRatiopy    ;
		bRectStressflx   = src.bRectStressflx  ;
		dRectStressflx   = src.dRectStressflx  ;
		bRectStressfly   = src.bRectStressfly  ;
		dRectStressfly   = src.dRectStressfly  ;
		
		bCircularStrengthfyh = src.bCircularStrengthfyh ;
		dCircularStrengthfyh = src.dCircularStrengthfyh;
		bGeneralRebarArea    = src.bGeneralRebarArea   ;
		bCircularRatiops     = src.bCircularRatiops    ;
		dCircularRatiops     = src.dCircularRatiops    ;

		bConfinedConcStrainecy     = FALSE;
		dConfinedConcStrainecy     = dConfinedConcStrainecc;
		bConcDataecy               = FALSE;
		dConcDataecy               = dConcDataeco * 0.7;
		bConcDataesp               = FALSE;
		dConcDataesp               = dConcDataeco * 10.0;
	};
};

// FEMA - Energy Dissipation Factor
enum D_FIMP_CON_FEMA_INPUT_TYPE { D_FIMP_CON_FEMA_INPUT_TYPE_VALUE, D_FIMP_CON_FEMA_INPUT_TYPE_RATIO };
struct T_FIMP_STL_FEMAEN
{
	BOOL   bSymmetric;
	int	   nInputType; // D_FIMP_CON_FEMA_INPUT_TYPE
	double dForceMomentRatio[2][9]; // [Displacement, Force][-E,.., A, B, ..,E]
	double dYieldForce[2]; // [+, -]
	BOOL   bEy;
	double dEy[2];
	double dE; // Elastic Modulus
	int    nShapeCurve; // D_PHGT_FEMA_SHAPE
	BOOL   bUseStrengthLoss;
	BOOL   bStrengthLoss; // Yes,No
	BOOL   bUseTotalStrLossAtE; // Check on/off 
	BOOL   bTotalStrLossAtE; // Yes,No
	T_PHGT_FEMA_ENRG_DISSIPATION enrgDissipation;

	void Initialize()
	{
		bSymmetric = TRUE;
		nInputType = 0;
		double dDeformRatio[] ={ 1.0, 6.0 , 6.3, 8.0 };
		double dForceRatio[]  ={ 1.0, 1.25, 0.2, 0.2 };
		int i = 0;
		for (i=0; i<4; ++i)
		{
			dForceMomentRatio[0][i]=-dDeformRatio[3-i];
			dForceMomentRatio[1][i]=-dForceRatio[3-i];
		}
		dForceMomentRatio[0][i]=0.;
		dForceMomentRatio[1][i]=0.;
		for (i=5; i<9; ++i)
		{
			dForceMomentRatio[0][i]=dDeformRatio[i-5];
			dForceMomentRatio[1][i]=dForceRatio[i-5];
		}
		for (int k=0; k<2; ++k)
		{
			dYieldForce[k] = 1.0;
			dEy[k] = 1.0;
		}
		bEy = TRUE;
		dE = 1.0;

		nShapeCurve             = D_PHGT_FEMA_SHAPE_GENERAL;
		bUseStrengthLoss        = FALSE;
		bStrengthLoss           = TRUE;
		bUseTotalStrLossAtE     = FALSE;
		bTotalStrLossAtE        = FALSE;
		enrgDissipation.Initialize();
	}
};

union T_FIMP_PROP_760
{   
	T_FIMP_CON_KENTPK_905 CON_KENTPK;	// Kent & Park (기존 콘크리트 모델)
	T_FIMP_CON_STANJP_890 CON_STANJP;	// 일본 콘크리트 표준시방서
	T_FIMP_CON_ROADJP CON_ROADJP;		// 일본 도로교 시방서
	T_FIMP_CON_NAGOYA CON_NAGOYA;		// 나고야 고속공사(명고사)  
	T_FIMP_CON_TRILIN CON_TRILIN;		// Trilinear Concrete Model
	T_FIMP_STL_MENEGO_890 STL_MENEGO;	// Menegotto-Pinto Model(기존 철근 모델)
	T_FIMP_STL_BILINE_890 STL_BILINE;	// Bilinear Model
	T_FIMP_STL_GENBIL STL_GENBIL;		// Unsymmetric Bilinear Steel Model(Frame3D like Model)
	T_FIMP_STL_TRILIN STL_TRILIN;		// Trilinear Steel Model
};

union T_FIMP_PROP_770
{
	T_FIMP_CON_KENTPK_905 CON_KENTPK;	// Kent & Park (기존 콘크리트 모델)
	T_FIMP_CON_STANJP_890 CON_STANJP;	// 일본 콘크리트 표준시방서
	T_FIMP_CON_ROADJP CON_ROADJP;		// 일본 도로교 시방서
	T_FIMP_CON_NAGOYA CON_NAGOYA;		// 나고야 고속공사(명고사)  
	T_FIMP_CON_TRILIN CON_TRILIN;		// Trilinear Concrete Model
	T_FIMP_CON_GB1002 CON_GB1002;		// China (GB50010-02)
	T_FIMP_STL_MENEGO_890 STL_MENEGO;	// Menegotto-Pinto Model(기존 철근 모델)
	T_FIMP_STL_BILINE_890 STL_BILINE;	// Bilinear Model
	T_FIMP_STL_GENBIL STL_GENBIL;		// Unsymmetric Bilinear Steel Model(Frame3D like Model)
	T_FIMP_STL_TRILIN STL_TRILIN;		// Trilinear Steel Model
};
union T_FIMP_PROP_790
{
	T_FIMP_CON_KENTPK_905 CON_KENTPK;	// Kent & Park (기존 콘크리트 모델)
	T_FIMP_CON_STANJP_890 CON_STANJP;	// 일본 콘크리트 표준시방서
	T_FIMP_CON_ROADJP CON_ROADJP;		// 일본 도로교 시방서
	T_FIMP_CON_NAGOYA CON_NAGOYA;		// 나고야 고속공사(명고사)  
	T_FIMP_CON_TRILIN CON_TRILIN;		// Trilinear Concrete Model
	T_FIMP_CON_GB1002 CON_GB1002;		// China (GB50010-02)
	T_FIMP_CON_MANDER_800 CON_MANDER;	// Mander Model
	T_FIMP_STL_MENEGO_890 STL_MENEGO;	// Menegotto-Pinto Model(기존 철근 모델)
	T_FIMP_STL_BILINE_890 STL_BILINE;	// Bilinear Model
	T_FIMP_STL_GENBIL STL_GENBIL;		// Unsymmetric Bilinear Steel Model(Frame3D like Model)
	T_FIMP_STL_TRILIN STL_TRILIN;		// Trilinear Steel Model
};
union T_FIMP_PROP_800
{   
	T_FIMP_CON_KENTPK_905 CON_KENTPK;	// Kent & Park (기존 콘크리트 모델)
	T_FIMP_CON_STANJP_890 CON_STANJP;	// 일본 콘크리트 표준시방서
	T_FIMP_CON_ROADJP CON_ROADJP;		// 일본 도로교 시방서
	T_FIMP_CON_NAGOYA CON_NAGOYA;		// 나고야 고속공사(명고사)  
	T_FIMP_CON_TRILIN CON_TRILIN;		// Trilinear Concrete Model
	T_FIMP_CON_GB1002 CON_GB1002;		// China (GB50010-02)
	T_FIMP_CON_MANDER_800 CON_MANDER;	// Mander Model
	T_FIMP_STL_MENEGO_890 STL_MENEGO;	// Menegotto-Pinto Model(기존 철근 모델)
	T_FIMP_STL_BILINE_890 STL_BILINE;	// Bilinear Model
	T_FIMP_STL_GENBIL STL_GENBIL;		// Unsymmetric Bilinear Steel Model(Frame3D like Model)
	T_FIMP_STL_TRILIN STL_TRILIN;		// Trilinear Steel Model
	T_FIMP_STL_PARK   STL_PARK;			// Park Model
};
union T_FIMP_PROP_830
{   
	T_FIMP_CON_KENTPK_905 CON_KENTPK;	// Kent & Park (기존 콘크리트 모델)
	T_FIMP_CON_STANJP_890 CON_STANJP;	// 일본 콘크리트 표준시방서
	T_FIMP_CON_ROADJP CON_ROADJP;		// 일본 도로교 시방서
	T_FIMP_CON_NAGOYA CON_NAGOYA;		// 나고야 고속공사(명고사)  
	T_FIMP_CON_TRILIN CON_TRILIN;		// Trilinear Concrete Model
	T_FIMP_CON_GB1002 CON_GB1002;		// China (GB50010-02)
	T_FIMP_CON_MANDER_881 CON_MANDER;	// Mander Model
	T_FIMP_STL_MENEGO_890 STL_MENEGO;	// Menegotto-Pinto Model(기존 철근 모델)
	T_FIMP_STL_BILINE_890 STL_BILINE;	// Bilinear Model
	T_FIMP_STL_GENBIL STL_GENBIL;		// Unsymmetric Bilinear Steel Model(Frame3D like Model)
	T_FIMP_STL_TRILIN STL_TRILIN;		// Trilinear Steel Model
	T_FIMP_STL_PARK   STL_PARK;			// Park Model
};
union T_FIMP_PROP_881
{   
	T_FIMP_CON_KENTPK_905 CON_KENTPK;	// Kent & Park (기존 콘크리트 모델)
	T_FIMP_CON_STANJP_890 CON_STANJP;	// 일본 콘크리트 표준시방서
	T_FIMP_CON_ROADJP CON_ROADJP;		// 일본 도로교 시방서 H14
	T_FIMP_CON_NAGOYA CON_NAGOYA;		// 나고야 고속공사(명고사)  
	T_FIMP_CON_TRILIN CON_TRILIN;		// Trilinear Concrete Model
	T_FIMP_CON_GB1002 CON_GB1002;		// China (GB50010-02)
	T_FIMP_CON_GB1010 CON_GB1010;		// China (GB/T50010-10)
	T_FIMP_CON_MANDER_881 CON_MANDER;	// Mander Model
	T_FIMP_CON_ROADJP CON_ROADJP_H24;	// 일본 도로교 시방서 H24
	T_FIMP_STL_MENEGO_890 STL_MENEGO;	// Menegotto-Pinto Model(기존 철근 모델)
	T_FIMP_STL_BILINE_890 STL_BILINE;	// Bilinear Model
	T_FIMP_STL_GENBIL STL_GENBIL;		// Unsymmetric Bilinear Steel Model(Frame3D like Model)
	T_FIMP_STL_TRILIN STL_TRILIN;		// Trilinear Steel Model
	T_FIMP_STL_PARK   STL_PARK;			// Park Model
	T_FIMP_STL_ROADJP STL_ROADJP;		// 일본 도로교 시방서
	T_FIMP_STL_GB2010 STL_GB2010;		// China (GB50010-2010)
};
union T_FIMP_PROP_905
{
	T_FIMP_CON_KENTPK_905 CON_KENTPK;		// Kent & Park (기존 콘크리트 모델)
	T_FIMP_CON_STANJP_970 CON_STANJP;		// 일본 콘크리트 표준시방서
	T_FIMP_CON_ROADJP CON_ROADJP;		// 일본 도로교 시방서 H14
	T_FIMP_CON_NAGOYA CON_NAGOYA;		// 나고야 고속공사(명고사)  
	T_FIMP_CON_TRILIN CON_TRILIN;		// Trilinear Concrete Model
	T_FIMP_CON_GB1002 CON_GB1002;		// China (GB50010-02)
	T_FIMP_CON_GB1010 CON_GB1010;		// China (GB/T50010-10)
	T_FIMP_CON_MANDER CON_MANDER;		// Mander Model
	T_FIMP_CON_ROADJP CON_ROADJP_H24;	// 일본 도로교 시방서 H24
	T_FIMP_STL_MENEGO STL_MENEGO;		// Menegotto-Pinto Model(기존 철근 모델)
	T_FIMP_STL_BILINE STL_BILINE;		// Bilinear Model
	T_FIMP_STL_GENBIL STL_GENBIL;		// Unsymmetric Bilinear Steel Model(Frame3D like Model)
	T_FIMP_STL_TRILIN STL_TRILIN;		// Trilinear Steel Model
	T_FIMP_STL_PARK   STL_PARK;			// Park Model
	T_FIMP_STL_ROADJP STL_ROADJP;		// 일본 도로교 시방서
	T_FIMP_STL_GB2010 STL_GB2010;		// China (GB50010-2010)
	T_FIMP_STL_FEMAEN STL_FEMAEN;		// FEMA - Energy Dissipation Factor
};
union T_FIMP_PROP_970
{
	T_FIMP_CON_KENTPK CON_KENTPK;		// Kent & Park (기존 콘크리트 모델)
	T_FIMP_CON_STANJP_970 CON_STANJP;		// 일본 콘크리트 표준시방서
	T_FIMP_CON_ROADJP CON_ROADJP;		// 일본 도로교 시방서 H14
	T_FIMP_CON_NAGOYA CON_NAGOYA;		// 나고야 고속공사(명고사)  
	T_FIMP_CON_TRILIN CON_TRILIN;		// Trilinear Concrete Model
	T_FIMP_CON_GB1002 CON_GB1002;		// China (GB50010-02)
	T_FIMP_CON_GB1010 CON_GB1010;		// China (GB/T50010-10)
	T_FIMP_CON_MANDER CON_MANDER;		// Mander Model
	T_FIMP_CON_ROADJP CON_ROADJP_H24;	// 일본 도로교 시방서 H24
	T_FIMP_STL_MENEGO STL_MENEGO;		// Menegotto-Pinto Model(기존 철근 모델)
	T_FIMP_STL_BILINE STL_BILINE;		// Bilinear Model
	T_FIMP_STL_GENBIL STL_GENBIL;		// Unsymmetric Bilinear Steel Model(Frame3D like Model)
	T_FIMP_STL_TRILIN STL_TRILIN;		// Trilinear Steel Model
	T_FIMP_STL_PARK   STL_PARK;			// Park Model
	T_FIMP_STL_ROADJP STL_ROADJP;		// 일본 도로교 시방서
	T_FIMP_STL_GB2010 STL_GB2010;		// China (GB50010-2010)
	T_FIMP_STL_FEMAEN STL_FEMAEN;		// FEMA - Energy Dissipation Factor
};
#define D_FIMP_CURV_CONC_KENPAR		0
#define D_FIMP_CURV_CONC_JPSTND		1
#define D_FIMP_CURV_CONC_JP_H14		2
#define D_FIMP_CURV_CONC_NAGOYA		3
#define D_FIMP_CURV_CONC_TRILIN		4
#define D_FIMP_CURV_CONC_CHGB02		5
#define D_FIMP_CURV_CONC_MANDER		6
#define D_FIMP_CURV_CONC_JP_H24		7
#define D_FIMP_CURV_CONC_CHGB10		8

#define D_FIMP_CURV_STEL_MENEGO		0
#define D_FIMP_CURV_STEL_BILINE		1
#define D_FIMP_CURV_STEL_GENBIL		2
#define D_FIMP_CURV_STEL_TRILIN		3
#define D_FIMP_CURV_STEL_PARKMD		4
#define D_FIMP_CURV_STEL_JPROAD		5
#define D_FIMP_CURV_STEL_CHGB10		6
#define D_FIMP_CURV_STEL_ENRGDS		7 
#define D_FIMP_CURV_STEL_DUMMY_		8 // 솔버에서 D_FIMP_CURV_STEL_ENRGDS을 8로 쓰고 있다... 7은 modified menego..가 있었던 듯? 다음에는 9부터 쓰세요

union T_FIMP_PROP
{   
	T_FIMP_CON_KENTPK CON_KENTPK;		// Kent & Park (기존 콘크리트 모델)
	T_FIMP_CON_STANJP CON_STANJP;		// 일본 콘크리트 표준시방서
	T_FIMP_CON_ROADJP CON_ROADJP;		// 일본 도로교 시방서 H14
	T_FIMP_CON_NAGOYA CON_NAGOYA;		// 나고야 고속공사(명고사)  
	T_FIMP_CON_TRILIN CON_TRILIN;		// Trilinear Concrete Model
	T_FIMP_CON_GB1002 CON_GB1002;		// China (GB50010-02)
	T_FIMP_CON_GB1010 CON_GB1010;		// China (GB/T50010-10)
	T_FIMP_CON_MANDER CON_MANDER;		// Mander Model
	T_FIMP_CON_ROADJP CON_ROADJP_H24;	// 일본 도로교 시방서 H24
	T_FIMP_STL_MENEGO STL_MENEGO;		// Menegotto-Pinto Model(기존 철근 모델)
	T_FIMP_STL_BILINE STL_BILINE;		// Bilinear Model
	T_FIMP_STL_GENBIL STL_GENBIL;		// Unsymmetric Bilinear Steel Model(Frame3D like Model)
	T_FIMP_STL_TRILIN STL_TRILIN;		// Trilinear Steel Model
	T_FIMP_STL_PARK   STL_PARK;			// Park Model
	T_FIMP_STL_ROADJP STL_ROADJP;		// 일본 도로교 시방서
	T_FIMP_STL_GB2010 STL_GB2010;		// China (GB50010-2010)
	T_FIMP_STL_FEMAEN STL_FEMAEN;		// FEMA - Energy Dissipation Factor


	void Convert881(const T_FIMP_PROP_881 &PROP, const int &nMaterialType, const int &nHysModelType)
	{
		if(nMaterialType==0)
		{
			switch(nHysModelType)
			{
			case D_FIMP_CURV_CONC_KENPAR: CON_KENTPK.Convert905(PROP.CON_KENTPK);	break; // Kent&Park
			case D_FIMP_CURV_CONC_JPSTND: CON_STANJP.Convert890(PROP.CON_STANJP);	break; // JP Conc. Standard Spec
			case D_FIMP_CURV_CONC_JP_H14: CON_ROADJP     = PROP.CON_ROADJP;			break; // JP Roadway Spec H14
			case D_FIMP_CURV_CONC_NAGOYA: CON_NAGOYA     = PROP.CON_NAGOYA;			break; // Nagoya
			case D_FIMP_CURV_CONC_TRILIN: CON_TRILIN     = PROP.CON_TRILIN;			break; // Trilinear Concrete Model
			case D_FIMP_CURV_CONC_CHGB02: CON_GB1002     = PROP.CON_GB1002;			break; // China (GB50010-02)
			case D_FIMP_CURV_CONC_MANDER: CON_MANDER.Convert881(PROP.CON_MANDER);	break; // Mander Model
			case D_FIMP_CURV_CONC_JP_H24: CON_ROADJP_H24 = PROP.CON_ROADJP_H24;		break; // JP Roadway Spec H24
			case D_FIMP_CURV_CONC_CHGB10: CON_GB1010     = PROP.CON_GB1010;			break; // China (GB/T50010-10)
			default: ASSERT(0); break;
			}
		}
		else if(nMaterialType==1)
		{
			switch(nHysModelType)
			{
			case D_FIMP_CURV_STEL_MENEGO: STL_MENEGO.Convert890(PROP.STL_MENEGO);	break; // Menegotto
			case D_FIMP_CURV_STEL_BILINE: STL_BILINE.Convert890(PROP.STL_BILINE);	break; // Bilinear
			case D_FIMP_CURV_STEL_GENBIL: STL_GENBIL = PROP.STL_GENBIL;				break; // Unsymmetric Bilinear Steel Model
			case D_FIMP_CURV_STEL_TRILIN: STL_TRILIN = PROP.STL_TRILIN;				break; // Trilinear Steel Model
			case D_FIMP_CURV_STEL_PARKMD: STL_PARK   = PROP.STL_PARK;				break; // Park Model
			case D_FIMP_CURV_STEL_JPROAD: STL_ROADJP = PROP.STL_ROADJP;				break; // JP Roadway Spec
			case D_FIMP_CURV_STEL_CHGB10: STL_GB2010 = PROP.STL_GB2010;				break; // China (GB50010-2010)
			default: ASSERT(0); break;
			}
		}
	}

	void Convert830(const T_FIMP_PROP_830 &PROP, const int &nMaterialType, const int &nHysModelType)
	{
		if(nMaterialType==0)
		{
			switch(nHysModelType)
			{
			case D_FIMP_CURV_CONC_KENPAR: CON_KENTPK.Convert905(PROP.CON_KENTPK);	break; // Kent&Park
			case D_FIMP_CURV_CONC_JPSTND: CON_STANJP.Convert890(PROP.CON_STANJP);	break; // JP Conc. Standard Spec
			case D_FIMP_CURV_CONC_JP_H14: CON_ROADJP = PROP.CON_ROADJP;				break; // JP Roadway Spec H14
			case D_FIMP_CURV_CONC_NAGOYA: CON_NAGOYA = PROP.CON_NAGOYA;				break; // Nagoya
			case D_FIMP_CURV_CONC_TRILIN: CON_TRILIN = PROP.CON_TRILIN;				break; // Trilinear Concrete Model
			case D_FIMP_CURV_CONC_CHGB02: CON_GB1002 = PROP.CON_GB1002;				break; // China (GB50010-02)
			case D_FIMP_CURV_CONC_MANDER: CON_MANDER.Convert881(PROP.CON_MANDER);	break; // Mander Model
			case D_FIMP_CURV_CONC_JP_H24: CON_ROADJP_H24.Initialize();				break; // JP Roadway Spec H24
			case D_FIMP_CURV_CONC_CHGB10: CON_GB1010.Initialize();					break; // China (GB/T50010-10)
			default: ASSERT(0); break;
			}
		}
		else if(nMaterialType==1)
		{
			switch(nHysModelType)
			{
			case D_FIMP_CURV_STEL_MENEGO: STL_MENEGO.Convert890(PROP.STL_MENEGO);	break; // Menegotto
			case D_FIMP_CURV_STEL_BILINE: STL_BILINE.Convert890(PROP.STL_BILINE);	break; // Bilinear
			case D_FIMP_CURV_STEL_GENBIL: STL_GENBIL = PROP.STL_GENBIL;				break; // Unsymmetric Bilinear Steel Model
			case D_FIMP_CURV_STEL_TRILIN: STL_TRILIN = PROP.STL_TRILIN;				break; // Trilinear Steel Model
			case D_FIMP_CURV_STEL_PARKMD: STL_PARK   = PROP.STL_PARK;				break; // Park Model
			case D_FIMP_CURV_STEL_JPROAD: STL_ROADJP.Initialize();					break; // JP Roadway Spec
			case D_FIMP_CURV_STEL_CHGB10: STL_GB2010.Initialize();					break; // China (GB50010-2010)
			default: ASSERT(0); break;
			}
		}
	}

	void Convert800(const T_FIMP_PROP_800 &PROP, const int &nMaterialType, const int &nHysModelType)
	{
		if(nMaterialType==0)
		{
			switch(nHysModelType)
			{
			case D_FIMP_CURV_CONC_KENPAR: CON_KENTPK.Convert905(PROP.CON_KENTPK);	break; // Kent&Park
			case D_FIMP_CURV_CONC_JPSTND: CON_STANJP.Convert890(PROP.CON_STANJP);	break; // JP Conc. Standard Spec
			case D_FIMP_CURV_CONC_JP_H14: CON_ROADJP = PROP.CON_ROADJP;				break; // JP Roadway Spec
			case D_FIMP_CURV_CONC_NAGOYA: CON_NAGOYA = PROP.CON_NAGOYA;				break; // Nagoya
			case D_FIMP_CURV_CONC_TRILIN: CON_TRILIN = PROP.CON_TRILIN;				break; // Trilinear Concrete Model
			case D_FIMP_CURV_CONC_CHGB02: CON_GB1002 = PROP.CON_GB1002;				break; // China (GB50010-02)
			case D_FIMP_CURV_CONC_MANDER: CON_MANDER.Convert800(PROP.CON_MANDER);	break; // Mander Model
			case D_FIMP_CURV_CONC_JP_H24: CON_ROADJP_H24.Initialize();				break; // JP Roadway Spec H24
			case D_FIMP_CURV_CONC_CHGB10: CON_GB1010.Initialize();					break; // China (GB/T50010-10)
			default: ASSERT(0); break;
			}
		}
		else if(nMaterialType==1)
		{
			switch(nHysModelType)
			{
			case D_FIMP_CURV_STEL_MENEGO: STL_MENEGO.Convert890(PROP.STL_MENEGO);	break; // Menegotto
			case D_FIMP_CURV_STEL_BILINE: STL_BILINE.Convert890(PROP.STL_BILINE);	break; // Bilinear
			case D_FIMP_CURV_STEL_GENBIL: STL_GENBIL = PROP.STL_GENBIL;				break; // Unsymmetric Bilinear Steel Model
			case D_FIMP_CURV_STEL_TRILIN: STL_TRILIN = PROP.STL_TRILIN;				break; // Trilinear Steel Model
			case D_FIMP_CURV_STEL_PARKMD: STL_PARK   = PROP.STL_PARK;				break; // Park Model
			case D_FIMP_CURV_STEL_JPROAD: STL_ROADJP.Initialize();					break; // JP Roadway Spec
			case D_FIMP_CURV_STEL_CHGB10: STL_GB2010.Initialize();					break; // China (GB50010-2010)
			default: ASSERT(0); break;
			}
		}
	}

	void Convert790(const T_FIMP_PROP_790 &PROP, const int &nMaterialType, const int &nHysModelType)
	{
		if(nMaterialType==0)
		{
			switch(nHysModelType)
			{
			case D_FIMP_CURV_CONC_KENPAR: CON_KENTPK.Convert905(PROP.CON_KENTPK);	break; // Kent&Park
			case D_FIMP_CURV_CONC_JPSTND: CON_STANJP.Convert890(PROP.CON_STANJP);	break; // JP Conc. Standard Spec
			case D_FIMP_CURV_CONC_JP_H14: CON_ROADJP = PROP.CON_ROADJP;				break; // JP Roadway Spec
			case D_FIMP_CURV_CONC_NAGOYA: CON_NAGOYA = PROP.CON_NAGOYA;				break; // Nagoya
			case D_FIMP_CURV_CONC_TRILIN: CON_TRILIN = PROP.CON_TRILIN;				break; // Trilinear Concrete Model
			case D_FIMP_CURV_CONC_CHGB02: CON_GB1002 = PROP.CON_GB1002;				break; // China (GB50010-02)
			case D_FIMP_CURV_CONC_MANDER: CON_MANDER.Convert800(PROP.CON_MANDER);	break; // Mander Model
			case D_FIMP_CURV_CONC_JP_H24: CON_ROADJP_H24.Initialize();				break; // JP Roadway Spec H24
			case D_FIMP_CURV_CONC_CHGB10: CON_GB1010.Initialize();					break; // China (GB/T50010-10)
			default: ASSERT(0); break;
			}
		}
		else if(nMaterialType==1)
		{
			switch(nHysModelType)
			{
			case D_FIMP_CURV_STEL_MENEGO: STL_MENEGO.Convert890(PROP.STL_MENEGO);	break; // Menegotto
			case D_FIMP_CURV_STEL_BILINE: STL_BILINE.Convert890(PROP.STL_BILINE);	break; // Bilinear
			case D_FIMP_CURV_STEL_GENBIL: STL_GENBIL = PROP.STL_GENBIL;				break; // Unsymmetric Bilinear Steel Model
			case D_FIMP_CURV_STEL_TRILIN: STL_TRILIN = PROP.STL_TRILIN;				break; // Trilinear Steel Model
			case D_FIMP_CURV_STEL_PARKMD: STL_PARK.Initialize();					break; // Park Model
			case D_FIMP_CURV_STEL_JPROAD: STL_ROADJP.Initialize();					break; // JP Roadway Spec
			case D_FIMP_CURV_STEL_CHGB10: STL_GB2010.Initialize();					break; // China (GB50010-2010)
			default: ASSERT(0); break;
			}
		}
	}
	void Convert770(const T_FIMP_PROP_770 &PROP, const int &nMaterialType, const int &nHysModelType)
	{
		if(nMaterialType==0)
		{
			switch(nHysModelType)
			{
			case D_FIMP_CURV_CONC_KENPAR: CON_KENTPK.Convert905(PROP.CON_KENTPK);	break; // Kent&Park
			case D_FIMP_CURV_CONC_JPSTND: CON_STANJP.Convert890(PROP.CON_STANJP);	break; // JP Conc. Standard Spec
			case D_FIMP_CURV_CONC_JP_H14: CON_ROADJP = PROP.CON_ROADJP;				break; // JP Roadway Spec
			case D_FIMP_CURV_CONC_NAGOYA: CON_NAGOYA = PROP.CON_NAGOYA;				break; // Nagoya
			case D_FIMP_CURV_CONC_TRILIN: CON_TRILIN = PROP.CON_TRILIN;				break; // Trilinear Concrete Model
			case D_FIMP_CURV_CONC_CHGB02: CON_GB1002 = PROP.CON_GB1002;				break; // China (GB50010-02)
			case D_FIMP_CURV_CONC_MANDER: CON_MANDER.Initialize();					break; // Mander Model
			case D_FIMP_CURV_CONC_JP_H24: CON_ROADJP_H24.Initialize();				break; // JP Roadway Spec H24
			case D_FIMP_CURV_CONC_CHGB10: CON_GB1010.Initialize();					break; // China (GB/T50010-10)
			default: ASSERT(0); break;
			}
		}
		else if(nMaterialType==1)
		{
			switch(nHysModelType)
			{
			case D_FIMP_CURV_STEL_MENEGO: STL_MENEGO.Convert890(PROP.STL_MENEGO);	break; // Menegotto
			case D_FIMP_CURV_STEL_BILINE: STL_BILINE.Convert890(PROP.STL_BILINE);	break; // Bilinear
			case D_FIMP_CURV_STEL_GENBIL: STL_GENBIL = PROP.STL_GENBIL;				break; // Unsymmetric Bilinear Steel Model
			case D_FIMP_CURV_STEL_TRILIN: STL_TRILIN = PROP.STL_TRILIN;				break; // Trilinear Steel Model
			case D_FIMP_CURV_STEL_PARKMD: STL_PARK.Initialize();					break; // Park Model
			case D_FIMP_CURV_STEL_JPROAD: STL_ROADJP.Initialize();					break; // JP Roadway Spec
			case D_FIMP_CURV_STEL_CHGB10: STL_GB2010.Initialize();					break; // China (GB50010-2010)
			default: ASSERT(0); break;
			}
		}
	}
	void Convert760(const T_FIMP_PROP_760 &PROP, const int &nMaterialType, const int &nHysModelType)
	{
		if(nMaterialType==0)
		{
			switch(nHysModelType)
			{
			case D_FIMP_CURV_CONC_KENPAR: CON_KENTPK.Convert905(PROP.CON_KENTPK);	break; // Kent&Park
			case D_FIMP_CURV_CONC_JPSTND: CON_STANJP.Convert890(PROP.CON_STANJP);	break; // JP Conc. Standard Spec
			case D_FIMP_CURV_CONC_JP_H14: CON_ROADJP = PROP.CON_ROADJP;				break; // JP Roadway Spec
			case D_FIMP_CURV_CONC_NAGOYA: CON_NAGOYA = PROP.CON_NAGOYA;				break; // Nagoya
			case D_FIMP_CURV_CONC_TRILIN: CON_TRILIN = PROP.CON_TRILIN;				break; // Trilinear Concrete Model
			case D_FIMP_CURV_CONC_CHGB02: CON_GB1002.Initialize();					break; // China (GB50010-02) 
			case D_FIMP_CURV_CONC_MANDER: CON_MANDER.Initialize();					break; // Mander Model
			case D_FIMP_CURV_CONC_JP_H24: CON_ROADJP_H24.Initialize();				break; // JP Roadway Spec H24
			case D_FIMP_CURV_CONC_CHGB10: CON_GB1010.Initialize();					break; // China (GB/T50010-10)
			default: ASSERT(0); break;
			}
		}
		else if(nMaterialType==1)
		{
			switch(nHysModelType)
			{
			case D_FIMP_CURV_STEL_MENEGO: STL_MENEGO.Convert890(PROP.STL_MENEGO);	break; // Menegotto
			case D_FIMP_CURV_STEL_BILINE: STL_BILINE.Convert890(PROP.STL_BILINE);	break; // Bilinear
			case D_FIMP_CURV_STEL_GENBIL: STL_GENBIL = PROP.STL_GENBIL;				break; // Unsymmetric Bilinear Steel Model
			case D_FIMP_CURV_STEL_TRILIN: STL_TRILIN = PROP.STL_TRILIN;				break; // Trilinear Steel Model
			case D_FIMP_CURV_STEL_PARKMD: STL_PARK.Initialize();					break; // Park Model
			case D_FIMP_CURV_STEL_JPROAD: STL_ROADJP.Initialize();					break; // JP Roadway Spec
			case D_FIMP_CURV_STEL_CHGB10: STL_GB2010.Initialize();					break; // China (GB50010-2010)
			default: ASSERT(0); break;
			}
		}
	}
};
struct T_FIMP_PROP_UNIT
{
	T_FIMP_PROP_UNIT()
	{
		dFc = D_UNITSYS_BASE_STRESS;
		dFy = D_UNITSYS_BASE_STRESS;
		dE  = D_UNITSYS_BASE_STRESS;
		dA  = D_UNITSYS_BASE_AREA;
		dL  = D_UNITSYS_BASE_LENGTH;    

		dConcDatafco             = D_UNITSYS_BASE_STRESS;
		dConcDataEc              = D_UNITSYS_BASE_STRESS;
		dTensConcDataft          = D_UNITSYS_BASE_STRESS;
		dCoefficientAcc          = D_UNITSYS_BASE_AREA;
		dCoefficientAe           = D_UNITSYS_BASE_AREA;
		dfl                      = D_UNITSYS_BASE_STRESS;
		dConfinedConcStrengthfcc = D_UNITSYS_BASE_STRESS;
		dXdirectionbc            = D_UNITSYS_BASE_LENGTH;
		dXdirectionwxi           = D_UNITSYS_BASE_LENGTH;
		dYdirectiondc            = D_UNITSYS_BASE_LENGTH;
		dYdirectionwyj           = D_UNITSYS_BASE_LENGTH;
		dds                      = D_UNITSYS_BASE_LENGTH;
		dLongiRebarArea          = D_UNITSYS_BASE_AREA;
		dGeneralRebarArea        = D_UNITSYS_BASE_AREA;
		dHoopsSpaceS1            = D_UNITSYS_BASE_LENGTH;
		dClearHoopsSpaceS2       = D_UNITSYS_BASE_LENGTH;
		dGeneralAsx              = D_UNITSYS_BASE_AREA;
		dGeneralAsy              = D_UNITSYS_BASE_AREA;
		dRectStrengthfyh         = D_UNITSYS_BASE_STRESS;
		dRectStressflx           = D_UNITSYS_BASE_STRESS;
		dRectStressfly           = D_UNITSYS_BASE_STRESS;
		dCircularStrengthfyh     = D_UNITSYS_BASE_STRESS;
		dSigsy                   = D_UNITSYS_BASE_STRESS;
		dEs                      = D_UNITSYS_BASE_STRESS;
	}
	int dFc;
	int dFy;
	int dE;
	int dA;
	int dL;

	int dConcDatafco;
	int dConcDataEc;
	int dTensConcDataft;
	int dCoefficientAcc;
	int dCoefficientAe;
	int dfl;
	int dConfinedConcStrengthfcc;
	int dXdirectionbc;
	int dXdirectionwxi;
	int dYdirectiondc;
	int dYdirectionwyj;
	int dds;
	int dLongiRebarArea;
	int dGeneralRebarArea;
	int dHoopsSpaceS1;
	int dClearHoopsSpaceS2;
	int dGeneralAsx;
	int dGeneralAsy;
	int dRectStrengthfyh;
	int dRectStressflx;
	int dRectStressfly;
	int dCircularStrengthfyh;
	int dSigsy;
	int dEs;
};
struct T_FIMP_D_650
{
	CString strName;
	int     nMaterialType;    // 0=Concrete, 1=Steel
	T_FIMP_CONC ConcData;
	T_FIMP_STL  StlData;
	int     nSeq;
	void Initialize()
	{
		strName.Empty();
		nMaterialType = 0;
		ConcData.Initialize();
		StlData.Initialize();
		nSeq = 0;
	}
};
struct T_FIMP_D_760
{
	CString strName;
	int     nMaterialType;    // 0=Concrete, 1=Steel  
	int     nHysModelType;    // nMaterialType = 0(Concrete) : 
														//    0:Kent&Park, 1:JP Conc. Standard Spec, 2:JP Roadway Spec., 3:Nagoya, 
														//    4:Trilinear Concrete Model, 5:China (GB50010-02)
														// nMaterialType = 1(Steel)    : 
														//    0:Menegotto, 1:Bilinear, 2:Unsymmetric Bilinear Steel Model, 
														//    3:Trilinear Steel Model
	
	T_FIMP_PROP_760 PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
};
struct T_FIMP_D_770
{
	CString strName;
	int     nMaterialType;    // 0=Concrete, 1=Steel  
	int     nHysModelType;    // nMaterialType = 0(Concrete) : 
														//    0:Kent&Park, 1:JP Conc. Standard Spec, 2:JP Roadway Spec., 3:Nagoya, 
														//    4:Trilinear Concrete Model, 5:China (GB50010-02)
														// nMaterialType = 1(Steel)    : 
														//    0:Menegotto, 1:Bilinear, 2:Unsymmetric Bilinear Steel Model, 
														//    3:Trilinear Steel Model
														
	T_FIMP_PROP_770 PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
};
struct T_FIMP_D_790
{
	CString strName;
	int     nMaterialType;    // 0=Concrete, 1=Steel  
	int     nHysModelType;    // nMaterialType = 0(Concrete) : 
														//    0:Kent&Park, 1:JP Conc. Standard Spec, 2:JP Roadway Spec., 3:Nagoya, 
														//    4:Trilinear Concrete Model, 5:China (GB50010-02) 6:Mander Model
														// nMaterialType = 1(Steel)    : 
														//    0:Menegotto, 1:Bilinear, 2:Unsymmetric Bilinear Steel Model, 
														//    3:Trilinear Steel Model
														
	T_FIMP_PROP_790 PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
};

struct T_FIMP_D_800
{
	CString strName;
	int     nMaterialType;    // 0=Concrete, 1=Steel  
	int     nHysModelType;    // nMaterialType = 0(Concrete) : 
	//    0:Kent&Park, 1:JP Conc. Standard Spec, 2:JP Roadway Spec., 3:Nagoya, 
	//    4:Trilinear Concrete Model, 5:China (GB50010-02) 6:Mander Model
	// nMaterialType = 1(Steel)    : 
	//    0:Menegotto, 1:Bilinear, 2:Unsymmetric Bilinear Steel Model, 
	//    3:Trilinear Steel Model
	
	T_FIMP_PROP_800 PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
};

struct T_FIMP_D_830
{
	CString strName;
	int     nMaterialType;    // 0=Concrete, 1=Steel  
	int     nHysModelType;    // nMaterialType = 0(Concrete) : 
														//    0:Kent&Park, 1:JP Conc. Standard Spec, 2:JP Roadway Spec., 3:Nagoya, 
														//    4:Trilinear Concrete Model, 5:China (GB50010-02) 6:Mander Model
														//    7:Eurocode Schematic, 8:Eurocode Parabola-Rectangle 
														// nMaterialType = 1(Steel)    : 
														//    0:Menegotto, 1:Bilinear, 2:Unsymmetric Bilinear Steel Model, 
														//    3:Trilinear Steel Model, 4:Park Model
	T_FIMP_PROP_830 PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
};

struct T_FIMP_D_861
{
	CString strName;
	int     nMaterialType;    // 0=Concrete, 1=Steel  
	int     nHysModelType;    // nMaterialType = 0(Concrete) : 
														//    0:Kent&Park, 1:JP Conc. Standard Spec, 2:JP Roadway Spec. H14, 3:Nagoya, 
														//    4:Trilinear Concrete Model, 5:China (GB50010-02) 6:Mander Model
														//    7:JP Roadway Spec. H24
														// nMaterialType = 1(Steel)    : 
														//    0:Menegotto, 1:Bilinear, 2:Unsymmetric Bilinear Steel Model, 
														//    3:Trilinear Steel Model, 4:Park Model
														//    5:JP Roadway Spec.

	T_FIMP_PROP_881 PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
};

enum D_FIBR_TYPE { D_FIBR_TYPE_IEHP, D_FIBR_TYPE_PHGT, D_FIBR_TYPE_MNL };
struct T_FIMP_D_881
{
	CString strName;
	int     nMaterialType;
	int     nHysModelType;
	T_FIMP_PROP PROP;
	int     nSeq;
	double  dTemp1;
	double  dTemp2;
	int     nAssignType;
};

enum D_FIMP_MATLTYPE { D_FIMP_MATLTYPE_CONC, D_FIMP_MATLTYPE_STL };
struct T_FIMP_D
{
	CString strName;
	int     nMaterialType;	// 0=Concrete, 1=Steel  
	int     nHysModelType;	// nMaterialType = 0(Concrete) : 
							//    0:Kent&Park, 1:JP Conc. Standard Spec, 2:JP Roadway Spec. H14, 3:Nagoya, 
							//    4:Trilinear Concrete Model, 5:China (GB50010-02) 6:Mander Model
							//    7:JP Roadway Spec. H24, 8: China (GB/T50010-10), 9: Energy Dissipation Factor
							// nMaterialType = 1(Steel)    : 
							//    0:Menegotto, 1:Bilinear, 2:Unsymmetric Bilinear Steel Model, 
							//    3:Trilinear Steel Model, 4:Park Model
							//    5:JP Roadway Spec., 6:China (GB50010-2010)
														
	T_FIMP_PROP PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	int     nAssignType;       // D_FIBR_TYPE
	void Initialize()
	{
		strName.Empty();
		nMaterialType = 0;
		nHysModelType = 0;    
		PROP.CON_MANDER.Initialize();   // union이므로 가장 큰 사이즈만 초기화
		nSeq = 0;
		dTemp1 = 0.;
		dTemp2 = 0.;
		nAssignType = D_FIBR_TYPE_IEHP;
	}

	double Get_fc() // concrete yield strength
	{
			double dfck = 0.0;
			if(nMaterialType != 0)  return dfck;

			switch (nHysModelType)
			{
			case D_FIMP_CURV_CONC_KENPAR: // 0:Kent&Park
					dfck = PROP.CON_KENTPK.dFc;
					break;
			case D_FIMP_CURV_CONC_JPSTND: // 1:JP Conc. Standard Spec
					dfck = PROP.CON_STANJP.dFc;
					break;
			case D_FIMP_CURV_CONC_JP_H14: // 2:JP Roadway Spec. H14
					dfck = PROP.CON_ROADJP.dSigmack;
					break;
			case D_FIMP_CURV_CONC_NAGOYA: // 3:Nagoya
					dfck = PROP.CON_NAGOYA.dSigmack;
					break;
			case D_FIMP_CURV_CONC_TRILIN: // 4:Trilinear Concrete Model
					dfck = PROP.CON_TRILIN.dSigmay[0];
					break;
			case D_FIMP_CURV_CONC_CHGB02: // 5:China (GB50010-02)
					dfck = PROP.CON_GB1002.dFck;
					break;
			case D_FIMP_CURV_CONC_MANDER: // 6:Mander Model
					dfck = PROP.CON_MANDER.dConcDatafco;
					break;
			case D_FIMP_CURV_CONC_JP_H24: // 7:JP Roadway Spec. H24
					dfck = PROP.CON_ROADJP_H24.dSigmack;
					break;
			case D_FIMP_CURV_CONC_CHGB10: // 8:China (GB/T50010-10)
					dfck = PROP.CON_GB1010.dFcr;
					break;
			default:
					ASSERT(0);
					break;
			}
			return dfck;
	}

	double Get_fy() // steel yield strength
	{
			double dfy = 0.0;
			if(nMaterialType != 1)  return dfy;

			switch (nHysModelType)
			{
			case D_FIMP_CURV_STEL_MENEGO: // 0:Menegotto
					dfy = PROP.STL_MENEGO.dFy;
					break;
			case D_FIMP_CURV_STEL_BILINE: // 1:Bilinear
					dfy = PROP.STL_BILINE.dFy;
					break;
			case D_FIMP_CURV_STEL_GENBIL: // 2:Unsymmetric Bilinear Steel Model
					dfy = PROP.STL_GENBIL.dSigmay;
					break;
			case D_FIMP_CURV_STEL_TRILIN: // 3:Trilinear Steel Model
					dfy = PROP.STL_TRILIN.dSigmay[0];
					break;
			case D_FIMP_CURV_STEL_PARKMD: // 4:Park Model
					dfy = PROP.STL_PARK.dFy;
					break;
			case D_FIMP_CURV_STEL_JPROAD: // 5:JP Roadway Spec.
					dfy = PROP.STL_ROADJP.dSigsy;
					break;
			case D_FIMP_CURV_STEL_CHGB10: // 6:China (GB50010-2010)
					dfy = PROP.STL_GB2010.dFyr;
					break;
			default:
					ASSERT(0);
					break;
			}

			return dfy;
	}
};

struct T_FIMP_D_CH_650
{
	char    strName[20];
	int     nMaterialType;    // 0=Concrete, 1=Steel
	T_FIMP_CONC ConcData;
	T_FIMP_STL  StlData;
	int     nSeq;
};
struct T_FIMP_D_CH_760
{
	char    strName[20];
	int     nMaterialType;    // 0=Concrete, 1=Steel
	int     nHysModelType;
	T_FIMP_PROP_760 PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
};
struct T_FIMP_D_CH_770
{
	char    strName[20];
	int     nMaterialType;    // 0=Concrete, 1=Steel
	int     nHysModelType;
	T_FIMP_PROP_770 PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
};
struct T_FIMP_D_CH_790
{
	char    strName[20];
	int     nMaterialType;    // 0=Concrete, 1=Steel
	int     nHysModelType;
	T_FIMP_PROP_790 PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
};

struct T_FIMP_D_CH_800
{
	char    strName[20];
	int     nMaterialType;    // 0=Concrete, 1=Steel
	int     nHysModelType;
	T_FIMP_PROP_800 PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
};

struct T_FIMP_D_CH_830
{
	char    strName[20];
	int     nMaterialType;    // 0=Concrete, 1=Steel
	int     nHysModelType;
	T_FIMP_PROP_830 PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
};

struct T_FIMP_D_CH_861
{
	char    strName[20];
	int     nMaterialType;    // 0=Concrete, 1=Steel
	int     nHysModelType;
	T_FIMP_PROP_881 PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
};

struct T_FIMP_D_CH_870
{
	char    strName[20];
	int     nMaterialType;    // 0=Concrete, 1=Steel
	int     nHysModelType;
	T_FIMP_PROP_881 PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	int     nAssignType;       // D_FIBR_TYPE
};

struct T_FIMP_D_CH_881
{
	char    strName[80];
	int     nMaterialType;    // 0=Concrete, 1=Steel
	int     nHysModelType;
	T_FIMP_PROP_881 PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	int     nAssignType;       // D_FIBR_TYPE
};

struct T_FIMP_D_CH_905
{
	char    strName[80];
	int     nMaterialType;    // 0=Concrete, 1=Steel
	int     nHysModelType;
	T_FIMP_PROP_905 PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	int     nAssignType;       // D_FIBR_TYPE
};

struct T_FIMP_D_CH
{
	char    strName[80];
	int     nMaterialType;    // 0=Concrete, 1=Steel
	int     nHysModelType;
	T_FIMP_PROP PROP;
	int     nSeq;
	double  dTemp1;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	double  dTemp2;            // Version 관리를 위한 임시 변수(실제 사용되지 않음)
	int     nAssignType;       // D_FIBR_TYPE

	void ConvertToString(T_FIMP_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nMaterialType = nMaterialType;
		rData.nHysModelType = nHysModelType;
		rData.PROP = PROP;
		rData.nSeq = nSeq;
		rData.dTemp1 = dTemp1;
		rData.dTemp2 = dTemp2;
		rData.nAssignType = nAssignType;
	}
	void ConvertToChar(T_FIMP_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		nMaterialType = rData.nMaterialType;
		nHysModelType = rData.nHysModelType;
		PROP = rData.PROP;
		nSeq = rData.nSeq;
		dTemp1 = rData.dTemp1;
		dTemp2 = rData.dTemp2;
		nAssignType = rData.nAssignType;
	}
	void Convert650(T_FIMP_D_CH_650& rData)
	{
		memset(strName,0,sizeof(strName));
		memcpy(strName, rData.strName, sizeof(strName));
		nMaterialType = rData.nMaterialType;
		nHysModelType = 0;
		PROP.STL_GENBIL.Initialize();
		if(nMaterialType == 0)   // Concrete
		{
			PROP.CON_KENTPK.dFc = rData.ConcData.dFc;
			PROP.CON_KENTPK.dE0 = rData.ConcData.dE0;
			PROP.CON_KENTPK.dK  = rData.ConcData.dK;
			PROP.CON_KENTPK.dZ  = rData.ConcData.dZ;
			PROP.CON_KENTPK.dEu = rData.ConcData.dEu;
		}
		else if(nMaterialType == 1)   // Steel
		{
			PROP.STL_MENEGO.dFy = rData.StlData.dFy;
			PROP.STL_MENEGO.dR0 = rData.StlData.dR0;
			PROP.STL_MENEGO.dE  = rData.StlData.dE;
			PROP.STL_MENEGO.dA1 = rData.StlData.dA1;
			PROP.STL_MENEGO.dA2 = rData.StlData.dA2;
			PROP.STL_MENEGO.dB  = rData.StlData.dB;
		}
		nSeq = rData.nSeq;
		dTemp1 = 0.;
		dTemp2 = 0.;
		nAssignType = D_FIBR_TYPE_IEHP;
	}
	void Convert760(T_FIMP_D_CH_760& rData)
	{
		memset(strName,0,sizeof(strName));
		memcpy(strName, rData.strName, sizeof(strName));
		nMaterialType = rData.nMaterialType;
		nHysModelType = 0;
		
		PROP.Convert760(rData.PROP, rData.nMaterialType, rData.nHysModelType);
		nSeq = rData.nSeq;
		dTemp1 = 0.;
		dTemp2 = 0.;
		nAssignType = D_FIBR_TYPE_IEHP;
	}
	void Convert770(T_FIMP_D_CH_770& rData)
	{
		memset(strName,0,sizeof(strName));
		memcpy(strName, rData.strName, sizeof(strName));
		nMaterialType = rData.nMaterialType;
		nHysModelType = rData.nHysModelType;
		
		PROP.Convert770(rData.PROP, rData.nMaterialType, rData.nHysModelType);
		nSeq = rData.nSeq;
		dTemp1 = rData.dTemp1;
		dTemp2 = rData.dTemp2;
		nAssignType = D_FIBR_TYPE_IEHP;
	}
	void Convert790(T_FIMP_D_CH_790& rData)
	{
		memset(strName,0,sizeof(strName));
		memcpy(strName, rData.strName, sizeof(strName));
		nMaterialType = rData.nMaterialType;
		nHysModelType = rData.nHysModelType;
		
		PROP.Convert790(rData.PROP, rData.nMaterialType, rData.nHysModelType);
		nSeq = rData.nSeq;
		dTemp1 = rData.dTemp1;
		dTemp2 = rData.dTemp2;
		nAssignType = D_FIBR_TYPE_IEHP;
	}

	void Convert800(T_FIMP_D_CH_800& rData)
	{
		memset(strName,0,sizeof(strName));
		memcpy(strName, rData.strName, sizeof(strName));
		nMaterialType = rData.nMaterialType;
		nHysModelType = rData.nHysModelType;
		
		PROP.Convert800(rData.PROP, rData.nMaterialType, rData.nHysModelType);
		nSeq = rData.nSeq;
		dTemp1 = rData.dTemp1;
		dTemp2 = rData.dTemp2;
		nAssignType = D_FIBR_TYPE_IEHP;
	}

	void Convert830(T_FIMP_D_CH_830& rData)
	{
		memset(strName,0,sizeof(strName));
		memcpy(strName, rData.strName, sizeof(strName));
		nMaterialType = rData.nMaterialType;
		nHysModelType = rData.nHysModelType;
		
		PROP.Convert830(rData.PROP, rData.nMaterialType, rData.nHysModelType);
		nSeq = rData.nSeq;
		dTemp1 = rData.dTemp1;
		dTemp2 = rData.dTemp2;
		nAssignType = D_FIBR_TYPE_IEHP;
	}

	void Convert861(T_FIMP_D_CH_861& rData)
	{
		memset(strName,0,sizeof(strName));
		memcpy(strName, rData.strName, sizeof(strName));
		nMaterialType = rData.nMaterialType;
		nHysModelType = rData.nHysModelType;

		PROP.Convert881(rData.PROP, rData.nMaterialType, rData.nHysModelType);
		nSeq = rData.nSeq;
		dTemp1 = rData.dTemp1;
		dTemp2 = rData.dTemp2;
		nAssignType = D_FIBR_TYPE_IEHP;
	}
	void Convert870(T_FIMP_D_CH_870& rData)
	{
		memset(strName,0,sizeof(strName));
		memcpy(strName, rData.strName, sizeof(strName));
		nMaterialType = rData.nMaterialType;
		nHysModelType = rData.nHysModelType;

		PROP.Convert881(rData.PROP, rData.nMaterialType, rData.nHysModelType);
		nSeq = rData.nSeq;
		dTemp1 = rData.dTemp1;
		dTemp2 = rData.dTemp2;
		nAssignType = rData.nAssignType;
	}
	void Convert881(T_FIMP_D_CH_881& rData)
	{
		memset(strName,0,sizeof(strName));
		memcpy(strName, rData.strName, sizeof(strName));
		nMaterialType = rData.nMaterialType;
		nHysModelType = rData.nHysModelType;

		PROP.Convert881(rData.PROP, rData.nMaterialType, rData.nHysModelType);
		nSeq = rData.nSeq;
		dTemp1 = rData.dTemp1;
		dTemp2 = rData.dTemp2;
		nAssignType = rData.nAssignType;
	}
};
struct T_FIMP_UDRD_D_650
{
	T_FIMP_K key;
	T_FIMP_D_650 data;
};
struct T_FIMP_UDRD_D_760
{
	T_FIMP_K key;
	T_FIMP_D_760 data;
};
struct T_FIMP_UDRD_D_770
{
	T_FIMP_K key;
	T_FIMP_D_770 data;
};
struct T_FIMP_UDRD_D_790
{
	T_FIMP_K key;
	T_FIMP_D_790 data;
};
struct T_FIMP_UDRD_D_800
{
	T_FIMP_K key;
	T_FIMP_D_800 data;
};
struct T_FIMP_UDRD_D_830
{
	T_FIMP_K key;
	T_FIMP_D_830 data;
};
struct T_FIMP_UDRD_D_861
{
	T_FIMP_K key;
	T_FIMP_D_861 data;
};
struct T_FIMP_UDRD_D_881
{
	T_FIMP_K key;
	T_FIMP_D_881 data;
};
struct T_FIMP_UDRD_D
{
	T_FIMP_K key;
	T_FIMP_D data;
};
struct T_FIMP_UDRD_D_CH_650
{
	T_FIMP_K key;
	T_FIMP_D_CH_650 data;
};
struct T_FIMP_UDRD_D_CH_760
{
	T_FIMP_K key;
	T_FIMP_D_CH_760 data;
};
struct T_FIMP_UDRD_D_CH_770
{
	T_FIMP_K key;
	T_FIMP_D_CH_770 data;
};
struct T_FIMP_UDRD_D_CH_790
{
	T_FIMP_K key;
	T_FIMP_D_CH_790 data;
};
struct T_FIMP_UDRD_D_CH_800
{
	T_FIMP_K key;
	T_FIMP_D_CH_800 data;
};
struct T_FIMP_UDRD_D_CH_830
{
	T_FIMP_K key;
	T_FIMP_D_CH_830 data;
};
struct T_FIMP_UDRD_D_CH_861
{
	T_FIMP_K key;
	T_FIMP_D_CH_861 data;
};
struct T_FIMP_UDRD_D_CH_870
{
	T_FIMP_K key;
	T_FIMP_D_CH_870 data;
};
struct T_FIMP_UDRD_D_CH_881
{
	T_FIMP_K key;
	T_FIMP_D_CH_881 data;
};
struct T_FIMP_UDRD_D_CH
{
	T_FIMP_K key;
	T_FIMP_D_CH data;
};
struct T_FIMP_UNIT
{
	T_FIMP_PROP_UNIT PROP;
};
/*
struct T_FIMP_UNIT
{
	T_FIMP_UNIT()
	{
		dFc = D_UNITSYS_BASE_STRESS;
		dFy = D_UNITSYS_BASE_STRESS;
		dE  = D_UNITSYS_BASE_STRESS;
	}
	int dFc;
	int dFy;
	int dE;
};
*/
#define HASHSIZEFIMP 11

// Inelastic Material Property for FiberModel
#define T_IMFM_K T_MATL_K
struct T_IMFM_D_940 
{
	T_FIMP_K SteelK;  // Steel 일때와 Rebar 용으로사용
	T_FIMP_K ConcK;   // ConcType일 경우엔 Core로 사용, 그 외는 Concrete 로 사용
};
struct T_IMFM_D
{
	T_FIMP_K SteelK;		// Steel 일때와 Rebar 용으로사용
	T_FIMP_K ConcK;			// Concrete 로 사용 (Unconfined Concrete)
	T_FIMP_K Confined_ConcK;   // Confined Concrete 로 사용 (Core Conc)
	T_FIMP_K RebarK;		// SRC 단면 대응을 위해 Rebar, Steel 분리함

	T_IMFM_D() : SteelK(0), ConcK(0), Confined_ConcK(0), RebarK(0) {}
	void Initialize()
	{
		SteelK = 0;
		ConcK = 0;
		Confined_ConcK = 0;
		RebarK = 0;
	}
	void Convert940(T_IMFM_D_940& rData)
	{
		Initialize();
		SteelK = rData.SteelK;
		ConcK = rData.ConcK;
		Confined_ConcK = 0;
		if ( rData.ConcK > 0 )
		{
			RebarK = rData.SteelK; // Steel 에 들어있던 RebarK 를 Rebar로 복사
			SteelK = 0;			   // 기존 Steel 은 0
		}
	}
	bool IsUniqueKey(T_FIMP_K ImfmK)
	{
		// Array of all keys in the structure
		T_FIMP_K keys[] = { SteelK, ConcK, Confined_ConcK, RebarK };
		// Check each key in the structure
		for ( int i = 0; i < 4; i++ )
		{
			// If the key is not zero and doesn't match ImfmK, return false
			if ( keys[i] != 0 && keys[i] != ImfmK )
			{
				return false;
			}
		}
		// If no other non-zero keys were found, return true
		return true;
	}
};
struct T_IMFM_UDRD_D_940
{
	T_IMFM_K key;
	T_IMFM_D_940 data;
};
struct T_IMFM_UDRD_D
{
	T_IMFM_K key;
	T_IMFM_D data;
};
struct T_IMFM_UNIT
{
	T_IMFM_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;
	}
	int nNoUnit;
};
#define HASHSIZEIMFM HASHSIZEMATL

// Pushover Material Property for FiberModel
#define T_PMFM_K T_MATL_K
struct T_PMFM_D
{
	int nType;  // 0: Concrete, 1: Steel
	T_FIMP_K SteelK;
	T_FIMP_K ConcK;
	T_FIMP_K RebarK;

	void Initialize()
	{
		nType  = 0;
		SteelK = 0;
		ConcK  = 0;
		RebarK = 0;
	}
};
struct T_PMFM_UDRD_D
{
	T_PMFM_K key;
	T_PMFM_D data;
};
struct T_PMFM_UNIT
{
	T_PMFM_UNIT()
	{
		nNoUnit=D_UNITSYS_NONE;
	}
	int nNoUnit;
};
#define HASHSIZEPMFM HASHSIZEMATL

// Fiber Division of Section
#define T_FIBR_K unsigned int
/*
struct T_FIBR_BASE
{
	UINT   FibrBaseKey;
	BOOL   bRebar;
	CArray<double, double> aPointY;
	CArray<double, double> aPointZ;
	double dArea;
	double dCenterY;
	double dCenterZ;
	int    FibrMatlID;   // 0=Type1, 1=Type2, 2=Type3
	void Initialize()
	{
		FibrBaseKey = 0;
		bRebar = FALSE;
		aPointY.RemoveAll();
		aPointZ.RemoveAll();
		dArea = 0.;
		dCenterY = 0.;
		dCenterZ = 0.;
		FibrMatlID = 0;
	}
	T_FIBR_BASE() {}
	T_FIBR_BASE(T_FIBR_BASE& src) {*this = src;}
	T_FIBR_BASE& operator=(const T_FIBR_BASE& src)
	{
		FibrBaseKey = src.FibrBaseKey;
		bRebar = src.bRebar;
		aPointY.Copy(src.aPointY);
		aPointZ.Copy(src.aPointZ);
		dArea = src.dArea;
		dCenterY = src.dCenterY;
		dCenterZ = src.dCenterZ;
		FibrMatlID = src.FibrMatlID;
		return *this;
	}
};
struct T_FIBR_BASE_RW
{
	UINT   FibrBaseKey;
	BOOL   bRebar;
	int    nCountPoint;
	double dArea;
	double dCenterY;
	double dCenterZ;
	int    FibrMatlID;   // 0=Type1, 1=Type2, 2=Type3
};
struct T_FIBR_D
{  
	CString strName;
	UINT   SectKey;
	CArray<T_FIBR_BASE, T_FIBR_BASE&> aFibrBase;
	T_FIMP_K FimpKey[3];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nSeq;
	void Initialize()
	{
		strName.Empty();
		SectKey = 0;
		aFibrBase.RemoveAll();
		for(int i = 0; i < 3; i++) FimpKey[i] = 0;
		bMonitoredFiber = TRUE;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = 0;
		nSeq = 0;
	}
	T_FIBR_D() {}
	T_FIBR_D(T_FIBR_D& src) {*this = src;}
	T_FIBR_D& operator=(const T_FIBR_D& src)
	{
		strName = src.strName;
		SectKey = src.SectKey;
		aFibrBase.Copy(src.aFibrBase);
		memcpy(FimpKey, src.FimpKey, sizeof(FimpKey));
		memcpy(MonitoredFiber, src.MonitoredFiber, sizeof(MonitoredFiber));
		bMonitoredFiber = src.bMonitoredFiber;
		nSeq = src.nSeq;
		return *this;
	}
};
struct T_FIBR_D_TEMP
{ 
	CString strName;
	UINT   SectKey;
	CArray<T_FIBR_BASE_RW, T_FIBR_BASE_RW&> aFibrBase;
	T_FIMP_K FimpKey[3];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	CArray<double, double> aTotalPointY;  // 각 fiber의 절점 array를 모두 모은 것 
	CArray<double, double> aTotalPointZ;  // 각 fiber의 절점 array를 모두 모은 것
	int    nSeq;
	void Set(T_FIBR_D& rData)
	{
		strName = rData.strName;
		SectKey = rData.SectKey;
		for(int i = 0; i < 3; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];    
		aFibrBase.SetSize(rData.aFibrBase.GetSize());
		for(int i = 0; i < rData.aFibrBase.GetSize(); i++)
		{
			aFibrBase[i].FibrBaseKey = rData.aFibrBase[i].FibrBaseKey;
			aFibrBase[i].bRebar      = rData.aFibrBase[i].bRebar;
			aFibrBase[i].dArea       = rData.aFibrBase[i].dArea;
			aFibrBase[i].dCenterY    = rData.aFibrBase[i].dCenterY;
			aFibrBase[i].dCenterZ    = rData.aFibrBase[i].dCenterZ;
			aFibrBase[i].FibrMatlID  = rData.aFibrBase[i].FibrMatlID;
			aFibrBase[i].nCountPoint = rData.aFibrBase[i].aPointY.GetSize();
			for(int j = 0; j < rData.aFibrBase[i].aPointY.GetSize(); j++)
			{
				aTotalPointY.Add(rData.aFibrBase[i].aPointY[j]);
				aTotalPointZ.Add(rData.aFibrBase[i].aPointZ[j]);        
			}      
		}
		nSeq = rData.nSeq;
	}
	void Get(T_FIBR_D& rData)
	{
		rData.strName = strName;
		rData.SectKey = SectKey;
		for(int i = 0; i < 3; i++) rData.FimpKey[i] = FimpKey[i];
		rData.bMonitoredFiber = bMonitoredFiber;
		for(int i = 0; i < 8; i++) rData.MonitoredFiber[i] = MonitoredFiber[i];
		int nBeginNum = 0;
		rData.aFibrBase.SetSize(aFibrBase.GetSize());
		for(int i = 0; i < aFibrBase.GetSize(); i++)
		{
			rData.aFibrBase[i].FibrBaseKey = aFibrBase[i].FibrBaseKey;
			rData.aFibrBase[i].bRebar      = aFibrBase[i].bRebar;
			rData.aFibrBase[i].dArea       = aFibrBase[i].dArea;
			rData.aFibrBase[i].dCenterY    = aFibrBase[i].dCenterY;
			rData.aFibrBase[i].dCenterZ    = aFibrBase[i].dCenterZ;
			rData.aFibrBase[i].FibrMatlID  = aFibrBase[i].FibrMatlID;
			for(int j = 0; j < aFibrBase[i].nCountPoint; j++)
			{
				rData.aFibrBase[i].aPointY.Add(aTotalPointY[nBeginNum]);
				rData.aFibrBase[i].aPointZ.Add(aTotalPointZ[nBeginNum]);
				nBeginNum++;
			}      
		}
		rData.nSeq = nSeq;
	}  
};
struct T_FIBR_D_CH
{  
	char   strName[20];
	UINT   SectKey;
	CArray<T_FIBR_BASE, T_FIBR_BASE&> aFibrBase;
	T_FIMP_K FimpKey[3];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nSeq;
	void ConvertToString(T_FIBR_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.SectKey = SectKey;
		rData.aFibrBase.Copy(aFibrBase);
		memcpy(rData.FimpKey, FimpKey, sizeof(FimpKey));
		rData.bMonitoredFiber = bMonitoredFiber;
		memcpy(rData.MonitoredFiber, MonitoredFiber, sizeof(MonitoredFiber));
		rData.nSeq = nSeq;
	}
	void ConvertToChar(T_FIBR_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		SectKey = rData.SectKey;
		aFibrBase.Copy(rData.aFibrBase);
		memcpy(FimpKey, rData.FimpKey, sizeof(FimpKey));
		bMonitoredFiber = rData.bMonitoredFiber;
		memcpy(MonitoredFiber, rData.MonitoredFiber, sizeof(MonitoredFiber));
		nSeq = rData.nSeq;
	}
};
struct T_FIBR_D_CH_TEMP
{ 
	char   strName[20];
	UINT   SectKey;
	CArray<T_FIBR_BASE_RW, T_FIBR_BASE_RW&> aFibrBase;
	T_FIMP_K FimpKey[3];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	CArray<double, double> aTotalPointY;  // 각 fiber의 절점 array를 모두 모은 것 
	CArray<double, double> aTotalPointZ;  // 각 fiber의 절점 array를 모두 모은 것
	int    nSeq;
	void Set(T_FIBR_D_CH& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));    
		SectKey = rData.SectKey;
		for(int i = 0; i < 3; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];    
		aFibrBase.SetSize(rData.aFibrBase.GetSize());
		aTotalPointY.RemoveAll();
		aTotalPointZ.RemoveAll();
		for(int i = 0; i < rData.aFibrBase.GetSize(); i++)
		{
			aFibrBase[i].FibrBaseKey = rData.aFibrBase[i].FibrBaseKey;
			aFibrBase[i].bRebar      = rData.aFibrBase[i].bRebar;
			aFibrBase[i].dArea       = rData.aFibrBase[i].dArea;
			aFibrBase[i].dCenterY    = rData.aFibrBase[i].dCenterY;
			aFibrBase[i].dCenterZ    = rData.aFibrBase[i].dCenterZ;
			aFibrBase[i].FibrMatlID  = rData.aFibrBase[i].FibrMatlID;
			aFibrBase[i].nCountPoint = rData.aFibrBase[i].aPointY.GetSize();
			for(int j = 0; j < rData.aFibrBase[i].aPointY.GetSize(); j++)
			{
				aTotalPointY.Add(rData.aFibrBase[i].aPointY[j]);
				aTotalPointZ.Add(rData.aFibrBase[i].aPointZ[j]);        
			}      
		}
		nSeq = rData.nSeq;
	}
	void Get(T_FIBR_D_CH& rData)
	{
		memcpy(rData.strName, strName, sizeof(strName));    
		rData.SectKey = SectKey;
		for(int i = 0; i < 3; i++) rData.FimpKey[i] = FimpKey[i];
		rData.bMonitoredFiber = bMonitoredFiber;
		for(int i = 0; i < 8; i++) rData.MonitoredFiber[i] = MonitoredFiber[i];
		rData.aFibrBase.SetSize(aFibrBase.GetSize());
		int nBeginNum = 0;
		for(int i = 0; i < aFibrBase.GetSize(); i++)
		{
			rData.aFibrBase[i].FibrBaseKey = aFibrBase[i].FibrBaseKey;
			rData.aFibrBase[i].bRebar      = aFibrBase[i].bRebar;
			rData.aFibrBase[i].dArea       = aFibrBase[i].dArea;
			rData.aFibrBase[i].dCenterY    = aFibrBase[i].dCenterY;
			rData.aFibrBase[i].dCenterZ    = aFibrBase[i].dCenterZ;
			rData.aFibrBase[i].FibrMatlID  = aFibrBase[i].FibrMatlID;
			rData.aFibrBase[i].aPointY.RemoveAll();
			rData.aFibrBase[i].aPointZ.RemoveAll();
			for(int j = 0; j < aFibrBase[i].nCountPoint; j++)
			{
				rData.aFibrBase[i].aPointY.Add(aTotalPointY[nBeginNum]);
				rData.aFibrBase[i].aPointZ.Add(aTotalPointZ[nBeginNum]);
				nBeginNum++;
			}      
		}
		rData.nSeq = nSeq;
	}  
};
struct T_FIBR_UDRD_D
{
	T_FIBR_K key;
	T_FIBR_D data;
};
struct T_FIBR_UDRD_D_CH
{
	T_FIBR_K key;
	T_FIBR_D_CH data;
};
struct T_FIBR_UDRD_D_CH_RW
{   
	T_FIBR_K key;
	char   strName[20];
	UINT   SectKey;
	UINT   FimpKey[3];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nCountFibrBase;
	int    nCountTotalPoint;
	int    nSeq;
	void SetFibr(const T_FIBR_K Key, const T_FIBR_D_CH_TEMP& rData)
	{
		key = Key;
		memcpy(strName, rData.strName, sizeof(strName));    
		SectKey = rData.SectKey;
		for(int i = 0; i < 3; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];
		nCountFibrBase = rData.aFibrBase.GetSize();
		nCountTotalPoint = rData.aTotalPointY.GetSize();
		nSeq = rData.nSeq;
	}
	void GetFibr(T_FIBR_K& rKey, T_FIBR_D_CH_TEMP& rData)
	{
		rKey = key;
		memcpy(rData.strName, strName, sizeof(strName));    
		rData.SectKey = SectKey;
		for(int i = 0; i < 3; i++) rData.FimpKey[i] = FimpKey[i];
		rData.bMonitoredFiber = bMonitoredFiber;
		for(int i = 0; i < 8; i++) rData.MonitoredFiber[i] = MonitoredFiber[i];        
		rData.aFibrBase.RemoveAll();
		rData.aTotalPointY.RemoveAll();
		rData.aTotalPointZ.RemoveAll();
		rData.nSeq = nSeq;
	}  
};
*/

struct T_FIBR_BASE_660
{
	UINT   FibrBaseKey;
	BOOL   bRebar;
	CArray<double, double> aPointY;
	CArray<double, double> aPointZ;
	double dArea;
	double dCenterY;
	double dCenterZ;
	int    FibrMatlID;   // 0=Type1, 1=Type2, 2=Type3
};

struct T_FIBR_BASE_885
{
	UINT   FibrBaseKey;
	BOOL   bRebar;
	CArray<double, double> aPointY;
	CArray<double, double> aPointZ;
	double dArea;
	double dCenterY;
	double dCenterZ;
	CString strRebarName;
	int    FibrMatlID;   // 0=Type1, 1=Type2, 2=Type3
};

struct T_FIBR_BASE
{
	UINT   FibrBaseKey;
	BOOL   bRebar;
	CArray<double, double> aPointY;
	CArray<double, double> aPointZ;
	double dArea;
	double dCenterY;
	double dCenterZ;
	int    FibrMatlID;   // 0=Type1, 1=Type2, 2=Type3
	CString strRebarName;  // 6.7.0 추가
	double dAreaConsiderRebar; // bRebar 가 FALSE 일 경우만 씀. Fiber 영역에서 Rebar를 뺀 Area를 저장 함
	void Initialize()
	{
		FibrBaseKey = 0;
		bRebar = FALSE;
		aPointY.RemoveAll();
		aPointZ.RemoveAll();
		dArea = 0.;
		dCenterY = 0.;
		dCenterZ = 0.;
		FibrMatlID = 0;
		strRebarName.Empty();
		dAreaConsiderRebar = 0.0;
	}
	T_FIBR_BASE() {}
	T_FIBR_BASE(T_FIBR_BASE& src) {*this = src;}
	T_FIBR_BASE& operator=(const T_FIBR_BASE& src)
	{
		FibrBaseKey = src.FibrBaseKey;
		bRebar = src.bRebar;
		aPointY.Copy(src.aPointY);
		aPointZ.Copy(src.aPointZ);
		dArea = src.dArea;
		dCenterY = src.dCenterY;
		dCenterZ = src.dCenterZ;
		FibrMatlID = src.FibrMatlID;
		strRebarName = src.strRebarName;
		dAreaConsiderRebar = src.dAreaConsiderRebar;
		return *this;
	}
};
struct T_FIBR_BASE_CH_660
{
	UINT   FibrBaseKey;
	BOOL   bRebar;
	CArray<double, double> aPointY;
	CArray<double, double> aPointZ;
	double dArea;
	double dCenterY;
	double dCenterZ;
	int    FibrMatlID;   // 0=Type1, 1=Type2, 2=Type3
};

struct T_FIBR_BASE_CH_885
{
	UINT   FibrBaseKey;
	BOOL   bRebar;
	CArray<double, double> aPointY;
	CArray<double, double> aPointZ;
	double dArea;
	double dCenterY;
	double dCenterZ;
	int    FibrMatlID;   // 0=Type1, 1=Type2, 2=Type3
	char   strRebarName[20];
};

struct T_FIBR_BASE_CH
{
	UINT   FibrBaseKey;
	BOOL   bRebar;
	CArray<double, double> aPointY;
	CArray<double, double> aPointZ;
	double dArea;
	double dCenterY;
	double dCenterZ;
	int    FibrMatlID;   // 0=Type1, 1=Type2, 2=Type3
	char   strRebarName[20];  // 6.7.0 추가
	double dAreaConsiderRebar;

	void ConvertToString(T_FIBR_BASE& rData)
	{
		rData.FibrBaseKey = FibrBaseKey;
		rData.bRebar = bRebar;
		rData.aPointY.Copy(aPointY);
		rData.aPointZ.Copy(aPointZ);
		rData.dArea = dArea;
		rData.dCenterY = dCenterY;
		rData.dCenterZ = dCenterZ;
		rData.FibrMatlID = FibrMatlID;
		ConvertCharStr(strRebarName, rData.strRebarName, sizeof(strRebarName));
		rData.dAreaConsiderRebar = dAreaConsiderRebar;
	}
	void ConvertToChar(T_FIBR_BASE& rData)
	{
		FibrBaseKey = rData.FibrBaseKey;
		bRebar = rData.bRebar;
		aPointY.Copy(rData.aPointY);
		aPointZ.Copy(rData.aPointZ);
		dArea = rData.dArea;
		dCenterY = rData.dCenterY;
		dCenterZ = rData.dCenterZ;
		FibrMatlID = rData.FibrMatlID;    
		ConvertStrChar(rData.strRebarName, strRebarName, sizeof(strRebarName));
		dAreaConsiderRebar = rData.dAreaConsiderRebar;
	}
	void Convert660(T_FIBR_BASE_CH_660& rData)
	{    
		FibrBaseKey = rData.FibrBaseKey;
		bRebar = rData.bRebar;
		aPointY.Copy(rData.aPointY);
		aPointZ.Copy(rData.aPointZ);
		dArea = rData.dArea;
		dCenterY = rData.dCenterY;
		dCenterZ = rData.dCenterZ;
		FibrMatlID = rData.FibrMatlID;  
		strRebarName[0] = '\0';
		dAreaConsiderRebar = dArea;
	}
	void Convert885(T_FIBR_BASE_CH_885& rData)
	{    
		FibrBaseKey = rData.FibrBaseKey;
		bRebar = rData.bRebar;
		aPointY.Copy(rData.aPointY);
		aPointZ.Copy(rData.aPointZ);
		dArea = rData.dArea;
		dCenterY = rData.dCenterY;
		dCenterZ = rData.dCenterZ;
		FibrMatlID = rData.FibrMatlID;  
		for (int i = 0; i < 20; i++)
		{
			strRebarName[i] = rData.strRebarName[i];
		}
		dAreaConsiderRebar = dArea;
	}
};  
struct T_FIBR_BASE_CH_RW_660
{
	UINT   FibrBaseKey;
	BOOL   bRebar;
	int    nCountPoint;  
	double dArea;
	double dCenterY;
	double dCenterZ;
	int    FibrMatlID;   // 0=Type1, 1=Type2, 2=Type3  
};
struct T_FIBR_BASE_CH_RW_885
{
	UINT   FibrBaseKey;
	BOOL   bRebar;
	int    nCountPoint;  
	double dArea;
	double dCenterY;
	double dCenterZ;
	int    FibrMatlID;   // 0=Type1, 1=Type2, 2=Type3  
	char   strRebarName[20];
};
struct T_FIBR_BASE_CH_RW
{
	UINT   FibrBaseKey;
	BOOL   bRebar;
	int    nCountPoint;  
	double dArea;
	double dCenterY;
	double dCenterZ;
	int    FibrMatlID;   // 0=Type1, 1=Type2, 2=Type3
	char   strRebarName[20];  // 6.7.0 추가
	double dAreaConsiderRebar;
	void Get(T_FIBR_BASE_CH& rData)
	{
		rData.FibrBaseKey = FibrBaseKey;
		rData.bRebar = bRebar;
		rData.dArea = dArea;
		rData.dCenterY = dCenterY;
		rData.dCenterZ = dCenterZ;
		rData.FibrMatlID = FibrMatlID;
		memcpy(rData.strRebarName, strRebarName, sizeof(strRebarName));
		rData.dAreaConsiderRebar = dAreaConsiderRebar;		
	}
	void Set(const T_FIBR_BASE_CH& rData)
	{
		FibrBaseKey = rData.FibrBaseKey;
		bRebar = rData.bRebar;
		nCountPoint = (int)rData.aPointY.GetSize();    
		dArea = rData.dArea;
		dCenterY = rData.dCenterY;
		dCenterZ = rData.dCenterZ;
		FibrMatlID = rData.FibrMatlID;
		memcpy(strRebarName, rData.strRebarName, sizeof(strRebarName));
		dAreaConsiderRebar = rData.dAreaConsiderRebar;
	}
	void Convert660(T_FIBR_BASE_CH_RW_660& rData)
	{
		FibrBaseKey = rData.FibrBaseKey;
		bRebar = rData.bRebar;
		nCountPoint = rData.nCountPoint;    
		dArea = rData.dArea;
		dCenterY = rData.dCenterY;
		dCenterZ = rData.dCenterZ;
		FibrMatlID = rData.FibrMatlID;    
		strRebarName[0] = '\0';
		dAreaConsiderRebar = dArea;
	}
	void Convert885(T_FIBR_BASE_CH_RW_885& rData)
	{
		FibrBaseKey = rData.FibrBaseKey;
		bRebar = rData.bRebar;
		nCountPoint = rData.nCountPoint;    
		dArea = rData.dArea;
		dCenterY = rData.dCenterY;
		dCenterZ = rData.dCenterZ;
		FibrMatlID = rData.FibrMatlID;    
		for (int i = 0; i < 20; i++)
		{
			strRebarName[i] = rData.strRebarName[i];
		}
		dAreaConsiderRebar = dArea;
	}
};

struct T_FIBR_BASE_POS_CH_RW
{
	double dPosY;
	double dPosZ;
	T_FIBR_BASE_POS_CH_RW() : dPosY(0.0), dPosZ(0.0) {}
	T_FIBR_BASE_POS_CH_RW(double dYpos, double dZpos ) : dPosY(dYpos), dPosZ(dZpos) {}
};

struct T_FIBR_BASE_ARRAY_INDEX_RW_970
{
	UINT nFibrK;

	int nBaseSt;
	int nBaseSize;
	int nPosSt;
	int nPosSize;

	T_FIBR_BASE_ARRAY_INDEX_RW_970() : nFibrK(0), nBaseSt(0), nBaseSize(0), nPosSt(0), nPosSize(0) {}
};

struct T_FIBR_BASE_ARRAY_INDEX_RW
{
	UINT nFibrK;

	int nBaseSt;
	int nBaseSize;
	int nPosSt;
	int nPosSize;

	int nBaseStJ;
	int nBaseSizeJ;
	int nPosStJ;
	int nPosSizeJ;

	T_FIBR_BASE_ARRAY_INDEX_RW() : nFibrK(0), nBaseSt(0),  nBaseSize(0),  nPosSt(0),  nPosSize(0),
	                                          nBaseStJ(0), nBaseSizeJ(0), nPosStJ(0), nPosSizeJ(0) {}

	void Convert970(const T_FIBR_BASE_ARRAY_INDEX_RW_970& rData)
	{
		nFibrK    = rData.nFibrK;

		nBaseSt   = rData.nBaseSt;
		nBaseSize = rData.nBaseSize;
		nPosSt    = rData.nPosSt;
		nPosSize  = rData.nPosSize;

		nBaseStJ   = 0;
		nBaseSizeJ = 0;
		nPosStJ    = 0;
		nPosSizeJ  = 0;
	}
};

struct T_FIBR_D_660
{  
	CString strName;
	UINT   SectKey;
	CArray<T_FIBR_BASE_660, T_FIBR_BASE_660&> aFibrBase;
	T_FIMP_K FimpKey[3];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nSeq;
};
struct T_FIBR_D_692
{  
	CString strName;
	UINT   SectKey;
	CArray<T_FIBR_BASE, T_FIBR_BASE&> aFibrBase;
	T_FIMP_K FimpKey[3];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nSeq;
};
struct T_FIBR_D_860
{  
	CString strName;
	UINT   SectKey;
	CArray<T_FIBR_BASE, T_FIBR_BASE&> aFibrBase;
	T_FIMP_K FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
															 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nSeq;
};

struct T_FIBR_D_950
{
	CString strName;
	UINT   SectKey;
	CArray<T_FIBR_BASE, T_FIBR_BASE&> aFibrBase;
	T_FIMP_K FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	// FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nSeq;
	int    nAssignType; // D_FIBR_TYPE
};

struct T_FIBR_D_970
{  
	CString strName;
	UINT   SectKey;
	CArray<T_FIBR_BASE, T_FIBR_BASE&> aFibrBase;
	T_FIMP_K FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
															 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nSeq;
	int    nAssignType; // D_FIBR_TYPE
	COLORREF FimpColor[6];
};

enum D_FIBR_DLG_TYPE { D_FIBR_DLG_TYPE_IEHP_BEAM, D_FIBR_DLG_TYPE_IEHP_WALL, D_FIBR_DLG_TYPE_PHGT_BEAM, D_FIBR_DLG_TYPE_PHGT_WALL };
struct T_FIBR_D
{  
	CString strName;
	UINT   SectKey;
	CArray<T_FIBR_BASE, T_FIBR_BASE&> aFibrBase;
	CArray<T_FIBR_BASE, T_FIBR_BASE&> aFibrBaseJ; // cell information for J end
	T_FIMP_K FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
															 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nSeq;
	int    nAssignType; // D_FIBR_TYPE
	COLORREF FimpColor[6];
	UINT GrupKey;
	UINT ElemKey;
	void Initialize()
	{
		strName.Empty();
		SectKey = 0;
		aFibrBase.RemoveAll();
		aFibrBaseJ.RemoveAll();
		for(int i = 0; i < 6; i++) FimpKey[i] = 0;
		bMonitoredFiber = TRUE;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = 0;
		nSeq = 0;
		nAssignType = D_FIBR_TYPE_IEHP;
		FimpColor[0] = RGB(121, 198, 220);
		FimpColor[1] = RGB(245, 242, 31);
		FimpColor[2] = RGB(214, 126, 176);
		FimpColor[3] = RGB(63, 174, 179);
		FimpColor[4] = RGB(40, 130, 89);
		FimpColor[5] = RGB(223, 176, 192);
		GrupKey = 0;
		ElemKey = 0;
	}
	T_FIBR_D() {}
	T_FIBR_D(T_FIBR_D& src) {*this = src;}
	T_FIBR_D& operator=(const T_FIBR_D& src)
	{
		strName = src.strName;
		SectKey = src.SectKey;
		aFibrBase.Copy(src.aFibrBase);
		aFibrBaseJ.Copy(src.aFibrBaseJ);
		memcpy(FimpKey, src.FimpKey, sizeof(FimpKey));
		memcpy(MonitoredFiber, src.MonitoredFiber, sizeof(MonitoredFiber));
		bMonitoredFiber = src.bMonitoredFiber;
		nSeq = src.nSeq;
		nAssignType = src.nAssignType;
		memcpy(FimpColor, src.FimpColor, sizeof(FimpColor));
		GrupKey = src.GrupKey;
		ElemKey = src.ElemKey;
		return *this;
	}
};
struct T_FIBR_D_CH_660
{  
	char   strName[20];
	UINT   SectKey;
	CArray<T_FIBR_BASE_CH_660, T_FIBR_BASE_CH_660&> aFibrBase;
	T_FIMP_K FimpKey[3];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nSeq;
};
struct T_FIBR_D_CH_692
{  
	char   strName[20];
	UINT   SectKey;
	CArray<T_FIBR_BASE_CH, T_FIBR_BASE_CH&> aFibrBase;
	T_FIMP_K FimpKey[3];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nSeq;
};
struct T_FIBR_D_CH_860
{  
	char   strName[20];
	UINT   SectKey;
	CArray<T_FIBR_BASE_CH, T_FIBR_BASE_CH&> aFibrBase;
	T_FIMP_K FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
															 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nSeq;
};

struct T_FIBR_D_CH_950
{
	char   strName[40];
	UINT   SectKey;
	CArray<T_FIBR_BASE_CH, T_FIBR_BASE_CH&> aFibrBase;
	T_FIMP_K FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	// FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nSeq;
	int    nAssignType; // D_FIBR_TYPE
};
struct T_FIBR_D_CH
{  
	char   strName[40];
	UINT   SectKey;
	CArray<T_FIBR_BASE_CH, T_FIBR_BASE_CH&> aFibrBase;
	CArray<T_FIBR_BASE_CH, T_FIBR_BASE_CH&> aFibrBaseJ; // cell information for J end
	T_FIMP_K FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
															 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nSeq;
	int    nAssignType; // D_FIBR_TYPE
	COLORREF FimpColor[6];
	UINT   GrupKey;
	UINT   ElemKey;

	void ConvertToString(T_FIBR_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.SectKey = SectKey;
		auto ConvertFibrBase = []( CArray<T_FIBR_BASE_CH, T_FIBR_BASE_CH&>& ch_aFibrBase,
			CArray<T_FIBR_BASE, T_FIBR_BASE&>& rData_aFibrBase) {
			rData_aFibrBase.SetSize(ch_aFibrBase.GetSize());
			for(int i = 0; i < ch_aFibrBase.GetSize(); i++)
			{
				rData_aFibrBase[i].aPointY.SetSize(ch_aFibrBase[i].aPointY.GetSize());
				rData_aFibrBase[i].aPointZ.SetSize(ch_aFibrBase[i].aPointZ.GetSize());
				rData_aFibrBase[i].FibrBaseKey = ch_aFibrBase[i].FibrBaseKey;
				rData_aFibrBase[i].bRebar      = ch_aFibrBase[i].bRebar;
				rData_aFibrBase[i].dArea       = ch_aFibrBase[i].dArea;
				rData_aFibrBase[i].dCenterY    = ch_aFibrBase[i].dCenterY;
				rData_aFibrBase[i].dCenterZ    = ch_aFibrBase[i].dCenterZ;
				rData_aFibrBase[i].FibrMatlID  = ch_aFibrBase[i].FibrMatlID;
				rData_aFibrBase[i].aPointY.Copy(ch_aFibrBase[i].aPointY);
				rData_aFibrBase[i].aPointZ.Copy(ch_aFibrBase[i].aPointZ);
				ConvertCharStr(ch_aFibrBase[i].strRebarName, rData_aFibrBase[i].strRebarName, sizeof(ch_aFibrBase[i].strRebarName));
			}
			};
		ConvertFibrBase(aFibrBase, rData.aFibrBase );
		ConvertFibrBase(aFibrBaseJ,rData.aFibrBaseJ);
		memcpy(rData.FimpKey, FimpKey, sizeof(FimpKey));
		rData.bMonitoredFiber = bMonitoredFiber;
		memcpy(rData.MonitoredFiber, MonitoredFiber, sizeof(MonitoredFiber));
		rData.nSeq = nSeq;
		rData.nAssignType = nAssignType;
		memcpy(rData.FimpColor, FimpColor, sizeof(FimpColor));
		rData.GrupKey = GrupKey;
		rData.ElemKey = ElemKey;
	}
	void ConvertToChar(T_FIBR_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		SectKey = rData.SectKey;
		auto ConvertFibrBase = [](CArray<T_FIBR_BASE, T_FIBR_BASE&>& rData_aFibrBase,
			CArray<T_FIBR_BASE_CH, T_FIBR_BASE_CH&>& ch_aFibrBase) {
			ch_aFibrBase.SetSize(rData_aFibrBase.GetSize());
			for(int i = 0; i < ch_aFibrBase.GetSize(); i++)
			{
				ch_aFibrBase[i].aPointY.SetSize(rData_aFibrBase[i].aPointY.GetSize());
				ch_aFibrBase[i].aPointZ.SetSize(rData_aFibrBase[i].aPointZ.GetSize());
				ch_aFibrBase[i].FibrBaseKey = rData_aFibrBase[i].FibrBaseKey;
				ch_aFibrBase[i].bRebar      = rData_aFibrBase[i].bRebar;
				ch_aFibrBase[i].dArea       = rData_aFibrBase[i].dArea;
				ch_aFibrBase[i].dCenterY    = rData_aFibrBase[i].dCenterY;
				ch_aFibrBase[i].dCenterZ    = rData_aFibrBase[i].dCenterZ;
				ch_aFibrBase[i].FibrMatlID  = rData_aFibrBase[i].FibrMatlID;
				ch_aFibrBase[i].aPointY.Copy(rData_aFibrBase[i].aPointY);
				ch_aFibrBase[i].aPointZ.Copy(rData_aFibrBase[i].aPointZ);
				ConvertStrChar(rData_aFibrBase[i].strRebarName, ch_aFibrBase[i].strRebarName, sizeof(rData_aFibrBase[i].strRebarName));
			}
			};
		ConvertFibrBase(rData.aFibrBase, aFibrBase);
		ConvertFibrBase(rData.aFibrBaseJ,aFibrBaseJ);
		memcpy(FimpKey, rData.FimpKey, sizeof(FimpKey));
		bMonitoredFiber = rData.bMonitoredFiber;
		memcpy(MonitoredFiber, rData.MonitoredFiber, sizeof(MonitoredFiber));
		nSeq = rData.nSeq;
		nAssignType = rData.nAssignType;
		memcpy(FimpColor, rData.FimpColor, sizeof(FimpColor));
		GrupKey = rData.GrupKey;
		ElemKey = rData.ElemKey;
	}
};
struct T_FIBR_D_CH_TEMP_885
{ 
	char   strName[20];
	UINT   SectKey;
	CArray<T_FIBR_BASE_CH_RW_885, T_FIBR_BASE_CH_RW_885&> aFibrBase;
	T_FIMP_K FimpKey[3];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	CArray<double, double> aTotalPointY;  // 각 fiber의 절점 array를 모두 모은 것 
	CArray<double, double> aTotalPointZ;  // 각 fiber의 절점 array를 모두 모은 것
	int    nSeq;
};
struct T_FIBR_D_CH_TEMP_660
{ 
	char   strName[20];
	UINT   SectKey;
	CArray<T_FIBR_BASE_CH_RW_660, T_FIBR_BASE_CH_RW_660&> aFibrBase;
	T_FIMP_K FimpKey[3];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	CArray<double, double> aTotalPointY;  // 각 fiber의 절점 array를 모두 모은 것 
	CArray<double, double> aTotalPointZ;  // 각 fiber의 절점 array를 모두 모은 것
	int    nSeq;
};
struct T_FIBR_D_CH_TEMP_692
{ 
	char   strName[20];
	UINT   SectKey;
	CArray<T_FIBR_BASE_CH_RW, T_FIBR_BASE_CH_RW&> aFibrBase;
	T_FIMP_K FimpKey[3];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	CArray<double, double> aTotalPointY;  // 각 fiber의 절점 array를 모두 모은 것 
	CArray<double, double> aTotalPointZ;  // 각 fiber의 절점 array를 모두 모은 것
	int    nSeq;
};
struct T_FIBR_D_CH_TEMP_860
{ 
	char   strName[20];
	UINT   SectKey;
	CArray<T_FIBR_BASE_CH_RW, T_FIBR_BASE_CH_RW&> aFibrBase;
	T_FIMP_K FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
															 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	CArray<double, double> aTotalPointY;  // 각 fiber의 절점 array를 모두 모은 것 
	CArray<double, double> aTotalPointZ;  // 각 fiber의 절점 array를 모두 모은 것
	int    nSeq;
};

struct T_FIBR_D_CH_TEMP_940
{
	char   strName[40];
	UINT   SectKey;
	CArray<T_FIBR_BASE_CH_RW, T_FIBR_BASE_CH_RW&> aFibrBase;
	T_FIMP_K FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
															 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	CArray<double, double> aTotalPointY;  // 각 fiber의 절점 array를 모두 모은 것 
	CArray<double, double> aTotalPointZ;  // 각 fiber의 절점 array를 모두 모은 것
	int    nSeq;
	int    nAssignType;
};

struct T_FIBR_D_CH_TEMP_970
{
	char   strName[40];
	UINT   SectKey;
	CArray<T_FIBR_BASE_CH_RW, T_FIBR_BASE_CH_RW&> aFibrBase;
	T_FIMP_K FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
															 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	CArray<T_FIBR_BASE_POS_CH_RW, T_FIBR_BASE_POS_CH_RW&> aTotalPoint;  // 각 fiber의 절점 array를 모두 모은 것
	int    nSeq;
	int    nAssignType;
	COLORREF FimpColor[6];
};

struct T_FIBR_D_CH_TEMP
{ 
	char   strName[40];
	UINT   SectKey;
	CArray<T_FIBR_BASE_CH_RW, T_FIBR_BASE_CH_RW&> aFibrBase;
	CArray<T_FIBR_BASE_CH_RW, T_FIBR_BASE_CH_RW&> aFibrBaseJ;
	T_FIMP_K FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
															 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	CArray<T_FIBR_BASE_POS_CH_RW, T_FIBR_BASE_POS_CH_RW&> aTotalPoint;  // 각 fiber의 절점 array를 모두 모은 것
	CArray<T_FIBR_BASE_POS_CH_RW, T_FIBR_BASE_POS_CH_RW&> aTotalPointJ;  // 각 fiber의 절점 array를 모두 모은 것
	int    nSeq;
	int    nAssignType;
	COLORREF FimpColor[6];
	UINT GrupKey;
	UINT ElemKey;
	void Set(T_FIBR_D_CH& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));    
		SectKey = rData.SectKey;
		for(int i = 0; i < 6; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];    
		auto SetFibrBase = [](CArray<T_FIBR_BASE_CH, T_FIBR_BASE_CH&>& rData_aFibrBase,
			CArray<T_FIBR_BASE_CH_RW, T_FIBR_BASE_CH_RW&>& ch_aFibrBase, CArray<T_FIBR_BASE_POS_CH_RW,
			T_FIBR_BASE_POS_CH_RW&>& ch_aTotalPoint) {
				ch_aFibrBase.SetSize(rData_aFibrBase.GetSize());
				ch_aTotalPoint.RemoveAll();
				for(int i = 0; i < rData_aFibrBase.GetSize(); i++)
				{
					ch_aFibrBase[i].FibrBaseKey = rData_aFibrBase[i].FibrBaseKey;
					ch_aFibrBase[i].bRebar      = rData_aFibrBase[i].bRebar;
					ch_aFibrBase[i].dArea       = rData_aFibrBase[i].dArea;
					ch_aFibrBase[i].dCenterY    = rData_aFibrBase[i].dCenterY;
					ch_aFibrBase[i].dCenterZ    = rData_aFibrBase[i].dCenterZ;
					ch_aFibrBase[i].FibrMatlID  = rData_aFibrBase[i].FibrMatlID;
					ch_aFibrBase[i].nCountPoint = (int)rData_aFibrBase[i].aPointY.GetSize();
					memcpy(ch_aFibrBase[i].strRebarName, rData_aFibrBase[i].strRebarName, sizeof(ch_aFibrBase[i].strRebarName));

					for(int j = 0; j < rData_aFibrBase[i].aPointY.GetSize(); j++)
					{
						T_FIBR_BASE_POS_CH_RW TotalPos(rData_aFibrBase[i].aPointY[j], rData_aFibrBase[i].aPointZ[j]);
						ch_aTotalPoint.Add(TotalPos);
					}
				}
			};
		SetFibrBase(rData.aFibrBase,  aFibrBase,  aTotalPoint);
		SetFibrBase(rData.aFibrBaseJ, aFibrBaseJ, aTotalPointJ);
		nSeq = rData.nSeq;
		nAssignType = rData.nAssignType;
		memcpy(FimpColor, rData.FimpColor, sizeof(FimpColor));
		GrupKey = rData.GrupKey;
		ElemKey = rData.ElemKey;
	}
	void Get(T_FIBR_D_CH& rData)
	{
		memcpy(rData.strName, strName, sizeof(strName));    
		rData.SectKey = SectKey;
		for(int i = 0; i < 6; i++) rData.FimpKey[i] = FimpKey[i];
		rData.bMonitoredFiber = bMonitoredFiber;
		for(int i = 0; i < 8; i++) rData.MonitoredFiber[i] = MonitoredFiber[i];
		auto GetFibrBase = []( CArray<T_FIBR_BASE_CH_RW, T_FIBR_BASE_CH_RW&>& ch_aFibrBase, CArray<T_FIBR_BASE_POS_CH_RW,
			T_FIBR_BASE_POS_CH_RW&>& ch_aTotalPoint,
			CArray<T_FIBR_BASE_CH, T_FIBR_BASE_CH&>& rData_aFibrBase) {
				rData_aFibrBase.SetSize(ch_aFibrBase.GetSize());
				int nBeginNum = 0;
				for(int i = 0; i < ch_aFibrBase.GetSize(); i++)
				{
					rData_aFibrBase[i].FibrBaseKey = ch_aFibrBase[i].FibrBaseKey;
					rData_aFibrBase[i].bRebar      = ch_aFibrBase[i].bRebar;
					rData_aFibrBase[i].dArea       = ch_aFibrBase[i].dArea;
					rData_aFibrBase[i].dCenterY    = ch_aFibrBase[i].dCenterY;
					rData_aFibrBase[i].dCenterZ    = ch_aFibrBase[i].dCenterZ;
					rData_aFibrBase[i].FibrMatlID  = ch_aFibrBase[i].FibrMatlID;
					rData_aFibrBase[i].aPointY.RemoveAll();
					rData_aFibrBase[i].aPointZ.RemoveAll();
					memcpy(rData_aFibrBase[i].strRebarName, ch_aFibrBase[i].strRebarName, sizeof(ch_aFibrBase[i].strRebarName));
					for(int j = 0; j < ch_aFibrBase[i].nCountPoint; j++)
					{
						rData_aFibrBase[i].aPointY.Add(ch_aTotalPoint[nBeginNum].dPosY);
						rData_aFibrBase[i].aPointZ.Add(ch_aTotalPoint[nBeginNum].dPosZ);
						nBeginNum++;
					}
				}
			};
		GetFibrBase(aFibrBase,  aTotalPoint,  rData.aFibrBase);
		GetFibrBase(aFibrBaseJ, aTotalPointJ, rData.aFibrBaseJ);
		rData.nSeq = nSeq;
		rData.nAssignType = nAssignType;
		memcpy(rData.FimpColor, FimpColor, sizeof(FimpColor));
		rData.GrupKey = GrupKey;
		rData.ElemKey = ElemKey;
	}
	void Convert970(T_FIBR_D_CH_TEMP_970& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));    
		SectKey = rData.SectKey;
		for(int i = 0; i < 6; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];    
		aFibrBase.SetSize(rData.aFibrBase.GetSize());
		for(int i = 0; i < rData.aFibrBase.GetSize(); i++)
		{
			aFibrBase[i].FibrBaseKey = rData.aFibrBase[i].FibrBaseKey;
			aFibrBase[i].bRebar      = rData.aFibrBase[i].bRebar;
			aFibrBase[i].dArea       = rData.aFibrBase[i].dArea;
			aFibrBase[i].dCenterY    = rData.aFibrBase[i].dCenterY;
			aFibrBase[i].dCenterZ    = rData.aFibrBase[i].dCenterZ;
			aFibrBase[i].FibrMatlID  = rData.aFibrBase[i].FibrMatlID;  
			aFibrBase[i].nCountPoint = rData.aFibrBase[i].nCountPoint;      
		}

		aTotalPoint.SetSize(rData.aTotalPoint.GetSize());
		for (int j = 0; j < rData.aTotalPoint.GetSize(); j++)
			aTotalPoint[j]=rData.aTotalPoint[j];

		nSeq = rData.nSeq;
		nAssignType = D_FIBR_TYPE_IEHP;
		memcpy(FimpColor, rData.FimpColor, sizeof(FimpColor));
		GrupKey = 0;
		ElemKey = 0;
	}
	void Convert860(T_FIBR_D_CH_TEMP_940& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));
		SectKey = rData.SectKey;
		for (int i = 0; i < 6; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for (int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];
		aFibrBase.SetSize(rData.aFibrBase.GetSize());
		for (int i = 0; i < rData.aFibrBase.GetSize(); i++)
		{
			aFibrBase[i].FibrBaseKey = rData.aFibrBase[i].FibrBaseKey;
			aFibrBase[i].bRebar = rData.aFibrBase[i].bRebar;
			aFibrBase[i].dArea = rData.aFibrBase[i].dArea;
			aFibrBase[i].dCenterY = rData.aFibrBase[i].dCenterY;
			aFibrBase[i].dCenterZ = rData.aFibrBase[i].dCenterZ;
			aFibrBase[i].FibrMatlID = rData.aFibrBase[i].FibrMatlID;
			aFibrBase[i].nCountPoint = rData.aFibrBase[i].nCountPoint;
		}
		aTotalPoint.SetSize(rData.aTotalPointY.GetSize());
		for (int j = 0; j < rData.aTotalPointY.GetSize(); j++)
		{
			aTotalPoint[j].dPosY = rData.aTotalPointY[j];
			aTotalPoint[j].dPosZ = rData.aTotalPointZ[j];
		}

		nSeq = rData.nSeq;
		nAssignType = rData.nAssignType;
		GrupKey = 0;
		ElemKey = 0;
	}
	void Convert860(T_FIBR_D_CH_TEMP_860& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));    
		SectKey = rData.SectKey;
		for(int i = 0; i < 6; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];    
		aFibrBase.SetSize(rData.aFibrBase.GetSize());
		for(int i = 0; i < rData.aFibrBase.GetSize(); i++)
		{
			aFibrBase[i].FibrBaseKey = rData.aFibrBase[i].FibrBaseKey;
			aFibrBase[i].bRebar      = rData.aFibrBase[i].bRebar;
			aFibrBase[i].dArea       = rData.aFibrBase[i].dArea;
			aFibrBase[i].dCenterY    = rData.aFibrBase[i].dCenterY;
			aFibrBase[i].dCenterZ    = rData.aFibrBase[i].dCenterZ;
			aFibrBase[i].FibrMatlID  = rData.aFibrBase[i].FibrMatlID;  
			aFibrBase[i].nCountPoint = rData.aFibrBase[i].nCountPoint;      
		}
		aTotalPoint.SetSize(rData.aTotalPointY.GetSize());
		for (int j = 0; j < rData.aTotalPointY.GetSize(); j++)
		{
			aTotalPoint[j].dPosY = rData.aTotalPointY[j];
			aTotalPoint[j].dPosZ = rData.aTotalPointZ[j];
		}

		nSeq = rData.nSeq;
		nAssignType = D_FIBR_TYPE_IEHP;
		GrupKey = 0;
		ElemKey = 0;
	}
	void Convert692(T_FIBR_D_CH_TEMP_692& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));    
		SectKey = rData.SectKey;
		for(int i = 0; i < 3; i++) FimpKey[i] = rData.FimpKey[i];
		FimpKey[3] = FimpKey[4] = FimpKey[5] = 0;
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];    
		aFibrBase.SetSize(rData.aFibrBase.GetSize());
		for(int i = 0; i < rData.aFibrBase.GetSize(); i++)
		{
			aFibrBase[i].FibrBaseKey = rData.aFibrBase[i].FibrBaseKey;
			aFibrBase[i].bRebar      = rData.aFibrBase[i].bRebar;
			aFibrBase[i].dArea       = rData.aFibrBase[i].dArea;
			aFibrBase[i].dCenterY    = rData.aFibrBase[i].dCenterY;
			aFibrBase[i].dCenterZ    = rData.aFibrBase[i].dCenterZ;
			aFibrBase[i].FibrMatlID  = rData.aFibrBase[i].FibrMatlID;  
			aFibrBase[i].nCountPoint = rData.aFibrBase[i].nCountPoint;      
		}
		aTotalPoint.SetSize(rData.aTotalPointY.GetSize());
		for (int j = 0; j < rData.aTotalPointY.GetSize(); j++)
		{
			aTotalPoint[j].dPosY = rData.aTotalPointY[j];
			aTotalPoint[j].dPosZ = rData.aTotalPointZ[j];
		}
		nSeq = rData.nSeq;
		nAssignType = D_FIBR_TYPE_IEHP;
		GrupKey = 0;
		ElemKey = 0;
	}
	void Convert660(T_FIBR_D_CH_TEMP_660& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));    
		SectKey = rData.SectKey;
		for(int i = 0; i < 3; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];    
		aFibrBase.SetSize(rData.aFibrBase.GetSize());
		for(int i = 0; i < rData.aFibrBase.GetSize(); i++)
		{
			aFibrBase[i].FibrBaseKey = rData.aFibrBase[i].FibrBaseKey;
			aFibrBase[i].bRebar      = rData.aFibrBase[i].bRebar;
			aFibrBase[i].dArea       = rData.aFibrBase[i].dArea;
			aFibrBase[i].dCenterY    = rData.aFibrBase[i].dCenterY;
			aFibrBase[i].dCenterZ    = rData.aFibrBase[i].dCenterZ;
			aFibrBase[i].FibrMatlID  = rData.aFibrBase[i].FibrMatlID;  
			aFibrBase[i].nCountPoint = rData.aFibrBase[i].nCountPoint;      
		}
		aTotalPoint.SetSize(rData.aTotalPointY.GetSize());
		for (int j = 0; j < rData.aTotalPointY.GetSize(); j++)
		{
			aTotalPoint[j].dPosY = rData.aTotalPointY[j];
			aTotalPoint[j].dPosZ = rData.aTotalPointZ[j];
		}		
		nSeq = rData.nSeq;
		nAssignType = D_FIBR_TYPE_IEHP;
		GrupKey = 0;
		ElemKey = 0;
	}
	void Convert885(T_FIBR_D_CH_TEMP_885& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));    
		SectKey = rData.SectKey;
		for(int i = 0; i < 3; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];    
		aFibrBase.SetSize(rData.aFibrBase.GetSize());
		for(int i = 0; i < rData.aFibrBase.GetSize(); i++)
		{
			aFibrBase[i].FibrBaseKey = rData.aFibrBase[i].FibrBaseKey;
			aFibrBase[i].bRebar      = rData.aFibrBase[i].bRebar;
			aFibrBase[i].dArea       = rData.aFibrBase[i].dArea;
			aFibrBase[i].dCenterY    = rData.aFibrBase[i].dCenterY;
			aFibrBase[i].dCenterZ    = rData.aFibrBase[i].dCenterZ;
			aFibrBase[i].FibrMatlID  = rData.aFibrBase[i].FibrMatlID;  
			aFibrBase[i].nCountPoint = rData.aFibrBase[i].nCountPoint;      
		}
		aTotalPoint.SetSize(rData.aTotalPointY.GetSize());
		for (int j = 0; j < rData.aTotalPointY.GetSize(); j++)
		{
			aTotalPoint[j].dPosY = rData.aTotalPointY[j];
			aTotalPoint[j].dPosZ = rData.aTotalPointZ[j];
		}
		nSeq = rData.nSeq;
		nAssignType = D_FIBR_TYPE_IEHP;
		GrupKey = 0;
		ElemKey = 0;
	}
};
struct T_FIBR_UDRD_D_660
{
	T_FIBR_K key;
	T_FIBR_D_660 data;
};
struct T_FIBR_UDRD_D_692
{
	T_FIBR_K key;
	T_FIBR_D_692 data;
};
struct T_FIBR_UDRD_D_860
{
	T_FIBR_K key;
	T_FIBR_D_692 data;
};
struct T_FIBR_UDRD_D
{
	T_FIBR_K key;
	T_FIBR_D data;
};
struct T_FIBR_UDRD_D_CH_660
{
	T_FIBR_K key;
	T_FIBR_D_CH_660 data;
};
struct T_FIBR_UDRD_D_CH_692
{
	T_FIBR_K key;
	T_FIBR_D_CH_692 data;
};
struct T_FIBR_UDRD_D_CH_860
{
	T_FIBR_K key;
	T_FIBR_D_CH_692 data;
};
struct T_FIBR_UDRD_D_CH
{
	T_FIBR_K key;
	T_FIBR_D_CH data;
};
struct T_FIBR_UDRD_D_CH_RW_660
{   
	T_FIBR_K key;
	char   strName[20];
	UINT   SectKey;
	UINT   FimpKey[3];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nCountFibrBase;
	int    nCountTotalPoint;
	int    nSeq;
};
struct T_FIBR_UDRD_D_CH_RW_692
{   
	T_FIBR_K key;
	char   strName[20];
	UINT   SectKey;
	UINT   FimpKey[3];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nCountFibrBase;
	int    nCountTotalPoint;
	int    nSeq;
	int    nTemp;              // Size 구별하기 위한 임시 변수
};
struct T_FIBR_UDRD_D_CH_RW_860
{   
	T_FIBR_K key;
	char   strName[20];
	UINT   SectKey;
	UINT   FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
														 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nCountFibrBase;
	int    nCountTotalPoint;
	int    nSeq;
	int    nTemp;              // Size 구별하기 위한 임시 변수
};
struct T_FIBR_UDRD_D_CH_RW_865
{   
	T_FIBR_K key;
	char   strName[20];
	UINT   SectKey;
	UINT   FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	// FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nCountFibrBase;
	int    nCountTotalPoint;
	int    nSeq;
	int    nAssignType;
	int    nTemp;              // Size 구별하기 위한 임시 변수
};

struct T_FIBR_UDRD_D_CH_RW_950
{
	T_FIBR_K key;
	char   strName[40];
	UINT   SectKey;
	UINT   FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	// FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nCountFibrBase;
	int    nCountTotalPoint;
	int    nSeq;
	int    nAssignType;
	int    nTotalArraySave;
};
struct T_FIBR_UDRD_D_CH_RW_970
{   
	T_FIBR_K key;
	char   strName[40];
	UINT   SectKey;
	UINT   FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
														 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nCountFibrBase;
	int    nCountTotalPoint;
	int    nSeq;
	int    nAssignType;
	int    nTotalArraySave;              // 값이 1일 때는 FIBR 안에 있는 Array데이터를 다 모아서 한번에 저장하는 방식 사용(파일 저장 속도 때문)
	COLORREF FimpColor[6];
};

struct T_FIBR_UDRD_D_CH_RW
{   
	T_FIBR_K key;
	char   strName[40];
	UINT   SectKey;
	UINT   FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
														 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nCountFibrBase;
	int    nCountTotalPoint;
	int    nCountFibrBaseJ;
	int    nCountTotalPointJ;
	int    nSeq;
	int    nAssignType;
	int    nTotalArraySave;              // 값이 1일 때는 FIBR 안에 있는 Array데이터를 다 모아서 한번에 저장하는 방식 사용(파일 저장 속도 때문)
	COLORREF FimpColor[6];
	UINT   GrupKey;
	UINT   ElemKey;
	void SetFibr(const T_FIBR_K Key, const T_FIBR_D_CH_TEMP& rData)
	{
		key = Key;
		memcpy(strName, rData.strName, sizeof(strName));    
		SectKey = rData.SectKey;
		for(int i = 0; i < 6; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];
		nCountFibrBase  = (int)rData.aFibrBase.GetSize();
		nCountTotalPoint  = (int)rData.aTotalPoint.GetSize();
		nCountFibrBaseJ  = (int)rData.aFibrBaseJ.GetSize();
		nCountTotalPointJ  = (int)rData.aTotalPointJ.GetSize();
		nSeq = rData.nSeq;   
		nAssignType = rData.nAssignType;
		nTotalArraySave = 1;

		memcpy(FimpColor, rData.FimpColor, sizeof(FimpColor));

		GrupKey = rData.GrupKey;
		ElemKey = rData.ElemKey;
	}
	void GetFibr(T_FIBR_K& rKey, T_FIBR_D_CH_TEMP& rData)
	{
		rKey = key;
		memcpy(rData.strName, strName, sizeof(strName));    
		rData.SectKey = SectKey;
		for(int i = 0; i < 6; i++) rData.FimpKey[i] = FimpKey[i];
		rData.bMonitoredFiber = bMonitoredFiber;
		for(int i = 0; i < 8; i++) rData.MonitoredFiber[i] = MonitoredFiber[i];        
		rData.aFibrBase.RemoveAll();
		rData.aFibrBaseJ.RemoveAll();
		rData.aTotalPoint.RemoveAll();
		rData.aTotalPointJ.RemoveAll();
		rData.nSeq = nSeq;       
		rData.nAssignType = nAssignType;

		memcpy(rData.FimpColor, FimpColor, sizeof(FimpColor));

		rData.GrupKey = GrupKey ;
		rData.ElemKey = ElemKey;
	}
	void Convert692(T_FIBR_UDRD_D_CH_RW_692& rData)
	{
		key = rData.key;
		memset(strName,0,sizeof(strName));
		memcpy(strName, rData.strName, sizeof(rData.strName));    
		SectKey = rData.SectKey;
		for(int i = 0; i < 3; i++) FimpKey[i] = rData.FimpKey[i];
		FimpKey[3] = FimpKey[4] = FimpKey[5] = 0;
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];
		nCountFibrBase = rData.nCountFibrBase;
		nCountTotalPoint = rData.nCountTotalPoint;
		nSeq = rData.nSeq;
		nAssignType = D_FIBR_TYPE_IEHP;
		nTotalArraySave = 0;

		FimpColor[0] = RGB(121, 198, 220);
		FimpColor[1] = RGB(245, 242, 31);
		FimpColor[2] = RGB(214, 126, 176);
		FimpColor[3] = RGB(63, 174, 179);
		FimpColor[4] = RGB(40, 130, 89);
		FimpColor[5] = RGB(223, 176, 192);

		GrupKey = 0;
		ElemKey = 0;
	}
	void Convert660(T_FIBR_UDRD_D_CH_RW_660& rData)
	{
		key = rData.key;
		memset(strName,0,sizeof(strName));
		memcpy(strName, rData.strName, sizeof(rData.strName));    
		SectKey = rData.SectKey;
		for(int i = 0; i < 3; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];
		nCountFibrBase = rData.nCountFibrBase;
		nCountTotalPoint = rData.nCountTotalPoint;
		nSeq = rData.nSeq;
		nAssignType = D_FIBR_TYPE_IEHP;
		nTotalArraySave = 0;

		FimpColor[0] = RGB(121, 198, 220);
		FimpColor[1] = RGB(245, 242, 31);
		FimpColor[2] = RGB(214, 126, 176);
		FimpColor[3] = RGB(63, 174, 179);
		FimpColor[4] = RGB(40, 130, 89);
		FimpColor[5] = RGB(223, 176, 192);

		GrupKey = 0;
		ElemKey = 0;
	}
	void Convert860(T_FIBR_UDRD_D_CH_RW_860& rData)
	{
		key = rData.key;
		memset(strName,0,sizeof(strName));
		memcpy(strName, rData.strName, sizeof(rData.strName));    
		SectKey = rData.SectKey;
		for(int i = 0; i < 6; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];
		nCountFibrBase = rData.nCountFibrBase;
		nCountTotalPoint = rData.nCountTotalPoint;
		nSeq = rData.nSeq;
		nAssignType = D_FIBR_TYPE_IEHP;
		nTotalArraySave = 0;

		FimpColor[0] = RGB(121, 198, 220);
		FimpColor[1] = RGB(245, 242, 31);
		FimpColor[2] = RGB(214, 126, 176);
		FimpColor[3] = RGB(63, 174, 179);
		FimpColor[4] = RGB(40, 130, 89);
		FimpColor[5] = RGB(223, 176, 192);

		GrupKey = 0;
		ElemKey = 0;
	}
	void Convert865(T_FIBR_UDRD_D_CH_RW_865& rData)
	{
		key = rData.key;
		memset(strName,0,sizeof(strName));
		memcpy(strName, rData.strName, sizeof(rData.strName));    
		SectKey = rData.SectKey;
		for(int i = 0; i < 6; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];
		nCountFibrBase = rData.nCountFibrBase;
		nCountTotalPoint = rData.nCountTotalPoint;
		nSeq = rData.nSeq;
		nAssignType = rData.nAssignType;
		nTotalArraySave = 0;

		FimpColor[0] = RGB(121, 198, 220);
		FimpColor[1] = RGB(245, 242, 31);
		FimpColor[2] = RGB(214, 126, 176);
		FimpColor[3] = RGB(63, 174, 179);
		FimpColor[4] = RGB(40, 130, 89);
		FimpColor[5] = RGB(223, 176, 192);

		GrupKey = 0;
		ElemKey = 0;
	}
	void Convert950(T_FIBR_UDRD_D_CH_RW_950& rData)
	{
		key = rData.key;
		memset(strName, 0, sizeof(strName));
		memcpy(strName, rData.strName, sizeof(rData.strName));
		SectKey = rData.SectKey;
		for (int i = 0; i < 6; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for (int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];
		nCountFibrBase = rData.nCountFibrBase;
		nCountTotalPoint = rData.nCountTotalPoint;
		nSeq = rData.nSeq;
		nAssignType = rData.nAssignType;
		nTotalArraySave = rData.nTotalArraySave;

		FimpColor[0] = RGB(121, 198, 220);
		FimpColor[1] = RGB(245, 242, 31);
		FimpColor[2] = RGB(214, 126, 176);
		FimpColor[3] = RGB(63, 174, 179);
		FimpColor[4] = RGB(40, 130, 89);
		FimpColor[5] = RGB(223, 176, 192);

		GrupKey = 0;
		ElemKey = 0;
	}
	void Convert970(T_FIBR_UDRD_D_CH_RW_970& rData)
	{
		key = rData.key;
		memset(strName, 0, sizeof(strName));
		memcpy(strName, rData.strName, sizeof(rData.strName));
		SectKey = rData.SectKey;
		for (int i = 0; i < 6; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for (int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];
		nCountFibrBase = rData.nCountFibrBase;
		nCountTotalPoint = rData.nCountTotalPoint;
		nCountFibrBaseJ = 0;
		nCountTotalPointJ = 0;
		nSeq = rData.nSeq;
		nAssignType = rData.nAssignType;
		nTotalArraySave = rData.nTotalArraySave;
		memcpy(FimpColor, rData.FimpColor, sizeof(FimpColor));

		GrupKey = 0;
		ElemKey = 0;
	}
};

struct T_FIBR_UNIT
{
	T_FIBR_UNIT()
	{
		dPoint = D_UNITSYS_BASE_LENGTH;
		dArea  = D_UNITSYS_BASE_AREA;    
	}
	int dPoint;
	int dArea;  
};
#define HASHSIZEFIBR 10001

// 번호 절대 변경하지 말껏(-1) : solver에 넘기는 값임.
#define D_IEHP_KINE     0  // Kinematic Hardening
#define D_IEHP_ORIG     1  // Origin-oriented
#define D_IEHP_PICK     2  // Peak-oriented
#define D_IEHP_CLOU     3  // Clough
#define D_IEHP_DEGR     4  // Degrading Trilinear
#define D_IEHP_TAKE     5  // Takeda
#define D_IEHP_MTAK     6  // Modified Takeda
#define D_IEHP_NELS     7  // 비선형탄성형(대칭)
#define D_IEHP_ASYM     8  // 비대칭역향형
#define D_IEHP_NELA     9  // 비선형탄성형(비대칭)
#define D_IEHP_NBIL    10  // Normal Bilinear
#define D_IEHP_EBIL    11  // Elastic Bilinear
#define D_IEHP_ETRI    12  // Elastic Trilinear
#define D_IEHP_ETET    13  // Elastic Tetralinear
#define D_IEHP_TTET    14  // Takeda Tetralinear
#define D_IEHP_MTTE    15  // Modified Takeda Tetralinear
#define D_IEHP_ORGT    16  // Origin-Oriented Tetralinear
#define D_IEHP_ORGP    19  // Origin Peak-Oriented
#define D_IEHP_SLPB    30  // SLIP Bilinear
#define D_IEHP_SLBT    31  // SLIP Bilinear/Tensile
#define D_IEHP_SLBC    32  // SLIP Bilinear/Compress
#define D_IEHP_SLPT    33  // SLIP Trilinear
#define D_IEHP_SLTT    34  // SLIP Trilinear/Tensile
#define D_IEHP_SLTC    35  // SLIP Trilinear/Compress
#define D_IEHP_FBIL    40  // FEMA Bilinear
#define D_IEHP_FINF    41  // FEMA Infill strut type - Compression only
#define D_IEHP_FTRI    42  // FEMA Trilinear
#define D_IEHP_LRBB    51  // LRB Isolator Bilinear Type
#define D_IEHP_LRBT    52  // LRB Isolator Trilinear Type
#define D_IEHP_DMPR    53  // High Damping Rubber Isolator
#define D_IEHP_RO      60  // RO(RAMBERG OSGOOD)
#define D_IEHP_HD      61  // HD(HARDIN DRNEVICH)
#define D_IEHP_MLEL    70  // Multilinear Elastic
#define D_IEHP_MLKI    71  // Multilinear Plastic Kinematic
#define D_IEHP_MLTA    72  // Multilinear Plastic Takeda
#define D_IEHP_MLPI    73  // Multilinear Plastic Pivot
#define D_IEHP_TAKS    74   // Takeda Slip
#define D_IEHP_AXIAL   76   // Axial Stiffness
#define D_IEHP_BUCK    78   // Steel Buckling
#define D_IEHP_PMMC    86   // P-M Multi-Curve Type
#define D_IEHP_LRBH16    88  // LRB Isolator Bilinear Type [H16]
#define D_IEHP_LRBH30    89  // LRB Isolator Bilinear Type [H30]
#define D_IEHP_DMPRH16    90  // High Damping Rubber Isolator [H16]
#define D_IEHP_DMPRH30    91  // High Damping Rubber Isolator [H30]
#define D_IEHP_SRCT   92  // SRC Tetralinear

// Inelastic Hinge Type
#define T_IEHP_K unsigned int

struct T_IEHP_KINEMA_650
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
};
struct T_IEHP_KINEMA_692
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  : 6.6.0부터 추가
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  : 6.6.0부터 추가
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    : 6.6.0부터 추가
	double dYieldDisp1st[2];    // : 6.6.0부터 추가
	double dYieldDisp2nd[2];    // : 6.6.0부터 추가
	double dYieldDisp3rd[2];    // : 6.6.0부터 추가
	double dYieldRotn1st[2];    // : 6.6.0부터 추가
	double dYieldRotn2nd[2];    // : 6.6.0부터 추가
	double dYieldRotn3rd[2];    // : 6.6.0부터 추가
};
struct T_IEHP_KINEMA
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  : 6.6.0부터 추가
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  : 6.6.0부터 추가
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    : 6.6.0부터 추가
	double dYieldDisp1st[2];    // : 6.6.0부터 추가
	double dYieldDisp2nd[2];    // : 6.6.0부터 추가
	double dYieldDisp3rd[2];    // : 6.6.0부터 추가
	double dYieldRotn1st[2];    // : 6.6.0부터 추가
	double dYieldRotn2nd[2];    // : 6.6.0부터 추가
	double dYieldRotn3rd[2];    // : 6.6.0부터 추가
	double dInitStiffP;         // 7.0.0 추가
	double dInitStiffN;         // 7.0.0 추가
	double dInitStiffnessDist;	// v955 추가
	void Initialize()
	{
		nSymmetryType = 0;
		nYieldStrengthOpt = 1;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i] = 0.5;
			dCrackMoment[i] = 0.5;
			dYieldForce[i] = 1.0;
			dYieldMoment[i] = 1.0;      
			dUltimateForce[i] = 1.0;
			dUltimateMoment[i] = 1.0;
			dStiffRatio1st[i] = 0.5;
			dStiffRatio2nd[i] = 0.1;
			dYieldDisp1st[i]  = 0.1;
			dYieldDisp2nd[i]  = 0.2;
			dYieldDisp3rd[i]  = 0.3;
			dYieldRotn1st[i]  = 0.1;
			dYieldRotn2nd[i]  = 0.2;
			dYieldRotn3rd[i]  = 0.3;
			dDeformCapacity[i][0] = 0.5;
			dDeformCapacity[i][1] = 1.0;
			dDeformCapacity[i][2] = 2.0;
			dDeformCapacity[i][3] = 4.0;
			dDeformCapacity[i][4] = 8.0;      
		} 
		nDeformDefineType = 1;
		nInitStiffType = 0;
		dInitStiffness = 1.0;
		nPAlphaDelta = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert(const T_IEHP_KINEMA& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dUltimateForce[i]    ;
			dUltimateMoment[i] = rData.dUltimateMoment[i]   ;
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i];  
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i];  
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i];  
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i];   
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i];  
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i];  
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness    = rData.dInitStiffness   ;
		nPAlphaDelta      = rData.nPAlphaDelta;
		dInitStiffP       = rData.dInitStiffP;
		dInitStiffN       = rData.dInitStiffN;
		dInitStiffnessDist = rData.dInitStiffnessDist;
	}
	void Convert650(const T_IEHP_KINEMA_650& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dYieldForce[i]    ;  // 호환
			dUltimateMoment[i] = rData.dYieldMoment[i]   ;  // 호환
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = 0.1;                       // 호환 
			dYieldDisp2nd[i]   = 0.2;                       // 호환
			dYieldDisp3rd[i]   = 0.3;                       // 호환 
			dYieldRotn1st[i]   = 0.1;                       // 호환  
			dYieldRotn2nd[i]   = 0.2;                       // 호환
			dYieldRotn3rd[i]   = 0.3;                       // 호환 
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness    = rData.dInitStiffness   ;
		nPAlphaDelta      = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert692(const T_IEHP_KINEMA_692& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dYieldForce[i]    ;
			dUltimateMoment[i] = rData.dYieldMoment[i]   ;
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i];  
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i];  
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i];  
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i];   
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i];  
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i];  
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness    = rData.dInitStiffness   ;
		nPAlphaDelta      = rData.nPAlphaDelta;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
};
struct T_IEHP_ORIGIN_650
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
};
struct T_IEHP_ORIGIN_692
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  : 6.6.0부터 추가
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  : 6.6.0부터 추가
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    : 6.6.0부터 추가
	double dYieldDisp1st[2];    // : 6.6.0부터 추가
	double dYieldDisp2nd[2];    // : 6.6.0부터 추가
	double dYieldDisp3rd[2];    // : 6.6.0부터 추가
	double dYieldRotn1st[2];    // : 6.6.0부터 추가
	double dYieldRotn2nd[2];    // : 6.6.0부터 추가
	double dYieldRotn3rd[2];    // : 6.6.0부터 추가
};
struct T_IEHP_ORIGIN
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  : 6.6.0부터 추가
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  : 6.6.0부터 추가
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    : 6.6.0부터 추가
	double dYieldDisp1st[2];    // : 6.6.0부터 추가
	double dYieldDisp2nd[2];    // : 6.6.0부터 추가
	double dYieldDisp3rd[2];    // : 6.6.0부터 추가
	double dYieldRotn1st[2];    // : 6.6.0부터 추가
	double dYieldRotn2nd[2];    // : 6.6.0부터 추가
	double dYieldRotn3rd[2];    // : 6.6.0부터 추가
	double dInitStiffP;         // 7.0.0 추가
	double dInitStiffN;         // 7.0.0 추가
	double dInitStiffnessDist;	// v955 추가
	void Initialize()
	{
		nSymmetryType = 0;
		nYieldStrengthOpt = 1;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i] = 0.5;
			dCrackMoment[i] = 0.5;
			dYieldForce[i] = 1.0;
			dYieldMoment[i] = 1.0;      
			dUltimateForce[i] = 1.0;
			dUltimateMoment[i] = 1.0;
			dStiffRatio1st[i] = 0.5;
			dStiffRatio2nd[i] = 0.1;
			dYieldDisp1st[i]  = 0.1;
			dYieldDisp2nd[i]  = 0.2;
			dYieldDisp3rd[i]  = 0.3;
			dYieldRotn1st[i]  = 0.1;
			dYieldRotn2nd[i]  = 0.2;
			dYieldRotn3rd[i]  = 0.3;
			dDeformCapacity[i][0] = 0.5;
			dDeformCapacity[i][1] = 1.0;
			dDeformCapacity[i][2] = 2.0;
			dDeformCapacity[i][3] = 4.0;
			dDeformCapacity[i][4] = 8.0;      
		} 
		nDeformDefineType = 1;
		nInitStiffType = 0;
		dInitStiffness = 1.0;
		nPAlphaDelta = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffness = 1.0;
	}
	void Convert(const T_IEHP_ORIGIN& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dUltimateForce[i]    ;
			dUltimateMoment[i] = rData.dUltimateMoment[i]   ;
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i];  
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i];  
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i];  
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i];  
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i];  
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i];  
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness	  = rData.dInitStiffness;
		nPAlphaDelta      = rData.nPAlphaDelta;
		dInitStiffP = rData.dInitStiffP;
		dInitStiffN = rData.dInitStiffN;
		dInitStiffnessDist = rData.dInitStiffnessDist;
	}
	void Convert650(const T_IEHP_ORIGIN_650& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dYieldForce[i]    ;  // 호환
			dUltimateMoment[i] = rData.dYieldMoment[i]   ;  // 호환
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = 0.1;                       // 호환 
			dYieldDisp2nd[i]   = 0.2;                       // 호환
			dYieldDisp3rd[i]   = 0.3;                       // 호환 
			dYieldRotn1st[i]   = 0.1;                       // 호환  
			dYieldRotn2nd[i]   = 0.2;                       // 호환
			dYieldRotn3rd[i]   = 0.3;                       // 호환 
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness	  = rData.dInitStiffness;
		nPAlphaDelta      = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert692(const T_IEHP_ORIGIN_692& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dYieldForce[i]    ;
			dUltimateMoment[i] = rData.dYieldMoment[i]   ;
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i];  
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i];  
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i];  
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i];  
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i];  
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i];  
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness    = rData.dInitStiffness   ;
		nPAlphaDelta      = rData.nPAlphaDelta;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
};
struct T_IEHP_PEAK_650
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
};
struct T_IEHP_PEAK_692
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  : 6.6.0부터 추가
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  : 6.6.0부터 추가
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    : 6.6.0부터 추가
	double dYieldDisp1st[2];    // : 6.6.0부터 추가
	double dYieldDisp2nd[2];    // : 6.6.0부터 추가
	double dYieldDisp3rd[2];    // : 6.6.0부터 추가
	double dYieldRotn1st[2];    // : 6.6.0부터 추가
	double dYieldRotn2nd[2];    // : 6.6.0부터 추가
	double dYieldRotn3rd[2];    // : 6.6.0부터 추가
};
struct T_IEHP_PEAK
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  : 6.6.0부터 추가
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  : 6.6.0부터 추가
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    : 6.6.0부터 추가
	double dYieldDisp1st[2];    // : 6.6.0부터 추가
	double dYieldDisp2nd[2];    // : 6.6.0부터 추가
	double dYieldDisp3rd[2];    // : 6.6.0부터 추가
	double dYieldRotn1st[2];    // : 6.6.0부터 추가
	double dYieldRotn2nd[2];    // : 6.6.0부터 추가
	double dYieldRotn3rd[2];    // : 6.6.0부터 추가
	double dInitStiffP;         // 7.0.0 추가
	double dInitStiffN;         // 7.0.0 추가
	double dInitStiffnessDist;	// v955 추가
	void Initialize()
	{
		nSymmetryType = 0;
		nYieldStrengthOpt = 1;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i] = 0.5;
			dCrackMoment[i] = 0.5;
			dYieldForce[i] = 1.0;
			dYieldMoment[i] = 1.0;      
			dUltimateForce[i] = 1.0;
			dUltimateMoment[i] = 1.0;
			dStiffRatio1st[i] = 0.5;
			dStiffRatio2nd[i] = 0.1;
			dYieldDisp1st[i]  = 0.1;
			dYieldDisp2nd[i]  = 0.2;
			dYieldDisp3rd[i]  = 0.3;
			dYieldRotn1st[i]  = 0.1;
			dYieldRotn2nd[i]  = 0.2;
			dYieldRotn3rd[i]  = 0.3;
			dDeformCapacity[i][0] = 0.5;
			dDeformCapacity[i][1] = 1.0;
			dDeformCapacity[i][2] = 2.0;
			dDeformCapacity[i][3] = 4.0;
			dDeformCapacity[i][4] = 8.0;      
		} 
		nDeformDefineType = 1;
		nInitStiffType = 0;
		dInitStiffness = 1.0;
		nPAlphaDelta = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert(const T_IEHP_PEAK& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;
			dUltimateForce[i]  = rData.dUltimateForce[i]    ;
			dUltimateMoment[i] = rData.dUltimateMoment[i]   ;
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i];  
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i];  
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i];  
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i];  
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i];  
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i];  
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness	  = rData.dInitStiffness;
		nPAlphaDelta      = rData.nPAlphaDelta;
		dInitStiffP = rData.dInitStiffP;
		dInitStiffN = rData.dInitStiffN;
		dInitStiffnessDist = rData.dInitStiffnessDist;
	}
	void Convert650(const T_IEHP_PEAK_650& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dYieldForce[i]    ;  // 호환
			dUltimateMoment[i] = rData.dYieldMoment[i]   ;  // 호환
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = 0.1;                       // 호환 
			dYieldDisp2nd[i]   = 0.2;                       // 호환
			dYieldDisp3rd[i]   = 0.3;                       // 호환 
			dYieldRotn1st[i]   = 0.1;                       // 호환  
			dYieldRotn2nd[i]   = 0.2;                       // 호환
			dYieldRotn3rd[i]   = 0.3;                       // 호환 
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness	  = rData.dInitStiffness;
		nPAlphaDelta      = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert692(const T_IEHP_PEAK_692& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;
			dUltimateForce[i]  = rData.dYieldForce[i]    ;
			dUltimateMoment[i] = rData.dYieldMoment[i]   ;
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i];  
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i];  
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i];  
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i];  
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i];  
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i];  
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness    = rData.dInitStiffness   ;
		nPAlphaDelta      = rData.nPAlphaDelta;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
};
struct T_IEHP_CLOUGH_650
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)   Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	double dUnloadStiffCalcExpo;  // Exponent in Unloading Stiffness Calculation
};
struct T_IEHP_CLOUGH_692
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  : 6.6.0부터 추가
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  : 6.6.0부터 추가
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)   Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	double dUnloadStiffCalcExpo;  // Exponent in Unloading Stiffness Calculation
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    : 6.6.0부터 추가
	double dYieldDisp2nd[2];    // : 6.6.0부터 추가
	double dYieldDisp3rd[2];    // : 6.6.0부터 추가
	double dYieldRotn2nd[2];    // : 6.6.0부터 추가
	double dYieldRotn3rd[2];    // : 6.6.0부터 추가
};
struct T_IEHP_CLOUGH
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  : 6.6.0부터 추가
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  : 6.6.0부터 추가
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)   Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	double dUnloadStiffCalcExpo;  // Exponent in Unloading Stiffness Calculation
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    : 6.6.0부터 추가
	double dYieldDisp2nd[2];    // : 6.6.0부터 추가
	double dYieldDisp3rd[2];    // : 6.6.0부터 추가
	double dYieldRotn2nd[2];    // : 6.6.0부터 추가
	double dYieldRotn3rd[2];    // : 6.6.0부터 추가
	double dInitStiffP;         // 7.0.0 추가
	double dInitStiffN;         // 7.0.0 추가
	double dInitStiffnessDist;	// v955 추가
	void Initialize()
	{
		nSymmetryType = 0;
		nYieldStrengthOpt = 1;        
		for(int i = 0; i < 2; i++)
		{
			dYieldForce[i] = 1.0;
			dYieldMoment[i] = 1.0;            
			dUltimateForce[i] = 1.0;
			dUltimateMoment[i] = 1.0;      
			dStiffRatio2nd[i] = 0.1;
			dYieldDisp2nd[i]  = 0.2;
			dYieldDisp3rd[i]  = 0.3;
			dYieldRotn2nd[i]  = 0.2;
			dYieldRotn3rd[i]  = 0.3;
			dDeformCapacity[i][0] = 0.5;
			dDeformCapacity[i][1] = 1.0;
			dDeformCapacity[i][2] = 2.0;
			dDeformCapacity[i][3] = 4.0;
			dDeformCapacity[i][4] = 8.0;      
		} 
		nDeformDefineType = 1;
		nInitStiffType = 0;
		dInitStiffness = 1.0;
#if defined (_CIVIL_JP)
		dUnloadStiffCalcExpo = 0.5;
#else
		dUnloadStiffCalcExpo = 0.4;
#endif
		nPAlphaDelta = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert(const T_IEHP_CLOUGH& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;
			dUltimateForce[i]  = rData.dUltimateForce[i]    ;
			dUltimateMoment[i] = rData.dUltimateMoment[i]   ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i];  
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i];  
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i];  
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i];  
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness    = rData.dInitStiffness   ;
		dUnloadStiffCalcExpo = rData.dUnloadStiffCalcExpo;
		nPAlphaDelta      = rData.nPAlphaDelta;
		dInitStiffP = rData.dInitStiffP;
		dInitStiffN = rData.dInitStiffN;
		dInitStiffnessDist = rData.dInitStiffnessDist;
	}
	void Convert650(const T_IEHP_CLOUGH_650& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dYieldForce[i]    ;  // 호환
			dUltimateMoment[i] = rData.dYieldMoment[i]   ;  // 호환
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp2nd[i]   = 0.2;                       // 호환
			dYieldDisp3rd[i]   = 0.3;                       // 호환 
			dYieldRotn2nd[i]   = 0.2;                       // 호환
			dYieldRotn3rd[i]   = 0.3;                       // 호환 
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness    = rData.dInitStiffness   ;
		dUnloadStiffCalcExpo = rData.dUnloadStiffCalcExpo;
		nPAlphaDelta      = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert692(const T_IEHP_CLOUGH_692& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;
			dUltimateForce[i]  = rData.dYieldForce[i]    ;
			dUltimateMoment[i] = rData.dYieldMoment[i]   ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i];  
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i];  
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i];  
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i];  
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness    = rData.dInitStiffness   ;
		dUnloadStiffCalcExpo = rData.dUnloadStiffCalcExpo;
		nPAlphaDelta      = rData.nPAlphaDelta;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
};
struct T_IEHP_DEGRAD_650
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
};
struct T_IEHP_DEGRAD_692
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  : 6.6.0부터 추가
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  : 6.6.0부터 추가
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    : 6.6.0부터 추가
	double dYieldDisp1st[2];    // : 6.6.0부터 추가
	double dYieldDisp2nd[2];    // : 6.6.0부터 추가
	double dYieldDisp3rd[2];    // : 6.6.0부터 추가
	double dYieldRotn1st[2];    // : 6.6.0부터 추가
	double dYieldRotn2nd[2];    // : 6.6.0부터 추가
	double dYieldRotn3rd[2];    // : 6.6.0부터 추가
};
struct T_IEHP_DEGRAD
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  : 6.6.0부터 추가
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  : 6.6.0부터 추가
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    : 6.6.0부터 추가
	double dYieldDisp1st[2];    // : 6.6.0부터 추가
	double dYieldDisp2nd[2];    // : 6.6.0부터 추가
	double dYieldDisp3rd[2];    // : 6.6.0부터 추가
	double dYieldRotn1st[2];    // : 6.6.0부터 추가
	double dYieldRotn2nd[2];    // : 6.6.0부터 추가
	double dYieldRotn3rd[2];    // : 6.6.0부터 추가
	double dInitStiffP;         // 7.0.0 추가
	double dInitStiffN;         // 7.0.0 추가
	double dInitStiffnessDist;	// v955 추가
	void Initialize()
	{
		nSymmetryType = 0;
		nYieldStrengthOpt = 1;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i] = 0.5;
			dCrackMoment[i] = 0.5;
			dYieldForce[i] = 1.0;
			dYieldMoment[i] = 1.0;      
			dUltimateForce[i] = 1.0;
			dUltimateMoment[i] = 1.0;
			dStiffRatio1st[i] = 0.5;
			dStiffRatio2nd[i] = 0.1;
			dYieldDisp1st[i]  = 0.1;
			dYieldDisp2nd[i]  = 0.2;
			dYieldDisp3rd[i]  = 0.3;
			dYieldRotn1st[i]  = 0.1;
			dYieldRotn2nd[i]  = 0.2;
			dYieldRotn3rd[i]  = 0.3;
			dDeformCapacity[i][0] = 0.5;
			dDeformCapacity[i][1] = 1.0;
			dDeformCapacity[i][2] = 2.0;
			dDeformCapacity[i][3] = 4.0;
			dDeformCapacity[i][4] = 8.0;      
		} 
		nDeformDefineType = 1;
		nInitStiffType = 0;
		dInitStiffness = 1.0;
		nPAlphaDelta = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert(const T_IEHP_DEGRAD& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;
			dUltimateForce[i]  = rData.dUltimateForce[i]    ;
			dUltimateMoment[i] = rData.dUltimateMoment[i]   ;
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i];  
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i];  
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i];  
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i];  
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i];  
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i];  
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness    = rData.dInitStiffness   ;
		nPAlphaDelta      = rData.nPAlphaDelta;
		dInitStiffP  = rData.dInitStiffP;
		dInitStiffN  = rData.dInitStiffN;
		dInitStiffnessDist = rData.dInitStiffnessDist;
	}
	void Convert650(const T_IEHP_DEGRAD_650& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dYieldForce[i]    ;  // 호환
			dUltimateMoment[i] = rData.dYieldMoment[i]   ;  // 호환
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = 0.1;                       // 호환 
			dYieldDisp2nd[i]   = 0.2;                       // 호환
			dYieldDisp3rd[i]   = 0.3;                       // 호환 
			dYieldRotn1st[i]   = 0.1;                       // 호환  
			dYieldRotn2nd[i]   = 0.2;                       // 호환
			dYieldRotn3rd[i]   = 0.3;                       // 호환 
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness    = rData.dInitStiffness   ;
		nPAlphaDelta      = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert692(const T_IEHP_DEGRAD_692& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;
			dUltimateForce[i]  = rData.dYieldForce[i]    ;
			dUltimateMoment[i] = rData.dYieldMoment[i]   ;
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i];  
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i];  
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i];  
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i];  
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i];  
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i];  
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness    = rData.dInitStiffness   ;
		nPAlphaDelta      = rData.nPAlphaDelta;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
};
struct T_IEHP_TAKEDA_650
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	double dUnloadStiffCalcExpo;  // Exponent in Unloading Stiffness Calculation
	double dUnloadStiffReduFac;   // Inner Loop Unloading Stiffness Reduction Factor  
};
struct T_IEHP_TAKEDA_692
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  : 6.6.0부터 추가
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  : 6.6.0부터 추가
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	double dUnloadStiffCalcExpo;  // Exponent in Unloading Stiffness Calculation
	double dUnloadStiffReduFac;   // Inner Loop Unloading Stiffness Reduction Factor  
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    : 6.6.0부터 추가
	double dYieldDisp1st[2];    // : 6.6.0부터 추가
	double dYieldDisp2nd[2];    // : 6.6.0부터 추가
	double dYieldDisp3rd[2];    // : 6.6.0부터 추가
	double dYieldRotn1st[2];    // : 6.6.0부터 추가
	double dYieldRotn2nd[2];    // : 6.6.0부터 추가
	double dYieldRotn3rd[2];    // : 6.6.0부터 추가
};
struct T_IEHP_TAKEDA
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  : 6.6.0부터 추가
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  : 6.6.0부터 추가
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	double dUnloadStiffCalcExpo;  // Exponent in Unloading Stiffness Calculation
	double dUnloadStiffReduFac;   // Inner Loop Unloading Stiffness Reduction Factor  
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    : 6.6.0부터 추가
	double dYieldDisp1st[2];    // : 6.6.0부터 추가
	double dYieldDisp2nd[2];    // : 6.6.0부터 추가
	double dYieldDisp3rd[2];    // : 6.6.0부터 추가
	double dYieldRotn1st[2];    // : 6.6.0부터 추가
	double dYieldRotn2nd[2];    // : 6.6.0부터 추가
	double dYieldRotn3rd[2];    // : 6.6.0부터 추가
	double dInitStiffP;         // 7.0.0 추가
	double dInitStiffN;         // 7.0.0 추가
	double dPinchingRuleFac;    // 8.5.1 추가
	double dInitStiffnessDist;	// v955 추가
	void Initialize()
	{
		nSymmetryType = 0;
		nYieldStrengthOpt = 1;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i] = 0.5;
			dCrackMoment[i] = 0.5;
			dYieldForce[i] = 1.0;
			dYieldMoment[i] = 1.0;      
			dUltimateForce[i] = 1.0;
			dUltimateMoment[i] = 1.0;
			dStiffRatio1st[i] = 0.5;
			dStiffRatio2nd[i] = 0.1;
			dYieldDisp1st[i]  = 0.1;
			dYieldDisp2nd[i]  = 0.2;
			dYieldDisp3rd[i]  = 0.3;
			dYieldRotn1st[i]  = 0.1;
			dYieldRotn2nd[i]  = 0.2;
			dYieldRotn3rd[i]  = 0.3;
			dDeformCapacity[i][0] = 0.5;
			dDeformCapacity[i][1] = 1.0;
			dDeformCapacity[i][2] = 2.0;
			dDeformCapacity[i][3] = 4.0;
			dDeformCapacity[i][4] = 8.0;      
		} 
		nDeformDefineType = 1;
		nInitStiffType = 0;
		dInitStiffness = 1.0;
#if defined (_CIVIL_JP)
		dUnloadStiffCalcExpo = 0.5;
#else
		dUnloadStiffCalcExpo = 0.4;
#endif
		dUnloadStiffReduFac  = 1.0;
		nPAlphaDelta = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dPinchingRuleFac = 0.5;
		dInitStiffnessDist = 1.0;
	}
	void Convert(const T_IEHP_TAKEDA& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;
			dUltimateForce[i]  = rData.dUltimateForce[i]    ;
			dUltimateMoment[i] = rData.dUltimateMoment[i]   ;
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i];  
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i];  
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i];  
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i];  
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i];  
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i];  
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness    = rData.dInitStiffness   ;
		dUnloadStiffCalcExpo = rData.dUnloadStiffCalcExpo;
		dUnloadStiffReduFac  = rData.dUnloadStiffReduFac;
		nPAlphaDelta      = rData.nPAlphaDelta;
		dInitStiffP = rData.dInitStiffP;
		dInitStiffN = rData.dInitStiffN;
		dPinchingRuleFac = rData.dPinchingRuleFac;
		dInitStiffnessDist = rData.dInitStiffnessDist;
	}
	void Convert650(const T_IEHP_TAKEDA_650& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dYieldForce[i]    ;  // 호환
			dUltimateMoment[i] = rData.dYieldMoment[i]   ;  // 호환
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = 0.1;                       // 호환 
			dYieldDisp2nd[i]   = 0.2;                       // 호환
			dYieldDisp3rd[i]   = 0.3;                       // 호환 
			dYieldRotn1st[i]   = 0.1;                       // 호환  
			dYieldRotn2nd[i]   = 0.2;                       // 호환
			dYieldRotn3rd[i]   = 0.3;                       // 호환 
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness    = rData.dInitStiffness   ;
		dUnloadStiffCalcExpo = rData.dUnloadStiffCalcExpo;
		dUnloadStiffReduFac  = rData.dUnloadStiffReduFac;
		nPAlphaDelta      = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dPinchingRuleFac = 0.5;
		dInitStiffnessDist = 1.0;
	}
	void Convert692(const T_IEHP_TAKEDA_692& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;
			dUltimateForce[i]  = rData.dYieldForce[i]    ;
			dUltimateMoment[i] = rData.dYieldMoment[i]   ;
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i];  
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i];  
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i];  
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i];  
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i];  
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i];  
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType    = rData.nInitStiffType   ;
		dInitStiffness    = rData.dInitStiffness   ;
		dUnloadStiffCalcExpo = rData.dUnloadStiffCalcExpo;
		dUnloadStiffReduFac  = rData.dUnloadStiffReduFac;
		nPAlphaDelta      = rData.nPAlphaDelta;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dPinchingRuleFac = 0.5;
		dInitStiffnessDist = 1.0;
	}
};
struct T_IEHP_TAKTET_692
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dFractureForce[2];   // 0:FF(+), 1:FF(-)   (DOF Dx, Dy, Dz 일때 사용)  
	double dFractureMoment[2];  // 0:FF(+), 1:FF(-)   (DOF Rx, Ry, Rz 일때 사용)  
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	double dStiffRatio3rd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 3rd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	double dUnloadStiffCalcExpo;  // Exponent in Unloading Stiffness Calculation
	double dUnloadStiffReduFac;   // Inner Loop Unloading Stiffness Reduction Factor  
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    
	double dYieldDisp1st[2];    // 
	double dYieldDisp2nd[2];    // 
	double dYieldDisp3rd[2];    // 
	double dYieldDisp4th[2];    // 
	double dYieldRotn1st[2];    // 
	double dYieldRotn2nd[2];    // 
	double dYieldRotn3rd[2];    // 
	double dYieldRotn4th[2];    // 
};

struct T_IEHP_SRCTET          // : 9.6.0 추가 모델
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dFractureForce[2];   // 0:FF(+), 1:FF(-)   (DOF Dx, Dy, Dz 일때 사용)  
	double dFractureMoment[2];  // 0:FF(+), 1:FF(-)   (DOF Rx, Ry, Rz 일때 사용)  
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	double dStiffRatio3rd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 3rd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	double dUnloadStiffCalcExpo;  // Exponent in Unloading Stiffness Calculation
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    
	double dYieldDisp1st[2];    // 
	double dYieldDisp2nd[2];    // 
	double dYieldDisp3rd[2];    // 
	double dYieldDisp4th[2];    // 
	double dYieldRotn1st[2];    // 
	double dYieldRotn2nd[2];    // 
	double dYieldRotn3rd[2];    // 
	double dYieldRotn4th[2];    //
	double dInitStiffP;         // 7.0.0 추가
	double dInitStiffN;         // 7.0.0 추가
	double dInitStiffnessDist;	// v955 추가
	void Initialize()
	{
		nSymmetryType = 0;
		nYieldStrengthOpt = 1;
		for (int i = 0; i < 2; i++)
		{
			dCrackForce[i] = 0.5;
			dCrackMoment[i] = 0.5;
			dYieldForce[i] = 1.0;
			dYieldMoment[i] = 1.0;
			dUltimateForce[i] = 1.0;
			dUltimateMoment[i] = 1.0;
			dFractureForce[i] = 1.0;
			dFractureMoment[i] = 1.0;
			dStiffRatio1st[i] = 0.5;
			dStiffRatio2nd[i] = 0.1;
			dStiffRatio3rd[i] = 0.01;
			dYieldDisp1st[i] = 0.1;
			dYieldDisp2nd[i] = 0.2;
			dYieldDisp3rd[i] = 0.3;
			dYieldDisp4th[i] = 0.4;
			dYieldRotn1st[i] = 0.1;
			dYieldRotn2nd[i] = 0.2;
			dYieldRotn3rd[i] = 0.3;
			dYieldRotn4th[i] = 0.4;
			dDeformCapacity[i][0] = 0.5;
			dDeformCapacity[i][1] = 1.0;
			dDeformCapacity[i][2] = 2.0;
			dDeformCapacity[i][3] = 4.0;
			dDeformCapacity[i][4] = 8.0;
		}
		nDeformDefineType = 1;
		nInitStiffType = 0;
		dInitStiffness = 1.0;
		dUnloadStiffCalcExpo = 0.5;
		nPAlphaDelta = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert(const T_IEHP_SRCTET& rData)
	{
		nSymmetryType = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;
		for (int i = 0; i < 2; i++)
		{
			dCrackForce[i] = rData.dCrackForce[i];
			dCrackMoment[i] = rData.dCrackMoment[i];
			dYieldForce[i] = rData.dYieldForce[i];
			dYieldMoment[i] = rData.dYieldMoment[i];
			dUltimateForce[i] = rData.dUltimateForce[i];
			dUltimateMoment[i] = rData.dUltimateMoment[i];
			dFractureForce[i] = rData.dFractureForce[i];
			dFractureMoment[i] = rData.dFractureMoment[i];
			dStiffRatio1st[i] = rData.dStiffRatio1st[i];
			dStiffRatio2nd[i] = rData.dStiffRatio2nd[i];
			dStiffRatio3rd[i] = rData.dStiffRatio3rd[i];
			dYieldDisp1st[i] = rData.dYieldDisp1st[i];
			dYieldDisp2nd[i] = rData.dYieldDisp2nd[i];
			dYieldDisp3rd[i] = rData.dYieldDisp3rd[i];
			dYieldDisp4th[i] = rData.dYieldDisp4th[i];
			dYieldRotn1st[i] = rData.dYieldRotn1st[i];
			dYieldRotn2nd[i] = rData.dYieldRotn2nd[i];
			dYieldRotn3rd[i] = rData.dYieldRotn3rd[i];
			dYieldRotn4th[i] = rData.dYieldRotn4th[i];
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];
		}
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType = rData.nInitStiffType;
		dInitStiffness = rData.dInitStiffness;
		dUnloadStiffCalcExpo = rData.dUnloadStiffCalcExpo;
		nPAlphaDelta = rData.nPAlphaDelta;
		dInitStiffP = rData.dInitStiffP;
		dInitStiffN = rData.dInitStiffN;
		dInitStiffnessDist = rData.dInitStiffnessDist;
	}
};

struct T_IEHP_TAKTET          // : 6.6.0 추가 모델
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dFractureForce[2];   // 0:FF(+), 1:FF(-)   (DOF Dx, Dy, Dz 일때 사용)  
	double dFractureMoment[2];  // 0:FF(+), 1:FF(-)   (DOF Rx, Ry, Rz 일때 사용)  
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	double dStiffRatio3rd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 3rd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	double dUnloadStiffCalcExpo;  // Exponent in Unloading Stiffness Calculation
	double dUnloadStiffReduFac;   // Inner Loop Unloading Stiffness Reduction Factor  
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    
	double dYieldDisp1st[2];    // 
	double dYieldDisp2nd[2];    // 
	double dYieldDisp3rd[2];    // 
	double dYieldDisp4th[2];    // 
	double dYieldRotn1st[2];    // 
	double dYieldRotn2nd[2];    // 
	double dYieldRotn3rd[2];    // 
	double dYieldRotn4th[2];    //
	double dInitStiffP;         // 7.0.0 추가
	double dInitStiffN;         // 7.0.0 추가
	double dInitStiffnessDist;	// v955 추가
	void Initialize()
	{
		nSymmetryType = 0;
		nYieldStrengthOpt = 1;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i] = 0.5;
			dCrackMoment[i] = 0.5;
			dYieldForce[i] = 1.0;
			dYieldMoment[i] = 1.0;      
			dUltimateForce[i] = 1.0;
			dUltimateMoment[i] = 1.0;      
			dFractureForce[i] = 1.0;
			dFractureMoment[i] = 1.0;      
			dStiffRatio1st[i] = 0.5;
			dStiffRatio2nd[i] = 0.1;
			dStiffRatio3rd[i] = 0.01;
			dYieldDisp1st[i]  = 0.1;
			dYieldDisp2nd[i]  = 0.2;
			dYieldDisp3rd[i]  = 0.3;
			dYieldDisp4th[i]  = 0.4;
			dYieldRotn1st[i]  = 0.1;
			dYieldRotn2nd[i]  = 0.2;
			dYieldRotn3rd[i]  = 0.3;
			dYieldRotn4th[i]  = 0.4;
			dDeformCapacity[i][0] = 0.5;
			dDeformCapacity[i][1] = 1.0;
			dDeformCapacity[i][2] = 2.0;
			dDeformCapacity[i][3] = 4.0;
			dDeformCapacity[i][4] = 8.0;      
		} 
		nDeformDefineType = 1;
		nInitStiffType = 0;
		dInitStiffness = 1.0;
#if defined (_CIVIL_JP)
		dUnloadStiffCalcExpo = 0.5;
#else
		dUnloadStiffCalcExpo = 0.4;
#endif
		dUnloadStiffReduFac  = 1.0;
		nPAlphaDelta = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert(const T_IEHP_TAKTET& rData)
	{
		nSymmetryType = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i] = rData.dCrackForce[i];
			dCrackMoment[i] = rData.dCrackMoment[i];
			dYieldForce[i] = rData.dYieldForce[i];
			dYieldMoment[i] = rData.dYieldMoment[i];      
			dUltimateForce[i] = rData.dUltimateForce[i];
			dUltimateMoment[i] = rData.dUltimateMoment[i];      
			dFractureForce[i] = rData.dFractureForce[i];
			dFractureMoment[i] = rData.dFractureMoment[i];      
			dStiffRatio1st[i] = rData.dStiffRatio1st[i];
			dStiffRatio2nd[i] = rData.dStiffRatio2nd[i];
			dStiffRatio3rd[i] = rData.dStiffRatio3rd[i];
			dYieldDisp1st[i]  = rData.dYieldDisp1st[i];
			dYieldDisp2nd[i]  = rData.dYieldDisp2nd[i];
			dYieldDisp3rd[i]  = rData.dYieldDisp3rd[i];
			dYieldDisp4th[i]  = rData.dYieldDisp4th[i];
			dYieldRotn1st[i]  = rData.dYieldRotn1st[i];
			dYieldRotn2nd[i]  = rData.dYieldRotn2nd[i];
			dYieldRotn3rd[i]  = rData.dYieldRotn3rd[i];
			dYieldRotn4th[i]  = rData.dYieldRotn4th[i];
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType = rData.nInitStiffType;
		dInitStiffness = rData.dInitStiffness;
		dUnloadStiffCalcExpo = rData.dUnloadStiffCalcExpo;
		dUnloadStiffReduFac  = rData.dUnloadStiffReduFac;
		nPAlphaDelta = rData.nPAlphaDelta;
		dInitStiffP = rData.dInitStiffP;
		dInitStiffN = rData.dInitStiffN;
		dInitStiffnessDist = rData.dInitStiffnessDist;
	}
	void Convert692(const T_IEHP_TAKTET_692& rData)
	{
		nSymmetryType = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i] = rData.dCrackForce[i];
			dCrackMoment[i] = rData.dCrackMoment[i];
			dYieldForce[i] = rData.dYieldForce[i];
			dYieldMoment[i] = rData.dYieldMoment[i];      
			dUltimateForce[i] = rData.dUltimateForce[i];
			dUltimateMoment[i] = rData.dUltimateMoment[i];      
			dFractureForce[i] = rData.dFractureForce[i];
			dFractureMoment[i] = rData.dFractureMoment[i];      
			dStiffRatio1st[i] = rData.dStiffRatio1st[i];
			dStiffRatio2nd[i] = rData.dStiffRatio2nd[i];
			dStiffRatio3rd[i] = rData.dStiffRatio3rd[i];
			dYieldDisp1st[i]  = rData.dYieldDisp1st[i];
			dYieldDisp2nd[i]  = rData.dYieldDisp2nd[i];
			dYieldDisp3rd[i]  = rData.dYieldDisp3rd[i];
			dYieldDisp4th[i]  = rData.dYieldDisp4th[i];
			dYieldRotn1st[i]  = rData.dYieldRotn1st[i];
			dYieldRotn2nd[i]  = rData.dYieldRotn2nd[i];
			dYieldRotn3rd[i]  = rData.dYieldRotn3rd[i];
			dYieldRotn4th[i]  = rData.dYieldRotn4th[i];
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType = rData.nInitStiffType;
		dInitStiffness = rData.dInitStiffness;
		dUnloadStiffCalcExpo = rData.dUnloadStiffCalcExpo;
		dUnloadStiffReduFac  = rData.dUnloadStiffReduFac;
		nPAlphaDelta = rData.nPAlphaDelta;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
};
struct T_IEHP_NORBIL_692          // : 6.6.0 추가 모델
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용) 
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta   
	double dYieldDisp2nd[2];    // 
	double dYieldDisp3rd[2];    // 
	double dYieldRotn2nd[2];    // 
	double dYieldRotn3rd[2];    // 
};
struct T_IEHP_NORBIL          // : 6.6.0 추가 모델
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용) 
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta   
	double dYieldDisp2nd[2];    // 
	double dYieldDisp3rd[2];    // 
	double dYieldRotn2nd[2];    // 
	double dYieldRotn3rd[2];    //
	double dInitStiffP;         // 7.0.0 추가
	double dInitStiffN;         // 7.0.0 추가
	double dInitStiffnessDist;	// v955 추가
	void Initialize()
	{
		nSymmetryType = 0;
		nYieldStrengthOpt = 1;    
		for(int i = 0; i < 2; i++)
		{
			dYieldForce[i] = 1.0;
			dYieldMoment[i] = 1.0;      
			dUltimateForce[i] = 1.0;
			dUltimateMoment[i] = 1.0;
			dStiffRatio2nd[i] = 0.1;
			dYieldDisp2nd[i]  = 0.2;
			dYieldDisp3rd[i]  = 0.3;
			dYieldRotn2nd[i]  = 0.2;
			dYieldRotn3rd[i]  = 0.3;
			dDeformCapacity[i][0] = 0.5;
			dDeformCapacity[i][1] = 1.0;
			dDeformCapacity[i][2] = 2.0;
			dDeformCapacity[i][3] = 4.0;
			dDeformCapacity[i][4] = 8.0;      
		} 
		nDeformDefineType = 1;
		nInitStiffType = 0;
		dInitStiffness = 1.0;
		nPAlphaDelta = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert(const T_IEHP_NORBIL& rData)
	{
		nSymmetryType = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dYieldForce[i] = rData.dYieldForce[i];
			dYieldMoment[i] = rData.dYieldMoment[i];      
			dUltimateForce[i] = rData.dUltimateForce[i];
			dUltimateMoment[i] = rData.dUltimateMoment[i];
			dStiffRatio2nd[i] = rData.dStiffRatio2nd[i];
			dYieldDisp2nd[i]  = rData.dYieldDisp2nd[i];
			dYieldDisp3rd[i]  = rData.dYieldDisp3rd[i];
			dYieldRotn2nd[i]  = rData.dYieldRotn2nd[i];
			dYieldRotn3rd[i]  = rData.dYieldRotn3rd[i];
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType = rData.nInitStiffType;
		dInitStiffness = rData.dInitStiffness;
		nPAlphaDelta = rData.nPAlphaDelta;
		dInitStiffP = rData.dInitStiffP;
		dInitStiffN = rData.dInitStiffN;
		dInitStiffnessDist = rData.dInitStiffnessDist;
	}
	void Convert692(const T_IEHP_NORBIL_692& rData)
	{
		nSymmetryType = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dYieldForce[i] = rData.dYieldForce[i];
			dYieldMoment[i] = rData.dYieldMoment[i];      
			dUltimateForce[i] = rData.dUltimateForce[i];
			dUltimateMoment[i] = rData.dUltimateMoment[i];
			dStiffRatio2nd[i] = rData.dStiffRatio2nd[i];
			dYieldDisp2nd[i]  = rData.dYieldDisp2nd[i];
			dYieldDisp3rd[i]  = rData.dYieldDisp3rd[i];
			dYieldRotn2nd[i]  = rData.dYieldRotn2nd[i];
			dYieldRotn3rd[i]  = rData.dYieldRotn3rd[i];
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType = rData.nInitStiffType;
		dInitStiffness = rData.dInitStiffness;
		nPAlphaDelta = rData.nPAlphaDelta;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
};
struct T_IEHP_ELABIL_692    // : 6.6.0 추가 모델
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    
	double dYieldDisp2nd[2];    
	double dYieldDisp3rd[2];    
	double dYieldRotn2nd[2];    
	double dYieldRotn3rd[2]; 
};
struct T_IEHP_ELABIL    // : 6.6.0 추가 모델
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    
	double dYieldDisp2nd[2];    
	double dYieldDisp3rd[2];    
	double dYieldRotn2nd[2];    
	double dYieldRotn3rd[2]; 
	double dInitStiffP;         // 7.0.0 추가
	double dInitStiffN;         // 7.0.0 추가
	double dInitStiffnessDist;	// v955 추가
	void Initialize()
	{
		nSymmetryType = 0;
		nYieldStrengthOpt = 1;    
		for(int i = 0; i < 2; i++)
		{
			dYieldForce[i] = 1.0;
			dYieldMoment[i] = 1.0;      
			dUltimateForce[i] = 1.0;
			dUltimateMoment[i] = 1.0;
			dStiffRatio2nd[i] = 0.1;
			dYieldDisp2nd[i]  = 0.2;
			dYieldDisp3rd[i]  = 0.3;
			dYieldRotn2nd[i]  = 0.2;
			dYieldRotn3rd[i]  = 0.3;
			dDeformCapacity[i][0] = 0.5;
			dDeformCapacity[i][1] = 1.0;
			dDeformCapacity[i][2] = 2.0;
			dDeformCapacity[i][3] = 4.0;
			dDeformCapacity[i][4] = 8.0;      
		} 
		nDeformDefineType = 1;
		nInitStiffType = 0;
		dInitStiffness = 1.0;
		nPAlphaDelta = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert(const T_IEHP_ELABIL& rData)
	{
		nSymmetryType = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dYieldForce[i] = rData.dYieldForce[i];
			dYieldMoment[i] = rData.dYieldMoment[i];      
			dUltimateForce[i] = rData.dUltimateForce[i];
			dUltimateMoment[i] = rData.dUltimateMoment[i];
			dStiffRatio2nd[i] = rData.dStiffRatio2nd[i];
			dYieldDisp2nd[i]  = rData.dYieldDisp2nd[i] ;
			dYieldDisp3rd[i]  = rData.dYieldDisp3rd[i] ;
			dYieldRotn2nd[i]  = rData.dYieldRotn2nd[i] ;
			dYieldRotn3rd[i]  = rData.dYieldRotn3rd[i] ;
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType = rData.nInitStiffType;
		dInitStiffness = rData.dInitStiffness;
		nPAlphaDelta = rData.nPAlphaDelta;
		dInitStiffP = rData.dInitStiffP;
		dInitStiffN = rData.dInitStiffN;
		dInitStiffnessDist = rData.dInitStiffnessDist;
	}
	void Convert692(const T_IEHP_ELABIL_692& rData)
	{
		nSymmetryType = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dYieldForce[i] = rData.dYieldForce[i];
			dYieldMoment[i] = rData.dYieldMoment[i];      
			dUltimateForce[i] = rData.dUltimateForce[i];
			dUltimateMoment[i] = rData.dUltimateMoment[i];
			dStiffRatio2nd[i] = rData.dStiffRatio2nd[i];
			dYieldDisp2nd[i]  = rData.dYieldDisp2nd[i] ;
			dYieldDisp3rd[i]  = rData.dYieldDisp3rd[i] ;
			dYieldRotn2nd[i]  = rData.dYieldRotn2nd[i] ;
			dYieldRotn3rd[i]  = rData.dYieldRotn3rd[i] ;
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType = rData.nInitStiffType;
		dInitStiffness = rData.dInitStiffness;
		nPAlphaDelta = rData.nPAlphaDelta;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
};
struct T_IEHP_ELATRI_692      // : 6.6.0 추가 모델
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    
	double dYieldDisp1st[2];    // 
	double dYieldDisp2nd[2];    // 
	double dYieldDisp3rd[2];    // 
	double dYieldRotn1st[2];    // 
	double dYieldRotn2nd[2];    // 
	double dYieldRotn3rd[2];    //
};
struct T_IEHP_ELATRI          // : 6.6.0 추가 모델
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)  
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)  
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    
	double dYieldDisp1st[2];    // 
	double dYieldDisp2nd[2];    // 
	double dYieldDisp3rd[2];    // 
	double dYieldRotn1st[2];    // 
	double dYieldRotn2nd[2];    // 
	double dYieldRotn3rd[2];    // 
	double dInitStiffP;         // 7.0.0 추가
	double dInitStiffN;         // 7.0.0 추가
	double dInitStiffnessDist;	// v955 추가
	void Initialize()
	{
		nSymmetryType = 0;
		nYieldStrengthOpt = 1;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i] = 0.5;
			dCrackMoment[i] = 0.5;
			dYieldForce[i] = 1.0;
			dYieldMoment[i] = 1.0;      
			dUltimateForce[i] = 1.0;
			dUltimateMoment[i] = 1.0;
			dStiffRatio1st[i] = 0.5;
			dStiffRatio2nd[i] = 0.1;
			dYieldDisp1st[i]  = 0.1;
			dYieldDisp2nd[i]  = 0.2;
			dYieldDisp3rd[i]  = 0.3;
			dYieldRotn1st[i]  = 0.1;
			dYieldRotn2nd[i]  = 0.2;
			dYieldRotn3rd[i]  = 0.3;
			dDeformCapacity[i][0] = 0.5;
			dDeformCapacity[i][1] = 1.0;
			dDeformCapacity[i][2] = 2.0;
			dDeformCapacity[i][3] = 4.0;
			dDeformCapacity[i][4] = 8.0;      
		} 
		nDeformDefineType = 1;
		nInitStiffType = 0;
		dInitStiffness = 1.0;
		nPAlphaDelta = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert(const T_IEHP_ELATRI& rData)
	{
		nSymmetryType = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dUltimateForce[i] ;
			dUltimateMoment[i] = rData.dUltimateMoment[i];
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i]  ;
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i]  ;
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i]  ;
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i]  ;
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i]  ;
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i]  ;
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType = rData.nInitStiffType;
		dInitStiffness = rData.dInitStiffness;
		nPAlphaDelta = rData.nPAlphaDelta;
		dInitStiffP = rData.dInitStiffP;
		dInitStiffN = rData.dInitStiffN;
		dInitStiffnessDist = rData.dInitStiffnessDist;
	}
	void Convert692(const T_IEHP_ELATRI_692& rData)
	{
		nSymmetryType = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dUltimateForce[i] ;
			dUltimateMoment[i] = rData.dUltimateMoment[i];
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i]  ;
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i]  ;
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i]  ;
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i]  ;
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i]  ;
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i]  ;
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType = rData.nInitStiffType;
		dInitStiffness = rData.dInitStiffness;
		nPAlphaDelta = rData.nPAlphaDelta;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
};
struct T_IEHP_ELATET_692      // : 6.6.0 추가 모델
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dFractureForce[2];   // 0:FF(+), 1:FF(-)   (DOF Dx, Dy, Dz 일때 사용)  
	double dFractureMoment[2];  // 0:FF(+), 1:FF(-)   (DOF Rx, Ry, Rz 일때 사용)  
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	double dStiffRatio3rd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 3rd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    
	double dYieldDisp1st[2];    // 
	double dYieldDisp2nd[2];    // 
	double dYieldDisp3rd[2];    // 
	double dYieldDisp4th[2];    // 
	double dYieldRotn1st[2];    // 
	double dYieldRotn2nd[2];    // 
	double dYieldRotn3rd[2];    // 
	double dYieldRotn4th[2];    // 
};
struct T_IEHP_ELATET          // : 6.6.0 추가 모델
{
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric           // BOOL bSymmetric;
	int nYieldStrengthOpt;      // 0:User Input, 1:Auto Calculation    // BOOL bUseCalcValueYield;  
	double dCrackForce[2];      // 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) // dCrackValue[2]
	double dCrackMoment[2];     // 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     // 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   // 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dUltimateMoment[2];  // 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dFractureForce[2];   // 0:FF(+), 1:FF(-)   (DOF Dx, Dy, Dz 일때 사용)  
	double dFractureMoment[2];  // 0:FF(+), 1:FF(-)   (DOF Rx, Ry, Rz 일때 사용)  
	double dStiffRatio1st[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	double dStiffRatio3rd[2];   // 0:(+), 1:(-)     Stiffness Reduction Ratio 3rd
	int    nDeformDefineType;   // 0:D/DC, 1:D/DY   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;      // 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness(6.6.0추가), 5:Skeleton Curve(6.6.0추가)
	double dInitStiffness;
	int    nPAlphaDelta;        // 0:P-Alpha, 1:P-Delta    
	double dYieldDisp1st[2];    // 
	double dYieldDisp2nd[2];    // 
	double dYieldDisp3rd[2];    // 
	double dYieldDisp4th[2];    // 
	double dYieldRotn1st[2];    // 
	double dYieldRotn2nd[2];    // 
	double dYieldRotn3rd[2];    // 
	double dYieldRotn4th[2];    // 
	double dInitStiffP;         // 7.0.0 추가
	double dInitStiffN;         // 7.0.0 추가
	double dInitStiffnessDist;	// v955 추가
	void Initialize()
	{
		nSymmetryType = 0;
		nYieldStrengthOpt = 1;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i] = 0.5;
			dCrackMoment[i] = 0.5;
			dYieldForce[i] = 1.0;
			dYieldMoment[i] = 1.0;      
			dUltimateForce[i] = 1.0;
			dUltimateMoment[i] = 1.0;      
			dFractureForce[i] = 1.0;
			dFractureMoment[i] = 1.0;      
			dStiffRatio1st[i] = 0.5;
			dStiffRatio2nd[i] = 0.1;
			dStiffRatio3rd[i] = 0.01;
			dYieldDisp1st[i]  = 0.1;
			dYieldDisp2nd[i]  = 0.2;
			dYieldDisp3rd[i]  = 0.3;
			dYieldDisp4th[i]  = 0.4;
			dYieldRotn1st[i]  = 0.1;
			dYieldRotn2nd[i]  = 0.2;
			dYieldRotn3rd[i]  = 0.3;
			dYieldRotn4th[i]  = 0.4;
			dDeformCapacity[i][0] = 0.5;
			dDeformCapacity[i][1] = 1.0;
			dDeformCapacity[i][2] = 2.0;
			dDeformCapacity[i][3] = 4.0;
			dDeformCapacity[i][4] = 8.0;      
		} 
		nDeformDefineType = 1;
		nInitStiffType = 0;
		dInitStiffness = 1.0;
		nPAlphaDelta = 0;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert(const T_IEHP_ELATET& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dUltimateForce[i] ;
			dUltimateMoment[i] = rData.dUltimateMoment[i];      
			dFractureForce[i]  = rData.dFractureForce[i] ;
			dFractureMoment[i] = rData.dFractureMoment[i];      
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dStiffRatio3rd[i]  = rData.dStiffRatio3rd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i]  ;
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i]  ;
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i]  ;
			dYieldDisp4th[i]   = rData.dYieldDisp4th[i]  ;
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i]  ;
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i]  ;
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i]  ;
			dYieldRotn4th[i]   = rData.dYieldRotn4th[i]  ;
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType = rData.nInitStiffType;
		dInitStiffness = rData.dInitStiffness;
		nPAlphaDelta = rData.nPAlphaDelta;
		dInitStiffP = rData.dInitStiffP;
		dInitStiffN = rData.dInitStiffN;
		dInitStiffnessDist = rData.dInitStiffnessDist;
	}
	void Convert692(const T_IEHP_ELATET_692& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;    
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dUltimateForce[i] ;
			dUltimateMoment[i] = rData.dUltimateMoment[i];      
			dFractureForce[i]  = rData.dFractureForce[i] ;
			dFractureMoment[i] = rData.dFractureMoment[i];      
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dStiffRatio3rd[i]  = rData.dStiffRatio3rd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i]  ;
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i]  ;
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i]  ;
			dYieldDisp4th[i]   = rData.dYieldDisp4th[i]  ;
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i]  ;
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i]  ;
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i]  ;
			dYieldRotn4th[i]   = rData.dYieldRotn4th[i]  ;
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType = rData.nInitStiffType;
		dInitStiffness = rData.dInitStiffness;
		nPAlphaDelta = rData.nPAlphaDelta;
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
};
struct T_IEHP_YLDSUR_955
{
	int    nCurveUserAuto;  // 0:User Input, 1:Auto Calculation   : P-M Interaction Curve
	double dPC0;
	double dMC0[2];
	double dPC[2];
	double dPCB[2];
	double dMC[2];
	double dPY[2];
	double dPYB[2];
	double dMY[2];
	int    nSurfaceUserAuto; // 0:User Input, 1:Auto Calculation   : Approximation of Yield Surface Shape  
	double dP1st[2][11];     // y-axis P/PC  z-axis P/PC  (1st P-M Interaction Curve)
	double dM1st[2][11];     // y-axis M/MCy z-axis M/MCy (1st P-M Interaction Curve)
	double dP2nd[2][11];     // y-axis P/PC  z-axis P/PC  (2nd P-M Interaction Curve)
	double dM2nd[2][11];     // y-axis M/MCy z-axis M/MCy (2nd P-M Interaction Curve)
	double dBetay1st[2];     // 0: C, 1: T
	double dBetay2nd[2];     // 0: C, 1: T
	double dBetaz1st[2];     // 0: C, 1: T
	double dBetaz2nd[2];     // 0: C, 1: T
	double dGamma1st;
	double dGamma2nd;
	double dAlpha;
	int    nCouplingType;        // 0: Axial force, 1: Coupled in Status determination, 2:Fully coupled
};
struct T_IEHP_YLDSUR
{  
	int    nCurveUserAuto;  // 0:User Input, 1:Auto Calculation   : P-M Interaction Curve
	double dPC0;
	double dMC0[2];
	double dPC[2];
	double dPCB[2];
	double dMC[2];
	double dPY[2];
	double dPYB[2];
	double dMY[2];
	double dPU[2];
	double dPUB[2];
	double dMU[2];
	int    nSurfaceUserAuto; // 0:User Input, 1:Auto Calculation   : Approximation of Yield Surface Shape  
	double dP1st[2][11];     // y-axis P/PC  z-axis P/PC  (1st P-M Interaction Curve)
	double dM1st[2][11];     // y-axis M/MCy z-axis M/MCy (1st P-M Interaction Curve)
	double dP2nd[2][11];     // y-axis P/PC  z-axis P/PC  (2nd P-M Interaction Curve)
	double dM2nd[2][11];     // y-axis M/MCy z-axis M/MCy (2nd P-M Interaction Curve)
	double dP3rd[2][11];     // y-axis P/PC  z-axis P/PC  (3rd P-M Interaction Curve)
	double dM3rd[2][11];     // y-axis M/MCy z-axis M/MCy (3rd P-M Interaction Curve)
	double dBetay1st[2];     // 0: C, 1: T
	double dBetay2nd[2];     // 0: C, 1: T
	double dBetay3rd[2];     // 0: C, 1: T
	double dBetaz1st[2];     // 0: C, 1: T
	double dBetaz2nd[2];     // 0: C, 1: T
	double dBetaz3rd[2];     // 0: C, 1: T
	double dGamma1st;
	double dGamma2nd;
	double dGamma3rd;
	double dAlpha;               
	int    nCouplingType;        // 0: Axial force, 1: Coupled in Status determination, 2:Fully coupled

	void Initialize()
	{
		nCurveUserAuto = 1;
		dPC0 = 0.4;
		dPC[0] = 0.36;
		dPC[1] = 0.90;
		for(int i = 0; i < 2; i++)
		{
			dMC0[i] = 0.5;      
			dPCB[i] = 0.0;
			dMC[i] = 0.55;
			dPY[i] = 1.0;
			dPYB[i] = 0.0;
			dMY[i] = 1.0;
			dPU[i] = 1.0;
			dPUB[i] = 0.0;
			dMU[i] = 1.0;
		}        
		nSurfaceUserAuto = 1;
		double P[] = {1.0, 0.8, 0.6, 0.4, 0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
		double M[] = {0.0, 0.3, 0.6, 0.8, 0.9, 1.0, 0.9, 0.8, 0.6, 0.3, 0.0};    

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 11; j++)
			{
				dP1st[i][j] = dP2nd[i][j] = dP3rd[i][j] = P[j];
				dM1st[i][j] = dM2nd[i][j] = dM3rd[i][j] = M[j];
			}
		}
			
		for(int i = 0; i < 2; i++)
		{
			dBetay1st[i] = dBetay2nd[i] = dBetay3rd[i] = dBetaz1st[i] = dBetaz2nd[i] = dBetaz3rd[i] = 2.;
		}

		dGamma1st = 1.1;
		dGamma2nd = 1.1;
		dGamma3rd = 1.1;
		dAlpha = 1.4;
		nCouplingType = 2;
	}
	void Convert(const T_IEHP_YLDSUR& rData)
 	{
		 nCurveUserAuto=rData.nCurveUserAuto;
 		 dPC0 = rData.dPC0;
		 memcpy(dPC, rData.dPC, sizeof(dPC));
		 memcpy(dMC0, rData.dMC0, sizeof(dMC0));
		 memcpy(dPCB, rData.dPCB, sizeof(dPCB));
		 memcpy(dMC, rData.dMC, sizeof(dMC));
		 memcpy(dPY, rData.dPY, sizeof(dPY));
		 memcpy(dPYB, rData.dPYB, sizeof(dPYB));
		 memcpy(dMY, rData.dMY, sizeof(dMY));
		 memcpy(dPU, rData.dPU, sizeof(dPU));
		 memcpy(dPUB, rData.dPUB, sizeof(dPUB));
		 memcpy(dMU, rData.dMU, sizeof(dMU));
		 nSurfaceUserAuto=rData.nSurfaceUserAuto;
		 memcpy(dP1st, rData.dP1st, sizeof(dP1st));
		 memcpy(dP2nd, rData.dP2nd, sizeof(dP2nd));
		 memcpy(dP3rd, rData.dP3rd, sizeof(dP3rd));
		 memcpy(dM1st, rData.dM1st, sizeof(dM1st));    
		 memcpy(dM2nd, rData.dM2nd, sizeof(dM2nd));
		 memcpy(dM3rd, rData.dM3rd, sizeof(dM3rd));
		 memcpy(dBetay1st, rData.dBetay1st, sizeof(dBetay1st));
		 memcpy(dBetay2nd, rData.dBetay2nd, sizeof(dBetay2nd));
		 memcpy(dBetay3rd, rData.dBetay3rd, sizeof(dBetay3rd));
		 memcpy(dBetaz1st, rData.dBetaz1st, sizeof(dBetaz1st));
		 memcpy(dBetaz2nd, rData.dBetaz2nd, sizeof(dBetaz2nd));        
		 memcpy(dBetaz3rd, rData.dBetaz3rd, sizeof(dBetaz3rd));
		 dGamma1st = rData.dGamma1st;
		 dGamma2nd = rData.dGamma2nd;
		 dGamma3rd = rData.dGamma3rd;
		 dAlpha=rData.dAlpha;
		 nCouplingType=rData.nCouplingType;
 	}
	void Convert650(const T_IEHP_YLDSUR_955& rData)
	{
		nCurveUserAuto=rData.nCurveUserAuto;
		memcpy(dPC, rData.dPC, sizeof(dPC));
		memcpy(dMC0, rData.dMC0, sizeof(dMC0));
		memcpy(dPCB, rData.dPCB, sizeof(dPCB));
		memcpy(dMC, rData.dMC, sizeof(dMC));
		memcpy(dPY, rData.dPY, sizeof(dPY));
		memcpy(dPYB, rData.dPYB, sizeof(dPYB));
		memcpy(dMY, rData.dMY, sizeof(dMY));

		for (int i = 0; i < 2; i++)
		{
			dPU[i] = rData.dPY[i];
			dPUB[i] = rData.dPYB[i];
			dMU[i] = rData.dMY[i];
		}

		nSurfaceUserAuto=rData.nSurfaceUserAuto;
		memcpy(dP1st, rData.dP1st, sizeof(dP1st));
		memcpy(dP2nd, rData.dP2nd, sizeof(dP2nd));

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 11; j++)
			{
				dP3rd[i][j] = rData.dP2nd[i][j];
			}
		}

		memcpy(dM1st, rData.dM1st, sizeof(dM1st));    
		memcpy(dM2nd, rData.dM2nd, sizeof(dM2nd));

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 11; j++)
			{
				dM3rd[i][j] = rData.dM2nd[i][j];
			}			
		}

		memcpy(dBetay1st, rData.dBetay1st, sizeof(dBetay1st));
		memcpy(dBetay2nd, rData.dBetay2nd, sizeof(dBetay2nd));

		for (int i = 0; i < 2; i++)
		{
			dBetay3rd[i] = rData.dBetay2nd[i];
		}

		memcpy(dBetaz1st, rData.dBetaz1st, sizeof(dBetaz1st));
		memcpy(dBetaz2nd, rData.dBetaz2nd, sizeof(dBetaz2nd));        

		for (int i = 0; i < 2; i++)
		{
			dBetaz3rd[i] = rData.dBetaz2nd[i];
		}

		dGamma1st = rData.dGamma1st;
		dGamma2nd = rData.dGamma2nd;
		dGamma3rd = rData.dGamma2nd;
		dAlpha=rData.dAlpha;
		nCouplingType=rData.nCouplingType;    
	}    
	void Convert955(const T_IEHP_YLDSUR_955& rData)
	{
		nCurveUserAuto = rData.nCurveUserAuto;
		dPC0 = rData.dPC0;
		memcpy(dPC, rData.dPC, sizeof(dPC));
		memcpy(dMC0, rData.dMC0, sizeof(dMC0));
		memcpy(dPCB, rData.dPCB, sizeof(dPCB));
		memcpy(dMC, rData.dMC, sizeof(dMC));
		memcpy(dPY, rData.dPY, sizeof(dPY));
		memcpy(dPYB, rData.dPYB, sizeof(dPYB));
		memcpy(dMY, rData.dMY, sizeof(dMY));

		for (int i = 0; i < 2; i++)
		{
			dPU[i] = rData.dPY[i];
			dPUB[i] = rData.dPYB[i];
			dMU[i] = rData.dMY[i];
		}

		nSurfaceUserAuto = rData.nSurfaceUserAuto;
		memcpy(dP1st, rData.dP1st, sizeof(dP1st));
		memcpy(dP2nd, rData.dP2nd, sizeof(dP2nd));

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 11; j++)
			{
				dP3rd[i][j] = rData.dP2nd[i][j];
			}
		}

		memcpy(dM1st, rData.dM1st, sizeof(dM1st));
		memcpy(dM2nd, rData.dM2nd, sizeof(dM2nd));

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 11; j++)
			{
				dM3rd[i][j] = rData.dM2nd[i][j];
			}
		}

		memcpy(dBetay1st, rData.dBetay1st, sizeof(dBetay1st));
		memcpy(dBetay2nd, rData.dBetay2nd, sizeof(dBetay2nd));

		for (int i = 0; i < 2; i++)
		{
			dBetay3rd[i] = rData.dBetay2nd[i];
		}

		memcpy(dBetaz1st, rData.dBetaz1st, sizeof(dBetaz1st));
		memcpy(dBetaz2nd, rData.dBetaz2nd, sizeof(dBetaz2nd));

		for (int i = 0; i < 2; i++)
		{
			dBetaz3rd[i] = rData.dBetaz2nd[i];
		}

		dGamma1st = rData.dGamma1st;
		dGamma2nd = rData.dGamma2nd;
		dGamma3rd = rData.dGamma2nd;
		dAlpha = rData.dAlpha;
		nCouplingType = rData.nCouplingType;
	}
};
struct T_IEHP_LRBBIL
{
	double dQd50;
	double dH;
	double dAlf;
	double dRmin;
	void Initialize()
	{
		dQd50 = 0.;
		dH = 0.;
		dAlf = 0.1538462;
		dRmin = 0.05;
	}
	void Convert(const T_IEHP_LRBBIL& rData)
	{
		dQd50 = rData.dQd50;
		dH = rData.dH;
		dAlf = rData.dAlf;
		dRmin = rData.dRmin;
	}
};
struct T_IEHP_LRBTRI
{
	int    nSW;            // 1: HDR-G12, 2:HDR-G10, 3:LRB-G12, 4:LRB-G10, 5=RB-G12, 6=RB-G10
	double dH;
	double dArea;
	double dRmin;
	int    nRminUpdate;    // 0:Yes, 1:No
	void Initialize()
	{
		nSW = 1;    
		dH = 0.;
		dArea = 0.0;
		dRmin = 0.01;
		nRminUpdate = 0;
	}
	void Convert(const T_IEHP_LRBTRI& rData)
	{
		nSW = rData.nSW;
		dH = rData.dH;
		dArea = rData.dArea;
		dRmin = rData.dRmin;
		nRminUpdate = rData.nRminUpdate;
	}
};
struct T_IEHP_DMPRBR  
{
	int    nSW;        // 0:KL301, 1:KL401, 2:KL302, 3:KL501, 4:UHD-G6, 5:HD-G8, 6:TOYO, 7:G-8, 8:G-10, 9:G-12
	double dH;
	double dArea;
	double dGs;
	double dHs;
	double dUs;  
	void Initialize()
	{
		nSW = 0;    
		dH = 0.;
		dArea = 0.0;
		dGs = 1.0;
		dHs = 1.0;
		dUs = 1.0;    
	}
	void Convert(const T_IEHP_DMPRBR& rData)
	{
		nSW = rData.nSW;
		dH = rData.dH;
		dArea = rData.dArea;
		dGs = rData.dGs;
		dHs = rData.dHs;
		dUs = rData.dUs;
	}
};

struct T_IEHP_LRBH16
{
	int nSW;
	double dH;
	double dArea;
	double dAreap;
	void Initialize()
	{
		nSW = 0;
		dH = 0.0;
		dArea = 0.0;
		dAreap = 0.0;
	}
};

struct T_IEHP_LRBH30
{
	int nSW;
	double dH;
	double dArea;
	double dAreap;
	double dCb;
	void Initialize()
	{
		nSW = 0;
		dH = 0.0;
		dArea = 0.0;
		dAreap = 0.0;
		dCb = 1.0;
	}
};

struct T_IEHP_DMPRH16
{
	int    nHDR;       // 0:HDR, 1:HDRS
	int    nSW;        // 0:G-8, 1:G-10, 2:G-12
	double dH;
	double dArea;
	void Initialize()
	{
	  nHDR = 0;
		nSW = 0;
		dH = 0.;
		dArea = 0.0;
	}
};

struct T_IEHP_DMPRH30
{
	int    nSW;        // 0:G-8, 1:G-10, 2:G-12
	double dH;
	double dArea;
	double dCb;
	void Initialize()
	{
		nSW = 0;
		dH = 0.;
		dArea = 0.0;
		dCb = 1.0;
	}
};

struct T_IEHP_SLIP_692 				  // 6.9.0에 추가...By HP (2005/04/29)
{
	int		 nSymmetryType;					// 0:Symmetric, 1:Asymmetric  
	int		 nYieldStrengthOpt;			// 0:User Input, 1:Auto Calculation 
	int    nPAlphaDelta;					// 0:P-Alpha (Stiffness Reduction Ratio), 1:P-Delta (Yield Displacement)...
	double dCrackForce[2];				// 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) 
	double dCrackMoment[2];				// 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      	// 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     	// 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   	// 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용) 
	double dUltimateMoment[2];  	// 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dStiffRatio1st[2];   	// 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   	// 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	double dYieldDisp1st[2];    	// 0:(+), 1:(-)
	double dYieldDisp2nd[2];    	// 0:(+), 1:(-)
	double dYieldDisp3rd[2];    	// 0:(+), 1:(-)
	double dYieldRotn1st[2];    	// 0:(+), 1:(-)
	double dYieldRotn2nd[2];    	// 0:(+), 1:(-)
	double dYieldRotn3rd[2];    	// 0:(+), 1:(-)
	int    nDeformDefineType;			// 0:D/D1, 1:D/D2   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;				// 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness, 5:Skeleton Curve  
	double dInitStiffness;				// user input 일때 사용..  
	double dInitGapPositive;			// Initial Gap Positive Direction
	double dInitGapNegative;			// Initial Gap Negative Direction
};
struct T_IEHP_SLIP							// 6.9.0에 추가...By HP (2005/04/29)
{
	int		 nSymmetryType;					// 0:Symmetric, 1:Asymmetric  
	int		 nYieldStrengthOpt;			// 0:User Input, 1:Auto Calculation 
	int    nPAlphaDelta;					// 0:P-Alpha (Stiffness Reduction Ratio), 1:P-Delta (Yield Displacement)...
	double dCrackForce[2];				// 0:FC(+), 1:FC(-)   (DOF Dx, Dy, Dz 일때 사용) 
	double dCrackMoment[2];				// 0:FC(+), 1:FC(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dYieldForce[2];      	// 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일때 사용)
	double dYieldMoment[2];     	// 0:FY(+), 1:FY(-)   (DOF Rx, Ry, Rz 일때 사용)
	double dUltimateForce[2];   	// 0:FU(+), 1:FU(-)   (DOF Dx, Dy, Dz 일때 사용) 
	double dUltimateMoment[2];  	// 0:FU(+), 1:FU(-)   (DOF Rx, Ry, Rz 일때 사용) 
	double dStiffRatio1st[2];   	// 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];   	// 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd
	double dYieldDisp1st[2];    	// 0:(+), 1:(-)
	double dYieldDisp2nd[2];    	// 0:(+), 1:(-)
	double dYieldDisp3rd[2];    	// 0:(+), 1:(-)
	double dYieldRotn1st[2];    	// 0:(+), 1:(-)
	double dYieldRotn2nd[2];    	// 0:(+), 1:(-)
	double dYieldRotn3rd[2];    	// 0:(+), 1:(-)
	int    nDeformDefineType;			// 0:D/D1, 1:D/D2   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	int    nInitStiffType;				// 0:6EI/L, 1:3EI/L, 2:2EI/L, 3:User, 4:Elastic Stiffness, 5:Skeleton Curve  
	double dInitStiffness;				// user input 일때 사용..  
	double dInitGapPositive;			// Initial Gap Positive Direction
	double dInitGapNegative;			// Initial Gap Negative Direction
	double dInitStiffP;         // 7.0.0 추가
	double dInitStiffN;         // 7.0.0 추가
	double dInitStiffnessDist;		// v955 추가

	void Initialize()
	{
		nSymmetryType = 0;
		nYieldStrengthOpt = 1;  
		nPAlphaDelta = 0;
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i] = 0.5;
			dCrackMoment[i] = 0.5;
			dYieldForce[i] = 1.0;
			dYieldMoment[i] = 1.0;      
			dUltimateForce[i] = 1.0;
			dUltimateMoment[i] = 1.0;
			dStiffRatio1st[i] = 0.5;
			dStiffRatio2nd[i] = 0.1;
			dYieldDisp1st[i]  = 0.1;
			dYieldDisp2nd[i]  = 0.2;
			dYieldDisp3rd[i]  = 0.3;
			dYieldRotn1st[i]  = 0.1;
			dYieldRotn2nd[i]  = 0.2;
			dYieldRotn3rd[i]  = 0.3;
			dDeformCapacity[i][0] = 0.5;
			dDeformCapacity[i][1] = 1.0;
			dDeformCapacity[i][2] = 2.0;
			dDeformCapacity[i][3] = 4.0;
			dDeformCapacity[i][4] = 8.0;      
		} 
		nDeformDefineType = 1;
		nInitStiffType = 0;
		dInitStiffness = 1.0;
		dInitGapPositive = 0.0;
		dInitGapNegative  = 0.0;  
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert(const T_IEHP_SLIP& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;  
		nPAlphaDelta      = rData.nPAlphaDelta;
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dUltimateForce[i] ;
			dUltimateMoment[i] = rData.dUltimateMoment[i];
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i]  ;
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i]  ;
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i]  ;
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i]  ;
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i]  ;
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i]  ;
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		}
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType = rData.nInitStiffType;
		dInitStiffness = rData.dInitStiffness;
		dInitGapPositive = rData.dInitGapPositive;
		dInitGapNegative  = rData.dInitGapNegative;  
		dInitStiffP = rData.dInitStiffP;
		dInitStiffN = rData.dInitStiffN;
		dInitStiffnessDist = rData.dInitStiffnessDist;
	}
	void Convert692(const T_IEHP_SLIP_692& rData)
	{
		nSymmetryType     = rData.nSymmetryType;
		nYieldStrengthOpt = rData.nYieldStrengthOpt;  
		nPAlphaDelta      = rData.nPAlphaDelta;
		for(int i = 0; i < 2; i++)
		{
			dCrackForce[i]     = rData.dCrackForce[i]    ;
			dCrackMoment[i]    = rData.dCrackMoment[i]   ;
			dYieldForce[i]     = rData.dYieldForce[i]    ;
			dYieldMoment[i]    = rData.dYieldMoment[i]   ;      
			dUltimateForce[i]  = rData.dUltimateForce[i] ;
			dUltimateMoment[i] = rData.dUltimateMoment[i];
			dStiffRatio1st[i]  = rData.dStiffRatio1st[i] ;
			dStiffRatio2nd[i]  = rData.dStiffRatio2nd[i] ;
			dYieldDisp1st[i]   = rData.dYieldDisp1st[i]  ;
			dYieldDisp2nd[i]   = rData.dYieldDisp2nd[i]  ;
			dYieldDisp3rd[i]   = rData.dYieldDisp3rd[i]  ;
			dYieldRotn1st[i]   = rData.dYieldRotn1st[i]  ;
			dYieldRotn2nd[i]   = rData.dYieldRotn2nd[i]  ;
			dYieldRotn3rd[i]   = rData.dYieldRotn3rd[i]  ;
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		}
		nDeformDefineType = rData.nDeformDefineType;
		nInitStiffType = rData.nInitStiffType;
		dInitStiffness = rData.dInitStiffness;
		dInitGapPositive = rData.dInitGapPositive;
		dInitGapNegative  = rData.dInitGapNegative;  
		dInitStiffP = 1.0;
		dInitStiffN = 1.0;
		dInitStiffnessDist = 1.0;
	}
};
struct T_IEHP_ROHD // 7.0.0추가
{
	double dCriticalDispl;
	double dMaxDamping;
	void Initialize()
	{
		dCriticalDispl = 0.0;
		dMaxDamping    = 0.0;
	}
	void Convert(const T_IEHP_ROHD& rData)
	{
		dCriticalDispl = rData.dCriticalDispl;
		dMaxDamping = rData.dMaxDamping;
	}
};
struct T_IEHP_MLIN_815
{
	int    nMultiType;
	int    nDeformDefineType;			// 0:D/D1, 1:D/D2   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	double dInitStiffP;
	double dInitStiffN;
	double dHysParam_Alpha1;
	double dHysParam_Alpha2;
	double dHysParam_Beta1;
	double dHysParam_Beta2;
	double dHysParam_Eta;
};
struct T_IEHP_MLIN
{
	int    nMultiType;
	int    nDeformDefineType;			// 0:D/D1, 1:D/D2   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity
	double dInitStiffP;
	double dInitStiffN;
	double dHysParam_Alpha1;
	double dHysParam_Alpha2;
	double dHysParam_Beta1;
	double dHysParam_Beta2;
	double dHysParam_Eta;
	int    nType;         // 820  // 0:Both 1:Tens.Only 2:Comp.Only (D_IEHP_MLKI, D_IEHP_MLTA 이외는 Both type)  
	double dScaleF_Displ; // 820
	double dScaleF_Force; // 820
	
	void Initialize()
	{
		nMultiType = 0;
		nDeformDefineType = 1;
		dInitStiffP = 0.0;
		dInitStiffN = 0.0;

		for(int i = 0; i < 2; i++)
		{
			dDeformCapacity[i][0] = 0.5;
			dDeformCapacity[i][1] = 1.0;
			dDeformCapacity[i][2] = 2.0;
			dDeformCapacity[i][3] = 4.0;
			dDeformCapacity[i][4] = 8.0;      
		} 
		dHysParam_Alpha1 = 10.0;  
		dHysParam_Alpha2 = 10.0;
		dHysParam_Beta1  = 0.7;
		dHysParam_Beta2  = 0.7;
		dHysParam_Eta    = 0.0;
		nType = 0;    
		dScaleF_Displ = 1.;
		dScaleF_Force = 1.;
	}
	void Convert815(const T_IEHP_MLIN_815& rData)
	{		
		nMultiType = rData.nMultiType;
		nDeformDefineType = rData.nDeformDefineType;
		dInitStiffP = rData.dInitStiffP;
		dInitStiffN = rData.dInitStiffN;
		
		for(int i = 0; i < 2; i++)
		{
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4];      
		} 
		dHysParam_Alpha1 = rData.dHysParam_Alpha1;  
		dHysParam_Alpha2 = rData.dHysParam_Alpha2;
		dHysParam_Beta1  = rData.dHysParam_Beta1;
		dHysParam_Beta2  = rData.dHysParam_Beta2;
		dHysParam_Eta    = rData.dHysParam_Eta;
		nType = 0;
		dScaleF_Displ = 1.;
		dScaleF_Force = 1.;
	}
};

struct T_IEHP_AXIAL
{
	int nYieldStrengthOpt;        // 0:User Input, 1:Auto Calculation
	int nCompStiffOPt;            // 0:Elastic, 1:Degradation
	double dYieldForce[2];        // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일 때 사용)
	double dYieldReduFac;
	double dStiffRatio1st[2];     // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dStiffRatio2nd[2];     // 0:(+), 1:(-)     Stiffness Reduction Ratio 2nd

	int    nDeformDefineType;			// 0:D/D1, 1:D/D2   Ductility Factor
	double dDeformCapacity[2][5]; // Deformation Capacity

	int    nInitStiffType;        // 3:User, 4:Elastic Stiffness
	double dInitStiffness;
	double dInitStiffnessDist;	// v955 추가

	void Initialize()
	{
		nYieldStrengthOpt = 1;
		nCompStiffOPt     = 0;
		for(int i = 0; i < 2; i++)
		{
			dYieldForce[i] = 1.0;
			dDeformCapacity[i][0] = 0.5;
			dDeformCapacity[i][1] = 1.0;
			dDeformCapacity[i][2] = 2.0;
			dDeformCapacity[i][3] = 4.0;
			dDeformCapacity[i][4] = 8.0; 
		}
		dStiffRatio1st[0] = 0.2;
		dStiffRatio1st[1] = 1.0;
		dStiffRatio2nd[0] = 0.001;
		dStiffRatio2nd[1] = 1.0;
		dYieldReduFac = 1.0;
		nDeformDefineType = 1;
		nInitStiffType = 4;
		dInitStiffness = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert(const T_IEHP_AXIAL& rData)
	{
		nYieldStrengthOpt = rData.nYieldStrengthOpt;
		nCompStiffOPt     = rData.nCompStiffOPt    ;
		for(int i = 0; i < 2; i++)
		{
			dYieldForce[i]        = rData.dYieldForce[i]       ;
			dDeformCapacity[i][0] = rData.dDeformCapacity[i][0];
			dDeformCapacity[i][1] = rData.dDeformCapacity[i][1];
			dDeformCapacity[i][2] = rData.dDeformCapacity[i][2];
			dDeformCapacity[i][3] = rData.dDeformCapacity[i][3];
			dDeformCapacity[i][4] = rData.dDeformCapacity[i][4]; 
		}
		dStiffRatio1st[0] = rData.dStiffRatio1st[0] ;
		dStiffRatio1st[1] = rData.dStiffRatio1st[1] ;
		dStiffRatio2nd[0] = rData.dStiffRatio2nd[0] ;
		dStiffRatio2nd[1] = rData.dStiffRatio2nd[1] ;
		dYieldReduFac     = rData.dYieldReduFac     ;
		nDeformDefineType = rData.nDeformDefineType ;
		nInitStiffType    = rData.nInitStiffType    ;
		dInitStiffness    = rData.dInitStiffness    ;
		dInitStiffnessDist= rData.dInitStiffnessDist;
	}

};

struct T_IEHP_BUCKL
{
	int nYieldStrengthOpt;        // 0:User Input, 1:Auto Calculation
	int nBucklingOpt;             // 0:None(Constant), 1:Buckling Strength Deterioration

	double dYieldForce[2];        // 0:FY(+), 1:FY(-)   (DOF Dx, Dy, Dz 일 때 사용)
	double dStiffRatio1st[2];     // 0:(+), 1:(-)     Stiffness Reduction Ratio 1st
	double dSlednerRatio;         // Element Slenderness Ratio
	BOOL bUserKFactor;
	double dKFactor;              // Effective Buckling Length Factor

	int    nInitStiffType;        // 3:User, 4:Elastic Stiffness
	double dInitStiffness;
	double dInitStiffnessDist;	// v955 추가

	void Initialize()
	{
		nYieldStrengthOpt = 1;
		nBucklingOpt      = 0;
		for(int i = 0; i < 2; i++)
		{
			dYieldForce[i] = 1.0;
			dStiffRatio1st[i] = 1.0;
		}
		dSlednerRatio = 1.0;
		bUserKFactor = FALSE;
		dKFactor = 1.0;
		nInitStiffType = 4;
		dInitStiffness = 1.0;
		dInitStiffnessDist = 1.0;
	}
	void Convert(const T_IEHP_BUCKL& rData)
	{
		nYieldStrengthOpt = rData.nYieldStrengthOpt;
		nBucklingOpt      = rData.nBucklingOpt     ;
		for(int i = 0; i < 2; i++)
		{
			dYieldForce[i]    = rData.dYieldForce[i];
			dStiffRatio1st[i] = rData.dStiffRatio1st[i];
		}
		dSlednerRatio      = rData.dSlednerRatio     ;
		bUserKFactor       = rData.bUserKFactor      ;
		dKFactor           = rData.dKFactor          ;
		nInitStiffType     = rData.nInitStiffType    ;
		dInitStiffness     = rData.dInitStiffness    ;
		dInitStiffnessDist = rData.dInitStiffnessDist;
	}
};

struct T_IEHP_FEMA
{  
	int nYieldStrengthOpt; // 0:User Input, 1:Auto Calculation 
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

	// union인 T_IEHP_PROP에 끼워넣기 위해 = 연산자 제거한 버전
	// multi curve는 Civil-GSD 연동 시 사용하는 기능으로, 당분간 필요없을 듯 함. 필요 시 추가
	BOOL   bUseShapeCurve         ; // v880~
	int    nShapeCurve            ; // D_IEHP_FEMA_SHAPE
	BOOL   bUseStrengthLoss       ;
	BOOL   bStrengthLoss          ; // Yes,No
	int    nInfillStrutType       ; // D_IEHP_FEMA_IFST
	BOOL   bUseTotalStrLossAtE    ; // Check on/off 
	BOOL   bTotalStrLossAtE       ; // Yes,No
	BOOL   bUseUnloadStiffnessType;
	int    nUnloadStiffnessType   ;
	double dExpUnloadStiff_Exp    ;
	double dExpUnloadStiff_PRFact ;
	int    nInfillStiff           ; // Infill strut > 0:User, 1:Elastic Stiffness
	double dInfillStiff_KmiRop    ;
	double dInfillStiff_KmsRop    ;
	double dInfillStiff_Hm        ;
	double dInfillStiff_ThetaM    ;

	// T_PHGT_SKCV 내 변수들
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
	int nUnloadStiffOption; // D_PHGT_FEMA_UNLD_OPTION
	T_PHGT_FEMA_ENRG_DISSIPATION enrgDissipation;

	void Initialize()
	{
		nYieldStrengthOpt = 1;
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

		bUseShapeCurve          = FALSE;
		nShapeCurve             = D_PHGT_FEMA_SHAPE_GENERAL;
		bUseStrengthLoss        = FALSE;
		bStrengthLoss           = TRUE;
		nInfillStrutType        = 1;
		bUseTotalStrLossAtE     = FALSE;
		bTotalStrLossAtE        = TRUE;
		bUseUnloadStiffnessType = FALSE;
		nUnloadStiffnessType    = D_PHGT_FEMA_UNLD_ORIGIN_ORIENTED;
		dExpUnloadStiff_Exp     = 0.4;
		dExpUnloadStiff_PRFact  = 0.5;
		nInfillStiff            = 1;
		dInfillStiff_KmiRop     = 2000.0;
		dInfillStiff_KmsRop     = 500.0;
		dInfillStiff_Hm         = 1.0;
		dInfillStiff_ThetaM     = 1.0;

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
		nUnloadStiffOption = D_PHGT_FEMA_UNLD_OPTION_SELECT_HYST_MODEL;
		enrgDissipation.Initialize();
	}

	void Convert(const T_IEHP_FEMA& rData)
	{
		nYieldStrengthOpt = rData.nYieldStrengthOpt;
		bSymmetric        = rData.bSymmetric       ;
		bUserDefineRatio  = rData.bUserDefineRatio ;
		memcpy(dForceMomentRatio, rData.dForceMomentRatio, sizeof(dForceMomentRatio));
		memcpy(dYieldForce      , rData.dYieldForce      , sizeof(dYieldForce));
		memcpy(dYieldMoment     , rData.dYieldMoment     , sizeof(dYieldMoment));

		bUserDefineYieldDeform = rData.bUserDefineYieldDeform;
		
		memcpy(dDisplacement, rData.dDisplacement, sizeof(dDisplacement));
		memcpy(dStrain      , rData.dStrain      , sizeof(dStrain      ));
		memcpy(dRotation    , rData.dRotation    , sizeof(dRotation    ));
		memcpy(dRotationU   , rData.dRotationU   , sizeof(dRotationU   ));
		memcpy(dIO          , rData.dIO          , sizeof(dIO));
		memcpy(dLS          , rData.dLS          , sizeof(dLS));
		memcpy(dCP          , rData.dCP          , sizeof(dCP));

		nClass                  = rData.nClass;
		bUseShapeCurve          = rData.bUseShapeCurve         ;
		nShapeCurve             = rData.nShapeCurve            ;
		bUseStrengthLoss        = rData.bUseStrengthLoss       ;
		bStrengthLoss           = rData.bStrengthLoss          ;
		nInfillStrutType        = rData.nInfillStrutType       ;
		bUseTotalStrLossAtE     = rData.bUseTotalStrLossAtE    ;
		bTotalStrLossAtE        = rData.bTotalStrLossAtE       ;
		bUseUnloadStiffnessType = rData.bUseUnloadStiffnessType;
		nUnloadStiffnessType    = rData.nUnloadStiffnessType   ;
		dExpUnloadStiff_Exp     = rData.dExpUnloadStiff_Exp    ;
		dExpUnloadStiff_PRFact  = rData.dExpUnloadStiff_PRFact ;
		nInfillStiff            = rData.nInfillStiff           ;
		dInfillStiff_KmiRop     = rData.dInfillStiff_KmiRop    ;
		dInfillStiff_KmsRop     = rData.dInfillStiff_KmsRop    ;
		dInfillStiff_Hm         = rData.dInfillStiff_Hm        ;
		dInfillStiff_ThetaM     = rData.dInfillStiff_ThetaM    ;
		nInitStiffMethod        = rData.nInitStiffMethod       ;


		memcpy(dInitStiffUserTheta  , rData.dInitStiffUserTheta  , sizeof(dInitStiffUserTheta  ));
		memcpy(dInitStiffUserPhi    , rData.dInitStiffUserPhi    , sizeof(dInitStiffUserPhi    ));
		memcpy(dInitStiffUserSprg   , rData.dInitStiffUserSprg   , sizeof(dInitStiffUserSprg   ));
		memcpy(dInitStiffUserTheta_m, rData.dInitStiffUserTheta_m, sizeof(dInitStiffUserTheta_m));
		memcpy(dInitStiffUserPhi_m  , rData.dInitStiffUserPhi_m  , sizeof(dInitStiffUserPhi_m  ));
		memcpy(dInitStiffUserSprg_m , rData.dInitStiffUserSprg_m , sizeof(dInitStiffUserSprg_m ));

		bInitStiffAssymetric = rData.bInitStiffAssymetric;
		nUnloadStiffOption   = rData.nUnloadStiffOption;
		enrgDissipation.Convert(rData.enrgDissipation);
	}

	void InitializeRatio_Infill()
	{
		Initialize();
		dForceMomentRatio[0][0] = -0.005;  dForceMomentRatio[1][0] = -0;     
		dForceMomentRatio[0][1] = -0.005;  dForceMomentRatio[1][1] = -0;     
		dForceMomentRatio[0][2] = -6;      dForceMomentRatio[1][2] = -1;     
		dForceMomentRatio[0][3] = -1;      dForceMomentRatio[1][3] = -0.7;   
		dForceMomentRatio[0][4] = 0;       dForceMomentRatio[1][4] = 0.0;    
	}

	void ToPhgtFEMA(BOOL bInfillStrut, OUT T_PHGT_FEMA& rPhgtFEMA)
	{
		rPhgtFEMA.Initialize();
		rPhgtFEMA.bSymmetric = bSymmetric;
		rPhgtFEMA.bUserDefineRatio = bUserDefineRatio; 
		rPhgtFEMA.bUserDefineYieldDeform = bUserDefineYieldDeform;
		memcpy(rPhgtFEMA.dForceMomentRatio, dForceMomentRatio, sizeof(dForceMomentRatio));
		memcpy(rPhgtFEMA.dYieldForce      , dYieldForce      , sizeof(dYieldForce  ));
		memcpy(rPhgtFEMA.dYieldMoment     , dYieldMoment     , sizeof(dYieldMoment ));
		memcpy(rPhgtFEMA.dDisplacement    , dDisplacement    , sizeof(dDisplacement));
		memcpy(rPhgtFEMA.dStrain          , dStrain          , sizeof(dStrain      ));
		memcpy(rPhgtFEMA.dRotation        , dRotation        , sizeof(dRotation    ));
		memcpy(rPhgtFEMA.dRotationU       , dRotationU       , sizeof(dRotationU   ));
		memcpy(rPhgtFEMA.dIO              , dIO              , sizeof(dIO          ));
		memcpy(rPhgtFEMA.dLS              , dLS              , sizeof(dLS          ));
		memcpy(rPhgtFEMA.dCP              , dCP              , sizeof(dCP          ));
		rPhgtFEMA.nClass                  = nClass                 ; 
		rPhgtFEMA.bUseShapeCurve          = bUseShapeCurve         ; 
		rPhgtFEMA.nShapeCurve             = nShapeCurve            ; 
		rPhgtFEMA.bUseStrengthLoss        = bUseStrengthLoss       ; 
		rPhgtFEMA.bStrengthLoss           = bStrengthLoss          ; 
		rPhgtFEMA.nInfillStrutType        = nInfillStrutType       ; 
		rPhgtFEMA.bUseTotalStrLossAtE     = bUseTotalStrLossAtE    ; 
		rPhgtFEMA.bTotalStrLossAtE        = bTotalStrLossAtE       ; 
		rPhgtFEMA.bUseUnloadStiffnessType = bUseUnloadStiffnessType; 
		rPhgtFEMA.nUnloadStiffnessType    =(bInfillStrut)? D_PHGT_FEMA_UNLD_NORMAL_BILINEAR : nUnloadStiffnessType;
		rPhgtFEMA.dExpUnloadStiff_Exp     = dExpUnloadStiff_Exp    ; 
		rPhgtFEMA.dExpUnloadStiff_PRFact  = dExpUnloadStiff_PRFact ; 
		rPhgtFEMA.nInfillStiff            = nInfillStiff           ; 
		rPhgtFEMA.dInfillStiff_KmiRop     = dInfillStiff_KmiRop    ; 
		rPhgtFEMA.dInfillStiff_KmsRop     = dInfillStiff_KmsRop    ; 
		rPhgtFEMA.dInfillStiff_Hm         = dInfillStiff_Hm        ; 
		rPhgtFEMA.dInfillStiff_ThetaM     = dInfillStiff_ThetaM    ; 
		rPhgtFEMA.nUnloadStiffOption      = nUnloadStiffOption	   ;
		rPhgtFEMA.enrgDissipation		  = enrgDissipation		   ;
	}
	void FromPhgtFEMA(const T_PHGT_FEMA& rPhgtFEMA)
	{
		Initialize();
		bSymmetric              = rPhgtFEMA.bSymmetric;
		bUserDefineRatio        = rPhgtFEMA.bUserDefineRatio; 
		bUserDefineYieldDeform  = rPhgtFEMA.bUserDefineYieldDeform;
		memcpy(dForceMomentRatio, rPhgtFEMA.dForceMomentRatio, sizeof(dForceMomentRatio));
		memcpy(dYieldForce      , rPhgtFEMA.dYieldForce      , sizeof(dYieldForce  ));
		memcpy(dYieldMoment     , rPhgtFEMA.dYieldMoment     , sizeof(dYieldMoment ));
		memcpy(dDisplacement    , rPhgtFEMA.dDisplacement    , sizeof(dDisplacement));
		memcpy(dStrain          , rPhgtFEMA.dStrain          , sizeof(dStrain      ));
		memcpy(dRotation        , rPhgtFEMA.dRotation        , sizeof(dRotation    ));
		memcpy(dRotationU       , rPhgtFEMA.dRotationU       , sizeof(dRotationU   ));
		memcpy(dIO              , rPhgtFEMA.dIO              , sizeof(dIO          ));
		memcpy(dLS              , rPhgtFEMA.dLS              , sizeof(dLS          ));
		memcpy(dCP              , rPhgtFEMA.dCP              , sizeof(dCP          ));
		nClass                  = rPhgtFEMA.nClass                 ; 
		bUseShapeCurve          = rPhgtFEMA.bUseShapeCurve         ; 
		nShapeCurve             = rPhgtFEMA.nShapeCurve            ; 
		bUseStrengthLoss        = rPhgtFEMA.bUseStrengthLoss       ; 
		bStrengthLoss           = rPhgtFEMA.bStrengthLoss          ; 
		nInfillStrutType        = rPhgtFEMA.nInfillStrutType       ; 
		bUseTotalStrLossAtE     = rPhgtFEMA.bUseTotalStrLossAtE    ; 
		bTotalStrLossAtE        = rPhgtFEMA.bTotalStrLossAtE       ; 
		bUseUnloadStiffnessType = rPhgtFEMA.bUseUnloadStiffnessType; 
		nUnloadStiffnessType    = rPhgtFEMA.nUnloadStiffnessType   ; 
		dExpUnloadStiff_Exp     = rPhgtFEMA.dExpUnloadStiff_Exp    ; 
		dExpUnloadStiff_PRFact  = rPhgtFEMA.dExpUnloadStiff_PRFact ; 
		nInfillStiff            = rPhgtFEMA.nInfillStiff           ; 
		dInfillStiff_KmiRop     = rPhgtFEMA.dInfillStiff_KmiRop    ; 
		dInfillStiff_KmsRop     = rPhgtFEMA.dInfillStiff_KmsRop    ; 
		dInfillStiff_Hm         = rPhgtFEMA.dInfillStiff_Hm        ; 
		dInfillStiff_ThetaM     = rPhgtFEMA.dInfillStiff_ThetaM    ; 
		nUnloadStiffOption		= rPhgtFEMA.nUnloadStiffOption     ;
		enrgDissipation			= rPhgtFEMA.enrgDissipation		   ;
	}
};

struct T_IEHP_BASE
{
	double dblDisp;
	double dblForce;
	void Initialize()
	{
		dblDisp=0.;
		dblForce=0.;
	}
};

#define T_IEHP_DT CArray<T_IEHP_BASE, T_IEHP_BASE&>

struct T_IEHP_MULT_CH_RW
{
	int nMultiDataCount;
};
struct T_IEHP_MULT
{
	T_IEHP_DT arMultiLinearData;
	
	void Initialize()
	{
		arMultiLinearData.RemoveAll();
	}
	
	T_IEHP_MULT()
	{
		arMultiLinearData.RemoveAll();
	}
	T_IEHP_MULT(T_IEHP_MULT& src)
	{
		*this = src;
	}
	T_IEHP_MULT& operator=(T_IEHP_MULT& src)
	{
		arMultiLinearData.Copy(src.arMultiLinearData);
		return *this; 
	}  
};

struct T_IEHP_PROP_UNIT
{
	T_IEHP_PROP_UNIT()
	{
		dCrackForce = D_UNITSYS_BASE_FORCE;
		dCrackMoment = D_UNITSYS_BASE_MOMENT;
		dYieldForce = D_UNITSYS_BASE_FORCE;
		dYieldMoment = D_UNITSYS_BASE_MOMENT;
		dUltimateForce = D_UNITSYS_BASE_FORCE;
		dUltimateMoment = D_UNITSYS_BASE_MOMENT;
		dFractureForce = D_UNITSYS_BASE_FORCE;
		dFractureMoment = D_UNITSYS_BASE_MOMENT;
//    dInitStiffness = D_UNITSYS_BASE_MOMENT;
//    dInitStiffnessF = D_UNITSYS_BASE_UNITFORCE;
		dInitStiffMoment = D_UNITSYS_BASE_MOMENT;        // 6.6.0의 dInitStiffness
		dInitStiffForce = D_UNITSYS_BASE_FORCE;       
		dInitStiffUnitForce = D_UNITSYS_BASE_UNITFORCE;  // 6.6.0의 dInitStiffnessF
		dInitStiffDistStiff = D_UNITSYS_BASE_DISTSTIFF;
		dInitStiffRoSpring = D_UNITSYS_BASE_ROSPRING;
		dYieldDisplacement = D_UNITSYS_BASE_LENGTH; 
		dYieldRotation = D_UNITSYS_BASE_RADIAN;  
		dYieldUnitRotn = D_UNITSYS_BASE_RADIAN_L;
		dPC0 = D_UNITSYS_BASE_FORCE;
		dMC0 = D_UNITSYS_BASE_MOMENT;
		dPC = D_UNITSYS_BASE_FORCE;
		dPCB = D_UNITSYS_BASE_FORCE;
		dMC = D_UNITSYS_BASE_MOMENT;
		dPY = D_UNITSYS_BASE_FORCE;
		dPYB = D_UNITSYS_BASE_FORCE;
		dMY = D_UNITSYS_BASE_MOMENT;
		dPU = D_UNITSYS_BASE_FORCE;
		dPUB = D_UNITSYS_BASE_FORCE;
		dMU = D_UNITSYS_BASE_MOMENT;
		dPoint = D_UNITSYS_BASE_LENGTH;
		dArea  = D_UNITSYS_BASE_AREA;    
		dAreap = D_UNITSYS_BASE_AREA;			// 9.6.0
		dQd50  = D_UNITSYS_BASE_FORCE;
		dH = D_UNITSYS_BASE_LENGTH;
		dInitGapPositive = D_UNITSYS_BASE_LENGTH;				// 6.9.0 에서 추가...
		dInitGapNegative = D_UNITSYS_BASE_LENGTH;				// 6.9.0 에서 추가...
	}
	int dCrackForce;
	int dCrackMoment;
	int dYieldForce;
	int dYieldMoment;
	int dUltimateForce;
	int dUltimateMoment;
	int dFractureForce;
	int dFractureMoment;
//  int dInitStiffness;
//  int dInitStiffnessF;
	int dInitStiffMoment; 
	int dInitStiffForce;
	int dInitStiffUnitForce;
	int dInitStiffDistStiff;
	int dInitStiffRoSpring; 
	int dYieldDisplacement;
	int dYieldRotation;
	int dYieldUnitRotn;
	int dPC0;
	int dMC0;
	int dPC;
	int dPCB;
	int dMC;
	int dPY;
	int dPYB;
	int dMY;
	int dPU;
	int dPUB;
	int dMU;
	int dPoint;
	int dArea;
	int dAreap;
	int dQd50;
	int dH;
	int dInitGapPositive;	
	int dInitGapNegative;  
};
union T_IEHP_PROP_650
{
	T_IEHP_KINEMA_650 KINEMA;
	T_IEHP_ORIGIN_650 ORIGIN;
	T_IEHP_PEAK_650   PEAK;
	T_IEHP_CLOUGH_650 CLOUGH;
	T_IEHP_DEGRAD_650 DEGRAD;
	T_IEHP_TAKEDA_650 TAKEDA;
	T_IEHP_YLDSUR_955 YLDSUR;
};
union T_IEHP_PROP_692
{
	T_IEHP_YLDSUR_955 YLDSUR;
	T_IEHP_KINEMA_692 KINEMA;
	T_IEHP_ORIGIN_692 ORIGIN;
	T_IEHP_PEAK_692   PEAK;  
	T_IEHP_CLOUGH_692 CLOUGH;
	T_IEHP_DEGRAD_692 DEGRAD;
	T_IEHP_TAKEDA_692 TAKEDA;
	T_IEHP_TAKTET_692 TAKTET;
	T_IEHP_NORBIL_692 NORBIL;
	T_IEHP_ELABIL_692 ELABIL;
	T_IEHP_ELATRI_692 ELATRI;
	T_IEHP_ELATET_692 ELATET;
	T_IEHP_LRBBIL     LRBBIL;
	T_IEHP_LRBTRI     LRBTRI;
	T_IEHP_DMPRBR     DMPRBR;
	T_IEHP_SLIP_692		SLIP;	
	T_IEHP_ROHD       ROHD;  
};
union T_IEHP_PROP_815
{
	T_IEHP_YLDSUR_955 YLDSUR;
	T_IEHP_KINEMA KINEMA;
	T_IEHP_ORIGIN ORIGIN;
	T_IEHP_PEAK   PEAK;
	T_IEHP_CLOUGH CLOUGH;
	T_IEHP_DEGRAD DEGRAD;
	T_IEHP_TAKEDA TAKEDA;
	T_IEHP_TAKTET TAKTET;
	T_IEHP_NORBIL NORBIL;
	T_IEHP_ELABIL ELABIL;
	T_IEHP_ELATRI ELATRI;
	T_IEHP_ELATET ELATET;
	T_IEHP_LRBBIL LRBBIL;
	T_IEHP_LRBTRI LRBTRI;
	T_IEHP_DMPRBR DMPRBR;
	T_IEHP_SLIP		SLIP;
	T_IEHP_ROHD   ROHD;
	T_IEHP_MLIN_815  MULTLIN;
};
union T_IEHP_PROP_955
{
	T_IEHP_YLDSUR_955 YLDSUR;  // Yield Surface Properties
	T_IEHP_KINEMA KINEMA;  // Kinematic Hardening[D_IEHP_KINE]
						   // 비선형탄성형(대칭)[D_IEHP_NELS]
	T_IEHP_ORIGIN ORIGIN;  // Origin-oriented[D_IEHP_ORIG]
						   // (7.0.0) Origin Peak-Oriented[D_IEHP_ORGP]
	T_IEHP_PEAK   PEAK;    // Peak-oriented[D_IEHP_PICK]
	T_IEHP_CLOUGH CLOUGH;  // Clough[D_IEHP_CLOU]
	T_IEHP_DEGRAD DEGRAD;  // Degrading Trilinear[D_IEHP_DEGR]
	T_IEHP_TAKEDA TAKEDA;  // Takeda[D_IEHP_TAKE]
	     				   // Modified Takeda[D_IEHP_MTAK]
						   // 비대칭역향형[D_IEHP_ASYM]
						   // 비선형탄성형(비대칭)[D_IEHP_NELA]
	T_IEHP_TAKTET TAKTET;  // (6.6.0) Takeda Tetralinear[D_IEHP_TTET]
						   // (6.0.0) Modified Takeda Tetralinear[D_IEHP_MTTE]
						   // (7.0.0) Origin-Oriented Tetralinear[D_IEHP_ORGT]
	T_IEHP_NORBIL NORBIL;  // (6.6.0) Normal Bilinear[D_IEHP_NBIL]
	T_IEHP_ELABIL ELABIL;  // (6.6.0) Elastic Bilinear[D_IEHP_EBIL]
	T_IEHP_ELATRI ELATRI;  // (6.6.0) Elastic Trilinear[D_IEHP_ETRI]
	T_IEHP_ELATET ELATET;  // (6.6.0) Elastic Tetralinear[D_IEHP_ETET]
	T_IEHP_LRBBIL LRBBIL;  // (6.6.0) LRB Isolator Bilinear Type[D_IEHP_LRBB]
	T_IEHP_LRBTRI LRBTRI;  // (6.6.0) LRB Isolator Trilinear Type[D_IEHP_LRBT]
	T_IEHP_DMPRBR DMPRBR;  // (6.6.0) High Damping Rubber Isolator[D_IEHP_DMPR]
	T_IEHP_SLIP		SLIP;  // (6.9.0) SLIP Bilinear[D_IEHP_SLPB]
					       // (6.9.0) SLIP Bilinear/Tensile[D_IEHP_SLBT]
						   // (6.9.0) SLIP Bilinear/Compress[D_IEHP_SLBC]
						   // (6.9.0) SLIP Trilinear[D_IEHP_SLPT]
						   // (6.9.0) SLIP Trilinear/Tensile[D_IEHP_SLTT]
						   // (6.9.0) SLIP Trilinear/Compress[D_IEHP_SLTC]
	T_IEHP_ROHD   ROHD;    // (7.0.0) Ramberg Osgood[D_IEHP_RO]
						   // (7.0.0) Hardin Drnevich[D_IEHP_HD]
	T_IEHP_MLIN   MULTLIN; // (7.9.5) Multi Linear Type
	T_IEHP_AXIAL  AXIAL;   // (8.5.1) Axial Stiffness
	T_IEHP_BUCKL  BUCKL;   // (8.5.1) Steel Buckling
	T_IEHP_FEMA   FEMA;    // (8.8.0) FEMA
};
union T_IEHP_PROP				 
{
	T_IEHP_YLDSUR YLDSUR;  // Yield Surface Properties
	T_IEHP_KINEMA KINEMA;  // Kinematic Hardening[D_IEHP_KINE]
												 // 비선형탄성형(대칭)[D_IEHP_NELS]
	T_IEHP_ORIGIN ORIGIN;  // Origin-oriented[D_IEHP_ORIG]
												 // (7.0.0) Origin Peak-Oriented[D_IEHP_ORGP]
	T_IEHP_PEAK   PEAK;    // Peak-oriented[D_IEHP_PICK]
	T_IEHP_CLOUGH CLOUGH;  // Clough[D_IEHP_CLOU]
	T_IEHP_DEGRAD DEGRAD;  // Degrading Trilinear[D_IEHP_DEGR]
	T_IEHP_TAKEDA TAKEDA;  // Takeda[D_IEHP_TAKE]
												 // Modified Takeda[D_IEHP_MTAK]
												 // 비대칭역향형[D_IEHP_ASYM]
												 // 비선형탄성형(비대칭)[D_IEHP_NELA]
	T_IEHP_TAKTET TAKTET;  // (6.6.0) Takeda Tetralinear[D_IEHP_TTET]
												 // (6.0.0) Modified Takeda Tetralinear[D_IEHP_MTTE]
												 // (7.0.0) Origin-Oriented Tetralinear[D_IEHP_ORGT]
	T_IEHP_NORBIL NORBIL;  // (6.6.0) Normal Bilinear[D_IEHP_NBIL]
	T_IEHP_ELABIL ELABIL;  // (6.6.0) Elastic Bilinear[D_IEHP_EBIL]
	T_IEHP_ELATRI ELATRI;  // (6.6.0) Elastic Trilinear[D_IEHP_ETRI]
	T_IEHP_ELATET ELATET;  // (6.6.0) Elastic Tetralinear[D_IEHP_ETET]
	T_IEHP_LRBBIL LRBBIL;  // (6.6.0) LRB Isolator Bilinear Type[D_IEHP_LRBB]
	T_IEHP_LRBTRI LRBTRI;  // (6.6.0) LRB Isolator Trilinear Type[D_IEHP_LRBT]
	T_IEHP_DMPRBR DMPRBR;  // (6.6.0) High Damping Rubber Isolator[D_IEHP_DMPR]

	T_IEHP_DMPRH16 DMPRH16;  // (9.6.0) High Damping Rubber Isolator[D_IEHP_DMPRH16]
	T_IEHP_DMPRH30 DMPRH30;  // (9.6.0) High Damping Rubber Isolator[D_IEHP_DMPRH30]
	T_IEHP_LRBH16 LRBH16;  // (9.6.0) LRB Isolator Bilinear Type[D_IEHP_LRBH16]
	T_IEHP_LRBH30 LRBH30;  // (9.6.0) LRB Isolator Bilinear Type[D_IEHP_LRBH30]

	T_IEHP_SLIP		SLIP;		 // (6.9.0) SLIP Bilinear[D_IEHP_SLPB]
												 // (6.9.0) SLIP Bilinear/Tensile[D_IEHP_SLBT]
												 // (6.9.0) SLIP Bilinear/Compress[D_IEHP_SLBC]
												 // (6.9.0) SLIP Trilinear[D_IEHP_SLPT]
												 // (6.9.0) SLIP Trilinear/Tensile[D_IEHP_SLTT]
												 // (6.9.0) SLIP Trilinear/Compress[D_IEHP_SLTC]
	T_IEHP_ROHD   ROHD;    // (7.0.0) Ramberg Osgood[D_IEHP_RO]
												 // (7.0.0) Hardin Drnevich[D_IEHP_HD]
	T_IEHP_MLIN   MULTLIN; // (7.9.5) Multi Linear Type
	T_IEHP_AXIAL  AXIAL;   // (8.5.1) Axial Stiffness
	T_IEHP_BUCKL  BUCKL;   // (8.5.1) Steel Buckling
	T_IEHP_FEMA   FEMA;    // (8.8.0) FEMA

	T_IEHP_SRCTET SRCTET;  // (9.6.0) SRC Tetralineaer

	void Convert650(const T_IEHP_PROP_650& rData, int nHysModel, int ix, int nHingeType)
	{
		if(ix == 6)
		{
			YLDSUR.Convert650(rData.YLDSUR);
		}
		else
		{
			switch(nHysModel)
			{
			case D_IEHP_KINE: case D_IEHP_NELS: 
				KINEMA.Convert650(rData.KINEMA);
				if(nHingeType == 1 || nHingeType == 2) KINEMA.nInitStiffType = 4; // 660이전 버전의 Hinge Type이 Distributed or Spring 인 경우 
				break;                                                            // Initial Stiffness Type을 Elastic Stiffness로 강제 설정        
			case D_IEHP_ORIG: 
				ORIGIN.Convert650(rData.ORIGIN); 
				if(nHingeType == 1 || nHingeType == 2) ORIGIN.nInitStiffType = 4; 
				break;
			case D_IEHP_PICK: 
				PEAK.Convert650(rData.PEAK); 
				if(nHingeType == 1 || nHingeType == 2) PEAK.nInitStiffType = 4; 
				break;
			case D_IEHP_CLOU: 
				CLOUGH.Convert650(rData.CLOUGH); 
				if(nHingeType == 1 || nHingeType == 2) CLOUGH.nInitStiffType = 4; 
				break;
			case D_IEHP_DEGR: 
				DEGRAD.Convert650(rData.DEGRAD); 
				if(nHingeType == 1 || nHingeType == 2) DEGRAD.nInitStiffType = 4; 
				break;
			case D_IEHP_TAKE: case D_IEHP_MTAK: case D_IEHP_ASYM: case D_IEHP_NELA:
				TAKEDA.Convert650(rData.TAKEDA); 
				if(nHingeType == 1 || nHingeType == 2) TAKEDA.nInitStiffType = 4; 
				break;      
			}
		}
	}
	void Convert692(const T_IEHP_PROP_692& rData, int nHysModel)
	{
		switch(nHysModel)
		{
		case D_IEHP_KINE: case D_IEHP_NELS: 
			KINEMA.Convert692(rData.KINEMA);
			break;
		case D_IEHP_ORIG: case D_IEHP_ORGP:
			ORIGIN.Convert692(rData.ORIGIN); 
			break;
		case D_IEHP_PICK: 
			PEAK.Convert692(rData.PEAK); 
			break;
		case D_IEHP_CLOU: 
			CLOUGH.Convert692(rData.CLOUGH); 
			break;
		case D_IEHP_DEGR: 
			DEGRAD.Convert692(rData.DEGRAD); 
			break;
		case D_IEHP_TAKE: case D_IEHP_MTAK: case D_IEHP_ASYM:
		case D_IEHP_NELA:
			TAKEDA.Convert692(rData.TAKEDA); 
			break;
		case D_IEHP_TTET: case D_IEHP_MTTE: case D_IEHP_ORGT:
			TAKTET.Convert692(rData.TAKTET);
			break;
		case D_IEHP_NBIL:
			NORBIL.Convert692(rData.NORBIL);
			break;
		case D_IEHP_EBIL:
			ELABIL.Convert692(rData.ELABIL);
			break;
		case D_IEHP_ETRI:
			ELATRI.Convert692(rData.ELATRI);
			break;
		case D_IEHP_ETET:
			ELATET.Convert692(rData.ELATET);
			break;
		case D_IEHP_SLPB: case D_IEHP_SLBT: case D_IEHP_SLBC:
		case D_IEHP_SLPT: case D_IEHP_SLTT: case D_IEHP_SLTC:
			SLIP.Convert692(rData.SLIP);
			break;
		}
	}
	void Convert815(const T_IEHP_PROP_815& rData, int nHysModel)
	{
		switch(nHysModel)
		{
		case D_IEHP_KINE: case D_IEHP_NELS: 
			KINEMA.Convert(rData.KINEMA);
			break;
		case D_IEHP_ORIG: case D_IEHP_ORGP:
			ORIGIN.Convert(rData.ORIGIN); 
			break;
		case D_IEHP_PICK: 
			PEAK.Convert(rData.PEAK); 
			break;
		case D_IEHP_CLOU: 
			CLOUGH.Convert(rData.CLOUGH); 
			break;
		case D_IEHP_DEGR: 
			DEGRAD.Convert(rData.DEGRAD); 
			break;
		case D_IEHP_TAKE: case D_IEHP_MTAK: case D_IEHP_ASYM:
		case D_IEHP_NELA:
			TAKEDA.Convert(rData.TAKEDA); 
			break;
		case D_IEHP_TTET: case D_IEHP_MTTE: case D_IEHP_ORGT:
			TAKTET.Convert(rData.TAKTET);
			break;
		case D_IEHP_NBIL:
			NORBIL.Convert(rData.NORBIL);
			break;
		case D_IEHP_EBIL:
			ELABIL.Convert(rData.ELABIL);
			break;
		case D_IEHP_ETRI:
			ELATRI.Convert(rData.ELATRI);
			break;
		case D_IEHP_ETET:
			ELATET.Convert(rData.ELATET);
			break;
		case D_IEHP_SLPB: case D_IEHP_SLBT: case D_IEHP_SLBC:
		case D_IEHP_SLPT: case D_IEHP_SLTT: case D_IEHP_SLTC:
			SLIP.Convert(rData.SLIP);
			break;
		case D_IEHP_MLEL:	case D_IEHP_MLKI:
		case D_IEHP_MLTA:	case D_IEHP_MLPI:
			MULTLIN.Convert815(rData.MULTLIN);
			break;
		}
	}
	void Convert860(const T_IEHP_PROP_955& rData, int nHysModel)
	{
		switch(nHysModel)
		{
		case D_IEHP_KINE: case D_IEHP_NELS: 
			KINEMA.Convert(rData.KINEMA);
			break;
		case D_IEHP_ORIG: case D_IEHP_ORGP:
			ORIGIN.Convert(rData.ORIGIN); 
			break;
		case D_IEHP_PICK: 
			PEAK.Convert(rData.PEAK); 
			break;
		case D_IEHP_CLOU: 
			CLOUGH.Convert(rData.CLOUGH); 
			break;
		case D_IEHP_DEGR: 
			DEGRAD.Convert(rData.DEGRAD); 
			break;
		case D_IEHP_TAKE: case D_IEHP_MTAK: case D_IEHP_ASYM:
		case D_IEHP_NELA:
			TAKEDA.Convert(rData.TAKEDA); 
			break;
		case D_IEHP_TTET: case D_IEHP_MTTE: case D_IEHP_ORGT:
			TAKTET.Convert(rData.TAKTET);
			break;
		case D_IEHP_NBIL:
			NORBIL.Convert(rData.NORBIL);
			break;
		case D_IEHP_EBIL:
			ELABIL.Convert(rData.ELABIL);
			break;
		case D_IEHP_ETRI:
			ELATRI.Convert(rData.ELATRI);
			break;
		case D_IEHP_ETET:
			ELATET.Convert(rData.ELATET);
			break;
		case D_IEHP_SLPB: case D_IEHP_SLBT: case D_IEHP_SLBC:
		case D_IEHP_SLPT: case D_IEHP_SLTT: case D_IEHP_SLTC:
			SLIP.Convert(rData.SLIP);
			break;
		case D_IEHP_MLEL:	case D_IEHP_MLKI:
		case D_IEHP_MLTA:	case D_IEHP_MLPI:
			memcpy(&MULTLIN, &rData.MULTLIN, sizeof(T_IEHP_MLIN));
			break;
		}
	}
	void Convert955(const T_IEHP_PROP_955& rData, int nHysModel)
	{
		switch (nHysModel)
		{
		case D_IEHP_KINE: case D_IEHP_NELS:
			KINEMA.Convert(rData.KINEMA);
			break;
		case D_IEHP_ORIG: case D_IEHP_ORGP:
			ORIGIN.Convert(rData.ORIGIN);
			break;
		case D_IEHP_PICK:
			PEAK.Convert(rData.PEAK);
			break;
		case D_IEHP_CLOU:
			CLOUGH.Convert(rData.CLOUGH);
			break;
		case D_IEHP_DEGR:
			DEGRAD.Convert(rData.DEGRAD);
			break;
		case D_IEHP_TAKE: case D_IEHP_MTAK: case D_IEHP_ASYM:
		case D_IEHP_NELA: case D_IEHP_TAKS:
			TAKEDA.Convert(rData.TAKEDA);
			break;
		case D_IEHP_TTET: case D_IEHP_MTTE: case D_IEHP_ORGT:
			TAKTET.Convert(rData.TAKTET);
			break;
		case D_IEHP_NBIL:
			NORBIL.Convert(rData.NORBIL);
			break;
		case D_IEHP_EBIL:
			ELABIL.Convert(rData.ELABIL);
			break;
		case D_IEHP_ETRI:
			ELATRI.Convert(rData.ELATRI);
			break;
		case D_IEHP_ETET:
			ELATET.Convert(rData.ELATET);
			break;
		case D_IEHP_SLPB: case D_IEHP_SLBT: case D_IEHP_SLBC:
		case D_IEHP_SLPT: case D_IEHP_SLTT: case D_IEHP_SLTC:
			SLIP.Convert(rData.SLIP);
			break;
		case D_IEHP_MLEL:	case D_IEHP_MLKI:
		case D_IEHP_MLTA:	case D_IEHP_MLPI:
			memcpy(&MULTLIN, &rData.MULTLIN, sizeof(T_IEHP_MLIN));
			break;
		case D_IEHP_RO: case D_IEHP_HD:
			SLIP.Convert(rData.SLIP);
			break;
		case D_IEHP_AXIAL:
			AXIAL.Convert(rData.AXIAL);
			break;
		case D_IEHP_BUCK:
			BUCKL.Convert(rData.BUCKL);
			break;
		case D_IEHP_FBIL: case D_IEHP_FTRI: case D_IEHP_FINF:
			FEMA.Convert(rData.FEMA);
			break;
		case D_IEHP_DMPR:
			DMPRBR.Convert(rData.DMPRBR);
			break;
		case D_IEHP_LRBB:
			LRBBIL.Convert(rData.LRBBIL);
			break;
		case D_IEHP_LRBT:
			LRBTRI.Convert(rData.LRBTRI);
			break;
		}
	}
};

struct T_IEHP_PMMULT_AXIAL_882R1
{
	double dAxialForc;
	double dDisp[5];
	double dForc[5];
	double dInitStiff;

	void Initialize(double dAxial)
	{
		dAxialForc = dAxial;
		dDisp[0] = -0.5;
		dDisp[1] = -0.1;
		dDisp[2] = 0.;
		dDisp[3] = 0.1;
		dDisp[4] = 0.5;
		dForc[0] = -1001;
		dForc[1] = -1000;
		dForc[2] = 0.;
		dForc[3] = 1000;
		dForc[4] = 1001;
		dInitStiff = 10000.0;
	}
};
#define T_IEHP_PMMULT_AXIAL_882R1_DT CArray<T_IEHP_PMMULT_AXIAL_882R1, T_IEHP_PMMULT_AXIAL_882R1&>

struct T_IEHP_PMMULT_AXIAL_FORC
{
	double dDisp;
	double dForc;
	void Initialize()
	{
		dDisp = 0.0;
		dForc = 0.0;
	}
};
#define T_IEHP_PMMULT_AXIAL_FORC_DT CArray<T_IEHP_PMMULT_AXIAL_FORC, T_IEHP_PMMULT_AXIAL_FORC&>

struct T_IEHP_PMMULT_AXIAL_UNIT
{
	T_IEHP_PMMULT_AXIAL_UNIT()
	{
		dAxialForc = D_UNITSYS_BASE_FORCE;
		dDisp      = D_UNITSYS_BASE_RADIAN_L;  // rad/m
		dForc      = D_UNITSYS_BASE_MOMENT;    // N.m
		dInitStiff = D_UNITSYS_BASE_DISTSTIFF; // N.m^2 
	}
	int dAxialForc;
	int dDisp;
	int dForc;
	int dInitStiff;
};

struct T_IEHP_PMMULT_AXIAL
{
	double dAxialForc;
	double dDispNotUse[5];
	double dForcNotUse[5];
	double dInitStiffPlus;
	double dInitStiffMinus;
	T_IEHP_PMMULT_AXIAL_FORC_DT aDispForc;

	void Initialize(double dAxial)
	{
		dAxialForc = dAxial;
		
		for (int i = 0; i < 5; i++)
		{
			dDispNotUse[i] = 0.0;
			dForcNotUse[i] = 0.0;
		}
		dInitStiffPlus  = 10000.0;
		dInitStiffMinus = 10000.0;

		aDispForc.SetSize(5);
		aDispForc[0].dDisp = -0.5;
		aDispForc[1].dDisp = -0.1;
		aDispForc[2].dDisp = 0.;
		aDispForc[3].dDisp = 0.1;
		aDispForc[4].dDisp = 0.5;

		aDispForc[0].dForc = -1001;
		aDispForc[1].dForc = -1000;
		aDispForc[2].dForc = 0.;
		aDispForc[3].dForc = 1000;
		aDispForc[4].dForc = 1001;
	}
	T_IEHP_PMMULT_AXIAL() {Initialize(0.0);}
	T_IEHP_PMMULT_AXIAL(T_IEHP_PMMULT_AXIAL& src) {*this = src;}
	T_IEHP_PMMULT_AXIAL& operator=(const T_IEHP_PMMULT_AXIAL& src)
	{
		dAxialForc  = src.dAxialForc;
		for (int i = 0; i < 5; i++)
		{
			dDispNotUse[i] = src.dDispNotUse[i];
			dForcNotUse[i] = src.dForcNotUse[i];
		}
		dInitStiffPlus = src.dInitStiffPlus;
		dInitStiffMinus = src.dInitStiffMinus;
		aDispForc.RemoveAll();
		aDispForc.Copy(src.aDispForc);
		return *this;
	}
	void Convert(const T_IEHP_PMMULT_AXIAL_882R1& rData)
	{
		dAxialForc = rData.dAxialForc;
		for (int i = 0; i < 5; i++)
		{
			dDispNotUse[i] = 0.0;
			dForcNotUse[i] = 0.0;
		}
		dInitStiffPlus = rData.dInitStiff;
		dInitStiffMinus = rData.dInitStiff;

		aDispForc.SetSize(5);
		aDispForc[0].dDisp = rData.dDisp[0];
		aDispForc[1].dDisp = rData.dDisp[1];
		aDispForc[2].dDisp = rData.dDisp[2];
		aDispForc[3].dDisp = rData.dDisp[3];
		aDispForc[4].dDisp = rData.dDisp[4];

		aDispForc[0].dForc = rData.dForc[0];
		aDispForc[1].dForc = rData.dForc[1];
		aDispForc[2].dForc = rData.dForc[2];
		aDispForc[3].dForc = rData.dForc[3];
		aDispForc[4].dForc = rData.dForc[4];
	}

};
#define T_IEHP_PMMULT_AXIAL_DT CArray<T_IEHP_PMMULT_AXIAL, T_IEHP_PMMULT_AXIAL&>

struct T_IEHP_PMMULT_AXIAL_RW
{
	double dAxialForc;
	double dDispNotUse[5];
	double dForcNotUse[5];
	double dInitStiffPlus;
	double dInitStiffMinus;
	int nDispForcCnt;
	void GetIehp(T_IEHP_PMMULT_AXIAL& rData)
	{
		rData.dAxialForc = dAxialForc;
		memcpy(rData.dDispNotUse, dDispNotUse, sizeof(dDispNotUse));    
		memcpy(rData.dForcNotUse, dForcNotUse, sizeof(dForcNotUse));    
		rData.dInitStiffPlus = dInitStiffPlus;
		rData.dInitStiffMinus = dInitStiffMinus;
		// nDispForcCnt
	}
	void SetIehp(const T_IEHP_PMMULT_AXIAL& rData)
	{
		dAxialForc = rData.dAxialForc;
		memcpy(dDispNotUse, rData.dDispNotUse, sizeof(dDispNotUse));    
		memcpy(dForcNotUse, rData.dForcNotUse, sizeof(dForcNotUse));    
		dInitStiffPlus =  rData.dInitStiffPlus;
		dInitStiffMinus = rData.dInitStiffMinus;
		// nDispForcCnt
	}
};

struct T_IEHP_PMMULT_RW_882R1
{  
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric
	BOOL bMultiCurve;
	int nAxial;

	int nDeformDefineType;
	double dDeformCapacity[2][5];

	int nStnHdType; // 0: Isotropic, 1:Kinematic, 2:Mixed
	double dBeta;
};

struct T_IEHP_PMMULT
{  
	int nSymmetryType;          // 0:Symmetric, 1:Asymmetric
	BOOL bMultiCurve;
	T_IEHP_PMMULT_AXIAL_DT aAxial;
		
	int nDeformDefineType;
	double dDeformCapacity[2][5];

	int nStnHdType; // 0: Isotropic, 1:Kinematic, 2:Mixed
	double dBeta;

	void Initialize()
	{
		nSymmetryType = 1;
		bMultiCurve = TRUE;
		aAxial.SetSize(2);
		aAxial[0].Initialize(0.0);
		aAxial[1].Initialize(-100);

		nDeformDefineType = 1;
		dDeformCapacity[0][0] = 0.5;
		dDeformCapacity[0][1] = 1.0;
		dDeformCapacity[0][2] = 2.0;
		dDeformCapacity[0][3] = 4.0;
		dDeformCapacity[0][4] = 8.0;
		dDeformCapacity[1][0] = 0.5;
		dDeformCapacity[1][1] = 1.0;
		dDeformCapacity[1][2] = 2.0;
		dDeformCapacity[1][3] = 4.0;
		dDeformCapacity[1][4] = 8.0;
		nStnHdType = 0;
		dBeta = 0.0;
	}
	T_IEHP_PMMULT() {Initialize();}
	T_IEHP_PMMULT(T_IEHP_PMMULT& src) {*this = src;}
	T_IEHP_PMMULT& operator=(T_IEHP_PMMULT& src)
	{
		nSymmetryType  = src.nSymmetryType;
		bMultiCurve  = src.bMultiCurve;
		aAxial.Copy(src.aAxial);
		nDeformDefineType = src.nDeformDefineType;
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				dDeformCapacity[i][j] = src.dDeformCapacity[i][j];
			}
		}
		nStnHdType = src.nStnHdType;
		dBeta = src.dBeta;
		return *this;
	}
};

struct T_IEHP_PMMULT_CNT_RW // 가변Size 배열의 가변Size 배열 파일 입출력 처리를 위한 중간 구조체
{
	CArray<int, int> aAxial_aDispCnt;
	void Get(T_IEHP_PMMULT& rData)
	{
		aAxial_aDispCnt.SetSize(rData.aAxial.GetSize());
		for (int i = 0; i < aAxial_aDispCnt.GetSize(); i++)
		{
			aAxial_aDispCnt[i] = static_cast<int>(rData.aAxial[i].aDispForc.GetSize());
		}
	}
};

struct T_IEHP_PMMULT_RW
{  
	int nSymmetryType;
	BOOL bMultiCurve;
	int nAxialCount;

	int nDeformDefineType;
	double dDeformCapacity[2][5];
	
	int nStnHdType; // 0: Isotropic, 1:Kinematic, 2:Mixed
	double dBeta;

	void Initialize()
	{
		nSymmetryType = 0;
		bMultiCurve = TRUE;
		nAxialCount = 0;
		nDeformDefineType = 1;
		dDeformCapacity[0][0] = 0.5;
		dDeformCapacity[0][1] = 1.0;
		dDeformCapacity[0][2] = 2.0;
		dDeformCapacity[0][3] = 4.0;
		dDeformCapacity[0][4] = 8.0;
		dDeformCapacity[1][0] = 0.5;
		dDeformCapacity[1][1] = 1.0;
		dDeformCapacity[1][2] = 2.0;
		dDeformCapacity[1][3] = 4.0;
		dDeformCapacity[1][4] = 8.0;
		nStnHdType = 0;
		dBeta = 0.0;
	}
	void GetIehpPmmult(T_IEHP_PMMULT& rData)
	{
		rData.nSymmetryType = nSymmetryType;
		rData.bMultiCurve = bMultiCurve;
		//memcpy(rData.aAxial, aAxial, sizeof(aAxial));    
		rData.nDeformDefineType = nDeformDefineType;
		memcpy(rData.dDeformCapacity, dDeformCapacity, sizeof(dDeformCapacity));  
		rData.nStnHdType = nStnHdType;
		rData.dBeta = dBeta;
	}
	void SetIehpPmmult(const T_IEHP_PMMULT& rData)
	{
		nSymmetryType = rData.nSymmetryType;
		bMultiCurve = rData.bMultiCurve;
		//memcpy(aAxial, rData.aAxial, sizeof(aAxial));
		nDeformDefineType = rData.nDeformDefineType;
		memcpy(dDeformCapacity, rData.dDeformCapacity, sizeof(dDeformCapacity));
		nStnHdType = rData.nStnHdType;
		dBeta = rData.dBeta;
	}
};

struct T_IEHP_ALL_PROP_955
{
	T_IEHP_PROP_955 PROP[7]; // 고정SizeDB 인 경우 여기 추가해 주세요.

	// 가변 Size DB 인 경우 아래에 추가해 주세요
	T_IEHP_PMMULT PMMULT[7];  // P-M Multi-Curve Type
};
struct T_IEHP_ALL_PROP
{
	T_IEHP_PROP PROP[7]; // 고정SizeDB 인 경우 여기 추가해 주세요.
	
	// 가변 Size DB 인 경우 아래에 추가해 주세요
	T_IEHP_PMMULT PMMULT[7];  // P-M Multi-Curve Type

	void Initialize()
	{
		for (int i = 0; i < 7; i++)
		{
			PROP[i].YLDSUR.Initialize(); // Union 이여서 제일 큰것만 초기화 함
			PMMULT[i].Initialize();
		}
	}
	T_IEHP_ALL_PROP& operator=(T_IEHP_ALL_PROP& src)
	{
		for (int i = 0; i < 7; i++)
		{
			PROP[i] = src.PROP[i];
			PMMULT[i] = src.PMMULT[i];
		}
		
		return *this;
	}
};
struct T_IEHP_ALL_PROP_RW_955
{
	T_IEHP_PROP_955 PROP[7];
	T_IEHP_PMMULT_RW PMMULT[7];

	void GetIehpAllProp955(T_IEHP_ALL_PROP& rData, int nHysModel[9])
	{
		for (int i = 0; i < 7; i++)
		{
			if (i < 6)
			{
				rData.PROP[i].Convert955(PROP[i], nHysModel[i]);
			}
			else
			{
				rData.PROP[i].YLDSUR.Convert955(PROP[i].YLDSUR);
			}			
		}
		
		for (int i = 0; i < 7; i++)
		{
			PMMULT[i].GetIehpPmmult(rData.PMMULT[i]);
		}
	}
};
struct T_IEHP_ALL_PROP_RW
{
	T_IEHP_PROP PROP[7];
	T_IEHP_PMMULT_RW PMMULT[7];

	void GetIehpAllProp(T_IEHP_ALL_PROP& rData)
	{
		memcpy(rData.PROP, PROP, sizeof(PROP));
		for (int i = 0; i < 7; i++)
		{
			PMMULT[i].GetIehpPmmult(rData.PMMULT[i]);
		}
	}
	void SetIehpAllProp(const T_IEHP_ALL_PROP& rData)
	{
		memcpy(PROP, rData.PROP, sizeof(PROP));
		for (int i = 0; i < 7; i++)
		{
			PMMULT[i].SetIehpPmmult(rData.PMMULT[i]);
		}
	}
};

struct T_IEHP_D_640
{  
	CString strName;
	CString strDescription;
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=Filled type SRC, 3=Encased type SRC, 4=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Fx, Fy, Fz, Mx, My, Mz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[6];       // HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_PROP_650 PROP[7];    
	int     nSeq;
};
struct T_IEHP_D_650
{  
	CString strName;
	CString strDescription;
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=Filled type SRC, 3=Encased type SRC, 4=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
															// HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_PROP PROP[7];    
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
};
struct T_IEHP_D_692
{  
	CString strName;
	CString strDescription;
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=Filled type SRC, 3=Encased type SRC, 4=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가), 3=Truss(6.9.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
															// HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_PROP_692 PROP[7];    
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수
};
struct T_IEHP_D_791
{  
	CString strName;
	CString strDescription;
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=Filled type SRC, 3=Encased type SRC, 4=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가), 3=Truss(6.9.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_PROP_815 PROP[7];    
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수
};
struct T_IEHP_D_815
{  
	CString strName;
	CString strDescription;
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=Filled type SRC, 3=Encased type SRC, 4=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가), 3=Truss(6.9.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_PROP_815 PROP[7];    
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수
	
	T_IEHP_MULT MultiData[6];
};
struct T_IEHP_D_860
{  
	CString strName;
	CString strDescription;
	int     nMaterialType;     // 0=RC/SRC, 1=Steel/SRC
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;            // nHingeType==4(Wall) 일 때는 Section Key가 아니고 Thick Key 임.. // MNET:XXXX-JWKWON-20130712
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가), 3=Truss(6.9.0추가), 4=Wall(CRB)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX

	T_IEHP_PROP_955 PROP[7];				// I only, J only, Center, 즉 1개 의 정보 = 여기 저장
	T_IEHP_PROP_955 SubPROP[7];			// I&J가 필요할 경우, 즉 2개의 정보 = 여기도 저장
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수
	int			nWallType;
	BOOL		bIJExistYield;			// Yield Dlg에서 IJ 존재 유무
	BOOL		bExistIJProp[7];	  // Prop  Dlg에서 IJ 존재 유무
	T_IEHP_MULT MultiData[6];			
};
struct T_IEHP_D_881
{  
	CString strName;
	CString strDescription;
	int     nMaterialType;     // 0=RC/SRC, 1=Steel/SRC
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;            // nHingeType==4(Wall) 일 때는 Section Key가 아니고 Thick Key 임.. // MNET:XXXX-JWKWON-20130712
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // D_IEHP_HTYP. 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가), 3=Truss(6.9.0추가), 4=Wall(CRB)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX

	T_IEHP_PROP_955 PROP[7];				// I only, J only, Center, 즉 1개 의 정보 = 여기 저장
	T_IEHP_PROP_955 SubPROP[7];			// I&J가 필요할 경우, 즉 2개의 정보 = 여기도 저장
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // If nHingeType==D_IEHP_HTYP_WALL then FibwKey
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수
	int			nWallType;
	BOOL		bIJExistYield;			// Yield Dlg에서 IJ 존재 유무
	BOOL		bExistIJProp[7];	  // Prop  Dlg에서 IJ 존재 유무
	int     nFiberAutoUser;    // 0:Use option of IEHC, 1:User Defined
	int     nUseIehcShearR;    // 0:Use option of IEHC, 1:User Defined
	double  dShearR;           // Shear Spring Location : r
	int     nUseIehcConsOutpl; // 0:Use option of IEHC, 1:User Defined
	BOOL    bConsOutpl;
	int     nUseIehcLocation; 

	T_IEHP_MULT MultiData[6];			
};

struct T_IEHP_D_970
{
	CString strName;
	CString strDescription;
	int     nMaterialType;     // 0=RC/SRC, 1=Steel/SRC
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;            // nHingeType==4(Wall) 일 때는 Section Key가 아니고 Thick Key 임.. // MNET:XXXX-JWKWON-20130712
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // D_IEHP_HTYP. 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가), 3=Truss(6.9.0추가), 4=Wall(CRB),  5=Point Spring Support
	int     nInteractionType;   // D_IEHP_INTER_TYPE  : 0=None, 1=P-M, 2=P-M-M, 3=P-M Multi-Curve 
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX

	T_IEHP_ALL_PROP AllProp;				// I only, J only, Center, 즉 1개 의 정보 = 여기 저장
	T_IEHP_ALL_PROP AllSubProp;			// I&J가 필요할 경우, 즉 2개의 정보 = 여기도 저장
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // If nHingeType==D_IEHP_HTYP_WALL then FibwKey
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수
	int			nWallType;
	BOOL		bIJExistYield;			// Yield Dlg에서 IJ 존재 유무
	BOOL		bExistIJProp[7];	  // Prop  Dlg에서 IJ 존재 유무
	int     nFiberAutoUser;    // 0:Use option of IEHC, 1:User Defined
	int     nUseIehcShearR;    // 0:Use option of IEHC, 1:User Defined
	double  dShearR;           // Shear Spring Location : r
	int     nUseIehcConsOutpl; // 0:Use option of IEHC, 1:User Defined
	BOOL    bConsOutpl;
	int     nUseIehcLocation;

	T_IEHP_MULT MultiData[6];
};
///// ---------------  Multi Linear Type Data   -----------------------------------------------------------
enum D_IEHP_HTYP
{
	D_IEHP_HTYP_BEAM_LUMP,
	D_IEHP_HTYP_BEAM_DIST,
	D_IEHP_HTYP_SPRING, // General Link 
	D_IEHP_HTYP_TRUSS,
	D_IEHP_HTYP_WALL,
	D_IEHP_HTYP_POINTSPRING, // Point Spring Support
	D_IEHP_HTYP_MAX
};

enum D_IEHP_INTER_TYPE
{
	D_IEHP_INTER_TYPE_NONE,
	D_IEHP_INTER_TYPE_PM,             // PM  (P-M in Strength Calculation)  
	D_IEHP_INTER_TYPE_PMM,            // PMM (P-M in Status Determination)
	D_IEHP_INTER_TYPE_PM_MULTI_CURVE, // Multi Curve
	D_IEHP_INTER_TYPE_MAX
};

#define D_IEHP_SECTION_NUM_MAX 20  /// nSectionNum의 최대값 제한.
struct T_IEHP_D
{  
	CString strName;
	CString strDescription;
	int     nMaterialType;     // 0=RC/SRC, 1=Steel/SRC
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;            // nHingeType==4(Wall) 일 때는 Section Key가 아니고 Thick Key 임.. // MNET:XXXX-JWKWON-20130712
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // D_IEHP_HTYP. 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가), 3=Truss(6.9.0추가), 4=Wall(CRB),  5=Point Spring Support
	int     nInteractionType;   // D_IEHP_INTER_TYPE  : 0=None, 1=P-M, 2=P-M-M, 3=P-M Multi-Curve 
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
															// HYSTERESIS MODEL : D_IEHP_XXXX
	
	T_IEHP_ALL_PROP AllProp;				// I only, J only, Center, 즉 1개 의 정보 = 여기 저장
	T_IEHP_ALL_PROP AllSubProp;			// I&J가 필요할 경우, 즉 2개의 정보 = 여기도 저장
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // If nHingeType==D_IEHP_HTYP_WALL then FibwKey
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수
	int			nWallType;
	BOOL		bIJExistYield;			// Yield Dlg에서 IJ 존재 유무
	BOOL		bExistIJProp[7];	  // Prop  Dlg에서 IJ 존재 유무
	int     nFiberAutoUser;    // 0:Use option of IEHC, 1:User Defined
	int     nUseIehcShearR;    // 0:Use option of IEHC, 1:User Defined
	double  dShearR;           // Shear Spring Location : r
	int     nUseIehcConsOutpl; // 0:Use option of IEHC, 1:User Defined
	BOOL    bConsOutpl;
	int     nUseIehcLocation; 

	int     nFlexSitf;         // 0:Flexibility Method, 1:Stiffness Method
	int     nStifLoc[6];       // Fx, Fy, Fz, Mx, My, Mz Stiffness Location 0:Iend, 1:Jend, 2:Center
	BOOL    bUseNonlinearShearY; // Elastic or Nonlinear effect
	BOOL    bUseNonlinearShearZ; // Elastic or Nonlinear effect

	T_IEHP_MULT MultiData[6];			

	BOOL IsSectionalLocation()
	{
		return (nHingeType == 1 || (nHingeType == 4 && nDefinition == 1));
	}
	void Initialize()
	{
		strName.Empty();
		strDescription.Empty();
		bIJExistYield = FALSE;
		nMaterialType = 0;
		nWallType = 1;
		nCodeStl = -1;
		nCodeConc = -1;
		MatlKey = -1;
		SectKey = -1;
		nMemberType = -1;
		nUserAutoOpt = -1;

		nLocation = 0;
		nHingeType = 0;
		nInteractionType = 0;

		for(int i = 0; i < 8; i++) cDir[i] = '0';
		for(int i = 0; i < 6; i++) 
		{
			nSectionNum[i] = 3;
			nHingeLocation[i] = 2;
		}    
		for(int i = 0; i < 9; i++) nHysModel[i] = 0;

		AllProp.Initialize();    // union이므로 가장 큰 사이즈만 초기화
		AllSubProp.Initialize();

		for(int i = 0; i < 7; i++)  	
		{			
			bExistIJProp[i] = FALSE;
		}
		nSeq = 0;
		nDefinition = 0;
		FibrKey = 0;
		bAddData4D2E = FALSE;
		nTempVar = 1;
		nFiberAutoUser    = 1;
		nUseIehcShearR    = 0;
		dShearR           = 0.4;
		nUseIehcConsOutpl = 0;
		bConsOutpl        = FALSE;
		nUseIehcLocation  = 0;

		nFlexSitf = 0;
		for (int i = 0; i < 6; ++i)
		{
			nStifLoc[i] = 0;
		}
		bUseNonlinearShearY = TRUE;
		bUseNonlinearShearZ = TRUE;

		for(int i = 0; i < 6; i++)  MultiData[i].Initialize();            
	}
	T_IEHP_D()
	{
	}
	T_IEHP_D(T_IEHP_D& src)
	{
		*this = src;
	}
	T_IEHP_D& operator=(T_IEHP_D& src)
	{
		strName        = src.strName;
		strDescription = src.strDescription;
		nMaterialType  = src.nMaterialType;     
		nCodeStl       = src.nCodeStl;          
		nCodeConc      = src.nCodeConc;        
		MatlKey        = src.MatlKey;
		SectKey        = src.SectKey;  
		nMemberType    = src.nMemberType;    
		nLocation      = src.nLocation;        
		nHingeType     = src.nHingeType;         
		nInteractionType = src.nInteractionType;
		nWallType			 = src.nWallType;
		bIJExistYield = src.bIJExistYield;
		nSeq         = src.nSeq;
		nDefinition  = src.nDefinition;        
		FibrKey      = src.FibrKey;            
		nUserAutoOpt = src.nUserAutoOpt;       
		bAddData4D2E = src.bAddData4D2E;       
		nTempVar     = src.nTempVar;     

		for(int i = 0; i<8; i++) 
			cDir[i]						= src.cDir[i];

		for(int i = 0; i<6; i++) 
		{
			nSectionNum[i]			= src.nSectionNum[i]; 
			nHingeLocation[i]	= src.nHingeLocation[i]; 
			MultiData[i]			= src.MultiData[i];
		} 

		for(int i = 0; i<9; i++) 
			nHysModel[i]				= src.nHysModel[i];       

		AllProp						= src.AllProp;
		AllSubProp				= src.AllSubProp;

		for(int i = 0; i<7; i++) 
		{ 
			bExistIJProp[i]		= src.bExistIJProp[i];
		}

		nFiberAutoUser    = src.nFiberAutoUser   ;
		nUseIehcShearR    = src.nUseIehcShearR   ;
		dShearR           = src.dShearR          ;
		nUseIehcConsOutpl = src.nUseIehcConsOutpl;
		bConsOutpl        = src.bConsOutpl       ;
		nUseIehcLocation  = src.nUseIehcLocation ;

		nFlexSitf = src.nFlexSitf;
		for (int i = 0; i < 6; ++i)
		{
			nStifLoc[i] = src.nStifLoc[i];
		}
		bUseNonlinearShearY = src.bUseNonlinearShearY;
		bUseNonlinearShearZ = src.bUseNonlinearShearZ;

		return *this;
	}
};
struct T_IEHP_D_CH_640
{
	char    strName[20];
	char    strDescription[20]; 
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[6];       // HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_PROP_650 PROP[7];  
	int     nSeq;
};
struct T_IEHP_D_CH_650
{
	char    strName[20];
	char    strDescription[20]; 
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
															// HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_PROP_650 PROP[7];  
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가  
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
};
struct T_IEHP_D_CH_692
{
	char    strName[20];
	char    strDescription[20]; 
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
															// HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_PROP_692 PROP[7];  
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가  
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수
};
struct T_IEHP_D_CH_791
{
	char    strName[20];
	char    strDescription[20]; 
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_PROP_815 PROP[7];  
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가  
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수
};
struct T_IEHP_D_CH_815
{
	char    strName[20];
	char    strDescription[20]; 
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_PROP_815 PROP[7];  
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가  
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수
	
	T_IEHP_MULT  MultiData[6];  // 7.9.5추가 // MNET:4337-JWKWON-20110921
};
struct T_IEHP_D_CH_860
{
	char    strName[20];
	char    strDescription[20]; 
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_PROP_955 PROP[7];
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가  
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수

	T_IEHP_MULT  MultiData[6];  // 7.9.5추가 // MNET:4337-JWKWON-20110921

	T_IEHP_PROP_955 SubPROP[7];			// I&J가 필요할 경우, 즉 2개의 정보 = 여기도 저장
	int			nWallType;
	BOOL		bIJExistYield;			// Yield Dlg에서 IJ 존재 유무
	BOOL		bExistIJProp[7];	  // Prop  Dlg에서 IJ 존재 유무
};

struct T_IEHP_D_CH_881
{
	char    strName[20];
	char    strDescription[20];
	int     nMaterialType;     // 0=RC/SRC, 1=Steel/SRC
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;            // nHingeType==4(Wall) 일 때는 Section Key가 아니고 Thick Key 임.. // MNET:XXXX-JWKWON-20130712
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // D_IEHP_HTYP. 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가), 3=Truss(6.9.0추가), 4=Wall(CRB)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX

	T_IEHP_PROP_955 PROP[7];				// I only, J only, Center, 즉 1개 의 정보 = 여기 저장
	T_IEHP_PROP_955 SubPROP[7];			// I&J가 필요할 경우, 즉 2개의 정보 = 여기도 저장
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // If nHingeType==D_IEHP_HTYP_WALL then FibwKey
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수
	int			nWallType;
	BOOL		bIJExistYield;			// Yield Dlg에서 IJ 존재 유무
	BOOL		bExistIJProp[7];	  // Prop  Dlg에서 IJ 존재 유무
	int     nFiberAutoUser;    // 0:Use option of IEHC, 1:User Defined
	int     nUseIehcShearR;    // 0:Use option of IEHC, 1:User Defined
	double  dShearR;           // Shear Spring Location : r
	int     nUseIehcConsOutpl; // 0:Use option of IEHC, 1:User Defined
	BOOL    bConsOutpl;
	int     nUseIehcLocation; 

	T_IEHP_MULT MultiData[6];
};

struct T_IEHP_D_CH_970
{
	char    strName[20];
	char    strDescription[20];
	int     nMaterialType;     // 0=RC/SRC, 1=Steel/SRC
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;            // nHingeType==4(Wall) 일 때는 Section Key가 아니고 Thick Key 임.. // MNET:XXXX-JWKWON-20130712
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // D_IEHP_HTYP. 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가), 3=Truss(6.9.0추가), 4=Wall(CRB)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX

	T_IEHP_ALL_PROP AllProp;			// I only, J only, Center, 즉 1개 의 정보 = 여기 저장
	T_IEHP_ALL_PROP AllSubProp;			// I&J가 필요할 경우, 즉 2개의 정보 = 여기도 저장
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // If nHingeType==D_IEHP_HTYP_WALL then FibwKey
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수
	int			nWallType;
	BOOL		bIJExistYield;			// Yield Dlg에서 IJ 존재 유무
	BOOL		bExistIJProp[7];	  // Prop  Dlg에서 IJ 존재 유무
	int     nFiberAutoUser;    // 0:Use option of IEHC, 1:User Defined
	int     nUseIehcShearR;    // 0:Use option of IEHC, 1:User Defined
	double  dShearR;           // Shear Spring Location : r
	int     nUseIehcConsOutpl; // 0:Use option of IEHC, 1:User Defined
	BOOL    bConsOutpl;
	int     nUseIehcLocation;

	T_IEHP_MULT MultiData[6];
};

struct T_IEHP_D_CH
{
	char    strName[20];
	char    strDescription[20];
	int     nMaterialType;     // 0=RC/SRC, 1=Steel/SRC
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;            // nHingeType==4(Wall) 일 때는 Section Key가 아니고 Thick Key 임.. // MNET:XXXX-JWKWON-20130712
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // D_IEHP_HTYP. 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가), 3=Truss(6.9.0추가), 4=Wall(CRB)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX

	T_IEHP_ALL_PROP AllProp;			// I only, J only, Center, 즉 1개 의 정보 = 여기 저장
	T_IEHP_ALL_PROP AllSubProp;			// I&J가 필요할 경우, 즉 2개의 정보 = 여기도 저장
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // If nHingeType==D_IEHP_HTYP_WALL then FibwKey
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수
	int			nWallType;
	BOOL		bIJExistYield;			// Yield Dlg에서 IJ 존재 유무
	BOOL		bExistIJProp[7];	  // Prop  Dlg에서 IJ 존재 유무
	int     nFiberAutoUser;    // 0:Use option of IEHC, 1:User Defined
	int     nUseIehcShearR;    // 0:Use option of IEHC, 1:User Defined
	double  dShearR;           // Shear Spring Location : r
	int     nUseIehcConsOutpl; // 0:Use option of IEHC, 1:User Defined
	BOOL    bConsOutpl;
	int     nUseIehcLocation; 

	int     nFlexSitf;         // 0:Flexibility Method, 1:Stiffness Method
	int     nStifLoc[6];       // Fx, Fy, Fz, Mx, My, Mz Stiffness Location 0:Iend, 1:Jend, 2:Center
	BOOL    bUseNonlinearShearY; // Elastic or Nonlinear effect
	BOOL    bUseNonlinearShearZ; // Elastic or Nonlinear effect

	T_IEHP_MULT MultiData[6];

	void ConvertToString(T_IEHP_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDescription, rData.strDescription, sizeof(strDescription));
		rData.nMaterialType = nMaterialType;
		rData.nCodeStl = nCodeStl;
		rData.nCodeConc = nCodeConc;
		rData.MatlKey = MatlKey;
		rData.SectKey = SectKey;
		rData.nMemberType = nMemberType;
		rData.nLocation = nLocation;
		rData.nHingeType = nHingeType;
		rData.nInteractionType = nInteractionType;
		memcpy(rData.cDir, cDir, sizeof(cDir));
		memcpy(rData.nSectionNum, nSectionNum, sizeof(nSectionNum));
		memcpy(rData.nHingeLocation, nHingeLocation, sizeof(nHingeLocation));
		memcpy(rData.nHysModel, nHysModel, sizeof(nHysModel));
		rData.AllProp = AllProp;
		rData.AllSubProp = AllSubProp;
		rData.nSeq = nSeq;
		rData.nDefinition = nDefinition;
		rData.FibrKey = FibrKey; 
		rData.nUserAutoOpt = nUserAutoOpt;
		rData.bAddData4D2E = bAddData4D2E;
		rData.nTempVar = nTempVar;
		rData.nWallType = nWallType;
		rData.bIJExistYield = bIJExistYield;
		memcpy(rData.bExistIJProp, bExistIJProp, sizeof(bExistIJProp));    
		rData.nFiberAutoUser    = nFiberAutoUser   ;
		rData.nUseIehcShearR    = nUseIehcShearR   ;
		rData.dShearR           = dShearR          ;
		rData.nUseIehcConsOutpl = nUseIehcConsOutpl;
		rData.bConsOutpl        = bConsOutpl       ;
		rData.nUseIehcLocation  = nUseIehcLocation ;		
		for (int i=0; i<6; i++)
		{
			 rData.MultiData[i].arMultiLinearData.Copy(MultiData[i].arMultiLinearData);
		}

		rData.nFlexSitf = nFlexSitf;
		for (int i = 0; i < 6; ++i)
		{
			rData.nStifLoc[i] = nStifLoc[i];
		}
		rData.bUseNonlinearShearY	= bUseNonlinearShearY;
		rData.bUseNonlinearShearZ = bUseNonlinearShearZ;
	}
	void ConvertToChar(T_IEHP_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDescription, strDescription, sizeof(strDescription));
		nMaterialType = rData.nMaterialType;
		nCodeStl = rData.nCodeStl;
		nCodeConc = rData.nCodeConc;
		MatlKey = rData.MatlKey;
		SectKey = rData.SectKey;
		nMemberType = rData.nMemberType;
		nLocation = rData.nLocation;    
		nHingeType = rData.nHingeType;
		nInteractionType = rData.nInteractionType;
		memcpy(cDir, rData.cDir, sizeof(cDir));
		memcpy(nSectionNum, rData.nSectionNum, sizeof(nSectionNum));
		memcpy(nHingeLocation, rData.nHingeLocation, sizeof(nHingeLocation));
		memcpy(nHysModel, rData.nHysModel, sizeof(nHysModel));    
		AllProp = rData.AllProp;
		AllSubProp = rData.AllSubProp;
		nSeq=rData.nSeq;    
		nDefinition = rData.nDefinition;
		FibrKey = rData.FibrKey;    
		nUserAutoOpt = rData.nUserAutoOpt;
		bAddData4D2E = rData.bAddData4D2E;
		nTempVar = rData.nTempVar;
		nWallType = rData.nWallType;
		bIJExistYield = rData.bIJExistYield;
		memcpy(bExistIJProp, rData.bExistIJProp , sizeof(bExistIJProp));    
		nFiberAutoUser    = rData.nFiberAutoUser   ;
		nUseIehcShearR    = rData.nUseIehcShearR   ;
		dShearR           = rData.dShearR          ;
		nUseIehcConsOutpl = rData.nUseIehcConsOutpl;
		bConsOutpl        = rData.bConsOutpl       ;
		nUseIehcLocation  = rData.nUseIehcLocation ;
		for (int i=0; i<6; i++)
		{
			MultiData[i].arMultiLinearData.Copy(rData.MultiData[i].arMultiLinearData);
		}    

		nFlexSitf						= rData.nFlexSitf;
		for (int i = 0; i < 6; ++i)
		{
			nStifLoc[i] = rData.nStifLoc[i];
		}
		bUseNonlinearShearY	= rData.bUseNonlinearShearY;
		bUseNonlinearShearZ = rData.bUseNonlinearShearZ;
	}
	int ConvertMaterialTypeOld2Renewal(int nMaterialType_old)   // 재질,단면을 물고 있던 방식 -> Pushover 방식으로 변경하면서 바뀐 material Index 변환
	{
		int nMaterialType_renewal = 0;
		switch(nMaterialType_old)
		{
		case 0:  nMaterialType_renewal = 1;  break;   // Steel        -> Steel/SRC(filled)
		case 1:  nMaterialType_renewal = 0;  break;   // RC           -> RC/SRC(encased)
		case 2:  nMaterialType_renewal = 1;  break;   // SRC(filled)  -> Steel/SRC(filled)
		case 3:  nMaterialType_renewal = 0;  break;   // SRC(encased) -> RC/SRC(encased)
		case 4:  nMaterialType_renewal = 0;  break;   // User Defined -> RC/SRC(encased)
		default:  ASSERT(0);  nMaterialType_renewal = 0;  break;
		}

		return nMaterialType_renewal;
	}
	void Convert640(T_IEHP_D_CH_640& rData)
	{    
		memcpy(strName, rData.strName, sizeof(strName));    
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));    
		nMaterialType = ConvertMaterialTypeOld2Renewal(rData.nMaterialType);    //nMaterialType = rData.nMaterialType;
		nCodeStl = rData.nCodeStl;
		nCodeConc = rData.nCodeConc;
		MatlKey = rData.MatlKey;
		SectKey = rData.SectKey;
		nMemberType = rData.nMemberType;
		nLocation = rData.nLocation;    
		nHingeType = rData.nHingeType;
		nInteractionType = rData.nInteractionType;
		memcpy(cDir, rData.cDir, sizeof(cDir));
		memcpy(nSectionNum, rData.nSectionNum, sizeof(nSectionNum));
		memcpy(nHingeLocation, rData.nHingeLocation, sizeof(nHingeLocation));
		memcpy(nHysModel, rData.nHysModel, sizeof(rData.nHysModel));
		memcpy(AllProp.PROP, rData.PROP, sizeof(rData.PROP));
		memcpy(AllSubProp.PROP, rData.PROP, sizeof(rData.PROP));
		for (int i = 0; i < 7; i++)
		{
			AllProp.PMMULT[i].Initialize(); // v882 에서 추가
			AllSubProp.PMMULT[i].Initialize(); // v882 에서 추가
		}
		for(int i=0; i<7; i++)	bExistIJProp[i] = FALSE;
		nWallType = 0;
		bIJExistYield = FALSE;

		for (int i = 0; i < 6; i++)
		{
			AllProp.PROP[i].Convert650(rData.PROP[i], nHysModel[i], i, nHingeType);    
			AllSubProp.PROP[i].Convert650(rData.PROP[i], nHysModel[i], i, nHingeType);    
		}
		AllProp.PROP[6].YLDSUR.Convert955(rData.PROP[6].YLDSUR);
		AllSubProp.PROP[6].YLDSUR.Convert955(rData.PROP[6].YLDSUR);
		nSeq=rData.nSeq;    
		nDefinition = 0;
		FibrKey = 0;    
		nUserAutoOpt = 1;
		bAddData4D2E = FALSE;
		nTempVar = 1;
		nFiberAutoUser    = 1;
		nUseIehcShearR    = 0;
		dShearR           = 0.4;
		nUseIehcConsOutpl = 0;
		bConsOutpl        = FALSE;
		nUseIehcLocation  = 1;
		for (int i=0; i<6; i++)MultiData[i].arMultiLinearData.RemoveAll();

		nFlexSitf = 0;
		for (int i = 0; i < 6; ++i)
		{
			nStifLoc[i] = 0;
		}
		bUseNonlinearShearY = TRUE;
		bUseNonlinearShearZ = TRUE;
		
	}
	void Convert650(T_IEHP_D_CH_650& rData)
	{    
		memcpy(strName, rData.strName, sizeof(strName));    
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));    
		nMaterialType = ConvertMaterialTypeOld2Renewal(rData.nMaterialType);    //nMaterialType = rData.nMaterialType;
		nCodeStl = rData.nCodeStl;
		nCodeConc = rData.nCodeConc;
		MatlKey = rData.MatlKey;
		SectKey = rData.SectKey;
		nMemberType = rData.nMemberType;
		nLocation = rData.nLocation;    
		nHingeType = rData.nHingeType;
		nInteractionType = rData.nInteractionType;
		memcpy(cDir, rData.cDir, sizeof(cDir));
		memcpy(nSectionNum, rData.nSectionNum, sizeof(nSectionNum));
		memcpy(nHingeLocation, rData.nHingeLocation, sizeof(nHingeLocation));
		memcpy(nHysModel, rData.nHysModel, sizeof(nHysModel));
		memcpy(AllProp.PROP, rData.PROP, sizeof(rData.PROP));     // Convert 코드가 이상한거 같은데
		memcpy(AllSubProp.PROP, rData.PROP, sizeof(rData.PROP));  // Convert 코드가 이상한거 같은데  		 
		for (int i = 0; i < 7; i++)
		{
			AllProp.PMMULT[i].Initialize(); // v882 에서 추가
			AllSubProp.PMMULT[i].Initialize(); // v882 에서 추가
		}

		for(int i=0; i<7; i++)	bExistIJProp[i] = FALSE;
		nWallType = 0;
		bIJExistYield = FALSE;

		for (int i = 0; i < 6; i++)
		{
			AllProp.PROP[i].Convert650(rData.PROP[i], nHysModel[i], i, nHingeType);    
			AllSubProp.PROP[i].Convert650(rData.PROP[i], nHysModel[i], i, nHingeType);    
		}
		AllProp.PROP[6].YLDSUR.Convert955(rData.PROP[6].YLDSUR);
		AllSubProp.PROP[6].YLDSUR.Convert955(rData.PROP[6].YLDSUR);

		nSeq=rData.nSeq;    
		nDefinition = rData.nDefinition;
		FibrKey = rData.FibrKey;    
		nUserAutoOpt = rData.nUserAutoOpt;
		bAddData4D2E = rData.bAddData4D2E;
		nTempVar = 1;
		nFiberAutoUser    = 1;
		nUseIehcShearR    = 0;
		dShearR           = 0.4;
		nUseIehcConsOutpl = 0;
		bConsOutpl        = FALSE;
		nUseIehcLocation  = 1;
		for (int i=0; i<6; i++)MultiData[i].arMultiLinearData.RemoveAll();
		nFlexSitf = 0;
		for (int i = 0; i < 6; ++i)
		{
			nStifLoc[i] = 0;
		}
		bUseNonlinearShearY = TRUE;
		bUseNonlinearShearZ = TRUE;
	}
	void Convert692(T_IEHP_D_CH_692& rData)
	{    
		memcpy(strName, rData.strName, sizeof(strName));    
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));    
		nMaterialType = ConvertMaterialTypeOld2Renewal(rData.nMaterialType);    //nMaterialType = rData.nMaterialType;
		nCodeStl = rData.nCodeStl;
		nCodeConc = rData.nCodeConc;
		MatlKey = rData.MatlKey;
		SectKey = rData.SectKey;
		nMemberType = rData.nMemberType;
		nLocation = rData.nLocation;    
		nHingeType = rData.nHingeType;
		nInteractionType = rData.nInteractionType;
		memcpy(cDir, rData.cDir, sizeof(cDir));
		memcpy(nSectionNum, rData.nSectionNum, sizeof(nSectionNum));
		memcpy(nHingeLocation, rData.nHingeLocation, sizeof(nHingeLocation));
		memcpy(nHysModel, rData.nHysModel, sizeof(nHysModel));
		memcpy(AllProp.PROP, rData.PROP, sizeof(rData.PROP));     // Convert 코드가 이상한거 같은데
		memcpy(AllSubProp.PROP, rData.PROP, sizeof(rData.PROP));  // Convert 코드가 이상한거 같은데  	 	
		for (int i = 0; i < 7; i++)
		{
			AllProp.PMMULT[i].Initialize(); // v882 에서 추가
			AllSubProp.PMMULT[i].Initialize(); // v882 에서 추가
		}
		for(int i=0; i<7; i++)	bExistIJProp[i] = FALSE;
		nWallType = 0;
		bIJExistYield = FALSE;

		for (int i = 0; i < 6; i++) // yield surface 제외
		{
			AllProp.PROP[i].Convert692(rData.PROP[i], nHysModel[i]);
			AllSubProp.PROP[i].Convert692(rData.PROP[i], nHysModel[i]);
		}
		AllProp.PROP[6].YLDSUR.Convert955(rData.PROP[6].YLDSUR);
		AllSubProp.PROP[6].YLDSUR.Convert955(rData.PROP[6].YLDSUR);

		nSeq=rData.nSeq;    
		nDefinition = rData.nDefinition;
		FibrKey = rData.FibrKey;    
		nUserAutoOpt = rData.nUserAutoOpt;
		bAddData4D2E = rData.bAddData4D2E;
		nTempVar = 1;
		nFiberAutoUser    = 1;
		nUseIehcShearR    = 0;
		dShearR           = 0.4;
		nUseIehcConsOutpl = 0;
		bConsOutpl        = FALSE;
		nUseIehcLocation  = 1;
		for (int i=0; i<6; i++)MultiData[i].arMultiLinearData.RemoveAll();
		nFlexSitf = 0;
		for (int i = 0; i < 6; ++i)
		{
			nStifLoc[i] = 0;
		}
		bUseNonlinearShearY = TRUE;
		bUseNonlinearShearZ = TRUE;
	}
	void Convert791(T_IEHP_D_CH_791& rData)
	{    
		memcpy(strName, rData.strName, sizeof(strName));    
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));    
		nMaterialType = ConvertMaterialTypeOld2Renewal(rData.nMaterialType);    //nMaterialType = rData.nMaterialType;
		nCodeStl = rData.nCodeStl;
		nCodeConc = rData.nCodeConc;
		MatlKey = rData.MatlKey;
		SectKey = rData.SectKey;
		nMemberType = rData.nMemberType;
		nLocation = rData.nLocation;    
		nHingeType = rData.nHingeType;
		nInteractionType = rData.nInteractionType;
		memcpy(cDir, rData.cDir, sizeof(cDir));
		memcpy(nSectionNum, rData.nSectionNum, sizeof(nSectionNum));
		memcpy(nHingeLocation, rData.nHingeLocation, sizeof(nHingeLocation));
		memcpy(nHysModel, rData.nHysModel, sizeof(nHysModel));
		memcpy(AllProp.PROP, rData.PROP, sizeof(rData.PROP));        // Convert 코드가 이상한거 같은데
		memcpy(AllSubProp.PROP, rData.PROP, sizeof(rData.PROP));     // Convert 코드가 이상한거 같은데		 
		for (int i = 0; i < 7; i++)
		{
			AllProp.PMMULT[i].Initialize(); // v882 에서 추가
			AllSubProp.PMMULT[i].Initialize(); // v882 에서 추가
		}
		for(int i=0; i<7; i++)	bExistIJProp[i] = FALSE;
		nWallType = 0;
		bIJExistYield = FALSE;

		for (int i = 0; i < 6; i++) // yield surface 제외
		{
			AllProp.PROP[i].Convert815(rData.PROP[i], nHysModel[i]);
			AllSubProp.PROP[i].Convert815(rData.PROP[i], nHysModel[i]);
		}
		AllProp.PROP[6].YLDSUR.Convert955(rData.PROP[6].YLDSUR);
		AllSubProp.PROP[6].YLDSUR.Convert955(rData.PROP[6].YLDSUR);
		nSeq=rData.nSeq;    
		nDefinition = rData.nDefinition;
		FibrKey = rData.FibrKey;    
		nUserAutoOpt = rData.nUserAutoOpt;
		bAddData4D2E = rData.bAddData4D2E;
		nTempVar = 1;
		nFiberAutoUser    = 1;
		nUseIehcShearR    = 0;
		dShearR           = 0.4;
		nUseIehcConsOutpl = 0;
		bConsOutpl        = FALSE;
		nUseIehcLocation  = 1;
		for (int i=0; i<6; i++)MultiData[i].arMultiLinearData.RemoveAll();
		nFlexSitf = 0;
		for (int i = 0; i < 6; ++i)
		{
			nStifLoc[i] = 0;
		}
		bUseNonlinearShearY = TRUE;
		bUseNonlinearShearZ = TRUE;
	}
	void Convert815(T_IEHP_D_CH_815& rData)
	{    
		memcpy(strName, rData.strName, sizeof(strName));    
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));    
		nMaterialType = ConvertMaterialTypeOld2Renewal(rData.nMaterialType);    //nMaterialType = rData.nMaterialType;
		nCodeStl = rData.nCodeStl;
		nCodeConc = rData.nCodeConc;
		MatlKey = rData.MatlKey;
		SectKey = rData.SectKey;
		nMemberType = rData.nMemberType;
		nLocation = rData.nLocation;    
		nHingeType = rData.nHingeType;
		nInteractionType = rData.nInteractionType;
		memcpy(cDir, rData.cDir, sizeof(cDir));
		memcpy(nSectionNum, rData.nSectionNum, sizeof(nSectionNum));
		memcpy(nHingeLocation, rData.nHingeLocation, sizeof(nHingeLocation));
		memcpy(nHysModel, rData.nHysModel, sizeof(nHysModel));
		memcpy(AllProp.PROP, rData.PROP, sizeof(rData.PROP));
		memcpy(AllSubProp.PROP, rData.PROP, sizeof(rData.PROP));
		for (int i = 0; i < 7; i++)
		{
			AllProp.PMMULT[i].Initialize(); // v882 에서 추가
			AllSubProp.PMMULT[i].Initialize(); // v882 에서 추가
		}
		for(int i=0; i<7; i++)	bExistIJProp[i] = FALSE;
		nWallType = 0;
		bIJExistYield = FALSE;

		for (int i = 0; i < 6; i++) // yield surface 제외
		{
			AllProp.PROP[i].Convert815(rData.PROP[i], nHysModel[i]);
			AllSubProp.PROP[i].Convert815(rData.PROP[i], nHysModel[i]);
		}
		AllProp.PROP[6].YLDSUR.Convert955(rData.PROP[6].YLDSUR);
		AllSubProp.PROP[6].YLDSUR.Convert955(rData.PROP[6].YLDSUR);

		nSeq=rData.nSeq;    
		nDefinition = rData.nDefinition;
		FibrKey = rData.FibrKey;    
		nUserAutoOpt = rData.nUserAutoOpt;
		bAddData4D2E = rData.bAddData4D2E;
		nTempVar = 1;
		nFiberAutoUser    = 1;
		nUseIehcShearR    = 0;
		dShearR           = 0.4;
		nUseIehcConsOutpl = 0;
		bConsOutpl        = FALSE;
		nUseIehcLocation  = 1;
		for (int i=0; i<6; i++) MultiData[i].arMultiLinearData.Copy(rData.MultiData[i].arMultiLinearData);
		nFlexSitf = 0;
		for (int i = 0; i < 6; ++i)
		{
			nStifLoc[i] = 0;
		}
		bUseNonlinearShearY = TRUE;
		bUseNonlinearShearZ = TRUE;
	}
	void Convert860(T_IEHP_D_CH_860& rData)
	{    
		memcpy(strName, rData.strName, sizeof(strName));    
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));    
		nMaterialType = ConvertMaterialTypeOld2Renewal(rData.nMaterialType);    //nMaterialType = rData.nMaterialType;
		nCodeStl = rData.nCodeStl;
		nCodeConc = rData.nCodeConc;
		MatlKey = rData.MatlKey;
		SectKey = rData.SectKey;
		nMemberType = rData.nMemberType;
		nLocation = rData.nLocation;    
		nHingeType = rData.nHingeType;
		nInteractionType = rData.nInteractionType;
		memcpy(cDir, rData.cDir, sizeof(cDir));
		memcpy(nSectionNum, rData.nSectionNum, sizeof(nSectionNum));
		memcpy(nHingeLocation, rData.nHingeLocation, sizeof(nHingeLocation));
		memcpy(nHysModel, rData.nHysModel, sizeof(nHysModel));

		for (int i = 0; i < 6; i++)
		{
			AllProp.PROP[i].Convert955(rData.PROP[i], nHysModel[i]);
			AllSubProp.PROP[i].Convert955(rData.SubPROP[i], nHysModel[i]);
		}
		AllProp.PROP[6].YLDSUR.Convert955(rData.PROP[6].YLDSUR);
		AllSubProp.PROP[6].YLDSUR.Convert955(rData.SubPROP[6].YLDSUR);

		for (int i = 0; i < 7; i++)
		{
			AllProp.PMMULT[i].Initialize(); // v882 에서 추가
			AllSubProp.PMMULT[i].Initialize(); // v882 에서 추가
		}
		for(int i=0; i<7; i++)	bExistIJProp[i] = rData.bExistIJProp[i];
		nWallType = rData.nWallType;
		bIJExistYield = rData.bIJExistYield;

		for (int i = 0; i < 6; i++) // yield surface 제외
		{
			AllProp.PROP[i].Convert860(rData.PROP[i], nHysModel[i]);
			AllSubProp.PROP[i].Convert860(rData.SubPROP[i], nHysModel[i]);
		}
		nSeq=rData.nSeq;    
		nDefinition = rData.nDefinition;
		FibrKey = rData.FibrKey;    
		nUserAutoOpt = rData.nUserAutoOpt;
		bAddData4D2E = rData.bAddData4D2E;
		nTempVar = 1;
		for (int i=0; i<6; i++) MultiData[i].arMultiLinearData.Copy(rData.MultiData[i].arMultiLinearData);

		nFiberAutoUser    = 1;    //v865 추가
		nUseIehcShearR    = 0;     
		dShearR           = 0.4;
		nUseIehcConsOutpl = 0;
		bConsOutpl        = FALSE;
		nUseIehcLocation  = 1;

		nFlexSitf = 0;
		for (int i = 0; i < 6; ++i)
		{
			nStifLoc[i] = 0;
		}
		bUseNonlinearShearY = TRUE;
		bUseNonlinearShearZ = TRUE;
	}
	void Convert881(T_IEHP_D_CH_881& rData)
	{    
		memcpy(strName, rData.strName, sizeof(strName));    
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));    
		nMaterialType = ConvertMaterialTypeOld2Renewal(rData.nMaterialType);    //nMaterialType = rData.nMaterialType;
		nCodeStl = rData.nCodeStl;
		nCodeConc = rData.nCodeConc;
		MatlKey = rData.MatlKey;
		SectKey = rData.SectKey;
		nMemberType = rData.nMemberType;
		nLocation = rData.nLocation;    
		nHingeType = rData.nHingeType;
		nInteractionType = rData.nInteractionType;
		memcpy(cDir, rData.cDir, sizeof(cDir));
		memcpy(nSectionNum, rData.nSectionNum, sizeof(nSectionNum));
		memcpy(nHingeLocation, rData.nHingeLocation, sizeof(nHingeLocation));
		memcpy(nHysModel, rData.nHysModel, sizeof(nHysModel));

		for (int i = 0; i < 6; i++)
		{
			AllProp.PROP[i].Convert955(rData.PROP[i], nHysModel[i]);
			AllSubProp.PROP[i].Convert955(rData.SubPROP[i], nHysModel[i]);
		}
		AllProp.PROP[6].YLDSUR.Convert955(rData.PROP[6].YLDSUR);
		AllSubProp.PROP[6].YLDSUR.Convert955(rData.SubPROP[6].YLDSUR);

		for (int i = 0; i < 7; i++)
		{
			AllProp.PMMULT[i].Initialize(); // v882 에서 추가
			AllSubProp.PMMULT[i].Initialize(); // v882 에서 추가
		}
		for(int i=0; i<7; i++)	bExistIJProp[i] = rData.bExistIJProp[i];
		nWallType = rData.nWallType;
		bIJExistYield = rData.bIJExistYield;    
		nSeq=rData.nSeq;    
		nDefinition = rData.nDefinition;
		FibrKey = rData.FibrKey;    
		nUserAutoOpt = rData.nUserAutoOpt;
		bAddData4D2E = rData.bAddData4D2E;
		nTempVar = 1;
		for (int i=0; i<6; i++) MultiData[i].arMultiLinearData.Copy(rData.MultiData[i].arMultiLinearData);

		nFiberAutoUser    = rData.nFiberAutoUser;
		nUseIehcShearR    = rData.nUseIehcShearR;     
		dShearR           = rData.dShearR;
		nUseIehcConsOutpl = rData.nUseIehcConsOutpl;
		bConsOutpl        = rData.bConsOutpl;
		nUseIehcLocation  = rData.nUseIehcLocation;

		nFlexSitf = 0;
		for (int i = 0; i < 6; ++i)
		{
			nStifLoc[i] = 0;
		}
		bUseNonlinearShearY = TRUE;
		bUseNonlinearShearZ = TRUE;
	}
};

struct T_IEHP_D_CH_RW_RENEW_830
{
	char    strName[20];
	char    strDescription[20]; 
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_PROP_955 PROP[7];  
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가  
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수

	T_IEHP_MULT_CH_RW  MultiData[6];  // 7.9.5추가 // MNET:4337-JWKWON-20110921

	void GetIehp(T_IEHP_D_CH& rData)
	{
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strDescription, strDescription, sizeof(strDescription));
		rData.nMaterialType = rData.ConvertMaterialTypeOld2Renewal(nMaterialType);   // rData.nMaterialType = nMaterialType;
		rData.nCodeStl = nCodeStl;
		rData.nCodeConc = nCodeConc;
		rData.MatlKey = MatlKey;
		rData.SectKey = SectKey;
		rData.nMemberType = nMemberType;
		rData.nLocation = nLocation;
		rData.nHingeType = nHingeType;
		rData.nInteractionType = nInteractionType;
		memcpy(rData.cDir, cDir, sizeof(cDir));
		memcpy(rData.nSectionNum, nSectionNum, sizeof(nSectionNum));
		memcpy(rData.nHingeLocation, nHingeLocation, sizeof(nHingeLocation));
		memcpy(rData.nHysModel, nHysModel, sizeof(nHysModel));

		for (int i = 0; i < 6; i++)
		{
			rData.AllProp.PROP[i].Convert955(PROP[i], nHysModel[i]);
			rData.AllSubProp.PROP[i].Convert955(PROP[i], nHysModel[i]);
		}
		rData.AllProp.PROP[6].YLDSUR.Convert955(PROP[6].YLDSUR);
		rData.AllSubProp.PROP[6].YLDSUR.Convert955(PROP[6].YLDSUR);

		for (int i = 0; i < 7; i++)
		{
			rData.AllProp.PMMULT[i].Initialize(); // v882 에서 추가
			rData.AllSubProp.PMMULT[i].Initialize(); // v882 에서 추가
		}
		for(int i=0; i<7; i++)	rData.bExistIJProp[i] = FALSE;
		rData.nWallType = 0;
		rData.bIJExistYield = FALSE;

		rData.nSeq = nSeq;
		rData.nDefinition = nDefinition;
		rData.FibrKey = FibrKey; 
		rData.nUserAutoOpt = nUserAutoOpt;
		rData.bAddData4D2E = bAddData4D2E;
		rData.nTempVar = nTempVar;		
		rData.nFiberAutoUser    = 1;    //v865 추가
		rData.nUseIehcShearR    = 0;     
		rData.dShearR           = 0.4;
		rData.nUseIehcConsOutpl = 0;
		rData.bConsOutpl        = FALSE;
		rData.nUseIehcLocation  = 1;

		for (int i=0; i<6; i++) rData.MultiData[i].arMultiLinearData.SetSize(MultiData[i].nMultiDataCount);

		rData.nFlexSitf = 0;
		for (int i = 0; i < 6; ++i)
		{
			rData.nStifLoc[i] = 0;
		}
		rData.bUseNonlinearShearY = TRUE;
		rData.bUseNonlinearShearZ = TRUE;
	}
};

struct T_IEHP_D_CH_RW_RENEW_860
{
	char    strName[20];
	char    strDescription[20]; 
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_PROP_955 PROP[7];
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가  
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수

	T_IEHP_MULT_CH_RW  MultiData[6];  // 7.9.5추가 // MNET:4337-JWKWON-20110921

	T_IEHP_PROP_955 SubPROP[7];			// I&J가 필요할 경우, 즉 2개의 정보 = 여기도 저장
	int			nWallType;
	BOOL		bIJExistYield;			// Yield Dlg에서 IJ 존재 유무
	BOOL		bExistIJProp[7];	  // Prop  Dlg에서 IJ 존재 유무

	void GetIehp(T_IEHP_D_CH& rData)
	{
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strDescription, strDescription, sizeof(strDescription));
		rData.nMaterialType = nMaterialType;
		rData.nCodeStl = nCodeStl;
		rData.nCodeConc = nCodeConc;
		rData.MatlKey = MatlKey;
		rData.SectKey = SectKey;
		rData.nMemberType = nMemberType;
		rData.nLocation = nLocation;
		rData.nHingeType = nHingeType;
		rData.nInteractionType = nInteractionType;
		memcpy(rData.cDir, cDir, sizeof(cDir));
		memcpy(rData.nSectionNum, nSectionNum, sizeof(nSectionNum));
		memcpy(rData.nHingeLocation, nHingeLocation, sizeof(nHingeLocation));
		memcpy(rData.nHysModel, nHysModel, sizeof(nHysModel));
		
		for (int i = 0; i < 6; i++)
		{
			rData.AllProp.PROP[i].Convert955(PROP[i], nHysModel[i]);
			rData.AllSubProp.PROP[i].Convert955(SubPROP[i], nHysModel[i]);
		}
		rData.AllProp.PROP[6].YLDSUR.Convert955(PROP[6].YLDSUR);
		rData.AllSubProp.PROP[6].YLDSUR.Convert955(SubPROP[6].YLDSUR);

		for (int i = 0; i < 7; i++)
		{
			rData.AllProp.PMMULT[i].Initialize(); // v882 에서 추가
			rData.AllSubProp.PMMULT[i].Initialize(); // v882 에서 추가
		}
		memcpy(rData.bExistIJProp, bExistIJProp, sizeof(bExistIJProp));    
		rData.nWallType = nWallType;
		rData.bIJExistYield = bIJExistYield;
		rData.nSeq = nSeq;
		rData.nDefinition = nDefinition;
		rData.FibrKey = FibrKey; 
		rData.nUserAutoOpt = nUserAutoOpt;
		rData.bAddData4D2E = bAddData4D2E;
		rData.nTempVar = nTempVar;		
		rData.nFiberAutoUser    = 1;    //v865 추가
		rData.nUseIehcShearR    = 0;     
		rData.dShearR           = 0.4;
		rData.nUseIehcConsOutpl = 0;
		rData.bConsOutpl        = FALSE;
		rData.nUseIehcLocation  = 1;

		rData.nFlexSitf = 0;
		for (int i = 0; i < 6; ++i)
		{
			rData.nStifLoc[i] = 0;
		}
		rData.bUseNonlinearShearY = TRUE;
		rData.bUseNonlinearShearZ = TRUE;
	}
};

struct T_IEHP_D_CH_RW_881
{
	char    strName[20];
	char    strDescription[20]; 
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_PROP_955 PROP[7];  
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가  
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수

	T_IEHP_MULT_CH_RW  MultiData[6];  // 7.9.5추가 // MNET:4337-JWKWON-20110921

	T_IEHP_PROP_955 SubPROP[7];			// I&J가 필요할 경우, 즉 2개의 정보 = 여기도 저장
	int			nWallType;
	BOOL		bIJExistYield;			// Yield Dlg에서 IJ 존재 유무
	BOOL		bExistIJProp[7];	  // Prop  Dlg에서 IJ 존재 유무
	int     nFiberAutoUser    ; // 0:Use option of IEHC, 1:User Defined  
	int     nUseIehcShearR    ; // 0:Use option of IEHC, 1:User Defined
	double  dShearR           ; // Shear Spring Location : r
	int     nUseIehcConsOutpl ; // 0:Use option of IEHC, 1:User Defined
	BOOL    bConsOutpl        ;
	int     nUseIehcLocation  ; 

	void GetIehp(T_IEHP_D_CH& rData)
	{
		//     rKey=key;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strDescription, strDescription, sizeof(strDescription));
		rData.nMaterialType = nMaterialType;
		rData.nCodeStl = nCodeStl;
		rData.nCodeConc = nCodeConc;
		rData.MatlKey = MatlKey;
		rData.SectKey = SectKey;
		rData.nMemberType = nMemberType;
		rData.nLocation = nLocation;
		rData.nHingeType = nHingeType;
		rData.nInteractionType = nInteractionType;
		memcpy(rData.cDir, cDir, sizeof(cDir));
		memcpy(rData.nSectionNum, nSectionNum, sizeof(nSectionNum));
		memcpy(rData.nHingeLocation, nHingeLocation, sizeof(nHingeLocation));
		memcpy(rData.nHysModel, nHysModel, sizeof(nHysModel));

		for (int i = 0; i < 6; i++)
		{
			rData.AllProp.PROP[i].Convert955(PROP[i], nHysModel[i]);
			rData.AllSubProp.PROP[i].Convert955(SubPROP[i], nHysModel[i]);
		}

		rData.AllProp.PROP[6].YLDSUR.Convert955(PROP[6].YLDSUR);
		rData.AllSubProp.PROP[6].YLDSUR.Convert955(SubPROP[6].YLDSUR);

		for (int i = 0; i < 7; i++)
		{
			rData.AllProp.PMMULT[i].Initialize(); // v882 에서 추가
			rData.AllSubProp.PMMULT[i].Initialize(); // v882 에서 추가
		}

		memcpy(rData.bExistIJProp, bExistIJProp, sizeof(bExistIJProp));    
		rData.nWallType = nWallType;
		rData.bIJExistYield = bIJExistYield;
		rData.nSeq = nSeq;
		rData.nDefinition = nDefinition;
		rData.FibrKey = FibrKey; 
		rData.nUserAutoOpt = nUserAutoOpt;
		rData.bAddData4D2E = bAddData4D2E;
		rData.nTempVar = nTempVar;		
		rData.nFiberAutoUser    = nFiberAutoUser   ;		
		rData.nUseIehcShearR    = nUseIehcShearR   ;		
		rData.dShearR           = dShearR          ;		
		rData.nUseIehcConsOutpl = nUseIehcConsOutpl;		
		rData.bConsOutpl        = bConsOutpl       ;		
		rData.nUseIehcLocation  = nUseIehcLocation ;		
		//     for (int i=0; i<6; i++) rData.MultiData[i].arMultiLinearData.Copy(MultiData[i].arMultiLinearData);    

		rData.nFlexSitf = 0;
		for (int i = 0; i < 6; ++i)
		{
			rData.nStifLoc[i] = 0;
		}
		rData.bUseNonlinearShearY = TRUE;
		rData.bUseNonlinearShearZ = TRUE;
	}
};

struct T_IEHP_D_CH_RW_955
{
	char    strName[20];
	char    strDescription[20];
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_ALL_PROP_RW_955 AllProp;
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가  
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수

	T_IEHP_MULT_CH_RW  MultiData[6];  // 7.9.5추가 // MNET:4337-JWKWON-20110921

	T_IEHP_ALL_PROP_RW_955 AllSubProp;			// I&J가 필요할 경우, 즉 2개의 정보 = 여기도 저장
	int			nWallType;
	BOOL		bIJExistYield;			// Yield Dlg에서 IJ 존재 유무
	BOOL		bExistIJProp[7];	  // Prop  Dlg에서 IJ 존재 유무
	int     nFiberAutoUser; // 0:Use option of IEHC, 1:User Defined  
	int     nUseIehcShearR; // 0:Use option of IEHC, 1:User Defined
	double  dShearR; // Shear Spring Location : r
	int     nUseIehcConsOutpl; // 0:Use option of IEHC, 1:User Defined
	BOOL    bConsOutpl;
	int     nUseIehcLocation;

	void GetIehp(T_IEHP_D_CH& rData)
	{
		//     rKey=key;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strDescription, strDescription, sizeof(strDescription));
		rData.nMaterialType = nMaterialType;
		rData.nCodeStl = nCodeStl;
		rData.nCodeConc = nCodeConc;
		rData.MatlKey = MatlKey;
		rData.SectKey = SectKey;
		rData.nMemberType = nMemberType;
		rData.nLocation = nLocation;
		rData.nHingeType = nHingeType;
		rData.nInteractionType = nInteractionType;
		memcpy(rData.cDir, cDir, sizeof(cDir));
		memcpy(rData.nSectionNum, nSectionNum, sizeof(nSectionNum));
		memcpy(rData.nHingeLocation, nHingeLocation, sizeof(nHingeLocation));
		memcpy(rData.nHysModel, nHysModel, sizeof(nHysModel));
		AllProp.GetIehpAllProp955(rData.AllProp, nHysModel);
		AllSubProp.GetIehpAllProp955(rData.AllSubProp, nHysModel);
		memcpy(rData.bExistIJProp, bExistIJProp, sizeof(bExistIJProp));
		rData.nWallType = nWallType;
		rData.bIJExistYield = bIJExistYield;
		rData.nSeq = nSeq;
		rData.nDefinition = nDefinition;
		rData.FibrKey = FibrKey;
		rData.nUserAutoOpt = nUserAutoOpt;
		rData.bAddData4D2E = bAddData4D2E;
		rData.nTempVar = nTempVar;
		rData.nFiberAutoUser = nFiberAutoUser;
		rData.nUseIehcShearR = nUseIehcShearR;
		rData.dShearR = dShearR;
		rData.nUseIehcConsOutpl = nUseIehcConsOutpl;
		rData.bConsOutpl = bConsOutpl;
		rData.nUseIehcLocation = nUseIehcLocation;
		//     for (int i=0; i<6; i++) rData.MultiData[i].arMultiLinearData.Copy(MultiData[i].arMultiLinearData);    

		rData.nFlexSitf = 0;
		for (int i = 0; i < 6; ++i)
		{
			rData.nStifLoc[i] = 0;
		}
		rData.bUseNonlinearShearY = TRUE;
		rData.bUseNonlinearShearZ = TRUE;
	}
};

struct T_IEHP_D_CH_RW_970
{
	char    strName[20];
	char    strDescription[20];
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
	// HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_ALL_PROP_RW AllProp;
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가  
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수

	T_IEHP_MULT_CH_RW  MultiData[6];  // 7.9.5추가 // MNET:4337-JWKWON-20110921

	T_IEHP_ALL_PROP_RW AllSubProp;			// I&J가 필요할 경우, 즉 2개의 정보 = 여기도 저장
	int			nWallType;
	BOOL		bIJExistYield;			// Yield Dlg에서 IJ 존재 유무
	BOOL		bExistIJProp[7];	  // Prop  Dlg에서 IJ 존재 유무
	int     nFiberAutoUser; // 0:Use option of IEHC, 1:User Defined  
	int     nUseIehcShearR; // 0:Use option of IEHC, 1:User Defined
	double  dShearR; // Shear Spring Location : r
	int     nUseIehcConsOutpl; // 0:Use option of IEHC, 1:User Defined
	BOOL    bConsOutpl;
	int     nUseIehcLocation;

	void GetIehp(T_IEHP_D_CH& rData)
	{
		//     rKey=key;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strDescription, strDescription, sizeof(strDescription));
		rData.nMaterialType = nMaterialType;
		rData.nCodeStl = nCodeStl;
		rData.nCodeConc = nCodeConc;
		rData.MatlKey = MatlKey;
		rData.SectKey = SectKey;
		rData.nMemberType = nMemberType;
		rData.nLocation = nLocation;
		rData.nHingeType = nHingeType;
		rData.nInteractionType = nInteractionType;
		memcpy(rData.cDir, cDir, sizeof(cDir));
		memcpy(rData.nSectionNum, nSectionNum, sizeof(nSectionNum));
		memcpy(rData.nHingeLocation, nHingeLocation, sizeof(nHingeLocation));
		memcpy(rData.nHysModel, nHysModel, sizeof(nHysModel));
		AllProp.GetIehpAllProp(rData.AllProp);
		AllSubProp.GetIehpAllProp(rData.AllSubProp);
		memcpy(rData.bExistIJProp, bExistIJProp, sizeof(bExistIJProp));
		rData.nWallType = nWallType;
		rData.bIJExistYield = bIJExistYield;
		rData.nSeq = nSeq;
		rData.nDefinition = nDefinition;
		rData.FibrKey = FibrKey;
		rData.nUserAutoOpt = nUserAutoOpt;
		rData.bAddData4D2E = bAddData4D2E;
		rData.nTempVar = nTempVar;
		rData.nFiberAutoUser = nFiberAutoUser;
		rData.nUseIehcShearR = nUseIehcShearR;
		rData.dShearR = dShearR;
		rData.nUseIehcConsOutpl = nUseIehcConsOutpl;
		rData.bConsOutpl = bConsOutpl;
		rData.nUseIehcLocation = nUseIehcLocation;
		//     for (int i=0; i<6; i++) rData.MultiData[i].arMultiLinearData.Copy(MultiData[i].arMultiLinearData);    
		rData.nFlexSitf = 0;
		for (int i = 0; i < 6; ++i)
		{
			rData.nStifLoc[i] = 0;
		}
		rData.bUseNonlinearShearY = TRUE;
		rData.bUseNonlinearShearZ = TRUE;
	}
};

struct T_IEHP_D_CH_RW
{
	char    strName[20];
	char    strDescription[20]; 
	int     nMaterialType;     // 0=Steel, 1=Concrete, 2=User Defined
	int     nCodeStl;          // 0:AISC, 1:AIJ
	int     nCodeConc;         // 0:ACI,  1:AIJ
	UINT    MatlKey;
	UINT    SectKey;  
	int     nMemberType;        // 0=Beam, 1=Column, 2=Brace
	int     nLocation;          // 0=I, 1=M, 2=J 
	int     nHingeType;         // 0=Lumped, 1=Distributed, 2=Spring(6.5.0추가)
	int     nInteractionType;   // 0=None, 1=P-M, 2=P-M-M
	char    cDir[8];            // '0':꺼짐, '1':켜짐 : 6개만 사용(Dx, Dy, Dz, Rx, Ry, Rz)
	int     nSectionNum[6];     // Number of Section
	int     nHingeLocation[6];  // 0:I, 1:J, 2:I&J, 3:Center
	int     nHysModel[9];       // 6.5.0변경  : nHysModel[6] -> nHysModel[9] <= [9]로 바꾼것은 잘못 바꾼것 같음(바꿀필요 없었음)
															// HYSTERESIS MODEL : D_IEHP_XXXX
	T_IEHP_ALL_PROP_RW AllProp;  
	int     nSeq;
	int     nDefinition;        // 6.5.0추가 0=Skeleton, 1=Fiber
	UINT    FibrKey;            // 6.5.0추가  
	int     nUserAutoOpt;       // 6.5.0추가 0 =User Input, 1=Auto-Calculation
	BOOL    bAddData4D2E;       // 6.5.0추가 : Dyna2E용으로만  쓰이는 데이터(Civil JP 에서만 사용)
	int     nTempVar;           // 6.6.0추가 : 데이터 사이즈 맞추기 위한 임시 변수
	
	T_IEHP_MULT_CH_RW  MultiData[6];  // 7.9.5추가 // MNET:4337-JWKWON-20110921

	T_IEHP_ALL_PROP_RW AllSubProp;			// I&J가 필요할 경우, 즉 2개의 정보 = 여기도 저장
	int			nWallType;
	BOOL		bIJExistYield;			// Yield Dlg에서 IJ 존재 유무
	BOOL		bExistIJProp[7];	  // Prop  Dlg에서 IJ 존재 유무
	int     nFiberAutoUser    ; // 0:Use option of IEHC, 1:User Defined  
	int     nUseIehcShearR    ; // 0:Use option of IEHC, 1:User Defined
	double  dShearR           ; // Shear Spring Location : r
	int     nUseIehcConsOutpl ; // 0:Use option of IEHC, 1:User Defined
	BOOL    bConsOutpl        ;
	int     nUseIehcLocation  ; 

	int     nFlexSitf;         // 0:Flexibility Method, 1:Stiffness Method
	int     nStifLoc[6];       // Fx, Fy, Fz, Mx, My, Mz Stiffness Location 0:Iend, 1:Jend, 2:Center
	BOOL    bUseNonlinearShearY; // Elastic or Nonlinear effect
	BOOL    bUseNonlinearShearZ; // Elastic or Nonlinear effect

	void GetIehp(T_IEHP_D_CH& rData)
	{
//     rKey=key;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strDescription, strDescription, sizeof(strDescription));
		rData.nMaterialType = nMaterialType;
		rData.nCodeStl = nCodeStl;
		rData.nCodeConc = nCodeConc;
		rData.MatlKey = MatlKey;
		rData.SectKey = SectKey;
		rData.nMemberType = nMemberType;
		rData.nLocation = nLocation;
		rData.nHingeType = nHingeType;
		rData.nInteractionType = nInteractionType;
		memcpy(rData.cDir, cDir, sizeof(cDir));
		memcpy(rData.nSectionNum, nSectionNum, sizeof(nSectionNum));
		memcpy(rData.nHingeLocation, nHingeLocation, sizeof(nHingeLocation));
		memcpy(rData.nHysModel, nHysModel, sizeof(nHysModel));    
		AllProp.GetIehpAllProp(rData.AllProp);
		AllSubProp.GetIehpAllProp(rData.AllSubProp);
		memcpy(rData.bExistIJProp, bExistIJProp, sizeof(bExistIJProp));    
		rData.nWallType = nWallType;
		rData.bIJExistYield = bIJExistYield;
		rData.nSeq = nSeq;
		rData.nDefinition = nDefinition;
		rData.FibrKey = FibrKey; 
		rData.nUserAutoOpt = nUserAutoOpt;
		rData.bAddData4D2E = bAddData4D2E;
		rData.nTempVar = nTempVar;		
		rData.nFiberAutoUser    = nFiberAutoUser   ;		
		rData.nUseIehcShearR    = nUseIehcShearR   ;		
		rData.dShearR           = dShearR          ;		
		rData.nUseIehcConsOutpl = nUseIehcConsOutpl;		
		rData.bConsOutpl        = bConsOutpl       ;		
		rData.nUseIehcLocation  = nUseIehcLocation ;		
//     for (int i=0; i<6; i++) rData.MultiData[i].arMultiLinearData.Copy(MultiData[i].arMultiLinearData);    

		rData.nFlexSitf						= nFlexSitf;
		for (int i = 0; i < 6; ++i)
		{
			rData.nStifLoc[i] = nStifLoc[i];
		}
		rData.bUseNonlinearShearY	= bUseNonlinearShearY;
		rData.bUseNonlinearShearZ = bUseNonlinearShearZ;
	}
	void SetIehp(const T_IEHP_D_CH& rData)
	{
//     key = Key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nMaterialType = rData.nMaterialType;
		nCodeStl = rData.nCodeStl;
		nCodeConc = rData.nCodeConc;
		MatlKey = rData.MatlKey;
		SectKey = rData.SectKey;
		nMemberType = rData.nMemberType;
		nLocation = rData.nLocation;    
		nHingeType = rData.nHingeType;
		nInteractionType = rData.nInteractionType;
		memcpy(cDir, rData.cDir, sizeof(cDir));
		memcpy(nSectionNum, rData.nSectionNum, sizeof(nSectionNum));
		memcpy(nHingeLocation, rData.nHingeLocation, sizeof(nHingeLocation));
		memcpy(nHysModel, rData.nHysModel, sizeof(nHysModel));    
		AllProp.SetIehpAllProp(rData.AllProp);
		AllSubProp.SetIehpAllProp(rData.AllSubProp);
		memcpy(bExistIJProp, rData.bExistIJProp, sizeof(bExistIJProp));    
		nWallType = rData.nWallType;
		bIJExistYield = rData.bIJExistYield;

		nSeq=rData.nSeq;    
		nDefinition = rData.nDefinition;
		FibrKey = rData.FibrKey;    
		nUserAutoOpt = rData.nUserAutoOpt;
		bAddData4D2E = rData.bAddData4D2E;
		nTempVar = rData.nTempVar;		
		nFiberAutoUser    = rData.nFiberAutoUser   ;		
		nUseIehcShearR    = rData.nUseIehcShearR   ;		
		dShearR           = rData.dShearR          ;		
		nUseIehcConsOutpl = rData.nUseIehcConsOutpl;		
		bConsOutpl        = rData.bConsOutpl       ;		
		nUseIehcLocation  = rData.nUseIehcLocation ;		
		for (int i=0; i<6; i++) 
		{
			MultiData[i].nMultiDataCount = (int)rData.MultiData[i].arMultiLinearData.GetSize();
		}

		nFlexSitf						= rData.nFlexSitf;
		for (int i = 0; i < 6; ++i)
		{
			nStifLoc[i] = rData.nStifLoc[i];
		}
		bUseNonlinearShearY = rData.bUseNonlinearShearY;
		bUseNonlinearShearZ = rData.bUseNonlinearShearZ;
	}

	void SetMultiData(T_IEHP_D_CH& rData)
	{
		for(int i=0; i<6; i++) 
		{
			MultiData[i].nMultiDataCount = (int)rData.MultiData[i].arMultiLinearData.GetSize();
		}
	}
};

struct T_IEHP_UNIT
{
	T_IEHP_PROP_UNIT PROP;
	T_IEHP_PMMULT_AXIAL_UNIT PROPMulti;
};
struct T_IEHP_UDRD_D_640
{
	T_IEHP_K key;
	T_IEHP_D_640 data;
};
struct T_IEHP_UDRD_D_650
{
	T_IEHP_K key;
	T_IEHP_D_650 data;
};
struct T_IEHP_UDRD_D_692
{
	T_IEHP_K key;
	T_IEHP_D_692 data;
};

struct T_IEHP_UDRD_D_791
{
	T_IEHP_K key;
	T_IEHP_D_791 data;
};

struct T_IEHP_UDRD_D
{
	T_IEHP_K key;
	T_IEHP_D data;
};
struct T_IEHP_UDRD_D_CH_640
{
	T_IEHP_K key;
	T_IEHP_D_CH_640 data;
};
struct T_IEHP_UDRD_D_CH_650
{
	T_IEHP_K key;
	T_IEHP_D_CH_650 data;
};
struct T_IEHP_UDRD_D_CH_692
{
	T_IEHP_K key;
	T_IEHP_D_CH_692 data;
};
struct T_IEHP_UDRD_D_CH_791
{
	T_IEHP_K key;
	T_IEHP_D_CH_791 data;
};
struct T_IEHP_UDRD_D_CH_815
{
	T_IEHP_K key;
	T_IEHP_D_CH_815 data;
};

struct T_IEHP_UDRD_D_CH
{
	T_IEHP_K key;
	T_IEHP_D_CH data;
};

struct T_IEHP_UDRD_D_CH_RW_RENEW_830
{
	T_IEHP_K key;
	T_IEHP_D_CH_RW_RENEW_830 data;
};

struct T_IEHP_UDRD_D_CH_RW_RENEW_860
{
	T_IEHP_K key;
	T_IEHP_D_CH_RW_RENEW_860 data;
};

struct T_IEHP_UDRD_D_CH_RW_881
{
	T_IEHP_K key;
	T_IEHP_D_CH_RW_881 data;
};

struct T_IEHP_UDRD_D_CH_RW_955
{
	T_IEHP_K key;
	T_IEHP_D_CH_RW_955 data;
};

struct T_IEHP_UDRD_D_CH_RW_970
{
	T_IEHP_K key;
	T_IEHP_D_CH_RW_970 data;
};

struct T_IEHP_UDRD_D_CH_RW
{
	T_IEHP_K key;
	T_IEHP_D_CH_RW data;
};

#define HASHSIZEIEHP 10007

#define T_IEHG_K_960 unsigned int // CVLRMT
#define T_IEHG_KEY unsigned int

enum D_IEHG_TYPE {

	D_IEHG_TYPE_BEAMCOLUMN = 1,
	D_IEHG_TYPE_WALL,
	D_IEHG_TYPE_TRUSS,
	D_IEHG_TYPE_GLINK,
	D_IEHG_TYPE_POINTSPRING,
};

union T_IEHG_K
{
	T_IEHG_KEY keymap;
	struct
	{
		unsigned int type : 4; // 1:Beam/Column, 2:Wall, 3:Truss, 4:GeneralLink(NLNK), 5:Point Spring Support
		unsigned int objK : 28; // type==1,3 ->ElemK, type==2 -> WALL_K, type==4 -> NLNK_K, type==5 -> NSPR_KEY
	} key;

	T_IEHG_K() { key.type = D_IEHG_TYPE_BEAMCOLUMN; key.objK = 0; }
	operator unsigned int() { return keymap; }
	T_IEHG_K& operator=(int src)
	{
		keymap = src;
		return *this;
	}
	BOOL operator==(const T_IEHG_K& src) const
	{
		return src.keymap == keymap;
	}
	BOOL IsElemType()
	{
		return key.type == D_IEHG_TYPE_BEAMCOLUMN || key.type == D_IEHG_TYPE_WALL || key.type == D_IEHG_TYPE_TRUSS;
	}
	unsigned int GetElemKey()
	{
		return key.objK;
	}
};

struct T_IEHG_D_865
{
	T_IEHP_K IehpKey;   // Inelastic hinge property key
};
struct T_IEHG_D
{
	T_IEHP_K IehpKey;   // Inelastic hinge property key
	T_KEY FiberKey;     // Iehp가 Fiber&Auto 일 때는 이 값을 사용
	T_IEHG_D() : IehpKey(0), FiberKey(0) {}
	void Initialize()
	{
		IehpKey = 0;
		FiberKey= 0;
	}
	void Convert865(T_IEHG_D_865& rData)
	{
		IehpKey = rData.IehpKey;
		FiberKey= 0;
	}
};
struct T_IEHG_UDRD_D_865
{
	T_IEHG_K key;
	T_IEHG_D_865 data;
};
struct T_IEHG_UDRD_D
{
	T_IEHG_K key;
	T_IEHG_D data;
};
struct T_IEHG_UNIT
{
	T_IEHG_UNIT()
	{
		IehpKey = D_UNITSYS_NONE;
	}
	int IehpKey;
};
#define HASHSIZEIEHG  HASHSIZEELEM

////////////////////////////////////////
// T_IETH_D 는 사용하지 않음
struct T_IETH_BASE
{
	int nModeId;
	double dDamping;
	void Initialize()
	{
		nModeId=0;
		dDamping=0.;
	}
};
#define T_IETH_DT CArray<T_IETH_BASE, T_IETH_BASE&>
struct T_IETH_PROPDAMPING
{
	BOOL   bMassProp;
	BOOL   bStiffProp;
	int    nCoefDefine;  // 1=Direct Specification, 2=Calculate From Modal Damping
	int    nFreqPeriod;  // 1=Frequency, 2=Period
	double dRm;          // Mass Coefficient        
	double dRk;          // Stiffness Coefficient
	double dFreqOne;
	double dFreqTwo;
	double dPeriodOne;   // sec
	double dPeriodTwo;   // sec
	double dDampingOne;
	double dDampingTwo;
	void Initialize()
	{
		bMassProp  = TRUE;
		bStiffProp = TRUE;
		nCoefDefine = 1;
		nFreqPeriod = 1;
		dRm = 0.;        
		dRk = 0.;        
		dFreqOne = 0.0;
		dFreqTwo = 0.0;
		dPeriodOne = 0.0;
		dPeriodTwo = 0.0;
		dDampingOne = 0.0;
		dDampingTwo = 0.0;
	}
};
struct T_IETH_BOUNDARY
{  
	double  dMinSubStepSize;       // sec 580 추가
	int     nMaxIterNum;
	double  dConvergenceTol;
	void Initialize()
	{
		dMinSubStepSize = 1.e-5;
		nMaxIterNum = 10;
		dConvergenceTol = 1.e-3;
	}  
};

#define T_IETH_K unsigned int
struct T_IETH_D
{
	CString LoadCaseName;
	CString Description;
	double dEndTime;      // End Time 
	double dDelta;        // Time Increment 
	int nOut;             // Step Number Increment for Output
	int nAnalType;        // 1= Linear, 2 = Nonlinear
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   
	int nTimeHistoryType; // 1=Transient, 2=Periodic          
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad    
	T_IETH_K KeyFinalState; // Previous Load Case
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping     
	double dDampAll;      // Damping Ratio for All Modes 
	T_IETH_DT aDampData;
	T_IETH_PROPDAMPING PropDamping;  
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input 
	double dGamma;         
	double dBeta;          
	T_IETH_BOUNDARY BoundaryNL;
	unsigned int LoadCaseId;
	void Initialize()
	{
		LoadCaseName.Empty();
		Description.Empty();
		dEndTime=1.0;
		dDelta=0.1;
		nOut=1;
		nAnalType = 2;
		nAnalMethod = 2;
		nTimeHistoryType = 1;
		nInitCondition =1;
		KeyFinalState = 0;
		nDampingType = 1;
		dDampAll=0.;
		aDampData.RemoveAll();
		PropDamping.Initialize();
		nIntegrationPara = 1;
		dGamma = 0.5;
		dBeta = 0.25;
		BoundaryNL.Initialize();
		LoadCaseId=0;
	}
	T_IETH_D() {}
	T_IETH_D(T_IETH_D& src) {*this = src;}
	T_IETH_D& operator=(T_IETH_D& src)
	{
		LoadCaseName=src.LoadCaseName;
		Description=src.Description;
		dEndTime=src.dEndTime;
		dDelta=src.dDelta;
		nOut=src.nOut;
		nAnalType=src.nAnalType;
		nAnalMethod = src.nAnalMethod;
		nTimeHistoryType = src.nTimeHistoryType;
		nInitCondition = src.nInitCondition;
		KeyFinalState = src.KeyFinalState;
		nDampingType = src.nDampingType;
		dDampAll=src.dDampAll;
		aDampData.Copy(src.aDampData);
		PropDamping = src.PropDamping;
		nIntegrationPara = src.nIntegrationPara;
		dGamma = src.dGamma;
		dBeta = src.dBeta;
		BoundaryNL = src.BoundaryNL;
		LoadCaseId=src.LoadCaseId;
		return *this;
	}
};
struct T_IETH_D_CH
{
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;      // End Time 
	double dDelta;        // Time Increment 
	int nOut;             // Step Number Increment for Output
	int nAnalType;        // 1= Linear, 2 = Nonlinear
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   
	int nTimeHistoryType; // 1=Transient, 2=Periodic          
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad    
	T_IETH_K KeyFinalState; // Previous Load Case
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping     
	double dDampAll;      // Damping Ratio for All Modes 
	T_IETH_DT aDampData;
	T_IETH_PROPDAMPING PropDamping;  
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input 
	double dGamma;         
	double dBeta;          
	T_IETH_BOUNDARY BoundaryNL;
	unsigned int LoadCaseId;

	void ConvertToString(T_IETH_D& rData)
	{
		ConvertCharStr(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.dEndTime=dEndTime;
		rData.dDelta=dDelta;
		rData.nOut=nOut;
		rData.nAnalType=nAnalType;
		rData.nAnalMethod = nAnalMethod;
		rData.nTimeHistoryType = nTimeHistoryType;
		rData.nInitCondition = nInitCondition;
		rData.KeyFinalState = KeyFinalState;
		rData.nDampingType = nDampingType;
		rData.dDampAll=dDampAll;
		rData.aDampData.Copy(aDampData);
		rData.PropDamping = PropDamping;
		rData.nIntegrationPara = nIntegrationPara;
		rData.dGamma = dGamma;
		rData.dBeta = dBeta;
		rData.BoundaryNL = BoundaryNL;
		rData.LoadCaseId=LoadCaseId;
	}
	void ConvertToChar(T_IETH_D& rData)
	{
		ConvertStrChar(rData.LoadCaseName, LoadCaseName, sizeof(LoadCaseName));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		dEndTime=rData.dEndTime;
		dDelta=rData.dDelta;
		nOut=rData.nOut;
		nAnalType=rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nDampingType = rData.nDampingType;
		dDampAll=rData.dDampAll;
		aDampData.Copy(rData.aDampData);
		PropDamping = rData.PropDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;
		BoundaryNL = rData.BoundaryNL;
		LoadCaseId=rData.LoadCaseId;
	}
};
struct T_IETH_UDRD_D
{
	T_IETH_K key;
	T_IETH_D data;
};
struct T_IETH_UDRD_D_CH
{
	T_IETH_K key;
	T_IETH_D_CH data;
};
struct T_IETH_UDRD_D_CH_RW
{
	T_IETH_K key;

	char LoadCaseName[20];
	char Description[80];
	double dEndTime;      // End Time 
	double dDelta;        // Time Increment 
	int nOut;             // Step Number Increment for Output
	int nAnalType;        // 1= Linear, 2 = Nonlinear
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   
	int nTimeHistoryType; // 1=Transient, 2=Periodic          
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad    
	T_IETH_K KeyFinalState; // Previous Load Case
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping     
	double dDampAll;      // Damping Ratio for All Modes 
	int nCountDampData;
	T_IETH_PROPDAMPING PropDamping;  
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input 
	double dGamma;         
	double dBeta;          
	T_IETH_BOUNDARY BoundaryNL;
	unsigned int LoadCaseId;
	void GetIeth(T_IETH_K& rKey, T_IETH_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.LoadCaseName, LoadCaseName, sizeof(LoadCaseName));
		memcpy(rData.Description, Description, sizeof(Description));
		rData.dEndTime=dEndTime;
		rData.dDelta=dDelta;
		rData.nOut=nOut;
		rData.nAnalType=nAnalType;
		rData.nAnalMethod = nAnalMethod;     
		rData.nTimeHistoryType = nTimeHistoryType;
		rData.nInitCondition = nInitCondition;
		rData.KeyFinalState =  KeyFinalState;
		rData.nDampingType =  nDampingType;  
		rData.dDampAll=dDampAll;
		rData.PropDamping = PropDamping;     
		rData.nIntegrationPara = nIntegrationPara;
		rData.dGamma = dGamma;
		rData.dBeta = dBeta;
		rData.BoundaryNL = BoundaryNL;
		rData.LoadCaseId=LoadCaseId;
	}
	void SetIeth(const T_IETH_K Key, const T_IETH_D_CH& rData)
	{
		key=Key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime=rData.dEndTime;
		dDelta=rData.dDelta;
		nOut=rData.nOut;
		nAnalType=rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nDampingType = rData.nDampingType;
		dDampAll=rData.dDampAll;
		nCountDampData=(int)rData.aDampData.GetSize();
		PropDamping = rData.PropDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;
		BoundaryNL = rData.BoundaryNL;
		LoadCaseId=rData.LoadCaseId;
	}  
};
struct T_IETH_UNIT
{
	T_IETH_UNIT()
	{
		LoadCaseName=D_UNITSYS_NONE;
		Description=D_UNITSYS_NONE;
		dEndTime=D_UNITSYS_BASE_TIME;
		dDelta=D_UNITSYS_BASE_TIME;
		nOut=D_UNITSYS_NONE;
		nAnalType=D_UNITSYS_NONE;
		nAnalMethod = D_UNITSYS_NONE;
		nTimeHistoryType = D_UNITSYS_NONE;
		nInitCondition = D_UNITSYS_NONE;
		KeyFinalState = D_UNITSYS_NONE;
		nDampingType = D_UNITSYS_NONE;    
		dDampAll=D_UNITSYS_NONE;
		nModeId=D_UNITSYS_NONE;
		dDamping=D_UNITSYS_NONE;
		PropDamping_dPeriodOne = D_UNITSYS_BASE_TIME; 
		PropDamping_dPeriodTwo = D_UNITSYS_BASE_TIME; 
		nIntegrationPara = D_UNITSYS_NONE;
		dGamma = D_UNITSYS_NONE;
		dBeta = D_UNITSYS_NONE;
		Boundary_dEndTime = D_UNITSYS_BASE_TIME;
		Boundary_dMinSubStepSize = D_UNITSYS_BASE_TIME; 
		LoadCaseId=D_UNITSYS_NONE;
	}
	int LoadCaseName;
	int Description;
	int dEndTime;
	int dDelta;
	int nOut;
	int nAnalType;
	int nAnalMethod;
	int nTimeHistoryType;
	int nInitCondition;
	int KeyFinalState;
	int nDampingType;
	int dDampAll;
	int nModeId;
	int dDamping;
	int PropDamping_dPeriodOne; 
	int PropDamping_dPeriodTwo;
	int nIntegrationPara;
	int dGamma;
	int dBeta;
	int Boundary_dEndTime;
	int Boundary_dMinSubStepSize; 
	int LoadCaseId;
};
#define HASHSIZEIETH 11
#define D_IETH_MINNUM       1   // 입력가능한 최소 하중조건번호
#define D_IETH_MAXNUM    9999   // 입력가능한 최대 하중조건번호

// M-Phi Global Data
#define T_MPGB_K unsigned int
struct T_MPGB_D_700
{
	int    nEarthquakeType;    // 0=Type1, 1=Type2
	int    nInitialForce;      // 0=Equilibrium Element Force Table, 1=Load Case
	int    nLoadCaseType;      // 0=Static Load Case, 1=Construction Stage Load Case
	UINT   LoadCaseKey;
	int    nSectionDivision;
	int    nMaxIteration;
	double dTolerance;
	int    nYieldPoint;        // 0=초항복점(Initial Yielding Point), 1=항복점(Yielding Point)
	void Initialize()
	{
		nEarthquakeType  = 1; 
		nInitialForce    = 0;   
		nLoadCaseType    = 0;   
		LoadCaseKey      = 0;
		nSectionDivision = 50;
		nMaxIteration    = 20;    
		dTolerance       = 0.01;  
		nYieldPoint      = 0;
	}  
};
struct T_MPGB_D_856
{
	int    nEarthquakeType;    // 0=Type1, 1=Type2
	int    nInitialForce;      // 0=Equilibrium Element Force Table, 1=Load Case
	int    nLoadCaseType;      // 0=Static Load Case, 1=Construction Stage Load Case
	UINT   LoadCaseKey;
	int    nSectionDivision;
	int    nMaxIteration;
	double dTolerance;
	int    nYieldPoint;        // 0=초항복점(Initial Yielding Point), 1=항복점(Yielding Point)
	BOOL   bIncludeTendon;     // Include Tendon Secondary , default = FALSE, 호환처리 TRUE
	void Initialize()
	{
		nEarthquakeType  = 1; 
		nInitialForce    = 0;   
		nLoadCaseType    = 0;   
		LoadCaseKey      = 0;
		nSectionDivision = 50;
		nMaxIteration    = 20;    
		dTolerance       = 0.01;  
		nYieldPoint      = 0;
		bIncludeTendon   = FALSE;
	}  
};
struct T_MPGB_D_895
{
	int    nEarthquakeType;    // 890 이후부터 안씀
	int    nInitialForce;      // 0=Equilibrium Element Force Table, 1=Load Case
	int    nLoadCaseType;      // 0=Static Load Case, 1=Construction Stage Load Case
	UINT   LoadCaseKey;
	int    nSectionDivision;
	int    nMaxIteration;
	double dTolerance;
	int    nYieldPoint;        // 0=초항복점(Initial Yielding Point), 1=항복점(Yielding Point)
	BOOL   bIncludeTendon;     // Include Tendon Secondary , default = FALSE, 호환처리 TRUE
	int    nCode;              // 890 이후부터 안씀
};

struct T_MPGB_D
{
	int    nEarthquakeType;    // 890 이후부터 안씀
	int    nInitialForce;      // 0=Equilibrium Element Force Table, 1=Load Case
	int    nLoadCaseType;      // 0=Static Load Case, 1=Construction Stage Load Case
	UINT   LoadCaseKey;
	int    nSectionDivision;
	int    nMaxIteration;
	double dTolerance;
	int    nYieldPoint;        // 0=초항복점(Initial Yielding Point), 1=항복점(Yielding Point)
	BOOL   bIncludeTendon;     // Include Tendon Secondary , default = FALSE, 호환처리 TRUE
	int    nCode;              // 890 이후부터 안씀 
	BOOL   bShiftcurve;        // default = FALSE, 호환처리 TRUE
	void Initialize()
	{
		nEarthquakeType  = 1; 
		nInitialForce    = 1;   
		nLoadCaseType    = 0;   
		LoadCaseKey      = 0;
		nSectionDivision = 350;
		nMaxIteration    = 40;    
		dTolerance       = 0.001;
		nYieldPoint      = 0;
		bIncludeTendon   = FALSE;
		nCode            = 4;
		bShiftcurve      = FALSE;
	}  
	void Convert700(T_MPGB_D_700 rData)
	{
		nEarthquakeType=rData.nEarthquakeType;
		nInitialForce=rData.nInitialForce;
		nLoadCaseType=rData.nLoadCaseType;
		LoadCaseKey=rData.LoadCaseKey;
		nSectionDivision=rData.nSectionDivision;
		nMaxIteration=rData.nMaxIteration;
		dTolerance=rData.dTolerance;
		nYieldPoint=rData.nYieldPoint;
		bIncludeTendon  = TRUE;
		nCode            = 4;
		bShiftcurve     = TRUE;
	}
	void Convert856(T_MPGB_D_856 rData)
	{
		nEarthquakeType=rData.nEarthquakeType;
		nInitialForce=rData.nInitialForce;
		nLoadCaseType=rData.nLoadCaseType;
		LoadCaseKey=rData.LoadCaseKey;
		nSectionDivision=rData.nSectionDivision;
		nMaxIteration=rData.nMaxIteration;
		dTolerance=rData.dTolerance;
		nYieldPoint=rData.nYieldPoint;
		bIncludeTendon  = rData.bIncludeTendon;
		nCode            = 4;
		bShiftcurve     = TRUE;
	}
	void Convert895(T_MPGB_D_895 rData)
	{
		nEarthquakeType=rData.nEarthquakeType;
		nInitialForce=rData.nInitialForce;
		nLoadCaseType=rData.nLoadCaseType;
		LoadCaseKey=rData.LoadCaseKey;
		nSectionDivision=rData.nSectionDivision;
		nMaxIteration=rData.nMaxIteration;
		dTolerance=rData.dTolerance;
		nYieldPoint=rData.nYieldPoint;
		bIncludeTendon  = rData.bIncludeTendon;
		nCode           = rData.nCode;
		bShiftcurve     = TRUE;
	}

};
struct T_MPGB_UDRD_D_700
{
	T_MPGB_D_700 data;
};
struct T_MPGB_UDRD_D_856
{
	T_MPGB_D_856 data;
};
struct T_MPGB_UDRD_D_895
{
	T_MPGB_D_895 data;
};
struct T_MPGB_UDRD_D
{
	T_MPGB_D data;
};
struct T_MPGB_UNIT
{
	T_MPGB_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};
#define HASHSIZEMPGB 1

// M-Phi Inelastic Hinge
#include "DB_ST_DT_MPHG.h"

// M-Phi Section
#include "DB_ST_DT_MPST.h"

// M-Phi Curve Calculation
#define T_MPCC_K unsigned int
struct T_MPCC_LCOM
{
	UINT AnalType;          // D_LOADCASE_???
	UINT LoadCaseKey;
	UINT MnMxType;

	void Initialize()
	{
		AnalType = 0;
		LoadCaseKey = 0;
		MnMxType = 0;
	}

	T_MPCC_LCOM() {}
	T_MPCC_LCOM(T_MPCC_LCOM& src) {*this = src;}
	T_MPCC_LCOM& operator=(T_MPCC_LCOM& src)
	{
		AnalType=src.AnalType;
		LoadCaseKey=src.LoadCaseKey;
		MnMxType=src.MnMxType;

		return *this;
	}
};
struct T_MPCC_D
{  
	CString  strName;           // Name
	T_SECT_K SectK;             // Section
	int      nPosition;         // 0=I, 1=M, 2=J
	T_FIMP_K MatlConcK1;        // Concrete1
	T_FIMP_K MatlConcK2;        // Concrete2
	T_FIMP_K MatlSteelK;        // Steel
	double   dAxialForce;       // Axial Force
	double   dAxisAngle;        // National Axis Angle
	int      nNumPoint;         // No. of Point
	BOOL     bIdealize;         // Display Idealized Model
	BOOL     bCurvature;        // User Defined Curvature (for Ideal model)
	double   dCurvature;        // User Defined Curvature (for Ideal model)
	BOOL     bEvgp;             // Perform Seismic Evaluation
	T_EVGP_K EvgpK;             // Evaluation Group
	int      nEvgpPos;          // 0:Top, 1:Bot
	int      nIdealOpt;         // 0:Option1, 1:Option2, 2:Option3
	T_FIMP_K MatlConcK1Trans;   // Concrete1 - Transverse
	T_FIMP_K MatlConcK2Trans;   // Concrete2 - Transverse
	int      nAxialForce;       // 0:Auto, 1:User
	T_MPCC_LCOM MpccLcom;       // Load Case/Combination
	int      nEvgpColm;         // Evaluation Group Column Index

	void Initialize()
	{
		strName.Empty();
		SectK = 0;
		nPosition = 0;
		MatlConcK1 = 0;
		MatlConcK2 = 0;
		MatlSteelK = 0;
		dAxialForce = 0;
		dAxisAngle = 0;
		nNumPoint = 50;

#if defined(_CIVIL_ORG)
		bIdealize = TRUE;
#else
		bIdealize = FALSE;
#endif

		bCurvature = FALSE;
		dCurvature = 0;

#if defined(_CIVIL_ORG)
		bEvgp = TRUE;
#else
		bEvgp = FALSE;
#endif
		EvgpK = 0;
		nEvgpPos = 0;
#if defined(_CIVIL_ORG)
		nIdealOpt = 0;
#else
		nIdealOpt = 1;
#endif		
		MatlConcK1Trans = 0;
		MatlConcK2Trans = 0;
#if defined(_CIVIL_ORG)
		nAxialForce = 0;
#else
		nAxialForce = 1;  // CEditData::CheckMpcc 에서 체크 원활하게 하기 위함.
#endif
		MpccLcom.Initialize();
		nEvgpColm = 0;
	}
	T_MPCC_D() {}
	T_MPCC_D(T_MPCC_D& src) {*this = src;}
	T_MPCC_D& operator=(T_MPCC_D& src)
	{
		strName=src.strName;
		SectK=src.SectK;
		nPosition=src.nPosition;
		MatlConcK1=src.MatlConcK1;
		MatlConcK2=src.MatlConcK2;
		MatlSteelK=src.MatlSteelK;
		dAxialForce=src.dAxialForce;
		dAxisAngle=src.dAxisAngle;
		nNumPoint=src.nNumPoint;
		bIdealize=src.bIdealize;
		bCurvature=src.bCurvature;
		dCurvature=src.dCurvature;
		bEvgp=src.bEvgp;
		EvgpK=src.EvgpK;
		nEvgpPos=src.nEvgpPos;
		nIdealOpt=src.nIdealOpt;
		MatlConcK1Trans=src.MatlConcK1Trans;
		MatlConcK2Trans=src.MatlConcK2Trans;
		nAxialForce=src.nAxialForce;
		MpccLcom=src.MpccLcom;
		nEvgpColm=src.nEvgpColm;

		return *this;
	}
};

struct T_MPCC_D_CH_882
{
	char     strName[20];  // Name
	T_SECT_K SectK;        // Section
	int      nPosition;    // 1=I, 2=M, 3=J
	T_FIMP_K MatlConcK1;   // Concrete1
	T_FIMP_K MatlConcK2;   // Concrete2
	T_FIMP_K MatlSteelK;   // Steel
	double   dAxialForce;  // Axial Force
	double   dAxisAngle;   // National Axis Angle
	int      nNumPoint;    // No. of Point
	BOOL     bIdealize;    // Display Idealized Model
	BOOL     bCurvature;   // User Defined Curvature (for Ideal model)
	double   dCurvature;   // User Defined Curvature (for Ideal model)
};

struct T_MPCC_D_CH_885
{
	char     strName[20];       // Name
	T_SECT_K SectK;             // Section
	int      nPosition;         // 1=I, 2=M, 3=J
	T_FIMP_K MatlConcK1;        // Concrete1
	T_FIMP_K MatlConcK2;        // Concrete2
	T_FIMP_K MatlSteelK;        // Steel
	double   dAxialForce;       // Axial Force
	double   dAxisAngle;        // National Axis Angle
	int      nNumPoint;         // No. of Point
	BOOL     bIdealize;         // Display Idealized Model
	BOOL     bCurvature;        // User Defined Curvature (for Ideal model)
	double   dCurvature;        // User Defined Curvature (for Ideal model)
	BOOL     bEvgp;             // Perform Seismic Evaluation
	T_EVGP_K EvgpK;             // Evaluation Group
	int      nEvgpPos;          // 0:Top, 1:Bot
	int      nIdealOpt;         // 0:Option1, 1:Option2, 2:Option3
	T_FIMP_K MatlConcK1Trans;   // Concrete1 - Transverse
	T_FIMP_K MatlConcK2Trans;   // Concrete2 - Transverse
	int      nAxialForce;       // 0:Auto, 1:User
	T_MPCC_LCOM MpccLcom;       // Load Case/Combination
};

struct T_MPCC_D_CH_945
{
	char     strName[20];       // Name
	T_SECT_K SectK;             // Section
	int      nPosition;         // 1=I, 2=M, 3=J
	T_FIMP_K MatlConcK1;        // Concrete1
	T_FIMP_K MatlConcK2;        // Concrete2
	T_FIMP_K MatlSteelK;        // Steel
	double   dAxialForce;       // Axial Force
	double   dAxisAngle;        // National Axis Angle
	int      nNumPoint;         // No. of Point
	BOOL     bIdealize;         // Display Idealized Model
	BOOL     bCurvature;        // User Defined Curvature (for Ideal model)
	double   dCurvature;        // User Defined Curvature (for Ideal model)
	BOOL     bEvgp;             // Perform Seismic Evaluation
	T_EVGP_K EvgpK;             // Evaluation Group
	int      nEvgpPos;          // 0:Top, 1:Bot
	int      nIdealOpt;         // 0:Option1, 1:Option2, 2:Option3
	T_FIMP_K MatlConcK1Trans;   // Concrete1 - Transverse
	T_FIMP_K MatlConcK2Trans;   // Concrete2 - Transverse
	int      nAxialForce;       // 0:Auto, 1:User
	T_MPCC_LCOM MpccLcom;       // Load Case/Combination
	int      nEvgpColm;         // Evaluation Group Column Index
};

struct T_MPCC_D_CH_950_Temp
{
	char     strName[20];       // Name
	T_SECT_K SectK;             // Section
	int      nPosition;         // 1=I, 2=M, 3=J
	T_FIMP_K MatlConcK1;        // Concrete1
	T_FIMP_K MatlConcK2;        // Concrete2
	T_FIMP_K MatlSteelK;        // Steel
	double   dAxialForce;       // Axial Force
	double   dAxisAngle;        // National Axis Angle
	int      nNumPoint;         // No. of Point
	BOOL     bIdealize;         // Display Idealized Model
	BOOL     bCurvature;        // User Defined Curvature (for Ideal model)
	double   dCurvature;        // User Defined Curvature (for Ideal model)
	BOOL     bEvgp;             // Perform Seismic Evaluation
	T_EVGP_K EvgpK;             // Evaluation Group
	int      nEvgpPos;          // 0:Top, 1:Bot
	int      nIdealOpt;         // 0:Option1, 1:Option2, 2:Option3
	T_FIMP_K MatlConcK1Trans;   // Concrete1 - Transverse
	T_FIMP_K MatlConcK2Trans;   // Concrete2 - Transverse
	int      nAxialForce;       // 0:Auto, 1:User
	T_MPCC_LCOM MpccLcom;       // Load Case/Combination
	int      nEvgpColm;         // Evaluation Group Column Index

	int nDgnType;       // (v950) 국내내진 1:1차(CDgnSeisMPhiItemDlg::OnDgnSeisMPhiOKBtn())  2:2차 (CSeisCvlSectEffStiffDlg)
	BOOL bUserDefined;  // (v950) 국내내진
	double dI_EffL;     // (v950) 국내내진, 2차 Iy_Eff(Long.) (CSeisCvlSectEffStiffDlg)
	double dI_EffT;     // (v950) 국내내진, 2차 Iy_Eff(Trans.)(CSeisCvlSectEffStiffDlg)
};

struct T_MPCC_D_CH
{
	char     strName[20];       // Name
	T_SECT_K SectK;             // Section
	int      nPosition;         // 1=I, 2=M, 3=J
	T_FIMP_K MatlConcK1;        // Concrete1
	T_FIMP_K MatlConcK2;        // Concrete2
	T_FIMP_K MatlSteelK;        // Steel
	double   dAxialForce;       // Axial Force
	double   dAxisAngle;        // National Axis Angle
	int      nNumPoint;         // No. of Point
	BOOL     bIdealize;         // Display Idealized Model
	BOOL     bCurvature;        // User Defined Curvature (for Ideal model)
	double   dCurvature;        // User Defined Curvature (for Ideal model)
	BOOL     bEvgp;             // Perform Seismic Evaluation
	T_EVGP_K EvgpK;             // Evaluation Group
	int      nEvgpPos;          // 0:Top, 1:Bot
	int      nIdealOpt;         // 0:Option1, 1:Option2, 2:Option3
	T_FIMP_K MatlConcK1Trans;   // Concrete1 - Transverse
	T_FIMP_K MatlConcK2Trans;   // Concrete2 - Transverse
	int      nAxialForce;       // 0:Auto, 1:User
	T_MPCC_LCOM MpccLcom;       // Load Case/Combination
	int      nEvgpColm;         // Evaluation Group Column Index

	void ConvertToString(T_MPCC_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.SectK=SectK;
		rData.nPosition=nPosition;
		rData.MatlConcK1=MatlConcK1;
		rData.MatlConcK2=MatlConcK2;
		rData.MatlSteelK=MatlSteelK;
		rData.dAxialForce=dAxialForce;
		rData.dAxisAngle=dAxisAngle;
		rData.nNumPoint=nNumPoint;
		rData.bIdealize=bIdealize;
		rData.bCurvature=bCurvature;
		rData.dCurvature=dCurvature;
		rData.bEvgp=bEvgp;
		rData.EvgpK=EvgpK;
		rData.nEvgpPos=nEvgpPos;
		rData.nIdealOpt=nIdealOpt;
		rData.MatlConcK1Trans=MatlConcK1Trans;
		rData.MatlConcK2Trans=MatlConcK2Trans;
		rData.nAxialForce=nAxialForce;
		rData.MpccLcom=MpccLcom;
		rData.nEvgpColm=nEvgpColm;
	}
	void ConvertToChar(T_MPCC_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		SectK=rData.SectK;
		nPosition=rData.nPosition;
		MatlConcK1=rData.MatlConcK1;
		MatlConcK2=rData.MatlConcK2;
		MatlSteelK=rData.MatlSteelK;
		dAxialForce=rData.dAxialForce;
		dAxisAngle=rData.dAxisAngle;
		nNumPoint=rData.nNumPoint;
		bIdealize=rData.bIdealize;
		bCurvature=rData.bCurvature;
		dCurvature=rData.dCurvature;
		bEvgp=rData.bEvgp;
		EvgpK=rData.EvgpK;
		nEvgpPos=rData.nEvgpPos;
		nIdealOpt=rData.nIdealOpt;
		MatlConcK1Trans=rData.MatlConcK1Trans;
		MatlConcK2Trans=rData.MatlConcK2Trans;
		nAxialForce=rData.nAxialForce;
		MpccLcom=rData.MpccLcom;
		nEvgpColm=rData.nEvgpColm;
	}
	void Convert882(T_MPCC_D_CH_882& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));
		SectK=rData.SectK;
		nPosition=rData.nPosition;
		MatlConcK1=rData.MatlConcK1;
		MatlConcK2=rData.MatlConcK2;
		MatlSteelK=rData.MatlSteelK;
		dAxialForce=rData.dAxialForce;
		dAxisAngle=rData.dAxisAngle;
		nNumPoint=rData.nNumPoint;
		bIdealize=rData.bIdealize;
		bCurvature=rData.bCurvature;
		dCurvature=rData.dCurvature;
		bEvgp=FALSE;
		EvgpK=0;
		nEvgpPos=0;
#if defined(_CIVIL_ORG)
		nIdealOpt = 0;
#else
		nIdealOpt = 1;
#endif		
		MatlConcK1Trans = rData.MatlConcK1; //
		MatlConcK2Trans = rData.MatlConcK2; //
		nAxialForce = 1;
		MpccLcom.Initialize();
		nEvgpColm = 0;
	}
	void Convert885(T_MPCC_D_CH_885& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));
		SectK=rData.SectK;
		nPosition=rData.nPosition;
		MatlConcK1=rData.MatlConcK1;
		MatlConcK2=rData.MatlConcK2;
		MatlSteelK=rData.MatlSteelK;
		dAxialForce=rData.dAxialForce;
		dAxisAngle=rData.dAxisAngle;
		nNumPoint=rData.nNumPoint;
		bIdealize=rData.bIdealize;
		bCurvature=rData.bCurvature;
		dCurvature=rData.dCurvature;
		bEvgp = rData.bEvgp;
		EvgpK = rData.EvgpK;
		nEvgpPos = rData.nEvgpPos;
		nIdealOpt = rData.nIdealOpt;
		MatlConcK1Trans = rData.MatlConcK1Trans;
		MatlConcK2Trans = rData.MatlConcK2Trans;
		nAxialForce = rData.nAxialForce;
		MpccLcom = rData.MpccLcom;
		nEvgpColm = 0;
	}
	void Convert945(T_MPCC_D_CH_945& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));
		SectK = rData.SectK;
		nPosition = rData.nPosition;
		MatlConcK1 = rData.MatlConcK1;
		MatlConcK2 = rData.MatlConcK2;
		MatlSteelK = rData.MatlSteelK;
		dAxialForce = rData.dAxialForce;
		dAxisAngle = rData.dAxisAngle;
		nNumPoint = rData.nNumPoint;
		bIdealize = rData.bIdealize;
		bCurvature = rData.bCurvature;
		dCurvature = rData.dCurvature;
		bEvgp = rData.bEvgp;
		EvgpK = rData.EvgpK;
		nEvgpPos = rData.nEvgpPos;
		nIdealOpt = rData.nIdealOpt;
		MatlConcK1Trans = rData.MatlConcK1Trans;
		MatlConcK2Trans = rData.MatlConcK2Trans;
		nAxialForce = rData.nAxialForce;
		MpccLcom = rData.MpccLcom;
		nEvgpColm = rData.nEvgpColm;
	}
	void Convert950_Temp(T_MPCC_D_CH_950_Temp& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));
		SectK = rData.SectK;
		nPosition = rData.nPosition;
		MatlConcK1 = rData.MatlConcK1;
		MatlConcK2 = rData.MatlConcK2;
		MatlSteelK = rData.MatlSteelK;
		dAxialForce = rData.dAxialForce;
		dAxisAngle = rData.dAxisAngle;
		nNumPoint = rData.nNumPoint;
		bIdealize = rData.bIdealize;
		bCurvature = rData.bCurvature;
		dCurvature = rData.dCurvature;
		bEvgp = rData.bEvgp;
		EvgpK = rData.EvgpK;
		nEvgpPos = rData.nEvgpPos;
		nIdealOpt = rData.nIdealOpt;
		MatlConcK1Trans = rData.MatlConcK1Trans;
		MatlConcK2Trans = rData.MatlConcK2Trans;
		nAxialForce = rData.nAxialForce;
		MpccLcom = rData.MpccLcom;
		nEvgpColm = rData.nEvgpColm;
	}
};
struct T_MPCC_UDRD_D
{
	T_MPCC_K key;
	T_MPCC_D data;
};
struct T_MPCC_UDRD_D_CH_882
{
	T_MPCC_K key;
	T_MPCC_D_CH_882 data;
};
struct T_MPCC_UDRD_D_CH_885
{
	T_MPCC_K key;
	T_MPCC_D_CH_885 data;
};
struct T_MPCC_UDRD_D_CH_950_Temp
{
	T_MPCC_K key;
	T_MPCC_D_CH_950_Temp data;
};
struct T_MPCC_UDRD_D_CH
{
	T_MPCC_K key;
	T_MPCC_D_CH data;
};
struct T_MPCC_UNIT
{
	T_MPCC_UNIT()
	{
		dAxialForce = D_UNITSYS_BASE_FORCE;
		dAxisAngle = D_UNITSYS_BASE_RADIAN;
		dCurvature = D_UNITSYS_BASE_PERLENGTH;
	}
	int dAxialForce;
	int dAxisAngle;
	int dCurvature;
};
#define HASHSIZEMPCC 11

// Seismic Yield Effective Stiffness
#define T_SYEF_K T_MPCC_K
struct T_SYEF_D
{
	int nDgnType;      // (v950) 국내내진 0:Original 1:1차(CDgnSeisMPhiItemDlg::OnDgnSeisMPhiOKBtn())  2:2차 (CSeisCvlSectEffStiffDlg)
	BOOL bUserDefined; // (v950) 국내내진
	double dI_EffL;    // (v950) 국내내진, 2차 Iy_Eff(Long.) (CSeisCvlSectEffStiffDlg)
	double dI_EffT;    // (v950) 국내내진, 2차 Iy_Eff(Trans.)(CSeisCvlSectEffStiffDlg)
	double dMyL;       // (v950) 국내내진, 2차 계산용,       (CSeisCvlSectEffStiffDlg)
	double dMeL;       // (v950) 국내내진, 2차 계산용,       (CSeisCvlSectEffStiffDlg)
	double dMyT;       // (v950) 국내내진, 2차 계산용,       (CSeisCvlSectEffStiffDlg)
	double dMeT;       // (v950) 국내내진, 2차 계산용,       (CSeisCvlSectEffStiffDlg)

	void Initialize()
	{
		nDgnType = 0;
		bUserDefined = FALSE;
		dI_EffL = 0.0;
		dI_EffT = 0.0;
		dMyL = 0.0;
		dMeL = 0.0;
		dMyT = 0.0;
		dMeT = 0.0;
	}
	T_SYEF_D() { Initialize(); }
	T_SYEF_D(T_SYEF_D& src) { *this = src; }
	T_SYEF_D& operator=(T_SYEF_D& src)
	{
		nDgnType = src.nDgnType;
		bUserDefined = src.bUserDefined;
		dI_EffL = src.dI_EffL;
		dI_EffT = src.dI_EffT;
		dMyL = src.dMyL;
		dMeL = src.dMeL;
		dMyT = src.dMyT;
		dMeT = src.dMeT;

		return *this;
	}
};
struct T_SYEF_UDRD_D
{
	T_SYEF_K key;
	T_SYEF_D data;
};
struct T_SYEF_UNIT
{
	T_SYEF_UNIT()
	{
		dMyL = D_UNITSYS_BASE_MOMENT;
		dMeL = D_UNITSYS_BASE_MOMENT;
		dMyT = D_UNITSYS_BASE_MOMENT;
		dMeT = D_UNITSYS_BASE_MOMENT;
	}
	int dMyL;
	int dMeL;
	int dMyT;
	int dMeT;
};
#define HASHSIZESYEF 11



#define T_ECUC_K unsigned int
struct T_ECUC_D
{
	BOOL   becu;
	int    necuType;
	double decu_User;
	BOOL   bfck85;
	BOOL   beru_tens;
	BOOL   beru_comp;
	double deru_comp;
	int    neruType;

	void Initialize()
	{
		becu=TRUE;
		necuType=0;
		decu_User=0.003;
		bfck85=FALSE;
		beru_tens=FALSE;
		beru_comp=FALSE;
		deru_comp=0.001;
		neruType=0;
	}
};
struct T_ECUC_UDRD_D
{
	T_ECUC_D data;
};
struct T_ECUC_UNIT
{
	T_ECUC_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};
#define HASHSIZEECUC 1

// Inelastic Properties Control Data
#define T_IEHC_K unsigned int

struct T_IEHC_D_865
{
	int    nBeamLoc     ; // Reference location for distributed hinges
	int    nBeamDivNumNy; // Number of divisions(Beam-Column) - Ny
	int    nBeamDivNumNz; // Number of divisions(Beam-Column) - Nz
	BOOL   bWallConsOut ; // Consider out of plane nonlinearity of plate type
	int    nWallDivNumZ ; // Number of divisions - z
	int    nWallDivNumY ; // Number of divisions - y
	double dR           ; // Shear Spring Location r    
};

struct T_IEHC_D_885
{
	int    nBeamLoc     ; // Reference location for distributed hinges
	int    nBeamDivNumNy; // Number of divisions(Beam-Column) - Ny
	int    nBeamDivNumNz; // Number of divisions(Beam-Column) - Nz
	BOOL   bWallConsOut ; // Consider out of plane nonlinearity of plate type
	int    nWallDivNumZ ; // Number of divisions - z
	int    nWallDivNumY ; // Number of divisions - y
	double dR           ; // Shear Spring Location r    
	int    nWAreaSize   ; // Fiber Wall Areas
};

enum { D_IEHC_BEAM_LOC_I, D_IEHC_BEAM_LOC_C, D_IEHC_BEAM_LOC_J };
enum { D_IEHC_WSIZE_AUTO, D_IEHC_WSIZE_EQUAL };
enum { D_IEHC_FSIZE_AUTO, D_IEHC_FSIZE_EQUAL };
struct T_IEHC_D
{
	int    nBeamLoc     ; // Reference location for distributed hinges
	int    nBeamDivNumNy; // Number of divisions(Beam-Column) - Ny, Core
	int    nBeamDivNumNz; // Number of divisions(Beam-Column) - Nz, Core
	BOOL   bWallConsOut ; // Consider out of plane nonlinearity of plate type
	int    nWallDivNumZ ; // Number of divisions - z, Core
	int    nWallDivNumY ; // Number of divisions - y, Core
	double dR           ; // Shear Spring Location r        
	int    nWAreaSize   ; // Fiber Wall Areas Core, D_IEHC_FSIZE_
	BOOL   bConsiderRebarArea1D; // Beam-Column, Consider Rebar Area
	BOOL   bConsiderRebarAreaWall; // Wall, Consider Rebar Area
	int    nFAreaSizeCore; // Fiber Beam Areas Core, 0:Auto Size 1: Equal
	int    nFAreaSizeCover; // Fiber Beam Areas Cover, D_IEHC_FSIZE_
	int    nWAreaSizeCover; // Fiber Wall Areas Cover, D_IEHC_FSIZE_
	int    nBeamDivNumNyCover; // Number of divisions(Beam-Column) - Ny, Cover
	int    nBeamDivNumNzCover; // Number of divisions(Beam-Column) - Nz, Cover
	int    nWallDivNumZCover ; // Number of divisions - z, Cover
	int    nWallDivNumYCover ; // Number of divisions - y, Cover

	void Initialize()
	{
		nBeamLoc      = D_IEHC_BEAM_LOC_I;
		nBeamDivNumNy = 15;
		nBeamDivNumNz = 15;
		bWallConsOut  = FALSE;
		nWallDivNumZ  = 8;
		nWallDivNumY  = 1;
		dR            = 0.4;
		nWAreaSize    = D_IEHC_WSIZE_AUTO;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_IEHC_FSIZE_EQUAL;
		nFAreaSizeCover = D_IEHC_FSIZE_EQUAL;
		nWAreaSizeCover = D_IEHC_FSIZE_EQUAL;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover = nWallDivNumZ;
		nWallDivNumYCover = nWallDivNumY;
	}
	void Convert865(T_IEHC_D_865& rData865)
	{
		nBeamLoc      = rData865.nBeamLoc     ; 
		nBeamDivNumNy = rData865.nBeamDivNumNy; 
		nBeamDivNumNz = rData865.nBeamDivNumNz; 
		bWallConsOut  = rData865.bWallConsOut ; 
		nWallDivNumZ  = rData865.nWallDivNumZ ; 
		nWallDivNumY  = rData865.nWallDivNumY ; 
		dR            = rData865.dR           ; 
		nWAreaSize    = D_IEHC_WSIZE_EQUAL   ;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_IEHC_FSIZE_EQUAL;
		nFAreaSizeCover = D_IEHC_FSIZE_EQUAL;
		nWAreaSizeCover = D_IEHC_FSIZE_EQUAL;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover = nWallDivNumZ;
		nWallDivNumYCover = nWallDivNumY;
	}
	void Convert885(T_IEHC_D_885& rData885)
	{
		nBeamLoc      = rData885.nBeamLoc     ; 
		nBeamDivNumNy = rData885.nBeamDivNumNy; 
		nBeamDivNumNz = rData885.nBeamDivNumNz; 
		bWallConsOut  = rData885.bWallConsOut ; 
		nWallDivNumZ  = rData885.nWallDivNumZ ; 
		nWallDivNumY  = rData885.nWallDivNumY ; 
		dR            = rData885.dR           ; 
		nWAreaSize    = rData885.nWAreaSize   ;
		bConsiderRebarArea1D = FALSE;
		bConsiderRebarAreaWall = FALSE;
		nFAreaSizeCore = D_IEHC_FSIZE_EQUAL;
		nFAreaSizeCover = D_IEHC_FSIZE_EQUAL;
		nWAreaSizeCover = D_IEHC_FSIZE_EQUAL;
		nBeamDivNumNyCover = nBeamDivNumNy;
		nBeamDivNumNzCover = nBeamDivNumNz;
		nWallDivNumZCover = nWallDivNumZ;
		nWallDivNumYCover = nWallDivNumY;
	}
};
struct T_IEHC_UDRD_D
{
	T_IEHC_D data;
};
struct T_IEHC_UNIT
{
	T_IEHC_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};
#define HASHSIZEIEHC 1

// Fiber section (wall)
enum D_FIBW_TYPE { D_FIBW_TYPE_IEHP, D_FIBW_TYPE_PHGT };
#define T_FIBW_K unsigned int
struct T_FIBW_D
{  
	CString strName;
	int     nWallID;
	T_STOR_K StorK;
	CArray<T_FIBR_BASE, T_FIBR_BASE&> aFibwBase;
	T_FIMP_K FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
															 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nSeq;
	int    nAssignType; // D_FIBW_TYPE
	COLORREF FimpColor[6];
	void Initialize()
	{
		strName.Empty();
		nWallID = 0;
		StorK = 0;
		aFibwBase.RemoveAll();
		for(int i = 0; i < 6; i++) FimpKey[i] = 0;
		bMonitoredFiber = TRUE;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = 0;
		nSeq = 0;
		nAssignType=D_FIBW_TYPE_IEHP;
		FimpColor[0] = RGB(121, 198, 220);
		FimpColor[1] = RGB(245, 242, 31);
		FimpColor[2] = RGB(214, 126, 176);
		FimpColor[3] = RGB(63, 174, 179);
		FimpColor[4] = RGB(40, 130, 89);
		FimpColor[5] = RGB(223, 176, 192);
	}
	T_FIBW_D() {}
	T_FIBW_D(T_FIBW_D& src) {*this = src;}
	T_FIBW_D& operator=(const T_FIBW_D& src)
	{
		strName = src.strName;
		nWallID = src.nWallID;
		StorK   = src.StorK;
		aFibwBase.Copy(src.aFibwBase);
		memcpy(FimpKey, src.FimpKey, sizeof(FimpKey));
		memcpy(MonitoredFiber, src.MonitoredFiber, sizeof(MonitoredFiber));
		bMonitoredFiber = src.bMonitoredFiber;
		nSeq = src.nSeq;
		nAssignType = src.nAssignType;
		memcpy(FimpColor, src.FimpColor, sizeof(FimpColor));
		return *this;
	}
	// 결과 출력 시에는 Section, Wall ID, Story등의 정보가 필요없고, T_FIBR_D와 동일하기 때문에 변환해서 같이 사용함
	void ConvertToFibr(OUT T_FIBR_D& Fibr)
	{
		Fibr.strName = strName;
		Fibr.aFibrBase.Copy(aFibwBase);
		memcpy(Fibr.FimpKey, FimpKey, sizeof(FimpKey));
		memcpy(Fibr.MonitoredFiber, MonitoredFiber, sizeof(MonitoredFiber));
		Fibr.bMonitoredFiber = bMonitoredFiber;
		Fibr.nSeq = nSeq;
		Fibr.nAssignType = nAssignType;
		memcpy(Fibr.FimpColor, FimpColor, sizeof(FimpColor));
	}
};
struct T_FIBW_D_CH
{  
	char   strName[40];
	int       nWallID;
	T_STOR_K  StorK  ;
	CArray<T_FIBR_BASE_CH, T_FIBR_BASE_CH&> aFibwBase;
	T_FIMP_K FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
															 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nSeq;
	int    nAssignType; // D_FIBW_TYPE
	COLORREF FimpColor[6];
	void ConvertToString(T_FIBW_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nWallID = nWallID;
		rData.StorK   = StorK  ;
		rData.aFibwBase.SetSize(aFibwBase.GetSize());    
		for(int i = 0; i < aFibwBase.GetSize(); i++)
		{
			rData.aFibwBase[i].aPointY.SetSize(aFibwBase[i].aPointY.GetSize());
			rData.aFibwBase[i].aPointZ.SetSize(aFibwBase[i].aPointZ.GetSize());
			rData.aFibwBase[i].FibrBaseKey = aFibwBase[i].FibrBaseKey;
			rData.aFibwBase[i].bRebar      = aFibwBase[i].bRebar;
			rData.aFibwBase[i].dArea       = aFibwBase[i].dArea;
			rData.aFibwBase[i].dCenterY    = aFibwBase[i].dCenterY;
			rData.aFibwBase[i].dCenterZ    = aFibwBase[i].dCenterZ;
			rData.aFibwBase[i].FibrMatlID  = aFibwBase[i].FibrMatlID;
			rData.aFibwBase[i].aPointY.Copy(aFibwBase[i].aPointY);
			rData.aFibwBase[i].aPointZ.Copy(aFibwBase[i].aPointZ);
			ConvertCharStr(aFibwBase[i].strRebarName, rData.aFibwBase[i].strRebarName, sizeof(aFibwBase[i].strRebarName));
		}    
		memcpy(rData.FimpKey, FimpKey, sizeof(FimpKey));
		rData.bMonitoredFiber = bMonitoredFiber;
		memcpy(rData.MonitoredFiber, MonitoredFiber, sizeof(MonitoredFiber));
		rData.nSeq = nSeq;
		rData.nAssignType = nAssignType;
		memcpy(rData.FimpColor, FimpColor, sizeof(FimpColor));
	}
	void ConvertToChar(T_FIBW_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		nWallID = rData.nWallID;
		StorK   = rData.StorK  ;
		aFibwBase.SetSize(rData.aFibwBase.GetSize());
		for(int i = 0; i < aFibwBase.GetSize(); i++)
		{
			aFibwBase[i].aPointY.SetSize(rData.aFibwBase[i].aPointY.GetSize());
			aFibwBase[i].aPointZ.SetSize(rData.aFibwBase[i].aPointZ.GetSize());
			aFibwBase[i].FibrBaseKey = rData.aFibwBase[i].FibrBaseKey;
			aFibwBase[i].bRebar      = rData.aFibwBase[i].bRebar;
			aFibwBase[i].dArea       = rData.aFibwBase[i].dArea;
			aFibwBase[i].dCenterY    = rData.aFibwBase[i].dCenterY;
			aFibwBase[i].dCenterZ    = rData.aFibwBase[i].dCenterZ;
			aFibwBase[i].FibrMatlID  = rData.aFibwBase[i].FibrMatlID;
			aFibwBase[i].aPointY.Copy(rData.aFibwBase[i].aPointY);
			aFibwBase[i].aPointZ.Copy(rData.aFibwBase[i].aPointZ);
			ConvertStrChar(rData.aFibwBase[i].strRebarName, aFibwBase[i].strRebarName, sizeof(rData.aFibwBase[i].strRebarName));
		}
		memcpy(FimpKey, rData.FimpKey, sizeof(FimpKey));
		bMonitoredFiber = rData.bMonitoredFiber;
		memcpy(MonitoredFiber, rData.MonitoredFiber, sizeof(MonitoredFiber));
		nSeq = rData.nSeq;
		nAssignType = rData.nAssignType;
		memcpy(FimpColor, rData.FimpColor, sizeof(FimpColor));
	}
};
struct T_FIBW_D_CH_TEMP
{ 
	char   strName[40];
	int     nWallID;
	T_STOR_K StorK;
	CArray<T_FIBR_BASE_CH_RW, T_FIBR_BASE_CH_RW&> aFibwBase;
	CArray<T_FIBR_BASE_CH_RW, T_FIBR_BASE_CH_RW&> aFibwBaseJ;
	T_FIMP_K FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
															 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	CArray<T_FIBR_BASE_POS_CH_RW, T_FIBR_BASE_POS_CH_RW&> aTotalPoint;  // 각 fiber의 절점 array를 모두 모은 것 
	CArray<T_FIBR_BASE_POS_CH_RW, T_FIBR_BASE_POS_CH_RW&> aTotalPointJ;  // 각 fiber의 절점 array를 모두 모은 것 
	int    nSeq;
	int    nAssignType; // D_FIBW_TYPE
	COLORREF FimpColor[6];
	T_FIBW_D_CH_TEMP& operator=(const T_FIBW_D_CH_TEMP& src)
	{
		memcpy(strName, src.strName, sizeof(strName));
		nWallID = src.nWallID;
		StorK = src.StorK;
		aFibwBase.Copy(src.aFibwBase);
		aFibwBaseJ.Copy(src.aFibwBaseJ);
		memcpy(FimpKey, src.FimpKey, sizeof(FimpKey));
		memcpy(MonitoredFiber, src.MonitoredFiber, sizeof(MonitoredFiber));
		bMonitoredFiber = src.bMonitoredFiber;
		aTotalPoint.Copy(src.aTotalPoint);
		aTotalPointJ.Copy(src.aTotalPointJ);
		nSeq = src.nSeq;
		nAssignType = src.nAssignType;
		memcpy(FimpColor, src.FimpColor, sizeof(FimpColor));
		return *this;
	}

	void Set(T_FIBW_D_CH& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));    
		nWallID = rData.nWallID;
		StorK   = rData.StorK  ;
		for(int i = 0; i < 6; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];    
		aFibwBase.SetSize(rData.aFibwBase.GetSize());
		aTotalPoint.RemoveAll();
		for(int i = 0; i < rData.aFibwBase.GetSize(); i++)
		{
			aFibwBase[i].FibrBaseKey = rData.aFibwBase[i].FibrBaseKey;
			aFibwBase[i].bRebar      = rData.aFibwBase[i].bRebar;
			aFibwBase[i].dArea       = rData.aFibwBase[i].dArea;
			aFibwBase[i].dCenterY    = rData.aFibwBase[i].dCenterY;
			aFibwBase[i].dCenterZ    = rData.aFibwBase[i].dCenterZ;
			aFibwBase[i].FibrMatlID  = rData.aFibwBase[i].FibrMatlID;
			aFibwBase[i].nCountPoint = (int)rData.aFibwBase[i].aPointY.GetSize();
			memcpy(aFibwBase[i].strRebarName, rData.aFibwBase[i].strRebarName, sizeof(aFibwBase[i].strRebarName));
			for(int j = 0; j < rData.aFibwBase[i].aPointY.GetSize(); j++)
			{
				T_FIBR_BASE_POS_CH_RW TotalPos(rData.aFibwBase[i].aPointY[j], rData.aFibwBase[i].aPointZ[j]);
				aTotalPoint.Add(TotalPos);
			}      
		}
		nSeq = rData.nSeq;
		nAssignType = rData.nAssignType;
		memcpy(FimpColor, rData.FimpColor, sizeof(FimpColor));
	}
	void Get(T_FIBW_D_CH& rData)
	{
		memcpy(rData.strName, strName, sizeof(strName));    
		rData.nWallID = nWallID;
		rData.StorK   = StorK  ;
		for(int i = 0; i < 6; i++) rData.FimpKey[i] = FimpKey[i];
		rData.bMonitoredFiber = bMonitoredFiber;
		for(int i = 0; i < 8; i++) rData.MonitoredFiber[i] = MonitoredFiber[i];
		rData.aFibwBase.SetSize(aFibwBase.GetSize());
		int nBeginNum = 0;
		for(int i = 0; i < aFibwBase.GetSize(); i++)
		{
			rData.aFibwBase[i].FibrBaseKey = aFibwBase[i].FibrBaseKey;
			rData.aFibwBase[i].bRebar      = aFibwBase[i].bRebar;
			rData.aFibwBase[i].dArea       = aFibwBase[i].dArea;
			rData.aFibwBase[i].dCenterY    = aFibwBase[i].dCenterY;
			rData.aFibwBase[i].dCenterZ    = aFibwBase[i].dCenterZ;
			rData.aFibwBase[i].FibrMatlID  = aFibwBase[i].FibrMatlID;
			rData.aFibwBase[i].aPointY.RemoveAll();
			rData.aFibwBase[i].aPointZ.RemoveAll();
			memcpy(rData.aFibwBase[i].strRebarName, aFibwBase[i].strRebarName, sizeof(aFibwBase[i].strRebarName));
			for(int j = 0; j < aFibwBase[i].nCountPoint; j++)
			{
				rData.aFibwBase[i].aPointY.Add(aTotalPoint[nBeginNum].dPosY);
				rData.aFibwBase[i].aPointZ.Add(aTotalPoint[nBeginNum].dPosZ);
				nBeginNum++;
			}      
		}
		rData.nSeq = nSeq;
		rData.nAssignType = nAssignType;
		memcpy(rData.FimpColor, FimpColor, sizeof(FimpColor));
	}
};
struct T_FIBW_UDRD_D
{
	T_FIBW_K key;
	T_FIBW_D data;
};
struct T_FIBW_UDRD_D_CH
{
	T_FIBW_K key;
	T_FIBW_D_CH data;
};
struct T_FIBW_UDRD_D_CH_RW_865
{   
	T_FIBW_K key;
	char   strName[20];
	int     nWallID;
	T_STOR_K StorK;
	UINT   FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	// FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nCountFibwBase;
	int    nCountTotalPoint;
	int    nSeq;
	int    nAssignType; // D_FIBW_TYPE
	int    nTemp;              // Size 구별하기 위한 임시 변수
};

struct T_FIBW_UDRD_D_CH_RW_950
{
	T_FIBW_K key;
	char   strName[40];
	int     nWallID;
	T_STOR_K StorK;
	UINT   FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
	// FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nCountFibwBase;
	int    nCountTotalPoint;
	int    nSeq;
	int    nAssignType; // D_FIBW_TYPE
	int    nTotalArraySave;
};

struct T_FIBW_UDRD_D_CH_RW
{   
	T_FIBW_K key;
	char   strName[40];
	int     nWallID;
	T_STOR_K StorK;
	UINT   FimpKey[6];         // FimpKey[0]=Type1, FimpKey[1]=Type2, FimpKey[2]=Type3
														 // FimpKey[3]=Type4, FimpKey[4]=Type5, FimpKey[5]=Type6
	BOOL   bMonitoredFiber;
	UINT   MonitoredFiber[8];
	int    nCountFibwBase;
	int    nCountTotalPoint;
	int    nSeq;
	int    nAssignType; // D_FIBW_TYPE
	int    nTotalArraySave;              // 값이 1일 때는 FIBW 안에 있는 Array데이터를 다 모아서 한번에 저장하는 방식 사용(파일 저장 속도 때문)
	COLORREF FimpColor[6];
	void SetFibw(const T_FIBW_K Key, const T_FIBW_D_CH_TEMP& rData)
	{
		key = Key;
		memcpy(strName, rData.strName, sizeof(strName));    
		nWallID = rData.nWallID;
		StorK   = rData.StorK  ;
		for(int i = 0; i < 6; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];
		nCountFibwBase = (int)rData.aFibwBase.GetSize();
		nCountTotalPoint = (int)rData.aTotalPoint.GetSize();
		nSeq = rData.nSeq;   
		nAssignType = rData.nAssignType;
		nTotalArraySave = 1; // 값이 1일 때는 FIBW 안에 있는 Array데이터를 다 모아서 한번에 저장
		memcpy(FimpColor, rData.FimpColor, sizeof(FimpColor));
	}
	void GetFibw(T_FIBW_K& rKey, T_FIBW_D_CH_TEMP& rData)
	{
		rKey = key;
		memcpy(rData.strName, strName, sizeof(strName));    
		rData.nWallID = nWallID;
		rData.StorK   = StorK  ;
		for(int i = 0; i < 6; i++) rData.FimpKey[i] = FimpKey[i];
		rData.bMonitoredFiber = bMonitoredFiber;
		for(int i = 0; i < 8; i++) rData.MonitoredFiber[i] = MonitoredFiber[i];        
		rData.aFibwBase.RemoveAll();
		rData.aTotalPoint.RemoveAll();
		rData.nSeq = nSeq;       
		rData.nAssignType = nAssignType;
		memcpy(rData.FimpColor, FimpColor, sizeof(FimpColor));
	}
	void Convert865(T_FIBW_UDRD_D_CH_RW_865& rData)
	{
		key = rData.key;
		memset(strName,0,sizeof(strName));
		memcpy(strName, rData.strName, sizeof(rData.strName));    
		nWallID = rData.nWallID;
		StorK   = rData.StorK  ;
		for(int i = 0; i < 6; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for(int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];
		nCountFibwBase   = rData.nCountFibwBase  ;
		nCountTotalPoint = rData.nCountTotalPoint;
		nSeq             = rData.nSeq            ;
		nAssignType      = rData.nAssignType     ;
		nTotalArraySave  = rData.nTemp           ;
		FimpColor[0] = RGB(121, 198, 220);
		FimpColor[1] = RGB(245, 242, 31);
		FimpColor[2] = RGB(214, 126, 176);
		FimpColor[3] = RGB(63, 174, 179);
		FimpColor[4] = RGB(40, 130, 89);
		FimpColor[5] = RGB(223, 176, 192);
	}
	void Convert950(T_FIBW_UDRD_D_CH_RW_950& rData)
	{
		key = rData.key;
		memset(strName, 0, sizeof(strName));
		memcpy(strName, rData.strName, sizeof(rData.strName));
		nWallID = rData.nWallID;
		StorK = rData.StorK;
		for (int i = 0; i < 6; i++) FimpKey[i] = rData.FimpKey[i];
		bMonitoredFiber = rData.bMonitoredFiber;
		for (int i = 0; i < 8; i++) MonitoredFiber[i] = rData.MonitoredFiber[i];
		nCountFibwBase = rData.nCountFibwBase;
		nCountTotalPoint = rData.nCountTotalPoint;
		nSeq = rData.nSeq;
		nAssignType = rData.nAssignType;
		nTotalArraySave = rData.nTotalArraySave;
		FimpColor[0] = RGB(121, 198, 220);
		FimpColor[1] = RGB(245, 242, 31);
		FimpColor[2] = RGB(214, 126, 176);
		FimpColor[3] = RGB(63, 174, 179);
		FimpColor[4] = RGB(40, 130, 89);
		FimpColor[5] = RGB(223, 176, 192);
	}
};
struct T_FIBW_UNIT
{
	T_FIBW_UNIT()
	{
		dPoint = D_UNITSYS_BASE_LENGTH;
		dArea  = D_UNITSYS_BASE_AREA;    
	}
	int dPoint;
	int dArea;  
};
#define HASHSIZEFIBW 1001

// Select Inelastic Hinge Result Output - Element
#define T_SIHO_K T_ELEM_K
struct T_SIHO_D
{  
	BOOL bFiberResult; // Fiber Section Result
	T_SIHO_D() { Initialize(); }
	void Initialize()
	{
		bFiberResult = FALSE;
	}
};
struct T_SIHO_UDRD_D
{
	T_SIHO_K key;
	T_SIHO_D data;
};
struct T_SIHO_UNIT
{
	T_SIHO_UNIT() {}
};
#define HASHSIZESIHO HASHSIZEIEHG

// Select Inelastic Hinge Result Output - General Link
#define T_SIHG_K T_NLNK_K
struct T_SIHG_D
{  
	BOOL bDummy;
	T_SIHG_D() { Initialize(); }
	void Initialize()
	{
		bDummy = FALSE;
	}
};
struct T_SIHG_UDRD_D
{
	T_SIHG_K key;
	T_SIHG_D data;
};
struct T_SIHG_UNIT
{
	T_SIHG_UNIT() {}
};
#define HASHSIZESIHG HASHSIZENLNK

// Select Inelastic Hinge Result Output - Point Spring Support
#define T_SIHP_K T_NODE_K
struct T_SIHP_D
{
	BOOL bDummy;
	T_SIHP_D() { Initialize(); }
	void Initialize()
	{
		bDummy = FALSE;
	}
};
struct T_SIHP_UDRD_D
{
	T_SIHP_K key;
	T_SIHP_D data;
};
struct T_SIHP_UNIT
{
	T_SIHP_UNIT() {}
};
#define HASHSIZESIHP HASHSIZENODE

#pragma pack(pop)

#endif  // __DB_ST_DT_ITHA_H__
