#ifndef __DB_ST_DT_POCV_H__
#define __DB_ST_DT_POCV_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#define T_POCV_K unsigned int

struct T_POCV_D    
{
	int  nPlotType;           // Capacity Spectrum일 때 쓰임
	int  nPlotType2;          // Plot type
	int  nProcType;
	int nBehaviorType;        // Structural Behavior Type
	double dDampingParam_Damping;

	//  Additional Curves at Other Nodes
	double	dPlotAdditNode1;
	double	dPlotAdditNode2;
	double	dPlotAdditNode3;
	double	dPlotAdditNode4;

	BOOL bDemandSpectra;    // Demand Spectra at Damping Ratio
	BOOL bConstPeriod;      // Constant Period Lines at Periods
	double dSpectDamping[4];
	double dConstPeriod[4];
	
	int nSpecCodeType;      // 0: User, 1: D_SPFC_CODE_??
	T_SPFC_CODE CodeParam;

	int nSpecCodeType_sub;     // D_SPFC_CODE_EURO2004
	T_SPFC_CODE CodeParam_sub; // For Target Displacement에서 저장할 데이터
	T_SPFC_K SpfcK_User;

	T_SPFC_K SpfcK_SLO;
	T_SPFC_K SpfcK_SLD;
	T_SPFC_K SpfcK_SLV;
	T_SPFC_K SpfcK_SLC;

	T_SPFC_DT arSpecFuncData;

	int nSpecFuncType;    // =1 Normalized Acceleration =2 Acceleration =3 Velocity =4 Displacement
	int nScaleMethod;     // 0=Scale Factor, 1=Maximum Value
	double ScaleFactor;
	double Gravity;  // Normalized Acceleration일때만 사용
	double dMaxValue;     // 6.4.0추가
	double dDampingRatio; // 6.5.0추가
	int    nGammaCalc;
	void Initialize()
	{
		nPlotType  = 0;
		nPlotType2 = 0;
		nProcType  = 0;
		bDemandSpectra      = FALSE;
		bConstPeriod        = FALSE;
		nBehaviorType  = 0;
		dDampingParam_Damping = 5.0;
		dPlotAdditNode1 = 0.0;
		dPlotAdditNode2 = 0.0;
		dPlotAdditNode3 = 0.0;
		dPlotAdditNode4 = 0.0;
		memset(dSpectDamping, 0, sizeof(dSpectDamping));
		memset(dConstPeriod , 0, sizeof(dConstPeriod)); 
		nSpecCodeType = D_SPFC_CODE_KBC2015;
		CodeParam.POInitialize(nSpecCodeType);
		nSpecCodeType_sub = D_SPFC_CODE_EURO2004;
		CodeParam_sub.POInitialize(nSpecCodeType_sub);
		SpfcK_User = 0;
		SpfcK_SLO = 0;
		SpfcK_SLD = 0;
		SpfcK_SLV = 0;
		SpfcK_SLC = 0;
		arSpecFuncData.RemoveAll();
		nSpecFuncType = 0;
		nScaleMethod  = 0;
		ScaleFactor=0.;
		Gravity=0.;
		dMaxValue = 0;
		dDampingRatio = 0.05;
		nGammaCalc = 0;
	}
	T_POCV_D() {}
	T_POCV_D(T_POCV_D& src)
	{
		*this = src;
	}
	T_POCV_D& operator=(T_POCV_D& src)
	{
		nPlotType       = src.nPlotType     ;
		nPlotType2      = src.nPlotType2    ;
		nProcType       = src.nProcType     ;
		bDemandSpectra  = src.bDemandSpectra;
		bConstPeriod    = src.bConstPeriod  ;
		nBehaviorType   = src.nBehaviorType ;
		dDampingParam_Damping = src.dDampingParam_Damping;

		dPlotAdditNode1 = src.dPlotAdditNode1;
		dPlotAdditNode2 = src.dPlotAdditNode2;
		dPlotAdditNode3 = src.dPlotAdditNode3;
		dPlotAdditNode4 = src.dPlotAdditNode4;

		nSpecCodeType     = src.nSpecCodeType;
		CodeParam         = src.CodeParam;
		nSpecCodeType_sub = src.nSpecCodeType_sub;
		CodeParam_sub     = src.CodeParam_sub;

		for(int k=0; k<4; ++k)
		{
			dSpectDamping[k]= src.dSpectDamping[k];
			dConstPeriod[k] = src.dConstPeriod[k];   
		}
		arSpecFuncData.Copy(src.arSpecFuncData);
		nSpecFuncType = src.nSpecFuncType;
		nScaleMethod  = src.nScaleMethod;

		ScaleFactor=src.ScaleFactor;
		Gravity=src.Gravity;
		dMaxValue = src.dMaxValue;
		dDampingRatio = src.dDampingRatio;    
		nGammaCalc = src.nGammaCalc;
		return *this;
	}
};

