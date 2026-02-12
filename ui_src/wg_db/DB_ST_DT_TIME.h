#ifndef __DB_ST_DT_TIME_H__
#define __DB_ST_DT_TIME_H__

#define MIDAS_SDK

#pragma pack(push, 1)
#pragma warning ( disable : 4244 )

struct T_THIS_BASE
{
	int nModeId;
	double dDamping;
	void Initialize()
	{
		nModeId=1;
		dDamping=0.;
	}
};
#define T_THIS_DT CArray<T_THIS_BASE, T_THIS_BASE&>
struct T_THIS_PROPDAMPING
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

struct T_THIS_BOUNDARY_570
{
	BOOL    bDampingRatio;            // 580에서 없어짐
	double  dEndTime;       // sec     : 580에서 없어짐
	int     nMaxIterNum;
	double  dConvergenceTol;
	void Initialize()
	{
		bDampingRatio = FALSE;
		dEndTime = 0.;
		nMaxIterNum = 5;
		dConvergenceTol = 1.e-3;
	}
};
struct T_THIS_BOUNDARY_610
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
struct T_THIS_BOUNDARY_632
{  
	double  dMinSubStepSize;       // sec 580 추가
	int     nMaxIterNum;
	double  dConvergenceTol;
	int     nDampingMatrixUpdate;  // 620에서 추가 0=No, 1=Yes
	int     nTemp;                 // 570과 Size 같은 것을 막기 위한 임시 변수
};
struct T_THIS_BOUNDARY_640
{  
	double  dMinSubStepSize;       // sec 580 추가
	int     nMaxIterNum;  
	int     nDampingMatrixUpdate;  // 620에서 추가 0=No, 1=Yes  
	BOOL    bDispNorm;           // 6.4.0 추가
	BOOL    bForceNorm;          // 6.4.0 추가
	BOOL    bEnergyNorm;         // 6.4.0 추가 
	double  dDispNorm;           // 6.4.0 추가
	double  dForceNorm;          // 6.4.0 추가
	double  dEnergyNorm;         // 6.4.0 추가 
};
struct T_THIS_BOUNDARY_692
{  
	double  dMinSubStepSize;       // sec 580 추가
	int     nMaxIterNum;  
	int     nDampingMatrixUpdate;  // 620에서 추가 0=No, 1=Yes  
	BOOL    bDispNorm;           // 6.4.0 추가
	BOOL    bForceNorm;          // 6.4.0 추가
	BOOL    bEnergyNorm;         // 6.4.0 추가 
	double  dDispNorm;           // 6.4.0 추가
	double  dForceNorm;          // 6.4.0 추가
	double  dEnergyNorm;         // 6.4.0 추가 
	int     nMaxNumSubStep;      // 6.5.0 추가 
	BOOL    bPerformIteration;   // 6.5.0추가
	BOOL    bPermitConvergence;  // 6.5.0추가
};
struct T_THIS_BOUNDARY_881
{  
	double  dMinSubStepSize;       // sec 580 추가
	int     nMaxIterNum;  
	int     nDampingMatrixUpdate;  // 620에서 추가 0=No, 1=Yes  
	BOOL    bDispNorm;             // 6.4.0 추가
	BOOL    bForceNorm;            // 6.4.0 추가
	BOOL    bEnergyNorm;           // 6.4.0 추가 
	double  dDispNorm;             // 6.4.0 추가
	double  dForceNorm;            // 6.4.0 추가
	double  dEnergyNorm;           // 6.4.0 추가 
	int     nMaxNumSubStep;        // 6.5.0 추가 
	BOOL    bPerformIteration;     // 6.5.0추가
	BOOL    bPermitConvergence;    // 6.5.0추가
	int     nRungeKuttaMethod;     // 7.0.0추가, 0:Fehlberg Method(General Iteration), 1:Cash-Karp Method(Adaptive Step Size Control)
	double  dRungeKuttaTolerance;  // 7.0.0추가(단위없음)
};

struct T_THIS_BOUNDARY
{  
	double  dMinSubStepSize;       // sec 580 추가
	int     nMaxIterNum;  
	int     nDampingMatrixUpdate;  // 620에서 추가 0=No, 1=Yes  
	BOOL    bDispNorm;             // 6.4.0 추가
	BOOL    bForceNorm;            // 6.4.0 추가
	BOOL    bEnergyNorm;           // 6.4.0 추가 
	double  dDispNorm;             // 6.4.0 추가
	double  dForceNorm;            // 6.4.0 추가
	double  dEnergyNorm;           // 6.4.0 추가 
	int     nMaxNumSubStep;        // 6.5.0 추가 
	BOOL    bPerformIteration;     // 6.5.0추가
	BOOL    bPermitConvergence;    // 6.5.0추가
	int     nRungeKuttaMethod;     // 7.0.0추가, 0:Fehlberg Method(General Iteration), 1:Cash-Karp Method(Adaptive Step Size Control)
	double  dRungeKuttaTolerance;  // 7.0.0추가(단위없음)
	BOOL    bUseLineSearch;        // 8.8.2추가
	int     nNumSearch;            // 8.8.2추가
	void Initialize()
	{
#if defined(_JP)
		dMinSubStepSize = 0.01;
#else
		dMinSubStepSize = 1.e-5;
#endif
		nMaxIterNum = 10;
#if defined (_MGEN) && defined(_CH)
		nDampingMatrixUpdate = 0;
#else
		nDampingMatrixUpdate = 1;
#endif
		bDispNorm   = TRUE;  
		bForceNorm  = FALSE; 
		bEnergyNorm = FALSE;
		dDispNorm   = 0.001;  
		dForceNorm  = 0.001; 
		dEnergyNorm = 0.001; 
		nMaxNumSubStep = 10;
		bPerformIteration = TRUE;
		bPermitConvergence = TRUE;
		nRungeKuttaMethod = 0;
		dRungeKuttaTolerance = 1.e-8;
		bUseLineSearch = FALSE;
		nNumSearch = 5;
	}
	void Convert570(T_THIS_BOUNDARY_570& rData)
	{
		dMinSubStepSize = 1.e-5;
		nMaxIterNum = rData.nMaxIterNum;    
		nDampingMatrixUpdate = 0;
		bDispNorm   = TRUE;  
		bForceNorm  = FALSE; 
		bEnergyNorm = FALSE;
		dDispNorm   = 0.001;  
		dForceNorm  = 0.001; 
		dEnergyNorm = 0.001;
		nMaxNumSubStep = 10;
		bPerformIteration = TRUE;
		bPermitConvergence = FALSE;
		nRungeKuttaMethod = 0;
		dRungeKuttaTolerance = 1.e-8;
		bUseLineSearch = FALSE;
		nNumSearch = 5;
	}
	void Convert610(T_THIS_BOUNDARY_610& rData)
	{
		dMinSubStepSize = rData.dMinSubStepSize;
		nMaxIterNum = rData.nMaxIterNum;    
		nDampingMatrixUpdate = 0;
		bDispNorm   = TRUE;  
		bForceNorm  = FALSE; 
		bEnergyNorm = FALSE;
		dDispNorm   = 0.001;  
		dForceNorm  = 0.001; 
		dEnergyNorm = 0.001;
		nMaxNumSubStep = 10;
		bPerformIteration = TRUE;
		bPermitConvergence = FALSE;
		nRungeKuttaMethod = 0;
		dRungeKuttaTolerance = 1.e-8;
		bUseLineSearch = FALSE;
		nNumSearch = 5;
	}
	void Convert632(T_THIS_BOUNDARY_632& rData)
	{
		dMinSubStepSize = rData.dMinSubStepSize;
		nMaxIterNum = rData.nMaxIterNum;    
		nDampingMatrixUpdate = rData.nDampingMatrixUpdate;
		bDispNorm   = TRUE;  
		bForceNorm  = FALSE; 
		bEnergyNorm = FALSE;
		dDispNorm   = 0.001;  
		dForceNorm  = 0.001; 
		dEnergyNorm = 0.001;
		nMaxNumSubStep = 10;
		bPerformIteration = TRUE;
		bPermitConvergence = FALSE;
		nRungeKuttaMethod = 0;
		dRungeKuttaTolerance = 1.e-8;
		bUseLineSearch = FALSE;
		nNumSearch = 5;
	}
	void Convert640(T_THIS_BOUNDARY_640& rData)
	{
		dMinSubStepSize = rData.dMinSubStepSize;
		nMaxIterNum = rData.nMaxIterNum;    
		nDampingMatrixUpdate = rData.nDampingMatrixUpdate;
		bDispNorm   = rData.bDispNorm;  
		bForceNorm  = rData.bForceNorm ; 
		bEnergyNorm = rData.bEnergyNorm;
		dDispNorm   = rData.dDispNorm  ;  
		dForceNorm  = rData.dForceNorm ; 
		dEnergyNorm = rData.dEnergyNorm;
		nMaxNumSubStep = 10;
		bPerformIteration = TRUE;
		bPermitConvergence = FALSE;
		nRungeKuttaMethod = 0;
		dRungeKuttaTolerance = 1.e-8;
		bUseLineSearch = FALSE;
		nNumSearch = 5;
	}
	void Convert692(T_THIS_BOUNDARY_692& rData)
	{
		dMinSubStepSize = rData.dMinSubStepSize;
		nMaxIterNum = rData.nMaxIterNum;    
		nDampingMatrixUpdate = rData.nDampingMatrixUpdate;
		bDispNorm   = rData.bDispNorm;  
		bForceNorm  = rData.bForceNorm ; 
		bEnergyNorm = rData.bEnergyNorm;
		dDispNorm   = rData.dDispNorm  ;  
		dForceNorm  = rData.dForceNorm ; 
		dEnergyNorm = rData.dEnergyNorm;
		nMaxNumSubStep = rData.nMaxNumSubStep;
		bPerformIteration = rData.bPerformIteration;
		bPermitConvergence = rData.bPermitConvergence;
		nRungeKuttaMethod = 0;
		dRungeKuttaTolerance = 1.e-8;
		bUseLineSearch = FALSE;
		nNumSearch = 5;
	}
	void Convert881(T_THIS_BOUNDARY_881& rData)
	{
		dMinSubStepSize = rData.dMinSubStepSize;
		nMaxIterNum = rData.nMaxIterNum;    
		nDampingMatrixUpdate = rData.nDampingMatrixUpdate;
		bDispNorm   = rData.bDispNorm;  
		bForceNorm  = rData.bForceNorm ; 
		bEnergyNorm = rData.bEnergyNorm;
		dDispNorm   = rData.dDispNorm  ;  
		dForceNorm  = rData.dForceNorm ; 
		dEnergyNorm = rData.dEnergyNorm;
		nMaxNumSubStep = rData.nMaxNumSubStep;
		bPerformIteration = rData.bPerformIteration;
		bPermitConvergence = rData.bPermitConvergence;
		nRungeKuttaMethod = rData.nRungeKuttaMethod;
		dRungeKuttaTolerance = rData.dRungeKuttaTolerance;
		bUseLineSearch = FALSE;
		nNumSearch = 5;
	}
};

struct T_THIS_GRUPDAMPING
{
	int    nRefFrequency;  // 0=Frequency, 1=Period, 2=Mode Index from Modal Analysis
	double dFrequency;  
	double dPeriod;     
	int    nModeIndex;  
	void Initialize()
	{
		nRefFrequency = 1;
		dFrequency = 1.0;
		dPeriod = 1.0;
		nModeIndex = 1;
	}
};

struct T_THIS_STATIC_640
{
	int    nCtrlMethod;     // 0=Load Control, 1=Displacement Control
	int    nCtrlOption;     // 0=Global Control, 1=Master Node Control
	double dTotalTransDisp;
	UINT   MasterNode;
	int    nMasterDir;      // 1=DX, 2=DY, 3=DZ
	double dTotalIncDisp;
	BOOL   bCumulateHistory;
};

struct T_THIS_STATIC
{
	int    nCtrlMethod;     // 0=Load Control, 1=Displacement Control
	int    nCtrlOption;     // 0=Global Control, 1=Master Node Control
	double dTotalTransDisp;
	UINT   MasterNode;
	int    nMasterDir;      // 1=DX, 2=DY, 3=DZ
	double dTotalIncDisp;
	BOOL   bCumulateHistory;
	double dScaleFactor;       // 6.5.0 추가
	void Initialize()
	{
		nCtrlMethod = 0;
		nCtrlOption = 0;     
		dTotalTransDisp = 0.;
		MasterNode = 0;
		nMasterDir = 1;      
		dTotalIncDisp = 0.;  
		bCumulateHistory = FALSE;
		dScaleFactor = 1.0;
	}
	void Convert640(T_THIS_STATIC_640& rData)
	{
		nCtrlMethod      = rData.nCtrlMethod     ;
		nCtrlOption      = rData.nCtrlOption     ;     
		dTotalTransDisp  = rData.dTotalTransDisp ;
		MasterNode       = rData.MasterNode      ;
		nMasterDir       = rData.nMasterDir      ;      
		dTotalIncDisp    = rData.dTotalIncDisp   ;  
		bCumulateHistory = rData.bCumulateHistory;
		dScaleFactor     = 1.0;
	}
};
struct T_THIS_D_570
{
	CString LoadCaseName;
	CString Description;
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;     // 1= Transient,  2=Periodic, 3=Boundary N.T.
	double dDampAll;
	T_THIS_DT aDampData;
	T_THIS_BOUNDARY_570 BoundaryNL;
	unsigned int LoadCaseId;
	void Initialize()
	{
		LoadCaseName.Empty();
		Description.Empty();
		dEndTime=0.;
		dDelta=0.;
		nOut=0;
		nAnalType=0;
		dDampAll=0.;
		aDampData.RemoveAll();
		BoundaryNL.Initialize();
		LoadCaseId=0;
	}
};

enum D_THIS_INITMETHOD	{ D_THIS_INITMETHOD_INITIALLOAD, D_THIS_INITMETHOD_ORDER_IN_SEQLOAD };
enum D_THIS_ANALTYPE	{ D_THIS_ANALTYPE_LINEAR = 1, D_THIS_ANALTYPE_NONLINEAR = 2 };
enum D_THIS_ANALMETHOD	{ D_THIS_ANALMETHOD_MODAL = 1, D_THIS_ANALMETHOD_DIRECT = 2, D_THIS_ANALMETHOD_STATIC = 3 };
enum D_THIS_TIMEHISTORYTYPE { D_THIS_TIMEHISTORYTYPE_TRANSIENT = 1, D_THIS_TIMEHISTORYTYPE_PERIODIC = 2 };
enum D_THIS_GEOMNONLIN	{ D_THIS_GEOMNONLIN_NONE, D_THIS_GEOMNONLIN_LARGE_DISP, D_THIS_GEOMNONLIN_P_DELTA };
enum D_THIS_INITCOND    { D_THIS_INITIAL = 0, D_THIS_SUBSEQUENT_TO = 1 };
enum D_THIS_SUBSEQUENT  { D_THIS_LOAD_CASE = 0, D_THIS_EQUILIBRIUM_ELEMENT_FORCE = 1, D_THIS_FORCE_FOR_GEOMETRIC_STIFFNESS = 2, D_THIS_INITIAL_ELEMENT_FORCE_FOR_GRID = 3 };
enum D_THIS_SUBLOADCASE { D_THIS_STATIC_LOAD_CASE = 0, D_THIS_CONSTRUCTION_STAGE_LOAD_CASE = 1, D_THIS_TIME_HISTORY_LOAD_CASE = 2 };
enum D_THIS_DAPIMGTYPE  { D_THIS_DIRECT_MODAL = 1, D_THIS_MASS_AND_STIFFNESS_PROPORTIONAL = 2, D_THIS_STRAIN_ENERGY_PROPORTIONAL = 3, D_THIS_ELEMENT_MASS__STIFFNESS_PROPORTIONA = 4 };
enum D_THIS_NEWMARK     { D_THIS_NEWMARK_CONST = 1, D_THIS_NEWMARK_LINEAR = 2, D_THIS_NEWMARK_USER = 3 };

#define T_THIS_K unsigned int
class T_THIS_K_LIST : public CArray< T_THIS_K, T_THIS_K>
{
public:
	T_THIS_K_LIST() {};
	T_THIS_K_LIST(const T_THIS_K_LIST& src)
	{
		*this = src;
	}

	T_THIS_K_LIST& operator=(const T_THIS_K_LIST& src)
	{
		this->Copy(src);
		return *this;
	}

	INT_PTR FindIndex(const T_THIS_K& key) const
	{
		for (INT_PTR i = 0; i < this->GetSize(); i++)
		{
			if (this->GetAt(i) == key)
				return i;
		}
		return -1;
	}
};

namespace NTIS_BOUNDARY
{
	enum LineSearch : int
	{
		Auto = 1, // Line Search시 default 데이터 사용
		UserDefined // Line Search시 사용자가 정의한 데이터 사용
	};
}

struct T_NTIS_BOUNDARY
{
	BOOL    bPermitConvergence;
	int     nMaxIterNum;

	BOOL    bDispNorm;
	BOOL    bForceNorm;
	BOOL    bEnergyNorm;
	double  dDispNorm;
	double  dForceNorm;
	double  dEnergyNorm;
	int nStiffUpdateScheme;
	int nIterStiffUpdate;

	int  nMaxBisectLevel;
	BOOL bSmartBisection;
	int  nDiverThreshold;

	BOOL bEnableLineSearch;
	int  nLineSearchOption;
	int  nNumSearch;
	int  nMaxLineSearch;
	double dLineSearchTol;

	BOOL bPerformIteration;
	int  nDampingMatrixUpdate;

	int nRungeKuttaMethod;        
	double dRungeKuttaTolerance;

	bool operator==(const T_NTIS_BOUNDARY& src) const
	{
		if (this == &src) return true;
		if (bPermitConvergence != src.bPermitConvergence) return false;
		if (nMaxIterNum != src.nMaxIterNum) return false;

		if (bDispNorm != src.bDispNorm) return false;
		if (bForceNorm != src.bForceNorm) return false;
		if (bEnergyNorm != src.bEnergyNorm) return false;
		if (dDispNorm != src.dDispNorm) return false;
		if (dForceNorm != src.dForceNorm) return false;
		if (dEnergyNorm != src.dEnergyNorm) return false;
		if (nStiffUpdateScheme != src.nStiffUpdateScheme) return false;
		if (nIterStiffUpdate != src.nIterStiffUpdate) return false;

		if (nMaxBisectLevel != src.nMaxBisectLevel) return false;
		if (bSmartBisection != src.bSmartBisection) return false;
		if (nDiverThreshold != src.nDiverThreshold) return false;

		if (bEnableLineSearch != src.bEnableLineSearch) return false;
		if (nLineSearchOption != src.nLineSearchOption) return false;
		if (nNumSearch != src.nNumSearch) return false;
		if (nMaxLineSearch != src.nMaxLineSearch) return false;
		if (dLineSearchTol != src.dLineSearchTol) return false;

		if (bPerformIteration != src.bPerformIteration) return false;
		if (nDampingMatrixUpdate != src.nDampingMatrixUpdate) return false;

		if (nRungeKuttaMethod != src.nRungeKuttaMethod) return false;
		if (dRungeKuttaTolerance != src.dRungeKuttaTolerance) return false;
		return true;
	}

	bool operator!=(const T_NTIS_BOUNDARY& src) const { return !(*this == src); }

	void Initialize()
	{
		bPermitConvergence = TRUE;
		nMaxIterNum = 10;
		bDispNorm = TRUE;
		bForceNorm = FALSE;
		bEnergyNorm = FALSE;
		dDispNorm = 0.001;
		dForceNorm = 0.001;
		dEnergyNorm = 0.001;
		nStiffUpdateScheme = 1;
		nIterStiffUpdate = 1;
		nMaxBisectLevel = 5;
		bSmartBisection = FALSE;
		nDiverThreshold = 3;
		bEnableLineSearch = TRUE;
		nLineSearchOption = NTIS_BOUNDARY::Auto;
		nNumSearch = 3;
		nMaxLineSearch = 4;
		dLineSearchTol = 0.5;
		bPerformIteration = TRUE;
		nDampingMatrixUpdate = 0;
		nRungeKuttaMethod = 0;
		dRungeKuttaTolerance = 1.e-8;
	}
};

#define T_THIS_K unsigned int
struct T_THIS_D
{
	CString LoadCaseName;
	CString Description;
	double dEndTime;      // End Time 
	double dDelta;        // Time Increment 
	int nOut;             // Step Number Increment for Output
	int nAnalType;        // 1= Linear, 2 = Nonlinear
	double dDampAll;      // Damping Ratio for All Modes 
	T_THIS_DT aDampData;
	T_THIS_BOUNDARY BoundaryNL_Old;  // 6.4.0에서 변경  
	unsigned int LoadCaseId;
	int nInitCondition;		// D_THIS_INITMETHOD==D_THIS_INITMETHOD_ORDER_IN_SEQLOAD only : 0 = Initial, 1 = Subsequent to
	T_THIS_K KeyFinalState; // D_THIS_INITMETHOD==D_THIS_INITMETHOD_ORDER_IN_SEQLOAD only : Previous Load Case
	int nAnalMethod;      // 1=Modal, 2=Direct Integration, 3=Static   : 580추가(3=Static은 6.4.0 추가)
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Modal, 2=Mass and Stiffness Proportional, 3=Strain Energy Proportional(650까지 Group)
												// 4=Element Mass & Stiffness Proportional : 580추가, Group은 620에서 추가, 4인 경우는 660에서 추가
	T_THIS_PROPDAMPING PropDamping;   // 580추가
	T_THIS_GRUPDAMPING GrupDamping;  // 620추가(6.4.0부터 사용않음)  
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가
	T_THIS_STATIC StaticCtrl;    // 6.4.0 추가  
	int  nIncrementStep;     // 6.5.0추가 
	BOOL bCumulateDVA;       // 6.5.0추가
	int  nSubsequentType;    // D_THIS_INITMETHOD==D_THIS_INITMETHOD_ORDER_IN_SEQLOAD only : 0=Load Case, 1=Equilibrium Element Force, 2=Force for Geometric stiffness 3=Initial Element Forces from Grid Model
	int  nSubLoadCaseType;   // D_THIS_INITMETHOD==D_THIS_INITMETHOD_ORDER_IN_SEQLOAD only : 0=Static Load Case, 1=Construction Stage Load Case, 2=Time History Load Case
	BOOL bAnalPDelta;
	int  nGeomNonlinearType;  // 0 : None, 1 : Large Displacements, 2 : P-Delta
	BOOL bUseInitial; 
	int  nInitLoadMethod; // D_THIS_INITMETHOD
	CArray<UINT, UINT> aGilcCase; // (v955)
	int  nTimeIntegrationParam;
	T_NTIS_BOUNDARY BoundaryNL;
	BOOL bFinalStepAcc;

	void Initialize()
	{
		LoadCaseName.Empty();
		Description.Empty();
		dEndTime=1.0;
		dDelta=0.01;
		nOut=1;
		nAnalType=D_THIS_ANALTYPE_LINEAR;
		dDampAll=0.;
		aDampData.RemoveAll();
		BoundaryNL_Old.Initialize();
		LoadCaseId=0;
		nInitCondition = D_THIS_INITIAL;
		KeyFinalState = 0;
		nAnalMethod = D_THIS_ANALMETHOD_MODAL;
		nTimeHistoryType = 1;
		nDampingType = D_THIS_DIRECT_MODAL;
#if defined(_JP)
		nDampingType = 3;
#endif
		PropDamping.Initialize();
		GrupDamping.Initialize();
		nIntegrationPara = 1;
		dGamma = 0.5;
		dBeta = 0.25;
		bFinalStepLoad = TRUE;
		StaticCtrl.Initialize();
		nIncrementStep = 1;
		bCumulateDVA = FALSE;   
		nSubsequentType = D_THIS_LOAD_CASE;
		nSubLoadCaseType = D_THIS_TIME_HISTORY_LOAD_CASE;
		bAnalPDelta = FALSE;
		nGeomNonlinearType = D_THIS_GEOMNONLIN_NONE;
		bUseInitial = FALSE;
		nInitLoadMethod = D_THIS_INITMETHOD_ORDER_IN_SEQLOAD;
		aGilcCase.RemoveAll();
		nTimeIntegrationParam = 0;
		BoundaryNL.Initialize();
		bFinalStepAcc = FALSE;
	}
	
