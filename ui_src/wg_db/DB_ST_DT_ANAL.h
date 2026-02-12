#ifndef __DB_ST_DT_ANAL_H__
#define __DB_ST_DT_ANAL_H__

#define MIDAS_SDK

#pragma pack(push, 1)

/// analysis control ///
#define T_ACTL_K unsigned int
struct T_ACTL_D_413
{
	BOOL bAutoConstraintNormal;
	BOOL bAutoConstraintRotation;
	int nTrussIteration;
	double dblTrussTolerance;
	BOOL bPerformAnalysis[20];
};
struct T_ACTL_D_670
{
	BOOL bAutoConstraintNormal;
	BOOL bAutoConstraintRotation;
	int nIterationType;  // 0= Include Inactive Element 1=Exclude Inactive Element
	int nTrussIteration;   // if nIterationType == 0
	int nIterPerLoad;      // if nIterationType == 1
	int nSubIterPerIter;   // if nIterationType == 1
	double dblTrussTolerance;
	BOOL bPerformAnalysis[20];
};
struct T_ACTL_D_671_ALPHA // 호환 유지 목적
{
	BOOL bAutoConstraintNormal;
	BOOL bAutoConstraintRotation;
	int nIterationType;  // 0= Include Inactive Element 1=Exclude Inactive Element
	int nTrussIteration;   // if nIterationType == 0
	int nIterPerLoad;      // if nIterationType == 1
	int nSubIterPerIter;   // if nIterationType == 1
	double dblTrussTolerance;
	BOOL bPerformAnalysis[20];
	BOOL bConsiderSecf;		// 6.7.1에서 추가
};
struct T_ACTL_D_692
{
	BOOL bAutoConstraintNormal;
	BOOL bAutoConstraintRotation;
	int nIterationType;  // 0= Include Inactive Element 1=Exclude Inactive Element
	int nTrussIteration;   // if nIterationType == 0
	int nIterPerLoad;      // if nIterationType == 1
	int nSubIterPerIter;   // if nIterationType == 1
	double dblTrussTolerance;
	BOOL bPerformAnalysis[20];
	BOOL bConsiderSecf;		// 6.7.1에서 추가
	BOOL bConsiderRbar;		// 6.7.1에서 추가
};
struct T_ACTL_D_750
{
	BOOL bAutoConstraintNormal;
	BOOL bAutoConstraintRotation;
	int nIterationType;    // 0= Include Inactive Element 1=Exclude Inactive Element 
	// v720이후 nIterationType=2로 고정// MNET:2567 20061108 JHKIM
	int nTrussIteration;   // if nIterationType == 0 // v720이후 사용안함// MNET:2567 20061108 JHKIM
	int nIterPerLoad;      // if nIterationType == 1
	int nSubIterPerIter;   // if nIterationType == 1 // v720이후 사용안함// MNET:2567 20061108 JHKIM
	double dblTrussTolerance;
	BOOL bPerformAnalysis[20];
	BOOL bConsiderSecf;		// 6.7.1에서 추가
	BOOL bConsiderRbar;		// 6.7.1에서 추가
	BOOL bConsiderSectOffset; // 7.0.0에서 추가 (JP Only)
	BOOL bConsiderTDF;  // 7.0.0에서 추가 
	BOOL bConsiderSCS;  // 7.0.0에서 추가 (JP Only)
};
struct T_ACTL_D_797
{
	BOOL bAutoConstraintNormal;
	BOOL bAutoConstraintRotation;
	int nIterationType;    // 0= Include Inactive Element 1=Exclude Inactive Element 
	// v720이후 nIterationType=2로 고정// MNET:2567 20061108 JHKIM
	int nTrussIteration;   // if nIterationType == 0 // v720이후 사용안함// MNET:2567 20061108 JHKIM
	int nIterPerLoad;      // if nIterationType == 1
	int nSubIterPerIter;   // if nIterationType == 1 // v720이후 사용안함// MNET:2567 20061108 JHKIM
	double dblTrussTolerance;
	BOOL bPerformAnalysis[20];
	BOOL bConsiderSecf;		// 6.7.1에서 추가
	BOOL bConsiderRbar;		// 6.7.1에서 추가
	BOOL bConsiderSectOffset; // 7.0.0에서 추가 (JP Only)
	BOOL bConsiderTDF;  // 7.0.0에서 추가 
	BOOL bConsiderSCS;  // 7.0.0에서 추가 (JP Only)
	BOOL bTransferReac;
};
struct T_ACTL_D_831
{
	BOOL bAutoConstraintNormal;
	BOOL bAutoConstraintRotation;
	int nIterationType;    // 0= Include Inactive Element 1=Exclude Inactive Element 
	// v720이후 nIterationType=2로 고정// MNET:2567 20061108 JHKIM
	int nTrussIteration;   // if nIterationType == 0 // v720이후 사용안함// MNET:2567 20061108 JHKIM
	int nIterPerLoad;      // if nIterationType == 1
	int nSubIterPerIter;   // if nIterationType == 1 // v720이후 사용안함// MNET:2567 20061108 JHKIM
	double dblTrussTolerance;
	BOOL bPerformAnalysis[20];
	BOOL bConsiderSecf;		// 6.7.1에서 추가
	BOOL bConsiderRbar;		// 6.7.1에서 추가
	BOOL bConsiderSectOffset; // 7.0.0에서 추가 (JP Only)
	BOOL bConsiderTDF;  // 7.0.0에서 추가 
	BOOL bConsiderSCS;  // 7.0.0에서 추가 (JP Only)
	BOOL bTransferReac;
	BOOL bChangeLcAxOfTapSect; // 7.9.8 (Civil Only)
};

struct T_ACTL_D_845
{
	BOOL bAutoConstraintNormal;
	BOOL bAutoConstraintRotation;
	int nIterationType;    // 0= Include Inactive Element 1=Exclude Inactive Element 
	// v720이후 nIterationType=2로 고정// MNET:2567 20061108 JHKIM
	int nTrussIteration;   // if nIterationType == 0 // v720이후 사용안함// MNET:2567 20061108 JHKIM
	int nIterPerLoad;      // if nIterationType == 1
	int nSubIterPerIter;   // if nIterationType == 1 // v720이후 사용안함// MNET:2567 20061108 JHKIM
	double dblTrussTolerance;
	BOOL bPerformAnalysis[20];
	BOOL bConsiderSecf;		// 6.7.1에서 추가
	BOOL bConsiderRbar;		// 6.7.1에서 추가
	BOOL bConsiderSectOffset; // 7.0.0에서 추가 (JP Only)
	BOOL bConsiderTDF;  // 7.0.0에서 추가 
	BOOL bConsiderSCS;  // 7.0.0에서 추가 (JP Only)
	BOOL bTransferReac; // 7.6.0에서 추가
	BOOL bChangeLcAxOfTapSect; // 7.9.8 (Civil Only)
	BOOL bAutoWallConstraint; // 8.3.5에서 추가 (Renewal Gen 벽 개구부 추가하면서 불연속 상하벽을 해석시 자동구속)
};

struct T_ACTL_D_960
{
	BOOL bAutoConstraintNormal;
	BOOL bAutoConstraintRotation;
	int nIterationType;    // 0= Include Inactive Element 1=Exclude Inactive Element 
	// v720이후 nIterationType=2로 고정// MNET:2567 20061108 JHKIM
	int nTrussIteration;   // if nIterationType == 0 // v720이후 사용안함// MNET:2567 20061108 JHKIM
	int nIterPerLoad;      // if nIterationType == 1
	int nSubIterPerIter;   // if nIterationType == 1 // v720이후 사용안함// MNET:2567 20061108 JHKIM
	double dblTrussTolerance;
	BOOL bPerformAnalysis[20];
	BOOL bConsiderSecf;		// 6.7.1에서 추가
	BOOL bConsiderRbar;		// 6.7.1에서 추가
	BOOL bConsiderSectOffset; // 7.0.0에서 추가 (JP Only)
	BOOL bConsiderTDF;  // 7.0.0에서 추가 
	BOOL bConsiderSCS;  // 7.0.0에서 추가 (JP Only)
	BOOL bTransferReac; // 7.6.0에서 추가
	BOOL bChangeLcAxOfTapSect; // 7.9.8 (Civil Only)
	BOOL bAutoWallConstraint; // 8.3.5에서 추가 (Renewal Gen 벽 개구부 추가하면서 불연속 상하벽을 해석시 자동구속)
	BOOL bCalculateBeamStresses; // 8.5.0에서 추가 Calculate Beam Stresses (Von-Mises and Max-Shear)
};

struct T_ACTL_D
{
	BOOL bAutoConstraintNormal;
	BOOL bAutoConstraintRotation;
	int nIterationType;    // 0= Include Inactive Element 1=Exclude Inactive Element 
												 // v720이후 nIterationType=2로 고정// MNET:2567 20061108 JHKIM
	int nTrussIteration;   // if nIterationType == 0 // v720이후 사용안함// MNET:2567 20061108 JHKIM
	int nIterPerLoad;      // if nIterationType == 1
	int nSubIterPerIter;   // if nIterationType == 1 // v720이후 사용안함// MNET:2567 20061108 JHKIM
	double dblTrussTolerance;
	BOOL bPerformAnalysis[20];
	BOOL bConsiderSecf;		// 6.7.1에서 추가
	BOOL bConsiderRbar;		// 6.7.1에서 추가
	BOOL bConsiderSectOffset; // 7.0.0에서 추가 (JP Only)
	BOOL bConsiderTDF;  // 7.0.0에서 추가 
	BOOL bConsiderSCS;  // 7.0.0에서 추가 (JP Only)
	BOOL bTransferReac; // 7.6.0에서 추가
	BOOL bChangeLcAxOfTapSect; // 7.9.8 (Civil Only)
	BOOL bAutoWallConstraint; // 8.3.5에서 추가 (Renewal Gen 벽 개구부 추가하면서 불연속 상하벽을 해석시 자동구속)
	BOOL bCalculateBeamStresses; // 8.5.0에서 추가 Calculate Beam Stresses (Von-Mises and Max-Shear)
	/// MEC
	BOOL bClassicalFormulaSolid;
	int  nBeamSectionChange;
	int	nLoadStepType;
	int nNumberOfIncrement;
	int nIntermediateOutputRequest;
	CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> aLoadSteps;
	BOOL bConvFlag[3];        // 0:Displacement(U), 1:Load(P), 2:Work(W)
	double dConvValue[3];     // 0:Displacement(U), 1:Load(P), 2:Work(W)
	T_NLCT_NLCTRL_PARM AdvancedParam;
	BOOL bConsiderCable;  // Consider Initial Tension / Unstrained Initial Length for Cable Element (Equivalent Truss)
	///
	T_ACTL_D() {}
	T_ACTL_D(T_ACTL_D& src) { *this = src; }
	T_ACTL_D& operator=(T_ACTL_D& src)
	{
		bAutoConstraintNormal = src.bAutoConstraintNormal;
		bAutoConstraintRotation = src.bAutoConstraintRotation;
		nIterationType = src.nIterationType;
		nTrussIteration = src.nTrussIteration;
		nIterPerLoad = src.nIterPerLoad;
		nSubIterPerIter = src.nSubIterPerIter;
		dblTrussTolerance = src.dblTrussTolerance;
		memcpy(bPerformAnalysis, src.bPerformAnalysis, sizeof(bPerformAnalysis));
		bConsiderSecf = src.bConsiderSecf;
		bConsiderRbar = src.bConsiderRbar;
		bConsiderSectOffset = src.bConsiderSectOffset;
		bConsiderTDF = src.bConsiderTDF;
		bConsiderSCS = src.bConsiderSCS;
		bTransferReac = src.bTransferReac;
		bChangeLcAxOfTapSect = src.bChangeLcAxOfTapSect;
		bAutoWallConstraint = src.bAutoWallConstraint;
		bCalculateBeamStresses = src.bCalculateBeamStresses;
		bClassicalFormulaSolid = src.bClassicalFormulaSolid;
		nBeamSectionChange = src.nBeamSectionChange;
		nLoadStepType = src.nLoadStepType;
		nNumberOfIncrement = src.nNumberOfIncrement;
		nIntermediateOutputRequest = src.nIntermediateOutputRequest;
		aLoadSteps.Copy(src.aLoadSteps);
		for (int i = 0; i < 3; i++)
		{
			bConvFlag[i] = src.bConvFlag[i];
			dConvValue[i] = src.dConvValue[i];
		}
		AdvancedParam = src.AdvancedParam;
		bConsiderCable = src.bConsiderCable;

		return *this;
	}

