#ifndef __DB_ST_DT_ERPR_H__
#define __DB_ST_DT_ERPR_H__

#define MIDAS_SDK

#pragma pack(push, 1)

enum { SELECT_GRUP = 1, SELECT_ELEM = 2 };

#define T_POSP_K unsigned int
//Parameters of Soil Properties
enum { POSP_BASE_DATA_NUM = 7 }; //Base Data 변수 추가시 개수 올려줄 것

struct T_POSP_BASE_DATA_900
{
	double dHeight;
	double dAngleOrN;
	double dDensity;
	double dVs;
	double dKh;
	void Initialize()
	{
		dHeight = 1.0;
		dAngleOrN = 30.0;
		dDensity = 18000.0;
		dVs = 100.0;
		dKh = 4082000.0;
	}
	
};

struct T_POSP_BASE_DATA_930
{
	double dHeight;
	double dAngleOrN;
	double dDensity;
	double dVs;
	double dKh;
	double dDisp;

	void Initialize()
	{
		dHeight = 1.0;
		dAngleOrN = 30.0;
		dDensity = 18000.0;
		dVs = 100.0;
		dKh = 4082000.0;
		dDisp = 0.001;
	}
};

struct T_POSP_BASE_DATA
{
	double dHeight;
	double dAngleOrN;
	double dDensity;
	double dPoissonRatio;
	double dVs;
	double dKh;
	double dDisp;

	void Initialize()
	{
		dHeight = 1.0;
		dAngleOrN = 30.0;
		dDensity = 18000.0;
		dPoissonRatio = 0;
		dVs = 100.0;
		dKh = 4082000.0;
		dDisp = 0.001;
	}
	T_POSP_BASE_DATA()
	{
		Initialize();
	}
	void Convert900(T_POSP_BASE_DATA_900& rData)
	{
		dHeight = rData.dHeight;
		dAngleOrN = rData.dAngleOrN;
		dDensity = rData.dDensity;
		dVs = rData.dVs;
		dKh = rData.dKh;
	}
	void Convert930(T_POSP_BASE_DATA_930& rData)
	{
		dHeight = rData.dHeight;
		dAngleOrN = rData.dAngleOrN;
		dDensity = rData.dDensity;
		dVs = rData.dVs;
		dKh = rData.dKh;
		dDisp = rData.dDisp;
	}
};

struct T_POSP_D_900
{
	CString strName;
	CString strDescription;
	BOOL bUseN;
	CArray<T_POSP_BASE_DATA_900, T_POSP_BASE_DATA_900&> aSoilParam;
	double dGroundLevel;
	double dBedrockLevel;
	double dFootingLevel;
};

struct T_POSP_D_930
{
	CString strName;
	CString strDescription;
	BOOL bUseN;
	CArray<T_POSP_BASE_DATA_930, T_POSP_BASE_DATA_930&> aSoilParam;
	double dGroundLevel;
	double dBedrockLevel;
	double dFootingLevel;
};

struct T_POSP_D
{
	CString strName;
	CString strDescription;
	BOOL bUseN;
	CArray<T_POSP_BASE_DATA, T_POSP_BASE_DATA&> aSoilParam;
	double dGroundLevel;
	double dBedrockLevel;
	double dFootingLevel;
	double dStructureWidth;
	double dStructureHeight;