	T_THIS_D()
	{
	}
	T_THIS_D(T_THIS_D& src)
	{
		*this = src;
	}
	T_THIS_D& operator=(T_THIS_D& src)
	{
		LoadCaseName=src.LoadCaseName;
		Description=src.Description;
		dEndTime=src.dEndTime;
		dDelta=src.dDelta;
		nOut=src.nOut;
		nAnalType=src.nAnalType;
		dDampAll=src.dDampAll;
		aDampData.Copy(src.aDampData);
		BoundaryNL_Old = src.BoundaryNL_Old;
		LoadCaseId=src.LoadCaseId;
		nInitCondition = src.nInitCondition;
		KeyFinalState = src.KeyFinalState;
		nAnalMethod = src.nAnalMethod;
		nTimeHistoryType = src.nTimeHistoryType;
		nDampingType = src.nDampingType;
		PropDamping = src.PropDamping;
		GrupDamping = src.GrupDamping;
		nIntegrationPara = src.nIntegrationPara;
		dGamma = src.dGamma;
		dBeta = src.dBeta;    
		bFinalStepLoad = src.bFinalStepLoad;
		StaticCtrl = src.StaticCtrl;
		nIncrementStep = src.nIncrementStep;
		bCumulateDVA = src.bCumulateDVA;    
		nSubsequentType = src.nSubsequentType;
		nSubLoadCaseType = src.nSubLoadCaseType;
		bAnalPDelta = src.bAnalPDelta;
		nGeomNonlinearType = src.nGeomNonlinearType;
		bUseInitial = src.bUseInitial;
		nInitLoadMethod = src.nInitLoadMethod;
		aGilcCase.Copy(src.aGilcCase);
		nTimeIntegrationParam = src.nTimeIntegrationParam;
		BoundaryNL = src.BoundaryNL;
		bFinalStepAcc = src.bFinalStepAcc;
		return *this;
	}
};
struct T_THIS_D_CH
{
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;     // 1= Transient,  2=Periodic
	double dDampAll;
	T_THIS_DT aDampData;
	T_THIS_BOUNDARY BoundaryNL_Old;
	unsigned int LoadCaseId;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	T_THIS_GRUPDAMPING GrupDamping; // 620 추가
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가
	T_THIS_STATIC StaticCtrl;    // 6.4.0 추가  
	int  nIncrementStep;     // 6.5.0추가 
	BOOL bCumulateDVA;      // 6.5.0추가  
	int  nSubsequentType;    // 6.6.0추가 : 0=Load Case, 1=Equilibrium Element Force
	int  nSubLoadCaseType;   // 6.6.0추가 : 0=Static Load Case, 1=Construction Stage Load Case, 2=Time History Load Case
	BOOL bAnalPDelta;         // 780
	int  nGeomNonlinearType;  // 842
	BOOL bUseInitial; // 881
	int  nInitLoadMethod; // 881
	CArray<UINT, UINT> aGilcCase; // 955
	int nTimeIntegrationParam; //MEC
	T_NTIS_BOUNDARY BoundaryNL;
	BOOL bFinalStepAcc;

	void ConvertToString(T_THIS_D& rData)
	{
		ConvertCharStr(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.dEndTime=dEndTime;
		rData.dDelta=dDelta;
		rData.nOut=nOut;
		rData.nAnalType=nAnalType;
		rData.dDampAll=dDampAll;
		rData.aDampData.Copy(aDampData);
		rData.BoundaryNL_Old = BoundaryNL_Old;
		rData.LoadCaseId=LoadCaseId;
		rData.nInitCondition = nInitCondition;
		rData.KeyFinalState = KeyFinalState;
		rData.nAnalMethod = nAnalMethod;
		rData.nTimeHistoryType = nTimeHistoryType;
		rData.nDampingType = nDampingType;
		rData.PropDamping = PropDamping;
		rData.GrupDamping = GrupDamping;
		rData.nIntegrationPara = nIntegrationPara;
		rData.dGamma = dGamma;
		rData.dBeta = dBeta;
		rData.bFinalStepLoad = bFinalStepLoad;
		rData.StaticCtrl = StaticCtrl;
		rData.nIncrementStep = nIncrementStep;
		rData.bCumulateDVA = bCumulateDVA;    
		rData.nSubsequentType = nSubsequentType;
		rData.nSubLoadCaseType = nSubLoadCaseType;
		rData.bAnalPDelta = bAnalPDelta;
		rData.nGeomNonlinearType = nGeomNonlinearType;
		rData.bUseInitial = bUseInitial;
		rData.nInitLoadMethod = nInitLoadMethod;
		rData.aGilcCase.Copy(aGilcCase);
		rData.nTimeIntegrationParam = nTimeIntegrationParam;
		rData.BoundaryNL = BoundaryNL;
		rData.bFinalStepAcc = bFinalStepAcc;
	}
	void ConvertToChar(T_THIS_D& rData)
	{
		ConvertStrChar(rData.LoadCaseName, LoadCaseName, sizeof(LoadCaseName));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		dEndTime=rData.dEndTime;
		dDelta=rData.dDelta;
		nOut=rData.nOut;
		nAnalType=rData.nAnalType;
		dDampAll=rData.dDampAll;
		aDampData.Copy(rData.aDampData);
		BoundaryNL_Old = rData.BoundaryNL_Old;
		LoadCaseId=rData.LoadCaseId;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		nDampingType = rData.nDampingType;
		PropDamping = rData.PropDamping;
		GrupDamping = rData.GrupDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;    
		bFinalStepLoad = rData.bFinalStepLoad;
		StaticCtrl = rData.StaticCtrl;
		nIncrementStep = rData.nIncrementStep;
		bCumulateDVA = rData.bCumulateDVA;      
		nSubsequentType = rData.nSubsequentType;
		nSubLoadCaseType = rData.nSubLoadCaseType;
		bAnalPDelta = rData.bAnalPDelta;
		nGeomNonlinearType = rData.nGeomNonlinearType;
		bUseInitial = rData.bUseInitial;
		nInitLoadMethod = rData.nInitLoadMethod;
		aGilcCase.Copy(rData.aGilcCase);
		nTimeIntegrationParam = rData.nTimeIntegrationParam;
		BoundaryNL = rData.BoundaryNL;
		bFinalStepAcc = rData.bFinalStepAcc;
	}
};
struct T_THIS_UDRD_D
{
	T_THIS_K key;
	T_THIS_D data;
};
struct T_THIS_UDRD_D_CH
{
	T_THIS_K key;
	T_THIS_D_CH data;
};
struct T_THIS_UDRD_D_CH_RW_530
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	unsigned int LoadCaseId;
	int nCountDampData;
};
struct T_THIS_UDRD_D_CH_RW_570
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY_570 BoundaryNL_Old;
	unsigned int LoadCaseId;
	int nCountDampData;
};
struct T_THIS_UDRD_D_CH_RW_590
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY_610 BoundaryNL_Old;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
};
struct T_THIS_UDRD_D_CH_RW_610
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY_610 BoundaryNL_Old;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가 
	BOOL bStaticLoadingOption; // : 600추가 
};
struct T_THIS_UDRD_D_CH_RW_632
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY_632 BoundaryNL_Old;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	T_THIS_GRUPDAMPING GrupDamping; // 620추가    
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가 
	BOOL bStaticLoadingOption; // : 600추가   
};
struct T_THIS_UDRD_D_CH_RW_640
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY_640 BoundaryNL_Old;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	T_THIS_GRUPDAMPING GrupDamping; // 620추가    
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가 
	T_THIS_STATIC_640 StaticCtrl;    // 6.4.0 추가  
};
struct T_THIS_UDRD_D_CH_RW_650
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY_692 BoundaryNL_Old;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	T_THIS_GRUPDAMPING GrupDamping; // 620추가    
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가 
	T_THIS_STATIC StaticCtrl;    // 6.4.0 추가  
	int  nIncrementStep;     // 6.5.0추가 
	BOOL bCumulateDVA;      // 6.5.0추가  
};
struct T_THIS_UDRD_D_CH_RW_692
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY_692 BoundaryNL_Old;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	T_THIS_GRUPDAMPING GrupDamping; // 620추가    
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가 
	T_THIS_STATIC StaticCtrl;    // 6.4.0 추가  
	int  nIncrementStep;     // 6.5.0추가 
	BOOL bCumulateDVA;      // 6.5.0추가  
	int  nSubsequentType;    // 6.6.0추가 : 0=Load Case, 1=Equilibrium Element Force
	int  nSubLoadCaseType;   // 6.6.0추가 : 0=Static Load Case, 1=Construction Stage Load Case, 2=Time History Load Case
};
struct T_THIS_UDRD_D_CH_RW_770
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY_881 BoundaryNL_Old;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	T_THIS_GRUPDAMPING GrupDamping; // 620추가    
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가 
	T_THIS_STATIC StaticCtrl;    // 6.4.0 추가  
	int  nIncrementStep;     // 6.5.0추가 
	BOOL bCumulateDVA;      // 6.5.0추가  
	int  nSubsequentType;    // 6.6.0추가 : 0=Load Case, 1=Equilibrium Element Force
	int  nSubLoadCaseType;   // 6.6.0추가 : 0=Static Load Case, 1=Construction Stage Load Case, 2=Time History Load Case
};
struct T_THIS_UDRD_D_CH_RW_841
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY_881 BoundaryNL_Old;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	T_THIS_GRUPDAMPING GrupDamping; // 620추가    
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가 
	T_THIS_STATIC StaticCtrl;    // 6.4.0 추가  
	int  nIncrementStep;     // 6.5.0추가 
	BOOL bCumulateDVA;      // 6.5.0추가  
	int  nSubsequentType;    // 6.6.0추가 : 0=Load Case, 1=Equilibrium Element Force
	int  nSubLoadCaseType;   // 6.6.0추가 : 0=Static Load Case, 1=Construction Stage Load Case, 2=Time History Load Case
	BOOL bAnalPDelta;       // 780
};
struct T_THIS_UDRD_D_CH_RW_880
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY_881 BoundaryNL_Old;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	T_THIS_GRUPDAMPING GrupDamping; // 620추가    
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가 
	T_THIS_STATIC StaticCtrl;    // 6.4.0 추가  
	int  nIncrementStep;     // 6.5.0추가 
	BOOL bCumulateDVA;      // 6.5.0추가  
	int  nSubsequentType;    // 6.6.0추가 : 0=Load Case, 1=Equilibrium Element Force
	int  nSubLoadCaseType;   // 6.6.0추가 : 0=Static Load Case, 1=Construction Stage Load Case, 2=Time History Load Case
	BOOL bAnalPDelta;       // 780
	int  nGeomNonlinearType;  // 842
};
struct T_THIS_UDRD_D_CH_RW_881
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY_881 BoundaryNL_Old;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	T_THIS_GRUPDAMPING GrupDamping; // 620추가    
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가 
	T_THIS_STATIC StaticCtrl;    // 6.4.0 추가  
	int  nIncrementStep;     // 6.5.0추가 
	BOOL bCumulateDVA;      // 6.5.0추가  
	int  nSubsequentType;    // 6.6.0추가 : 0=Load Case, 1=Equilibrium Element Force
	int  nSubLoadCaseType;   // 6.6.0추가 : 0=Static Load Case, 1=Construction Stage Load Case, 2=Time History Load Case
	BOOL bAnalPDelta;       // 780
	int  nGeomNonlinearType;  // 842
	BOOL bUseInitial; // v881
	int  nInitLoadMethod; // v881
};
struct T_THIS_UDRD_D_CH_RW_950
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY BoundaryNL_Old;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	T_THIS_GRUPDAMPING GrupDamping; // 620추가    
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가 
	T_THIS_STATIC StaticCtrl;    // 6.4.0 추가  
	int  nIncrementStep;     // 6.5.0추가 
	BOOL bCumulateDVA;      // 6.5.0추가  
	int  nSubsequentType;    // 6.6.0추가 : 0=Load Case, 1=Equilibrium Element Force
	int  nSubLoadCaseType;   // 6.6.0추가 : 0=Static Load Case, 1=Construction Stage Load Case, 2=Time History Load Case
	BOOL bAnalPDelta;       // 780
	int  nGeomNonlinearType;  // 842
	BOOL bUseInitial; // v881
	int  nInitLoadMethod; // v881
};
struct T_THIS_UDRD_D_CH_RW_960
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY BoundaryNL_Old;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	T_THIS_GRUPDAMPING GrupDamping; // 620추가    
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가 
	T_THIS_STATIC StaticCtrl;    // 6.4.0 추가  
	int  nIncrementStep;     // 6.5.0추가 
	BOOL bCumulateDVA;      // 6.5.0추가  
	int  nSubsequentType;    // 6.6.0추가 : 0=Load Case, 1=Equilibrium Element Force
	int  nSubLoadCaseType;   // 6.6.0추가 : 0=Static Load Case, 1=Construction Stage Load Case, 2=Time History Load Case
	BOOL bAnalPDelta;       // 780
	int  nGeomNonlinearType;  // 842
	BOOL bUseInitial; // v881
	int  nInitLoadMethod; // v881
	int  nGilcCase;       // 955
};
struct T_THIS_UDRD_D_CH_RW
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY BoundaryNL_Old;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	T_THIS_GRUPDAMPING GrupDamping; // 620추가    
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가 
	T_THIS_STATIC StaticCtrl;    // 6.4.0 추가  
	int  nIncrementStep;     // 6.5.0추가 
	BOOL bCumulateDVA;      // 6.5.0추가  
	int  nSubsequentType;    // 6.6.0추가 : 0=Load Case, 1=Equilibrium Element Force
	int  nSubLoadCaseType;   // 6.6.0추가 : 0=Static Load Case, 1=Construction Stage Load Case, 2=Time History Load Case
	BOOL bAnalPDelta;       // 780
	int  nGeomNonlinearType;  // 842
	BOOL bUseInitial; // v881
	int  nInitLoadMethod; // v881
	int  nGilcCase;       // 955
	int nTimeIntegrationParam;
	T_NTIS_BOUNDARY BoundaryNL;
	BOOL bFinalStepAcc;

	void GetThis(T_THIS_K& rKey, T_THIS_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.LoadCaseName, LoadCaseName, sizeof(LoadCaseName));
		memcpy(rData.Description, Description, sizeof(Description));
		rData.dEndTime=dEndTime;
		rData.dDelta=dDelta;
		rData.nOut=nOut;
		rData.nAnalType=nAnalType;
		rData.dDampAll=dDampAll;
		rData.BoundaryNL_Old = BoundaryNL_Old;
		rData.LoadCaseId=LoadCaseId;
		rData.nInitCondition = nInitCondition;
		rData.KeyFinalState =  KeyFinalState;
		rData.nAnalMethod = nAnalMethod;     
		rData.nTimeHistoryType = nTimeHistoryType;
		rData.nDampingType =  nDampingType;  
		rData.PropDamping = PropDamping;
		rData.GrupDamping = GrupDamping;
		rData.nIntegrationPara = nIntegrationPara;
		rData.dGamma = dGamma;
		rData.dBeta = dBeta;
		rData.bFinalStepLoad = bFinalStepLoad;
		rData.StaticCtrl = StaticCtrl;
		rData.nIncrementStep = nIncrementStep;
		rData.bCumulateDVA = bCumulateDVA;  
		rData.nSubsequentType = nSubsequentType;
		rData.nSubLoadCaseType = nSubLoadCaseType;
		rData.bAnalPDelta = bAnalPDelta;
		rData.nGeomNonlinearType = nGeomNonlinearType;
		rData.bUseInitial = bUseInitial;
		rData.nInitLoadMethod = nInitLoadMethod;
		rData.nTimeIntegrationParam = nTimeIntegrationParam;
		rData.BoundaryNL = BoundaryNL;
		rData.bFinalStepAcc = bFinalStepAcc;
	}
	void SetThis(const T_THIS_K Key, const T_THIS_D_CH& rData)
	{
		key=Key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime=rData.dEndTime;
		dDelta=rData.dDelta;
		nOut=rData.nOut;
		nAnalType=rData.nAnalType;
		dDampAll=rData.dDampAll;
		BoundaryNL_Old = rData.BoundaryNL_Old;
		LoadCaseId=rData.LoadCaseId;
		nCountDampData=(int)rData.aDampData.GetSize();
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		nDampingType = rData.nDampingType;
		PropDamping = rData.PropDamping;
		GrupDamping = rData.GrupDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;
		bFinalStepLoad = rData.bFinalStepLoad;
		StaticCtrl = rData.StaticCtrl;
		nIncrementStep = rData.nIncrementStep;
		bCumulateDVA = rData.bCumulateDVA;  
		nSubsequentType = rData.nSubsequentType;
		nSubLoadCaseType = rData.nSubLoadCaseType;
		bAnalPDelta = rData.bAnalPDelta;
		nGeomNonlinearType = rData.nGeomNonlinearType;
		bUseInitial = rData.bUseInitial;
		nInitLoadMethod = rData.nInitLoadMethod;
		nGilcCase = (int)rData.aGilcCase.GetSize();
		nTimeIntegrationParam = rData.nTimeIntegrationParam;
		BoundaryNL = rData.BoundaryNL;
		bFinalStepAcc = rData.bFinalStepAcc;
	}
	void Convert530(T_THIS_UDRD_D_CH_RW_530& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL_Old.Initialize();
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition =0;
		KeyFinalState = 0;
		if(rData.nAnalType == 1)  // Linear Transient
		{
			nAnalType = 1;          // 1=Linear,    2=Nonlinear
			nAnalMethod = 1;        // 1=Modal,     2=Direct Integration
			nTimeHistoryType = 1;   // 1=Transient, 2=Periodic 
		}
		else if(rData.nAnalType == 2)  // Linear Periodic
		{
			nAnalType = 1;
			nAnalMethod = 1;
			nTimeHistoryType = 2;
		}
		else if(rData.nAnalType == 3)  // Boundary Nonlinear Transient
		{
			nAnalType = 2;
			nAnalMethod = 1;
			nTimeHistoryType = 1;
		}
		else 
		{
			nAnalType = 1;          // 1=Linear,    2=Nonlinear
			nAnalMethod = 1;        // 1=Modal,     2=Direct Integration
			nTimeHistoryType = 1;   // 1=Transient, 2=Periodic 
		}    
		nDampingType = 1;
		PropDamping.Initialize();
		GrupDamping.Initialize();
		nIntegrationPara = 1;
		dGamma = 0.5;
		dBeta = 0.25;
		bFinalStepLoad = TRUE;
		StaticCtrl.Initialize();
		nIncrementStep = 2;
		bCumulateDVA = TRUE;   
		nSubsequentType = 0;
		nSubLoadCaseType = 2;
		bAnalPDelta = FALSE;
		nGeomNonlinearType = 0;
		bUseInitial = FALSE;
		nInitLoadMethod = D_THIS_INITMETHOD_ORDER_IN_SEQLOAD;
		nGilcCase = 0;
		nTimeIntegrationParam = 1;
		BoundaryNL.Initialize();
		bFinalStepAcc = FALSE;
	}
	void Convert570(T_THIS_UDRD_D_CH_RW_570& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL_Old.Convert570(rData.BoundaryNL_Old);
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition =0;
		KeyFinalState = 0;
		if(rData.nAnalType == 1)  // Linear Transient
		{
			nAnalType = 1;          // 1=Linear,    2=Nonlinear
			nAnalMethod = 1;        // 1=Modal,     2=Direct Integration
			nTimeHistoryType = 1;   // 1=Transient, 2=Periodic 
		}
		else if(rData.nAnalType == 2)  // Linear Periodic
		{
			nAnalType = 1;
			nAnalMethod = 1;
			nTimeHistoryType = 2;
		}
		else if(rData.nAnalType == 3)  // Boundary Nonlinear Transient
		{
			nAnalType = 2;
			nAnalMethod = 1;
			nTimeHistoryType = 1;
		}
		else 
		{
			nAnalType = 1;          // 1=Linear,    2=Nonlinear
			nAnalMethod = 1;        // 1=Modal,     2=Direct Integration
			nTimeHistoryType = 1;   // 1=Transient, 2=Periodic 
		}    
		nDampingType = 1;
		PropDamping.Initialize();
		GrupDamping.Initialize();
		nIntegrationPara = 1;
		dGamma = 0.5;
		dBeta = 0.25;
		bFinalStepLoad = TRUE;
		StaticCtrl.Initialize();
		nIncrementStep = 2;
		bCumulateDVA = TRUE;     
		nSubsequentType = 0;
		nSubLoadCaseType = 2;
		bAnalPDelta = FALSE;
		nGeomNonlinearType = 0;
		bUseInitial = FALSE;
		nInitLoadMethod = D_THIS_INITMETHOD_ORDER_IN_SEQLOAD;
		nGilcCase = 0;
		nTimeIntegrationParam = 1;
		BoundaryNL.Initialize();
		bFinalStepAcc = FALSE;
	}
	void Convert590(T_THIS_UDRD_D_CH_RW_590& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL_Old.Convert610(rData.BoundaryNL_Old);
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		if(rData.nInitCondition == 1) 
		{
			nInitCondition = 0;
			bFinalStepLoad = FALSE;
		} 
		else if(rData.nInitCondition == 2) 
		{
			nInitCondition = 1;
			bFinalStepLoad = FALSE;
		}
		else if(rData.nInitCondition == 3) 
		{
			nInitCondition = 1;
			bFinalStepLoad = TRUE;
		}
		KeyFinalState = rData.KeyFinalState;
		nAnalType = rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		if(rData.nAnalMethod == 2 && rData.nDampingType == 3)  // Direct Integration && Group
			nDampingType = 4;
		else nDampingType = rData.nDampingType;
		PropDamping = rData.PropDamping;
		GrupDamping.Initialize();
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;    
		StaticCtrl.Initialize();
		nIncrementStep = 2;
		bCumulateDVA = TRUE;  
		nSubsequentType = 0;
		nSubLoadCaseType = 2;
		bAnalPDelta = FALSE;
		nGeomNonlinearType = 0;
		bUseInitial = FALSE;
		nInitLoadMethod = D_THIS_INITMETHOD_ORDER_IN_SEQLOAD;
		nGilcCase = 0;
		nTimeIntegrationParam = 1;
		BoundaryNL.Initialize();
		bFinalStepAcc = FALSE;
	}
	void Convert610(T_THIS_UDRD_D_CH_RW_610& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL_Old.Convert610(rData.BoundaryNL_Old);
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalType = rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		if(rData.nAnalMethod == 2 && rData.nDampingType == 3)  // Direct Integration && Group
			nDampingType = 4;
		else nDampingType = rData.nDampingType;    
		PropDamping = rData.PropDamping;
		GrupDamping.Initialize();
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;    
		bFinalStepLoad = rData.bFinalStepLoad;
		nIncrementStep = 2;
		if(rData.nAnalType == 2 && rData.nAnalMethod == 2 && rData.nInitCondition == 0 && rData.bStaticLoadingOption) 
		{
			nAnalMethod = 3;
			nIncrementStep = (int)(rData.dEndTime / rData.dDelta);
		}
		StaticCtrl.Initialize();        
		bCumulateDVA = TRUE;    
		nSubsequentType = 0;
		nSubLoadCaseType = 2;
		bAnalPDelta = FALSE;
		nGeomNonlinearType = 0;
		bUseInitial = FALSE;
		nInitLoadMethod = D_THIS_INITMETHOD_ORDER_IN_SEQLOAD;
		nGilcCase = 0;
		nTimeIntegrationParam = 1;
		BoundaryNL.Initialize();
		bFinalStepAcc = FALSE;
	}
	void Convert632(T_THIS_UDRD_D_CH_RW_632& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL_Old.Convert632(rData.BoundaryNL_Old);
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalType = rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		if(rData.nAnalMethod == 2 && rData.nDampingType == 3)  // Direct Integration && Group
			nDampingType = 4;
		else nDampingType = rData.nDampingType;    
		PropDamping = rData.PropDamping;
		GrupDamping = rData.GrupDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;    
		bFinalStepLoad = rData.bFinalStepLoad;
		nIncrementStep = 2;
		if(rData.nAnalType == 2 && rData.nAnalMethod == 2 && rData.nInitCondition == 0 && rData.bStaticLoadingOption) 
		{
			nAnalMethod = 3;
			nIncrementStep = (int)(rData.dEndTime / rData.dDelta);
		}
		StaticCtrl.Initialize();            
		bCumulateDVA = TRUE;   
		nSubsequentType = 0;
		nSubLoadCaseType = 2;
		bAnalPDelta = FALSE;
		nGeomNonlinearType = 0;
		bUseInitial = FALSE;
		nInitLoadMethod = D_THIS_INITMETHOD_ORDER_IN_SEQLOAD;
		nGilcCase = 0;
		nTimeIntegrationParam = 1;
		BoundaryNL.Initialize();
		bFinalStepAcc = FALSE;
	}
	void Convert640(T_THIS_UDRD_D_CH_RW_640& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL_Old.Convert640(rData.BoundaryNL_Old);
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalType = rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		if(rData.nAnalMethod == 2 && rData.nDampingType == 3)  // Direct Integration && Group
			nDampingType = 4;
		else nDampingType = rData.nDampingType;    
		PropDamping = rData.PropDamping;
		GrupDamping = rData.GrupDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;    
		bFinalStepLoad = rData.bFinalStepLoad;    
		StaticCtrl.Convert640(rData.StaticCtrl);    
		nIncrementStep = 2;
		bCumulateDVA = TRUE;
		nSubsequentType = 0;
		nSubLoadCaseType = 2;
		bAnalPDelta = FALSE;
		nGeomNonlinearType = 0;
		bUseInitial = FALSE;
		nInitLoadMethod = D_THIS_INITMETHOD_ORDER_IN_SEQLOAD;
		nGilcCase = 0;
		nTimeIntegrationParam = 1;
		BoundaryNL.Initialize();
		bFinalStepAcc = FALSE;
	}
	void Convert650(T_THIS_UDRD_D_CH_RW_650& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL_Old.Convert692(rData.BoundaryNL_Old);
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalType = rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		if(rData.nAnalMethod == 2 && rData.nDampingType == 3)  // Direct Integration && Group
			nDampingType = 4;
		else nDampingType = rData.nDampingType;    
		PropDamping = rData.PropDamping;
		GrupDamping = rData.GrupDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;    
		bFinalStepLoad = rData.bFinalStepLoad;    
		StaticCtrl = rData.StaticCtrl;    
		nIncrementStep = rData.nIncrementStep;
		bCumulateDVA = rData.bCumulateDVA;
		nSubsequentType = 0;
		nSubLoadCaseType = 2;
		bAnalPDelta = FALSE;
		nGeomNonlinearType = 0;
		bUseInitial = FALSE;
		nInitLoadMethod = D_THIS_INITMETHOD_ORDER_IN_SEQLOAD;
		nGilcCase = 0;
		nTimeIntegrationParam = 1;
		BoundaryNL.Initialize();
		bFinalStepAcc = FALSE;
	}
	void Convert692(T_THIS_UDRD_D_CH_RW_692& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL_Old.Convert692(rData.BoundaryNL_Old);
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalType = rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		nDampingType = rData.nDampingType;    
		PropDamping = rData.PropDamping;
		GrupDamping = rData.GrupDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;    
		bFinalStepLoad = rData.bFinalStepLoad;    
		StaticCtrl = rData.StaticCtrl;    
		nIncrementStep = rData.nIncrementStep;
		bCumulateDVA = rData.bCumulateDVA;
		nSubsequentType = rData.nSubsequentType;
		nSubLoadCaseType = rData.nSubLoadCaseType;
		bAnalPDelta = FALSE;
		nGeomNonlinearType = 0;
		bUseInitial = FALSE;
		nInitLoadMethod = D_THIS_INITMETHOD_ORDER_IN_SEQLOAD;
		nGilcCase = 0;
		nTimeIntegrationParam = 1;
		BoundaryNL.Initialize();
		bFinalStepAcc = FALSE;
	}
	void Convert770(T_THIS_UDRD_D_CH_RW_770& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL_Old.Convert881(rData.BoundaryNL_Old);
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalType = rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		nDampingType = rData.nDampingType;    
		PropDamping = rData.PropDamping;
		GrupDamping = rData.GrupDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;    
		bFinalStepLoad = rData.bFinalStepLoad;    
		StaticCtrl = rData.StaticCtrl;    
		nIncrementStep = rData.nIncrementStep;
		bCumulateDVA = rData.bCumulateDVA;
		nSubsequentType = rData.nSubsequentType;
		nSubLoadCaseType = rData.nSubLoadCaseType;
		bAnalPDelta = FALSE;
		nGeomNonlinearType = 0;
		bUseInitial = FALSE;
		nInitLoadMethod = D_THIS_INITMETHOD_ORDER_IN_SEQLOAD;
		nGilcCase = 0;
		nTimeIntegrationParam = 1;
		BoundaryNL.Initialize();
		bFinalStepAcc = FALSE;
	}
	void Convert841(T_THIS_UDRD_D_CH_RW_841& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL_Old.Convert881(rData.BoundaryNL_Old);
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalType = rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		nDampingType = rData.nDampingType;    
		PropDamping = rData.PropDamping;
		GrupDamping = rData.GrupDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;    
		bFinalStepLoad = rData.bFinalStepLoad;    
		StaticCtrl = rData.StaticCtrl;    
		nIncrementStep = rData.nIncrementStep;
		bCumulateDVA = rData.bCumulateDVA;
		nSubsequentType = rData.nSubsequentType;
		nSubLoadCaseType = rData.nSubLoadCaseType;
		bAnalPDelta = rData.bAnalPDelta;
		nGeomNonlinearType = 0;
		bUseInitial = FALSE;
		nInitLoadMethod = D_THIS_INITMETHOD_ORDER_IN_SEQLOAD;
		nGilcCase = 0;
		nTimeIntegrationParam = 1;
		BoundaryNL.Initialize();
		bFinalStepAcc = FALSE;
	}
	void Convert880(T_THIS_UDRD_D_CH_RW_880& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL_Old.Convert881(rData.BoundaryNL_Old);
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalType = rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		nDampingType = rData.nDampingType;    
		PropDamping = rData.PropDamping;
		GrupDamping = rData.GrupDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;    
		bFinalStepLoad = rData.bFinalStepLoad;    
		StaticCtrl = rData.StaticCtrl;    
		nIncrementStep = rData.nIncrementStep;
		bCumulateDVA = rData.bCumulateDVA;
		nSubsequentType = rData.nSubsequentType;
		nSubLoadCaseType = rData.nSubLoadCaseType;
		bAnalPDelta = rData.bAnalPDelta;
		nGeomNonlinearType = rData.nGeomNonlinearType;
		bUseInitial = FALSE;
		nInitLoadMethod = D_THIS_INITMETHOD_ORDER_IN_SEQLOAD;
		nGilcCase = 0;
		nTimeIntegrationParam = 1;
		BoundaryNL.Initialize();
		bFinalStepAcc = FALSE;
	}
	void Convert881(T_THIS_UDRD_D_CH_RW_881& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL_Old.Convert881(rData.BoundaryNL_Old);
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalType = rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		nDampingType = rData.nDampingType;    
		PropDamping = rData.PropDamping;
		GrupDamping = rData.GrupDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;    
		bFinalStepLoad = rData.bFinalStepLoad;    
		StaticCtrl = rData.StaticCtrl;    
		nIncrementStep = rData.nIncrementStep;
		bCumulateDVA = rData.bCumulateDVA;
		nSubsequentType = rData.nSubsequentType;
		nSubLoadCaseType = rData.nSubLoadCaseType;
		bAnalPDelta = rData.bAnalPDelta;
		nGeomNonlinearType = rData.nGeomNonlinearType;
		bUseInitial = rData.bUseInitial;
		nInitLoadMethod = rData.nInitLoadMethod;
		nGilcCase = 0;
		nTimeIntegrationParam = 1;
		BoundaryNL.Initialize();
		bFinalStepAcc = FALSE;
	}
	void Convert950(T_THIS_UDRD_D_CH_RW_950& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;
		dDampAll = rData.dDampAll;
		BoundaryNL_Old = rData.BoundaryNL_Old;
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalType = rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		nDampingType = rData.nDampingType;
		PropDamping = rData.PropDamping;
		GrupDamping = rData.GrupDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;
		bFinalStepLoad = rData.bFinalStepLoad;
		StaticCtrl = rData.StaticCtrl;
		nIncrementStep = rData.nIncrementStep;
		bCumulateDVA = rData.bCumulateDVA;
		nSubsequentType = rData.nSubsequentType;
		nSubLoadCaseType = rData.nSubLoadCaseType;
		bAnalPDelta = rData.bAnalPDelta;
		nGeomNonlinearType = rData.nGeomNonlinearType;
		bUseInitial = rData.bUseInitial;
		nInitLoadMethod = rData.nInitLoadMethod;
		nGilcCase = 0;
		nTimeIntegrationParam = 1;
		BoundaryNL.Initialize();
		bFinalStepAcc = FALSE;
	}

	void Convert960(T_THIS_UDRD_D_CH_RW_960& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;
		dDampAll = rData.dDampAll;
		BoundaryNL_Old = rData.BoundaryNL_Old;
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalType = rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		nDampingType = rData.nDampingType;
		PropDamping = rData.PropDamping;
		GrupDamping = rData.GrupDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;
		bFinalStepLoad = rData.bFinalStepLoad;
		StaticCtrl = rData.StaticCtrl;
		nIncrementStep = rData.nIncrementStep;
		bCumulateDVA = rData.bCumulateDVA;
		nSubsequentType = rData.nSubsequentType;
		nSubLoadCaseType = rData.nSubLoadCaseType;
		bAnalPDelta = rData.bAnalPDelta;
		nGeomNonlinearType = rData.nGeomNonlinearType;
		bUseInitial = rData.bUseInitial;
		nInitLoadMethod = rData.nInitLoadMethod;
		nGilcCase = rData.nGilcCase;
		nTimeIntegrationParam = 1;
		BoundaryNL.Initialize();
		bFinalStepAcc = FALSE;
	}
};
struct T_THIS_UNIT
{
	T_THIS_UNIT()
	{
		LoadCaseName=D_UNITSYS_NONE;
		Description=D_UNITSYS_NONE;
		dEndTime=D_UNITSYS_BASE_TIME;
		dDelta=D_UNITSYS_BASE_TIME;
		nOut=D_UNITSYS_NONE;
		nAnalType=D_UNITSYS_NONE;
		dDampAll=D_UNITSYS_NONE;
		nModeId=D_UNITSYS_NONE;
		dDamping=D_UNITSYS_NONE;
		Boundary_dEndTime = D_UNITSYS_BASE_TIME;
		LoadCaseId=D_UNITSYS_NONE;
		Boundary_dMinSubStepSize = D_UNITSYS_BASE_TIME; 
		PropDamping_dPeriodOne = D_UNITSYS_BASE_TIME; 
		PropDamping_dPeriodTwo = D_UNITSYS_BASE_TIME; 
		nInitCondition = D_UNITSYS_NONE;
		KeyFinalState = D_UNITSYS_NONE;
		nAnalMethod = D_UNITSYS_NONE;
		nTimeHistoryType = D_UNITSYS_NONE;
		nDampingType = D_UNITSYS_NONE;    
		nIntegrationPara = D_UNITSYS_NONE;
		dGamma = D_UNITSYS_NONE;
		dBeta = D_UNITSYS_NONE;
		dTotalTransDisp = D_UNITSYS_BASE_LENGTH;
		dTotalIncDisp = D_UNITSYS_BASE_LENGTH;
	}
	int LoadCaseName;
	int Description;
	int dEndTime;
	int dDelta;
	int nOut;
	int nAnalType;
	int dDampAll;
	int nModeId;
	int dDamping;
	int Boundary_dEndTime;
	int LoadCaseId;
	int Boundary_dMinSubStepSize; 
	int PropDamping_dPeriodOne; 
	int PropDamping_dPeriodTwo;
	int nInitCondition;
	int KeyFinalState;
	int nAnalMethod;
	int nTimeHistoryType;
	int nDampingType;
	int nIntegrationPara;
	int dGamma;
	int dBeta;
	int dTotalTransDisp;
	int dTotalIncDisp;
};
#define HASHSIZETHIS 11
#define D_THIS_MINNUM       1   // 입력가능한 최소 하중조건번호
#define D_THIS_MAXNUM    9999   // 입력가능한 최대 하중조건번호