	void Initialize()
	{
		bAutoConstraintNormal=TRUE;
		bAutoConstraintRotation=TRUE;
		nIterationType=2;
		nTrussIteration=100;
		nIterPerLoad=20;
		nSubIterPerIter=3;
		dblTrussTolerance=1.e-3;
		for (int i=0; i<20; i++)bPerformAnalysis[i]=FALSE;
		bConsiderSecf = FALSE;
#if defined(_MGEN)||defined(_CIVIL_CH)
		bConsiderRbar = TRUE;
#else
		bConsiderRbar = FALSE; 
#endif
		bConsiderSectOffset = FALSE;
#if defined(_CIVIL) && defined(_JP)// MNET:1907 20060125 JHKIM(default변경)
		bConsiderTDF = TRUE;
		bConsiderSCS = TRUE;
#else
		bConsiderTDF = FALSE;
		bConsiderSCS = FALSE;
#endif
		bTransferReac = TRUE;
		bChangeLcAxOfTapSect = FALSE;
		bAutoWallConstraint = TRUE;
		bCalculateBeamStresses = FALSE;
		bClassicalFormulaSolid = FALSE;	
		nBeamSectionChange = 1;
		nLoadStepType = 0;
		nNumberOfIncrement = 1;
		nIntermediateOutputRequest = 1;
		aLoadSteps.RemoveAll();
		AdvancedParam.Initialize();
		bConvFlag[0] = TRUE;
		bConvFlag[1] = FALSE;
		bConvFlag[2] = FALSE;
		dConvValue[0] = dblTrussTolerance;
		dConvValue[1] = 0.001;
		dConvValue[2] = 1.e-6;
		AdvancedParam.bUseDefaultNLParm = FALSE;
		AdvancedParam.nMaxBisectLevel = 0;
		bConsiderCable = TRUE;
	}	
	void Convert413(T_ACTL_D_413& rData)
	{
		bAutoConstraintNormal=rData.bAutoConstraintNormal;
		bAutoConstraintRotation=rData.bAutoConstraintRotation;
		nIterationType=1;
		nTrussIteration=rData.nTrussIteration;
		nIterPerLoad=10;
		nSubIterPerIter=5;
		dblTrussTolerance=rData.dblTrussTolerance;
		memcpy(bPerformAnalysis, rData.bPerformAnalysis, sizeof(bPerformAnalysis));
		bConsiderSecf = TRUE;
		bConsiderRbar = TRUE;
		bConsiderSectOffset = FALSE;
		bConsiderTDF = FALSE;
		bConsiderSCS = FALSE;
		bTransferReac = TRUE;
		bChangeLcAxOfTapSect = FALSE;
		bAutoWallConstraint = FALSE;
		bCalculateBeamStresses = FALSE;
		bClassicalFormulaSolid = TRUE;
		nBeamSectionChange = 1;
		nLoadStepType = 0;
		nNumberOfIncrement = 1;
		nIntermediateOutputRequest = 1;
		aLoadSteps.RemoveAll();
		AdvancedParam.Initialize();
		bConvFlag[0] = TRUE;
		bConvFlag[1] = FALSE;
		bConvFlag[2] = FALSE;
		dConvValue[0] = dblTrussTolerance;
		dConvValue[1] = 0.001;
		dConvValue[2] = 1.e-6;
		AdvancedParam.bUseDefaultNLParm = FALSE;
		AdvancedParam.nMaxBisectLevel = 0;
		bConsiderCable = FALSE;
	}
	void Convert670(T_ACTL_D_670& rData)
	{
		bAutoConstraintNormal=rData.bAutoConstraintNormal;
		bAutoConstraintRotation=rData.bAutoConstraintRotation;
		nIterationType=rData.nIterationType;
		nTrussIteration=rData.nTrussIteration;
		nIterPerLoad=rData.nIterPerLoad;
		nSubIterPerIter=rData.nSubIterPerIter;
		dblTrussTolerance=rData.dblTrussTolerance;
		memcpy(bPerformAnalysis, rData.bPerformAnalysis, sizeof(bPerformAnalysis));
		bConsiderSecf = TRUE;
		bConsiderRbar = TRUE;
		bConsiderSectOffset = FALSE;
		bConsiderTDF = FALSE;
		bConsiderSCS = FALSE;
		bTransferReac = TRUE;
		bChangeLcAxOfTapSect = FALSE;
		bAutoWallConstraint = FALSE;
		bCalculateBeamStresses = FALSE;
		bClassicalFormulaSolid = TRUE;
		nBeamSectionChange = 1;
		nLoadStepType = 0;
		nNumberOfIncrement = 1;
		nIntermediateOutputRequest = 1;
		aLoadSteps.RemoveAll();
		AdvancedParam.Initialize();
		bConvFlag[0] = TRUE;
		bConvFlag[1] = FALSE;
		bConvFlag[2] = FALSE;
		dConvValue[0] = dblTrussTolerance;
		dConvValue[1] = 0.001;
		dConvValue[2] = 1.e-6;
		AdvancedParam.bUseDefaultNLParm = FALSE;
		AdvancedParam.nMaxBisectLevel = 0;
		bConsiderCable = FALSE;
	}
	void Convert671_ALPHA(T_ACTL_D_671_ALPHA& rData)
	{
		bAutoConstraintNormal=rData.bAutoConstraintNormal;
		bAutoConstraintRotation=rData.bAutoConstraintRotation;
		nIterationType=rData.nIterationType;
		nTrussIteration=rData.nTrussIteration;
		nIterPerLoad=rData.nIterPerLoad;
		nSubIterPerIter=rData.nSubIterPerIter;
		dblTrussTolerance=rData.dblTrussTolerance;
		memcpy(bPerformAnalysis, rData.bPerformAnalysis, sizeof(bPerformAnalysis));
		bConsiderSecf = rData.bConsiderSecf;
		bConsiderRbar = TRUE;
		bConsiderSectOffset = FALSE;
		bConsiderTDF = FALSE;
		bConsiderSCS = FALSE;
		bTransferReac = TRUE;
		bChangeLcAxOfTapSect = FALSE;
		bAutoWallConstraint = FALSE;
		bCalculateBeamStresses = FALSE;
		bClassicalFormulaSolid = TRUE;
		nBeamSectionChange = 1;
		nLoadStepType = 0;
		nNumberOfIncrement = 1;
		nIntermediateOutputRequest = 1;
		aLoadSteps.RemoveAll();
		AdvancedParam.Initialize();
		bConvFlag[0] = TRUE;
		bConvFlag[1] = FALSE;
		bConvFlag[2] = FALSE;
		dConvValue[0] = dblTrussTolerance;
		dConvValue[1] = 0.001;
		dConvValue[2] = 1.e-6;
		AdvancedParam.bUseDefaultNLParm = FALSE;
		AdvancedParam.nMaxBisectLevel = 0;
		bConsiderCable = FALSE;
	}
	void Convert692(T_ACTL_D_692 &rData)
	{
		bAutoConstraintNormal=rData.bAutoConstraintNormal;
		bAutoConstraintRotation=rData.bAutoConstraintRotation;
		nIterationType=rData.nIterationType;
		nTrussIteration=rData.nTrussIteration;
		nIterPerLoad=rData.nIterPerLoad;
		nSubIterPerIter=rData.nSubIterPerIter;
		dblTrussTolerance=rData.dblTrussTolerance;
		memcpy(bPerformAnalysis, rData.bPerformAnalysis, sizeof(bPerformAnalysis));
		bConsiderSecf = rData.bConsiderSecf;
		bConsiderRbar = rData.bConsiderRbar;
		bConsiderSectOffset = FALSE;
		bConsiderTDF = FALSE;
		bConsiderSCS = FALSE;
		bTransferReac = TRUE;
		bChangeLcAxOfTapSect = FALSE;
		bAutoWallConstraint = FALSE;
		bCalculateBeamStresses = FALSE;
		bClassicalFormulaSolid = TRUE;
		nBeamSectionChange = 1;
		nLoadStepType = 0;
		nNumberOfIncrement = 1;
		nIntermediateOutputRequest = 1;
		aLoadSteps.RemoveAll();
		AdvancedParam.Initialize();
		bConvFlag[0] = TRUE;
		bConvFlag[1] = FALSE;
		bConvFlag[2] = FALSE;
		dConvValue[0] = dblTrussTolerance;
		dConvValue[1] = 0.001;
		dConvValue[2] = 1.e-6;
		AdvancedParam.bUseDefaultNLParm = FALSE;
		AdvancedParam.nMaxBisectLevel = 0;
		bConsiderCable = FALSE;
	}
	void Convert750(T_ACTL_D_750 &rData)
	{
		bAutoConstraintNormal=rData.bAutoConstraintNormal;
		bAutoConstraintRotation=rData.bAutoConstraintRotation;
		nIterationType=rData.nIterationType;
		nTrussIteration=rData.nTrussIteration;
		nIterPerLoad=rData.nIterPerLoad;
		nSubIterPerIter=rData.nSubIterPerIter;
		dblTrussTolerance=rData.dblTrussTolerance;
		memcpy(bPerformAnalysis, rData.bPerformAnalysis, sizeof(bPerformAnalysis));
		bConsiderSecf = rData.bConsiderSecf;
		bConsiderRbar = rData.bConsiderRbar;
		bConsiderSectOffset = rData.bConsiderSectOffset;
		bConsiderTDF = rData.bConsiderTDF;
		bConsiderSCS = rData.bConsiderSCS;
		bTransferReac = TRUE;
		bChangeLcAxOfTapSect = FALSE;
		bAutoWallConstraint = FALSE;
		bCalculateBeamStresses = FALSE;
		bClassicalFormulaSolid = TRUE;
		nBeamSectionChange = 1;
		nLoadStepType = 0;
		nNumberOfIncrement = 1;
		nIntermediateOutputRequest = 1;
		aLoadSteps.RemoveAll();
		AdvancedParam.Initialize();
		bConvFlag[0] = TRUE;
		bConvFlag[1] = FALSE;
		bConvFlag[2] = FALSE;
		dConvValue[0] = dblTrussTolerance;
		dConvValue[1] = 0.001;
		dConvValue[2] = 1.e-6;
		AdvancedParam.bUseDefaultNLParm = FALSE;
		AdvancedParam.nMaxBisectLevel = 0;
		bConsiderCable = FALSE;
	}
	void Convert797(T_ACTL_D_797 &rData)
	{
		bAutoConstraintNormal=rData.bAutoConstraintNormal;
		bAutoConstraintRotation=rData.bAutoConstraintRotation;
		nIterationType=rData.nIterationType;
		nTrussIteration=rData.nTrussIteration;
		nIterPerLoad=rData.nIterPerLoad;
		nSubIterPerIter=rData.nSubIterPerIter;
		dblTrussTolerance=rData.dblTrussTolerance;
		memcpy(bPerformAnalysis, rData.bPerformAnalysis, sizeof(bPerformAnalysis));
		bConsiderSecf = rData.bConsiderSecf;
		bConsiderRbar = rData.bConsiderRbar;
		bConsiderSectOffset = rData.bConsiderSectOffset;
		bConsiderTDF = rData.bConsiderTDF;
		bConsiderSCS = rData.bConsiderSCS;
		bTransferReac = rData.bTransferReac;
		bChangeLcAxOfTapSect = FALSE;
		bAutoWallConstraint = FALSE;
		bCalculateBeamStresses = FALSE;
		bClassicalFormulaSolid = TRUE;
		nBeamSectionChange = 1;
		nLoadStepType = 0;
		nNumberOfIncrement = 1;
		nIntermediateOutputRequest = 1;
		aLoadSteps.RemoveAll();
		AdvancedParam.Initialize();
		bConvFlag[0] = TRUE;
		bConvFlag[1] = FALSE;
		bConvFlag[2] = FALSE;
		dConvValue[0] = dblTrussTolerance;
		dConvValue[1] = 0.001;
		dConvValue[2] = 1.e-6;
		AdvancedParam.bUseDefaultNLParm = FALSE;
		AdvancedParam.nMaxBisectLevel = 0;
		bConsiderCable = FALSE;
	}
	void Convert831(T_ACTL_D_831 &rData)
	{
		bAutoConstraintNormal=rData.bAutoConstraintNormal;
		bAutoConstraintRotation=rData.bAutoConstraintRotation;
		nIterationType=rData.nIterationType;
		nTrussIteration=rData.nTrussIteration;
		nIterPerLoad=rData.nIterPerLoad;
		nSubIterPerIter=rData.nSubIterPerIter;
		dblTrussTolerance=rData.dblTrussTolerance;
		memcpy(bPerformAnalysis, rData.bPerformAnalysis, sizeof(bPerformAnalysis));
		bConsiderSecf = rData.bConsiderSecf;
		bConsiderRbar = rData.bConsiderRbar;
		bConsiderSectOffset = rData.bConsiderSectOffset;
		bConsiderTDF = rData.bConsiderTDF;
		bConsiderSCS = rData.bConsiderSCS;
		bTransferReac = rData.bTransferReac;
		bChangeLcAxOfTapSect = rData.bChangeLcAxOfTapSect;
		bAutoWallConstraint = FALSE;
		bCalculateBeamStresses = FALSE;
		bClassicalFormulaSolid = TRUE;
		nBeamSectionChange = 1;
		nLoadStepType = 0;
		nNumberOfIncrement = 1;
		nIntermediateOutputRequest = 1;
		aLoadSteps.RemoveAll();
		AdvancedParam.Initialize();
		bConvFlag[0] = TRUE;
		bConvFlag[1] = FALSE;
		bConvFlag[2] = FALSE;
		dConvValue[0] = dblTrussTolerance;
		dConvValue[1] = 0.001;
		dConvValue[2] = 1.e-6;
		AdvancedParam.bUseDefaultNLParm = FALSE;
		AdvancedParam.nMaxBisectLevel = 0;
		bConsiderCable = FALSE;
	}
	void Convert845(T_ACTL_D_845 &rData)
	{
		bAutoConstraintNormal=rData.bAutoConstraintNormal;
		bAutoConstraintRotation=rData.bAutoConstraintRotation;
		nIterationType=rData.nIterationType;
		nTrussIteration=rData.nTrussIteration;
		nIterPerLoad=rData.nIterPerLoad;
		nSubIterPerIter=rData.nSubIterPerIter;
		dblTrussTolerance=rData.dblTrussTolerance;
		memcpy(bPerformAnalysis, rData.bPerformAnalysis, sizeof(bPerformAnalysis));
		bConsiderSecf = rData.bConsiderSecf;
		bConsiderRbar = rData.bConsiderRbar;
		bConsiderSectOffset = rData.bConsiderSectOffset;
		bConsiderTDF = rData.bConsiderTDF;
		bConsiderSCS = rData.bConsiderSCS;
		bTransferReac = rData.bTransferReac;
		bChangeLcAxOfTapSect = rData.bChangeLcAxOfTapSect;
		bAutoWallConstraint = rData.bAutoWallConstraint;
		bCalculateBeamStresses = FALSE;
		bClassicalFormulaSolid = TRUE;
		nBeamSectionChange = 1;
		nLoadStepType = 0;
		nNumberOfIncrement = 1;
		nIntermediateOutputRequest = 1;
		aLoadSteps.RemoveAll();
		AdvancedParam.Initialize();
		bConvFlag[0] = TRUE;
		bConvFlag[1] = FALSE;
		bConvFlag[2] = FALSE;
		dConvValue[0] = dblTrussTolerance;
		dConvValue[1] = 0.001;
		dConvValue[2] = 1.e-6;
		AdvancedParam.bUseDefaultNLParm = FALSE;
		AdvancedParam.nMaxBisectLevel = 0;
		bConsiderCable = FALSE;
	}
	void Convert960(T_ACTL_D_960& rData)
	{
		bAutoConstraintNormal = rData.bAutoConstraintNormal;
		bAutoConstraintRotation = rData.bAutoConstraintRotation;
		nIterationType = rData.nIterationType;
		nTrussIteration = rData.nTrussIteration;
		nIterPerLoad = rData.nIterPerLoad;
		nSubIterPerIter = rData.nSubIterPerIter;
		dblTrussTolerance = rData.dblTrussTolerance;
		memcpy(bPerformAnalysis, rData.bPerformAnalysis, sizeof(bPerformAnalysis));
		bConsiderSecf = rData.bConsiderSecf;
		bConsiderRbar = rData.bConsiderRbar;
		bConsiderSectOffset = rData.bConsiderSectOffset;
		bConsiderTDF = rData.bConsiderTDF;
		bConsiderSCS = rData.bConsiderSCS;
		bTransferReac = rData.bTransferReac;
		bChangeLcAxOfTapSect = rData.bChangeLcAxOfTapSect;
		bAutoWallConstraint = rData.bAutoWallConstraint;
		bCalculateBeamStresses = rData.bCalculateBeamStresses;
		bClassicalFormulaSolid = TRUE;
		nBeamSectionChange = 1;
		nLoadStepType = 0;
		nNumberOfIncrement = 1;
		nIntermediateOutputRequest = 1;
		aLoadSteps.RemoveAll();
		AdvancedParam.Initialize();
		bConvFlag[0] = TRUE;
		bConvFlag[1] = FALSE;
		bConvFlag[2] = FALSE;
		dConvValue[0] = dblTrussTolerance;
		dConvValue[1] = 0.001;
		dConvValue[2] = 1.e-6;
		AdvancedParam.bUseDefaultNLParm = FALSE;
		AdvancedParam.nMaxBisectLevel = 0;
		bConsiderCable = FALSE;
	}
	void GetNlct(T_NLCT_DATA& NlctData)
	{
		NlctData.Initialize();

		NlctData.nLoadStepType = nLoadStepType;
		NlctData.nNumberOfIncrement = nNumberOfIncrement;
		NlctData.nIntermediateOutputRequest = nIntermediateOutputRequest;
		NlctData.aLoadSteps.RemoveAll();
		NlctData.aLoadSteps.Copy(aLoadSteps);
		NlctData.bConvFlag[0] = bConvFlag[0];
		NlctData.bConvFlag[1] = bConvFlag[1];
		NlctData.bConvFlag[2] = bConvFlag[2];

		NlctData.dConvValue[0] = dConvValue[0];
		NlctData.dConvValue[1] = dConvValue[1];
		NlctData.dConvValue[2] = dConvValue[2];

		NlctData.AdvancedParam = AdvancedParam;
	}
};
struct T_ACTL_D_RW
{
	BOOL bAutoConstraintNormal;
	BOOL bAutoConstraintRotation;
	int nIterationType;    // 0= Include Inactive Element 1=Exclude Inactive Element 
	// v720이후 nIterationType=2로 고정// MNET:2567 20061108 JHKIM
	int nTrussIteration;   // if nIterationType == 0 // v720이후 사용안함// MNET:2567 20061108 JHKIM
	int nIterPerLoad;      // if nIterationType == 1
	int nSubIterPerIter;   // if nIterationType == 1 // v720이후 사용안함// MNET:2567 20061108 JHKIM
	double dblTrussTolerance;
	BOOL bPerformAnalysis[20];
	BOOL bConsiderSecf;		// 6.7.1에서 추가
	BOOL bConsiderRbar;		// 6.7.1에서 추가
	BOOL bConsiderSectOffset; // 7.0.0에서 추가 (JP Only)
	BOOL bConsiderTDF;  // 7.0.0에서 추가 
	BOOL bConsiderSCS;  // 7.0.0에서 추가 (JP Only)
	BOOL bTransferReac; // 7.6.0에서 추가
	BOOL bChangeLcAxOfTapSect; // 7.9.8 (Civil Only)
	BOOL bAutoWallConstraint; // 8.3.5에서 추가 (Renewal Gen 벽 개구부 추가하면서 불연속 상하벽을 해석시 자동구속)
	BOOL bCalculateBeamStresses; // 8.5.0에서 추가 Calculate Beam Stresses (Von-Mises and Max-Shear)
	BOOL bClassicalFormulaSolid;
	int  nBeamSectionChange;
	int	nLoadStepType;
	int nNumberOfIncrement;
	int nIntermediateOutputRequest;
	int nNumLoadSteps;
	BOOL bConvFlag[3];        // 0:Displacement(U), 1:Load(P), 2:Work(W)
	double dConvValue[3];     // 0:Displacement(U), 1:Load(P), 2:Work(W)
	T_NLCT_NLCTRL_PARM AdvancedParam;
	BOOL bConsiderCable;  // Consider Initial Tension / Unstrained Initial Length for Cable Element (Equivalent Truss)