	void Initialize()
	{
		strName.Empty();
		strDescription.Empty();
		bUseN = FALSE;
		aSoilParam.RemoveAll();
		dGroundLevel = 0.0;
		dBedrockLevel = 0.0;
		dFootingLevel = 0.0;
		dStructureWidth = 10;
		dStructureHeight = 10;
	}
	T_POSP_D()
	{
		Initialize();
	}
	T_POSP_D(T_POSP_D& src)
	{
		*this = src;
	}
	T_POSP_D& operator=(T_POSP_D& src)
	{
		strName				= src.strName;
		strDescription		= src.strDescription;
		bUseN				= src.bUseN;
		aSoilParam.Copy(src.aSoilParam);
		dGroundLevel		= src.dGroundLevel;
		dBedrockLevel		= src.dBedrockLevel;
		dFootingLevel		= src.dFootingLevel;
		dStructureWidth		= src.dStructureWidth;
		dStructureHeight		= src.dStructureHeight;
		return *this;
	}
	void Convert900(T_POSP_D_900& rData)
	{
		strName = rData.strName;
		strDescription = rData.strDescription;
		bUseN = rData.bUseN;
		dGroundLevel = rData.dGroundLevel;
		dBedrockLevel = rData.dBedrockLevel;
		dFootingLevel = rData.dFootingLevel;
		aSoilParam.RemoveAll();
		for (int i = 0; i < rData.aSoilParam.GetSize(); i++)
		{
			T_POSP_BASE_DATA SoilBase;
			SoilBase.Convert900(rData.aSoilParam[i]);
			aSoilParam.Add(SoilBase);
		}
	}
	void Convert930(T_POSP_D_930& rData)
	{
		strName = rData.strName;
		strDescription = rData.strDescription;
		bUseN = rData.bUseN;
		dGroundLevel = rData.dGroundLevel;
		dBedrockLevel = rData.dBedrockLevel;
		dFootingLevel = rData.dFootingLevel;
		aSoilParam.RemoveAll();
		for (int i = 0; i < rData.aSoilParam.GetSize(); i++)
		{
			T_POSP_BASE_DATA SoilBase;
			SoilBase.Convert930(rData.aSoilParam[i]);
			aSoilParam.Add(SoilBase);
		}
	}
};

struct T_POSP_UDRD_D
{
	T_POSP_K key;
	T_POSP_D data;
};

#define D_POSP_STR_MAX_LENGTH 80
struct T_POSP_UDRD_D_CH_RW_930
{
	T_POSP_K key;
	char strName[D_POSP_STR_MAX_LENGTH];
	char strDescription[D_POSP_STR_MAX_LENGTH];
	BOOL bUseN;
	int arKeysSoilParam;
	double dGroundLevel;
	double dBedrockLevel;
	double dFootingLevel;
	
};

struct T_POSP_UDRD_D_CH_RW
{
	T_POSP_K key;
	char strName[D_POSP_STR_MAX_LENGTH];
	char strDescription[D_POSP_STR_MAX_LENGTH];
	BOOL bUseN;
	int arKeysSoilParam;
	double dGroundLevel;
	double dBedrockLevel;
	double dFootingLevel;
	double dStructureWidth;
	double dStructureHeight;

	void GetPosp(T_POSP_K& rKey, T_POSP_D& rData)
	{
		rKey = key;
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDescription, rData.strDescription, sizeof(strDescription));
		rData.bUseN = bUseN;
		rData.aSoilParam.SetSize(arKeysSoilParam);
		rData.dGroundLevel = dGroundLevel;
		rData.dBedrockLevel = dBedrockLevel;
		rData.dFootingLevel = dFootingLevel;
	}
	void SetPosp(const T_POSP_K tKey, T_POSP_D& rData)
	{
		key = tKey;
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDescription, strDescription, sizeof(strDescription));
		bUseN = rData.bUseN;
		arKeysSoilParam = (int)rData.aSoilParam.GetSize();
		dGroundLevel = rData.dGroundLevel;
		dBedrockLevel = rData.dBedrockLevel;
		dFootingLevel = rData.dFootingLevel;
	}
	void Convert930(T_POSP_UDRD_D_CH_RW_930& rData)
	{
		key = rData.key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		bUseN = rData.bUseN;
		arKeysSoilParam = rData.arKeysSoilParam;
		dGroundLevel = rData.dGroundLevel;
		dBedrockLevel = rData.dBedrockLevel;
		dFootingLevel = rData.dFootingLevel;
	}
};

struct T_POSP_UNIT
{
	T_POSP_UNIT()
	{
		strName				= D_UNITSYS_NONE;
		strDescription		= D_UNITSYS_NONE;
		bUseN				= D_UNITSYS_NONE;
		dHeight				= D_UNITSYS_BASE_LENGTH;
		dAngleOrN			= D_UNITSYS_BASE_DEGREE;
		dDensity			= D_UNITSYS_BASE_DENSITY;
		dPoissonRatio		= D_UNITSYS_NONE;
		dVs					= D_UNITSYS_BASE_VELOCITY;
		dKh					= D_UNITSYS_BASE_DENSITY;
		dDisp				= D_UNITSYS_BASE_LENGTH;
		dGroundLevel		= D_UNITSYS_BASE_LENGTH;
		dBedrockLevel		= D_UNITSYS_BASE_LENGTH;
		dFootingLevel		= D_UNITSYS_BASE_LENGTH;
		dStructureWidth		= D_UNITSYS_BASE_LENGTH;
		dStructureHeight	= D_UNITSYS_BASE_LENGTH;
	}
	int strName;
	int strDescription;
	int bUseN;
	int dHeight;
	int dAngleOrN;
	int dDensity;
	int dPoissonRatio;
	int dVs;
	int dKh;
	int dDisp;
	int dGroundLevel;
	int dBedrockLevel;
	int dFootingLevel;
	int dStructureWidth;
	int dStructureHeight;
};
#define HASHSIZEPOSP 101