///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
/*
struct T_THIS_BASE
{
	int nModeId;
	double dDamping;
	void Initialize()
	{
		nModeId=0;
		dDamping=0.;
	}
};
#define T_THIS_DT CArray<T_THIS_BASE, T_THIS_BASE&>
struct T_THIS_PROPDAMPING
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

struct T_THIS_BOUNDARY_570
{
	BOOL    bDampingRatio;            // 580에서 없어짐
	double  dEndTime;       // sec     : 580에서 없어짐
	int     nMaxIterNum;
	double  dConvergenceTol;
	void Initialize()
	{
		bDampingRatio = FALSE;
		dEndTime = 0.;
		nMaxIterNum = 5;
		dConvergenceTol = 1.e-3;
	}
};
struct T_THIS_BOUNDARY_610
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
struct T_THIS_BOUNDARY
{  
	double  dMinSubStepSize;       // sec 580 추가
	int     nMaxIterNum;
	double  dConvergenceTol;
	int     nDampingMatrixUpdate;  // 620에서 추가 0=No, 1=Yes
	int     nTemp;                 // 570과 Size 같은 것을 막기 위한 임시 변수
	void Initialize()
	{
		dMinSubStepSize = 1.e-5;
		nMaxIterNum = 10;
		dConvergenceTol = 1.e-3;
		nDampingMatrixUpdate = 0;
		nTemp = 0;
	}
	void Convert570(T_THIS_BOUNDARY_570& rData)
	{
		dMinSubStepSize = 1.e-5;
		nMaxIterNum = rData.nMaxIterNum;
		dConvergenceTol = rData.dConvergenceTol;
		nDampingMatrixUpdate = 0;
		nTemp = 0;
	}
	void Convert610(T_THIS_BOUNDARY_610& rData)
	{
		dMinSubStepSize = rData.dMinSubStepSize;
		nMaxIterNum = rData.nMaxIterNum;
		dConvergenceTol = rData.dConvergenceTol;
		nDampingMatrixUpdate = 0;
		nTemp = 0;
	}
};

struct T_THIS_GRUPDAMPING
{
	int    nRefFrequency;  // 0=Frequency, 1=Period, 2=Mode Index from Modal Analysis
	double dFrequency;  
	double dPeriod;     
	int    nModeIndex;  
	void Initialize()
	{
		nRefFrequency = 1;
		dFrequency = 1.0;
		dPeriod = 1.0;
		nModeIndex = 1;
	}
};

#define T_THIS_K unsigned int
struct T_THIS_D_530
{
	CString LoadCaseName;
	CString Description;
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;     // 1= Transient,  2=Periodic
	double dDampAll;
	T_THIS_DT aDampData;
	unsigned int LoadCaseId;
	void Initialize()
	{
		LoadCaseName.Empty();
		Description.Empty();
		dEndTime=0.;
		dDelta=0.;
		nOut=0;
		nAnalType=0;
		dDampAll=0.;
		aDampData.RemoveAll();
		LoadCaseId=0;
	}
};
struct T_THIS_D_570
{
	CString LoadCaseName;
	CString Description;
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;     // 1= Transient,  2=Periodic, 3=Boundary N.T.
	double dDampAll;
	T_THIS_DT aDampData;
	T_THIS_BOUNDARY_570 BoundaryNL;
	unsigned int LoadCaseId;
	void Initialize()
	{
		LoadCaseName.Empty();
		Description.Empty();
		dEndTime=0.;
		dDelta=0.;
		nOut=0;
		nAnalType=0;
		dDampAll=0.;
		aDampData.RemoveAll();
		BoundaryNL.Initialize();
		LoadCaseId=0;
	}
};
struct T_THIS_D_590
{
	CString LoadCaseName;
	CString Description;
	double dEndTime;      // End Time 
	double dDelta;        // Time Increment 
	int nOut;             // Step Number Increment for Output
	int nAnalType;        // 1= Linear, 2 = Nonlinear
	double dDampAll;      // Damping Ratio for All Modes 
	T_THIS_DT aDampData;
	T_THIS_BOUNDARY_610 BoundaryNL;  // 580, 620에서 변경
	unsigned int LoadCaseId;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	void Initialize()
	{
		LoadCaseName.Empty();
		Description.Empty();
		dEndTime=1.0;
		dDelta=0.1;
		nOut=1;
		nAnalType=1;
		dDampAll=0.;
		aDampData.RemoveAll();
		BoundaryNL.Initialize();
		LoadCaseId=0;
		nInitCondition = 0;
		KeyFinalState = 0;
		nAnalMethod = 1;
		nTimeHistoryType = 1;
		nDampingType = 1;
		PropDamping.Initialize();
		nIntegrationPara = 1;
		dGamma = 0.5;
		dBeta = 0.25;
	}
};
struct T_THIS_D_610
{
	CString LoadCaseName;
	CString Description;
	double dEndTime;      // End Time 
	double dDelta;        // Time Increment 
	int nOut;             // Step Number Increment for Output
	int nAnalType;        // 1= Linear, 2 = Nonlinear
	double dDampAll;      // Damping Ratio for All Modes 
	T_THIS_DT aDampData;
	T_THIS_BOUNDARY_610 BoundaryNL;  // 580, 620에서 변경
	unsigned int LoadCaseId;
	int nInitCondition;   // 0 = Initial, 1 = Subsequent to     // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가
	BOOL bStaticLoadingOption; // : 600추가  
};
struct T_THIS_D
{
	CString LoadCaseName;
	CString Description;
	double dEndTime;      // End Time 
	double dDelta;        // Time Increment 
	int nOut;             // Step Number Increment for Output
	int nAnalType;        // 1= Linear, 2 = Nonlinear
	double dDampAll;      // Damping Ratio for All Modes 
	T_THIS_DT aDampData;
	T_THIS_BOUNDARY BoundaryNL;  // 620에서 변경  
	unsigned int LoadCaseId;
	int nInitCondition;   // 0 = Initial, 1 = Subsequent to     // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Modal, 2=Mass and Stiffness Proportional, 3=Group  : 580추가, Group은 620에서 추가
	T_THIS_PROPDAMPING PropDamping;   // 580추가
	T_THIS_GRUPDAMPING GrupDamping;  // 620추가
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가
	BOOL bStaticLoadingOption; // : 600추가  
	void Initialize()
	{
		LoadCaseName.Empty();
		Description.Empty();
		dEndTime=1.0;
		dDelta=0.1;
		nOut=1;
		nAnalType=1;
		dDampAll=0.;
		aDampData.RemoveAll();
		BoundaryNL.Initialize();
		LoadCaseId=0;
		nInitCondition = 0;
		KeyFinalState = 0;
		nAnalMethod = 1;
		nTimeHistoryType = 1;
		nDampingType = 1;
		PropDamping.Initialize();
		GrupDamping.Initialize();
		nIntegrationPara = 1;
		dGamma = 0.5;
		dBeta = 0.25;
		bFinalStepLoad = FALSE;
		bStaticLoadingOption = FALSE;    
	}
	T_THIS_D()
	{
	}
	T_THIS_D(T_THIS_D& src)
	{
		*this = src;
	}
	T_THIS_D& operator=(T_THIS_D& src)
	{
		LoadCaseName=src.LoadCaseName;
		Description=src.Description;
		dEndTime=src.dEndTime;
		dDelta=src.dDelta;
		nOut=src.nOut;
		nAnalType=src.nAnalType;
		dDampAll=src.dDampAll;
		aDampData.Copy(src.aDampData);
		BoundaryNL = src.BoundaryNL;
		LoadCaseId=src.LoadCaseId;
		nInitCondition = src.nInitCondition;
		KeyFinalState = src.KeyFinalState;
		nAnalMethod = src.nAnalMethod;
		nTimeHistoryType = src.nTimeHistoryType;
		nDampingType = src.nDampingType;
		PropDamping = src.PropDamping;
		GrupDamping = src.GrupDamping;
		nIntegrationPara = src.nIntegrationPara;
		dGamma = src.dGamma;
		dBeta = src.dBeta;    
		bFinalStepLoad = src.bFinalStepLoad;
		bStaticLoadingOption = src.bStaticLoadingOption;    
		return *this;
	}
};
struct T_THIS_D_CH
{
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;     // 1= Transient,  2=Periodic
	double dDampAll;
	T_THIS_DT aDampData;
	T_THIS_BOUNDARY BoundaryNL;
	unsigned int LoadCaseId;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	T_THIS_GRUPDAMPING GrupDamping; // 620 추가
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가
	BOOL bStaticLoadingOption; // : 600추가  
	void ConvertToString(T_THIS_D& rData)
	{
		ConvertCharStr(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.dEndTime=dEndTime;
		rData.dDelta=dDelta;
		rData.nOut=nOut;
		rData.nAnalType=nAnalType;
		rData.dDampAll=dDampAll;
		rData.aDampData.Copy(aDampData);
		rData.BoundaryNL = BoundaryNL;
		rData.LoadCaseId=LoadCaseId;
		rData.nInitCondition = nInitCondition;
		rData.KeyFinalState = KeyFinalState;
		rData.nAnalMethod = nAnalMethod;
		rData.nTimeHistoryType = nTimeHistoryType;
		rData.nDampingType = nDampingType;
		rData.PropDamping = PropDamping;
		rData.GrupDamping = GrupDamping;
		rData.nIntegrationPara = nIntegrationPara;
		rData.dGamma = dGamma;
		rData.dBeta = dBeta;
		rData.bFinalStepLoad = bFinalStepLoad;
		rData.bStaticLoadingOption = bStaticLoadingOption;    
	}
	void ConvertToChar(T_THIS_D& rData)
	{
		ConvertStrChar(rData.LoadCaseName, LoadCaseName, sizeof(LoadCaseName));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		dEndTime=rData.dEndTime;
		dDelta=rData.dDelta;
		nOut=rData.nOut;
		nAnalType=rData.nAnalType;
		dDampAll=rData.dDampAll;
		aDampData.Copy(rData.aDampData);
		BoundaryNL = rData.BoundaryNL;
		LoadCaseId=rData.LoadCaseId;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		nDampingType = rData.nDampingType;
		PropDamping = rData.PropDamping;
		GrupDamping = rData.GrupDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;    
		bFinalStepLoad = rData.bFinalStepLoad;
		bStaticLoadingOption = rData.bStaticLoadingOption;    
	}
};
struct T_THIS_UDRD_D
{
	T_THIS_K key;
	T_THIS_D data;
};
struct T_THIS_UDRD_D_CH
{
	T_THIS_K key;
	T_THIS_D_CH data;
};
struct T_THIS_UDRD_D_CH_RW_530
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	unsigned int LoadCaseId;
	int nCountDampData;
};
struct T_THIS_UDRD_D_CH_RW_570
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY_570 BoundaryNL;
	unsigned int LoadCaseId;
	int nCountDampData;
};
struct T_THIS_UDRD_D_CH_RW_590
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY_610 BoundaryNL;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
};
struct T_THIS_UDRD_D_CH_RW_610
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY_610 BoundaryNL;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가 
	BOOL bStaticLoadingOption; // : 600추가 
};
struct T_THIS_UDRD_D_CH_RW
{
	T_THIS_K key;
	char LoadCaseName[20];
	char Description[80];
	double dEndTime;
	double dDelta;
	int nOut;
	int nAnalType;
	double dDampAll;
	T_THIS_BOUNDARY BoundaryNL;
	unsigned int LoadCaseId;
	int nCountDampData;
	int nInitCondition;   // 1=!bInitCondition, 2=bInitCondition && !bFinalStepLoad, 3=bInitCondition && bFinalStepLoad : 580추가
	T_THIS_K KeyFinalState; // Previous Load Case : 580추가
	int nAnalMethod;      // 1=Modal, 2=Direct Integration   : 580추가
	int nTimeHistoryType; // 1=Transient, 2=Periodic          : 580추가
	int nDampingType;     // 1=Direct Specification, 2=Proportional Damping   : 580추가
	T_THIS_PROPDAMPING PropDamping; // : 580추가
	T_THIS_GRUPDAMPING GrupDamping; // 620추가    
	int nIntegrationPara; // 1=Constant Acceleraion, 2=Linear Acceleration, 3=User Input : 580추가
	double dGamma;        // : 580추가
	double dBeta;         // : 580추가
	BOOL bFinalStepLoad;  // : 600추가 
	BOOL bStaticLoadingOption; // : 600추가   
	void GetThis(T_THIS_K& rKey, T_THIS_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.LoadCaseName, LoadCaseName, sizeof(LoadCaseName));
		memcpy(rData.Description, Description, sizeof(Description));
		rData.dEndTime=dEndTime;
		rData.dDelta=dDelta;
		rData.nOut=nOut;
		rData.nAnalType=nAnalType;
		rData.dDampAll=dDampAll;
		rData.BoundaryNL = BoundaryNL;
		rData.LoadCaseId=LoadCaseId;
		rData.nInitCondition = nInitCondition;
		rData.KeyFinalState =  KeyFinalState;
		rData.nAnalMethod = nAnalMethod;     
		rData.nTimeHistoryType = nTimeHistoryType;
		rData.nDampingType =  nDampingType;  
		rData.PropDamping = PropDamping;
		rData.GrupDamping = GrupDamping;
		rData.nIntegrationPara = nIntegrationPara;
		rData.dGamma = dGamma;
		rData.dBeta = dBeta;
		rData.bFinalStepLoad = bFinalStepLoad;
		rData.bStaticLoadingOption = bStaticLoadingOption;     
	}
	void SetThis(const T_THIS_K Key, const T_THIS_D_CH& rData)
	{
		key=Key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime=rData.dEndTime;
		dDelta=rData.dDelta;
		nOut=rData.nOut;
		nAnalType=rData.nAnalType;
		dDampAll=rData.dDampAll;
		BoundaryNL = rData.BoundaryNL;
		LoadCaseId=rData.LoadCaseId;
		nCountDampData=rData.aDampData.GetSize();
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		nDampingType = rData.nDampingType;
		PropDamping = rData.PropDamping;
		GrupDamping = rData.GrupDamping;
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;
		bFinalStepLoad = rData.bFinalStepLoad;
		bStaticLoadingOption = rData.bStaticLoadingOption;    
	}
	void Convert530(T_THIS_UDRD_D_CH_RW_530& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL.Initialize();
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition =0;
		KeyFinalState = 0;
		if(rData.nAnalType == 1)  // Linear Transient
		{
			nAnalType = 1;          // 1=Linear,    2=Nonlinear
			nAnalMethod = 1;        // 1=Modal,     2=Direct Integration
			nTimeHistoryType = 1;   // 1=Transient, 2=Periodic 
		}
		else if(rData.nAnalType == 2)  // Linear Periodic
		{
			nAnalType = 1;
			nAnalMethod = 1;
			nTimeHistoryType = 2;
		}
		else if(rData.nAnalType == 3)  // Boundary Nonlinear Transient
		{
			nAnalType = 2;
			nAnalMethod = 1;
			nTimeHistoryType = 1;
		}
		else 
		{
			nAnalType = 1;          // 1=Linear,    2=Nonlinear
			nAnalMethod = 1;        // 1=Modal,     2=Direct Integration
			nTimeHistoryType = 1;   // 1=Transient, 2=Periodic 
		}    
		nDampingType = 1;
		PropDamping.Initialize();
		GrupDamping.Initialize();
		nIntegrationPara = 1;
		dGamma = 0.5;
		dBeta = 0.25;
		bFinalStepLoad = FALSE;
		bStaticLoadingOption = FALSE;    
	}
	void Convert570(T_THIS_UDRD_D_CH_RW_570& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL.Convert570(rData.BoundaryNL);
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition =0;
		KeyFinalState = 0;
		if(rData.nAnalType == 1)  // Linear Transient
		{
			nAnalType = 1;          // 1=Linear,    2=Nonlinear
			nAnalMethod = 1;        // 1=Modal,     2=Direct Integration
			nTimeHistoryType = 1;   // 1=Transient, 2=Periodic 
		}
		else if(rData.nAnalType == 2)  // Linear Periodic
		{
			nAnalType = 1;
			nAnalMethod = 1;
			nTimeHistoryType = 2;
		}
		else if(rData.nAnalType == 3)  // Boundary Nonlinear Transient
		{
			nAnalType = 2;
			nAnalMethod = 1;
			nTimeHistoryType = 1;
		}
		else 
		{
			nAnalType = 1;          // 1=Linear,    2=Nonlinear
			nAnalMethod = 1;        // 1=Modal,     2=Direct Integration
			nTimeHistoryType = 1;   // 1=Transient, 2=Periodic 
		}    
		nDampingType = 1;
		PropDamping.Initialize();
		GrupDamping.Initialize();
		nIntegrationPara = 1;
		dGamma = 0.5;
		dBeta = 0.25;
		bFinalStepLoad = FALSE;
		bStaticLoadingOption = FALSE;    
	}
	void Convert590(T_THIS_UDRD_D_CH_RW_590& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL.Convert610(rData.BoundaryNL);    
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		if(rData.nInitCondition == 1) 
		{
			nInitCondition = 0;
			bFinalStepLoad = FALSE;
		} 
		else if(rData.nInitCondition == 2) 
		{
			nInitCondition = 1;
			bFinalStepLoad = FALSE;
		}
		else if(rData.nInitCondition == 3) 
		{
			nInitCondition = 1;
			bFinalStepLoad = TRUE;
		}
		KeyFinalState = rData.KeyFinalState;
		nAnalType = rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		nDampingType = rData.nDampingType;
		PropDamping = rData.PropDamping;
		GrupDamping.Initialize();
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;    
		bStaticLoadingOption = FALSE;    
	}
	void Convert610(T_THIS_UDRD_D_CH_RW_610& rData)
	{
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		dEndTime = rData.dEndTime;
		dDelta = rData.dDelta;
		nOut = rData.nOut;    
		dDampAll = rData.dDampAll;
		BoundaryNL.Convert610(rData.BoundaryNL);        
		LoadCaseId = rData.LoadCaseId;
		nCountDampData = rData.nCountDampData;
		nInitCondition = rData.nInitCondition;
		KeyFinalState = rData.KeyFinalState;
		nAnalType = rData.nAnalType;
		nAnalMethod = rData.nAnalMethod;
		nTimeHistoryType = rData.nTimeHistoryType;
		nDampingType = rData.nDampingType;
		PropDamping = rData.PropDamping;
		GrupDamping.Initialize();
		nIntegrationPara = rData.nIntegrationPara;
		dGamma = rData.dGamma;
		dBeta = rData.dBeta;    
		bFinalStepLoad = rData.bFinalStepLoad;
		bStaticLoadingOption = rData.bStaticLoadingOption;    
	}
};
struct T_THIS_UNIT
{
	T_THIS_UNIT()
	{
		LoadCaseName=D_UNITSYS_NONE;
		Description=D_UNITSYS_NONE;
		dEndTime=D_UNITSYS_BASE_TIME;
		dDelta=D_UNITSYS_BASE_TIME;
		nOut=D_UNITSYS_NONE;
		nAnalType=D_UNITSYS_NONE;
		dDampAll=D_UNITSYS_NONE;
		nModeId=D_UNITSYS_NONE;
		dDamping=D_UNITSYS_NONE;
		Boundary_dEndTime = D_UNITSYS_BASE_TIME;
		LoadCaseId=D_UNITSYS_NONE;
		Boundary_dMinSubStepSize = D_UNITSYS_BASE_TIME; 
		PropDamping_dPeriodOne = D_UNITSYS_BASE_TIME; 
		PropDamping_dPeriodTwo = D_UNITSYS_BASE_TIME; 
		nInitCondition = D_UNITSYS_NONE;
		KeyFinalState = D_UNITSYS_NONE;
		nAnalMethod = D_UNITSYS_NONE;
		nTimeHistoryType = D_UNITSYS_NONE;
		nDampingType = D_UNITSYS_NONE;    
		nIntegrationPara = D_UNITSYS_NONE;
		dGamma = D_UNITSYS_NONE;
		dBeta = D_UNITSYS_NONE;
	}
	int LoadCaseName;
	int Description;
	int dEndTime;
	int dDelta;
	int nOut;
	int nAnalType;
	int dDampAll;
	int nModeId;
	int dDamping;
	int Boundary_dEndTime;
	int LoadCaseId;
	int Boundary_dMinSubStepSize; 
	int PropDamping_dPeriodOne; 
	int PropDamping_dPeriodTwo;
	int nInitCondition;
	int KeyFinalState;
	int nAnalMethod;
	int nTimeHistoryType;
	int nDampingType;
	int nIntegrationPara;
	int dGamma;
	int dBeta;
};
#define HASHSIZETHIS 11
#define D_THIS_MINNUM       1   // 입력가능한 최소 하중조건번호
#define D_THIS_MAXNUM    9999   // 입력가능한 최대 하중조건번호
*/