	void Get(T_ACTL_D& rData)
	{
		// T_ACTL_D_RW 데이터를 T_ACTL_D로 변환
		rData.bAutoConstraintNormal = bAutoConstraintNormal;
		rData.bAutoConstraintRotation = bAutoConstraintRotation;
		rData.nIterationType = nIterationType;
		rData.nTrussIteration = nTrussIteration;
		rData.nIterPerLoad = nIterPerLoad;
		rData.nSubIterPerIter = nSubIterPerIter;
		rData.dblTrussTolerance = dblTrussTolerance;
		memcpy(rData.bPerformAnalysis, bPerformAnalysis, sizeof(bPerformAnalysis));
		rData.bConsiderSecf = bConsiderSecf;
		rData.bConsiderRbar = bConsiderRbar;
		rData.bConsiderSectOffset = bConsiderSectOffset;
		rData.bConsiderTDF = bConsiderTDF;
		rData.bConsiderSCS = bConsiderSCS;
		rData.bTransferReac = bTransferReac;
		rData.bChangeLcAxOfTapSect = bChangeLcAxOfTapSect;
		rData.bAutoWallConstraint = bAutoWallConstraint;
		rData.bCalculateBeamStresses = bCalculateBeamStresses;
		rData.bClassicalFormulaSolid = bClassicalFormulaSolid;
		rData.nBeamSectionChange = nBeamSectionChange;
		rData.nLoadStepType = nLoadStepType;
		rData.nNumberOfIncrement = nNumberOfIncrement;
		rData.nIntermediateOutputRequest = nIntermediateOutputRequest;
		rData.aLoadSteps.SetSize(nNumLoadSteps);
		memcpy(rData.bConvFlag, bConvFlag, sizeof(bConvFlag));
		memcpy(rData.dConvValue, dConvValue, sizeof(dConvValue));
		rData.AdvancedParam = AdvancedParam;
		rData.bConsiderCable = bConsiderCable;
	}
	void Set(const T_ACTL_D& Data)
	{
		// T_ACTL_D 데이터를 T_ACTL_D_RW로 변환해줘
		bAutoConstraintNormal = Data.bAutoConstraintNormal;
		bAutoConstraintRotation = Data.bAutoConstraintRotation;
		nIterationType = Data.nIterationType;
		nTrussIteration = Data.nTrussIteration;
		nIterPerLoad = Data.nIterPerLoad;
		nSubIterPerIter = Data.nSubIterPerIter;
		dblTrussTolerance = Data.dblTrussTolerance;
		memcpy(bPerformAnalysis, Data.bPerformAnalysis, sizeof(bPerformAnalysis));
		bConsiderSecf = Data.bConsiderSecf;
		bConsiderRbar = Data.bConsiderRbar;
		bConsiderSectOffset = Data.bConsiderSectOffset;
		bConsiderTDF = Data.bConsiderTDF;
		bConsiderSCS = Data.bConsiderSCS;
		bTransferReac = Data.bTransferReac;
		bChangeLcAxOfTapSect = Data.bChangeLcAxOfTapSect;
		bAutoWallConstraint = Data.bAutoWallConstraint;
		bCalculateBeamStresses = Data.bCalculateBeamStresses;
		bClassicalFormulaSolid = Data.bClassicalFormulaSolid;
		nBeamSectionChange = Data.nBeamSectionChange;
		nLoadStepType = Data.nLoadStepType;
		nNumberOfIncrement = Data.nNumberOfIncrement;
		nIntermediateOutputRequest = Data.nIntermediateOutputRequest;
		nNumLoadSteps = Data.aLoadSteps.GetSize();
		memcpy(bConvFlag, Data.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, Data.dConvValue, sizeof(dConvValue));
		AdvancedParam = Data.AdvancedParam;
		bConsiderCable = Data.bConsiderCable;
	}
};

struct T_ACTL_UDRD_D
{
	T_ACTL_D data;
};

struct T_ACTL_UDRD_D_RW
{
	T_ACTL_D_RW data;
	void GetActl(T_ACTL_D& ACTLD)
	{
		data.Get(ACTLD);
	}
	void SetActl(const T_ACTL_D& ACTLD)
	{
		data.Set(ACTLD);
	}
};

struct T_ACTL_UNIT
{
	T_ACTL_UNIT()
	{
		bAutoConstraintNormal=D_UNITSYS_NONE;
		bAutoConstraintRotation=D_UNITSYS_NONE;
		nInteraionType=D_UNITSYS_NONE;
		nTrussIteration=D_UNITSYS_NONE;
		nIterPerLoad=D_UNITSYS_NONE;
		nSubIterPerIter=D_UNITSYS_NONE;
		dblTrussTolerance=D_UNITSYS_NONE;
		bPerformAnalysis=D_UNITSYS_NONE;
	}
	int bAutoConstraintNormal;
	int bAutoConstraintRotation;
	int nInteraionType;
	int nTrussIteration;
	int nIterPerLoad;
	int nSubIterPerIter;
	int dblTrussTolerance;
	int bPerformAnalysis;
};
#define HASHSIZEACTL 1

#define D_EIGV_NUMLC  50
#define T_EIGV_K unsigned int

struct T_EIGV_COMP
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
		bMassProp = TRUE;
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

struct T_EIGV_RITZ_LC
{
	int      nType;       // 0:Static Loadcase, 1:Ground Acc
	T_STLD_K LoadCaseKey; // if nType == 1, 1:Acc X, 2:Acc Y, 3:Acc Z
	int      nCount;
};
struct T_EIGV_RITZ_710
{
	T_EIGV_RITZ_LC LC[D_EIGV_NUMLC];
	BOOL           bIncludeForceVectors;
	int            nGenerationNum;
};
struct T_EIGV_RITZ
{
	T_EIGV_RITZ_LC LC[D_EIGV_NUMLC];
	BOOL           bIncludeForceVectors;
	int            nGenerationNum;
	BOOL           bSTRUM;
	void Initialize()
	{
		for (int i = 0; i < D_EIGV_NUMLC; i++)
		{
			LC[i].nType = 0; LC[i].LoadCaseKey = 0; LC[i].nCount = 0;
		}
		bIncludeForceVectors = FALSE;
		nGenerationNum = 0;
		bSTRUM=FALSE;
	}
	void Convert710(T_EIGV_RITZ_710& rData)
	{
		for (int i = 0; i < D_EIGV_NUMLC; i++)
		{
			LC[i].nType = rData.LC[i].nType; LC[i].LoadCaseKey = rData.LC[i].LoadCaseKey;
			LC[i].nCount = rData.LC[i].nCount; 
		}
		bIncludeForceVectors = rData.bIncludeForceVectors;
		nGenerationNum = rData.nGenerationNum;
		bSTRUM=FALSE;
	}
};
struct T_EIGV_D_530
{
	int nNumMode;
	double FrMin;
	double FrMax;
	int nNumItr;
	int nNPA;
	double SimTol;
	double FShift;
};
struct T_EIGV_D_630
{
	int nType;      // 0:Eigen Vectors-Subspace Iteration, 1:Ritz Vectors, 2:Eigen Vectors-Lanczos
	int nNumMode;   // Eigen : Number of Frequencies
	double FrMin;   // Eigen : Lower Frequency
	double FrMax;   // Eigen : Upper Frequency
	int nNumItr;    // Eigen : Number of Iteration
	int nNPA;       // Eigen : Subspace Dimension
	double SimTol;  // Eigen : Convergence Tolerance
	double FShift;  // Eigen : Frequency Shift
	T_EIGV_RITZ_710 Ritz;
};
struct T_EIGV_D_640
{
	int nType;      // 0:Eigen Vectors-Subspace Iteration, 1:Ritz Vectors, 2:Eigen Vectors-Lanczos
	int nNumMode;   // Eigen : Number of Frequencies
	double FrMin;   // Eigen : Lower Frequency
	double FrMax;   // Eigen : Upper Frequency
	int nNumItr;    // Eigen : Number of Iteration
	int nNPA;       // Eigen : Subspace Dimension
	double SimTol;  // Eigen : Convergence Tolerance
	double FShift;  // Eigen : Frequency Shift
	T_EIGV_RITZ_710 Ritz;
	double dFreqShiftD2E;  // 6.4.0 추가 : D2E용
	int    nNumPrintD2E;   // 6.4.0 추가 : D2E용
	int    nNumFileD2E;    // 6.4.0 추가 : D2E용
};
struct T_EIGV_D_710
{
	int nType;      // 0:Eigen Vectors-Subspace Iteration, 1:Ritz Vectors, 2:Eigen Vectors-Lanczos
	int nNumMode;   // Eigen : Number of Frequencies
	double FrMin;   // Eigen : Lower Frequency
	double FrMax;   // Eigen : Upper Frequency
	int nNumItr;    // Eigen : Number of Iteration
	int nNPA;       // Eigen : Subspace Dimension
	double SimTol;  // Eigen : Convergence Tolerance
	double FShift;  // Eigen : Frequency Shift
	T_EIGV_RITZ_710 Ritz;
	int    nTemp;     // 데이터 사이즈 구분용 변수
};
struct T_EIGV_D_930
{
	int nType;      // 0:Eigen Vectors-Subspace Iteration, 1:Ritz Vectors, 2:Eigen Vectors-Lanczos
	int nNumMode;   // Eigen : Number of Frequencies
	BOOL bMinMax;   // Eigen : Wheher Use MinMax or not
	double FrMin;   // Eigen : Lower Frequency
	double FrMax;   // Eigen : Upper Frequency
	int nNumItr;    // Eigen : Number of Iteration
	int nNPA;       // Eigen : Subspace Dimension
	double SimTol;  // Eigen : Convergence Tolerance
	double FShift;  // Eigen : Frequency Shift
	T_EIGV_RITZ Ritz;
	int    nTemp;     // 데이터 사이즈 구분용 변수
};
struct T_EIGV_D_930_1
{
	int nType;      // 0:Eigen Vectors-Subspace Iteration, 1:Ritz Vectors, 2:Eigen Vectors-Lanczos, 3:Complex 4:Complex group
	int nNumMode;   // Eigen : Number of Frequencies
	BOOL bMinMax;   // Eigen : Wheher Use MinMax or not
	double FrMin;   // Eigen : Lower Frequency
	double FrMax;   // Eigen : Upper Frequency
	int nNumItr;    // Eigen : Number of Iteration
	int nNPA;       // Eigen : Subspace Dimension
	double SimTol;  // Eigen : Convergence Tolerance
	double FShift;  // Eigen : Frequency Shift
	T_EIGV_RITZ Ritz; //7.1.1 버전으로 수정
	int    nTemp;     // 데이터 사이즈 구분용 변수

	T_EIGV_COMP CompMass;
};
struct T_EIGV_D
{
	enum enType
	{
		type_eigen_vectors_subspace_iteration = 0,
		type_ritz_vectors = 1,
		type_eigen_vectors_lanczos = 2,
	};
	int nType;      // 0:Eigen Vectors-Subspace Iteration, 1:Ritz Vectors, 2:Eigen Vectors-Lanczos, 3:Complex 4:Complex group
	int nNumMode;   // Eigen : Number of Frequencies
	BOOL bMinMax;   // Eigen : Wheher Use MinMax or not
	double FrMin;   // Eigen : Lower Frequency
	double FrMax;   // Eigen : Upper Frequency
	int nNumItr;    // Eigen : Number of Iteration
	int nNPA;       // Eigen : Subspace Dimension
	double SimTol;  // Eigen : Convergence Tolerance
	double FShift;  // Eigen : Frequency Shift
	T_EIGV_RITZ Ritz; //7.1.1 버전으로 수정
	int    nTemp;     // 데이터 사이즈 구분용 변수
//  double dFreqShiftD2E;  // 6.4.0 추가 : D2E용   => Dyna2E 를 위한 별도의 데이터(EVCT) 생성으로 삭제함
//  int    nNumPrintD2E;   // 6.4.0 추가 : D2E용   => Dyna2E 를 위한 별도의 데이터(EVCT) 생성으로 삭제함
//  int    nNumFileD2E;    // 6.4.0 추가 : D2E용   => Dyna2E 를 위한 별도의 데이터(EVCT) 생성으로 삭제함
//  int    nNumItrD2E;     // 6.5.0 추가 : D2E용   => Dyna2E 를 위한 별도의 데이터(EVCT) 생성으로 삭제함
//  double dToleranceD2E;  // 6.5.0 추가 : D2E용   => Dyna2E 를 위한 별도의 데이터(EVCT) 생성으로 삭제함
	BOOL bElement;
	T_EIGV_COMP CompMass;
	void Initialize()
	{
		nType = type_eigen_vectors_lanczos;
		nNumMode=1;
		FrMin=0.;
		FrMax=1600.;
		nNumItr=20;
		nNPA=1;
		SimTol=1.e-10;
		FShift=1.0;    
		Ritz.Initialize();
		nTemp = 0;
		bMinMax=FALSE;
		bElement = FALSE;
		CompMass.Initialize();
	}
	void Convert530(T_EIGV_D_530& rData)
	{
		nType = 0;
		nNumMode=rData.nNumMode;
		FrMin=rData.FrMin;
		FrMax=rData.FrMax;
		nNumItr=rData.nNumItr;
		nNPA=rData.nNPA;
		SimTol=rData.SimTol;
		FShift=rData.FShift;
		Ritz.Initialize();
		nTemp = 0;
		bMinMax=FALSE;
	}
	void Convert630(T_EIGV_D_630& rData)
	{
		nType = rData.nType;
		nNumMode=rData.nNumMode;
		FrMin=rData.FrMin;
		FrMax=rData.FrMax;
		nNumItr=rData.nNumItr;
		nNPA=rData.nNPA;
		SimTol=rData.SimTol;
		FShift=rData.FShift;
		Ritz.Convert710(rData.Ritz);
		nTemp = 0;
		bMinMax=FALSE;
	}
	void Convert640(T_EIGV_D_640& rData)
	{
		nType = rData.nType;
		nNumMode=rData.nNumMode;
		FrMin=rData.FrMin;
		FrMax=rData.FrMax;
		nNumItr=rData.nNumItr;
		nNPA=rData.nNPA;
		SimTol=rData.SimTol;
		FShift=rData.FShift;
		Ritz.Convert710(rData.Ritz);
		nTemp = 0;
		bMinMax=FALSE;
	}
	void Convert710(T_EIGV_D_710& rData)
	{
		nType = rData.nType;
		nNumMode=rData.nNumMode;
		FrMin=rData.FrMin;
		FrMax=rData.FrMax;
		nNumItr=rData.nNumItr;
		nNPA=rData.nNPA;
		SimTol=rData.SimTol;
		FShift=rData.FShift;
		Ritz.Convert710(rData.Ritz);
		nTemp = rData.nTemp;
		bMinMax=FALSE;
	}
	void Convert930(T_EIGV_D_930& rData)
	{
		nType = rData.nType;
		nNumMode = rData.nNumMode;
		FrMin = rData.FrMin;
		FrMax = rData.FrMax;
		nNumItr = rData.nNumItr;
		nNPA = rData.nNPA;
		SimTol = rData.SimTol;
		FShift = rData.FShift;
		Ritz = rData.Ritz;
		nTemp = rData.nTemp;
		bMinMax = rData.bMinMax;
		bElement = FALSE;
		CompMass.Initialize();

	}
	void Convert930_1(T_EIGV_D_930_1& rData)
	{
		nType = rData.nType;
		nNumMode = rData.nNumMode;
		FrMin = rData.FrMin;
		FrMax = rData.FrMax;
		nNumItr = rData.nNumItr;
		nNPA = rData.nNPA;
		SimTol = rData.SimTol;
		FShift = rData.FShift;
		Ritz = rData.Ritz;
		nTemp = rData.nTemp;
		bMinMax = rData.bMinMax;
		bElement = FALSE;
		CompMass = rData.CompMass;
	}
};
struct T_EIGV_UDRD_D_530
{
	T_EIGV_D_530 data;
};
struct T_EIGV_UDRD_D_630
{
	T_EIGV_D_630 data;
};
struct T_EIGV_UDRD_D_640
{
	T_EIGV_D_640 data;
};
struct T_EIGV_UDRD_D_710
{
	T_EIGV_D_710 data;
};
struct T_EIGV_UDRD_D_930
{
	T_EIGV_D_930 data;
};
struct T_EIGV_UDRD_D_930_1
{
	T_EIGV_D_930_1 data;
};
struct T_EIGV_UDRD_D
{
	T_EIGV_D data;
};
struct T_EIGV_UNIT
{
	T_EIGV_UNIT()
	{
		nCount=7;
		nUnit[0]=D_UNITSYS_NONE;
		nUnit[1]=D_UNITSYS_BASE_CPS;
		nUnit[2]=D_UNITSYS_BASE_CPS;
		nUnit[3]=D_UNITSYS_NONE;
		nUnit[4]=D_UNITSYS_NONE;
		nUnit[5]=D_UNITSYS_NONE;
		nUnit[6]=D_UNITSYS_BASE_CPS;
		nNumMode=nUnit[0];
		FrMin=nUnit[1];
		FrMax=nUnit[2];
		nNumItr=nUnit[3];
		nNPA=nUnit[4];
		SimTol=nUnit[5];
		FShift=nUnit[6];
	}
	int nNumMode;
	int FrMin;
	int FrMax;
	int nNumItr;
	int nNPA;
	int SimTol;
	int FShift;
	int nCount;
	int nUnit[7];
};
#define HASHSIZEEIGV 1