struct T_EPST_PRESSURE_PROFILE
{
	double dLevel;
	double dSoilPressure;
	double dAdditionalPressure;

	BOOL operator==(const T_EPST_PRESSURE_PROFILE& src) const
	{
		if (fabs(dLevel - src.dLevel) > 1.e-10) return FALSE;
		if (fabs(dSoilPressure - src.dSoilPressure) > 1.e-10) return FALSE;
		if (fabs(dAdditionalPressure - src.dAdditionalPressure) > 1.e-10) return FALSE;
		return TRUE;
	}

};

#define T_EPST_K unsigned int
//Static Earth Pressure
struct T_EPST_D
{
	T_STLD_K KeyStld; //Load Case Name
	int		 nDirection;   //1=X-Y, 2=Normal
	double	 dAngle;
	double	 dInnerPnt[3];   // 공간구조의 경우 Nomal을 구하기위한 좌표.
	double	 dScaleFactor;
	int		 nEarthPressureType;
	double	 dSurchargeLoad;
	double	 dWaterLevel;
	T_POSP_K KeyPosp;		//Parameters of Soil Properties
	int		 nSelection;    // 1=Group, 2=Element
	T_LAPL_K KeyLapl;       // 결국 DB에는 Lapl단위로 저장이 된다....
	CArray<T_ELEM_K, T_ELEM_K> arKeysDefineElem;
	CArray<T_NODE_K, T_NODE_K> arKeysDefineNode;
	CArray<T_EPST_PRESSURE_PROFILE, T_EPST_PRESSURE_PROFILE&> arPresProfile;

	void Initialize()
	{
		KeyStld = 0;
		nDirection = 1;
		dAngle = 0.0;
		dInnerPnt[0] = 0.0;
		dInnerPnt[1] = 0.0;
		dInnerPnt[2] = 0.0;
		dScaleFactor = 1.0;
		nEarthPressureType = 0;
		dSurchargeLoad = 0.0;
		dWaterLevel = 0.0;
		KeyPosp = 0;
		nSelection = SELECT_ELEM;
		KeyLapl = 0;
		arKeysDefineElem.RemoveAll();
		arKeysDefineNode.RemoveAll();
		arPresProfile.RemoveAll();
	}
	T_EPST_D()
	{
		Initialize();
	}
	T_EPST_D(const T_EPST_D& src)
	{
		*this = src;
	}
	T_EPST_D& operator=(const T_EPST_D& src)
	{
		KeyStld = src.KeyStld;
		nDirection = src.nDirection;
		dAngle = src.dAngle;
		dInnerPnt[0] = src.dInnerPnt[0];
		dInnerPnt[1] = src.dInnerPnt[1];
		dInnerPnt[2] = src.dInnerPnt[2];
		dScaleFactor = src.dScaleFactor;
		nEarthPressureType = src.nEarthPressureType;
		dSurchargeLoad = src.dSurchargeLoad;
		dWaterLevel = src.dWaterLevel;
		KeyPosp = src.KeyPosp;
		nSelection = src.nSelection;
		KeyLapl = src.KeyLapl;
		arKeysDefineElem.Copy(src.arKeysDefineElem);
		arKeysDefineNode.Copy(src.arKeysDefineNode);
		arPresProfile.Copy(src.arPresProfile);
		return *this;
	}
};