struct T_THFC_BASE
{
	double dblTime;
	double dblValue;
	void Initialize()
	{
        dblTime = 0.;
        dblValue = 0.;
	}
};
#define T_THFC_DT CArray<T_THFC_BASE, T_THFC_BASE&>
#define T_THFC_K unsigned int
struct T_THFC_D_431
{
	CString ThisFuncName;
	int nThisFuncType;    // =1 Time Function =2 Sinusoidal Function
	int nThisDataType;    // =1 Normalized Acceleration =2 Acceleration =3 Force =4 Moment
	double ScaleFactor;
	double Gravity;  // Normalized Acceleration일때만 사용
	unsigned int nThisFuncId;    // 순번
	double SinuA;
	double SinuC;
	double SinuFrequency;
	double SinuDampingFactor;
	double SinuPhaseAngle;
	T_THFC_DT arThisFuncData;
};
struct T_THFC_D_632
{
	CString ThisFuncName;
	int nThisFuncType;    // =1 Time Function =2 Sinusoidal Function
	int nThisDataType;    // =1 Normalized Acceleration =2 Acceleration =3 Force =4 Moment =5 Normal
	double ScaleFactor;
	double Gravity;  // Normalized Acceleration일때만 사용
	unsigned int nThisFuncId;    // 순번
	double SinuA;
	double SinuC;
	double SinuFrequency;
	double SinuDampingFactor;
	double SinuPhaseAngle;
	T_THFC_DT arThisFuncData;
	CString Description;
};
struct T_THFC_D
{
    enum enThisFuncType
    {
        time_function       = 1,
        sinusoidal_function = 2
    };

    enum enScaleMethod
    {
        scale_factor  = 0,
        maximum_value = 1
    };

    enum enThisDataType
    {
        normalaccel = 1,
        accel       = 2,
        force       = 3,
        moment      = 4,
        normal      = 5
    };