struct T_POCV_UNIT
{
	int NoUnit;
	
	T_POCV_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
};
struct T_POCV_UDRD_D
{
	T_POCV_K key;
	T_POCV_D data;
};
struct T_POCV_UDRD_D_CH_RW_925
{
	T_POCV_K key;
	int  nPlotType;           // Capacity Spectrum일 때 쓰임
	int  nPlotType2;          // Plot type
	int  nProcType;
	int  nBehaviorType;        // Structural Behavior Type
	double dDampingParam_Damping;

	//  Additional Curves at Other Nodes
	double	dPlotAdditNode1;
	double	dPlotAdditNode2;
	double	dPlotAdditNode3;
	double	dPlotAdditNode4;

	BOOL bDemandSpectra;    // Demand Spectra at Damping Ratio
	BOOL bConstPeriod;      // Constant Period Lines at Periods
	double dSpectDamping[4];
	double dConstPeriod[4];

	int nSpecCodeType;      // 0: User, 1: D_SPFC_CODE_??
	T_SPFC_CODE_925 CodeParam;

	// For Target Displacement에서 저장할 데이터
	int nSpecCodeType_sub;  // 0: User, 1: D_SPFC_CODE_??
	T_SPFC_CODE_925 CodeParam_sub;

	int nCountFuncData;
	int nSpecFuncType;
	int nScaleMethod;
	double ScaleFactor;
	double Gravity;       // Normalized Acceleration일때만 사용
	double dMaxValue;     // 6.4.0추가
	double dDampingRatio; // 6.5.0추가
	int    nGammaCalc;
};

struct T_POCV_UDRD_D_CH_RW
{
	T_POCV_K key;
	int  nPlotType;           // Capacity Spectrum일 때 쓰임
	int  nPlotType2;          // Plot type
	int  nProcType;
	int  nBehaviorType;        // Structural Behavior Type
	double dDampingParam_Damping;

	//  Additional Curves at Other Nodes
	double	dPlotAdditNode1;
	double	dPlotAdditNode2;
	double	dPlotAdditNode3;
	double	dPlotAdditNode4;

	BOOL bDemandSpectra;    // Demand Spectra at Damping Ratio
	BOOL bConstPeriod;      // Constant Period Lines at Periods
	double dSpectDamping[4];
	double dConstPeriod[4];

	int nSpecCodeType;      // 0: User, 1: D_SPFC_CODE_??
	T_SPFC_CODE CodeParam;

	// For Target Displacement에서 저장할 데이터
	int nSpecCodeType_sub;  // 0: User, 1: D_SPFC_CODE_??
	T_SPFC_CODE CodeParam_sub;

