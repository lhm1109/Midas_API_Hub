#ifndef __DB_ST_DT_NONL_H__
#define __DB_ST_DT_NONL_H__

#define MIDAS_SDK

#pragma pack(push, 1)


// None linear analysis control
#define T_NLCT_K unsigned int

struct T_NLCT_VALUE_BASE
{  
	double dValue;
	void Initialize()
	{ 
		dValue = 0;    
	}
};
struct T_NLCT_NEWTON   // 590추가,  if nIterationMethod == 0
{
	T_STLD_K StldKey;
	int nLoadStep;            
	int nMaxIterPerLoadStep;  
	CArray<T_NLCT_VALUE_BASE, T_NLCT_VALUE_BASE&> arLoadFactor;
	void Initialize()
	{
		StldKey = 0;
		nLoadStep = 1;
		nMaxIterPerLoadStep = 30;
		arLoadFactor.RemoveAll();
	}
	T_NLCT_NEWTON() {Initialize();}
	T_NLCT_NEWTON(const T_NLCT_NEWTON& src) { *this = src;}
	T_NLCT_NEWTON& operator=(const T_NLCT_NEWTON& rData)
	{
		StldKey = rData.StldKey;
		nLoadStep = rData.nLoadStep;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		arLoadFactor.Copy(rData.arLoadFactor);
		return *this;
	}
};
struct T_NLCT_ARCLEN_690
{
	T_STLD_K StldKey;
	int nMaxIncStep;          
	int nMaxIterIncStep;      
	int nIniForceRatio;       //  (단위:%)
	double dMaxDisplacement;  
};
struct T_NLCT_ARCLEN   // 590추가,  if nIterationMethod == 1
{
	T_STLD_K StldKey;
	int nMaxIncStep;          
	int nMaxIterIncStep;      
	double dIniForceRatio;       //  (단위:%) 6.9.1에서 double로 변경
	double dMaxDisplacement;  
	void Initialize()
	{
		StldKey          = 0;    
		nMaxIncStep      = 100;
		nMaxIterIncStep  = 10;
		dIniForceRatio   = 5.0;
		dMaxDisplacement = 0.0;    
	}  
	void Convert690(T_NLCT_ARCLEN_690& rData)
	{
		StldKey = rData.StldKey;
		nMaxIncStep = rData.nMaxIncStep;
		nMaxIterIncStep = rData.nMaxIterIncStep;
		dIniForceRatio = (double)rData.nIniForceRatio;
		dMaxDisplacement = rData.dMaxDisplacement;
	}
};
struct T_NLCT_DISPCT   // 590추가,  if nIterationMethod == 2
{
	T_STLD_K StldKey;
	int nMumDispStep;         
	int nMaxIterIncStep;      
	T_NODE_K MasterNodeKey;   
	int nDirection;           // 0 = DX, 1 = DY, 2 = DZ 
	double dMaxDisplacement;  
	CArray<T_NLCT_VALUE_BASE, T_NLCT_VALUE_BASE&> arNodeDisp;
	void Initialize()
	{
		StldKey          = 0;
		nMumDispStep     = 1;       
		nMaxIterIncStep  = 10;  
		MasterNodeKey    = 0;      
		nDirection       = 0;           
		dMaxDisplacement = 0.0;      
		arNodeDisp.RemoveAll();
	}
	T_NLCT_DISPCT() {Initialize();}
	T_NLCT_DISPCT(const T_NLCT_DISPCT& src) { *this = src;}
	T_NLCT_DISPCT& operator=(const T_NLCT_DISPCT& rData)
	{
		StldKey          = rData.StldKey;
		nMumDispStep     = rData.nMumDispStep;       
		nMaxIterIncStep  = rData.nMaxIterIncStep;  
		MasterNodeKey    = rData.MasterNodeKey;      
		nDirection       = rData.nDirection;           
		dMaxDisplacement = rData.dMaxDisplacement;      
		arNodeDisp.Copy(rData.arNodeDisp);
		return *this;
	}
};
struct T_NLCT_LOADSTEP
{
	UINT nStep;
	double dLoadIncFactor;
	BOOL bSave;

	void Initialize()
	{
		nStep = 1;
		dLoadIncFactor = 1.0;
		bSave = FALSE;
	}

	T_NLCT_LOADSTEP() {}
	T_NLCT_LOADSTEP(const T_NLCT_LOADSTEP& src) { *this = src; }
	T_NLCT_LOADSTEP& operator=(const T_NLCT_LOADSTEP& rData)
	{
		nStep = rData.nStep;
		dLoadIncFactor = rData.dLoadIncFactor;
		bSave = rData.bSave;

		return *this;
	}
};
namespace NLCT_NLCTRL_PARM
{
	enum LineSearch
	{
		Auto = 1,  // Do not change number
		UserDefined
	};
}
struct T_NLCT_NLCTRL_PARM
{
	BOOL bUseDefaultNLParm;
	int	 nStiffUpdateScheme;	   // 0:Custom, 1:Full Newton-Raphson, 2:Initial Stiffness, 3:Modified Newton-Raphson, 4:Quasi-Newton(Search)
	int	 nCustomUpdateMethod;	   // 0:Automatic(AUTO), 1:iteration(ITER), 2:Semi-Automatic(SEMI)
	int	 nIterStiffUpdate;
	int	 nMaxQuasiNewtonVec;
	BOOL bTerminateAnalOnFailConv;
	int	 nMaxIterPerIncre;
	int	 nMaxBisectLevel;
	BOOL bEnableLineSearch;
	int	 nMaxLineSearch;
	double dLineSearchTol;
	int	nDiverThreshold;
	BOOL bSmartBisection;
	int nLineSearchOption;

	void Initialize()
	{
		bUseDefaultNLParm = TRUE;
		nStiffUpdateScheme = 1;
		nCustomUpdateMethod = 1;
		nIterStiffUpdate = 1;
		nMaxQuasiNewtonVec = 0;
		bTerminateAnalOnFailConv = FALSE;
		nMaxIterPerIncre = 50;
		nMaxBisectLevel = 5;
		bEnableLineSearch = TRUE;
		nMaxLineSearch = 4;
		dLineSearchTol = 0.5;
		nDiverThreshold = 3;
		bSmartBisection = FALSE;
		nLineSearchOption = NLCT_NLCTRL_PARM::Auto;
	}

	T_NLCT_NLCTRL_PARM() {}
	T_NLCT_NLCTRL_PARM(const T_NLCT_NLCTRL_PARM& src) { *this = src; }
	T_NLCT_NLCTRL_PARM& operator=(const T_NLCT_NLCTRL_PARM& rData)
	{
		bUseDefaultNLParm = rData.bUseDefaultNLParm;
		nStiffUpdateScheme = rData.nStiffUpdateScheme;
		nCustomUpdateMethod = rData.nCustomUpdateMethod;
		nIterStiffUpdate = rData.nIterStiffUpdate;
		nMaxQuasiNewtonVec = rData.nMaxQuasiNewtonVec;
		bTerminateAnalOnFailConv = rData.bTerminateAnalOnFailConv;
		nMaxIterPerIncre = rData.nMaxIterPerIncre;
		nMaxBisectLevel = rData.nMaxBisectLevel;
		bEnableLineSearch = rData.bEnableLineSearch;
		nMaxLineSearch = rData.nMaxLineSearch;
		dLineSearchTol = rData.dLineSearchTol;
		nDiverThreshold = rData.nDiverThreshold;
		bSmartBisection = rData.bSmartBisection;
		nLineSearchOption = rData.nLineSearchOption;

		return *this;
	}
};
struct T_NLCT_D_580 // MgtOldVerParse.cpp에서 사용.
{
	int nIterationMethod;     // 0= Newton-Raphson 1=Arc-Length
	int nLoadStep;            // if nIterationMethod == 0
	int nMaxIterPerLoadStep;  // if nIterationMethod == 0
	int nIniForceRatio;       // if nIterationMethod == 1 (단위:%)
	int nMaxIncStep;          // if nIterationMethod == 1
	int nMaxIterIncStep;      // if nIterationMethod == 1
	double dMaxDisplacement;  // if nIterationMethod == 1
	BOOL bConvFlag[3];        // index=0:Engergy, index=1:Displacement, index=2:Force
	double dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force  
};

struct T_NLCT_D_960
{
	int nNonlinearType;       // 590추가 : 0 = Geometry Nonlinear, 1 = Material Nonlinear, 2=Geometry+Material Nonlinear
	int nIterationMethod;     // 0 = Newton-Raphson, 1 = Arc-Length, 2 = Displacement-Control
	BOOL bConvFlag[3];        // 0 = Engergy, 1 = Displacement, 2 = Force
	double dConvValue[3];     // 0 = Engergy, 1 = Displacement, 2 = Force
	int nLoadStepNewton;             // if nIterationMethod == 0
	int nMaxIterPerLoadStepNewton;   // if nIterationMethod == 0
	int nMaxIncStepArclen;           // if nIterationMethod == 1
	int nMaxIterIncStepArclen;       // if nIterationMethod == 1
	double dIniForceRatioArclen;     // if nIterationMethod == 1 (단위:%) // 6.9.1에서 double로 변경
	double dMaxDisplacementArclen;   // if nIterationMethod == 1
	int nMumDispStepDispct;          // if nIterationMethod == 2
	int nMaxIterIncStepDispct;       // if nIterationMethod == 2 
	T_NODE_K MasterNodeKeyDispct;    // if nIterationMethod == 2
	int nDirectionDispct;            // if nIterationMethod == 2  0 = DX, 1 = DY, 2 = DZ 
	double dMaxDisplacementDispct;   // if nIterationMethod == 2
	CArray<T_NLCT_NEWTON, T_NLCT_NEWTON&> arNewton;
	CArray<T_NLCT_ARCLEN, T_NLCT_ARCLEN&> arArclen;
	CArray<T_NLCT_DISPCT, T_NLCT_DISPCT&> arDispct;
};