#define D_PDEL_NUMPDEL_OLD  15
#define D_PDEL_NUMPDEL_MAX  100
#define T_PDEL_K unsigned int
struct T_PDEL_BASE
{
	T_STLD_K LoadCaseKey;
	double dblFactor;
	void Initialize()
	{
		LoadCaseKey=0;
		dblFactor=0;
	}
};

struct T_PDEL_D_940
{
	int nNumItr;
	double ConvTol;
	int nNumPdelCase;
	T_PDEL_BASE PdelCase[D_PDEL_NUMPDEL_OLD];
};

struct T_PDEL_D
{
	int nNumItr;
	double ConvTol;
	int nNumPdelCase;
	CArray<T_PDEL_BASE, T_PDEL_BASE&> PdelCase;

	void Initialize()
	{
		nNumItr=5;
		ConvTol=1.e-5;
		nNumPdelCase=0;
		PdelCase.RemoveAll();
	}
	T_PDEL_D() { Initialize(); }
	T_PDEL_D(const T_PDEL_D &src) { *this = src; }
	T_PDEL_D& operator=(const T_PDEL_D &src)
	{
		nNumItr = src.nNumItr;
		ConvTol = src.ConvTol;
		nNumPdelCase = src.nNumPdelCase;
		PdelCase.Copy(src.PdelCase);
		return *this;
	}

	void Convert940(T_PDEL_D_940& rData)
	{
		nNumItr = rData.nNumItr;
		ConvTol = rData.ConvTol;
		nNumPdelCase = rData.nNumPdelCase;

		PdelCase.SetSize(nNumPdelCase);
		for(int i=0; i< nNumPdelCase; i++)
		{
			PdelCase[i] = rData.PdelCase[i];
		}
	}
};

struct T_PDEL_D_RW
{
	int nNumItr;
	double ConvTol;
	int nNumPdelCase;

	void GetPdel(T_PDEL_D &rData)const
	{
		rData.nNumItr = nNumItr;
		rData.ConvTol = ConvTol;
		rData.nNumPdelCase = nNumPdelCase;

		rData.PdelCase.RemoveAll();
	}
	void SetPdel(const T_PDEL_D &rData)
	{
		nNumItr = rData.nNumItr;
		ConvTol = rData.ConvTol;
		nNumPdelCase = rData.nNumPdelCase;
		ASSERT(nNumPdelCase == rData.PdelCase.GetCount());
	}
};

struct T_PDEL_UDRD_D
{
	T_PDEL_D data;
};
struct T_PDEL_UNIT
{
	T_PDEL_UNIT()
	{
		nCount=5;
		nUnit[0]=D_UNITSYS_NONE;
		nUnit[1]=D_UNITSYS_NONE;
		nUnit[2]=D_UNITSYS_NONE;
		nUnit[3]=D_UNITSYS_NONE;
		nUnit[4]=D_UNITSYS_NONE;
		nNumItr=nUnit[0];
		ConvTol=nUnit[1];
		nNumPdelCase=nUnit[2];
		LoadCaseKey=nUnit[3];
		dblFactor=nUnit[4];
	}
	int nNumItr;
	int ConvTol;
	int nNumPdelCase;
	int LoadCaseKey;
	int dblFactor;
	int nCount;
	int nUnit[5];
};
#define HASHSIZEPDEL 1

#define D_BUCK_NUMBUCK  15
#define T_BUCK_K unsigned int
struct T_BUCK_BASE_630
{
	T_STLD_K LoadCaseKey;
	double dblFactor;
	void Initialize()
	{
		LoadCaseKey=0;
		dblFactor=0;
	}
};
struct T_BUCK_BASE
{
	T_STLD_K LoadCaseKey;
	double dblFactor;
	int    nLoadType;     // 640추가 : 0=Variable, 1=Constant
	void Initialize()
	{
		LoadCaseKey=0;
		dblFactor=0;
		nLoadType = 0;
	}
	void Convert630(T_BUCK_BASE_630& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		dblFactor = rData.dblFactor;
		nLoadType = 0;
	}
};
struct T_BUCK_D_630
{
	int nNumMode;
	int nNumItr;
	double ConvTol;
	int nNumBuckCase;
	T_BUCK_BASE_630 BuckCase[D_BUCK_NUMBUCK];
};
struct T_BUCK_D_710
{
	int nNumMode;
	int nNumItr;
	double ConvTol;
	int nNumBuckCase;
	T_BUCK_BASE BuckCase[D_BUCK_NUMBUCK];
};
struct T_BUCK_D_730
{
	int nNumMode;
	int nNumItr;
	double ConvTol;
	int nNumBuckCase;
	T_BUCK_BASE BuckCase[D_BUCK_NUMBUCK];
	BOOL bPositive;
};
struct T_BUCK_D_750
{
	int nNumMode;
	int nNumItr;
	double ConvTol;
	int nNumBuckCase;
	T_BUCK_BASE BuckCase[D_BUCK_NUMBUCK];
	BOOL bPositive;
	BOOL bConsiderAxialOnly;
};
struct T_BUCK_D
{
	int nNumMode;
	int nNumItr;
	double ConvTol;
	int nNumBuckCase;
	T_BUCK_BASE BuckCase[D_BUCK_NUMBUCK];
	BOOL bPositive; // TRUE: Positive Value Only, FALSE: Search
	BOOL bConsiderAxialOnly;
	double dLoadFactorFrom; // 760
	double dLoadFactorTo;   // 760
	BOOL bSturmSeq;         // 760
	void Initialize()
	{
		nNumMode=0;
		nNumItr=30;
		ConvTol=1.e-10;	// by JBSEON-051020 req by 정진상
		nNumBuckCase=0;
		for(int i=0; i<D_BUCK_NUMBUCK; i++)BuckCase[i].Initialize();
		bPositive=FALSE; // FALSE from 760
		bConsiderAxialOnly=FALSE;
		dLoadFactorFrom=0.;
		dLoadFactorTo=0.;
		bSturmSeq = FALSE;
	}
	void Convert750(T_BUCK_D_750& rData)
	{
		nNumMode = rData.nNumMode;
		nNumItr = rData.nNumItr;
		ConvTol = rData.ConvTol;
		nNumBuckCase = rData.nNumBuckCase;
		for(int i = 0; i < D_BUCK_NUMBUCK; i++)
		{
			BuckCase[i].LoadCaseKey = rData.BuckCase[i].LoadCaseKey;
			BuckCase[i].dblFactor = rData.BuckCase[i].dblFactor;
			BuckCase[i].nLoadType = rData.BuckCase[i].nLoadType;      
		}
		bPositive=rData.bPositive;
		bConsiderAxialOnly=rData.bConsiderAxialOnly;
		dLoadFactorFrom=0.;
		dLoadFactorTo=0.;
		bSturmSeq = FALSE;
	}
	void Convert730(T_BUCK_D_730& rData)
	{
		nNumMode = rData.nNumMode;
		nNumItr = rData.nNumItr;
		ConvTol = rData.ConvTol;
		nNumBuckCase = rData.nNumBuckCase;
		for(int i = 0; i < D_BUCK_NUMBUCK; i++)
		{
			BuckCase[i].LoadCaseKey = rData.BuckCase[i].LoadCaseKey;
			BuckCase[i].dblFactor = rData.BuckCase[i].dblFactor;
			BuckCase[i].nLoadType = rData.BuckCase[i].nLoadType;      
		}
		bPositive=rData.bPositive;
		bConsiderAxialOnly=FALSE;
		dLoadFactorFrom=0.;
		dLoadFactorTo=0.;
		bSturmSeq = FALSE;
	}
	void Convert710(T_BUCK_D_710& rData)
	{
		nNumMode = rData.nNumMode;
		nNumItr = rData.nNumItr;
		ConvTol = rData.ConvTol;
		nNumBuckCase = rData.nNumBuckCase;
		for(int i = 0; i < D_BUCK_NUMBUCK; i++)
		{
			BuckCase[i].LoadCaseKey = rData.BuckCase[i].LoadCaseKey;
			BuckCase[i].dblFactor = rData.BuckCase[i].dblFactor;
			BuckCase[i].nLoadType = rData.BuckCase[i].nLoadType;      
		}
		bPositive=FALSE;
		bConsiderAxialOnly=FALSE;
		dLoadFactorFrom=0.;
		dLoadFactorTo=0.;
		bSturmSeq = FALSE;
	}

	void Convert630(T_BUCK_D_630& rData)
	{
		nNumMode = rData.nNumMode;
		nNumItr = rData.nNumItr;
		ConvTol = rData.ConvTol;
		nNumBuckCase = rData.nNumBuckCase;
		for(int i = 0; i < D_BUCK_NUMBUCK; i++)
		{
			BuckCase[i].LoadCaseKey = rData.BuckCase[i].LoadCaseKey;
			BuckCase[i].dblFactor = rData.BuckCase[i].dblFactor;
			BuckCase[i].nLoadType = 0;      
		}
		bPositive=FALSE;
		bConsiderAxialOnly=FALSE;
		dLoadFactorFrom=0.;
		dLoadFactorTo=0.;
		bSturmSeq = FALSE;
	}
};
struct T_BUCK_UDRD_D_630
{
	T_BUCK_D_630 data;
};
struct T_BUCK_UDRD_D_710
{
	T_BUCK_D_710 data;
};
struct T_BUCK_UDRD_D_730
{
	T_BUCK_D_730 data;
};
struct T_BUCK_UDRD_D_750
{
	T_BUCK_D_750 data;
};
struct T_BUCK_UDRD_D
{
	T_BUCK_D data;
};
struct T_BUCK_UNIT
{
	T_BUCK_UNIT()
	{
		nCount=6;
		nUnit[0]=D_UNITSYS_NONE;
		nUnit[1]=D_UNITSYS_NONE;
		nUnit[2]=D_UNITSYS_NONE;
		nUnit[3]=D_UNITSYS_NONE;
		nUnit[4]=D_UNITSYS_NONE;
		nUnit[5]=D_UNITSYS_NONE;
		nNumMode=nUnit[0];
		nNumItr=nUnit[1];
		ConvTol=nUnit[2];
		nNumBuckCase=nUnit[3];
		LoadCaseKey=nUnit[4];
		dblFactor=nUnit[5];
	}
	int nNumMode;
	int nNumItr;
	int ConvTol;
	int nNumBuckCase;
	int LoadCaseKey;
	int dblFactor;
	int nCount;
	int nUnit[6];
};
#define HASHSIZEBUCK 1

#define D_CSBK_NUMBUCK  15
#define T_CSBK_K unsigned int
struct T_CSBK_BASE
{
	T_STLD_K LoadCaseKey;	// General Static 인 경우 STLD_K 사용.
	                        // Construction 인 경우는 처리 필요.
	double dblFactor;
	int    nLoadType;       // 640추가 : 0=Variable, 1=Constant
	int    nCSType;         // 0=General Static, 1=Construction (summation, dead), 2=Construction Erection
	void Initialize()
	{
		LoadCaseKey = 0;
		dblFactor = 0;
		nLoadType = 0;
		nCSType = 0;
	}
};

struct T_CSBK_D
{
	int nNumMode;
	int nNumItr;
	double ConvTol;
	int nNumSTBuckCase;

	T_CSBK_BASE STBuckCase[D_CSBK_NUMBUCK];
	CArray<T_CSBK_BASE, T_CSBK_BASE&> CSBucKCase;
	BOOL bPositive; // TRUE: Positive Value Only, FALSE: Search
	BOOL bConsiderAxialOnly;
	double dLoadFactorFrom;
	double dLoadFactorTo;
	BOOL bSturmSeq;

	void Initialize()
	{
		nNumMode = 0;
		nNumItr = 30;
		ConvTol = 1.e-10;
		nNumSTBuckCase = 0;
		for (int i = 0; i < D_CSBK_NUMBUCK; i++)STBuckCase[i].Initialize();
		CSBucKCase.RemoveAll();
		bPositive = FALSE;
		bConsiderAxialOnly = FALSE;
		dLoadFactorFrom = 0.;
		dLoadFactorTo = 0.;
		bSturmSeq = FALSE;
	}

	T_CSBK_D() { Initialize(); }
	T_CSBK_D(const T_CSBK_D& src) { *this = src; }
	T_CSBK_D& operator=(const T_CSBK_D& src)
	{
		nNumMode = src.nNumMode;
		nNumItr = src.nNumItr;
		ConvTol = src.ConvTol;
		nNumSTBuckCase = src.nNumSTBuckCase;
		memcpy(STBuckCase, src.STBuckCase, sizeof(STBuckCase));

		CSBucKCase.Copy(src.CSBucKCase);

		bPositive = src.bPositive;
		bConsiderAxialOnly = src.bConsiderAxialOnly;
		dLoadFactorFrom = src.dLoadFactorFrom;
		dLoadFactorTo = src.dLoadFactorTo;
		bSturmSeq = src.bSturmSeq;
		return *this;
	}

