#if !defined(__StldBtmpDefCodeProvUtil_H__)
#define __StldBtmpDefCodeProvUtil_H__

#define D_BTMP_CODE_SNIP_2050384		0
#define D_BTMP_CODE_SP_35_13330_2011	1
#define D_BTMP_CODE_JTG_D60_2015		2
#define D_BTMP_CODE_IRC6_2017		    3
#define D_BTMP_CODE_AASHTO_LRFD_2020	4
#define D_BTMP_CODE_EUROCODE	    	5
#define D_BTMP_CODE_AS       	    	6

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DBDoc.h"

struct BTMP_DEF_CODE_PROV_SNIP2050284_D
{
	BTMP_DEF_CODE_PROV_SNIP2050284_D()
	{
		Initialize();
	}
	void Initialize()
	{
		nSectType = 0;
		nTempGrad = 0;
		dTMax = 30;
	}

	int nSectType; // 0 : Steel Composite
	int nTempGrad; // 0:Type1, 1:Type2
	double dTMax;
};

struct BTMP_DEF_CODE_PROV_JTG_2015_2018_D
{
	BTMP_DEF_CODE_PROV_JTG_2015_2018_D()
	{
		Initialize();
	}
	void Initialize()
	{
		nSectType = 0;
		dT1 = 14.0;
		dT2 = 5.5;
		dT1_Dumb = 8.0;
		dT2_Dumb = 6.0;
		dSmallT = 150;
		nDumbType = 0;
	}
	int nSectType; // 0 : PSC, 1 : Steel Composite
	double dT1;
	double dT2;
	double dT1_Dumb;
	double dT2_Dumb;
	double dSmallT; // mm 단위로 고정
	int nDumbType;  // 0: Single Tube 1:Dumbbell
};

struct BTMP_DEF_CODE_PROV_IRC6_2017_D
{
    BTMP_DEF_CODE_PROV_IRC6_2017_D()
    {
        Initialize();
    }
    void Initialize()
    {
        nSectType = 0;
        nTempGrad = 0;
        dT1 = 17.8;
        dT2 = 4.0;
        dT3 = 0.0;
        dT4 = 2.1;
        
    }

    int nSectType; // 0 : PSC, 1 : Steel Composite
    int nTempGrad; // 0:Positive Temperature Gradient, 1:Reverse Temperature Gradient
    double dT1;
    double dT2;
    double dT3;
    double dT4;
    
};

struct BTMP_DEF_CODE_PROV_AASHTO_2020_D
{
	BTMP_DEF_CODE_PROV_AASHTO_2020_D()
	{
		Initialize();
	}
	void Initialize()
	{
		nSectType = 0;
		nTempGrad = 0;
		dT1 = 54.0;   // F 단위로 고정
		dT2 = 14.0;   // F 단위로 고정
		dT3 =  5.0;   // F 단위로 고정
		dSmallT = 150;
		nSmallT = 0;
		dA = 12;
		nA = 0;
		
	}
	int nSectType; // 0 : PSC, 1 : Steel Composite
	int nTempGrad;  // 0 :Positive 1 : Negative 
	double dT1;
	double dT2;
	double dT3;
	double dSmallT; // in. 단위로 고정
	int nSmallT;    // 0: Auto  1: User
	double dA;      // in. 단위로 고정
 	int  nA;        // 0: Auto  1: User  
};

struct BTMP_DEF_CODE_PROV_EUROCODE_D
{
	BTMP_DEF_CODE_PROV_EUROCODE_D()
	{
		Initialize();
	}
	void Initialize()
	{
		nSectType = 0;
		nTempGrad = 0;
		nProcedure = 0;
		dT1 = 0.0;
		dT2 = 0.0;
		dT3 = 0.0;
		dT4 = 0.0;
		dH0 = 0.0;
		dH1 = 0.0;
		dH2 = 0.0;
		dH3 = 0.0;
		dH4 = 0.0;
		nH0 = 0;
		nH1 = 0;
		nH2 = 0;
		nH3 = 0;
		nH4 = 0;
	}
	int nSectType;  // 0 : PSC,    1 : Steel Composite
	int nTempGrad;  // 0 : Heat    1 : Cool
	int nProcedure; // 0 : Normal, 1 : Simplified
	double dT1;
	double dT2;
	double dT3;
	double dT4;
	double dH0;
	double dH1;
	double dH2;
	double dH3;
	double dH4;
	int  nH0;        // 0: Auto  1: User  
	int  nH1;        // 0: Auto  1: User  
	int  nH2;        // 0: Auto  1: User  
	int  nH3;        // 0: Auto  1: User  
	int  nH4;        // 0: Auto  1: User  
};

struct BTMP_DEF_CODE_PROV_AS_D
{
	BTMP_DEF_CODE_PROV_AS_D()
	{
		Initialize();
	}
	void Initialize()
	{
		nSectType = 0;
		nTempGrad = 0;
		dSmallT = 300.0;
		dT1 = 0.0;
		dT2 = 0.0;
		dH1 = 0.0;
		dH2 = 0.0;
		nSmallT = 0;
		nH1 = 0;
		nH2 = 0;
	}
	int nSectType;  // 0 : PSC,   1:Steel Composite 
	int nTempGrad;  // 0 : Positive 1 : Negative
	double dSmallT; // mm. 단위로 고정
	double dT1;
	double dT2;
	double dH1;
	double dH2;
	int nSmallT;    // 0: Auto  1: User
	int  nH1;        // 0: Auto  1: User  
	int  nH2;        // 0: Auto  1: User  
};