struct T_NLCT_DATA
{
	int nNonlinearType;       // 590추가 : 0 = Geometry Nonlinear, 1 = Material Nonlinear, 2=Geometry+Material Nonlinear
	int nIterationMethod;     // 0:Force-Control, 1:Arc-Length, 2:Displacement-Control
	BOOL bConvFlag[3];        // 0:Displacement(U), 1:Load(P), 2:Work(W)
	double dConvValue[3];     // 0:Displacement(U), 1:Load(P), 2:Work(W)
	int nLoadStepNewton;             // NX MEC 에서 사용 안 함 // if nIterationMethod == 0
	int nMaxIterPerLoadStepNewton;   // NX MEC 에서 사용 안 함 // if nIterationMethod == 0
	int nMaxIncStepArclen;           // if nIterationMethod == 1
	int nMaxIterIncStepArclen;       // NX MEC 에서 사용 안 함 // if nIterationMethod == 1
	double dIniForceRatioArclen;     // NX MEC 에서 사용 안 함 // if nIterationMethod == 1 (단위:%) // 6.9.1에서 double로 변경
	double dMaxDisplacementArclen;   // NX MEC 에서 사용 안 함 // if nIterationMethod == 1
	int nMumDispStepDispct;          // NX MEC 에서 사용 안 함 // if nIterationMethod == 2
	int nMaxIterIncStepDispct;       // NX MEC 에서 사용 안 함 // if nIterationMethod == 2 
	T_NODE_K MasterNodeKeyDispct;    // if nIterationMethod == 2
	int nDirectionDispct;            // if nIterationMethod == 2  0 = DX, 1 = DY, 2 = DZ 
	double dMaxDisplacementDispct;   // if nIterationMethod == 2
	CArray<T_NLCT_NEWTON, T_NLCT_NEWTON&> arNewton;   // NX MEC 에서 사용 안 함 
	CArray<T_NLCT_ARCLEN, T_NLCT_ARCLEN&> arArclen;	  // NX MEC 에서 사용 안 함 
	CArray<T_NLCT_DISPCT, T_NLCT_DISPCT&> arDispct;   // NX MEC 에서 사용 안 함 

	int	nLoadStepType;  // 0:Num. Of Inc. 1:User Defined Step
	int nNumberOfIncrement;
	int nIntermediateOutputRequest;
	CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> aLoadSteps;
	double dMinArcRatio;
	double dMaxArcRatio;
	BOOL bRelativeNode;
	T_NODE_K RelativeNodeKey;

	T_NLCT_NLCTRL_PARM AdvancedParam;

	void Initialize()
	{
		nNonlinearType = 0;
		nIterationMethod = 0;
		bConvFlag[0] = FALSE;
		bConvFlag[1] = TRUE;
		bConvFlag[2] = TRUE;
		dConvValue[0] = 0.001;
		dConvValue[1] = 0.001;
#ifdef _MEC
		dConvValue[2] = 1.e-6;
#else
		dConvValue[2] = 0.001;
#endif
		nLoadStepNewton = 1;
		nMaxIterPerLoadStepNewton = 30;
		nMaxIncStepArclen = 100;
		nMaxIterIncStepArclen = 10;
		dIniForceRatioArclen = 5.0;
		dMaxDisplacementArclen = 0.0;
		nMumDispStepDispct = 1;
		nMaxIterIncStepDispct = 10;
		MasterNodeKeyDispct = 0;
		nDirectionDispct = 0;
		dMaxDisplacementDispct = 0.0;
		arNewton.RemoveAll();
		arArclen.RemoveAll();
		arDispct.RemoveAll();

		nLoadStepType = 0;
		nNumberOfIncrement = 1;
		nIntermediateOutputRequest = 0;
		aLoadSteps.RemoveAll();
		dMinArcRatio = 0.25;
		dMaxArcRatio = 4.0;
		bRelativeNode = FALSE;
		RelativeNodeKey = 0;
		AdvancedParam.Initialize();
	}
	T_NLCT_DATA() {}
	T_NLCT_DATA(const T_NLCT_DATA& src) { *this = src; }
	T_NLCT_DATA& operator=(const T_NLCT_DATA& src)
	{
		nNonlinearType = src.nNonlinearType;
		nIterationMethod = src.nIterationMethod;
		for (int i = 0; i < 3; i++)
		{
			bConvFlag[i] = src.bConvFlag[i];
			dConvValue[i] = src.dConvValue[i];
		}
		nLoadStepNewton = src.nLoadStepNewton;
		nMaxIterPerLoadStepNewton = src.nMaxIterPerLoadStepNewton;
		nMaxIncStepArclen = src.nMaxIncStepArclen;
		nMaxIterIncStepArclen = src.nMaxIterIncStepArclen;
		dIniForceRatioArclen = src.dIniForceRatioArclen;
		dMaxDisplacementArclen = src.dMaxDisplacementArclen;
		nMumDispStepDispct = src.nMumDispStepDispct;
		nMaxIterIncStepDispct = src.nMaxIterIncStepDispct;
		MasterNodeKeyDispct = src.MasterNodeKeyDispct;
		nDirectionDispct = src.nDirectionDispct;
		dMaxDisplacementDispct = src.dMaxDisplacementDispct;
		arNewton.Copy(src.arNewton);
		arArclen.Copy(src.arArclen);
		arDispct.Copy(src.arDispct);

		nLoadStepType = src.nLoadStepType;
		nNumberOfIncrement = src.nNumberOfIncrement;
		nIntermediateOutputRequest = src.nIntermediateOutputRequest;
		aLoadSteps.Copy(src.aLoadSteps);
		dMinArcRatio = src.dMinArcRatio;
		dMaxArcRatio = src.dMaxArcRatio;
		bRelativeNode = src.bRelativeNode;
		RelativeNodeKey = src.RelativeNodeKey;

		AdvancedParam = src.AdvancedParam;

		return *this;
	}
};

struct T_NLCT_LCASE_D
{
	T_STLD_K StldK;
	T_NLCT_DATA NlctData;
	void Initialize()
	{
		StldK = 0;
		NlctData.Initialize();
	}
	T_NLCT_LCASE_D() {}
	T_NLCT_LCASE_D(const T_NLCT_LCASE_D& src) { *this = src; }
	T_NLCT_LCASE_D& operator=(const T_NLCT_LCASE_D& src)
	{
		StldK = src.StldK;
		NlctData = src.NlctData;

		return *this;
	}
};

struct T_NLCT_D
{	
	T_NLCT_DATA GlobalData;
	CArray<T_NLCT_LCASE_D, T_NLCT_LCASE_D&> aLCaseData;
	int nGlobalOrLcase;  // 0:Global, 1:LCase
	