	BOOL operator!=(const T_CSBK_D& src)const
	{
		if (nNumMode != src.nNumMode) return TRUE;
		if (nNumItr != src.nNumItr) return TRUE;
		if (ConvTol != src.ConvTol) return TRUE;

		if (nNumSTBuckCase != src.nNumSTBuckCase) return TRUE;
		for (int i=0; i < nNumSTBuckCase; i++)
		{
			if (STBuckCase[i].LoadCaseKey != src.STBuckCase[i].LoadCaseKey) return TRUE;
			if (STBuckCase[i].dblFactor != src.STBuckCase[i].dblFactor) return TRUE;
			if (STBuckCase[i].nLoadType != src.STBuckCase[i].nLoadType) return TRUE;
			if (STBuckCase[i].nCSType != src.STBuckCase[i].nCSType) return TRUE;
		}

		if (CSBucKCase.GetSize() != src.CSBucKCase.GetSize()) return TRUE;
		for (int i = 0; i < CSBucKCase.GetSize(); i++)
		{
			if (CSBucKCase[i].LoadCaseKey != src.CSBucKCase[i].LoadCaseKey) return TRUE;
			if (CSBucKCase[i].dblFactor != src.CSBucKCase[i].dblFactor) return TRUE;
			if (CSBucKCase[i].nLoadType != src.CSBucKCase[i].nLoadType) return TRUE;
			if (CSBucKCase[i].nCSType != src.CSBucKCase[i].nCSType) return TRUE;
		}
		if (bPositive != src.bPositive) return TRUE;
		if (bConsiderAxialOnly != src.bConsiderAxialOnly) return TRUE;
		if (dLoadFactorFrom != src.dLoadFactorFrom) return TRUE;
		if (dLoadFactorTo != src.dLoadFactorTo) return TRUE;
		if (bSturmSeq != src.bSturmSeq) return TRUE;
		return FALSE;
	}
};

struct T_CSBK_D_RW
{
	int nNumMode;
	int nNumItr;
	double ConvTol;
	int nNumSTBuckCase;
	T_CSBK_BASE STBuckCase[D_CSBK_NUMBUCK];

	int nNumCSBuckCase;

	BOOL bPositive; // TRUE: Positive Value Only, FALSE: Search
	BOOL bConsiderAxialOnly;
	double dLoadFactorFrom;
	double dLoadFactorTo;
	BOOL bSturmSeq;

	void Get(T_CSBK_D& rData)const
	{
		rData.nNumMode = nNumMode;
		rData.nNumItr = nNumItr;
		rData.ConvTol = ConvTol;
		rData.nNumSTBuckCase = nNumSTBuckCase;
		memcpy(rData.STBuckCase, STBuckCase, sizeof(STBuckCase));
		rData.CSBucKCase.SetSize(nNumCSBuckCase);

		rData.bPositive = bPositive;
		rData.bConsiderAxialOnly = bConsiderAxialOnly;
		rData.dLoadFactorFrom = dLoadFactorFrom;
		rData.dLoadFactorTo = dLoadFactorTo;
		rData.bSturmSeq = bSturmSeq;
	}
	void Set(const T_CSBK_D& rData)
	{
		nNumMode = rData.nNumMode;
		nNumItr = rData.nNumItr;
		ConvTol = rData.ConvTol;
		nNumSTBuckCase = rData.nNumSTBuckCase;
		memcpy(STBuckCase, rData.STBuckCase, sizeof(STBuckCase));
		nNumCSBuckCase = (int)rData.CSBucKCase.GetSize();

		bPositive = rData.bPositive;
		bConsiderAxialOnly = rData.bConsiderAxialOnly;
		dLoadFactorFrom = rData.dLoadFactorFrom;
		dLoadFactorTo = rData.dLoadFactorTo;
		bSturmSeq = rData.bSturmSeq;
	}
};

struct T_CSBK_UDRD_D
{
	T_CSBK_K key;
	T_CSBK_D data;
};

struct T_CSBK_UDRD_D_RW
{
	T_CSBK_K key;
	T_CSBK_D_RW data;
	void GetCsbk(T_CSBK_K& rKey, T_CSBK_D& rData)
	{
		rKey = key;
		data.Get(rData);
	}
	void SetCsbk(const T_CSBK_K Key, const T_CSBK_D& rData)
	{
		key = Key;
		data.Set(rData);
	}
};

//struct T_CSBK_UNIT
//{
//	T_CSBK_UNIT()
//	{
//		nCount = 6;
//		nUnit[0] = D_UNITSYS_NONE;
//		nUnit[1] = D_UNITSYS_NONE;
//		nUnit[2] = D_UNITSYS_NONE;
//		nUnit[3] = D_UNITSYS_NONE;
//		nUnit[4] = D_UNITSYS_NONE;
//		nUnit[5] = D_UNITSYS_NONE;
//		nUnit[6] = D_UNITSYS_NONE;
//		nNumMode = nUnit[0];
//		nNumItr = nUnit[1];
//		ConvTol = nUnit[2];
//		nNumSTBuckCase = nUnit[3];
//		nNumCSBuckCase = nUnit[4];
//		LoadCaseKey = nUnit[5];
//		dblFactor = nUnit[6];
//	}
//	int nNumMode;
//	int nNumItr;
//	int ConvTol;
//	int nNumSTBuckCase;
//	int nNumCSBuckCase;
//	int LoadCaseKey;
//	int dblFactor;
//	int nCount;
//	int nUnit[7];
//};
#define HASHSIZECSBK 101


#define D_SBCT_NUMSBCT  15
#define T_SBCT_K unsigned int
struct T_SBCT_BASE
{
	T_STLD_K LoadCaseKey;
	double dblFactor;
	void Initialize()
	{
		LoadCaseKey=0;
		dblFactor=0;
	}
};
struct T_SBCT_DISP 
{
	UINT  nGrupK;
	int   nDispType;  // 0: Zero Disp., 1: User Defined Disp.
	CArray<UINT, UINT>  aNode;
	CArray<double, double> aZDisp;
	void Initialize()
	{
		nGrupK    =0;
		nDispType =0;
		aNode.RemoveAll();
		aZDisp.RemoveAll();
	}
	T_SBCT_DISP() { Initialize(); }
	T_SBCT_DISP(const T_SBCT_DISP &src) { *this = src; }
	T_SBCT_DISP& operator=(const T_SBCT_DISP &src)
	{
		nGrupK = src.nGrupK;
		nDispType = src.nDispType;
		aNode.Copy(src.aNode);
		aZDisp.Copy(src.aZDisp);
		return *this;
	}
};
struct T_SBCT_DISP_RW
{
	UINT  nGrupK;
	int   nDispType;
	int   nCount_aNode;
	int   nCount_aZDisp;
	void Get(T_SBCT_DISP &rData)
	{
		rData.nGrupK  = nGrupK;
		rData.nDispType  = nDispType;
		
		rData.aNode.SetSize(nCount_aNode);
		rData.aZDisp.SetSize(nCount_aZDisp);
	}
	void Set(T_SBCT_DISP Data)
	{
		nGrupK  = Data.nGrupK;
		nDispType  = Data.nDispType;

		nCount_aNode = (int)Data.aNode.GetSize();
		nCount_aZDisp = (int)Data.aZDisp.GetSize();
	}
};
struct T_SBCT_D_692
{
	int nNumItr;
	double ConvTol;
	UINT GrupKey;      // Structure Group : Node 만 사용
	UINT SagGrupKey;   // Structure Group : Node 만 사용
	int nNumSbctCase;
	T_SBCT_BASE SbctCase[D_SBCT_NUMSBCT];
	void Initialize()
	{
		nNumItr=5;
		ConvTol=1.e-5;
		GrupKey = 0;
		SagGrupKey = 0;
		nNumSbctCase=0;
		for(int i=0; i<D_SBCT_NUMSBCT; i++) SbctCase[i].Initialize();
	}
};
struct T_SBCT_D_712
{
	int nNumItr;
	double ConvTol;
	UINT GrupKey;    
	UINT SagGrupKey; 
	
	BOOL bConsHor;
	UINT CableGrupKey;
	double dHorForce;
	
	int nNumSbctCase;
	T_SBCT_BASE SbctCase[D_SBCT_NUMSBCT];
};

struct T_SBCT_D_785
{
	int nNumItr;
	double ConvTol;
	UINT GrupKey;      // Structure Group : Node 만 사용
	UINT SagGrupKey;   // Structure Group : Node 만 사용
	
	BOOL bConsHor;      // 7.0.0에서 추가
	UINT CableGrupKey;  // 7.0.0에서 추가 //Structure Group (Cable Gruoup) : Elem 만 사용
	double dHorForce;   // 7.0.0에서 추가
	
	BOOL bHanger;       // 7.2.0에서 추가
	UINT HangerGrupKey; // 7.2.0에서 추가 
	
	int nNumSbctCase;
	T_SBCT_BASE SbctCase[D_SBCT_NUMSBCT];
};

struct T_SBCT_D_790
{
	int nNumItr;
	double ConvTol;
	UINT GrupKey;      // Structure Group : Node 만 사용
	UINT SagGrupKey;   // Structure Group : Node 만 사용
	
	BOOL bConsHor;      // 7.0.0에서 추가
	UINT CableGrupKey;  // 7.0.0에서 추가 //Structure Group (Cable Gruoup) : Elem 만 사용
	double dHorForce;   // 7.0.0에서 추가
	
	BOOL bHanger;       // 7.2.0에서 추가
	UINT HangerGrupKey; // 7.2.0에서 추가
	int  nCount_aHangerZDisp; // 7.9.0에서 추가, Hanger Bottom Point Z-Disp. => 790 다음버젼에서 사용하기로 함. 20101012 KIMJM
	
	int nNumSbctCase;
	T_SBCT_BASE SbctCase[D_SBCT_NUMSBCT];
};

struct T_SBCT_D_800
{
	int nNumItr;
	double ConvTol;
	UINT GrupKey;      // Structure Group : Node 만 사용
	UINT SagGrupKey;   // Structure Group : Node 만 사용
	
	BOOL bConsHor;      // 7.0.0에서 추가
	UINT CableGrupKey;  // 7.0.0에서 추가 //Structure Group (Cable Gruoup) : Elem 만 사용
	double dHorForce;   // 7.0.0에서 추가
	
	BOOL bHanger;       // 7.2.0에서 추가
	UINT HangerGrupKey; // 7.2.0에서 추가
	int  nCount_aHangerZDisp; // 7.9.0에서 추가, Hanger Bottom Point Z-Disp. => 790 다음버젼에서 사용하기로 함. 20101012 KIMJM

	UINT CableGrupSubKey; // 8.0.0에서 추가
	
	int nNumSbctCase;
	T_SBCT_BASE SbctCase[D_SBCT_NUMSBCT];
};

struct T_SBCT_D_940
{
	int nNumItr;
	double ConvTol;
	UINT GrupKey;      // Structure Group : Node 만 사용
	UINT SagGrupKey;   // Structure Group : Node 만 사용

	BOOL bConsHor;      // 7.0.0에서 추가
	UINT CableGrupKey;  // 7.0.0에서 추가 //Structure Group (Cable Gruoup) : Elem 만 사용
	double dHorForce;   // 7.0.0에서 추가

	BOOL bHanger;       // 7.2.0에서 추가
	UINT HangerGrupKey; // 7.2.0에서 추가
	int  nCount_aHangerZDisp; // 7.9.0에서 추가, Hanger Bottom Point Z-Disp. => 790 다음버젼에서 사용하기로 함. 20101012 KIMJM

	UINT CableGrupSubKey; // 8.0.0에서 추가
	int nAnalMethod; // 8.0.1에서 추가, 0: Intial Force, 1: Optimization

	int nNumSbctCase;
	T_SBCT_BASE SbctCase[D_SBCT_NUMSBCT];
};

struct T_SBCT_D
{
	int nNumItr;
	double ConvTol;
	UINT GrupKey;      // Structure Group : Node 만 사용
	UINT SagGrupKey;   // Structure Group : Node 만 사용
	
	BOOL bConsHor;      // 7.0.0에서 추가
	UINT CableGrupKey;  // 7.0.0에서 추가 //Structure Group (Cable Gruoup) : Elem 만 사용
	double dHorForce;   // 7.0.0에서 추가
	
	BOOL bHanger;       // 7.2.0에서 추가
	UINT HangerGrupKey; // 7.2.0에서 추가
	CArray<T_SBCT_DISP, T_SBCT_DISP&> aHangerZDisp; // 7.9.0에서 추가, Hanger Bottom Point Z-Disp. => 790 다음버젼에서 사용하기로 함. 20101012 KIMJM

	UINT CableGrupSubKey; // 8.0.0에서 추가
	int nAnalMethod; // 8.0.1에서 추가, 0: Intial Force, 1: Optimization
	
	int nUpdateType;//9.4.5 GEN 0: Update Y,Z Coordinate of Node, 1: Update X,Y,Z Coordinate of Node. 

	int nNumSbctCase;
	T_SBCT_BASE SbctCase[D_SBCT_NUMSBCT];
	void Initialize()
	{
		nNumItr=5;
		ConvTol=1.e-5;
		GrupKey = 0;
		SagGrupKey = 0;
		bConsHor=FALSE;
		CableGrupKey=0;
		dHorForce=0.0;
		nNumSbctCase=0;
		bHanger = FALSE;
		HangerGrupKey = 0;
		for(int i=0; i<D_SBCT_NUMSBCT; i++) SbctCase[i].Initialize();
		aHangerZDisp.RemoveAll();
		CableGrupSubKey = 0;
		nAnalMethod = 0;
		nUpdateType = 0;
	}
	void Convert692(T_SBCT_D_692& rData)
	{
		nNumItr = rData.nNumItr;
		ConvTol = rData.ConvTol;
		GrupKey = rData.GrupKey;
		SagGrupKey = rData.SagGrupKey;
		nNumSbctCase = rData.nNumSbctCase;
		memcpy(SbctCase, rData.SbctCase, sizeof(SbctCase));
		bConsHor = FALSE;
		CableGrupKey = 0;
		dHorForce = 0.0;
		bHanger = FALSE;
		HangerGrupKey = 0;
		aHangerZDisp.RemoveAll();
		CableGrupSubKey = 0;
		nAnalMethod = 0;
	}
	void Convert712(T_SBCT_D_712& rData)
	{
		nNumItr = rData.nNumItr;
		ConvTol = rData.ConvTol;
		GrupKey = rData.GrupKey;
		SagGrupKey = rData.SagGrupKey;
		nNumSbctCase = rData.nNumSbctCase;
		memcpy(SbctCase, rData.SbctCase, sizeof(SbctCase));
		bConsHor = rData.bConsHor;
		CableGrupKey = rData.CableGrupKey;
		dHorForce = rData.dHorForce;
		bHanger = FALSE;
		HangerGrupKey = 0;
		aHangerZDisp.RemoveAll();
		CableGrupSubKey = 0;
		nAnalMethod = 0;
	}
	void Convert785(T_SBCT_D_785& rData)
	{
		nNumItr = rData.nNumItr;
		ConvTol = rData.ConvTol;
		GrupKey = rData.GrupKey;
		SagGrupKey = rData.SagGrupKey;
		nNumSbctCase = rData.nNumSbctCase;
		memcpy(SbctCase, rData.SbctCase, sizeof(SbctCase));
		bConsHor = rData.bConsHor;
		CableGrupKey = rData.CableGrupKey;
		dHorForce = rData.dHorForce;
		bHanger = FALSE;
		HangerGrupKey = 0;
		aHangerZDisp.RemoveAll();
		CableGrupSubKey = 0;
		nAnalMethod = 0;
	}
	void Convert790(T_SBCT_D_790& rData)
	{
		nNumItr = rData.nNumItr;
		ConvTol = rData.ConvTol;
		GrupKey = rData.GrupKey;
		SagGrupKey = rData.SagGrupKey;
		nNumSbctCase = rData.nNumSbctCase;
		memcpy(SbctCase, rData.SbctCase, sizeof(SbctCase));
		bConsHor = rData.bConsHor;
		CableGrupKey = rData.CableGrupKey;
		dHorForce = rData.dHorForce;
		bHanger = rData.bHanger;
		HangerGrupKey = rData.HangerGrupKey;
		aHangerZDisp.RemoveAll();
//    aHangerZDisp.Copy(rData.aHangerZDisp);
		CableGrupSubKey = 0;
		nAnalMethod = 0;
	}
	void Convert800(T_SBCT_D_800& rData)
	{
		nNumItr = rData.nNumItr;
		ConvTol = rData.ConvTol;
		GrupKey = rData.GrupKey;
		SagGrupKey = rData.SagGrupKey;
		nNumSbctCase = rData.nNumSbctCase;
		memcpy(SbctCase, rData.SbctCase, sizeof(SbctCase));
		bConsHor = rData.bConsHor;
		CableGrupKey = rData.CableGrupKey;
		dHorForce = rData.dHorForce;
		bHanger = rData.bHanger;
		HangerGrupKey = rData.HangerGrupKey;
		aHangerZDisp.RemoveAll();
		//    aHangerZDisp.Copy(rData.aHangerZDisp);
		CableGrupSubKey = rData.CableGrupKey;
		nAnalMethod = 0;
	}
	void Convert940(T_SBCT_D_940& rData)
	{
		nNumItr = rData.nNumItr;
		ConvTol = rData.ConvTol;
		GrupKey = rData.GrupKey;
		SagGrupKey = rData.SagGrupKey;
		nNumSbctCase = rData.nNumSbctCase;
		memcpy(SbctCase, rData.SbctCase, sizeof(SbctCase));
		bConsHor = rData.bConsHor;
		CableGrupKey = rData.CableGrupKey;
		dHorForce = rData.dHorForce;
		bHanger = rData.bHanger;
		HangerGrupKey = rData.HangerGrupKey;
		aHangerZDisp.RemoveAll();
		CableGrupSubKey = rData.CableGrupKey;
		nAnalMethod = rData.nAnalMethod;
		nUpdateType = 0;
	}
	T_SBCT_D() { Initialize(); }
	T_SBCT_D(const T_SBCT_D &src) { *this = src; }
	T_SBCT_D& operator=(const T_SBCT_D &src)
	{
		nNumItr = src.nNumItr;
		ConvTol = src.ConvTol;
		GrupKey = src.GrupKey;
		SagGrupKey = src.SagGrupKey;
		nNumSbctCase = src.nNumSbctCase;
		memcpy(SbctCase, src.SbctCase, sizeof(SbctCase));
		bConsHor = src.bConsHor;
		CableGrupKey = src.CableGrupKey;
		dHorForce = src.dHorForce;
		bHanger = src.bHanger;
		HangerGrupKey = src.HangerGrupKey;
		aHangerZDisp.Copy(src.aHangerZDisp);
		CableGrupSubKey = src.CableGrupSubKey;
		nAnalMethod = src.nAnalMethod;
		nUpdateType = src.nUpdateType;
		return *this;
	}
};