struct T_EPST_UDRD_D_RW
{
	T_EPST_K key;
	T_STLD_K KeyStld; //Load Case Name
	int		 nDirection;   //1=X-Y, 2=Normal
	double	 dAngle;
	double	 dInnerPnt[3];   // 공간구조의 경우 Nomal을 구하기위한 좌표.
	double	 dScaleFactor;
	int		 nEarthPressureType;
	double	 dSurchargeLoad;
	double	 dWaterLevel;
	T_POSP_K KeyPosp;		//Parameters of Soil Properties
	int		 nSelection;    // 1=Group, 2=Element
	T_LAPL_K KeyLapl;       // 결국 DB에는 Lapl단위로 저장이 된다....
	int arKeysDefineElem;
	int arKeysDefineNode;
	int arPresProfile;

	void GetEpst(T_EPST_K& rKey, T_EPST_D& rData)
	{
		rKey = key;
		rData.KeyStld = KeyStld;
		rData.nDirection = nDirection;
		rData.dAngle = dAngle;
		rData.dInnerPnt[0] = dInnerPnt[0];
		rData.dInnerPnt[1] = dInnerPnt[1];
		rData.dInnerPnt[2] = dInnerPnt[2];
		rData.dScaleFactor = dScaleFactor;
		rData.nEarthPressureType = nEarthPressureType;
		rData.dSurchargeLoad = dSurchargeLoad;
		rData.dWaterLevel = dWaterLevel;
		rData.KeyPosp = KeyPosp;
		rData.nSelection = nSelection;
		rData.KeyLapl = KeyLapl;
	}
	void SetEpst(const T_EPST_K tKey, const T_EPST_D& rData)
	{
		key = tKey;
		KeyStld = rData.KeyStld;
		nDirection = rData.nDirection;
		dAngle = rData.dAngle;
		dInnerPnt[0] = rData.dInnerPnt[0];
		dInnerPnt[1] = rData.dInnerPnt[1];
		dInnerPnt[2] = rData.dInnerPnt[2];
		dScaleFactor = rData.dScaleFactor;
		nEarthPressureType = rData.nEarthPressureType;
		dSurchargeLoad = rData.dSurchargeLoad;
		dWaterLevel = rData.dWaterLevel;
		KeyPosp = rData.KeyPosp;
		nSelection = rData.nSelection;
		KeyLapl = rData.KeyLapl;
		arKeysDefineElem = (int)rData.arKeysDefineElem.GetSize();
		arKeysDefineNode = (int)rData.arKeysDefineNode.GetSize();
		arPresProfile = (int)rData.arPresProfile.GetSize();
	}
};

struct T_EPST_UDRD_D
{
	T_EPST_K key;
	T_EPST_D data;
};

struct T_EPST_UNIT
{
	T_EPST_UNIT()
	{
		dInnerPnt							= D_UNITSYS_BASE_LENGTH;
		dScaleFactor						= D_UNITSYS_NONE;
		dSurchargeLoad						= D_UNITSYS_BASE_STRESS;
		dWaterLevel							= D_UNITSYS_BASE_LENGTH;
		arPresProfile_dLevel				= D_UNITSYS_BASE_LENGTH;
		arPresProfile_dSoilPressure			= D_UNITSYS_BASE_PRESSURE;
		arPresProfile_dAdditionalPressure	= D_UNITSYS_BASE_PRESSURE;
	}

	int	dInnerPnt;   
	int	dScaleFactor;
	int	dSurchargeLoad;
	int	dWaterLevel;
	int arPresProfile_dLevel;
	int arPresProfile_dSoilPressure;
	int arPresProfile_dAdditionalPressure;
};
#define HASHSIZEEPST 101

enum EN_EPSE_METHOD
{
	EN_EPSE_METHOD_EQUIVALENT = 0, EN_EPSE_METHOD_RESPONSE,
};
#define T_POSL_K unsigned int
// Parameter of Seismic Loads
struct T_POSL_KDS2019
{
	int	   nEPSEMethod;			// Method => 0: Equivalent Static Method, 1: Double Cosine Method(R.D.M)
	int    nSeismicZone;		// 0:1, 1:2
	double dEPA;				// 유효지반 가속도
	int    nSiteClass;			// 0:S1, 1:S2, 2:S3, 3:S4, 4:S5, 5:S6
	double dFa;					// Acceleration-based site coefficient
	double dFv;					// Velocity-based site coefficient
	double dSds;				// Design Spectral Response Acc. at short periods
	double dSd1;				// Design Spectral Response Acc. at 1 s periods
	int    nSeismicUseGroup;	// 0=Special, 1=I, 2=II
	double dImportanceFactor;	// 1.0, 1.2, 1.5  
	double dResponseModifyFactor;  // 1.5, 2.5, 3, 3.25, 4.5, 5, 5.5, 6, 6.5, 7, 8