	void Initialize()
	{		
		GlobalData.Initialize();
		aLCaseData.RemoveAll();
		nGlobalOrLcase = 0;
	}
	void Convert580(T_NLCT_D_580& rData)
	{		
		GlobalData.nNonlinearType   = 0;
		GlobalData.nIterationMethod = rData.nIterationMethod;
		for(int i = 0; i < 3; i++)
		{
			GlobalData.bConvFlag[i]  = rData.bConvFlag[i];
			GlobalData.dConvValue[i] = rData.dConvValue[i];
		} 
		GlobalData.nLoadStepNewton = 1;
		GlobalData.nMaxIterPerLoadStepNewton = 30;
		GlobalData.nMaxIncStepArclen      = 100;
		GlobalData.nMaxIterIncStepArclen  = 10;
		GlobalData.dIniForceRatioArclen   = 5.0;
		GlobalData.dMaxDisplacementArclen = 0.0;    
		GlobalData.nMumDispStepDispct     = 1;       
		GlobalData.nMaxIterIncStepDispct  = 10;  
		GlobalData.MasterNodeKeyDispct    = 0;      
		GlobalData.nDirectionDispct       = 0;           
		GlobalData.dMaxDisplacementDispct = 0.0; 
		if(GlobalData.nIterationMethod == 0)  // Newton-Raphson
		{
			GlobalData.nLoadStepNewton = rData.nLoadStep;
			GlobalData.nMaxIterPerLoadStepNewton = rData.nMaxIterPerLoadStep;
		}
		else if(GlobalData.nIterationMethod == 1)  // Arc-length
		{
			GlobalData.nMaxIncStepArclen      = rData.nMaxIncStep;
			GlobalData.nMaxIterIncStepArclen  = rData.nMaxIterIncStep;
			GlobalData.dIniForceRatioArclen   = (double)rData.nIniForceRatio;
			GlobalData.dMaxDisplacementArclen = rData.dMaxDisplacement;
		}        
		GlobalData.arNewton.RemoveAll();
		GlobalData.arArclen.RemoveAll();
		GlobalData.arDispct.RemoveAll();      

		GlobalData.nLoadStepType = 0;
		GlobalData.nNumberOfIncrement = 1;
		GlobalData.nIntermediateOutputRequest = 0;
		GlobalData.aLoadSteps.RemoveAll();
		GlobalData.dMinArcRatio = 0.25;
		GlobalData.dMaxArcRatio = 4.0;
		GlobalData.bRelativeNode = FALSE;
		GlobalData.RelativeNodeKey = 0;
		GlobalData.AdvancedParam.Initialize();
				
		aLCaseData.RemoveAll();
		nGlobalOrLcase = 0;
	}
	void ConvertMaster(T_NLCT_D_960& rData)
	{
		GlobalData.nNonlinearType = rData.nNonlinearType;
		GlobalData.nIterationMethod = rData.nIterationMethod;
		for (int i = 0; i < 3; i++)
		{
			GlobalData.bConvFlag[i] = rData.bConvFlag[i];
			GlobalData.dConvValue[i] = rData.dConvValue[i];
		}
		GlobalData.nLoadStepNewton = rData.nLoadStepNewton;
		GlobalData.nMaxIterPerLoadStepNewton = rData.nMaxIterPerLoadStepNewton;
		GlobalData.nMaxIncStepArclen = rData.nMaxIncStepArclen;
		GlobalData.nMaxIterIncStepArclen = rData.nMaxIterIncStepArclen;
		GlobalData.dIniForceRatioArclen = rData.dIniForceRatioArclen;
		GlobalData.dMaxDisplacementArclen = rData.dMaxDisplacementArclen;
		GlobalData.nMumDispStepDispct = rData.nMumDispStepDispct;
		GlobalData.nMaxIterIncStepDispct = rData.nMaxIterIncStepDispct;
		GlobalData.MasterNodeKeyDispct = rData.MasterNodeKeyDispct;
		GlobalData.nDirectionDispct = rData.nDirectionDispct;
		GlobalData.dMaxDisplacementDispct = rData.dMaxDisplacementDispct;
		GlobalData.arNewton.Copy(rData.arNewton);
		GlobalData.arArclen.Copy(rData.arArclen);
		GlobalData.arDispct.Copy(rData.arDispct);

		GlobalData.nLoadStepType = 0;
		GlobalData.nNumberOfIncrement = 1;
		GlobalData.nIntermediateOutputRequest = 0;
		GlobalData.aLoadSteps.RemoveAll();
		GlobalData.dMinArcRatio = 0.25;
		GlobalData.dMaxArcRatio = 4.0;
		GlobalData.bRelativeNode = FALSE;
		GlobalData.RelativeNodeKey = 0;
		GlobalData.AdvancedParam.Initialize();
				
		aLCaseData.RemoveAll();
		nGlobalOrLcase = 0;
	}
	T_NLCT_D() {}
	T_NLCT_D(T_NLCT_D& src) {*this = src;}
	T_NLCT_D& operator=(T_NLCT_D& src)
	{
		GlobalData = src.GlobalData;
		aLCaseData.Copy(src.aLCaseData);
		nGlobalOrLcase = src.nGlobalOrLcase;

		return *this;
	}
};
struct T_NLCT_UDRD_D_580
{
	T_NLCT_D_580 data;
};
struct T_NLCT_UDRD_D
{
	T_NLCT_K key;
	T_NLCT_D data;
};
struct T_NLCT_NEWTON_RW_BASE
{
	T_STLD_K StldKey;
	int nLoadStep;            
	int nMaxIterPerLoadStep;
	int nCountLoadFactor;  
};
struct T_NLCT_NEWTON_RW   // 590추가,  if nIterationMethod == 0
{
	CArray<T_NLCT_NEWTON_RW_BASE, T_NLCT_NEWTON_RW_BASE&> arNewton;
	void SetNlctNewton(T_NLCT_D& rData)
	{
		arNewton.SetSize(rData.GlobalData.arNewton.GetSize());
		for(int i = 0; i < arNewton.GetSize(); i++)
		{
			arNewton[i].StldKey = rData.GlobalData.arNewton[i].StldKey;
			arNewton[i].nLoadStep = rData.GlobalData.arNewton[i].nLoadStep;
			arNewton[i].nMaxIterPerLoadStep = rData.GlobalData.arNewton[i].nMaxIterPerLoadStep;
			arNewton[i].nCountLoadFactor = (int)rData.GlobalData.arNewton[i].arLoadFactor.GetSize();
		}
	}  
};
struct T_NLCT_DISPCT_RW_BASE   // 590추가,  if nIterationMethod == 2
{
	T_STLD_K StldKey;
	int nMumDispStep;         
	int nMaxIterIncStep;      
	T_NODE_K MasterNodeKey;   
	int nDirection;           // 0 = DX, 1 = DY, 2 = DZ 
	double dMaxDisplacement;  
	int nCountNodeDisp;
};
struct T_NLCT_DISPCT_RW   // 590추가,  if nIterationMethod == 2
{
	CArray<T_NLCT_DISPCT_RW_BASE, T_NLCT_DISPCT_RW_BASE&> arDispct;
	void SetNlctDispct(T_NLCT_D& rData)
	{
		arDispct.SetSize(rData.GlobalData.arDispct.GetSize());
		for(int i = 0; i < arDispct.GetSize(); i++)
		{
			arDispct[i].StldKey          = rData.GlobalData.arDispct[i].StldKey;
			arDispct[i].nMumDispStep     = rData.GlobalData.arDispct[i].nMumDispStep;
			arDispct[i].nMaxIterIncStep  = rData.GlobalData.arDispct[i].nMaxIterIncStep;
			arDispct[i].MasterNodeKey    = rData.GlobalData.arDispct[i].MasterNodeKey;
			arDispct[i].nDirection       = rData.GlobalData.arDispct[i].nDirection;
			arDispct[i].dMaxDisplacement = rData.GlobalData.arDispct[i].dMaxDisplacement;
			arDispct[i].nCountNodeDisp   = (int)rData.GlobalData.arDispct[i].arNodeDisp.GetSize();
		}
	}  
};
struct T_NLCT_RW_580
{
	int nIterationMethod;     
	int nLoadStep;            
	int nMaxIterPerLoadStep;  
	int nIniForceRatio;       
	int nMaxIncStep;          
	int nMaxIterIncStep;      
	double dMaxDisplacement;  
	BOOL bConvFlag[3];        
	double dConvValue[3];     
};
struct T_NLCT_RW_690
{
	int nNonlinearType;       
	int nIterationMethod;     
	BOOL bConvFlag[3];        
	double dConvValue[3];     
	int nLoadStepNewton;            
	int nMaxIterPerLoadStepNewton;
	int nMaxIncStepArclen;
	int nMaxIterIncStepArclen;      
	int nIniForceRatioArclen;       //  (단위:%)
	double dMaxDisplacementArclen;
	int nMumDispStepDispct;
	int nMaxIterIncStepDispct;
	T_NODE_K MasterNodeKeyDispct;
	int nDirectionDispct;           // 0 = DX, 1 = DY, 2 = DZ 
	double dMaxDisplacementDispct;  
	unsigned int nCountNewton;
	unsigned int nCountArclen;
	unsigned int nCountDispct;   
};
struct T_NLCT_RW_842
{
	int nNonlinearType;       
	int nIterationMethod;     
	BOOL bConvFlag[3];        
	double dConvValue[3];     
	int nLoadStepNewton;            
	int nMaxIterPerLoadStepNewton;
	int nMaxIncStepArclen;
	int nMaxIterIncStepArclen;      
	double dIniForceRatioArclen;       //  (단위:%)
	double dMaxDisplacementArclen;
	int nMumDispStepDispct;
	int nMaxIterIncStepDispct;
	T_NODE_K MasterNodeKeyDispct;
	int nDirectionDispct;           // 0 = DX, 1 = DY, 2 = DZ 
	double dMaxDisplacementDispct;  
	unsigned int nCountNewton;
	unsigned int nCountArclen;
	unsigned int nCountDispct;   
};
struct T_NLCT_RW_960
{
	int nNonlinearType;
	int nIterationMethod;
	BOOL bConvFlag[3];
	double dConvValue[3];
	int nLoadStepNewton;
	int nMaxIterPerLoadStepNewton;
	int nMaxIncStepArclen;
	int nMaxIterIncStepArclen;
	double dIniForceRatioArclen;       //  (단위:%)
	double dMaxDisplacementArclen;
	int nMumDispStepDispct;
	int nMaxIterIncStepDispct;
	T_NODE_K MasterNodeKeyDispct;
	int nDirectionDispct;           // 0 = DX, 1 = DY, 2 = DZ 
	double dMaxDisplacementDispct;
	unsigned int nCountNewton;
	unsigned int nCountArclen;
	unsigned int nCountDispct;
	BOOL bDummy; // T_NLCT_RW_842와 저장방식이 달라지기 때문에, 사이즈를 다르게 하기 위해 더미로 추가함.
};
struct T_NLCT_DATA_RW
{
	T_NLCT_K key;
	int nNonlinearType;
	int nIterationMethod;
	BOOL bConvFlag[3];
	double dConvValue[3];
	int nLoadStepNewton;
	int nMaxIterPerLoadStepNewton;
	int nMaxIncStepArclen;
	int nMaxIterIncStepArclen;
	double dIniForceRatioArclen;       //  (단위:%)
	double dMaxDisplacementArclen;
	int nMumDispStepDispct;
	int nMaxIterIncStepDispct;
	T_NODE_K MasterNodeKeyDispct;
	int nDirectionDispct;           // 0 = DX, 1 = DY, 2 = DZ 
	double dMaxDisplacementDispct;
	unsigned int nCountNewton;
	unsigned int nCountArclen;
	unsigned int nCountDispct;
	BOOL bDummy; // T_NLCT_RW_842와 저장방식이 달라지기 때문에, 사이즈를 다르게 하기 위해 더미로 추가함.

	int nAnalCtrlType; // 0 : Nonlinear Analysis for All Static Load Cases, 1: Select Load Case to be Analyzed as Nonlinear Analysis
	T_STLD_K StldKey;
	int	nLoadStepType;
	int nNumberOfIncrement;
	int nIntermediateOutputRequest;
	int nCountLoadSteps; // CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> aLoadSteps;
	double dMinArcRatio;
	double dMaxArcRatio;
	BOOL bRelativeNode;
	T_NODE_K RelativeNodeKey;

	T_NLCT_NLCTRL_PARM AdvancedParam;