struct T_SBCT_D_RW
{
	int nNumItr;
	double ConvTol;
	UINT GrupKey;
	UINT SagGrupKey;
	
	BOOL bConsHor;
	UINT CableGrupKey;
	double dHorForce;
	
	BOOL bHanger;
	UINT HangerGrupKey;
	int  nCount_aHangerZDisp;

	UINT CableGrupSubKey;
	int nAnalMethod;
	int nUpdateType;

	int nNumSbctCase;
	T_SBCT_BASE SbctCase[D_SBCT_NUMSBCT];
	void Get(T_SBCT_D &rData)
	{
		rData.nNumItr    = nNumItr   ;
		rData.ConvTol    = ConvTol   ;
		rData.GrupKey    = GrupKey   ;
		rData.SagGrupKey = SagGrupKey;
																			
		rData.bConsHor      = bConsHor    ;
		rData.CableGrupKey  = CableGrupKey;
		rData.dHorForce     = dHorForce   ;

		rData.bHanger    = bHanger   ;
		rData.HangerGrupKey= HangerGrupKey;

		rData.CableGrupSubKey = CableGrupSubKey;
		rData.nAnalMethod = nAnalMethod;
		rData.nUpdateType = nUpdateType;

		rData.nNumSbctCase    = nNumSbctCase   ;
		memcpy(rData.SbctCase, SbctCase, sizeof(rData.SbctCase));

		rData.aHangerZDisp.SetSize(nCount_aHangerZDisp);
	}
	void Set(T_SBCT_D Data)
	{
		nNumItr    = Data.nNumItr   ;
		ConvTol    = Data.ConvTol   ;
		GrupKey    = Data.GrupKey   ;
		SagGrupKey = Data.SagGrupKey;
		
		bConsHor      = Data.bConsHor    ;
		CableGrupKey  = Data.CableGrupKey;
		dHorForce     = Data.dHorForce   ;
		
		bHanger    = Data.bHanger   ;
		HangerGrupKey= Data.HangerGrupKey   ;
		
		CableGrupSubKey = Data.CableGrupSubKey;
		nAnalMethod = Data.nAnalMethod;
		nUpdateType = Data.nUpdateType;

		nNumSbctCase    = Data.nNumSbctCase   ;
		memcpy(SbctCase, Data.SbctCase, sizeof(SbctCase));

		nCount_aHangerZDisp = (int)Data.aHangerZDisp.GetSize();
	}
};
struct T_SBCT_UDRD_D
{
	T_SBCT_D data;
};
struct T_SBCT_UDRD_D_RW
{
	T_SBCT_D_RW data;
	void GetSbct(T_SBCT_D &SBCTD)
	{
		data.Get(SBCTD);
	}
	void SetSbct(const T_SBCT_D &SBCTD)
	{
		data.Set(SBCTD);
	}
};
struct T_SBCT_UNIT
{
	int dHorForce;
	int dHangerZDisp;
	T_SBCT_UNIT()
	{
		dHorForce=D_UNITSYS_BASE_FORCE;
		dHangerZDisp=D_UNITSYS_BASE_DISPLACEMENT;
	}
};
#define HASHSIZESBCT 1

#define D_EFCT_NUMEFCT  15
#define T_EFCT_K unsigned int
struct T_EFCT_BASE
{
	T_STLD_K LoadCaseKey;
	double dblFactor;
	void Initialize()
	{
		LoadCaseKey=0;
		dblFactor=0;
	}
};
struct T_EFCT_D_670
{
	int nNumEfctCase;
	T_EFCT_BASE EfctCase[D_EFCT_NUMEFCT];
};
struct T_EFCT_D_692
{
	int nNumEfctCase;
	T_EFCT_BASE EfctCase[D_EFCT_NUMEFCT];
	BOOL bIniForceComb;
	BOOL bSaveToLoadCase;
	T_STLD_K LoadCaseKey;
};
struct T_EFCT_D
{
	int nNumEfctCase;
	T_EFCT_BASE EfctCase[D_EFCT_NUMEFCT];
	BOOL bIniForceComb;      // 6.7.1에서 추가
	BOOL bSaveToLoadCase;    // 6.7.1에서 추가
	T_STLD_K LoadCaseKey;    // 6.7.1에서 추가 (Initial Force to Element Force의 LoadCase Key)
	BOOL bConsiderGeomStiff; // 7.0.0에서 추가
	void Initialize()
	{
		nNumEfctCase=0;
		for(int i=0; i<D_EFCT_NUMEFCT; i++) EfctCase[i].Initialize();
		bIniForceComb = FALSE;
		bSaveToLoadCase = FALSE;
		LoadCaseKey = 0;
		bConsiderGeomStiff = TRUE;
	}
	void Convert692(T_EFCT_D_692& rData)
	{
		nNumEfctCase = rData.nNumEfctCase;
		memcpy(EfctCase, rData.EfctCase, sizeof(EfctCase));
		bIniForceComb = rData.bIniForceComb;
		bSaveToLoadCase = rData.bSaveToLoadCase;
		LoadCaseKey = rData.LoadCaseKey;
		bConsiderGeomStiff = !bIniForceComb; // Initial Force Combination이 TRUE이면 사용못함.
	}
	void Convert670(T_EFCT_D_670& rData)
	{
		nNumEfctCase = rData.nNumEfctCase;
		memcpy(EfctCase, rData.EfctCase, sizeof(EfctCase));
		bIniForceComb = TRUE;
		bSaveToLoadCase = FALSE;
		LoadCaseKey = 0;
		bConsiderGeomStiff = FALSE;
	}
};
struct T_EFCT_UDRD_D
{
	T_EFCT_D data;
};
struct T_EFCT_UNIT
{
	T_EFCT_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};
#define HASHSIZEEFCT 1

#define	D_BNGR_SECF	  0 
#define	D_BNGR_ESSF	  1 // GEN
#define	D_BNGR_EWSF	  2
#define	D_BNGR_PSSF	  3
#define	D_BNGR_WSSF	  4 // GEN
#define	D_BNGR_CONS	  5
#define	D_BNGR_NSPR	  6 
#define	D_BNGR_GSPR	  7
#define	D_BNGR_SSPS	  8
#define	D_BNGR_ELNK	  9
#define	D_BNGR_RIGD	 10
#define	D_BNGR_NLNK	 11
#define	D_BNGR_CGLP	 12
#define	D_BNGR_FRLS	 13
#define	D_BNGR_OFFS	 14
#define	D_BNGR_PRLS	 15
#define	D_BNGR_MCON	 16
#define	D_BNGR_SIZE	 D_BNGR_MCON+1 // Last

struct T_LIST_BOX_ITEM_MEC
{
	CString str;
	int nKey;
	int nMask;
};

// Define Boundary Combination
#define T_BCCD_K unsigned int
struct T_BCCD_D
{
	CString strCombName;
	CArray<UINT, UINT> aBngrKey;

	void Initialize()
	{
		strCombName.Empty();
		aBngrKey.RemoveAll();
	}
	T_BCCD_D() {}
	T_BCCD_D(const T_BCCD_D& src) { *this = src; }
	T_BCCD_D& operator=(const T_BCCD_D& rData)
	{
		strCombName = rData.strCombName;
		aBngrKey.Copy(rData.aBngrKey);

		return *this;
	}
};
struct T_BCCD_D_CH
{
	char strCombName[20];
	CArray<UINT, UINT> aBngrKey;
	CArray<BOOL, BOOL> aSelect;

	void ConvertToString(T_BCCD_D& rData)
	{
		ConvertCharStr(strCombName, rData.strCombName, sizeof(strCombName));
		rData.aBngrKey.Copy(aBngrKey);
	}
	void ConvertToChar(T_BCCD_D& rData)
	{
		ConvertStrChar(rData.strCombName, strCombName, sizeof(strCombName));
		aBngrKey.Copy(rData.aBngrKey);
	}
};
struct T_BCCD_UDRD_D
{
	T_BCCD_K key;
	T_BCCD_D data;
};
struct T_BCCD_UDRD_D_CH
{
	T_BCCD_K key;
	T_BCCD_D_CH data;
};
struct T_BCCD_UDRD_D_CH_RW
{
	T_BCCD_K key;
	char strCombName[20];
	int nBngrKey;

	void GetBccd(T_BCCD_K& rKey, T_BCCD_D& rData)
	{
		rKey = key;
		ConvertCharStr(strCombName, rData.strCombName, sizeof(strCombName));
	}
	void SetBccd(const T_BCCD_K Key, T_BCCD_D& rData)
	{
		key = Key;
		ConvertStrChar(rData.strCombName, strCombName, sizeof(strCombName));
		nBngrKey = (int)rData.aBngrKey.GetSize();
	}
};

#define HASHSIZEBCCD 1

// Boundary Condition Change Analysis Control
#define T_BCCT_K unsigned int

struct T_BCCT_COMB
{
	CString strCombName;  
	CArray<UINT, UINT> aBngrKey;
	void Initialize()
	{
		strCombName.Empty();    
		aBngrKey.RemoveAll();    
	}
	T_BCCT_COMB() {}
	T_BCCT_COMB(const T_BCCT_COMB& src) { *this = src;}
	T_BCCT_COMB& operator=(const T_BCCT_COMB& rData)
	{
		strCombName = rData.strCombName;    
		aBngrKey.Copy(rData.aBngrKey);    
		return *this;
	}
};
struct T_BCCT_COMB_CH
{
	char   strCombName[20];  
	CArray<UINT, UINT> aBngrKey;  
	void ConvertToString(T_BCCT_COMB& rData)
	{
		ConvertCharStr(strCombName, rData.strCombName, sizeof(strCombName));    
		rData.aBngrKey.Copy(aBngrKey); 
	}
	void ConvertToChar(T_BCCT_COMB& rData)
	{
		ConvertStrChar(rData.strCombName, strCombName, sizeof(strCombName));    
		aBngrKey.Copy(rData.aBngrKey);        
	}
};  
struct T_BCCT_COMB_CH_RW
{
	char strCombName[20];  
	int  nCountBngr;  
	void Get(T_BCCT_COMB_CH& rData)
	{
		memcpy(rData.strCombName, strCombName, sizeof(strCombName));  
	}
	void Set(const T_BCCT_COMB_CH& rData)
	{
		memcpy(strCombName, rData.strCombName, sizeof(strCombName));    
		nCountBngr = (int)rData.aBngrKey.GetSize();  
	}
};

struct T_BCCT_BASE
{
	enum enType {
		StaicLoad = 0,
		Other
	};

	enum enKey {
		ULAT = 0, // Unlisted Analysis Types & Unchanged Boundary
		THRSEV,   // Time History/Response Spectrum/Eigenvalue
		THNS,     // Time History Nonlinear Static
		PO,       // Pushover
		MV,       // Moving, Grid
		SM        // Settlement
	};

	int   nLoadCaseType;   // 0 : Stld, 1:Other
	UINT  LoadCaseKey;     // nLoadCaseType==0 인 경우 : Stld key
	// nLoadCaseType==1 인 경우 : 0=Unlisted Analysis Types & Unchanged Boundary
	//                            1=Time History/Response Spectrum/Eigenvalue
	//                            2=Time History Nonlinear Static
	//                            3=Pushover  --> Pushover 해석 기능이 있는 Product에만 Gen과 Civil 국내버전
	//                            4=Moving, Grid  --> 추가 
	//                            5=Settlement    --> 추가 
	int   BcctCombIndex;   // Unchanged 경우 -1, 이외의 경우에는 T_BCCT_COMB의 Array Index
	                       // 이외의 경우에는 ( T_BCCD_K-1 )
	void Initialize()
	{
		nLoadCaseType = 0;
		LoadCaseKey = 0;
		BcctCombIndex = -1;    
	}
	T_BCCT_BASE() { Initialize(); }
	T_BCCT_BASE(const T_BCCT_BASE& src) { *this = src;}
	T_BCCT_BASE& operator=(const T_BCCT_BASE& rData)
	{
		nLoadCaseType = rData.nLoadCaseType;
		LoadCaseKey   = rData.LoadCaseKey;
		BcctCombIndex = rData.BcctCombIndex;    
		return *this;
	}
};
struct T_BCCT_BASE_MEC
{
	enum enType {
		StaicLoad = 0,
		Other,
		NTH,       // Nonlinear Time History
		PO         // Pushover
	};

	enum enKey {
		MV = 0,    // Moving, Grid
		SM,       // Settlement
		EIG,      // Eigenvalue Analysis
		RS,       // Response Spectrum Analysis
		LTH       // Linear Time History
	};

	int   nLoadCaseType;   // 0 : Stld, 1:Other, 2: Nonlinear Time History, 3:Pushover 
	UINT  LoadCaseKey;     // nLoadCaseType==0 : Stld key 
	// nLoadCaseType==1 : 0=Moving, Grid 
	//                    1=Settlement
	//                    2=Eigenvalue Analysis
	//                    3=Response Spectrum Analysis
	//                    4=Linear Time History
	//nLoadCaseType ==2 : Nonlinear Time History
	//nLoadCaseType ==3 : Pushover

	int   BcctCombIndex;   // Unchanged 경우 -1, 
	// 이외의 경우에는 ( T_BCCD_K-1 )
	void Initialize()
	{
		nLoadCaseType = 0;
		LoadCaseKey = 0;
		BcctCombIndex = -1;
	}
	T_BCCT_BASE_MEC() { Initialize(); }
	T_BCCT_BASE_MEC(const T_BCCT_BASE_MEC& src) { *this = src; }
	T_BCCT_BASE_MEC& operator=(const T_BCCT_BASE_MEC& rData)
	{
		nLoadCaseType = rData.nLoadCaseType;
		LoadCaseKey = rData.LoadCaseKey;
		BcctCombIndex = rData.BcctCombIndex;
		return *this;
	}