	int nCountFuncData; 
	int nSpecFuncType;
	int nScaleMethod; 
	double ScaleFactor;
	double Gravity;       // Normalized Acceleration일때만 사용
	double dMaxValue;     // 6.4.0추가
	double dDampingRatio; // 6.5.0추가
	int    nGammaCalc;
	void GetPocv(T_POCV_K& rKey, T_POCV_D& rData)
	{
		rKey=key;
		rData.nPlotType     = nPlotType;
		rData.nPlotType2    = nPlotType2;
		rData.nProcType     = nProcType;
		rData.nBehaviorType = nBehaviorType;
		rData.dDampingParam_Damping = dDampingParam_Damping;

		rData.dPlotAdditNode1 = dPlotAdditNode1;
		rData.dPlotAdditNode2 = dPlotAdditNode2;
		rData.dPlotAdditNode3 = dPlotAdditNode3;
		rData.dPlotAdditNode4 = dPlotAdditNode4;

		rData.bDemandSpectra = bDemandSpectra;
		rData.bConstPeriod   = bConstPeriod;
		for(int k=0; k<4; ++k)
		{
			rData.dSpectDamping[k]= dSpectDamping[k];
			rData.dConstPeriod[k] = dConstPeriod[k];   
		}
		rData.nSpecCodeType = nSpecCodeType;
		rData.CodeParam     = CodeParam;

		rData.nSpecCodeType_sub = nSpecCodeType_sub;
		rData.CodeParam_sub     = CodeParam_sub;

		rData.nSpecFuncType = nSpecFuncType;
		rData.nScaleMethod  = nScaleMethod;

		rData.ScaleFactor   = ScaleFactor;
		rData.Gravity       = Gravity;
		rData.dMaxValue     = dMaxValue;
		rData.dDampingRatio = dDampingRatio;    
		rData.nGammaCalc    = nGammaCalc;
	}
	void SetPocv(const T_POCV_K Key, const T_POCV_D& rData)
	{
		key = Key;
		nPlotType     = rData.nPlotType;
		nPlotType2    = rData.nPlotType2;
		nProcType     = rData.nProcType;
		nBehaviorType = rData.nBehaviorType;
		dDampingParam_Damping = rData.dDampingParam_Damping;

		dPlotAdditNode1 = rData.dPlotAdditNode1;
		dPlotAdditNode2 = rData.dPlotAdditNode2;
		dPlotAdditNode3 = rData.dPlotAdditNode3;
		dPlotAdditNode4 = rData.dPlotAdditNode4;

		bDemandSpectra  = rData.bDemandSpectra;
		bConstPeriod    = rData.bConstPeriod;
		for(int k=0; k<4; ++k)
		{
			dSpectDamping[k]= rData.dSpectDamping[k];
			dConstPeriod[k] = rData.dConstPeriod[k];   
		}
		nSpecCodeType     = rData.nSpecCodeType;
		CodeParam         = rData.CodeParam;
		nSpecCodeType_sub = rData.nSpecCodeType_sub;
		CodeParam_sub     = rData.CodeParam_sub;

		nCountFuncData= (int)rData.arSpecFuncData.GetSize();
		
		nSpecFuncType = rData.nSpecFuncType;
		nScaleMethod  = rData.nScaleMethod;

		ScaleFactor   = rData.ScaleFactor;
		Gravity       = rData.Gravity;
		dMaxValue     = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio; 
		nGammaCalc    = rData.nGammaCalc;
	}
	void Convert925(const T_POCV_UDRD_D_CH_RW_925& rData)
	{
		key = rData.key;
		nPlotType = rData.nPlotType;
		nPlotType2 = rData.nPlotType2;
		nProcType = rData.nProcType;
		nBehaviorType = rData.nBehaviorType;
		dDampingParam_Damping = rData.dDampingParam_Damping;

		dPlotAdditNode1 = rData.dPlotAdditNode1;
		dPlotAdditNode2 = rData.dPlotAdditNode2;
		dPlotAdditNode3 = rData.dPlotAdditNode3;
		dPlotAdditNode4 = rData.dPlotAdditNode4;

		bDemandSpectra = rData.bDemandSpectra;
		bConstPeriod = rData.bConstPeriod;
		for (int k = 0; k < 4; ++k)
		{
			dSpectDamping[k] = rData.dSpectDamping[k];
			dConstPeriod[k] = rData.dConstPeriod[k];
		}
		nSpecCodeType = rData.nSpecCodeType;
		CodeParam.Convert925(nSpecCodeType, rData.CodeParam);
		nSpecCodeType_sub = rData.nSpecCodeType_sub;
		CodeParam_sub.Convert925(nSpecCodeType_sub, rData.CodeParam_sub);

		nCountFuncData = rData.nCountFuncData;

		nSpecFuncType = rData.nSpecFuncType;
		nScaleMethod = rData.nScaleMethod;

		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		dMaxValue = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nGammaCalc = rData.nGammaCalc;
	}
};

#define HASHSIZEPOCV 11


#pragma pack(pop)

#endif  // __DB_ST_DT_POCV_H__