	void GetNlctData(T_NLCT_DATA& rData)
	{
		rData.nNonlinearType = nNonlinearType;
		rData.nIterationMethod = nIterationMethod;
		for (int i = 0; i < 3; i++)
		{
			rData.bConvFlag[i] = bConvFlag[i];
			rData.dConvValue[i] = dConvValue[i];
		}
		rData.nLoadStepNewton = nLoadStepNewton;
		rData.nMaxIterPerLoadStepNewton = nMaxIterPerLoadStepNewton;
		rData.nMaxIncStepArclen = nMaxIncStepArclen;
		rData.nMaxIterIncStepArclen = nMaxIterIncStepArclen;
		rData.dIniForceRatioArclen = dIniForceRatioArclen;
		rData.dMaxDisplacementArclen = dMaxDisplacementArclen;
		rData.nMumDispStepDispct = nMumDispStepDispct;
		rData.nMaxIterIncStepDispct = nMaxIterIncStepDispct;
		rData.MasterNodeKeyDispct = MasterNodeKeyDispct;
		rData.nDirectionDispct = nDirectionDispct;
		rData.dMaxDisplacementDispct = dMaxDisplacementDispct;
		//rData.arNewton.SetSize(nCountNewton);
		//rData.arArclen.SetSize(nCountArclen);
		//rData.arDispct.SetSize(nCountDispct);

		rData.nLoadStepType = nLoadStepType;
		rData.nNumberOfIncrement = nNumberOfIncrement;
		rData.nIntermediateOutputRequest = nIntermediateOutputRequest;
		//rData.aLoadSteps.SetSize(nCountLoadSteps);
		rData.dMinArcRatio = dMinArcRatio;
		rData.dMaxArcRatio = dMaxArcRatio;
		rData.bRelativeNode = bRelativeNode;
		rData.RelativeNodeKey = RelativeNodeKey;

		rData.AdvancedParam = AdvancedParam;
	}
	void SetNlctData(const T_NLCT_DATA& rData)
	{
		nNonlinearType = rData.nNonlinearType;
		nIterationMethod = rData.nIterationMethod;
		for (int i = 0; i < 3; i++)
		{
			bConvFlag[i] = rData.bConvFlag[i];
			dConvValue[i] = rData.dConvValue[i];
		}
		nLoadStepNewton = rData.nLoadStepNewton;
		nMaxIterPerLoadStepNewton = rData.nMaxIterPerLoadStepNewton;
		nMaxIncStepArclen = rData.nMaxIncStepArclen;
		nMaxIterIncStepArclen = rData.nMaxIterIncStepArclen;
		dIniForceRatioArclen = rData.dIniForceRatioArclen;
		dMaxDisplacementArclen = rData.dMaxDisplacementArclen;
		nMumDispStepDispct = rData.nMumDispStepDispct;
		nMaxIterIncStepDispct = rData.nMaxIterIncStepDispct;
		MasterNodeKeyDispct = rData.MasterNodeKeyDispct;
		nDirectionDispct = rData.nDirectionDispct;
		dMaxDisplacementDispct = rData.dMaxDisplacementDispct;
		nCountNewton = rData.arNewton.GetSize();
		nCountArclen = rData.arArclen.GetSize();
		nCountDispct = rData.arDispct.GetSize();

		nLoadStepType = rData.nLoadStepType;
		nNumberOfIncrement = rData.nNumberOfIncrement;
		nCountLoadSteps = rData.aLoadSteps.GetSize();
		nIntermediateOutputRequest = rData.nIntermediateOutputRequest;
		dMinArcRatio = rData.dMinArcRatio;
		dMaxArcRatio = rData.dMaxArcRatio;
		bRelativeNode = rData.bRelativeNode;
		RelativeNodeKey = rData.RelativeNodeKey;

		AdvancedParam = rData.AdvancedParam;
	}
};

struct T_NLCT_LCASE_RW
{
	T_STLD_K StldK;
	T_NLCT_DATA_RW NlctData;
	void GetNlctLcase(T_NLCT_LCASE_D& rData)
	{
		rData.StldK = StldK;
		NlctData.GetNlctData(rData.NlctData);
	}

	void SetNlctLcase(const T_NLCT_LCASE_D& rData)
	{
		StldK = rData.StldK;
		NlctData.SetNlctData(rData.NlctData);
	}
};

struct T_NLCT_RW
{	
	T_NLCT_DATA_RW GlobalData;
	unsigned int nCountLCaseData;
	int nGlobalOrLcase;

	T_NLCT_RW() { GlobalData.bDummy=FALSE; }
	void GetNlct(T_NLCT_D& rData)
	{		
		GlobalData.GetNlctData(rData.GlobalData);

		//rData.aLCaseData.SetSize(nCountLCaseData);
		rData.nGlobalOrLcase = nGlobalOrLcase;
	}
	void SetNlct(const T_NLCT_D& rData)
	{
		GlobalData.SetNlctData(rData.GlobalData);

		nCountLCaseData = rData.aLCaseData.GetSize();
		nGlobalOrLcase = rData.nGlobalOrLcase;
	}
	void Convert580(T_NLCT_RW_580& rData)
	{
		GlobalData.nNonlinearType   = 0;     
		GlobalData.nIterationMethod = rData.nIterationMethod;
		for(int i = 0; i < 3; i++)
		{
			GlobalData.bConvFlag[i]  = rData.bConvFlag[i]; 
			GlobalData.dConvValue[i] = rData.dConvValue[i];      
		}
		GlobalData.nLoadStepNewton = 1;
		GlobalData.nMaxIterPerLoadStepNewton = 30;
		GlobalData.nMaxIncStepArclen      = 100;
		GlobalData.nMaxIterIncStepArclen  = 10;
		GlobalData.dIniForceRatioArclen   = 5.0;
		GlobalData.dMaxDisplacementArclen = 0.0;    
		GlobalData.nMumDispStepDispct     = 1;       
		GlobalData.nMaxIterIncStepDispct  = 10;  
		GlobalData.MasterNodeKeyDispct    = 0;      
		GlobalData.nDirectionDispct       = 0;           
		GlobalData.dMaxDisplacementDispct = 0.0;     
		if(GlobalData.nIterationMethod == 0)  // Newton-Raphson
		{
			GlobalData.nLoadStepNewton = rData.nLoadStep;
			GlobalData.nMaxIterPerLoadStepNewton = rData.nMaxIterPerLoadStep;
		}
		else if(GlobalData.nIterationMethod == 1)  // Arc-length
		{
			GlobalData.nMaxIncStepArclen      = rData.nMaxIncStep;
			GlobalData.nMaxIterIncStepArclen  = rData.nMaxIterIncStep;
			GlobalData.dIniForceRatioArclen   = (double)rData.nIniForceRatio;
			GlobalData.dMaxDisplacementArclen = rData.dMaxDisplacement;
		} 
		GlobalData.nCountNewton = 0;
		GlobalData.nCountArclen = 0;
		GlobalData.nCountDispct = 0;

		GlobalData.nAnalCtrlType = 0;
		GlobalData.StldKey = 0;
		GlobalData.nLoadStepType = 0;
		GlobalData.nNumberOfIncrement = 1;
		GlobalData.nIntermediateOutputRequest = 0;
		GlobalData.nCountLoadSteps = 0;
		GlobalData.dMinArcRatio = 0.25;
		GlobalData.dMaxArcRatio = 4.0;
		GlobalData.bRelativeNode = FALSE;
		GlobalData.RelativeNodeKey = 0;

		GlobalData.AdvancedParam.Initialize();

		nCountLCaseData = 0;
		nGlobalOrLcase = 0;
	}
	void Convert690(T_NLCT_RW_690& rData)
	{
		GlobalData.nNonlinearType   = rData.nNonlinearType;     
		GlobalData.nIterationMethod = rData.nIterationMethod;
		for(int i = 0; i < 3; i++)
		{
			GlobalData.bConvFlag[i]  = rData.bConvFlag[i]; 
			GlobalData.dConvValue[i] = rData.dConvValue[i];      
		}
		GlobalData.nLoadStepNewton = rData.nLoadStepNewton;
		GlobalData.nMaxIterPerLoadStepNewton = rData.nMaxIterPerLoadStepNewton;
		GlobalData.nMaxIncStepArclen      = rData.nMaxIncStepArclen;
		GlobalData.nMaxIterIncStepArclen  = rData.nMaxIterIncStepArclen;
		GlobalData.dIniForceRatioArclen   = (double)rData.nIniForceRatioArclen;
		GlobalData.dMaxDisplacementArclen = rData.dMaxDisplacementArclen;    
		GlobalData.nMumDispStepDispct     = rData.nMumDispStepDispct;       
		GlobalData.nMaxIterIncStepDispct  = rData.nMaxIterIncStepDispct;  
		GlobalData.MasterNodeKeyDispct    = rData.MasterNodeKeyDispct;      
		GlobalData.nDirectionDispct       = rData.nDirectionDispct;           
		GlobalData.dMaxDisplacementDispct = rData.dMaxDisplacementDispct;
		GlobalData.nCountNewton = rData.nCountNewton;
		GlobalData.nCountArclen = rData.nCountArclen;
		GlobalData.nCountDispct = rData.nCountDispct;

		GlobalData.nAnalCtrlType = 0;
		GlobalData.StldKey = 0;
		GlobalData.nLoadStepType = 0;
		GlobalData.nNumberOfIncrement = 1;
		GlobalData.nIntermediateOutputRequest = 0;
		GlobalData.nCountLoadSteps = 0;
		GlobalData.dMinArcRatio = 0.25;
		GlobalData.dMaxArcRatio = 4.0;
		GlobalData.bRelativeNode = FALSE;
		GlobalData.RelativeNodeKey = 0;

		GlobalData.AdvancedParam.Initialize();

		nCountLCaseData = 0;
		nGlobalOrLcase = 0;
	}
	void Convert842(T_NLCT_RW_842& rData)
	{
		GlobalData.nNonlinearType = rData.nNonlinearType;
		GlobalData.nIterationMethod = rData.nIterationMethod;
		for (int i = 0; i < 3; i++)
		{
			GlobalData.bConvFlag[i] = rData.bConvFlag[i];
			GlobalData.dConvValue[i] = rData.dConvValue[i];
		}
		GlobalData.nLoadStepNewton = rData.nLoadStepNewton;
		GlobalData.nMaxIterPerLoadStepNewton = rData.nMaxIterPerLoadStepNewton;
		GlobalData.nMaxIncStepArclen = rData.nMaxIncStepArclen;
		GlobalData.nMaxIterIncStepArclen = rData.nMaxIterIncStepArclen;
		GlobalData.dIniForceRatioArclen = rData.dIniForceRatioArclen;
		GlobalData.dMaxDisplacementArclen = rData.dMaxDisplacementArclen;
		GlobalData.nMumDispStepDispct = rData.nMumDispStepDispct;
		GlobalData.nMaxIterIncStepDispct = rData.nMaxIterIncStepDispct;
		GlobalData.MasterNodeKeyDispct = rData.MasterNodeKeyDispct;
		GlobalData.nDirectionDispct = rData.nDirectionDispct;
		GlobalData.dMaxDisplacementDispct = rData.dMaxDisplacementDispct;
		GlobalData.nCountNewton = rData.nCountNewton;
		GlobalData.nCountArclen = rData.nCountArclen;
		GlobalData.nCountDispct = rData.nCountDispct;

		GlobalData.nAnalCtrlType = 0;
		GlobalData.StldKey = 0;
		GlobalData.nLoadStepType = 0;
		GlobalData.nNumberOfIncrement = 1;
		GlobalData.nIntermediateOutputRequest = 0;
		GlobalData.nCountLoadSteps = 0;
		GlobalData.dMinArcRatio = 0.25;
		GlobalData.dMaxArcRatio = 4.0;
		GlobalData.bRelativeNode = FALSE;
		GlobalData.RelativeNodeKey = 0;

		GlobalData.AdvancedParam.Initialize();

		nCountLCaseData = 0;
		nGlobalOrLcase = 0;
	}
	void Convert960(const T_NLCT_RW_960& rData)
	{
		GlobalData.nNonlinearType = rData.nNonlinearType;
		GlobalData.nIterationMethod = rData.nIterationMethod;
		for (int i = 0; i < 3; i++)
		{
			GlobalData.bConvFlag[i] = rData.bConvFlag[i];
			GlobalData.dConvValue[i] = rData.dConvValue[i];
		}
		GlobalData.nLoadStepNewton = rData.nLoadStepNewton;
		GlobalData.nMaxIterPerLoadStepNewton = rData.nMaxIterPerLoadStepNewton;
		GlobalData.nMaxIncStepArclen = rData.nMaxIncStepArclen;
		GlobalData.nMaxIterIncStepArclen = rData.nMaxIterIncStepArclen;
		GlobalData.dIniForceRatioArclen = rData.dIniForceRatioArclen;
		GlobalData.dMaxDisplacementArclen = rData.dMaxDisplacementArclen;
		GlobalData.nMumDispStepDispct = rData.nMumDispStepDispct;
		GlobalData.nMaxIterIncStepDispct = rData.nMaxIterIncStepDispct;
		GlobalData.MasterNodeKeyDispct = rData.MasterNodeKeyDispct;
		GlobalData.nDirectionDispct = rData.nDirectionDispct;
		GlobalData.dMaxDisplacementDispct = rData.dMaxDisplacementDispct;

		GlobalData.nCountNewton = rData.nCountNewton;
		GlobalData.nCountArclen = rData.nCountArclen;
		GlobalData.nCountDispct = rData.nCountDispct;

		GlobalData.nAnalCtrlType = 0;
		GlobalData.StldKey = 0;
		GlobalData.nLoadStepType = 0;
		GlobalData.nNumberOfIncrement = 1;
		GlobalData.nIntermediateOutputRequest = 0;
		GlobalData.nCountLoadSteps = 0;
		GlobalData.dMinArcRatio = 0.25;
		GlobalData.dMaxArcRatio = 4.0;
		GlobalData.bRelativeNode = FALSE;
		GlobalData.RelativeNodeKey = 0;

		GlobalData.AdvancedParam.Initialize();

		nCountLCaseData = 0;
		nGlobalOrLcase = 0;
	}
};