	void Initialize()
	{

		nEPSEMethod = EN_EPSE_METHOD_RESPONSE;
		nSeismicZone = 0;
		dEPA = 0.22;
		nSiteClass = 0;
		dFa = 1.12;
		dFv = 0.84;
		dSds = 0.40167;
		dSd1 = 0.12320;
		nSeismicUseGroup = 1;
		dImportanceFactor = 1.2;
		dResponseModifyFactor = 3.0;
	}
};

struct T_POSL_KDS2018
{
	int    nSeismicZone;		// 0:Zone1(0.11), 1:Zone2(0.07)
	double dSeismicRiskFactor;  // Importance
	int    nSiteClass;			// 0:S1, 1:S2, 2:S3, 3:S4, 4:S5, 5:S6
	double dFa;					// Acceleration-based site coefficient
	double dFv;					// Velocity-based site coefficient
	double dDampingRatio;

	void Initialize()
	{
		nSeismicZone = 0;
		dSeismicRiskFactor = 0.4;
		nSiteClass = 0;
		dFa = 1.12;
		dFv = 0.84;
		dDampingRatio = 0;
	}
};

#define T_POSL_K unsigned int
// Parameter of Seismic Loads
struct T_POSL_D_925 
{	
	CString strName;
	int    nCodeType;			// KDS_E_2019 밖에 없음
	int	   nEPSEMethod;			// Method => 0: Equivalent Static Method, 1: Double Cosine Method(R.D.M)
	int    nSeismicZone;		// 0:1, 1:2
	double dEPA;				// 유효지반 가속도
	int    nSiteClass;			// 0:S1, 1:S2, 2:S3, 3:S4, 4:S5, 5:S6
	double dFa;					// Acceleration-based site coefficient
	double dFv;					// Velocity-based site coefficient
	double dSds;				// Design Spectral Response Acc. at short periods
	double dSd1;				// Design Spectral Response Acc. at 1 s periods
	int    nSeismicUseGroup;	// 0=Special, 1=I, 2=II
	double dImportanceFactor;	// 1.0, 1.2, 1.5  
	double dResponseModifyFactor;  // 1.5, 2.5, 3, 3.25, 4.5, 5, 5.5, 6, 6.5, 7, 8
};
struct T_POSL_D
{	
	CString strName;
	int    nCodeType;			// KDS_E_2019, KDS_E_2019 밖에 없음
	T_POSL_KDS2018 KDS2018;
	T_POSL_KDS2019 KDS2019;

	void Initialize()
	{
		strName.Empty();
		nCodeType = 0;
		KDS2018.Initialize();
		KDS2019.Initialize();
	}
	T_POSL_D()
	{
		Initialize();
	}
	T_POSL_D(T_POSL_D& src)
	{
		*this = src;
	}
	T_POSL_D& operator=(T_POSL_D& src)
	{
		strName = src.strName;
		nCodeType = src.nCodeType;
		KDS2019 = src.KDS2019;
		KDS2018 = src.KDS2018;
		return *this;
	}
};

struct T_POSL_UDRD_D
{
	T_POSL_K key;
	T_POSL_D data;
};

struct T_POSL_UDRD_D_925
{
	T_POSL_K key;
	T_POSL_D_925 data;
};

#define D_POSL_STR_MAX_LENGTH 80
struct T_POSL_UDRD_D_CH_925
{
	T_POSL_K key;
	char strName[D_POSL_STR_MAX_LENGTH];
	int    nCodeType;			// KDS_E_2019 밖에 없음
	int	   nEPSEMethod;			// Method => 0: Equivalent Static Method, 1: Double Cosine Method(R.D.M)
	int    nSeismicZone;		// 0:1, 1:2
	double dEPA;				// 유효지반 가속도
	int    nSiteClass;			// 0:S1, 1:S2, 2:S3, 3:S4, 4:S5, 5:S6
	double dFa;					// Acceleration-based site coefficient
	double dFv;					// Velocity-based site coefficient
	double dSds;				// Design Spectral Response Acc. at short periods
	double dSd1;				// Design Spectral Response Acc. at 1 s periods
	int    nSeismicUseGroup;	// 0=Special, 1=I, 2=II
	double dImportanceFactor;	// 1.0, 1.2, 1.5  
	double dResponseModifyFactor;  // 1.5, 2.5, 3, 3.25, 4.5, 5, 5.5, 6, 6.5, 7, 8
};