struct BTMP_DEF_CODE_PROV_D
{  
	BTMP_DEF_CODE_PROV_D()
	{
		Initialize();
	}
	void Initialize()
	{
		nDgnCode = D_BTMP_CODE_SNIP_2050384;
		Snip2050284.Initialize();
		Jtg2015_2018.Initialize();
        IRC6_2017.Initialize();
		AASHTO_2020.Initialize();
		EuroCode.Initialize();
		AsCode.Initialize();
	}

	int nDgnCode;
	BTMP_DEF_CODE_PROV_SNIP2050284_D Snip2050284;
	BTMP_DEF_CODE_PROV_JTG_2015_2018_D Jtg2015_2018;
    BTMP_DEF_CODE_PROV_IRC6_2017_D  IRC6_2017;
	BTMP_DEF_CODE_PROV_AASHTO_2020_D AASHTO_2020;
	BTMP_DEF_CODE_PROV_EUROCODE_D  EuroCode;
	BTMP_DEF_CODE_PROV_AS_D  AsCode;
};

class CStldBtmpDefCodeProvUtil
{
public:
 CStldBtmpDefCodeProvUtil();
 static BOOL AddBtmpByCodeProv(T_BTMP_D& BtmpD, BTMP_DEF_CODE_PROV_D& InputData, CArray<UINT, UINT>& aElemKey, BOOL bReplace );

protected:
 static BOOL CheckSectType(BTMP_DEF_CODE_PROV_D& InputData, CArray<UINT, UINT>& aElemKey);
 static BOOL MakeBtmpByCode(T_BTMP_D& BtmpD, BTMP_DEF_CODE_PROV_D& InputData, CArray<UINT, UINT>& aElemKey, CArray<T_BTMP_D, T_BTMP_D&>& aGenBtmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpByCode( BTMP_DEF_CODE_PROV_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, T_BTMP_D& BtmpD, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp , CArray<CString>& aErrorMsg);
 static BOOL MakeStmpSnip2050384Compo( BTMP_DEF_CODE_PROV_SNIP2050284_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpJTGD602015( BTMP_DEF_CODE_PROV_JTG_2015_2018_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpJTGD602015PSC( BTMP_DEF_CODE_PROV_JTG_2015_2018_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpJTGD602015Stlg( BTMP_DEF_CODE_PROV_JTG_2015_2018_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpJTGD602015Dumb(BTMP_DEF_CODE_PROV_JTG_2015_2018_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpJTGD602015Stub(BTMP_DEF_CODE_PROV_JTG_2015_2018_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL FuncSnip2050384CompoType1(double dTmax, double dZ, double dHw, double& dOut);
 static BOOL FuncSnip2050384CompoType2(double dTmax, double dZ, double& dOut); 
 static BOOL MakeStmpIRC62017( BTMP_DEF_CODE_PROV_IRC6_2017_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpIRC62017PSC( BTMP_DEF_CODE_PROV_IRC6_2017_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpIRC62017Stlg( BTMP_DEF_CODE_PROV_IRC6_2017_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpAASHTO2020    ( BTMP_DEF_CODE_PROV_AASHTO_2020_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpAASHTO2020PSC ( BTMP_DEF_CODE_PROV_AASHTO_2020_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpAASHTO2020PSCOld(BTMP_DEF_CODE_PROV_AASHTO_2020_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpAASHTO2020Stlg( BTMP_DEF_CODE_PROV_AASHTO_2020_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpEurocode    ( BTMP_DEF_CODE_PROV_EUROCODE_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpEurocodePSC ( BTMP_DEF_CODE_PROV_EUROCODE_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpEurocodeStlg( BTMP_DEF_CODE_PROV_EUROCODE_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpAs    ( BTMP_DEF_CODE_PROV_AS_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpAsPSCPostive( BTMP_DEF_CODE_PROV_AS_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpAsPSCNegative ( BTMP_DEF_CODE_PROV_AS_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpAsStlgPostive( BTMP_DEF_CODE_PROV_AS_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL MakeStmpAsStlgNegative( BTMP_DEF_CODE_PROV_AS_D& InputData, T_ELEM_K ElemK, T_SECT_D& SectD, int nBtmpType, CArray<T_BTMP_STMP, T_BTMP_STMP&>& GenStmp, CArray<CString>& aErrorMsg);
 static BOOL FuncAsPscPositive(double dTmax, double dZ, double d1200mm,  double& dOut);



 static int GetBestDivNumSnip2050384Compo(double dHw, double dBestSize, int nMaxSize);
 static double MakeStmpIRC62017Stlg_PosGrd_Part1(double dSlabDepth);
 static double MakeStmpIRC62017Stlg_PosGrd_Part2(double dstartht,double dendht,double dSlabDepth);
 static double MakeStmpIRC62017Stlg_NegGrd_Part1(double dendht,double dSlabDepth);
 static double MakeStmpIRC62017Stlg_NegGrd_Part2(double dstartht,double dendht,double dSlabDepth);
};

#endif // !defined(__StldBtmpDefCodeProvUtil_H__)