struct T_NLCT_UNIT
{
	T_NLCT_UNIT()
	{
		dMaxDisplacementArclen = D_UNITSYS_BASE_LENGTH;
		dMaxDisplacementDispct = D_UNITSYS_BASE_LENGTH;
		dMaxDisplacement = D_UNITSYS_BASE_LENGTH;
		dDispCtrlMaxDisp = D_UNITSYS_BASE_LENGTH;
		dMasterNodeDisp  = D_UNITSYS_BASE_LENGTH;
	}
	int dMaxDisplacementArclen;
	int dMaxDisplacementDispct;
	int dMaxDisplacement;
	int dDispCtrlMaxDisp;
	int dMasterNodeDisp;
};
#define HASHSIZENLCT 1

// loading sequence for non-linear analysis
#define T_LDSQ_K unsigned int
struct T_LDSQ_D
{
	CArray<T_STLD_K, T_STLD_K> aLoadingSequence;
	void Initialize()
	{
		aLoadingSequence.RemoveAll();
	}
	T_LDSQ_D() {}
	T_LDSQ_D(T_LDSQ_D& rData) { *this = rData; }
	T_LDSQ_D& operator=(T_LDSQ_D& rData)
	{
		aLoadingSequence.Copy(rData.aLoadingSequence);
		return *this;
	}
};
struct T_LDSQ_UDRD_D
{
	T_LDSQ_D data;
};
struct T_LDSQ_UDRD_D_RW
{
	unsigned int nNumSequence;
	void GetLdsq(T_LDSQ_D& rData)
	{
	}
	void SetLdsq(const T_LDSQ_D& rData)
	{
		nNumSequence = (int)rData.aLoadingSequence.GetSize();
	}
};
struct T_LDSQ_UNIT
{
	T_LDSQ_UNIT()
	{
		aLoadingSequence = D_UNITSYS_NONE;
	}
	int aLoadingSequence;
};
#define HASHSIZELDSQ 1


#define T_EPMT_K unsigned int

struct T_EPMT_TRESCA
{
	double dInitYieldStress;  
	BOOL   bHardening;
	int    nHardeningType;   // 0=Isotropic, 1=Kinematic, 2=Mixed  
	double dBackStressCoef;
	double dHardeningCoef;  
	void Initialize()
	{
		dInitYieldStress = 0.0;
		bHardening = FALSE;
		nHardeningType = 0;
		dBackStressCoef = 1.0;
		dHardeningCoef = 0.0;
	}
};
struct T_EPMT_VMISES_955
{
	double dInitYieldStress;
	BOOL   bHardening;
	int    nHardeningType;   // 0=Isotropic, 1=Kinematic, 2=Mixed  
	double dBackStressCoef;
	double dHardeningCoef;
};
struct T_EPMT_VMISES
{
	double dInitYieldStress;  
	BOOL   bHardening;
	int    nHardeningType;   // 0=Isotropic, 1=Kinematic, 2=Mixed  
	double dBackStressCoef;
	double dHardeningCoef;
	BOOL   bUseFiber;
	void Initialize()
	{
		dInitYieldStress = 0.0;
		bHardening = FALSE;
		nHardeningType = 0;
		dBackStressCoef = 1.0;
		dHardeningCoef = 0.0;
		bUseFiber = FALSE;
	}
	void Convert955(T_EPMT_VMISES_955& rData)
	{
		dInitYieldStress = rData.dInitYieldStress;
		bHardening       = rData.bHardening;
		nHardeningType   = rData.nHardeningType;
		dBackStressCoef  = rData.dBackStressCoef;
		dHardeningCoef   = rData.dHardeningCoef;
		bUseFiber = FALSE;
	}
};
struct T_EPMT_MOHRCL
{
	double dInitCohesion;  
	double dInitFricAngle;  
	BOOL   bHardening;
	int    nHardeningType;   // 0=Isotropic, 1=Kinematic, 2=Mixed  
	double dBackStressCoef;
	double dHardeningCoef;
	void Initialize()
	{
		dInitCohesion  = 0.0;
		dInitFricAngle = 0.0;
		bHardening = FALSE;
		dBackStressCoef = 1.0;
		nHardeningType = 0;
		dHardeningCoef = 0.0;
	}
};
struct T_EPMT_DRUCKER
{
	double dInitCohesion;  
	double dInitFricAngle;  
	BOOL   bHardening;
	int    nHardeningType;   // 0=Isotropic, 1=Kinematic, 2=Mixed  
	double dBackStressCoef;
	double dHardeningCoef;
	void Initialize()
	{
		dInitCohesion  = 0.0;
		dInitFricAngle = 0.0;
		bHardening = FALSE;
		nHardeningType = 0;
		dBackStressCoef = 1.0;
		dHardeningCoef = 0.0;
	}
};
struct T_EPMT_MICROPL
{
	double dParamK[4];
	double dParamC[17];
	double dMu;  
	void Initialize()
	{
		dParamK[0] = 1.65e-4;
		dParamK[1] = 160.0;
		dParamK[2] = 10.0;
		dParamK[3] = 150.0;
		dParamC[0] = 0.62;
		dParamC[1] = 2.76;
		dParamC[2] = 4.00;
		dParamC[3] = 70.0;
		dParamC[4] = 2.50;
		dParamC[5] = 1.30;
		dParamC[6] = 50.0;
		dParamC[7] = 8.00;
		dParamC[8] = 1.30;
		dParamC[9] = 0.73;
		dParamC[10] = 0.20;
		dParamC[11] = 7.00;
		dParamC[12] = 0.20;
		dParamC[13] = 0.50;
		dParamC[14] = 0.02;
		dParamC[15] = 0.01;
		dParamC[16] = 0.40;     
		dMu = 1.00;      
	}
};