	CString ThisFuncName;
	int nThisFuncType;    // =1 Time Function =2 Sinusoidal Function
	int nThisDataType;    // =1 Normalized Acceleration =2 Acceleration =3 Force =4 Moment =5 Normal
	double ScaleFactor;
	double Gravity;  // Normalized Acceleration일때만 사용
	unsigned int nThisFuncId;    // 순번
	double SinuA;
	double SinuC;
	double SinuFrequency;
	double SinuDampingFactor;
	double SinuPhaseAngle;
	T_THFC_DT arThisFuncData;
	CString Description;
	int nScaleMethod;     // 6.4.0추가 0=Scale Factor, 1=Maximum Value
	double dMaxValue;     // 6.4.0추가
	void Initialize()
	{
		ThisFuncName.Empty();
		nThisFuncType=0;
		nThisDataType=0;
		ScaleFactor=0.;
		Gravity=0.;
		nThisFuncId=0;
		SinuA=0.;
		SinuC=0.;
		SinuFrequency=0.;
		SinuDampingFactor=0.;
		SinuPhaseAngle=0.;
		arThisFuncData.RemoveAll();
		Description.Empty();
		nScaleMethod = 0;
		dMaxValue = 0;
	}
	T_THFC_D()
	{
	}
	T_THFC_D(T_THFC_D& src)
	{
		*this = src;
	}
	T_THFC_D& operator=(T_THFC_D& src)
	{
		ThisFuncName=src.ThisFuncName;
		nThisFuncType=src.nThisFuncType;
		nThisDataType=src.nThisDataType;
		ScaleFactor=src.ScaleFactor;
		Gravity=src.Gravity;
		nThisFuncId=src.nThisFuncId;
		SinuA=src.SinuA;
		SinuC=src.SinuC;
		SinuFrequency=src.SinuFrequency;
		SinuDampingFactor=src.SinuDampingFactor;
		SinuPhaseAngle=src.SinuPhaseAngle;
		arThisFuncData.Copy(src.arThisFuncData);
		Description = src.Description;
		nScaleMethod = src.nScaleMethod;
		dMaxValue = src.dMaxValue;
		return *this;
	}
	void Convert431(T_THFC_D_431& rData)
	{
		ThisFuncName = rData.ThisFuncName;
		nThisFuncType = rData.nThisFuncType;
		nThisDataType = rData.nThisDataType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nThisFuncId = rData.nThisFuncId;
		SinuA = rData.SinuA;
		SinuC = rData.SinuC;
		SinuFrequency = rData.SinuFrequency;
		SinuDampingFactor = rData.SinuDampingFactor;
		SinuPhaseAngle = rData.SinuPhaseAngle;
		arThisFuncData.Copy(rData.arThisFuncData);
		Description.Empty();
		nScaleMethod = 0;
		dMaxValue = 0;
	}
	void Convert632(T_THFC_D_632& rData)
	{
		ThisFuncName = rData.ThisFuncName;
		nThisFuncType = rData.nThisFuncType;
		nThisDataType = rData.nThisDataType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nThisFuncId = rData.nThisFuncId;
		SinuA = rData.SinuA;
		SinuC = rData.SinuC;
		SinuFrequency = rData.SinuFrequency;
		SinuDampingFactor = rData.SinuDampingFactor;
		SinuPhaseAngle = rData.SinuPhaseAngle;
		arThisFuncData.Copy(rData.arThisFuncData);
		Description = rData.Description;
		nScaleMethod = 0;
		dMaxValue = 0;
	}
};
struct T_THFC_D_CH
{
	char ThisFuncName[28];  // 650에서 28로 증가
	int nThisFuncType;    
	int nThisDataType;    
	double ScaleFactor;
	double Gravity;  // Normalized Acceleration일때만 사용
	unsigned int nThisFuncId;    // 순번
	double SinuA;
	double SinuC;
	double SinuFrequency;
	double SinuDampingFactor;
	double SinuPhaseAngle;
	T_THFC_DT arThisFuncData;
	char Description[80];
	int nScaleMethod;     // 6.4.0추가 0=Scale Factor, 1=Maximum Value
	double dMaxValue;     // 6.4.0추가
	void ConvertToString(T_THFC_D& rData)
	{
		ConvertCharStr(ThisFuncName, rData.ThisFuncName, sizeof(ThisFuncName));
		rData.nThisFuncType=nThisFuncType;
		rData.nThisDataType=nThisDataType;
		rData.ScaleFactor=ScaleFactor;
		rData.Gravity=Gravity;
		rData.nThisFuncId=nThisFuncId;
		rData.SinuA=SinuA;
		rData.SinuC=SinuC;
		rData.SinuFrequency=SinuFrequency;
		rData.SinuDampingFactor=SinuDampingFactor;
		rData.SinuPhaseAngle=SinuPhaseAngle;
		rData.arThisFuncData.Copy(arThisFuncData);
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.nScaleMethod = nScaleMethod;
		rData.dMaxValue = dMaxValue;
	}
	void ConvertToChar(T_THFC_D& rData)
	{
		ConvertStrChar(rData.ThisFuncName, ThisFuncName, sizeof(ThisFuncName));
		nThisFuncType=rData.nThisFuncType;
		nThisDataType=rData.nThisDataType;
		ScaleFactor=rData.ScaleFactor;
		Gravity=rData.Gravity;
		nThisFuncId=rData.nThisFuncId;
		SinuA=rData.SinuA;
		SinuC=rData.SinuC;
		SinuFrequency=rData.SinuFrequency;
		SinuDampingFactor=rData.SinuDampingFactor;
		SinuPhaseAngle=rData.SinuPhaseAngle;
		arThisFuncData.Copy(rData.arThisFuncData);
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
	}
};
struct T_THFC_UDRD_D
{
	T_THFC_K key;
	T_THFC_D data;
};
struct T_THFC_UDRD_D_CH
{
	T_THFC_K key;
	T_THFC_D_CH data;
};
struct T_THFC_UDRD_D_CH_RW_431
{
	T_THFC_K key;
	char ThisFuncName[20];
	int nThisFuncType;    
	int nThisDataType;    
	double ScaleFactor;
	double Gravity;  // Normalized Acceleration일때만 사용
	unsigned int nThisFuncId;    // 순번
	double SinuA;
	double SinuC;
	double SinuFrequency;
	double SinuDampingFactor;
	double SinuPhaseAngle;
	int nCountThisFuncData;
};
struct T_THFC_UDRD_D_CH_RW_632
{
	T_THFC_K key;
	char ThisFuncName[20];
	int nThisFuncType;    
	int nThisDataType;    
	double ScaleFactor;
	double Gravity;  // Normalized Acceleration일때만 사용
	unsigned int nThisFuncId;    // 순번
	double SinuA;
	double SinuC;
	double SinuFrequency;
	double SinuDampingFactor;
	double SinuPhaseAngle;
	int nCountThisFuncData;
	char Description[80];
};
struct T_THFC_UDRD_D_CH_RW_641
{
	T_THFC_K key;
	char ThisFuncName[20];
	int nThisFuncType;    
	int nThisDataType;    
	double ScaleFactor;
	double Gravity;  // Normalized Acceleration일때만 사용
	unsigned int nThisFuncId;    // 순번
	double SinuA;
	double SinuC;
	double SinuFrequency;
	double SinuDampingFactor;
	double SinuPhaseAngle;
	int nCountThisFuncData;
	char Description[80];
	int nScaleMethod;     // 6.4.0추가 0=Scale Factor, 1=Maximum Value
	double dMaxValue;     // 6.4.0추가
};
struct T_THFC_UDRD_D_CH_RW
{
	T_THFC_K key;
	char ThisFuncName[28];  // 650부터 28로 증가
	int nThisFuncType;    
	int nThisDataType;    
	double ScaleFactor;
	double Gravity;  // Normalized Acceleration일때만 사용
	unsigned int nThisFuncId;    // 순번
	double SinuA;
	double SinuC;
	double SinuFrequency;
	double SinuDampingFactor;
	double SinuPhaseAngle;
	int nCountThisFuncData;
	char Description[80];
	int nScaleMethod;     // 6.4.0추가 0=Scale Factor, 1=Maximum Value
	double dMaxValue;     // 6.4.0추가
	void GetThfc(T_THFC_K& rKey, T_THFC_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.ThisFuncName, ThisFuncName, sizeof(rData.ThisFuncName));
		rData.nThisFuncType=nThisFuncType;
		rData.nThisDataType=nThisDataType;
		rData.ScaleFactor=ScaleFactor;
		rData.Gravity=Gravity;
		rData.nThisFuncId=nThisFuncId;
		rData.SinuA=SinuA;
		rData.SinuC=SinuC;
		rData.SinuFrequency=SinuFrequency;
		rData.SinuDampingFactor=SinuDampingFactor;
		rData.SinuPhaseAngle=SinuPhaseAngle;
		memcpy(rData.Description, Description, sizeof(Description));
		rData.nScaleMethod = nScaleMethod;
		rData.dMaxValue = dMaxValue;
	}
	void SetThfc(const T_THFC_K Key, const T_THFC_D_CH& rData)
	{
		key=Key;
		memcpy(ThisFuncName, rData.ThisFuncName, sizeof(ThisFuncName));
		nThisFuncType=rData.nThisFuncType;
		nThisDataType=rData.nThisDataType;
		ScaleFactor=rData.ScaleFactor;
		Gravity=rData.Gravity;
		nThisFuncId=rData.nThisFuncId;
		SinuA=rData.SinuA;
		SinuC=rData.SinuC;
		SinuFrequency=rData.SinuFrequency;
		SinuDampingFactor=rData.SinuDampingFactor;
		SinuPhaseAngle=rData.SinuPhaseAngle;
		nCountThisFuncData=(int)rData.arThisFuncData.GetSize();
		memcpy(Description, rData.Description, sizeof(Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
	}
	void Convert431(T_THFC_UDRD_D_CH_RW_431& rData)
	{
		key = rData.key;
		memset(ThisFuncName, 0, sizeof(ThisFuncName));
		memcpy(ThisFuncName, rData.ThisFuncName, sizeof(rData.ThisFuncName));
		nThisFuncType=rData.nThisFuncType;
		nThisDataType=rData.nThisDataType;
		ScaleFactor=rData.ScaleFactor;
		Gravity=rData.Gravity;
		nThisFuncId=rData.nThisFuncId;
		SinuA=rData.SinuA;
		SinuC=rData.SinuC;
		SinuFrequency=rData.SinuFrequency;
		SinuDampingFactor=rData.SinuDampingFactor;
		SinuPhaseAngle=rData.SinuPhaseAngle;
		nCountThisFuncData=rData.nCountThisFuncData;
		memset(Description, 0, sizeof(Description));
		nScaleMethod = 0;
		dMaxValue = 0;
	}
	void Convert632(T_THFC_UDRD_D_CH_RW_632& rData)
	{
		key = rData.key;
		memset(ThisFuncName, 0, sizeof(ThisFuncName));
		memcpy(ThisFuncName, rData.ThisFuncName, sizeof(rData.ThisFuncName));
		nThisFuncType=rData.nThisFuncType;
		nThisDataType=rData.nThisDataType;
		ScaleFactor=rData.ScaleFactor;
		Gravity=rData.Gravity;
		nThisFuncId=rData.nThisFuncId;
		SinuA=rData.SinuA;
		SinuC=rData.SinuC;
		SinuFrequency=rData.SinuFrequency;
		SinuDampingFactor=rData.SinuDampingFactor;
		SinuPhaseAngle=rData.SinuPhaseAngle;
		nCountThisFuncData=rData.nCountThisFuncData;
		memcpy(Description, rData.Description, sizeof(Description));
		nScaleMethod = 0;
		dMaxValue = 0;
	}
	void Convert641(T_THFC_UDRD_D_CH_RW_641& rData)
	{
		key = rData.key;
		memset(ThisFuncName, 0, sizeof(ThisFuncName));
		memcpy(ThisFuncName, rData.ThisFuncName, sizeof(rData.ThisFuncName));
		nThisFuncType=rData.nThisFuncType;
		nThisDataType=rData.nThisDataType;
		ScaleFactor=rData.ScaleFactor;
		Gravity=rData.Gravity;
		nThisFuncId=rData.nThisFuncId;
		SinuA=rData.SinuA;
		SinuC=rData.SinuC;
		SinuFrequency=rData.SinuFrequency;
		SinuDampingFactor=rData.SinuDampingFactor;
		SinuPhaseAngle=rData.SinuPhaseAngle;
		nCountThisFuncData=rData.nCountThisFuncData;
		memcpy(Description, rData.Description, sizeof(Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
	}
};
struct T_THFC_UNIT
{
	T_THFC_UNIT()
	{
		ThisFuncName=D_UNITSYS_NONE;
		nThisFuncType=D_UNITSYS_NONE;
		nThisDataType=D_UNITSYS_NONE;
		ScaleFactor=D_UNITSYS_NONE;
		Gravity=D_UNITSYS_BASE_ACCELERATION;
		nThisFuncId=D_UNITSYS_NONE;
		SinuA[0]=D_UNITSYS_NONE;                  // if nThisDataType ==1;
		SinuA[1]=D_UNITSYS_BASE_ACCELERATION;     // if nThisDataType ==2;
		SinuA[2]=D_UNITSYS_BASE_FORCE;            // if nThisDataType ==3;
		SinuA[3]=D_UNITSYS_BASE_MOMENT;           // if nThisDataType ==4;
		SinuC[0]=D_UNITSYS_BASE_PERTIME;          // if nThisDataType ==1;
		SinuC[1]=D_UNITSYS_BASE_ACCELTIME;        // if nThisDataType ==2;
		SinuC[2]=D_UNITSYS_BASE_FORCETIME;        // if nThisDataType ==3;
		SinuC[3]=D_UNITSYS_BASE_MOMENTTIME;       // if nThisDataType ==4;
		SinuFrequency=D_UNITSYS_BASE_CPS;
		SinuDampingFactor=D_UNITSYS_NONE;
		SinuPhaseAngle=D_UNITSYS_BASE_DEGREE;
		dblTime=D_UNITSYS_BASE_TIME;
		dblValue[0]=D_UNITSYS_NONE;               // if nThisDataType ==1;
		dblValue[1]=D_UNITSYS_BASE_ACCELERATION;  // if nThisDataType ==2;
		dblValue[2]=D_UNITSYS_BASE_FORCE;         // if nThisDataType ==3;
		dblValue[3]=D_UNITSYS_BASE_MOMENT;        // if nThisDataType ==4;
	}
	int ThisFuncName;
	int nThisFuncType;
	int nThisDataType;
	int ScaleFactor;
	int Gravity;
	int nThisFuncId;
	int SinuA[4];
	int SinuC[4];
	int SinuFrequency;
	int SinuDampingFactor;
	int SinuPhaseAngle;
	int dblTime;
	int dblValue[4];
};
#define HASHSIZETHFC 11
#define D_THFC_MINNUM       1   // 
#define D_THFC_MAXNUM    9999   // 

#define T_THNL_KEY unsigned int
union T_THNL_K
{
	T_THNL_KEY keymap;
	struct
	{
		unsigned int entity : 20;   // 0-1048576 - node
		unsigned int serial : 12;   // 0-4096
	}key;
};

struct T_THNL_D
{
	T_THIS_K LoadCaseKey;
	int nLoadType;   // 1=Force  2=Moment
	unsigned int nThisFuncKey;
	int nDirection;  // 1=X  2=Y  3=Z
	double dArrivalTime;
	double dFactor;
	void Initialize()
	{
		LoadCaseKey=0;
		nLoadType=0;
		nThisFuncKey=0;
		nDirection=0;
		dArrivalTime=0.;
		dFactor=0.;
	}
};
struct T_THNL_UDRD_D
{
	T_THNL_K key;
	T_THNL_D data;
};
struct T_THNL_UNIT
{
	T_THNL_UNIT()
	{
		LoadCaseKey=D_UNITSYS_NONE;
		nLoadType=D_UNITSYS_NONE;
		nThisFuncKey=D_UNITSYS_NONE;
		nDirection=D_UNITSYS_NONE;
		dArrivalTime=D_UNITSYS_BASE_TIME;
		dFactor=D_UNITSYS_NONE;
	}
	int LoadCaseKey;
	int nLoadType;
	int nThisFuncKey;
	int nDirection;
	int dArrivalTime;
	int dFactor;
};
#define HASHSIZETHNL HASHSIZENODE

// Ground Accleration
#define T_THGA_K T_THIS_K
struct T_THGA_D_530
{
	unsigned int nThisFuncKey[3];
	double dAngle[3];
	double dArrivalTime[3];
	double dFactor[3];
};
struct T_THGA_D_630
{
	unsigned int nThisFuncKey[3];
	double dArrivalTime[3];
	double dFactor[3];
	double dAngleHGA;     // Angle of Horizontal Ground Acceleration
};
struct T_THGA_D
{
	unsigned int nThisFuncKey[3];
	double dArrivalTime[3];
	double dFactor[3];
	double dAngleHGA;     // Angle of Horizontal Ground Acceleration
	int    nDirectionD2E;   // 6.4.0 추가 DYNA2E  0=X, 1=Y, 2=Z         => 사용하지 않음  
	int    nScaleMethodD2E; // 6.4.0 추가 DYNA2E  0=Scaling, 1=Maximum  => 사용하지 않음  
	double dMaxValueD2E;    // 6.4.0 추가 DYNA2E                        => 사용하지 않음  
	int    nNumOutputD2E;   // 6.4.0 추가 DYNA2E                        => 사용하지 않음  
	void Initialize()
	{
		for(int i=0; i<3; i++)
		{
			nThisFuncKey[i]=0;
			dArrivalTime[i]=0.;
			dFactor[i]=1.;
		}
		dAngleHGA       = 0.0;    
		nDirectionD2E   = 0;  
		nScaleMethodD2E = 0;    
		dMaxValueD2E    = 0.;   
		nNumOutputD2E   = 0;
	}
	void Convert530(T_THGA_D_530& rData)
	{
		memcpy(nThisFuncKey, rData.nThisFuncKey, sizeof(nThisFuncKey));
		memcpy(dArrivalTime, rData.dArrivalTime, sizeof(dArrivalTime));
		memcpy(dFactor, rData.dFactor, sizeof(dFactor));
		dAngleHGA       = 0.0;    
		nDirectionD2E   = 0;  
		nScaleMethodD2E = 0;    
		dMaxValueD2E    = 0.;   
		nNumOutputD2E   = 0;
	}
	void Convert630(T_THGA_D_630& rData)
	{
		memcpy(nThisFuncKey, rData.nThisFuncKey, sizeof(nThisFuncKey));
		memcpy(dArrivalTime, rData.dArrivalTime, sizeof(dArrivalTime));
		memcpy(dFactor, rData.dFactor, sizeof(dFactor));
		dAngleHGA       = rData.dAngleHGA;    
		nDirectionD2E   = 0;  
		nScaleMethodD2E = 0;    
		dMaxValueD2E    = 0.;   
		nNumOutputD2E   = 0;
	}
};
struct T_THGA_UDRD_D_530
{
	T_THGA_K key;
	T_THGA_D_530 data;
};
struct T_THGA_UDRD_D_630
{
	T_THGA_K key;
	T_THGA_D_630 data;
};
struct T_THGA_UDRD_D
{
	T_THGA_K key;
	T_THGA_D data;
};
struct T_THGA_UNIT
{
	T_THGA_UNIT()
	{
		nThisFuncKey=D_UNITSYS_NONE;
		dArrivalTime=D_UNITSYS_BASE_TIME;
		dFactor=D_UNITSYS_NONE;
		dAngleHGA=D_UNITSYS_BASE_DEGREE;
	}
	int nThisFuncKey;
	int dArrivalTime;
	int dFactor;
	int dAngleHGA;
};
#define HASHSIZETHGA 1

// Multiple Support Excitation
#define T_THMS_KEY unsigned int
union T_THMS_K
{
	T_THMS_KEY keymap;
	struct
	{
		unsigned int entity : 20;  // 0-1048576 (절점번호)
		unsigned int serial : 12;  // 0-4096
	} key;
};

struct T_THMS_D
{
	T_THIS_K ThisKey;
	unsigned int nThisFuncKey[3];
	double dArrivalTime[3];
	double dFactor[3];
	double dAngleHGA;     // Angle of Horizontal Ground Acceleration    
	void Initialize()
	{
		ThisKey = 0;
		for(int i=0; i<3; i++)
		{
			nThisFuncKey[i]=0;
			dArrivalTime[i]=0.;
			dFactor[i]=1.;
		}
		dAngleHGA = 0.0;    
	}  
};
struct T_THMS_UDRD_D
{
	T_THMS_K key;
	T_THMS_D data;
};
struct T_THMS_UNIT
{
	T_THMS_UNIT()
	{
		nThisFuncKey=D_UNITSYS_NONE;
		dArrivalTime=D_UNITSYS_BASE_TIME;
		dFactor=D_UNITSYS_NONE;
		dAngleHGA=D_UNITSYS_BASE_DEGREE;
	}
	int nThisFuncKey;
	int dArrivalTime;
	int dFactor;
	int dAngleHGA;
};
#define HASHSIZETHMS HASHSIZENODE

// Dynamic Spatial Load
#define T_THSL_K UINT
struct T_THSL_D
{
	T_THIS_K nThisKey;      // LoadCaseKey
	T_STLD_K nStldKey;      // Spatial Load
	T_THFC_K nThisFuncKey;
	double dArrivalTime;
	double dFactor;
	void Initialize()
	{
		nThisKey = 0;
		nStldKey = 0;
		nThisFuncKey = 0;
		dArrivalTime = 0.0;
		dFactor = 1.0;
	}
};
struct T_THSL_UDRD_D
{
	T_THSL_K key;
	T_THSL_D data;
};
struct T_THSL_UNIT
{
	T_THSL_UNIT()
	{
		dArrivalTime=D_UNITSYS_BASE_TIME;
	}
	int dArrivalTime;
};
#define HASHSIZETHSL 1

// Define Time history Concurrent Result
#define T_THCR_K UINT
struct T_THCR_D
{
	CString GroupName;
	T_NODE_K MasterNode;
	CArray<T_NODE_K, T_NODE_K> aSubNode;
	int nOutType;    // 0:Disp. 1: Vel,  2: Acc.
	int nComponent;  // 0~5 : Dx~Rx

	void Initialize()
	{
		GroupName = _T("");
		MasterNode = 0;
		aSubNode.RemoveAll();
		nOutType   = 0;
		nComponent = 0;
	}

	T_THCR_D() { Initialize();}
	T_THCR_D(T_THCR_D& src) { *this = src; }
	T_THCR_D& operator=(T_THCR_D& src)
	{
		GroupName = src.GroupName;
		MasterNode = src.MasterNode;
		aSubNode.Copy(src.aSubNode);
		nOutType = src.nOutType;
		nComponent = src.nComponent;

		return *this;
	}
};

struct T_THCR_D_CH
{
	char GroupName[80];
	T_NODE_K MasterNode;
	CArray<T_NODE_K, T_NODE_K> aSubNode;
	int nOutType;    // 0:Disp. 1: Vel. 2: Acc.
	int nComponent;  // 0~5 : Dx~Rx

	void ConvertToString(T_THCR_D& rData)
	{
		ConvertCharStr(GroupName, rData.GroupName, sizeof(GroupName));
		rData.MasterNode = MasterNode;
		rData.aSubNode.Copy(aSubNode);
		rData.nOutType = nOutType;
		rData.nComponent = nComponent;
	}
	void ConvertToChar(T_THCR_D& rData)
	{
		ConvertStrChar(rData.GroupName, GroupName, sizeof(GroupName));
		MasterNode = rData.MasterNode;
		aSubNode.Copy(rData.aSubNode);
		nOutType = rData.nOutType;
		nComponent = rData.nComponent;
	}
};

struct T_THCR_UNIT
{
	T_THCR_UNIT()
	{
	}
};

struct T_THCR_UDRD_D
{
	T_THCR_K key;
	T_THCR_D data;
};

struct T_THCR_UDRD_D_CH
{
	T_THCR_K key;
	T_THCR_D_CH data;
};

struct T_THCR_UDRD_D_CH_RW
{
	T_THCR_K key;
	char GroupName[80];
	T_NODE_K MasterNode;
	int nCountSubNode;
	int nOutType;
	int nComponent;

	void GetThcr(T_THCR_K& rKey, T_THCR_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.GroupName, GroupName, sizeof(GroupName));
		rData.MasterNode = MasterNode;
		//rData.nCountSubNode 
		rData.nOutType = nOutType;
		rData.nComponent = nComponent;
	}
	void SetThcr(const T_THCR_K Key, const T_THCR_D_CH& rData)
	{
		key = Key;
		memcpy(GroupName, rData.GroupName, sizeof(GroupName));
		MasterNode = rData.MasterNode;
		nCountSubNode = (int)rData.aSubNode.GetSize();
		nOutType = rData.nOutType;
		nComponent = rData.nComponent;
	}
};
#define HASHSIZETHCR 1

// time history graph data
#define T_THRD_K unsigned int
struct T_THRD_D_501
{
	CString Name;
	unsigned int nEntity;
	int nFunctionType;
	int nSubType;
	int nPosition;
	int nComponent;
	BOOL bAllMode;
	int nSelectedMode;
};
struct T_THRD_D_570
{
	CString Name;
	unsigned int nEntity;
	int nFunctionType;   
	int nSubType;        
	int nPosition;       
	int nComponent;      
	BOOL bAllMode;
	int nSelectedMode;
	int nOption1;  
};
struct T_THRD_D_640
{
	CString Name;
	unsigned int nEntity;
	int nFunctionType;   
	int nSubType;        
	int nPosition;       
	int nComponent;      
	BOOL bAllMode;
	int nSelectedMode;
	int nOption1;  
	int nRefPoint;         // 1=Ground, 2=Add Ground Motion, 3=Another Node : 580추가
	T_NODE_K KeyRefNode;   // 580추가
	T_THIS_K KeyThis;      // 580추가
};
struct T_THRD_D
{
		CString Name;
		unsigned int nEntity; // Node(nFunctionType=1,2), Elem(nFunctionType=3,4,5,6,7,13), Nlnk(nFunctionType=8, nStepFunction=6)
													// Node(nStepFunction=2), Elem(nStepFunction=3,4)
													// Story(nFunctionType=12)
		int nFunctionType;   // 1=Reaction  2=Displacement  3=Truss  4=Beam  5=PlaneStress  6=Plate  7=Solid  8=General Link
												 // 9=Inelastic Hinge, 10=PlaneStrain, 11=Load Increment History, 12=Displ/Vel/Accel by Story, 13=Wall Force
		int nSubType;        // if nFuncType == 1                ; ?
												 // if nFuncType == 2                ; 1=Displ.  2=Vel.    3=Accel.
												 // if nFuncType == 3,4,5,7,10       ; 1=Force   2=Stress
												 // if nFuncType == 6                ; 1=Force   2=Stress  3=UnitForce
												 // if nFuncType == 8                ; 1=Deformation, 2=I-Node Force, 3=J-Node Force
												 // if nFuncType == 9                ; 1=Deform  2=Force
												 // if nFuncType == 11               ; 사용안함
												 // if nFuncType == 12               : 1=Displ.  2=Vel.    3=Accel
												 // if nFuncType == 13							 : 1=Force
												 // if nStepFunction == 2            ; 1=Displ.  2=Vel.    3=Accel.
												 // if nStepFunction == 3,4,5,7      ; 1=Force   2=Stress
												 
		int nPosition;       // if nFuncType == 1,2              ; 사용안함
												 // if nFuncType == 3,4              ; 1=I 2=J
												 // if nFuncType == 5, nSubType == 1 ; 1=I 2=J 3=K 4=L
												 // if nFuncType == 5, nSubType == 2 ; 1=CENT 2=I 3=J 4=K 5=L
												 // if nFuncType == 6, nSubType == 1 ; 1=I 2=J 3=K 4=L
												 // if nFuncType == 6, nSubType == 2 ; 1=CENT 2=I 3=J 4=K 5=L
												 // if nFuncType == 7, nSubType == 1 ; 1=I 2=J 3=K 4=L 5=M 6=N 7=O 8=P
												 // if nFuncType == 7, nSubType == 2 ; 1=CENT 2=I 3=J 4=K 5=L 6=M 7=N 8=O 9=P
												 // if nFuncType == 8                ; 사용안함
												 // if nFuncType == 9                ; Location으로 사용
												 // if nFuncType == 10, nSubType == 1; 1=I 2=J 3=K 4=L
												 // if nFuncType == 10, nSubType == 2; 1=CENT 2=I 3=J 4=K 5=L
												 // if nFuncType == 11               ; 사용안함
												 // if nFuncType == 12               : 사용안함
												 // if nFuncType == 13							 : 1=I 2=J
												 // if nStepFunction == 1,2          ; 사용안함
												 // if nStepFunction == 3            ; 1=I 2=J 3=M
												 // if nStepFunction == 4            ; 1=I 2=J
												 // if nStepFunction == 5            ; 1=I 2=J 3=M
												 // if nStepFunction == 6            ; 1=I 2=J                         
												 
		int nComponent;      // if nFuncType == 1                    ; ?
												 // if nFuncType == 2                    ; 1=DX 2=DY 3=DZ 4=RX 5=RY 6=RZ
												 // if nFuncType == 3, nSubType == 1     ; FX
												 // if nFuncType == 3, nSubType == 2     ; SX
												 // if nFuncType == 4, nSubType == 1     ; Fx Fy Fz Mm My Mz
		                     // if nFuncType == 13, nSubType == 1     ; Fx Fy Fz Mm My Mz
												 // if nFuncType == 4, nSubType == 2     ; Sx Sy Sz ByP ByM BzP BzM
												 // if nFuncType == 5, nSubType == 1     ; Fx Fy
												 // if nFuncType == 5, nSubType == 2     ; Sx Sy Sxy
												 // if nFuncType == 6, nSubType == 1     ; Fx Fy Fz Mx My Mz
												 // if nFuncType == 6, nSubType == 2     ; SxT SyT SxyT SxB SyB SxyB
												 // if nFuncType == 6, nSubType == 3     ; Fxx Fyy Fxy Mxx Myy Mzz Vxx Vyy
												 // if nFuncType == 7, nSubType == 1     ; FX FY FZ
												 // if nFuncType == 7, nSubType == 2     ; SXX SYY SZZ SXY SYZ SXZ
												 // if nFuncType == 8, nSubType == 1     ; Axial, Tran-y, Tran-z, Torsional, Rotation-y, Rotation-z
												 // if nFuncType == 8, nSubType == 2,3   ; Axial, Shear-y, Shear-z, Torsion, Moment-y, Moment-z
												 // if nFuncType == 10, nSubType == 1    ; Fx Fy Fz
												 // if nFuncType == 10, nSubType == 2    ; Sx Sy Sz Sxy
												 // if nFuncType == 11                   ; 사용안함
												 // if nFuncType == 12                   : 1=DX, 2=DY
												 // if nStepFunction == 2                ; 1=DX 2=DY 3=DZ 4=RX 5=RY 6=RZ
												 // if nStepFunction == 3, nSubType == 1 ; FX
												 // if nStepFunction == 3, nSubType == 2 ; SX
												 // if nStepFunction == 4, nSubType == 1 ; Fx Fy Fz Mm My Mz
												 // if nStepFunction == 4, nSubType == 2 ; Sx Sy Sz ByP ByM BzP BzM,C1,C2,C3,C4
												 // if nStepFunction == 5                ; Fx Fy Fz Mm My Mz
												 // if nStepFunction == 6                ; Axial, Shear-y, Shear-z, Torsion, Moment-y, Moment-z
												 
	BOOL bAllMode;
	int nSelectedMode;
	int nOption1;
	int nRefPoint;         // 1=Ground, 2=Add Ground Motion, 3=Another Node : 580추가
												 // 1=Ground, 2=Add Ground Motion, 3=Story Drift : nFuncType==12
	T_NODE_K KeyRefNode;   // 580추가
	T_THIS_K KeyThis;      // 580추가
	int    nGraphStep;        // 650추가  0=Graph Function, 1=Step Function
	int    nStepFunction;     // 650추가  1=Time Step, 2=Displ/Vel/Accel, 3=Truss Force/Stress, 4=Beam Force/Stress, 5=Designated DOF
														//          6=General Link Force 7=Wall Force
	int    nValueType;        // 650추가(nStepFunction=2, 3, 4, 5, 6 인 경우 사용)  
														// 0=Min, 1=Max, 2=Abs Max  
	double dTimeStep;         // 650추가
	int    nElemType4Iehg;    // 650추가  0=Beam, 1=General Link 
	int    nGlobalLocal;      // 650추가  0=Global, 1=Local : nFunctionType == 7(Solid), 10(Plane Strain)인 경우 사용
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
		KeyThis = 0;
		nGraphStep = 0;
		nStepFunction = 1;
		nValueType = 0;    
		dTimeStep = 0.;
		nElemType4Iehg = 0;
	nGlobalLocal = 0;
	}
	void Convert501(T_THRD_D_501& rData)
	{
		Name=rData.Name;;
		nEntity=rData.nEntity;
		nFunctionType=rData.nFunctionType;
		nSubType=rData.nSubType;
		nPosition=rData.nPosition;
		nComponent=rData.nComponent;
		bAllMode=rData.bAllMode;
		nSelectedMode=rData.nSelectedMode;
		nOption1=0;
		nRefPoint = 1;
		if(nFunctionType == 2 && nSubType == 3) nRefPoint = 2;
		KeyRefNode = 0;
		KeyThis = 0;
		nGraphStep = 0;
		nStepFunction = 2;
		nValueType = 0;    
		dTimeStep = 0.;
		nElemType4Iehg = 0;
	nGlobalLocal = 0;
	}
	void Convert570(T_THRD_D_570& rData)
	{
		Name=rData.Name;;
		nEntity=rData.nEntity;
		nFunctionType=rData.nFunctionType;
		nSubType=rData.nSubType;
		nPosition=rData.nPosition;
		nComponent=rData.nComponent;
		bAllMode=rData.bAllMode;
		nSelectedMode=rData.nSelectedMode;
		nOption1=rData.nOption1;
		nRefPoint = 1;
		if(nFunctionType == 2 && nSubType == 3) nRefPoint = 2;
		KeyRefNode = 0;
		KeyThis = 0;
		nGraphStep = 0;
		nStepFunction = 1;
		nValueType = 0;    
		dTimeStep = 0.;
		nElemType4Iehg = 0;
	nGlobalLocal = 0;
	}
	void Convert640(T_THRD_D_640& rData)
	{
		Name=rData.Name;;
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
		KeyThis = rData.KeyThis;
		nGraphStep = 0;
		nStepFunction = 1;
		nValueType = 0;    
		dTimeStep = 0.;
		nElemType4Iehg = 0;
	nGlobalLocal = 0;
	}
};
struct T_THRD_D_CH_501
{
	char Name[20];
	unsigned int nEntity;
	int nFunctionType;
	int nSubType;
	int nPosition;
	int nComponent;
	BOOL bAllMode;
	int nSelectedMode;
};
struct T_THRD_D_CH_570
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
};
struct T_THRD_D_CH_640
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
	T_THIS_K KeyThis;
};
struct T_THRD_D_CH
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
	T_THIS_K KeyThis;
	int    nGraphStep;        // 650추가  0=Graph Function, 1=Step Function
	int    nStepFunction; // 650추가  2=Displ/Vel/Accel, 3=Truss Force/Stress, 4=Beam Force/Stress  
	int    nValueType;        // 650추가  0=Min, 1=Max, 2=Abs Max  
	double dTimeStep;
	int    nElemType4Iehg;    // 650추가  0=Beam, 1=General Link 
	int    nGlobalLocal;      // 650추가  0=Global, 1=Local : nFunctionType == 7(Solid), 10(Plane Strain)인 경우 사용
	void ConvertToString(T_THRD_D& rData)
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
		rData.KeyThis = KeyThis;
		rData.nGraphStep = nGraphStep;
		rData.nStepFunction = nStepFunction;
		rData.nValueType = nValueType;    
		rData.dTimeStep = dTimeStep;
		rData.nElemType4Iehg = nElemType4Iehg;
	rData.nGlobalLocal = nGlobalLocal;
	}
	void ConvertToChar(T_THRD_D& rData)
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
		KeyThis = rData.KeyThis;
		nGraphStep = rData.nGraphStep;
		nStepFunction = rData.nStepFunction;
		nValueType = rData.nValueType;    
		dTimeStep = rData.dTimeStep;
		nElemType4Iehg = rData.nElemType4Iehg;
	nGlobalLocal = rData.nGlobalLocal;
	}
	void Convert501(T_THRD_D_CH_501& rData)
	{
		memcpy(Name, rData.Name, sizeof(Name));
		nEntity = rData.nEntity;
		nFunctionType=rData.nFunctionType;
		nSubType=rData.nSubType;
		nPosition=rData.nPosition;
		nComponent=rData.nComponent;
		bAllMode=rData.bAllMode;
		nSelectedMode=rData.nSelectedMode;
		nOption1=0;
		nRefPoint = 1;
		if(nFunctionType == 2 && nSubType == 3) nRefPoint = 2;
		KeyRefNode = 0;
		KeyThis = 0;
		nGraphStep = 0;
		nStepFunction = 1;
		nValueType = 0;    
		dTimeStep = 0.;
		nElemType4Iehg = 0;
	nGlobalLocal = 0;
	}
	void Convert570(T_THRD_D_CH_570& rData)
	{
		memcpy(Name, rData.Name, sizeof(Name));
		nEntity = rData.nEntity;
		nFunctionType=rData.nFunctionType;
		nSubType=rData.nSubType;
		nPosition=rData.nPosition;
		nComponent=rData.nComponent;
		bAllMode=rData.bAllMode;
		nSelectedMode=rData.nSelectedMode;
		nOption1=rData.nOption1;
		nRefPoint = 1;
		if(nFunctionType == 2 && nSubType == 3) nRefPoint = 2;
		KeyRefNode = 0;
		KeyThis = 0;
		nGraphStep = 0;
		nStepFunction = 1;
		nValueType = 0;    
		dTimeStep = 0.;
		nElemType4Iehg = 0;
	nGlobalLocal = 0;
	}
	void Convert640(T_THRD_D_CH_640& rData)
	{
		memcpy(Name, rData.Name, sizeof(Name));
		nEntity = rData.nEntity;
		nFunctionType=rData.nFunctionType;
		nSubType=rData.nSubType;
		nPosition=rData.nPosition;
		nComponent=rData.nComponent;
		bAllMode=rData.bAllMode;
		nSelectedMode=rData.nSelectedMode;
		nOption1=rData.nOption1;
		nRefPoint = rData.nRefPoint;    
		KeyRefNode = rData.KeyRefNode;
		KeyThis = rData.KeyThis;
		nGraphStep = 0;
		nStepFunction = 1;
		nValueType = 0;    
		dTimeStep = 0.;
		nElemType4Iehg = 0;
	nGlobalLocal = 0;
	}
};
struct T_THRD_UDRD_D
{
	T_THRD_K key;
	T_THRD_D data;
};
struct T_THRD_UDRD_D_CH_501
{
	T_THRD_K key;
	T_THRD_D_CH_501 data;
};
struct T_THRD_UDRD_D_CH_570
{
	T_THRD_K key;
	T_THRD_D_CH_570 data;
};
struct T_THRD_UDRD_D_CH_640
{
	T_THRD_K key;
	T_THRD_D_CH_640 data;
};
struct T_THRD_UDRD_D_CH
{
	T_THRD_K key;
	T_THRD_D_CH data;
};
struct T_THRD_UNIT
{
	T_THRD_UNIT()
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
		KeyThis=D_UNITSYS_NONE;
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
	int KeyThis;
};
#define HASHSIZETHRD 1