struct T_POSL_UDRD_D_CH
{
	T_POSL_K key;
	char strName[D_POSL_STR_MAX_LENGTH];
	int    nCodeType;			// KDS_E_2019 밖에 없음
	T_POSL_KDS2018 KDS2018;
	T_POSL_KDS2019 KDS2019;

	void GetPosl(T_POSL_K& rKey, T_POSL_D& rData)
	{
		rKey = key;
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nCodeType = nCodeType;
		rData.KDS2018 = KDS2018;
		rData.KDS2019 = KDS2019;
	}
	void SetPosl(const T_POSL_K tKey, T_POSL_D& rData)
	{
		key = tKey;
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		nCodeType = rData.nCodeType;
		KDS2018 = rData.KDS2018;
		KDS2019 = rData.KDS2019;
	}
	void Convert925(T_POSL_UDRD_D_CH_925& rData)
	{	
		key = rData.key;
		memcpy(strName, rData.strName, sizeof(strName));
		nCodeType = rData.nCodeType;
		KDS2019.nEPSEMethod = rData.nEPSEMethod;
		KDS2019.nSeismicZone = rData.nSeismicZone;
		KDS2019.dEPA = rData.dEPA;
		KDS2019.nSiteClass = rData.nSiteClass;
		KDS2019.dFa = rData.dFa;
		KDS2019.dFv = rData.dFv;
		KDS2019.dSds = rData.dSds;
		KDS2019.dSd1 = rData.dSd1;
		KDS2019.nSeismicUseGroup = rData.nSeismicUseGroup;
		KDS2019.dImportanceFactor = rData.dImportanceFactor;
		KDS2019.dResponseModifyFactor = rData.dResponseModifyFactor;
	}
};




#define HASHSIZEPOSL 101

#define T_EPSE_K unsigned int
// Seismic Earth Pressure

enum EN_EPSE_LAYER_METHOD
{
	EN_LAYER_METHOD_SINGLE = 0, EN_LAYER_METHOD_DOUBLE, EN_LAYER_METHOD_USER,
};

struct T_EPSE_PRESSURE_PROFILE
{
	double dLevel;
	double dReactionForce;
	double dDisplacement;
	double dPressure;
	double dAdditionalPressure;

	BOOL operator==(const T_EPSE_PRESSURE_PROFILE& src) const
	{
		if (fabs(dLevel - src.dLevel) > 1.e-10) return FALSE;
		if (fabs(dReactionForce - src.dReactionForce) > 1.e-10) return FALSE;
		if (fabs(dDisplacement - src.dDisplacement) > 1.e-10) return FALSE;
		if (fabs(dPressure - src.dPressure) > 1.e-10) return FALSE;
		if (fabs(dAdditionalPressure - src.dAdditionalPressure) > 1.e-10) return FALSE;
		return TRUE;
	}

};

struct T_EPSE_D
{
	T_STLD_K	KeyStld;		//Load Case Name
	int			nDirection;		//1=X-Y, 2=Normal, 3=local y+, 4=local y-, 5=local z+, 6=local z-
	double		dAngle;
	double		dInnerPnt[3];   // 공간구조의 경우 Nomal을 구하기위한 좌표.
	double		dScaleFactor;
	int			nCodeType;		// KDS_E_2019 밖에 없음
	//int			nEPSEMethod;
	int 		nLayerMethod;
	double		dLayerLevel;
	T_POSL_K	KeyPosl;		// Parameters of Seismic Loads
	T_POSP_K	KeyPosp;		// Parameters of Soil Properties
	int			nSelection;		// 1=Group, 2=Element
	T_LAPL_K	KeyLapl;		// 결국 DB에는 Lapl단위로 저장이 된다....
	CArray<T_ELEM_K, T_ELEM_K> arKeysDefineElem;
	CArray<T_NODE_K, T_NODE_K> arKeysDefineNode;
 	CArray<T_EPSE_PRESSURE_PROFILE, T_EPSE_PRESSURE_PROFILE&> arPresProfile;