struct T_EPMT_MASONRY_770
{
	// brick material properties
	double dBR_Es;   // Young's Modulus [F/L^2]
	double dBR_Pr;   // Poisson's Ratio
	double dBR_Fc;   // Compression Strength [F/L^2]
	double dBR_Ft;   // Tension Strength [F/L^2]
	double dBR_H;    // Softening Parameter
	double dBR_FA;   // Friction Angle [deg]
	double dBR_Val1; // temporary value 1
	double dBR_Val2; // temporary value 2
	double dBR_Val3; // temporary value 3
	double dBR_Val4; // temporary value 4
	double dBR_Val5; // temporary value 5
	double dBR_Val6; // temporary value 6
	double dBR_Val7; // temporary value 7
	// bed joint material properties
	double dBJ_Es;   // Young's Modulus [F/L^2]
	double dBJ_Pr;   // Poisson's Ratio
	double dBJ_Fc;   // Compression Strength [F/L^2]
	double dBJ_Ft;   // Tension Strength [F/L^2]
	double dBJ_H;    // Hardening Parameter
	double dBJ_BWS;  // Bond Wrench Strength [F/L^2]
	// head joint material properties
	double dHJ_Es;   // Young's Modulus [F/L^2]
	double dHJ_Pr;   // Poisson's Ratio
	double dHJ_Fc;   // Compression Strength [F/L^2]
	double dHJ_Ft;   // Tension Strength [F/L^2]
	double dHJ_H;    // Hardening Parameter
	double dHJ_BWS;  // Bond Wrench Strength [F/L^2]
	// geometric properties
	double dGP_BL;   // Brick Length [L]
	double dGP_BH;   // Brick Height [L]
	double dGP_Tb;   // Thickness of Bed [L]
	double dGP_Th;   // Thickness of Head [L]
};

struct T_EPMT_MASONRY_791
{
	// brick material properties
	double dBR_Es;   // Young's Modulus [F/L^2]
	double dBR_Pr;   // Poisson's Ratio
	double dBR_Fc;   // Compression Strength [F/L^2]
	double dBR_Ft;   // Tension Strength [F/L^2]
	double dBR_H;    // Softening Parameter
	double dBR_FA;   // Friction Angle [deg]
	double dBR_Val1; // temporary value 1
	double dBR_Val2; // temporary value 2
	double dBR_Val3; // temporary value 3
	double dBR_Val4; // temporary value 4
	double dBR_Val5; // temporary value 5
	double dBR_Val6; // temporary value 6
	double dBR_Val7; // temporary value 7
	// bed joint material properties
	double dBJ_Es;   // Young's Modulus [F/L^2]
	double dBJ_Pr;   // Poisson's Ratio
	double dBJ_Fc;   // Compression Strength [F/L^2]
	double dBJ_Ft;   // Tension Strength [F/L^2]
	double dBJ_H;    // Hardening Parameter
	double dBJ_BWS;  // Bond Wrench Strength [F/L^2]
	// head joint material properties
	double dHJ_Es;   // Young's Modulus [F/L^2]
	double dHJ_Pr;   // Poisson's Ratio
	double dHJ_Fc;   // Compression Strength [F/L^2]
	double dHJ_Ft;   // Tension Strength [F/L^2]
	double dHJ_H;    // Hardening Parameter
	double dHJ_BWS;  // Bond Wrench Strength [F/L^2]
	// geometric properties
	double dGP_BL;   // Brick Length [L]
	double dGP_BH;   // Brick Height [L]
	double dGP_Tb;   // Thickness of Bed [L]
	double dGP_Th;   // Thickness of Head [L]
	// Material Coordinates System
	int nMatlCoordKey;    // v780 추가.
												// key of Material Coord System.
												// =  0 : Global Coord.
												// = -1 : Element Local Coord.
												// >  0 : User Defined Material Coord System.
};

struct T_EPMT_MASONRY
{
	// brick material properties
	double dBR_Es;   // Young's Modulus [F/L^2]
	double dBR_Pr;   // Poisson's Ratio
	double dBR_Fc;   // Compression Strength [F/L^2]
	double dBR_Ft;   // Tension Strength [F/L^2]
	double dBR_H;    // Softening Parameter
	double dBR_FA;   // Friction Angle [deg]
	double dBR_Val1; // temporary value 1
	double dBR_Val2; // temporary value 2
	double dBR_Val3; // temporary value 3
	double dBR_Val4; // temporary value 4
	double dBR_Val5; // temporary value 5
	double dBR_Val6; // temporary value 6
	double dBR_Val7; // temporary value 7
	// bed joint material properties
	double dBJ_Es;   // Young's Modulus [F/L^2]
	double dBJ_Pr;   // Poisson's Ratio
	double dBJ_Fc;   // Compression Strength [F/L^2]
	double dBJ_Ft;   // Tension Strength [F/L^2]
	double dBJ_H;    // Hardening Parameter
	double dBJ_BWS;  // Bond Wrench Strength [F/L^2]
	// head joint material properties
	double dHJ_Es;   // Young's Modulus [F/L^2]
	double dHJ_Pr;   // Poisson's Ratio
	double dHJ_Fc;   // Compression Strength [F/L^2]
	double dHJ_Ft;   // Tension Strength [F/L^2]
	double dHJ_H;    // Hardening Parameter
	double dHJ_BWS;  // Bond Wrench Strength [F/L^2]
	// geometric properties
	double dGP_BL;   // Brick Length [L]
	double dGP_BH;   // Brick Height [L]
	double dGP_Tb;   // Thickness of Bed [L]
	double dGP_Th;   // Thickness of Head [L]
	// Material Coordinates System
	int nMatlCoordKey;    // v780 추가.
												// key of Material Coord System.
												// =  0 : Global-Y : Global-X // Global
												// = -1 : Local-y : Local-x   // Element Local
												// = -4 : Global-Z : Angle
												// >  0 : User Defined Material Coord System.
	double dAngle;    // v795 추가.  // MNET:XXXX-HSSHIM-20110828. 
										// if (nMatlCoordKey == -4(Global-Z:Angle)) then use this.

	void Initialize() // CAttrCtrl의 GetDefaultEpmt()를 사용할 것
	{
		dBR_Es = 0.0;
		dBR_Pr = 0.0;
		dBR_Fc = 0.0;
		dBR_Ft = 0.0;
		dBR_H  = 0.0;
		dBR_FA = 0.0;
		dBR_Val1= 0.0;
		dBR_Val2= 0.0;
		dBR_Val3= 0.0;
		dBR_Val4= 0.0;
		dBR_Val5= 0.0;
		dBR_Val6= 0.0;
		dBR_Val7= 0.0;
		dBJ_Es = 0.0;
		dBJ_Pr = 0.0;
		dBJ_Fc = 0.0;
		dBJ_Ft = 0.0;
		dBJ_H  = 0.0;
		dBJ_BWS= 0.0;
		dHJ_Es= 0.0;
		dHJ_Pr= 0.0;
		dHJ_Fc= 0.0;
		dHJ_Ft= 0.0;
		dHJ_H = 0.0;
		dHJ_BWS = 0.0;
		dGP_BL= 0.0;  
		dGP_BH= 0.0;  
		dGP_Tb= 0.0;  
		dGP_Th= 0.0;  
		nMatlCoordKey = -1;   // MNET:XXXX-HSSHIM-20100211.  이전버전과의 호환성, Global-Z 방향의 유효성 문제로 Default를 Element local로 변경함. 
		dAngle = 0.0;
	};
	void Convert770(T_EPMT_MASONRY_770& rData)
	{
		dBR_Es = rData.dBR_Es ;
		dBR_Pr = rData.dBR_Pr ;
		dBR_Fc = rData.dBR_Fc ;
		dBR_Ft = rData.dBR_Ft ;
		dBR_H  = rData.dBR_H  ;
		dBR_FA = rData.dBR_FA ;
		dBR_Val1 = rData.dBR_Val1 ;
		dBR_Val2 = rData.dBR_Val2 ;
		dBR_Val3 = rData.dBR_Val3 ;
		dBR_Val4 = rData.dBR_Val4 ;
		dBR_Val5 = rData.dBR_Val5 ;
		dBR_Val6 = rData.dBR_Val6 ;
		dBR_Val7 = rData.dBR_Val7 ;
		dBJ_Es = rData.dBJ_Es ;
		dBJ_Pr = rData.dBJ_Pr ;
		dBJ_Fc = rData.dBJ_Fc ;
		dBJ_Ft = rData.dBJ_Ft ;
		dBJ_H  = rData.dBJ_H  ;
		dBJ_BWS = rData.dBJ_BWS ;
		dHJ_Es = rData.dHJ_Es ;
		dHJ_Pr = rData.dHJ_Pr ;
		dHJ_Fc = rData.dHJ_Fc ;
		dHJ_Ft = rData.dHJ_Ft ;
		dHJ_H = rData.dHJ_H ;
		dHJ_BWS = rData.dHJ_BWS ;
		dGP_BL = rData.dGP_BL ;
		dGP_BH = rData.dGP_BH ;
		dGP_Tb = rData.dGP_Tb ;
		dGP_Th = rData.dGP_Th ;

		nMatlCoordKey = -1;   // MNET:XXXX-HSSHIM-20100211.  이전버전과의 호환성, Global-Z 방향의 유효성 문제로 Default를 Element local로 변경함. 
		dAngle = 0.0;
	};
	void Convert791(T_EPMT_MASONRY_791& rData)
	{
		dBR_Es = rData.dBR_Es ;
		dBR_Pr = rData.dBR_Pr ;
		dBR_Fc = rData.dBR_Fc ;
		dBR_Ft = rData.dBR_Ft ;
		dBR_H  = rData.dBR_H  ;
		dBR_FA = rData.dBR_FA ;
		dBR_Val1 = rData.dBR_Val1 ;
		dBR_Val2 = rData.dBR_Val2 ;
		dBR_Val3 = rData.dBR_Val3 ;
		dBR_Val4 = rData.dBR_Val4 ;
		dBR_Val5 = rData.dBR_Val5 ;
		dBR_Val6 = rData.dBR_Val6 ;
		dBR_Val7 = rData.dBR_Val7 ;
		dBJ_Es = rData.dBJ_Es ;
		dBJ_Pr = rData.dBJ_Pr ;
		dBJ_Fc = rData.dBJ_Fc ;
		dBJ_Ft = rData.dBJ_Ft ;
		dBJ_H  = rData.dBJ_H  ;
		dBJ_BWS = rData.dBJ_BWS ;
		dHJ_Es = rData.dHJ_Es ;
		dHJ_Pr = rData.dHJ_Pr ;
		dHJ_Fc = rData.dHJ_Fc ;
		dHJ_Ft = rData.dHJ_Ft ;
		dHJ_H = rData.dHJ_H ;
		dHJ_BWS = rData.dHJ_BWS ;
		dGP_BL = rData.dGP_BL ;
		dGP_BH = rData.dGP_BH ;
		dGP_Tb = rData.dGP_Tb ;
		dGP_Th = rData.dGP_Th ;
		
		nMatlCoordKey = rData.nMatlCoordKey;
		dAngle = 0.0;
	};
};
struct T_EPMT_D_712
{
	CString strMatlName;
	int     nModelType;
	T_EPMT_TRESCA      TRESCA;
	T_EPMT_VMISES_955  VMISES;
	T_EPMT_MOHRCL      MOHRCL;  
	T_EPMT_DRUCKER     DRUCKER;
	T_EPMT_MICROPL     MICROPL;
	unsigned int nSeq;
};
struct T_EPMT_D_770
{
	CString strMatlName;
	int     nModelType;     // 0=Tresca, 1=Von Mises, 2=Mohr-Coulomb, 3=Drucker-Prager, 4=Microplane, 5=masonry
	T_EPMT_TRESCA      TRESCA;
	T_EPMT_VMISES_955  VMISES;
	T_EPMT_MOHRCL      MOHRCL;  
	T_EPMT_DRUCKER     DRUCKER;
	T_EPMT_MICROPL     MICROPL;
	T_EPMT_MASONRY_770 MASONRY;
	unsigned int nSeq;      // 순번
};
struct T_EPMT_D_791
{
	CString strMatlName;
	int     nModelType;     // 0=Tresca, 1=Von Mises, 2=Mohr-Coulomb, 3=Drucker-Prager, 4=Microplane, 5=masonry
	T_EPMT_TRESCA      TRESCA;
	T_EPMT_VMISES_955  VMISES;
	T_EPMT_MOHRCL      MOHRCL;  
	T_EPMT_DRUCKER     DRUCKER;
	T_EPMT_MICROPL     MICROPL;
	T_EPMT_MASONRY_791 MASONRY;
	unsigned int nSeq;      // 순번
};