	T_BCCT_BASE_MEC& operator=(const T_BCCT_BASE& rData)
	{
		nLoadCaseType = rData.nLoadCaseType;
		LoadCaseKey = rData.LoadCaseKey;
		BcctCombIndex = rData.BcctCombIndex;
		return *this;
	}
};

struct T_BCCT_D_960
{
	CArray<T_BCCT_COMB, T_BCCT_COMB&> aBcctComb;
	CArray<T_BCCT_BASE, T_BCCT_BASE&> aBcctBase;
	char cSelect[16];            // '0':꺼짐, '1':켜짐 : 9개만 사용
	// 0=Support, 1=Point Spring, 2=General Spring, 3=Change General Link Property, 4=SECF, 5=WSSF, 6=PSSF, 7 = FRLS, 8 = ESSF
	BOOL  bApplyDisp;
	void Initialize()
	{
		aBcctComb.RemoveAll();
		aBcctBase.RemoveAll();
		for (int i = 0; i < 16; i++) cSelect[i] = '0';
		bApplyDisp = TRUE;
	}
	T_BCCT_D_960() {}
	T_BCCT_D_960(T_BCCT_D_960& src) { *this = src; }
	T_BCCT_D_960& operator=(T_BCCT_D_960& src)
	{
		aBcctComb.Copy(src.aBcctComb);
		aBcctBase.Copy(src.aBcctBase);
		memcpy(cSelect, src.cSelect, sizeof(cSelect));
		bApplyDisp = src.bApplyDisp;
		return *this;
	}
};
// NX Mec 브랜치에서 작업된 DB.
// NX Master로 넘어오면서 추가 변경사항이 생겼고, NX Mec 버전의 모델 파일 Open을 위해 Read할 수 있게 만듬.
struct T_BCCT_D_MEC_TEMP
{
	CArray<T_BCCT_COMB, T_BCCT_COMB&> aBcctComb;
	CArray<T_BCCT_BASE_MEC, T_BCCT_BASE_MEC&> aBcctBase;
	char cSelect[16];            // '0':꺼짐, '1':켜짐 : 9개만 사용
	// 0=Support, 1=Point Spring, 2=General Spring, 3=Change General Link Property, 4=SECF, 5=WSSF, 6=PSSF, 7 = FRLS, 8 = ESSF
	BOOL  bApplyDisp;
	CArray<BOOL, BOOL> aSelect;                  // cSelect 대체 CAttrCtrl::GetBcctBngrTypeItem 참고
	void Initialize()
	{
		aBcctComb.RemoveAll();
		aBcctBase.RemoveAll();
		for (int i = 0; i < 16; i++) cSelect[i] = '0';
		bApplyDisp = TRUE;
		aSelect.RemoveAll();
		for (int i = 0; i < D_BNGR_SIZE; i++)
		{
			aSelect.Add(1);
		}
	}
	T_BCCT_D_MEC_TEMP() {}
	T_BCCT_D_MEC_TEMP(T_BCCT_D_MEC_TEMP& src) { *this = src; }
	T_BCCT_D_MEC_TEMP& operator=(T_BCCT_D_MEC_TEMP& src)
	{
		aBcctComb.Copy(src.aBcctComb);
		aBcctBase.Copy(src.aBcctBase);
		memcpy(cSelect, src.cSelect, sizeof(cSelect));
		bApplyDisp = src.bApplyDisp;
		aSelect.Copy(src.aSelect);
		return *this;
	}
};

struct T_BCCT_D
{  
	CArray<T_BCCT_COMB, T_BCCT_COMB&> aBcctComb; // 사용안함.
	CArray<T_BCCT_BASE, T_BCCT_BASE&> aBcctBase;  
	CArray<T_BCCT_BASE_MEC, T_BCCT_BASE_MEC&> aBcctBase_MEC;  
	char cSelect[16];                            // 사용안함. // '0':꺼짐, '1':켜짐 : 9개만 사용   
											   				  // 0=Support, 1=Point Spring, 2=General Spring, 3=Change General Link Property, 4=SECF, 5=WSSF, 6=PSSF, 7 = FRLS, 8 = ESSF
	BOOL  bApplyDisp;                            // 사용안함.
	CArray<BOOL, BOOL> aSelect;                  // cSelect 대체 CAttrCtrl::GetBcctBngrTypeItem 참고
	void Initialize()
	{
		aBcctComb.RemoveAll();
		aBcctBase.RemoveAll();  
		aBcctBase_MEC.RemoveAll();
		for(int i = 0; i < 16; i++) cSelect[i] = '0';
		bApplyDisp = TRUE;
		aSelect.RemoveAll();
		for (int i = 0; i < D_BNGR_SIZE; i++)
		{
			aSelect.Add(1);
		}
	}
	T_BCCT_D() {}
	T_BCCT_D(T_BCCT_D& src) {*this = src;}
	T_BCCT_D& operator=(T_BCCT_D& src)
	{
		aBcctComb.Copy(src.aBcctComb);
		aBcctBase.Copy(src.aBcctBase);
		aBcctBase_MEC.Copy(src.aBcctBase_MEC);
		memcpy(cSelect, src.cSelect, sizeof(cSelect));    
		bApplyDisp = src.bApplyDisp;
		aSelect.Copy(src.aSelect);
		return *this;
	}
};
struct T_BCCT_D_CH
{  
	CArray<T_BCCT_COMB_CH, T_BCCT_COMB_CH&> aBcctComb;
	CArray<T_BCCT_BASE, T_BCCT_BASE&> aBcctBase;  
	CArray<T_BCCT_BASE_MEC, T_BCCT_BASE_MEC&> aBcctBase_MEC;
	char  cSelect[16]; 
	BOOL  bApplyDisp;
	CArray<BOOL, BOOL> aSelect;
	void ConvertToString(T_BCCT_D& rData)
	{
		memcpy(rData.cSelect, cSelect, sizeof(cSelect));
		rData.bApplyDisp = bApplyDisp;
		rData.aBcctBase.Copy(aBcctBase);
		rData.aBcctBase_MEC.Copy(aBcctBase_MEC);
		rData.aBcctComb.SetSize(aBcctComb.GetSize());
		for(int i = 0; i < aBcctComb.GetSize(); i++)
		{
			//ConvertCharStr(aBcctComb[i].strCombName, rData.aBcctComb[i].strCombName, sizeof(aBcctComb[i].strCombName));      
			//rData.aBcctComb[i].aBngrKey.Copy(aBcctComb[i].aBngrKey);      
			aBcctComb[i].ConvertToString(rData.aBcctComb[i]);
		}    
		rData.aSelect.Copy(aSelect);
	}
	void ConvertToChar(T_BCCT_D& rData)
	{
		memcpy(cSelect, rData.cSelect, sizeof(cSelect));
		bApplyDisp = rData.bApplyDisp;
		aBcctBase.Copy(rData.aBcctBase);
		aBcctBase_MEC.Copy(rData.aBcctBase_MEC);
		aBcctComb.SetSize(rData.aBcctComb.GetSize());
		for(int i = 0; i < rData.aBcctComb.GetSize(); i++)
		{
			//ConvertStrChar(rData.aBcctComb[i].strCombName, aBcctComb[i].strCombName, sizeof(rData.aBcctComb[i].strCombName));
			//aBcctComb[i].aBngrKey.Copy(rData.aBcctComb[i].aBngrKey);      
			aBcctComb[i].ConvertToChar(rData.aBcctComb[i]);
		}
		aSelect.Copy(rData.aSelect);
	}  
};
struct T_BCCT_D_CH_TEMP
{ 
	CArray<T_BCCT_COMB_CH_RW, T_BCCT_COMB_CH_RW&> aBcctComb;
	CArray<T_BCCT_BASE, T_BCCT_BASE&> aBcctBase;  
	CArray<T_BCCT_BASE_MEC, T_BCCT_BASE_MEC&> aBcctBase_MEC;
	char  cSelect[16]; 
	BOOL  bApplyDisp;  
	CArray<BOOL, BOOL> aSelect;
	CArray<UINT, UINT> aTotalBngrKey;  // 각 T_BCCT_COMB에 있는 BngrKey를 모두 모은 것  
	void Set(T_BCCT_D_CH& rData)
	{
		memcpy(cSelect, rData.cSelect, sizeof(rData.cSelect));
		bApplyDisp = rData.bApplyDisp;
		aBcctComb.SetSize(rData.aBcctComb.GetSize());
		aBcctBase.Copy(rData.aBcctBase);
		aBcctBase_MEC.Copy(rData.aBcctBase_MEC);
		aSelect.Copy(rData.aSelect);
		aTotalBngrKey.RemoveAll();    
		for(int i = 0; i < rData.aBcctComb.GetSize(); i++)
		{
			memcpy(aBcctComb[i].strCombName, rData.aBcctComb[i].strCombName, sizeof(aBcctComb[i].strCombName));      
			aBcctComb[i].nCountBngr  = (int)rData.aBcctComb[i].aBngrKey.GetSize();            
			for(int j = 0; j < rData.aBcctComb[i].aBngrKey.GetSize(); j++)
			{
				aTotalBngrKey.Add(rData.aBcctComb[i].aBngrKey[j]);            
			}      
		}    
	}
	void Get(T_BCCT_D_CH& rData)
	{
		memcpy(rData.cSelect, cSelect, sizeof(cSelect));
		rData.bApplyDisp = bApplyDisp;
		rData.aBcctComb.SetSize(aBcctComb.GetSize());    
		rData.aBcctBase.Copy(aBcctBase);
		rData.aBcctBase_MEC.Copy(aBcctBase_MEC);
		rData.aSelect.Copy(aSelect);
		int nBeginNum = 0;
		for(int i = 0; i < aBcctComb.GetSize(); i++)
		{
			memcpy(rData.aBcctComb[i].strCombName, aBcctComb[i].strCombName, sizeof(aBcctComb[i].strCombName));      
			rData.aBcctComb[i].aBngrKey.RemoveAll();      
			for(int j = 0; j < aBcctComb[i].nCountBngr; j++)
			{
				rData.aBcctComb[i].aBngrKey.Add(aTotalBngrKey[nBeginNum]);        
				nBeginNum++;
			}      
		}    
	}
};
struct T_BCCT_UDRD_D
{
	T_BCCT_D data;
};
struct T_BCCT_UDRD_D_CH
{
	T_BCCT_D_CH data;
};
struct T_BCCT_UDRD_D_CH_RW_960
{
	int  nCountBcctComb;
	int  nCountBcctBase;
	char cSelect[16];
	BOOL bApplyDisp;
	int  nCountTotalBngrKey;
};

struct T_BCCT_UDRD_D_CH_RW_MEC_TEMP
{
	int  nCountBcctComb;
	int  nCountBcctBase;
	char cSelect[16];
	BOOL bApplyDisp;
	int  nCountTotalBngrKey;
	int  nSelect;
};

struct T_BCCT_UDRD_D_CH_RW
{   
	int  nCountBcctComb;
	int  nCountBcctBase;
	int  nCountBcctBase_MEC;
	char cSelect[16];
	BOOL bApplyDisp;
	int  nCountTotalBngrKey;
	int  nSelect;
	void GetBcct(T_BCCT_D_CH_TEMP& rData)
	{     
		rData.aBcctBase.RemoveAll();
		rData.aBcctBase_MEC.RemoveAll();
		rData.aBcctComb.RemoveAll();
		memcpy(rData.cSelect, cSelect, sizeof(cSelect));
		rData.bApplyDisp= bApplyDisp;
		rData.aTotalBngrKey.RemoveAll();    
		rData.aSelect.RemoveAll();
	}
	void SetBcct(const T_BCCT_D_CH_TEMP& rData)
	{
		nCountBcctComb = (int)rData.aBcctComb.GetSize();
		nCountBcctBase = (int)rData.aBcctBase.GetSize();
		nCountBcctBase_MEC = (int)rData.aBcctBase_MEC.GetSize();
		memcpy(cSelect, rData.cSelect, sizeof(cSelect));
		bApplyDisp = rData.bApplyDisp;
		nCountTotalBngrKey = (int)rData.aTotalBngrKey.GetSize();    
		nSelect = (int)rData.aSelect.GetSize();
	}

	void Convert960(const T_BCCT_UDRD_D_CH_RW_960& rData)
	{
		nCountBcctComb = rData.nCountBcctComb;
		nCountBcctBase = rData.nCountBcctBase;
		nCountBcctBase_MEC = 0;
		memcpy(cSelect, rData.cSelect, sizeof(cSelect));
		bApplyDisp     = rData.bApplyDisp;
		nCountTotalBngrKey = rData.nCountTotalBngrKey;
		nSelect = 0;
	}

	void ConvertMEC_TEMP(const T_BCCT_UDRD_D_CH_RW_MEC_TEMP& rData)
	{
		nCountBcctComb = rData.nCountBcctComb;
		nCountBcctBase = rData.nCountBcctBase;
		nCountBcctBase_MEC = 0;
		memcpy(cSelect, rData.cSelect, sizeof(cSelect));
		bApplyDisp = rData.bApplyDisp;
		nCountTotalBngrKey = rData.nCountTotalBngrKey;
		nSelect = rData.nSelect;
	}
};

struct T_BCCT_UNIT
{
	T_BCCT_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};

#define HASHSIZEBCCT 1

//
struct T_LDCS_DATA_ITEM
{
	CString strLoadCaseName;
	int  BcctCombIndex;   // Undefined인 경우 -1, 이외의 경우에는 T_BCCT_COMB의 Array Index
	int  nLoadCaseType;
	void Initialize()
	{
		strLoadCaseName.Empty();
		BcctCombIndex = -1;
		nLoadCaseType = 0;
	}
	T_LDCS_DATA_ITEM() {}
	T_LDCS_DATA_ITEM(const T_LDCS_DATA_ITEM& src) { *this = src; }
	T_LDCS_DATA_ITEM& operator=(const T_LDCS_DATA_ITEM& rData)
	{
		strLoadCaseName = rData.strLoadCaseName;
		BcctCombIndex = rData.BcctCombIndex;
		nLoadCaseType = rData.nLoadCaseType;
		return *this;
	}
};
//

/// Analysis Control Option ///
#define T_ACOP_K unsigned int

#define D_ACOP_METHOD_TYPE_AUTO        0
#define D_ACOP_METHOD_TYPE_USER        1

#define D_ACOP_PDEL  0
#define D_ACOP_BUCK  1
#define D_ACOP_EIGE  2
#define D_ACOP_HEAT  3
#define D_ACOP_NONL  4
#define D_ACOP_CONS  5
#define D_ACOP_BOUN  6
#define D_ACOP_RESP  7
#define D_ACOP_TIME  8
#define D_ACOP_MOVI  9
#define D_ACOP_SETT 10
#define D_ACOP_COMP 11
#define D_ACOP_CRAN 12
#define D_ACOP_SUSP 13

struct T_ACOP_D
{
	int nMethod;             // D_ACOP_METHOD_TYPE_XXXX

	BOOL bLinearStatic;   // Linear Static Analysis
	BOOL bBuckling    ;   // Buckling Analysis
	BOOL bPDelta      ;   // P-Delta Analysis
	BOOL bResSpec     ;   // Response Spectrum Analysis
	BOOL bEigen       ;   // Eigen-value Analysis
	BOOL bMovingLoad  ;   // Moving Load Analysis
	BOOL bConstStage  ;   // Construction Stage Analysis
	BOOL bSettle      ;   // Settlement Analysis
	BOOL bComposSect  ;   // Composite Section Analysis
	BOOL bCraneLoad   ;   // Crane Load Analysis
	BOOL bTimeHist    ;   // Time History Analysis
	BOOL bNonlinear   ;   // Non-Linear Analysis
	BOOL bPushover    ;   // Pushover Analysis
	BOOL bHeatHydra   ;   // Heat of Hydration Analysis
	BOOL bBoundChange ;   // Boundary Change Assignment to Loadcase/Analysis
	BOOL bSuspension  ;   // Suspension Bridge Analysis (for CIVIL)