	int nLoadingType;  // 0:Frame  1:Planar
	double dWidth;

	void Initialize()
	{
		KeyStld = 0;
		nDirection = 1;
		dAngle = 0.0;
		dInnerPnt[0] = 0.0;
		dInnerPnt[1] = 0.0;
		dInnerPnt[2] = 0.0;
		dScaleFactor = 1.0;
		nCodeType = KDS_E_2019;
		//nEPSEMethod = EN_EPSE_METHOD_EQUIVALENT;
		nLayerMethod = EN_LAYER_METHOD_SINGLE;
		dLayerLevel = 0.0;
		KeyPosl = 0;
		KeyPosp = 0;
		nSelection = SELECT_ELEM;
		KeyLapl = 0;
		arKeysDefineElem.RemoveAll();
		arKeysDefineNode.RemoveAll();
 		arPresProfile.RemoveAll();
		nLoadingType = 1;
		dWidth = 0.0;
	}
	T_EPSE_D()
	{
		Initialize();
	}
	T_EPSE_D(T_EPSE_D& src)
	{
		*this = src;
	}
	T_EPSE_D& operator=(const T_EPSE_D& src)
	{
		KeyStld = src.KeyStld;
		nDirection = src.nDirection;
		dAngle = src.dAngle;
		dInnerPnt[0] = src.dInnerPnt[0];
		dInnerPnt[1] = src.dInnerPnt[1];
		dInnerPnt[2] = src.dInnerPnt[2];
		dScaleFactor = src.dScaleFactor;
		nCodeType    = src.nCodeType;
		//nEPSEMethod = src.nEPSEMethod;
		nLayerMethod = src.nLayerMethod;
		dLayerLevel = src.dLayerLevel;
		KeyPosl = src.KeyPosl;
		KeyPosp = src.KeyPosp;
		nSelection = src.nSelection;
		KeyLapl = src.KeyLapl;
		arKeysDefineElem.Copy(src.arKeysDefineElem);
		arKeysDefineNode.Copy(src.arKeysDefineNode);
 		arPresProfile.Copy(src.arPresProfile);

		nLoadingType = src.nLoadingType;
		dWidth       = src.dWidth;
		return *this;
	}
};

struct T_EPSE_UDRD_D_RW_925
{
	T_EPSE_K key;
	T_STLD_K KeyStld; //Load Case Name
	int		 nDirection;   //1=X-Y, 2=Normal
	double	 dAngle;
	double	 dInnerPnt[3];   // 공간구조의 경우 Nomal을 구하기위한 좌표.
	double	 dScaleFactor;
	int			nCodeType;		// KDS_E_2019 밖에 없음
	int 		nLayerMethod;
	double		dLayerLevel;
	T_POSL_K	KeyPosl;		// Parameters of Seismic Loads
	T_POSP_K	KeyPosp;		// Parameters of Soil Properties
	int			nSelection;		// 1=Group, 2=Element
	T_LAPL_K	KeyLapl;		// 결국 DB에는 Lapl단위로 저장이 된다....
	int arKeysDefineElem;
	int arKeysDefineNode;
	int arPresProfile;
};

struct T_EPSE_UDRD_D_RW
{
	T_EPSE_K key;
	T_STLD_K KeyStld; //Load Case Name
	int		 nDirection;   //1=X-Y, 2=Normal
	double	 dAngle;
	double	 dInnerPnt[3];   // 공간구조의 경우 Nomal을 구하기위한 좌표.
	double	 dScaleFactor;
	int			nCodeType;		// KDS_E_2019 밖에 없음
	int 		nLayerMethod;
	double		dLayerLevel;
	T_POSL_K	KeyPosl;		// Parameters of Seismic Loads
	T_POSP_K	KeyPosp;		// Parameters of Soil Properties
	int			nSelection;		// 1=Group, 2=Element
	T_LAPL_K	KeyLapl;		// 결국 DB에는 Lapl단위로 저장이 된다....
	int arKeysDefineElem;
	int arKeysDefineNode;
	int arPresProfile;

