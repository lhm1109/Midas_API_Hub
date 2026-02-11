#pragma once
#define MIDAS_SDK

#pragma pack(push, 1)
#pragma warning ( disable : 4819 )
#pragma warning ( disable : 4244 )

/************************************************************************/
/* Modify nonlinear deformation model of concrete:T_MNDC_D				*/
/************************************************************************/
#define T_MNDC_K T_ELEM_K
#define HASHSIZEMNDC HASHSIZEELEM

struct T_MNDC_D_950_OLD_1
{
	int nDiagramType;
	int nParametersType;
	double dCompression[6];
	double dTension[6];
};
struct T_MNDC_UDRD_D_950_OLD_1
{
	T_MNDC_K key;
	T_MNDC_D_950_OLD_1 data;
};


struct T_MNDC_D
{
	// 0: Two-line stress-strain diagram; 1:Three-line stress-strain diagram
	int nDiagramType;
	// DISCARD ---crack is possible--- 
	BOOL bCrackPossible;
	// 0: by materail; 1: user define
	int nParametersType;
	//
	double dCompression[6];
	double dTension[6];

	void Initialize()
	{
		nDiagramType = 0;
		bCrackPossible = TRUE;
		nParametersType = 0;
		for (int i = 0; i < 6; i++)
		{
			dCompression[i] = 0.0;
			dTension[i] = 0.0;
		}
	}
	T_MNDC_D()
	{
		Initialize();
	}
	T_MNDC_D(const T_MNDC_D& src)
	{
		*this = src;
	}
	T_MNDC_D& T_MNDC_D::operator=(const T_MNDC_D& src)
	{
		nDiagramType = src.nDiagramType;
		bCrackPossible = src.bCrackPossible;
		nParametersType = src.nParametersType;
		for (int i = 0; i < 6; i++)
		{
			dCompression[i] = src.dCompression[i];
			dTension[i] = src.dTension[i];
		}
		return *this;
	}
	void Convert950_OLD_1(T_MNDC_D_950_OLD_1& rData)
	{
		nDiagramType = rData.nDiagramType;
		bCrackPossible = TRUE;
		nParametersType = rData.nParametersType;
		for (int i = 0; i < 6; i++)
		{
			dCompression[i] = rData.dCompression[i];
			dTension[i] = rData.dTension[i];
		}
	}
};
struct T_MNDC_UDRD_D
{
	T_MNDC_K key;
	T_MNDC_D data;
};




/************************************************************************/
/*      Seismic accounting for member (RC) : T_SAFM_D					*/
/************************************************************************/
#define T_SAFM_K T_ELEM_K
#define HASHSIZESAFM HASHSIZEELEM
struct T_SAFM_D
{
	double dM_tr1;
	double dM_tr2;

	void Initialize()
	{
		dM_tr1 = 1.2;
		dM_tr2 = 1.0;
	}
	T_SAFM_D() { Initialize(); }
	T_SAFM_D(const T_SAFM_D& src) { *this = src; }
	T_SAFM_D& T_SAFM_D::operator=(const T_SAFM_D&src)
	{
		dM_tr1 = src.dM_tr1;
		dM_tr2 = src.dM_tr2;
		return *this;
	}
};
struct T_SAFM_UDRD_D
{
	T_SAFM_K key;
	T_SAFM_D data;
};

#pragma pack(pop)