struct T_EPMT_CONCDMG
{
	double dDilationAng; // Dilation Angle
	double dEccentricity;
	double dfboOffco; // fbo / fco
	double dK;
	double dViscosityParm; // Viscosity Parameter
	int nCompBhNum;
	double aCmpBhInelasticStrain[50]; // Compressive Behavior > Inelastic Strain
	double aCmpBhYieldStress[50];     // Compressive Behavior > Yield Stress
	double aCmpBhDamage[50];          // Compressive Behavior > Damage
	int nTnsBhNum;
	double aTnsBhInelasticStrain[50]; // Tensile Behavior > Inelastic Strain
	double aTnsBhYieldStress[50];     // Tensile Behavior > Yield Stress
	double aTnsBhDamage[50];          // Tensile Behavior > Damage
	void Initialize()
	{
		dDilationAng = 30.0;
		dEccentricity = 0.0;
		dfboOffco = 1.16;
		dK = 0.667;
		dViscosityParm = 0.0;

		nCompBhNum = 0;
		for (int i = 0; i < 50; i++)    { aCmpBhInelasticStrain[i] = 0.0;  }
		for (int i = 0; i < 50; i++)    { aCmpBhYieldStress[i] = 0.0;  }
		for (int i = 0; i < 50; i++)    { aCmpBhDamage[i] = 0.0;  }
		nTnsBhNum = 0;
		for (int i = 0; i < 50; i++)    { aTnsBhInelasticStrain[i] = 0.0;  }
		for (int i = 0; i < 50; i++)    { aTnsBhYieldStress[i] = 0.0;  }
		for (int i = 0; i < 50; i++)    { aTnsBhDamage[i] = 0.0;  }
	}

	T_EPMT_CONCDMG& operator=(T_EPMT_CONCDMG& src)
	{
		dDilationAng = src.dDilationAng;
		dEccentricity = src.dEccentricity;         
		dfboOffco = src.dfboOffco;            
		dK = src.dK;                   
		dViscosityParm = src.dViscosityParm;       
		nCompBhNum = src.nCompBhNum;
		for (int i = 0; i < 50; i++)    { aCmpBhInelasticStrain[i] = src.aCmpBhInelasticStrain[i];  }
		for (int i = 0; i < 50; i++)    { aCmpBhYieldStress[i] = src.aCmpBhYieldStress[i];  }
		for (int i = 0; i < 50; i++)    { aCmpBhDamage[i] = src.aCmpBhDamage[i];  }
		nTnsBhNum = src.nTnsBhNum;
		for (int i = 0; i < 50; i++)    { aTnsBhInelasticStrain[i] = src.aTnsBhInelasticStrain[i];  }
		for (int i = 0; i < 50; i++)    { aTnsBhYieldStress[i] = src.aTnsBhYieldStress[i];  }
		for (int i = 0; i < 50; i++)    { aTnsBhDamage[i] = src.aTnsBhDamage[i];  }
		return *this;
	}
};

struct T_EPMT_D_955
{
	CString strMatlName;
	int  nModelType;   // 0=Tresca, 1=Von Mises, 2=Mohr-Coulomb, 3=Drucker-Prager, 4=Microplane, 5=masonry, 6=Concrete-Damage
	T_EPMT_TRESCA      TRESCA;
	T_EPMT_VMISES_955  VMISES;
	T_EPMT_MOHRCL      MOHRCL;
	T_EPMT_DRUCKER     DRUCKER;
	T_EPMT_MICROPL     MICROPL;
	T_EPMT_MASONRY     MASONRY;
	unsigned int       nSeq;
	T_EPMT_CONCDMG     CONCDMG;
};

#define D_EPMT_TRESCA          0
#define D_EPMT_VON_MISES       1
#define D_EPMT_MOHR_COULOMB    2
#define D_EPMT_DRUCKER_PRAGER  3
#define D_EPMT_MICROPLANE      4
#define D_EPMT_MASONRY         5
#define D_EPMT_CONC_DAMAGE     6

struct T_EPMT_D
{
	CString strMatlName;
	int     nModelType;     // 0=Tresca, 1=Von Mises, 2=Mohr-Coulomb, 3=Drucker-Prager, 4=Microplane, 5=masonry, 6=Concrete-Damage
	T_EPMT_TRESCA  TRESCA;
	T_EPMT_VMISES  VMISES;
	T_EPMT_MOHRCL  MOHRCL;  
	T_EPMT_DRUCKER DRUCKER;
	T_EPMT_MICROPL MICROPL;
	T_EPMT_MASONRY MASONRY; // v720 추가  
	T_EPMT_CONCDMG CONCDMG; // v856 추가(Concrete-Damage)
	unsigned int   nSeq;      // 순번

	void Initialize() // CAttrCtrl의 GetDefaultEpmt()를 사용할 것
	{
		strMatlName.Empty();
		nModelType = 0;
		TRESCA.Initialize();
		VMISES.Initialize();
		MOHRCL.Initialize();
		DRUCKER.Initialize();
		MICROPL.Initialize();
		nSeq = 0;

		MASONRY.Initialize();
		CONCDMG.Initialize();
	}
	T_EPMT_D()
	{
	}
	T_EPMT_D(T_EPMT_D& src)
	{
		*this = src;
	}
	T_EPMT_D& operator=(T_EPMT_D& src)
	{
		strMatlName= src.strMatlName;
		nModelType = src.nModelType;
		TRESCA  = src.TRESCA;
		VMISES  = src.VMISES; 
		MOHRCL  = src.MOHRCL; 
		DRUCKER = src.DRUCKER;
		MICROPL = src.MICROPL;
		MASONRY = src.MASONRY;    
		nSeq    = src.nSeq;   
		CONCDMG = src.CONCDMG;
		return *this;
	}  
};
struct T_EPMT_D_CH_712
{
	char strMatlName[20];
	int  nModelType;
	T_EPMT_TRESCA      TRESCA;
	T_EPMT_VMISES_955  VMISES;
	T_EPMT_MOHRCL      MOHRCL;  
	T_EPMT_DRUCKER     DRUCKER;
	T_EPMT_MICROPL     MICROPL;  
	unsigned int       nSeq;
};
struct T_EPMT_D_CH_770
{
	char strMatlName[20];
	int  nModelType;   // 0=Tresca, 1=Von Mises, 2=Mohr-Coulomb, 3=Drucker-Prager, 4=Microplane, 5=masonry
	T_EPMT_TRESCA      TRESCA;
	T_EPMT_VMISES_955  VMISES;
	T_EPMT_MOHRCL      MOHRCL;  
	T_EPMT_DRUCKER     DRUCKER;
	T_EPMT_MICROPL     MICROPL;
	T_EPMT_MASONRY_770 MASONRY;
	unsigned int   nSeq;
};
struct T_EPMT_D_CH_791
{
	char strMatlName[20];
	int  nModelType;   // 0=Tresca, 1=Von Mises, 2=Mohr-Coulomb, 3=Drucker-Prager, 4=Microplane, 5=masonry
	T_EPMT_TRESCA      TRESCA;
	T_EPMT_VMISES_955  VMISES;
	T_EPMT_MOHRCL      MOHRCL;  
	T_EPMT_DRUCKER     DRUCKER;
	T_EPMT_MICROPL     MICROPL;
	T_EPMT_MASONRY_791 MASONRY;
	unsigned int   nSeq;
};