	int nLoadingType;  // 0:Frame  1:Planar
	double dWidth;

	void GetEpse(T_EPSE_K& rKey, T_EPSE_D& rData)
	{
		rKey = key;
		rData.KeyStld = KeyStld;
		rData.nDirection = nDirection;
		rData.dAngle = dAngle;
		rData.dInnerPnt[0] = dInnerPnt[0];
		rData.dInnerPnt[1] = dInnerPnt[1];
		rData.dInnerPnt[2] = dInnerPnt[2];
		rData.dScaleFactor = dScaleFactor;
		rData.nCodeType = nCodeType;
		rData.nLayerMethod = nLayerMethod;
		rData.dLayerLevel = dLayerLevel;
		rData.KeyPosl = KeyPosl;
		rData.KeyPosp = KeyPosp;
		rData.nSelection = nSelection;
		rData.KeyLapl = KeyLapl;
		rData.nLoadingType = nLoadingType;
		rData.dWidth = dWidth;

	}
	void SetEpse(const T_EPSE_K tKey, const T_EPSE_D& rData)
	{
		key = tKey;
		KeyStld = rData.KeyStld;
		nDirection = rData.nDirection;
		dAngle = rData.dAngle;
		dInnerPnt[0] = rData.dInnerPnt[0];
		dInnerPnt[1] = rData.dInnerPnt[1];
		dInnerPnt[2] = rData.dInnerPnt[2];
		dScaleFactor = rData.dScaleFactor;
		nCodeType = rData.nCodeType;
		nLayerMethod = rData.nLayerMethod;
		dLayerLevel = rData.dLayerLevel;
		KeyPosl = rData.KeyPosl;
		KeyPosp = rData.KeyPosp;
		nSelection = rData.nSelection;
		KeyLapl = rData.KeyLapl;
		arKeysDefineElem = (int)rData.arKeysDefineElem.GetSize();
		arKeysDefineNode = (int)rData.arKeysDefineNode.GetSize();
		arPresProfile = (int)rData.arPresProfile.GetSize();
		nLoadingType = rData.nLoadingType;
		dWidth = rData.dWidth;
	}
	void Convert925(T_EPSE_UDRD_D_RW_925& rData)
	{
		key = rData.key;
		KeyStld = rData.KeyStld;
		nDirection = rData.nDirection;
		dAngle = rData.dAngle;
		dInnerPnt[0] = rData.dInnerPnt[0];
		dInnerPnt[1] = rData.dInnerPnt[1];
		dInnerPnt[2] = rData.dInnerPnt[2];
		dScaleFactor = rData.dScaleFactor;
		nCodeType = rData.nCodeType;
		nLayerMethod = rData.nLayerMethod;
		dLayerLevel = rData.dLayerLevel;
		KeyPosl = rData.KeyPosl;
		KeyPosp = rData.KeyPosp;
		nSelection = rData.nSelection;
		KeyLapl = rData.KeyLapl;
		arKeysDefineElem = rData.arKeysDefineElem;
		arKeysDefineNode = rData.arKeysDefineNode;
		arPresProfile = rData.arPresProfile;
		nLoadingType = 1;
		dWidth = 0.0;
	}
};

struct T_EPSE_UDRD_D
{
	T_EPSE_K key;
	T_EPSE_D data;
};

struct T_EPSE_UNIT
{
	T_EPSE_UNIT()
	{
		dInnerPnt							= D_UNITSYS_BASE_LENGTH;
		dScaleFactor						= D_UNITSYS_NONE;
		dLayerLevel							= D_UNITSYS_BASE_LENGTH;
		dWidth                              = D_UNITSYS_BASE_LENGTH; 
		arPresProfile_dLevel				= D_UNITSYS_BASE_LENGTH;
		arPresProfile_dReactionForce		= D_UNITSYS_BASE_DENSITY;
		arPresProfile_Pressure				= D_UNITSYS_BASE_PRESSURE;
	}

	int	dInnerPnt;
	int	dScaleFactor;
	int	dLayerLevel;
	int dWidth;
	int arPresProfile_dLevel;
	int arPresProfile_dReactionForce;
	int arPresProfile_Pressure;
};
#define HASHSIZEEPSE 101
#pragma pack(pop)

#endif  // __DB_ST_DT_ERPR_H__