////////////////////
// time Stage Step Graph Data
#define T_STRD_K unsigned int
struct T_STRD_D
{
		CString Name;
		unsigned int nEntity; // Node(nFunctionType=1,2), Elem(nFunctionType=3,4,5,6,7), Nlnk(nFunctionType=8)
		int nFunctionType;   // 1=Reaction  2=Displacement  3=Truss  4=Beam  5=PlaneStress  6=Plate  7=Solid  8=Nllink
		int nSubType;        // if nFuncType == 1                ; ?
												 // if nFuncType == 2                ; 1=Displ.  2=Vel.    3=Accel.
												 // if nFuncType == 3,4,5,7          ; 1=Force   2=Stress
												 // if nFuncType == 6                ; 1=Force   2=Stress  3=UnitForce
												 // if nFuncType == 8                ; 1=Deformation, 2=I-Node Force, 3=J-Node Force
		int nPosition;       // if nFuncType == 1,2              ; 사용안함
												 // if nFuncType == 3,4              ; 1=I 2=J
												 // if nFuncType == 5, nSubType == 1 ; 1=I 2=J 3=K 4=L
												 // if nFuncType == 5, nSubType == 2 ; 1=CENT 2=I 3=J 4=K 5=L
												 // if nFuncType == 6, nSubType == 1 ; 1=I 2=J 3=K 4=L
												 // if nFuncType == 6, nSubType == 2 ; 1=CENT 2=I 3=J 4=K 5=L
												 // if nFuncType == 7, nSubType == 1 ; 1=I 2=J 3=K 4=L 5=M 6=N 7=O 8=P
												 // if nFuncType == 7, nSubType == 2 ; 1=CENT 2=I 3=J 4=K 5=L 6=M 7=N 8=O 9=P
												 // if nFuncType == 8                ; 사용안함
		int nComponent;      // if nFuncType == 1   ; ?
												 // if nFuncType == 2   ; 1=DX 2=DY 3=DZ 4=RX 5=RY 6=RZ
												 // if nFuncType == 3, nSubType == 1 ; FX
												 // if nFuncType == 3, nSubType == 2 ; SX
												 // if nFuncType == 4, nSubType == 1 ; Fx Fy Fz Mm My Mz
												 // if nFuncType == 4, nSubType == 2 ; Sx Sy Sz ByP ByM BzP BzM
												 // if nFuncType == 5, nSubType == 1 ; Fx Fy
												 // if nFuncType == 5, nSubType == 2 ; Sx Sy Sxy
												 // if nFuncType == 6, nSubType == 1 ; Fx Fy Fz Mx My Mz
												 // if nFuncType == 6, nSubType == 2 ; SxT SyT SxyT SxB SyB SxyB
												 // if nFuncType == 6, nSubType == 3 ; Fxx Fyy Fxy Mxx Myy Mzz Vxx Vyy
												 // if nFuncType == 7, nSubType == 1 ; FX FY FZ
												 // if nFuncType == 7, nSubType == 2 ; SXX SYY SZZ SXY SYZ SXZ
												 // if nFuncType == 8, nSubType == 1   ; Axial, Tran-y, Tran-z, Torsional, Rotation-y, Rotation-z
												 // if nFuncType == 8, nSubType == 2,3 ; Axial, Shear-y, Shear-z, Torsion, Moment-y, Moment-z
	BOOL bAllMode;
	int nSelectedMode;
	int nOption1;
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
	}
};
struct T_STRD_D_CH
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
	void ConvertToString(T_STRD_D& rData)
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
	}
	void ConvertToChar(T_STRD_D& rData)
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
	}
};
struct T_STRD_UDRD_D
{
	T_STRD_K key;
	T_STRD_D data;
};
struct T_STRD_UDRD_D_CH
{
	T_STRD_K key;
	T_STRD_D_CH data;
};
struct T_STRD_UNIT
{
	T_STRD_UNIT()
	{
		Name=D_UNITSYS_NONE;
		nEntity=D_UNITSYS_NONE;
		nFunctionType=D_UNITSYS_NONE;
		nSubType=D_UNITSYS_NONE;
		nPosition=D_UNITSYS_NONE;
		nComponent=D_UNITSYS_NONE;
		bAllMode=D_UNITSYS_NONE;
		nSelectedMode=D_UNITSYS_NONE;
	}
	int Name;
	int nEntity;
	int nFunctionType;
	int nSubType;
	int nPosition;
	int nComponent;
	int bAllMode;
	int nSelectedMode;
};
#define HASHSIZESTRD 1

struct T_GRDP_BASE_632
{
	int  nGrupType;         // 0=Structure Group, 1=Boundary Group
	UINT GrupBngrKey;  
	double dDampingRatio;  
};
struct T_GRDP_BASE_785
{
	int  nGrupType;         // 0=Structure Group, 1=Boundary Group
	UINT GrupBngrKey;  
	double dDampingRatio;
	double dStifCoef;       // 6.4.0 추가
	double dMassCoef;
};
struct T_GRDP_BASE
{
	int  nGrupType;         // 0=Structure Group, 1=Boundary Group , 2=Material
	UINT GrupBngrKey;       // or MatlK
	double dDampingRatio;
	double dStifCoef;       // 6.4.0 추가
	double dMassCoef;       // 6.4.0 추가 

	// 7.9.0
	BOOL   bMassProp;
	BOOL   bStiffProp;
	int    nDirectCalc;  // 0=Direct Specification, 1=Calculate From Modal Damping

	int    nFreqPeriod;  // 0=Frequency, 1=Period
	double dFreqOne;
	double dFreqTwo;
	double dPeriodOne;   // sec
	double dPeriodTwo;   // sec
	int    nDampInputType;    // 0 = Use Material Data,  1 = Direct Define
	double dDampingRatio2;  // mode 2     

	void Initialize()
	{
		nGrupType = 0;
		GrupBngrKey=0;    
		dDampingRatio=0.05;    
		dStifCoef = 0.;
		dMassCoef = 0.;

		bMassProp  = TRUE;
		bStiffProp = TRUE;
		nDirectCalc = 0;
		nFreqPeriod = 0;      
		dFreqOne = 0.0;
		dFreqTwo = 0.0;
		dPeriodOne = 0.0;
		dPeriodTwo = 0.0;
		nDampInputType = 0;
		dDampingRatio2 = 0.05;
	}
	T_GRDP_BASE() { Initialize(); }

	void Convert632(T_GRDP_BASE_632& rData)
	{
		Initialize();
		nGrupType = rData.nGrupType;
		GrupBngrKey = rData.GrupBngrKey;    
		dDampingRatio = dDampingRatio2 = rData.dDampingRatio;
		bMassProp = FALSE;
		nDampInputType = 1;
	}
	void Convert785(T_GRDP_BASE_785& rData)
	{
		Initialize();
		nGrupType = rData.nGrupType;
		GrupBngrKey = rData.GrupBngrKey;    
		dDampingRatio = dDampingRatio2 = rData.dDampingRatio;
		dStifCoef = rData.dStifCoef;
		dMassCoef = rData.dMassCoef;
		bMassProp = FALSE;
		nDampInputType = 1;
	}
};

#define T_GRDP_K unsigned int
struct T_GRDP_D_632
{
	double dDampingDefault;
	CArray<T_GRDP_BASE_632, T_GRDP_BASE_632&> aGrupDamping;
};
struct T_GRDP_D_640
{
	double dDampingDefault;
	double dStifCoefDefault;   // 6.4.0 추가
	double dMassCoefDefault;   // 6.4.0 추가
	CArray<T_GRDP_BASE, T_GRDP_BASE&> aGrupDamping;
};
struct T_GRDP_D_785
{
	double dDampingDefault;
	double dStifCoefDefault;   // 6.4.0 추가
	double dMassCoefDefault;   // 6.4.0 추가
	CArray<T_GRDP_BASE_785, T_GRDP_BASE_785&> aGrupDamping;
	BOOL   bCalculateWhenUsed; // 6.5.0 추가
};
struct T_GRDP_D
{
	double dDampingDefault;
	double dStifCoefDefault;   // 6.4.0 추가
	double dMassCoefDefault;   // 6.4.0 추가
	CArray<T_GRDP_BASE, T_GRDP_BASE&> aGrupDamping;
	BOOL   bCalculateWhenUsed; // 6.5.0 추가

	// 790
	BOOL   bMassPropDefault;
	BOOL   bStiffPropDefault;
	int    nDirectCalcDefault;  // 0=Direct Specification, 1=Calculate From Modal Damping
	int    nFreqPeriodDefault;  // 0=Frequency, 1=Period
	double dFreqOneDefault;
	double dFreqTwoDefault;
	double dPeriodOneDefault;   // sec
	double dPeriodTwoDefault;   // sec
	double dDampingDefault2;
	
	BOOL bExistElement;   // Element Mass & Stiffness Proportional 데이타가 설정되었는지
	BOOL bExistStrain;    // Strain Energy Damping 데이타가 설정되었는지

	double dStrainDampingDef;
	CArray<T_GRDP_BASE, T_GRDP_BASE&> aStrainGrupDamping;

	// 중복 설정시 우선순위
	int nElemGroupPriority;   // 0 : Material Data, 1: Structure Group
	int nElemValuePriority;   // 0 : Small Value,  1: Large Value
	int nStrainGroupPriority;
	int nStrainValuePriority;

	void Initialize()
	{
		dDampingDefault = 0.05;
		dStifCoefDefault = 0.;
		dMassCoefDefault = 0.;
		aGrupDamping.RemoveAll();
		bCalculateWhenUsed = TRUE;

		bMassPropDefault  = TRUE;
		bStiffPropDefault = TRUE;
		nDirectCalcDefault = 0;
		nFreqPeriodDefault = 0;      
		dFreqOneDefault = 0.0;
		dFreqTwoDefault = 0.0;
		dPeriodOneDefault = 0.0;
		dPeriodTwoDefault = 0.0;
		dDampingDefault2 = 0.05;

		bExistElement = FALSE;
		bExistStrain = FALSE;
		dStrainDampingDef = 0.05;
		aStrainGrupDamping.RemoveAll();

		nElemGroupPriority = 0;
		nElemValuePriority = 0;
		nStrainGroupPriority = 0;
		nStrainValuePriority = 0;
	}
	T_GRDP_D()  {}
	T_GRDP_D(T_GRDP_D& src) {*this = src;}
	T_GRDP_D& operator=(T_GRDP_D& src)
	{    
		dDampingDefault = src.dDampingDefault;
		dStifCoefDefault = src.dStifCoefDefault;
		dMassCoefDefault = src.dMassCoefDefault;
		aGrupDamping.Copy(src.aGrupDamping);
		bCalculateWhenUsed = src.bCalculateWhenUsed;

		bMassPropDefault  = src.bMassPropDefault ;
		bStiffPropDefault = src.bStiffPropDefault;
		nDirectCalcDefault = src.nDirectCalcDefault;
		nFreqPeriodDefault = src.nFreqPeriodDefault;      
		dFreqOneDefault = src.dFreqOneDefault;
		dFreqTwoDefault = src.dFreqTwoDefault;
		dPeriodOneDefault = src.dPeriodOneDefault;
		dPeriodTwoDefault = src.dPeriodTwoDefault;
		dDampingDefault2 = src.dDampingDefault2;

		bExistElement = src.bExistElement;
		bExistStrain = src.bExistStrain;
		dStrainDampingDef = src.dStrainDampingDef;
		aStrainGrupDamping.Copy(src.aStrainGrupDamping);

		nElemGroupPriority = src.nElemGroupPriority;
		nElemValuePriority = src.nElemValuePriority;
		nStrainGroupPriority = src.nStrainGroupPriority;
		nStrainValuePriority = src.nStrainValuePriority;
		return *this;
	}  
};
struct T_GRDP_UDRD_D_632
{  
	T_GRDP_D_632 data;
};
struct T_GRDP_UDRD_D_640
{  
	T_GRDP_D_640 data;
};
struct T_GRDP_UDRD_D
{  
	T_GRDP_D data;
};
struct T_GRDP_UDRD_D_RW_632
{  
	double dDampingDefault;
	int nCountGrdp;  
};
struct T_GRDP_UDRD_D_RW_640
{  
	double dDampingDefault;
	double dStifCoefDefault;   // 6.4.0 추가
	double dMassCoefDefault;   // 6.4.0 추가
	int    nCountGrdp;
	int    nTemp;
};
struct T_GRDP_UDRD_D_RW_785
{  
	double dDampingDefault;
	double dStifCoefDefault;   // 6.4.0 추가
	double dMassCoefDefault;   // 6.4.0 추가
	int    nCountGrdp;
	int    nTemp;
	BOOL   bCalculateWhenUsed; // 6.5.0 추가
};
struct T_GRDP_UDRD_D_RW
{  
	double dDampingDefault;
	double dStifCoefDefault;   // 6.4.0 추가
	double dMassCoefDefault;   // 6.4.0 추가
	int    nCountGrdp;
	int    nTemp;
	BOOL   bCalculateWhenUsed; // 6.5.0 추가
	BOOL   bMassPropDefault;     // 7.9.0
	BOOL   bStiffPropDefault;
	int    nDirectCalcDefault;  // 0=Direct Specification, 1=Calculate From Modal Damping
	int    nFreqPeriodDefault;  // 0=Frequency, 1=Period
	double dFreqOneDefault;
	double dFreqTwoDefault;
	double dPeriodOneDefault;   // sec
	double dPeriodTwoDefault;   // sec
	double dDampingDefault2;  // mode 2  
	BOOL bExistElement;
	BOOL bExistStrain;
	double dStrainDampingDef;
	int    nCountStrainGrdp;

	int nElemGroupPriority;   // 0 : Material Data, 1: Structure/Boundary Group
	int nElemValuePriority;   // 0 : Small Value,  1: Large Value
	int nStrainGroupPriority;
	int nStrainValuePriority;

	void Initialize()
	{
		dDampingDefault = 0.05;
		dStifCoefDefault = 0.;
		dMassCoefDefault = 0.;
		nCountGrdp = 0;
		nTemp = 0;
		bCalculateWhenUsed = TRUE;
		
		bMassPropDefault  = FALSE;
		bStiffPropDefault = TRUE;
		nDirectCalcDefault = 0;
		nFreqPeriodDefault = 0;      
		dFreqOneDefault = 0.0;
		dFreqTwoDefault = 0.0;
		dPeriodOneDefault = 0.0;
		dPeriodTwoDefault = 0.0;
		dDampingDefault2 = 0.05;
		bExistElement = TRUE;
		bExistStrain = TRUE;
		dStrainDampingDef = 0.05;
		nCountStrainGrdp = 0;
		nElemGroupPriority = 0;
		nElemValuePriority = 0;
		nStrainGroupPriority = 0;
		nStrainValuePriority = 0;
	}
	void GetGrdp(T_GRDP_D& rData)
	{
		rData.dDampingDefault = dDampingDefault;    
		rData.dStifCoefDefault = dStifCoefDefault;
		rData.dMassCoefDefault = dMassCoefDefault;
		rData.bCalculateWhenUsed = bCalculateWhenUsed;
		rData.bMassPropDefault  = bMassPropDefault ;
		rData.bStiffPropDefault = bStiffPropDefault;
		rData.nDirectCalcDefault = nDirectCalcDefault;
		rData.nFreqPeriodDefault = nFreqPeriodDefault;      
		rData.dFreqOneDefault = dFreqOneDefault;
		rData.dFreqTwoDefault = dFreqTwoDefault;
		rData.dPeriodOneDefault = dPeriodOneDefault;
		rData.dPeriodTwoDefault = dPeriodTwoDefault;
		rData.dDampingDefault2 = dDampingDefault2;
		rData.bExistElement = bExistElement;
		rData.bExistStrain = bExistStrain;
		rData.dStrainDampingDef = dStrainDampingDef;
		rData.nElemGroupPriority = nElemGroupPriority;
		rData.nElemValuePriority = nElemValuePriority;
		rData.nStrainGroupPriority = nStrainGroupPriority;
		rData.nStrainValuePriority = nStrainValuePriority;
	}
	void SetGrdp(T_GRDP_D& rData)
	{
		dDampingDefault = rData.dDampingDefault;
		dStifCoefDefault = rData.dStifCoefDefault;
		dMassCoefDefault = rData.dMassCoefDefault;
		nCountGrdp = (int)rData.aGrupDamping.GetSize();
		bCalculateWhenUsed = rData.bCalculateWhenUsed;
		bMassPropDefault  = rData.bMassPropDefault ;
		bStiffPropDefault = rData.bStiffPropDefault;
		nDirectCalcDefault = rData.nDirectCalcDefault;
		nFreqPeriodDefault = rData.nFreqPeriodDefault;      
		dFreqOneDefault = rData.dFreqOneDefault;
		dFreqTwoDefault = rData.dFreqTwoDefault;
		dPeriodOneDefault = rData.dPeriodOneDefault;
		dPeriodTwoDefault = rData.dPeriodTwoDefault;
		dDampingDefault2 = rData.dDampingDefault2;
		dDampingDefault2 = rData.dDampingDefault2;
		bExistElement = rData.bExistElement;
		bExistStrain = rData.bExistStrain;
		dStrainDampingDef = rData.dStrainDampingDef;
		nCountStrainGrdp = (int)rData.aStrainGrupDamping.GetSize();
		nElemGroupPriority = rData.nElemGroupPriority;
		nElemValuePriority = rData.nElemValuePriority;
		nStrainGroupPriority = rData.nStrainGroupPriority;
		nStrainValuePriority = rData.nStrainValuePriority;
	}
	void Convert785(T_GRDP_UDRD_D_RW_785& rData)
	{
		Initialize();
//    dDampingDefault = rData.dDampingDefault;
		dStifCoefDefault = rData.dStifCoefDefault;
		dMassCoefDefault = rData.dMassCoefDefault;
		nCountGrdp = rData.nCountGrdp;
		bCalculateWhenUsed = rData.bCalculateWhenUsed;
		dStrainDampingDef = rData.dDampingDefault;
	}
	void Convert640(T_GRDP_UDRD_D_RW_640& rData)
	{
		Initialize();
//    dDampingDefault = rData.dDampingDefault;
		dStifCoefDefault = rData.dStifCoefDefault;
		dMassCoefDefault = rData.dMassCoefDefault;
		nCountGrdp = rData.nCountGrdp;
		dStrainDampingDef = rData.dDampingDefault;
	}
	void Convert632(T_GRDP_UDRD_D_RW_632& rData)
	{
		Initialize();
//    dDampingDefault = rData.dDampingDefault;
		nCountGrdp = rData.nCountGrdp;
		dStrainDampingDef = rData.dDampingDefault;
	}
};

struct T_GRDP_UNIT
{
	T_GRDP_UNIT()
	{
		dPeriod = D_UNITSYS_BASE_TIME;
	}
	int dPeriod;
};
#define HASHSIZEGRDP 1

#define T_THAT_K T_THIS_K

struct T_THAT_D
{
	double dStartTime;
	double dEndTime;
	double dIncrement;
	void Initialize()
	{
		dStartTime = 0.;
		dEndTime   = 0.;
		dIncrement = 0.;
	}
	T_THAT_D() {}
	T_THAT_D(T_THAT_D& src){*this = src;}
	T_THAT_D& operator=(T_THAT_D& src)
	{
		dStartTime = src.dStartTime;
		dEndTime   = src.dEndTime;
		dIncrement = src.dIncrement;
		return *this;
	}
};
struct T_THAT_D_CH
{
	double dStartTime;
	double dEndTime;
	double dIncrement;
	void ConvertToString(T_THAT_D& rData)
	{
		rData.dStartTime = dStartTime;
		rData.dEndTime   = dEndTime  ;
		rData.dIncrement = dIncrement;
	}
	void ConvertToChar(T_THAT_D& rData)
	{
		dStartTime = rData.dStartTime;
		dEndTime   = rData.dEndTime  ;
		dIncrement = rData.dIncrement;
	}
};
struct T_THAT_UDRD_D
{
	T_THAT_K key;
	T_THAT_D data;
};
struct T_THAT_UDRD_D_CH
{
	T_THAT_K key;
	T_THAT_D_CH data;
};
struct T_THAT_UNIT
{
	T_THAT_UNIT()
	{
		dStartTime=D_UNITSYS_BASE_TIME;;    
		dEndTime=D_UNITSYS_BASE_TIME;;    
		dIncrement=D_UNITSYS_BASE_TIME;;    
	}
	int dStartTime;
	int dEndTime;
	int dIncrement;
};
#define HASHSIZETHAT 1

// Time History Swept Frequency Graph - Swept Frequency Response Analysis
#define T_THSF_K UINT
struct T_THSF_D
{
	T_NODE_K	NodeK;
	CArray<T_THIS_K, T_THIS_K> aThisK;
	int nTypeRes;   // 0 Disp;1 Velo;2 Accl;3 Limit
	int nLocation;  // Location. 0:max;1:min;2:all;
	int nComp;      // Component 0:Fx, ..., 5:Mz
	double dFrequency;
	double dPeak1;	//Peak 0-75%
	double dPeak2;	//Peak 75-125%
	double dPeak3;	//Speed peak
	double dPeak4;	//Peak acceleration

	T_THSF_D() { Initialize(); }
	T_THSF_D(T_THSF_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		NodeK = 0;
		aThisK.RemoveAll();
		nTypeRes = 0;
		nLocation = 0;
		nComp = 0;
		dFrequency = 50;
		dPeak1 = 0.00003;
		dPeak2 = 0.00002;
		dPeak3 = 0.01;
		dPeak4 = 1.0;
	}
	T_THSF_D& operator=(T_THSF_D& src)
	{
		NodeK = src.NodeK;
		aThisK.Copy(src.aThisK);
		nTypeRes = src.nTypeRes;
		nLocation = src.nLocation;
		nComp = src.nComp;
		dFrequency = src.dFrequency;
		dPeak1 = src.dPeak1;
		dPeak2 = src.dPeak2;
		dPeak3 = src.dPeak3;
		dPeak4 = src.dPeak4;
		return *this;
	}
	BOOL operator==(const T_THSF_D& src)
	{
		if (nTypeRes != src.nTypeRes) return FALSE;
		if (NodeK != src.NodeK) return FALSE;
		if (nLocation != src.nLocation) return FALSE;
		if (nComp != src.nComp) return FALSE;
		if (dFrequency != src.dFrequency) return FALSE;
		if (aThisK.GetSize() != src.aThisK.GetSize())return FALSE;
		for (int i = 0; i < aThisK.GetSize(); i++)
		{
			if (aThisK[i] != src.aThisK[i])
				return FALSE;
		}
		return TRUE;
	}
};
struct T_THSF_UDRD_D
{
	T_THSF_K key;
	T_THSF_D data;
};
struct T_THSF_UDRD_D_RW
{
	T_THSF_K key;
	T_NODE_K	NodeK;
	int nThisKCount; //CArray<T_THIS_K, T_THIS_K&> aThisK;
	int nTypeRes;   // D_THEF_TYPE
	int nLocation;  // Location. 0:max;1:min;2:all;
	int nComp;      // Component 0:Fx, ..., 5:Mz
	double dFrequency;
	double dPeak1;	//Peak 0-75%
	double dPeak2;	//Peak 75-125%
	double dPeak3;	//Speed peak
	double dPeak4;	//Peak acceleration

	void GetThsf(T_THSF_K& rKey, T_THSF_D& rData)
	{
		rKey = key;
		rData.NodeK = NodeK;
		rData.aThisK.RemoveAll();
		rData.nTypeRes = nTypeRes;
		rData.nLocation = nLocation;
		rData.nComp = nComp;
		rData.dPeak1 = dPeak1;
		rData.dPeak2 = dPeak2;
		rData.dPeak3 = dPeak3;
		rData.dPeak4 = dPeak4;
	}
	void SetThsf(const T_THSF_K Key, const T_THSF_D& rData)
	{
		key = Key;
		NodeK = rData.NodeK;
		nThisKCount = int(rData.aThisK.GetSize());
		nTypeRes = rData.nTypeRes;
		nLocation = rData.nLocation;
		nComp =  rData.nComp;
		dPeak1 = rData.dPeak1;
		dPeak2 = rData.dPeak2;
		dPeak3 = rData.dPeak3;
		dPeak4 = rData.dPeak4;
	}
};
#define HASHSIZETHSF 101

// Time History Result Graph - General Link
#define T_THRG_K UINT
struct T_THRG_D
{
	T_NLNK_K NlnkK;
	int nTypeRes;   // Type of Result. 0:Force-Deformation, 1:Force, 2:Deformation
	int nLocation;  // Location. 0:I-Node, 1:J-Node
	int nComp;      // Component 0:Fx, ..., 5:Mz
	T_THIS_K ThisK;

	T_THRG_D() { Initialize(); }
	T_THRG_D(T_THRG_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		NlnkK = 0;
		nTypeRes=0;
		nLocation=0;
		nComp=0;
		ThisK=0;
	}
	T_THRG_D& operator=(T_THRG_D& src)
	{
		NlnkK = src.NlnkK;
		nTypeRes  = src.nTypeRes;
		nLocation = src.nLocation;
		nComp     = src.nComp;
		ThisK     = src.ThisK;
		return *this;
	}
	BOOL operator==(const T_THRG_D& src)
	{
		if(NlnkK     != src.NlnkK    ) return FALSE;
		if(nTypeRes  != src.nTypeRes ) return FALSE;
		if(nLocation != src.nLocation) return FALSE;
		if(nComp     != src.nComp    ) return FALSE;
		if(ThisK     != src.ThisK    ) return FALSE;
		return TRUE;
	}
};
struct T_THRG_UDRD_D
{
	T_THRG_K key;
	T_THRG_D data;
};
struct T_THRG_UDRD_D_RW
{
	T_THRG_K key;
	