	void Initialize()
	{
		nMethod = D_ACOP_METHOD_TYPE_USER;
		
		SetAll(FALSE);
	}	
	void SetAll(BOOL bFlag)
	{
		bLinearStatic = bFlag;
		bBuckling     = bFlag;
		bPDelta       = bFlag;
		bResSpec      = bFlag;
		bEigen        = bFlag;
		bMovingLoad   = bFlag;
		bConstStage   = bFlag;
		bSettle       = bFlag;
		bComposSect   = bFlag;
		bCraneLoad    = bFlag;
		bTimeHist     = bFlag;
		bNonlinear    = bFlag;
		bPushover     = bFlag;
		bHeatHydra    = bFlag;
		bBoundChange  = bFlag;
		bSuspension   = bFlag;
	}	
};
struct T_ACOP_UDRD_D
{
	T_ACOP_D data;
};
struct T_ACOP_UNIT
{
	T_ACOP_UNIT()
	{
		bLinearStatic = D_UNITSYS_NONE;
		bBuckling     = D_UNITSYS_NONE;
		bPDelta       = D_UNITSYS_NONE;
		bResSpec      = D_UNITSYS_NONE;
		bEigen        = D_UNITSYS_NONE;
		bMovingLoad   = D_UNITSYS_NONE;
		bConstStage   = D_UNITSYS_NONE;
		bSettle       = D_UNITSYS_NONE;
		bComposSect   = D_UNITSYS_NONE;
		bCraneLoad    = D_UNITSYS_NONE;
		bTimeHist     = D_UNITSYS_NONE;
		bNonlinear    = D_UNITSYS_NONE;
		bPushover     = D_UNITSYS_NONE;
		bHeatHydra    = D_UNITSYS_NONE;
		bBoundChange  = D_UNITSYS_NONE;
		bSuspension   = D_UNITSYS_NONE;
	}
	int bLinearStatic;
	int bBuckling    ;
	int bPDelta      ;
	int bResSpec     ;
	int bEigen       ;
	int bMovingLoad  ;
	int bConstStage  ;
	int bSettle      ;
	int bComposSect  ;
	int bCraneLoad   ;
	int bTimeHist    ;
	int bNonlinear   ;
	int bPushover    ;
	int bHeatHydra   ;
	int bBoundChange ;
	int bSuspension  ;
};
#define HASHSIZEACOP 1

// [2010-05-17] Kim, Geun Young (Tel: 2042, gykim@midasit.com) - PMS:4092
#define T_CCTL_K unsigned int // Cracked Section Analysis Control

struct T_CCTL_D_925
{
	int nIterPerLoad;
	double dblTolerance;
};

struct T_CCTL_D
{
	int nIterPerLoad;
	double dblTolerance;
	BOOL bCreep;
	double dCreepCoeff;
	void Initialize()
	{
		nIterPerLoad=20;
		dblTolerance=1.e-3;
		bCreep = FALSE;
		dCreepCoeff = 1.0;
	}	  
	void Convert925(T_CCTL_D_925& rData)
	{
		nIterPerLoad = rData.nIterPerLoad;
		dblTolerance = rData.dblTolerance;
		bCreep = FALSE;
		dCreepCoeff = 1.0;
	}
};

struct T_CCTL_UDRD_D
{
	T_CCTL_D data;
};
struct T_CCTL_UNIT
{
	T_CCTL_UNIT()
	{
		nIterPerLoad=D_UNITSYS_NONE;
		dblTolerance=D_UNITSYS_NONE;
	}
	int nIterPerLoad;
	int dblTolerance;
};
#define HASHSIZECCTL 1

// Suspension Bridge Analysis Constol >> Constraint
#define T_SBAC_K unsigned int
struct T_SBAC_D
{
	CString ConstraintName;
	int nConstraintType; // 0: Reaction, 1:Displ. 2: Truss Force 3: Beam Force
	unsigned int ObjectID;  // Node or Elem Id
	int nPoint; // for Beam or Truss only, For Truss 0:I, 1:J, For Beam, 0:I 1:1/4, 2:2/4, 3:3/4, 4:J
	int nComponent; // for React & Displ. 0~5: DX~RZ, For Truss Always 0, For Beam, 0~5: Axial~MntZ
	int nEqualityCond; // 0: equality,1: inequality
	BOOL bValue; // TRUE: Value type, FALSE: Other Object Type
	double dValue;
	unsigned int OtherObject;
	BOOL bUpperBound;      // 5.9.0 추가
	BOOL bLowerBound;      // 5.9.0 추가
	double dUpperBound;    // 5.9.0 추가 
	double dLowerBound;    // 5.9.0 추가 
	T_SBAC_D()
	{
	}
	T_SBAC_D(T_SBAC_D& src)
	{
		*this = src;
	}
	T_SBAC_D& operator=(const T_SBAC_D& src)
	{
		ConstraintName=src.ConstraintName;
		nConstraintType=src.nConstraintType;
		ObjectID=src.ObjectID;
		nPoint=src.nPoint;
		nComponent=src.nComponent;
		nEqualityCond=src.nEqualityCond;
		bValue=src.bValue;
		dValue=src.dValue;
		OtherObject=src.OtherObject;
		bUpperBound = src.bUpperBound;
		bLowerBound = src.bLowerBound;
		dUpperBound = src.dUpperBound;
		dLowerBound = src.dLowerBound;
		return *this;
	}
	void Initialize()
	{
		ConstraintName.Empty();
		nConstraintType=1;
		ObjectID=0;
		nPoint=0;
		nComponent=0;
		nEqualityCond=0;
		bValue=TRUE;
		dValue=0.;
		OtherObject=0;
		bUpperBound = FALSE;
		bLowerBound = FALSE;
		dUpperBound = 0.;
		dLowerBound = 0.;
	}
};
struct T_SBAC_D_CH
{
	char ConstraintName[40];
	int nConstraintType; // 0: Reaction, 1:Displ. 2: Truss Force 3: Beam Force
	unsigned int ObjectID;  // Node or Elem Id
	int nPoint; // for Beam or Truss only, For Truss 0:I, 1:J, For Beam, 0:I 1:1/4, 2:2/4, 3:3/4, 4:J
	int nComponent; // for React & Displ. 0~5: DX~RZ, For Truss Always 0, For Beam, 0~5: Axial~MntZ
	int nEqualityCond; // 0: <= ,1: = , 2: >= 
	BOOL bValue; // TRUE: Value type, FALSE: Other Object Type
	double dValue;
	unsigned int OtherObject;
	BOOL bUpperBound;      // 5.9.0 추가
	BOOL bLowerBound;      // 5.9.0 추가
	double dUpperBound;    // 5.9.0 추가 
	double dLowerBound;    // 5.9.0 추가 
	void ConvertToString(T_SBAC_D& rData)
	{
		ConvertCharStr(ConstraintName, rData.ConstraintName, sizeof(ConstraintName));
		rData.nConstraintType=nConstraintType;
		rData.ObjectID=ObjectID;
		rData.nPoint=nPoint;
		rData.nComponent=nComponent;
		rData.nEqualityCond=nEqualityCond;
		rData.bValue=bValue;
		rData.dValue=dValue;
		rData.OtherObject=OtherObject;
		rData.bUpperBound = bUpperBound;
		rData.bLowerBound = bLowerBound;
		rData.dUpperBound = dUpperBound;
		rData.dLowerBound = dLowerBound;
	}
	void ConvertToChar(T_SBAC_D& rData)
	{
		ConvertStrChar(rData.ConstraintName, ConstraintName, sizeof(ConstraintName));
		nConstraintType=rData.nConstraintType;
		ObjectID=rData.ObjectID;
		nPoint=rData.nPoint;
		nComponent=rData.nComponent;
		nEqualityCond=rData.nEqualityCond;
		bValue=rData.bValue;
		dValue=rData.dValue;
		OtherObject=rData.OtherObject;
		bUpperBound = rData.bUpperBound;
		bLowerBound = rData.bLowerBound;
		dUpperBound = rData.dUpperBound;
		dLowerBound = rData.dLowerBound;
	}
};
struct T_SBAC_UDRD_D
{
	T_SBAC_K key;
	T_SBAC_D data;
};
struct T_SBAC_UDRD_D_CH
{
	T_SBAC_K key;
	T_SBAC_D_CH data;
};
struct T_SBAC_UNIT
{
	T_SBAC_UNIT()
	{
		ConstraintName=D_UNITSYS_NONE;
		nConstraintType=D_UNITSYS_NONE;
		ObjectID=D_UNITSYS_NONE;
		nPoint=D_UNITSYS_NONE;
		nComponent=D_UNITSYS_NONE;
		nEqualityCond=D_UNITSYS_NONE;
		bValue=D_UNITSYS_NONE;
		dValue=D_UNITSYS_NONE;   // 단위가변 UnitCtrl에서 Data Option 따라 가변처리
		OtherObject=D_UNITSYS_NONE;
		dUpperBound = D_UNITSYS_NONE;
		dLowerBound = D_UNITSYS_NONE;
	}
	int ConstraintName;
	int nConstraintType;
	int ObjectID;
	int nPoint;
	int nComponent;
	int nEqualityCond;
	int bValue;
	int dValue;
	int OtherObject;
	int dUpperBound;
	int dLowerBound;
};
#define HASHSIZESBAC 11

#define T_DACT_K unsigned int
struct T_DACT_NL_BASE
{
	T_STLD_K LoadCaseKey;
	double dblFactor;
	void Initialize()
	{
		LoadCaseKey = 0;
		dblFactor = 0;
	}
};

struct T_DACT_D
{
	int nAnalType;		// =0 : p-delta, = 1 ; GNL, =2:GMNL
	int nNLcase;

	// pdelta info.
	int nPdelIter;
	double pDelTol;

	// nonlinear info
	int nNLiter;
	BOOL bConvFlag[3];        // 0 = Engergy, 1 = Displacement, 2 = Force
	double dConvValue[3];     // 0 = Engergy, 1 = Displacement, 2 = Force

	// direct option info
	int iReductType;		// = 0: User(Section Stiffness/element stiffness scale), =1:Auto for nAnalType GNL
	int iAutotype;			// = 0: ASD, =1;LRFD, =2:FIXED(User) for nAnalType GNL
	double alpha;			// ASD = 1.6, LRFD = 1.0, User defined;

	CArray<T_DACT_NL_BASE, T_DACT_NL_BASE&> aNLcase;

	void Initialize()
	{
		nAnalType = 0;
		nNLcase = 0;

		nPdelIter = 5;
		pDelTol = 1.e-5;

		nNLiter = 30;
		for (int i = 0; i < 3; i++)
		{
			bConvFlag[i] = FALSE;
			dConvValue[i] = 0.001;
		}

		iReductType = 0;
		iAutotype = 0;
		alpha = 1.0;

		aNLcase.RemoveAll();
	}

	T_DACT_D() { Initialize(); }
	T_DACT_D(const T_DACT_D& src) { *this = src; }
	T_DACT_D& operator=(const T_DACT_D& src)
	{
		nAnalType = src.nAnalType;
		nNLcase = src.nNLcase;

		nPdelIter = src.nPdelIter;
		pDelTol = src.pDelTol;
	
		nNLiter = src.nNLiter;
		for (int i = 0; i < 3; i++)
		{
			bConvFlag[i] = src.bConvFlag[i];
			dConvValue[i] = src.dConvValue[i];
		}

		iReductType = src.iReductType;
		iAutotype = src.iAutotype;
		alpha = src.alpha;

		aNLcase.Copy(src.aNLcase);

		return *this;
	}
};

struct T_DACT_UDRD_D
{
	T_DACT_D   data;
};


struct T_DACT_D_RW
{
	int nAnalType;		// =0 : p-delta, = 1 ; GNL, =2:GMNL
	int nNLcase;

	// pdelta info.
	int nPdelIter;
	double pDelTol;

	// nonlinear info
	int nNLiter;
	BOOL bConvFlag[3];        // 0 = Engergy, 1 = Displacement, 2 = Force
	double dConvValue[3];     // 0 = Engergy, 1 = Displacement, 2 = Force

	// direct option info
	int iReductType;		// = 0: User(Section Stiffness/element stiffness scale), =1:Auto for nAnalType GNL
	int iAutotype;			// = 0: ASD, =1;LRFD, =2:FIXED(User) for nAnalType GNL
	double alpha;			// ASD = 1.6, LRFD = 1.0, User defined;

	void GetDact(T_DACT_D& rData)
	{
		rData.nAnalType = nAnalType;
		rData.nNLcase = nNLcase;

		rData.nPdelIter = nPdelIter;
		rData.pDelTol = pDelTol;

		rData.nNLiter = nNLiter;
		for (int i = 0; i < 3; i++)
		{
			rData.bConvFlag[i]  = bConvFlag[i];
			rData.dConvValue[i] = dConvValue[i];
		}

		rData.iReductType = iReductType;
		rData.iAutotype = iAutotype;
		rData.alpha = alpha;

		rData.aNLcase.RemoveAll();
	}
	void SetDact(const T_DACT_D& rData)
	{
		nAnalType = rData.nAnalType;
		nNLcase = rData.nNLcase;

		nPdelIter = rData.nPdelIter;
		pDelTol = rData.pDelTol;

		nNLiter = rData.nNLiter;
		for (int i = 0; i < 3; i++)
		{
			bConvFlag[i]  = rData.bConvFlag[i];
			dConvValue[i] = rData.dConvValue[i];
		}

		iReductType = rData.iReductType;
		iAutotype = rData.iAutotype;
		alpha = rData.alpha;

		ASSERT(nNLcase == rData.aNLcase.GetSize());
	}
};

struct T_DACT_UNIT
{
	T_DACT_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;
	}
	int nNoUnit;
};

#define HASHSIZEDACT 1 //


// Perform Analysis // JWKWON-20240530 
#define T_PANA_K unsigned int // Elem key
typedef CArray<T_PANA_K, T_PANA_K> T_PANA_K_LIST;

#define D_PANA_ANALCASE_STAGE				1	
#define D_PANA_ANALCASE_HYDRATION			2
#define D_PANA_ANALCASE_STATIC				3
#define D_PANA_ANALCASE_GEOMETRY_NONLINEAR  4
#define D_PANA_ANALCASE_MATERIAL_NONLINEAR  5
#define D_PANA_ANALCASE_GEO_MAT_NONLINEAR   6 
#define D_PANA_ANALCASE_MOVING_LOAD			7
#define D_PANA_ANALCASE_SETTLEMENT			8
#define D_PANA_ANALCASE_EIGENVALUE			9
#define D_PANA_ANALCASE_SPECTRUM			10
#define D_PANA_ANALCASE_LINEAR_HISTORY		11
#define D_PANA_ANALCASE_NONLINEAR_HISTORY	12
#define D_PANA_ANALCASE_PUSHOVER			13
#define D_PANA_ANALCASE_STAGEPOST			14

struct T_PANA_D
{
	int nAnalType;	//0: Static, 1: Dynamic, 2: Buckling, 3: Heat of Hydration, 4: Nonlinear, 5: Construction Stage, 6: Response Spectrum, 7: Time History, 8: Moving Load, 9: Settlement, 10: Composite Section, 11: Crane Load, 12: Suspension Bridge
	//BOOL bIsAnalGroup;		// 해석군으로 사용할 경우 TRUE;
//	UINT LCKey;	// 해석케이스으로 사용할 경우 LoadCase Key

	BOOL bUseAnal;			// 해석을 사용할지 여부	

	int nAnalCaseID;   // AnalCaseData의 Analysis ID
	CString strName;   // AnalCaseData의 string name
	int nAnalCaseType; // AnalCaseData의 Analysis type
	void Initialize()
	{
		nAnalType = 0;
		//bIsAnalGroup = FALSE;
//		LCKey = 0;
		bUseAnal = FALSE;
		nAnalCaseID = 0;
		nAnalCaseType = 0;
		strName.Empty();
	}
	T_PANA_D() { Initialize(); }
};

struct T_PANA_UDRD_D
{
	T_PANA_K key;
	T_PANA_D data;
};

#define HASHSIZEPANA 101

#pragma pack(pop)

#endif  // __DB_ST_DT_ANAL_H__