struct T_EPMT_D_CH_860
{
	char strMatlName[20];
	int  nModelType;   // 0=Tresca, 1=Von Mises, 2=Mohr-Coulomb, 3=Drucker-Prager, 4=Microplane, 5=masonry
	T_EPMT_TRESCA      TRESCA;
	T_EPMT_VMISES_955  VMISES;
	T_EPMT_MOHRCL      MOHRCL;  
	T_EPMT_DRUCKER     DRUCKER;
	T_EPMT_MICROPL     MICROPL;
	T_EPMT_MASONRY     MASONRY;
	unsigned int   nSeq;
};

struct T_EPMT_D_CH_955
{
	char strMatlName[20];
	int  nModelType;   // 0=Tresca, 1=Von Mises, 2=Mohr-Coulomb, 3=Drucker-Prager, 4=Microplane, 5=masonry, 6=Concrete-Damage
	T_EPMT_TRESCA      TRESCA;
	T_EPMT_VMISES_955  VMISES;
	T_EPMT_MOHRCL      MOHRCL;
	T_EPMT_DRUCKER     DRUCKER;
	T_EPMT_MICROPL     MICROPL;
	T_EPMT_MASONRY     MASONRY;
	unsigned int       nSeq;
	T_EPMT_CONCDMG     CONCDMG;
};

struct T_EPMT_D_CH
{
	char strMatlName[20];
	int  nModelType;   // 0=Tresca, 1=Von Mises, 2=Mohr-Coulomb, 3=Drucker-Prager, 4=Microplane, 5=masonry, 6=Concrete-Damage
	T_EPMT_TRESCA  TRESCA;
	T_EPMT_VMISES  VMISES;
	T_EPMT_MOHRCL  MOHRCL;
	T_EPMT_DRUCKER DRUCKER;
	T_EPMT_MICROPL MICROPL;
	T_EPMT_MASONRY MASONRY;
	T_EPMT_CONCDMG CONCDMG;
	unsigned int   nSeq;
	void ConvertToString(T_EPMT_D& rData)
	{
		ConvertCharStr(strMatlName, rData.strMatlName, sizeof(strMatlName));
		rData.nModelType = nModelType;
		rData.TRESCA  = TRESCA; 
		rData.VMISES  = VMISES; 
		rData.MOHRCL  = MOHRCL; 
		rData.DRUCKER = DRUCKER;
		rData.MICROPL = MICROPL;
		rData.MASONRY = MASONRY;
		rData.nSeq    = nSeq;
		rData.CONCDMG = CONCDMG;
	}
	void ConvertToChar(T_EPMT_D& rData)
	{
		ConvertStrChar(rData.strMatlName, strMatlName, sizeof(strMatlName));
		nModelType = rData.nModelType;
		TRESCA  = rData.TRESCA;     
		VMISES  = rData.VMISES; 
		MOHRCL  = rData.MOHRCL; 
		DRUCKER = rData.DRUCKER;
		MICROPL = rData.MICROPL;
		MASONRY = rData.MASONRY;
		nSeq    = rData.nSeq;
		CONCDMG = rData.CONCDMG;
	}
	void Convert712(T_EPMT_D_CH_712& rData)
	{
		memcpy(strMatlName, rData.strMatlName, sizeof(strMatlName));
		nModelType = rData.nModelType;
		TRESCA  = rData.TRESCA;
		VMISES.Convert955(rData.VMISES);
		MOHRCL  = rData.MOHRCL; 
		DRUCKER = rData.DRUCKER;
		MICROPL = rData.MICROPL;
		nSeq    = rData.nSeq;

		MASONRY.Initialize();
		CONCDMG.Initialize();
	}
	void Convert770(T_EPMT_D_CH_770& rData)
	{
		memcpy(strMatlName, rData.strMatlName, sizeof(strMatlName));
		nModelType = rData.nModelType;
		TRESCA  = rData.TRESCA;     
		VMISES.Convert955(rData.VMISES);
		MOHRCL  = rData.MOHRCL; 
		DRUCKER = rData.DRUCKER;
		MICROPL = rData.MICROPL;
		MASONRY.Convert770(rData.MASONRY);
		nSeq    = rData.nSeq;
		CONCDMG.Initialize();
	}
	void Convert791(T_EPMT_D_CH_791& rData)
	{
		memcpy(strMatlName, rData.strMatlName, sizeof(strMatlName));
		nModelType = rData.nModelType;
		TRESCA  = rData.TRESCA;     
		VMISES.Convert955(rData.VMISES);
		MOHRCL  = rData.MOHRCL; 
		DRUCKER = rData.DRUCKER;
		MICROPL = rData.MICROPL;
		MASONRY.Convert791(rData.MASONRY);
		nSeq    = rData.nSeq;
		CONCDMG.Initialize();
	}
	void Convert860(T_EPMT_D_CH_860& rData)
	{
		memcpy(strMatlName, rData.strMatlName, sizeof(strMatlName));
		nModelType = rData.nModelType;
		TRESCA  = rData.TRESCA;     
		VMISES.Convert955(rData.VMISES);
		MOHRCL  = rData.MOHRCL; 
		DRUCKER = rData.DRUCKER;
		MICROPL = rData.MICROPL;
		MASONRY = rData.MASONRY;
		nSeq    = rData.nSeq;
		CONCDMG.Initialize();
	}
	void Convert955(T_EPMT_D_CH_955& rData)
	{
		memcpy(strMatlName, rData.strMatlName, sizeof(strMatlName));
		nModelType = rData.nModelType;
		TRESCA = rData.TRESCA;
		VMISES.Convert955(rData.VMISES);
		MOHRCL = rData.MOHRCL;
		DRUCKER = rData.DRUCKER;
		MICROPL = rData.MICROPL;
		MASONRY = rData.MASONRY;
		CONCDMG = rData.CONCDMG;
		nSeq = rData.nSeq;
	}
};
struct T_EPMT_UDRD_D_712
{
	T_EPMT_K key;
	T_EPMT_D_712 data;
};
struct T_EPMT_UDRD_D_770
{
	T_EPMT_K key;
	T_EPMT_D_770 data;
};
struct T_EPMT_UDRD_D_791
{
	T_EPMT_K key;
	T_EPMT_D_791 data;
};
struct T_EPMT_UDRD_D_955
{
	T_EPMT_K key;
	T_EPMT_D_955 data;
};
struct T_EPMT_UDRD_D
{
	T_EPMT_K key;
	T_EPMT_D data;
};
struct T_EPMT_UDRD_D_CH_712
{
	T_EPMT_K key;
	T_EPMT_D_CH_712 data;
};
struct T_EPMT_UDRD_D_CH_770
{
	T_EPMT_K key;
	T_EPMT_D_CH_770 data;
};
struct T_EPMT_UDRD_D_CH_791
{
	T_EPMT_K key;
	T_EPMT_D_CH_791 data;
};
struct T_EPMT_UDRD_D_CH_860
{
	T_EPMT_K key;
	T_EPMT_D_CH_860 data;
};
struct T_EPMT_UDRD_D_CH_955
{
	T_EPMT_K key;
	T_EPMT_D_CH_955 data;
};
struct T_EPMT_UDRD_D_CH
{
	T_EPMT_K key;
	T_EPMT_D_CH data;
};
struct T_EPMT_UNIT
{
	T_EPMT_UNIT()
	{
		dInitYieldStress = D_UNITSYS_BASE_STRESS;
		dInitCohesion    = D_UNITSYS_BASE_STRESS;
		dHardeningCoef   = D_UNITSYS_BASE_STRESS;
		dEs              = D_UNITSYS_BASE_ELAST;
		dFc              = D_UNITSYS_BASE_STRESS;
		dFt              = D_UNITSYS_BASE_STRESS;
		dFA              = D_UNITSYS_BASE_DEGREE;
		dBWS             = D_UNITSYS_BASE_STRESS;
		dBL              = D_UNITSYS_BASE_LENGTH;
		dBH              = D_UNITSYS_BASE_LENGTH;
		dTb              = D_UNITSYS_BASE_LENGTH;
		dTh              = D_UNITSYS_BASE_LENGTH;
		dYieldStress     = D_UNITSYS_BASE_STRESS;
	}
	int dInitYieldStress;
	int dInitCohesion;
	int dHardeningCoef;
	int dEs;
	int dFc;
	int dFt;
	int dFA;
	int dBWS;
	int dBL;
	int dBH;
	int dTb;
	int dTh;
	int dYieldStress;
};

#define HASHSIZEEPMT 11

#define T_MNFC_K unsigned int
enum { D_MNFC_SIZE_AUTO, D_MNFC_SIZE_EQUAL };
struct T_MNFC_D
{
	int    nSectNum;      // Number of Sections (as fiber hinge
	int    nFibrAreaSize; // Fiber Areas size control, 0:Auto Size 1: Equal
	int    nBeamDivNumNy; // Number of divisions(Beam-Column) - Ny, Core
	int    nBeamDivNumNz; // Number of divisions(Beam-Column) - Nz, Core

	void Initialize()
	{
		nSectNum = 3;
		nFibrAreaSize = D_MNFC_SIZE_EQUAL;
		nBeamDivNumNy = 15;
		nBeamDivNumNz = 15;
	}
};
struct T_MNFC_UDRD_D
{
	T_MNFC_D data;
};
struct T_MNFC_UNIT
{
	T_MNFC_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;
	}
	int nNoUnit;
};

#define HASHSIZEMNFC 1

#pragma pack(pop)

#endif  // __DB_ST_DT_NONL_H__