	T_NLNK_K NlnkK;
	int nTypeRes;   // Type of Result
	int nLocation;  // Location
	int nComp;      // Component
	T_THIS_K ThisK;

	void GetThrg(T_THRG_K& rKey, T_THRG_D& rData)
	{
		rKey=key;
		rData.NlnkK = NlnkK;
		rData.nTypeRes=nTypeRes;
		rData.nLocation=nLocation;
		rData.nComp=nComp;
		rData.ThisK=ThisK;
	}
	void SetThrg(const T_THRG_K Key, const T_THRG_D& rData)
	{
		key=Key;
		NlnkK = rData.NlnkK;
		nTypeRes = rData.nTypeRes;
		nLocation = rData.nLocation;
		nComp = rData.nComp;
		ThisK = rData.ThisK;
	}
};
#define HASHSIZETHRG 101

// Time History Result Graph - Inelastic Hinge
#define D_THRI_TYPE_FRCDFM 0
#define D_THRI_TYPE_FORCE  1
#define D_THRI_TYPE_DEFORM 2 
#define T_THRI_K UINT
struct T_THRI_D
{
	int nTypeElem;  // Type of Element
	T_KEY PropK;    // nTypeElem==0,1 : T_ELEM_K
									// nTypeElem==2   : User Wall ID
									// nTypeElem==3   : T_NLNK_K
	T_STOR_K StorK; // nTypeElem==2일 때만 사용
	int nTypeRes;   // Type of Result. 0:Force-Deformation, 1:Force, 2:Deformation
	int nLocation;  // Location. 0:I-Node, 1:J-Node
	int nComp;      // Component 0:Fx, ..., 5:Mz
	T_THIS_K ThisK;

	T_THRI_D() { Initialize(); }
	T_THRI_D(T_THRI_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		nTypeElem=0;
		PropK=0;
		StorK=0;
		nTypeRes=0;
		nLocation=0;
		nComp=0;
		ThisK=0;
	}
	T_THRI_D& operator=(T_THRI_D& src)
	{
		nTypeElem = src.nTypeElem;
		PropK     = src.PropK    ;
		StorK     = src.StorK    ;
		nTypeRes  = src.nTypeRes;
		nLocation = src.nLocation;
		nComp     = src.nComp;
		ThisK     = src.ThisK;
		return *this;
	}
	BOOL operator==(const T_THRI_D& src)
	{
		if(nTypeElem != src.nTypeElem) return FALSE;
		if(PropK     != src.PropK    ) return FALSE;
		if(StorK     != src.StorK    ) return FALSE;
		if(nTypeRes  != src.nTypeRes ) return FALSE;
		if(nLocation != src.nLocation) return FALSE;
		if(nComp     != src.nComp    ) return FALSE;
		if(ThisK     != src.ThisK    ) return FALSE;
		return TRUE;
	}
};
struct T_THRI_UDRD_D
{
	T_THRI_K key;
	T_THRI_D data;
};
struct T_THRI_UDRD_D_RW
{
	T_THRI_K key;

	int nTypeElem;  // Type of Element
	T_KEY    PropK;
	T_STOR_K StorK;
	int nTypeRes;   // Type of Result
	int nLocation;  
	int nComp;      // Component
	T_THIS_K ThisK;

	void GetThri(T_THRI_K& rKey, T_THRI_D& rData)
	{
		rKey=key;
		rData.nTypeElem=nTypeElem;
		rData.PropK=PropK;
		rData.StorK=StorK;
		rData.nTypeRes=nTypeRes;
		rData.nLocation=nLocation;
		rData.nComp=nComp;
		rData.ThisK=ThisK;
	}
	void SetThri(const T_THRI_K Key, const T_THRI_D& rData)
	{
		key=Key;
		nTypeElem = rData.nTypeElem;
		PropK = rData.PropK;
		StorK = rData.StorK;
		nTypeRes = rData.nTypeRes;
		nLocation = rData.nLocation;
		nComp = rData.nComp;
		ThisK = rData.ThisK;
	}
};
#define HASHSIZETHRI 101

// Time History Result Graph - Seismic Control Devices
#define D_THRS_TYPE_FRCDFM 0
#define D_THRS_TYPE_FRCVLO 1
#define D_THRS_TYPE_FORCE  2 
#define D_THRS_TYPE_DEFORM 3
#define D_THRS_TYPE_VELCTY 4
#define D_THRS_TYPE_DCTFAC 5
#define D_THRS_TYPE_CUMUPD 6
#define D_THRS_TYPE_ENERGY 7

#define T_THRS_K UINT
struct T_THRS_D
{
	T_NLNK_K NlnkK;
	int nTypeRes;   // Type of Result
	int nComp;      // Component
	T_THIS_K ThisK;

	T_THRS_D() { Initialize(); }
	T_THRS_D(T_THRS_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		NlnkK = 0;
		nTypeRes=0;
		nComp=0;
		ThisK=0;
	}
	T_THRS_D& operator=(T_THRS_D& src)
	{
		NlnkK = src.NlnkK;
		nTypeRes  = src.nTypeRes;
		nComp     = src.nComp;
		ThisK     = src.ThisK;
		return *this;
	}
	BOOL operator==(const T_THRS_D& src)
	{
		if(NlnkK     != src.NlnkK    ) return FALSE;
		if(nTypeRes  != src.nTypeRes ) return FALSE;
		if(nComp     != src.nComp    ) return FALSE;
		if(ThisK     != src.ThisK    ) return FALSE;
		return TRUE;
	}
};
struct T_THRS_UDRD_D
{
	T_THRS_K key;
	T_THRS_D data;
};
struct T_THRS_UDRD_D_RW
{
	T_THRS_K key;
	
	T_NLNK_K NlnkK;
	int nTypeRes;   // Type of Result
	int nComp;      // Component
	T_THIS_K ThisK;

	void GetThrs(T_THRS_K& rKey, T_THRS_D& rData)
	{
		rKey=key;
		rData.NlnkK = NlnkK;
		rData.nTypeRes=nTypeRes;
		rData.nComp=nComp;
		rData.ThisK=ThisK;
	}
	void SetThrs(const T_THRS_K Key, const T_THRS_D& rData)
	{
		key=Key;
		NlnkK = rData.NlnkK;
		nTypeRes = rData.nTypeRes;
		nComp = rData.nComp;
		ThisK = rData.ThisK;
	}
};
#define HASHSIZETHRS 101

// Time History Result Graph - Element Force Result
enum D_THEF_TYPE { D_THEF_TYPE_FORCE };
enum D_THEF_ELEM { D_THEF_ELEM_BEAM=0, D_THEF_ELEM_TRUSS, D_THEF_ELEM_WALL };
#define T_THEF_K UINT
struct T_THEF_D
{
	int nTypeElem;  // D_THEF_ELEM
	T_KEY PropK;    // nTypeElem==0,1 : T_ELEM_K
									// nTypeElem==2   : User Wall ID
	T_STOR_K StorK; // nTypeElem==D_THEF_ELEM_WALL일 때만 사용
	int nTypeRes;   // D_THEF_TYPE
	int nLocation;  // Location. 0:I-Node, 1:J-Node
	int nComp;      // Component 0:Fx, ..., 5:Mz
	T_THIS_K ThisK;

	T_THEF_D() { Initialize(); }
	T_THEF_D(T_THEF_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		nTypeElem=D_THEF_ELEM_BEAM;
		PropK=0;
		StorK=0;
		nTypeRes=D_THEF_TYPE_FORCE;
		nLocation=0;
		nComp=0;
		ThisK=0;
	}
	T_THEF_D& operator=(T_THEF_D& src)
	{
		nTypeElem = src.nTypeElem;
		PropK     = src.PropK    ;
		StorK     = src.StorK    ;
		nTypeRes  = src.nTypeRes;
		nLocation = src.nLocation;
		nComp     = src.nComp;
		ThisK     = src.ThisK;
		return *this;
	}
	BOOL operator==(const T_THEF_D& src)
	{
		if(nTypeElem != src.nTypeElem) return FALSE;
		if(PropK     != src.PropK    ) return FALSE;
		if(StorK     != src.StorK    ) return FALSE;
		if(nTypeRes  != src.nTypeRes ) return FALSE;
		if(nLocation != src.nLocation) return FALSE;
		if(nComp     != src.nComp    ) return FALSE;
		if(ThisK     != src.ThisK    ) return FALSE;
		return TRUE;
	}
};
struct T_THEF_UDRD_D
{
	T_THEF_K key;
	T_THEF_D data;
};
struct T_THEF_UDRD_D_RW
{
	T_THEF_K key;

	int nTypeElem;  // Type of Element
	T_KEY    PropK;
	T_STOR_K StorK;
	int nTypeRes;   // Type of Result
	int nLocation;  
	int nComp;      // Component
	T_THIS_K ThisK;

	void GetThef(T_THEF_K& rKey, T_THEF_D& rData)
	{
		rKey=key;
		rData.nTypeElem=nTypeElem;
		rData.PropK=PropK;
		rData.StorK=StorK;
		rData.nTypeRes=nTypeRes;
		rData.nLocation=nLocation;
		rData.nComp=nComp;
		rData.ThisK=ThisK;
	}
	void SetThef(const T_THEF_K Key, const T_THEF_D& rData)
	{
		key=Key;
		nTypeElem = rData.nTypeElem;
		PropK = rData.PropK;
		StorK = rData.StorK;
		nTypeRes = rData.nTypeRes;
		nLocation = rData.nLocation;
		nComp = rData.nComp;
		ThisK = rData.ThisK;
	}
};
#define HASHSIZETHEF 101

// Time History Global Control
#define T_THGC_K UINT
struct T_TGLC_BASE
{
	// T_THGC_D에서 사용
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
};

enum D_THGC_INC_OUTPUT { D_THGC_INC_OUTPUT_FINAL_STEP, D_THGC_INC_OUTPUT_USER_INPUT_STEP };
enum D_THGC_OUTPUT_OPT { D_THGC_OUTPUT_OPT_ALL, D_THGC_OUTPUT_OPT_SEL, D_THGC_OUTPUT_OPT_MAXMIN };
enum D_THGC_GEOMNONLIN { D_THGC_GEOMNONLIN_NONE, D_THGC_GEOMNONLIN_LARGE_DISP, D_THGC_GEOMNONLIN_P_DELTA };

struct T_THGC_D
{
	BOOL bEnergyResult;
	BOOL bSdvi        ; // Viscous damper / Oil damper result
	BOOL bSdve        ; // Viscoelastic result
	BOOL bSdst        ; // Steel damper result
	BOOL bSdhy        ; // Hysteretic isolator result
	BOOL bSdis        ; // Isolator result
	BOOL bMSSStatus   ; // MSS Model Yield Status
	int  nGeomNonlinearType; // 0:None, 1:Large Displacements, 2:P-Delta
	int  nInitLoadMethod;    // 0:Perform Nonlinear Static.., 1:Import Static Analysis/...
	CArray<T_TGLC_BASE, T_TGLC_BASE&> aInitLoad;
	int  bConsiderIgnoreElem;
	int  nIncrementStep;
	int  nIncrResult; // D_THGC_INC_OUTPUT
	int  nIncrResultUserInputStep;
	BOOL bPermitFail;
	int  nSubstep;
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	T_THIS_K ThisK_DummyInitLoad; // 초기하중 결과 출력용 하중번호
	BOOL bUseLineSearch;
	int nNumSearch;
	int  nHingeOutput; // D_THGC_OUTPUT_OPT_ALL
	BOOL bFiberSameHinge;
	int  nFiberOutput;
	BOOL bConvertWallStiffness; // v945 : GEN 7660 Wall Stiffness Scale Factor Convert
	int  nSeqLoadingDisplType; // 0:Undisplaced Position, 1:Displacement Position

	int  nPointSpringSupport; //MEC 에서 추가됨
	int  nElasticLink;
	T_NTIS_BOUNDARY BoundaryNL;

	T_THGC_D() { Initialize(); }
	T_THGC_D(T_THGC_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		bEnergyResult=TRUE;
		bSdvi        =TRUE;
		bSdve        =TRUE;
		bSdst        =TRUE;
		bSdhy        =TRUE;
		bSdis        =TRUE;
		bMSSStatus   =TRUE;
		nGeomNonlinearType       = D_THGC_GEOMNONLIN_NONE;
		nInitLoadMethod          = 0;
		aInitLoad.RemoveAll();
		bConsiderIgnoreElem      = TRUE;
		nIncrementStep           = 1;
		nIncrResult              = D_THGC_INC_OUTPUT_FINAL_STEP;
		nIncrResultUserInputStep = 1;
		bPermitFail              = TRUE;
		nSubstep                 = 10;
		nMaxIterNum              = 10;
		bDisplNorm               = TRUE;
		bForceNorm               = FALSE;
		bEnergyNorm              = FALSE;
		dDisplNorm               = 0.001;
		dForceNorm               = 0.001;
		dEnergyNorm              = 0.001;
		ThisK_DummyInitLoad      = 0;
		bUseLineSearch           = FALSE;
		nNumSearch               = 5;
		nHingeOutput             = D_THGC_OUTPUT_OPT_ALL;
		bFiberSameHinge			 = FALSE;
		nFiberOutput             = D_THGC_OUTPUT_OPT_ALL;
		bConvertWallStiffness    = TRUE;
		nSeqLoadingDisplType     = 1;
		nPointSpringSupport = 0;
		nElasticLink = 1;
		BoundaryNL.Initialize();
	}
	T_THGC_D& operator=(T_THGC_D& src)
	{
		bEnergyResult = src.bEnergyResult;
		bSdvi         = src.bSdvi        ;
		bSdve         = src.bSdve        ;
		bSdst         = src.bSdst        ;
		bSdhy         = src.bSdhy        ;
		bSdis         = src.bSdis        ;
		bMSSStatus    = src.bMSSStatus   ;
		nGeomNonlinearType       = src.nGeomNonlinearType      ;
		nInitLoadMethod          = src.nInitLoadMethod         ;
		aInitLoad.Copy(src.aInitLoad);
		bConsiderIgnoreElem      = src.bConsiderIgnoreElem     ;
		nIncrementStep           = src.nIncrementStep          ;
		nIncrResult              = src.nIncrResult             ;
		nIncrResultUserInputStep = src.nIncrResultUserInputStep;
		bPermitFail              = src.bPermitFail             ;
		nSubstep                 = src.nSubstep                ;
		nMaxIterNum              = src.nMaxIterNum             ;
		bDisplNorm               = src.bDisplNorm              ;
		bForceNorm               = src.bForceNorm              ;
		bEnergyNorm              = src.bEnergyNorm             ;
		dDisplNorm               = src.dDisplNorm              ;
		dForceNorm               = src.dForceNorm              ;
		dEnergyNorm              = src.dEnergyNorm             ;
		ThisK_DummyInitLoad      = src.ThisK_DummyInitLoad     ;
		bUseLineSearch           = src.bUseLineSearch          ;
		nNumSearch               = src.nNumSearch              ;
		nHingeOutput             = src.nHingeOutput             ;
		bFiberSameHinge			 = src.bFiberSameHinge			 ;
		nFiberOutput             = src.nFiberOutput             ;
		bConvertWallStiffness    = src.bConvertWallStiffness;
		nSeqLoadingDisplType     = src.nSeqLoadingDisplType;
		nPointSpringSupport		 = src.nPointSpringSupport;
		nElasticLink			 = src.nElasticLink;
		BoundaryNL				 = src.BoundaryNL;

		return *this;
	}
};

struct T_THGC_UDRD_D
{
	T_THGC_K key;
	T_THGC_D data;
};

struct T_THGC_UDRD_D_RW_880
{
	BOOL bEnergyResult;
	BOOL bSdvi        ; // Viscous damper / Oil damper result
	BOOL bSdve        ; // Viscoelastic result
	BOOL bSdst        ; // Steel damper result
	BOOL bSdhy        ; // Hysteretic isolator result
	BOOL bSdis        ; // Isolator result
	BOOL bMSSStatus   ; // MSS Model Yield Status
};

struct T_THGC_UDRD_D_RW_881
{
	BOOL bEnergyResult;
	BOOL bSdvi        ; // Viscous damper / Oil damper result
	BOOL bSdve        ; // Viscoelastic result
	BOOL bSdst        ; // Steel damper result
	BOOL bSdhy        ; // Hysteretic isolator result
	BOOL bSdis        ; // Isolator result
	BOOL bMSSStatus   ; // MSS Model Yield Status
	int  nGeomNonlinearType; // v881~
	int  nInitLoadMethod;
	int  nCountInitLoad; //CArray<T_TGLC_BASE, T_TGLC_BASE&> aInitLoad;
	int  bConsiderIgnoreElem;
	int  nIncrementStep;
	int  nIncrResult; // D_THGC_INC_OUTPUT
	int  nIncrResultUserInputStep;
	BOOL bPermitFail;
	int  nSubstep;
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	T_THIS_K ThisK_DummyInitLoad;
};

struct T_THGC_UDRD_D_RW_885
{
	BOOL bEnergyResult;
	BOOL bSdvi        ; // Viscous damper / Oil damper result
	BOOL bSdve        ; // Viscoelastic result
	BOOL bSdst        ; // Steel damper result
	BOOL bSdhy        ; // Hysteretic isolator result
	BOOL bSdis        ; // Isolator result
	BOOL bMSSStatus   ; // MSS Model Yield Status
	int  nGeomNonlinearType; // v881~
	int  nInitLoadMethod;
	int  nCountInitLoad; //CArray<T_TGLC_BASE, T_TGLC_BASE&> aInitLoad;
	int  bConsiderIgnoreElem;
	int  nIncrementStep;
	int  nIncrResult; // D_THGC_INC_OUTPUT
	int  nIncrResultUserInputStep;
	BOOL bPermitFail;
	int  nSubstep;
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	T_THIS_K ThisK_DummyInitLoad;
	BOOL bUseLineSearch;
	int nNumSearch;
};

struct T_THGC_UDRD_D_RW_940
{
	BOOL bEnergyResult;
	BOOL bSdvi; // Viscous damper / Oil damper result
	BOOL bSdve; // Viscoelastic result
	BOOL bSdst; // Steel damper result
	BOOL bSdhy; // Hysteretic isolator result
	BOOL bSdis; // Isolator result
	BOOL bMSSStatus; // MSS Model Yield Status
	int  nGeomNonlinearType; // v881~
	int  nInitLoadMethod;
	int  nCountInitLoad; //CArray<T_TGLC_BASE, T_TGLC_BASE&> aInitLoad;
	int  bConsiderIgnoreElem;
	int  nIncrementStep;
	int  nIncrResult; // D_THGC_INC_OUTPUT
	int  nIncrResultUserInputStep;
	BOOL bPermitFail;
	int  nSubstep;
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	T_THIS_K ThisK_DummyInitLoad;
	BOOL bUseLineSearch;
	int nNumSearch;
	int  nHingeOutput; // D_THGC_OUTPUT_OPT_ALL
	BOOL bFiberSameHinge;
	int  nFiberOutput;
};

struct T_THGC_UDRD_D_RW_945
{
	BOOL bEnergyResult;
	BOOL bSdvi; // Viscous damper / Oil damper result
	BOOL bSdve; // Viscoelastic result
	BOOL bSdst; // Steel damper result
	BOOL bSdhy; // Hysteretic isolator result
	BOOL bSdis; // Isolator result
	BOOL bMSSStatus; // MSS Model Yield Status
	int  nGeomNonlinearType; // v881~
	int  nInitLoadMethod;
	int  nCountInitLoad; //CArray<T_TGLC_BASE, T_TGLC_BASE&> aInitLoad;
	int  bConsiderIgnoreElem;
	int  nIncrementStep;
	int  nIncrResult; // D_THGC_INC_OUTPUT
	int  nIncrResultUserInputStep;
	BOOL bPermitFail;
	int  nSubstep;
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	T_THIS_K ThisK_DummyInitLoad;
	BOOL bUseLineSearch;
	int nNumSearch;
	int  nHingeOutput; // D_THGC_OUTPUT_OPT_ALL
	BOOL bFiberSameHinge;
	int  nFiberOutput;
	BOOL bConvertWallStiffness; // v945 : GEN 7660 Wall Stiffness Scale Factor Convert
};

struct T_THGC_UDRD_D_RW_960
{
	BOOL bEnergyResult;
	BOOL bSdvi; // Viscous damper / Oil damper result
	BOOL bSdve; // Viscoelastic result
	BOOL bSdst; // Steel damper result
	BOOL bSdhy; // Hysteretic isolator result
	BOOL bSdis; // Isolator result
	BOOL bMSSStatus; // MSS Model Yield Status
	int  nGeomNonlinearType; // v881~
	int  nInitLoadMethod;
	int  nCountInitLoad; //CArray<T_TGLC_BASE, T_TGLC_BASE&> aInitLoad;
	int  bConsiderIgnoreElem;
	int  nIncrementStep;
	int  nIncrResult; // D_THGC_INC_OUTPUT
	int  nIncrResultUserInputStep;
	BOOL bPermitFail;
	int  nSubstep;
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	T_THIS_K ThisK_DummyInitLoad;
	BOOL bUseLineSearch;
	int nNumSearch;
	int  nHingeOutput; // D_THGC_OUTPUT_OPT_ALL
	BOOL bFiberSameHinge;
	int  nFiberOutput;
	BOOL bConvertWallStiffness;
	int  nSeqLoadingDisplType;
};

struct T_THGC_UDRD_D_RW
{
	BOOL bEnergyResult;
	BOOL bSdvi        ; // Viscous damper / Oil damper result
	BOOL bSdve        ; // Viscoelastic result
	BOOL bSdst        ; // Steel damper result
	BOOL bSdhy        ; // Hysteretic isolator result
	BOOL bSdis        ; // Isolator result
	BOOL bMSSStatus   ; // MSS Model Yield Status
	int  nGeomNonlinearType; // v881~
	int  nInitLoadMethod;
	int  nCountInitLoad; //CArray<T_TGLC_BASE, T_TGLC_BASE&> aInitLoad;
	int  bConsiderIgnoreElem;
	int  nIncrementStep;
	int  nIncrResult; // D_THGC_INC_OUTPUT
	int  nIncrResultUserInputStep;
	BOOL bPermitFail;
	int  nSubstep;
	int  nMaxIterNum;
	BOOL bDisplNorm;
	BOOL bForceNorm;
	BOOL bEnergyNorm;
	double dDisplNorm;
	double dForceNorm;
	double dEnergyNorm;
	T_THIS_K ThisK_DummyInitLoad;
	BOOL bUseLineSearch;
	int nNumSearch;
	int  nHingeOutput; // D_THGC_OUTPUT_OPT_ALL
	BOOL bFiberSameHinge;
	int  nFiberOutput;
	BOOL bConvertWallStiffness;
	int  nSeqLoadingDisplType;
	int  nPointSpringSupport;
	int  nElasticLink;
	T_NTIS_BOUNDARY BoundaryNL;

	void Convert880(T_THGC_UDRD_D_RW_880& rData)
	{
		bEnergyResult            = rData.bEnergyResult;
		bSdvi                    = rData.bSdvi        ;
		bSdve                    = rData.bSdve        ;
		bSdst                    = rData.bSdst        ;
		bSdhy                    = rData.bSdhy        ;
		bSdis                    = rData.bSdis        ;
		bMSSStatus               = rData.bMSSStatus   ;
		nGeomNonlinearType       = 0;
		nInitLoadMethod          = 0;
		nCountInitLoad           = 0;
		bConsiderIgnoreElem      = TRUE;
		nIncrementStep           = 1;
		nIncrResult              = D_THGC_INC_OUTPUT_FINAL_STEP;
		nIncrResultUserInputStep = 1;
		bPermitFail              = TRUE;
		nSubstep                 = 10;
		nMaxIterNum              = 10;
		bDisplNorm               = TRUE;
		bForceNorm               = FALSE;
		bEnergyNorm              = FALSE;
		dDisplNorm               = 0.001;
		dForceNorm               = 0.001;
		dEnergyNorm              = 0.001;
		ThisK_DummyInitLoad      = 0;
		bUseLineSearch           = FALSE;
		nNumSearch               = 5;
		nHingeOutput             = D_THGC_OUTPUT_OPT_ALL;
		bFiberSameHinge			 = FALSE;
		nFiberOutput             = D_THGC_OUTPUT_OPT_ALL;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif
		nSeqLoadingDisplType    = 0;
		nPointSpringSupport = 0;
		nElasticLink = 1;
		BoundaryNL.Initialize();
	}
	void Convert881(T_THGC_UDRD_D_RW_881& rData)
	{
		bEnergyResult            = rData.bEnergyResult           ;
		bSdvi                    = rData.bSdvi                   ;
		bSdve                    = rData.bSdve                   ;
		bSdst                    = rData.bSdst                   ;
		bSdhy                    = rData.bSdhy                   ;
		bSdis                    = rData.bSdis                   ;
		bMSSStatus               = rData.bMSSStatus              ;
		nGeomNonlinearType       = rData.nGeomNonlinearType      ;
		nInitLoadMethod          = rData.nInitLoadMethod         ;
		nCountInitLoad           = rData.nCountInitLoad          ;
		bConsiderIgnoreElem      = rData.bConsiderIgnoreElem     ;
		nIncrementStep           = rData.nIncrementStep          ;
		nIncrResult              = rData.nIncrResult             ;
		nIncrResultUserInputStep = rData.nIncrResultUserInputStep;
		bPermitFail              = rData.bPermitFail             ;
		nSubstep                 = rData.nSubstep                ;
		nMaxIterNum              = rData.nMaxIterNum             ;
		bDisplNorm               = rData.bDisplNorm              ;
		bForceNorm               = rData.bForceNorm              ;
		bEnergyNorm              = rData.bEnergyNorm             ;
		dDisplNorm               = rData.dDisplNorm              ;
		dForceNorm               = rData.dForceNorm              ;
		dEnergyNorm              = rData.dEnergyNorm             ;
		ThisK_DummyInitLoad      = rData.ThisK_DummyInitLoad     ;
		bUseLineSearch           = FALSE;
		nNumSearch               = 5;
		nHingeOutput             = D_THGC_OUTPUT_OPT_ALL;
		bFiberSameHinge			 = FALSE;
		nFiberOutput             = D_THGC_OUTPUT_OPT_ALL;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif
		nSeqLoadingDisplType    = 0;
		nPointSpringSupport = 0;
		nElasticLink = 1;
		BoundaryNL.Initialize();
	}
	void Convert885(T_THGC_UDRD_D_RW_885& rData)
	{
		bEnergyResult            = rData.bEnergyResult           ;
		bSdvi                    = rData.bSdvi                   ;
		bSdve                    = rData.bSdve                   ;
		bSdst                    = rData.bSdst                   ;
		bSdhy                    = rData.bSdhy                   ;
		bSdis                    = rData.bSdis                   ;
		bMSSStatus               = rData.bMSSStatus              ;
		nGeomNonlinearType       = rData.nGeomNonlinearType      ;
		nInitLoadMethod          = rData.nInitLoadMethod         ;
		nCountInitLoad           = rData.nCountInitLoad          ;
		bConsiderIgnoreElem      = rData.bConsiderIgnoreElem     ;
		nIncrementStep           = rData.nIncrementStep          ;
		nIncrResult              = rData.nIncrResult             ;
		nIncrResultUserInputStep = rData.nIncrResultUserInputStep;
		bPermitFail              = rData.bPermitFail             ;
		nSubstep                 = rData.nSubstep                ;
		nMaxIterNum              = rData.nMaxIterNum             ;
		bDisplNorm               = rData.bDisplNorm              ;
		bForceNorm               = rData.bForceNorm              ;
		bEnergyNorm              = rData.bEnergyNorm             ;
		dDisplNorm               = rData.dDisplNorm              ;
		dForceNorm               = rData.dForceNorm              ;
		dEnergyNorm              = rData.dEnergyNorm             ;
		ThisK_DummyInitLoad      = rData.ThisK_DummyInitLoad     ;
		bUseLineSearch           = rData.bUseLineSearch			 ;
		nNumSearch               = rData.nNumSearch     		 ;
		nHingeOutput             = D_THGC_OUTPUT_OPT_ALL;
		bFiberSameHinge			 = FALSE;
		nFiberOutput             = D_THGC_OUTPUT_OPT_ALL;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif
		nSeqLoadingDisplType    = 0;
		nPointSpringSupport = 0;
		nElasticLink = 1;
		BoundaryNL.Initialize();
	}
	void Convert940(T_THGC_UDRD_D_RW_940& rData)
	{
		bEnergyResult = rData.bEnergyResult;
		bSdvi = rData.bSdvi;
		bSdve = rData.bSdve;
		bSdst = rData.bSdst;
		bSdhy = rData.bSdhy;
		bSdis = rData.bSdis;
		bMSSStatus = rData.bMSSStatus;
		nGeomNonlinearType = rData.nGeomNonlinearType;
		nInitLoadMethod = rData.nInitLoadMethod;
		nCountInitLoad = rData.nCountInitLoad;
		bConsiderIgnoreElem = rData.bConsiderIgnoreElem;
		nIncrementStep = rData.nIncrementStep;
		nIncrResult = rData.nIncrResult;
		nIncrResultUserInputStep = rData.nIncrResultUserInputStep;
		bPermitFail = rData.bPermitFail;
		nSubstep = rData.nSubstep;
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm = rData.bDisplNorm;
		bForceNorm = rData.bForceNorm;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm = rData.dDisplNorm;
		dForceNorm = rData.dForceNorm;
		dEnergyNorm = rData.dEnergyNorm;
		ThisK_DummyInitLoad = rData.ThisK_DummyInitLoad;
		bUseLineSearch = rData.bUseLineSearch;
		nNumSearch = rData.nNumSearch;
		nHingeOutput = rData.nHingeOutput;
		bFiberSameHinge = rData.bFiberSameHinge;
		nFiberOutput = rData.nFiberOutput;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif
		nSeqLoadingDisplType = 0;
		nPointSpringSupport = 0;
		nElasticLink = 1;
		BoundaryNL.Initialize();
	}

	void Convert945(T_THGC_UDRD_D_RW_945& rData)
	{
		bEnergyResult = rData.bEnergyResult;
		bSdvi = rData.bSdvi;
		bSdve = rData.bSdve;
		bSdst = rData.bSdst;
		bSdhy = rData.bSdhy;
		bSdis = rData.bSdis;
		bMSSStatus = rData.bMSSStatus;
		nGeomNonlinearType = rData.nGeomNonlinearType;
		nInitLoadMethod = rData.nInitLoadMethod;
		nCountInitLoad = rData.nCountInitLoad;
		bConsiderIgnoreElem = rData.bConsiderIgnoreElem;
		nIncrementStep = rData.nIncrementStep;
		nIncrResult = rData.nIncrResult;
		nIncrResultUserInputStep = rData.nIncrResultUserInputStep;
		bPermitFail = rData.bPermitFail;
		nSubstep = rData.nSubstep;
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm = rData.bDisplNorm;
		bForceNorm = rData.bForceNorm;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm = rData.dDisplNorm;
		dForceNorm = rData.dForceNorm;
		dEnergyNorm = rData.dEnergyNorm;
		ThisK_DummyInitLoad = rData.ThisK_DummyInitLoad;
		bUseLineSearch = rData.bUseLineSearch;
		nNumSearch = rData.nNumSearch;
		nHingeOutput = rData.nHingeOutput;
		bFiberSameHinge = rData.bFiberSameHinge;
		nFiberOutput = rData.nFiberOutput;
#if defined(_MGEN_ORG) // Set to Check off when opening previous version in KR version
		bConvertWallStiffness = FALSE;
#else
		bConvertWallStiffness = TRUE;
#endif
		nSeqLoadingDisplType = 0;
		nPointSpringSupport = 0;
		nElasticLink = 1;
		BoundaryNL.Initialize();
	}

	void Convert960(T_THGC_UDRD_D_RW_960& rData)
	{
		bEnergyResult = rData.bEnergyResult;
		bSdvi = rData.bSdvi;
		bSdve = rData.bSdve;
		bSdst = rData.bSdst;
		bSdhy = rData.bSdhy;
		bSdis = rData.bSdis;
		bMSSStatus = rData.bMSSStatus;
		nGeomNonlinearType = rData.nGeomNonlinearType;
		nInitLoadMethod = rData.nInitLoadMethod;
		nCountInitLoad = rData.nCountInitLoad;
		bConsiderIgnoreElem = rData.bConsiderIgnoreElem;
		nIncrementStep = rData.nIncrementStep;
		nIncrResult = rData.nIncrResult;
		nIncrResultUserInputStep = rData.nIncrResultUserInputStep;
		bPermitFail = rData.bPermitFail;
		nSubstep = rData.nSubstep;
		nMaxIterNum = rData.nMaxIterNum;
		bDisplNorm = rData.bDisplNorm;
		bForceNorm = rData.bForceNorm;
		bEnergyNorm = rData.bEnergyNorm;
		dDisplNorm = rData.dDisplNorm;
		dForceNorm = rData.dForceNorm;
		dEnergyNorm = rData.dEnergyNorm;
		ThisK_DummyInitLoad = rData.ThisK_DummyInitLoad;
		bUseLineSearch = rData.bUseLineSearch;
		nNumSearch = rData.nNumSearch;
		nHingeOutput = rData.nHingeOutput;
		bFiberSameHinge = rData.bFiberSameHinge;
		nFiberOutput = rData.nFiberOutput;
		bConvertWallStiffness = rData.bConvertWallStiffness;
		nSeqLoadingDisplType = rData.nSeqLoadingDisplType;

		nPointSpringSupport = 0;
		nElasticLink = 1;
		BoundaryNL.Initialize();
	}
	void GetThgc(T_THGC_D& rData)
	{
		rData.bEnergyResult            = bEnergyResult           ;
		rData.bSdvi                    = bSdvi                   ;
		rData.bSdve                    = bSdve                   ;
		rData.bSdst                    = bSdst                   ;
		rData.bSdhy                    = bSdhy                   ;
		rData.bSdis                    = bSdis                   ;
		rData.bMSSStatus               = bMSSStatus              ;
		rData.nGeomNonlinearType       = nGeomNonlinearType      ;
		rData.nInitLoadMethod          = nInitLoadMethod         ;
		rData.aInitLoad.RemoveAll(); // 따로 입력
		rData.bConsiderIgnoreElem      = bConsiderIgnoreElem     ;
		rData.nIncrementStep           = nIncrementStep          ;
		rData.nIncrResult              = nIncrResult             ;
		rData.nIncrResultUserInputStep = nIncrResultUserInputStep;
		rData.bPermitFail              = bPermitFail             ;
		rData.nSubstep                 = nSubstep                ;
		rData.nMaxIterNum              = nMaxIterNum             ;
		rData.bDisplNorm               = bDisplNorm              ;
		rData.bForceNorm               = bForceNorm              ;
		rData.bEnergyNorm              = bEnergyNorm             ;
		rData.dDisplNorm               = dDisplNorm              ;
		rData.dForceNorm               = dForceNorm              ;
		rData.dEnergyNorm              = dEnergyNorm             ;
		rData.ThisK_DummyInitLoad      = ThisK_DummyInitLoad     ;
		rData.bUseLineSearch           = bUseLineSearch          ;
		rData.nNumSearch               = nNumSearch              ;
		rData.nHingeOutput             = nHingeOutput            ;
		rData.bFiberSameHinge		   = bFiberSameHinge		 ;
		rData.nFiberOutput             = nFiberOutput            ;
		rData.bConvertWallStiffness    = bConvertWallStiffness;
		rData.nSeqLoadingDisplType     = nSeqLoadingDisplType;
		rData.nPointSpringSupport		= nPointSpringSupport;
		rData.nElasticLink				= nElasticLink;
		rData.BoundaryNL				= BoundaryNL;
	}
	void SetThgc(T_THGC_D& rData)
	{
		bEnergyResult            = rData.bEnergyResult           ;
		bSdvi                    = rData.bSdvi                   ;
		bSdve                    = rData.bSdve                   ;
		bSdst                    = rData.bSdst                   ;
		bSdhy                    = rData.bSdhy                   ;
		bSdis                    = rData.bSdis                   ;
		bMSSStatus               = rData.bMSSStatus              ;
		nGeomNonlinearType       = rData.nGeomNonlinearType      ;
		nInitLoadMethod          = rData.nInitLoadMethod         ;
		nCountInitLoad           = (int)rData.aInitLoad.GetSize();
		bConsiderIgnoreElem      = rData.bConsiderIgnoreElem     ;
		nIncrementStep           = rData.nIncrementStep          ;
		nIncrResult              = rData.nIncrResult             ;
		nIncrResultUserInputStep = rData.nIncrResultUserInputStep;
		bPermitFail              = rData.bPermitFail             ;
		nSubstep                 = rData.nSubstep                ;
		nMaxIterNum              = rData.nMaxIterNum             ;
		bDisplNorm               = rData.bDisplNorm              ;
		bForceNorm               = rData.bForceNorm              ;
		bEnergyNorm              = rData.bEnergyNorm             ;
		dDisplNorm               = rData.dDisplNorm              ;
		dForceNorm               = rData.dForceNorm              ;
		dEnergyNorm              = rData.dEnergyNorm             ;
		ThisK_DummyInitLoad      = rData.ThisK_DummyInitLoad     ;
		bUseLineSearch           = rData.bUseLineSearch          ;
		nNumSearch               = rData.nNumSearch              ;
		nHingeOutput             = rData.nHingeOutput            ;
		bFiberSameHinge		     = rData.bFiberSameHinge		 ;
		nFiberOutput             = rData.nFiberOutput            ;
		bConvertWallStiffness    = rData.bConvertWallStiffness   ;
		nSeqLoadingDisplType     = rData.nSeqLoadingDisplType    ;
		nPointSpringSupport		 = rData.nPointSpringSupport	 ;
		nElasticLink			 = rData.nElasticLink			 ;
		BoundaryNL				 = rData.BoundaryNL;
	}
};

#define HASHSIZETHGC 1

// Time History Load Combination
enum D_THLC_TYPE { D_THLC_TYPE_ENV,D_THLC_TYPE_AVG };
#define T_THLC_K unsigned int
struct T_THLC_D
{
	CString strName;
	CString strDesc;
	int     nType  ; // D_THLC_TYPE
	CArray<T_THIS_K,T_THIS_K> aThisK;

	void Initialize()
	{
		strName.Empty();
		strDesc.Empty();
		nType=D_THLC_TYPE_ENV;
		aThisK.RemoveAll();
	}
	T_THLC_D()
	{
		Initialize();
	}
	T_THLC_D(T_THLC_D& src)
	{
		*this = src;
	}
	T_THLC_D& operator=(T_THLC_D& src)
	{
		strName=src.strName;
		strDesc=src.strDesc;
		nType=src.nType;
		aThisK.Copy(src.aThisK);
		return *this;
	}
};
struct T_THLC_D_CH
{
	char strName[20];
	char strDesc[80];
	int  nType;
	CArray<T_THIS_K,T_THIS_K> aThisK;
	void ConvertToString(T_THLC_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.nType=nType;
		rData.aThisK.Copy(aThisK);
	}
	void ConvertToChar(T_THLC_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDesc, strDesc, sizeof(strDesc));
		nType=rData.nType;
		aThisK.Copy(rData.aThisK);
	}
};
struct T_THLC_UDRD_D
{
	T_THLC_K key;
	T_THLC_D data;
};
struct T_THLC_UDRD_D_CH
{
	T_THLC_K key;
	T_THLC_D_CH data;
};

struct T_THLC_UDRD_D_CH_RW
{
	T_THLC_K key;
	char strName[20];
	char strDesc[80];
	int  nType;
	int  nCountThis;
	void GetThlc(T_THLC_K& rKey, T_THLC_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strDesc, strDesc, sizeof(strDesc));
		rData.nType=nType;
	}
	void SetThlc(const T_THLC_K Key, const T_THLC_D_CH& rData)
	{
		key=Key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDesc, rData.strDesc, sizeof(strDesc));
		nType=rData.nType;
		nCountThis=(int)rData.aThisK.GetSize();
	}
};
#define HASHSIZETHLC 101

// Import Time History Load Combination
// AIK-G-001-2021 에서 추가된 Json File Import 기능으로 조합된 Time History Lcom Data
enum D_THJI_TYPE { D_THJI_TYPE_AVG, D_THJI_TYPE_ENV };
enum D_THJI_OPTION {
	D_THJI_OPTION_ENV, D_THJI_OPTION_AVG, D_THJI_OPTION_SELECT_LCOM, D_THJI_OPTION_DUMMY
};
#define T_THJI_K unsigned int
const int THJI_NameSize_935 = 20;
const int THJI_DescSize = 80;
const int THJI_PathSize_935 = 200;
const int THJI_PathCount_935 = 30;
// 출시 전 사이즈 변경 (이미 테스트로 출시된 모델이 많아서 버전처리 추가)
const int THJI_NameSize = 80;
const int THJI_PathSize = 400;
const int THJI_PathCount = 40;

struct T_THJI_D // Time History Json Import
{
	CString strName;
	CString strDesc;
	int nType; // Envelope, Average
	CArray<CString, CString&> aJsonFilePath;
	int nOption;
	int nSelectIndex;

	void Initialize()
	{
		strName.Empty();
		strDesc.Empty();
		nType = D_THJI_TYPE_AVG;
		aJsonFilePath.RemoveAll();
		nOption = D_THJI_OPTION_SELECT_LCOM;
		nSelectIndex = 0;
	}
	T_THJI_D()
	{
		Initialize();
	}
	T_THJI_D(T_THJI_D& src)
	{
		*this = src;
	}
	T_THJI_D& operator=(T_THJI_D& src)
	{
		strName=src.strName;
		strDesc=src.strDesc;
		nType=src.nType;
		aJsonFilePath.Copy(src.aJsonFilePath);
		nOption=src.nOption;
		nSelectIndex=src.nSelectIndex;

		return *this;
	}
};
struct T_THJI_D_CH_935
{
	char strName[THJI_NameSize_935];
	char strDesc[THJI_DescSize];
	int  nType;
	int nJsonFileCount;
	char aJsonFilePath[THJI_PathCount_935][THJI_PathSize_935]; // 30개 제한
	int nOption;
	int nSelectIndex;
};
struct T_THJI_D_CH
{
	char strName[THJI_NameSize];
	char strDesc[THJI_DescSize];
	int  nType;
	int nJsonFileCount;
	char aJsonFilePath[THJI_PathCount][THJI_PathSize]; // 30개 제한
	int nOption;
	int nSelectIndex;

	void Initialize()
	{
		memset(strName, 0, sizeof(strName));
		memset(strDesc, 0, sizeof(strDesc));
		nType = D_THJI_TYPE_ENV;
		nJsonFileCount = 0;
		memset(aJsonFilePath, 0, sizeof(aJsonFilePath));
		nOption = D_THJI_OPTION_DUMMY;
		nSelectIndex = 0;
	}
	void ConvertToString(T_THJI_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDesc, rData.strDesc, sizeof(strDesc));
		rData.nType=nType;
		rData.aJsonFilePath.SetSize(nJsonFileCount);
		for ( int i = 0; i < nJsonFileCount; ++i )
		{
			ConvertCharStr(aJsonFilePath[i], rData.aJsonFilePath[i], sizeof(aJsonFilePath[i]));
		}
		rData.nOption=nOption;
		rData.nSelectIndex=nSelectIndex;
	}
	void ConvertToChar(T_THJI_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDesc, strDesc, sizeof(strDesc));
		nType=rData.nType;
		nJsonFileCount = rData.aJsonFilePath.GetCount();
		for ( int i = 0; i < nJsonFileCount; ++i )
		{
			ConvertStrChar(rData.aJsonFilePath[i], aJsonFilePath[i], sizeof(aJsonFilePath[i]));
		}
		nOption=rData.nOption;
		nSelectIndex=rData.nSelectIndex;
	}
	void Convert935(T_THJI_D_CH_935& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDesc, rData.strDesc, sizeof(strDesc));
		nType = rData.nType;
		nJsonFileCount = rData.nJsonFileCount;
		for ( int i = 0; i < nJsonFileCount; ++i )
		{
			memcpy(aJsonFilePath[i], rData.aJsonFilePath[i], sizeof(rData.aJsonFilePath[i]));
		}
		nOption = rData.nOption;
		nSelectIndex = rData.nSelectIndex;
	}
};
struct T_THJI_UDRD_D
{
	T_THJI_K key;
	T_THJI_D data;
};
struct T_THJI_UDRD_D_CH
{
	T_THJI_K key;
	T_THJI_D_CH data;
};
struct T_THJI_UDRD_D_CH_RW
{
	T_THJI_K key;
	T_THJI_D_CH data;

	void GetThji(T_THJI_K& rKey, T_THJI_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strName, data.strName, sizeof(data.strName));
		memcpy(rData.strDesc, data.strDesc, sizeof(data.strDesc));
		rData.nType = data.nType;
		rData.nJsonFileCount = data.nJsonFileCount;
		for ( int i = 0; i < rData.nJsonFileCount; ++i )
		{
			memcpy(rData.aJsonFilePath[i], data.aJsonFilePath[i], sizeof(data.aJsonFilePath[i]));
		}
		rData.nOption = data.nOption;
		rData.nSelectIndex = data.nSelectIndex;
	}
	void SetThji(const T_THJI_K Key, const T_THJI_D_CH& rData)
	{
		key=Key;
		memcpy(data.strName, rData.strName, sizeof(data.strName));
		memcpy(data.strDesc, rData.strDesc, sizeof(data.strDesc));
		data.nType=rData.nType;
		data.nJsonFileCount = rData.nJsonFileCount;
		for ( int i = 0; i < rData.nJsonFileCount; ++i )
		{
			memcpy(data.aJsonFilePath[i], rData.aJsonFilePath[i], sizeof(data.aJsonFilePath[i]));
		}
		data.nOption = rData.nOption;
		data.nSelectIndex = rData.nSelectIndex;
	}
};
struct T_THJI_UDRD_D_CH_RW_935
{
	T_THRD_K key;
	T_THJI_D_CH_935 data;
};
#define HASHSIZETHJI 11

struct T_NTIS_BASE
{
	int nModeId;
	double dDamping;
	void Initialize()
	{
		nModeId = 1;
		dDamping = 0.;
	}
};
#define T_NTIS_DT CArray<T_NTIS_BASE, T_NTIS_BASE&>
struct T_NTIS_PROPDAMPING
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

struct T_NTIS_GRUPDAMPING
{
	int    nRefFrequency;  // 0=Frequency, 1=Period, 2=Mode Index from Modal Analysis
	double dFrequency;
	double dPeriod;
	int    nModeIndex;
	void Initialize()
	{
		nRefFrequency = 1;
		dFrequency = 1.0;
		dPeriod = 1.0;
		nModeIndex = 1;
	}
};

struct T_NTIS_STATIC
{
	int    nCtrlMethod;     // 0=Load Control, 1=Displacement Control
	int    nCtrlOption;     // 0=Global Control, 1=Master Node Control
	double dTotalTransDisp;
	UINT   MasterNode;
	int    nMasterDir;      // 1=DX, 2=DY, 3=DZ
	double dTotalIncDisp;
	BOOL   bCumulateHistory;
	double dScaleFactor;       // 6.5.0 추가
	void Initialize()
	{
		nCtrlMethod = 0;
		nCtrlOption = 0;
		dTotalTransDisp = 0.;
		MasterNode = 0;
		nMasterDir = 1;
		dTotalIncDisp = 0.;
		bCumulateHistory = FALSE;
		dScaleFactor = 1.0;
	}
};

// Nonlinear Time History Result Output
#define T_THRO_K UINT

struct T_THRO_D
{
	int  nHingeOutput; // 0 : ALL, 1: Select
	BOOL bFiberSameHinge;
	int  nFiberOutput;

	BOOL bEnergyResult;
	BOOL bSdvi; // Viscous damper / Oil damper result
	BOOL bSdve; // Viscoelastic result
	BOOL bSdst; // Steel damper result
	BOOL bSdhy; // Hysteretic isolator result
	BOOL bSdis; // Isolator result

	T_THRO_D() { Initialize(); }
	T_THRO_D(T_THRO_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		bEnergyResult = TRUE;
		bSdvi = TRUE;
		bSdve = TRUE;
		bSdst = TRUE;
		bSdhy = TRUE;
		bSdis = TRUE;

		nHingeOutput = D_THGC_OUTPUT_OPT_ALL;
		bFiberSameHinge = FALSE;
		nFiberOutput = D_THGC_OUTPUT_OPT_ALL;

	}
	T_THRO_D& operator=(T_THRO_D& src)
	{
		// 복사 생성자 할당
		nHingeOutput = src.nHingeOutput;
		bFiberSameHinge = src.bFiberSameHinge;
		nFiberOutput = src.nFiberOutput;

		bEnergyResult = src.bEnergyResult;
		bSdvi = src.bSdvi;
		bSdve = src.bSdve;
		bSdst = src.bSdst;
		bSdhy = src.bSdhy;
		bSdis = src.bSdis;

		return *this;
	}
};

struct T_THRO_UDRD_D
{
	T_THRO_K key;
	T_THRO_D data;
};

#define HASHSIZETHRO 1
#pragma pack(pop)

#endif  // __DB_ST_DT_TIME_H__
