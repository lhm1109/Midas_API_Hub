#ifndef __DB_ST_DT_BNDR_H__
#define __DB_ST_DT_BNDR_H__

#pragma pack(push, 1)

// General Link Property
#define T_NLLP_K unsigned int

struct T_NLLP_VD_630
{
	double dDamperStiffness; 
	double dDamping;
	double dDampExp;
	double dBracingStiffness; 
	BOOL   bRigidBracing;
	double dEffectiveDamping;
};
struct T_NLLP_VD_632
{
	double dDamperStiffness; 
	double dDamping;
	double dDampExp;
	double dBracingStiffness; 
	BOOL   bRigidBracing;
	double dEffectiveDamping;
	double dRefVelocity;      // 6.3.1에서 추가
};
struct T_NLLP_VD
{
	double dDamperStiffness; 
	double dDamping;
	double dDampExp;
	double dBracingStiffness; 
	BOOL   bRigidBracing;  
//  double dEffectiveDamping;  // 6.4.0부터 삭제
	double dRefVelocity;      // 6.3.1에서 추가
	double dTemp1;            // 6.4.0 추가 : Size 맞추기 위한 임시변수
	double dTemp2;            // 6.4.0 추가 : Size 맞추기 위한 임시변수 
	void Initialize()
	{
		dDamperStiffness = 0.0;
		dDamping = 0.0;
		dDampExp = 1.0;
		dBracingStiffness = 0.0;
		bRigidBracing = FALSE;    
		dRefVelocity = 1.0;
		dTemp1 = 0.;
		dTemp2 = 0.;
	}
	void Convert630(const T_NLLP_VD_630& rData)
	{
		dDamperStiffness  = rData.dDamperStiffness;
		dDamping          = rData.dDamping;
		dDampExp          = rData.dDampExp;
		dBracingStiffness = rData.dBracingStiffness;
		bRigidBracing     = rData.bRigidBracing;    
		dRefVelocity      = 1.0;
		dTemp1 = 0.;
		dTemp2 = 0.;
	}
	void Convert632(const T_NLLP_VD_632& rData)
	{
		dDamperStiffness  = rData.dDamperStiffness;
		dDamping          = rData.dDamping;
		dDampExp          = rData.dDampExp;
		dBracingStiffness = rData.dBracingStiffness;
		bRigidBracing     = rData.bRigidBracing;    
		dRefVelocity      = rData.dRefVelocity;
		dTemp1 = 0.;
		dTemp2 = 0.;
	}
};
struct T_NLLP_GAP_632
{
	double dStiffness;
	double dOpen;
	double dEffectiveDamping;  
};
struct T_NLLP_GAP
{
	double dStiffness;
	double dOpen;  
//  double dEffectiveDamping;  // 6.4.0부터 삭제
	void Initialize()
	{
		dStiffness = 0.0;
		dOpen = 0.0;    
	}
	void Convert632(const T_NLLP_GAP_632& rData)
	{
		dStiffness = rData.dStiffness;
		dOpen = rData.dOpen;
	}
};
struct T_NLLP_HOOK_632
{
	double dStiffness;
	double dOpen;
	double dEffectiveDamping;
};
struct T_NLLP_HOOK
{
	double dStiffness;
	double dOpen;
//  double dEffectiveDamping;  // 6.4.0부터 삭제
	void Initialize()
	{
		dStiffness = 0.0;
		dOpen = 0.0;    
	}
	void Convert632(const T_NLLP_HOOK_632& rData)
	{
		dStiffness = rData.dStiffness;
		dOpen = rData.dOpen;
	}
};
struct T_NLLP_HS_632
{
	double dStiffness;
	double dYieldStrength;
	double dPostYieldStiffRatio;
	double dYieldingExp;
	double dLoopParamA; // auto calc (1.0 - |b|)
	double dLoopParamB;
	double dEffectiveDamping;  // 6.4.0부터 삭제
};
struct T_NLLP_HS
{
	double dStiffness;
	double dYieldStrength;
	double dPostYieldStiffRatio;
	double dYieldingExp;
	double dLoopParamA; // auto calc (1.0 - |b|)
	double dLoopParamB;
//  double dEffectiveDamping;  // 6.4.0부터 삭제
	void Initialize()
	{
		dStiffness = 0.0;
		dYieldStrength = 0.0;
		dPostYieldStiffRatio = 0.1;
		dYieldingExp = 2.0;
		dLoopParamA = 0.5;
		dLoopParamB = 0.5;    
	}
	void Convert632(const T_NLLP_HS_632& rData)
	{
		dStiffness = rData.dStiffness;
		dYieldStrength = rData.dYieldStrength;
		dPostYieldStiffRatio = rData.dPostYieldStiffRatio;
		dYieldingExp = rData.dYieldingExp;
		dLoopParamA  = rData.dLoopParamA ;
		dLoopParamB  = rData.dLoopParamB ;    
	}
};
struct T_NLLP_LRBI_632
{
	double dStiffness;
	double dYieldStrength;
	double dPostYieldStiffRatio;
	double dLoopParamA; // auto calc (1.0 - |b|)
	double dLoopParamB;
	double dEffectiveDamping;  // 6.4.0부터 삭제
};
struct T_NLLP_LRBI
{
	double dStiffness;
	double dYieldStrength;
	double dPostYieldStiffRatio;
	double dLoopParamA; // auto calc (1.0 - |b|)
	double dLoopParamB;
//  double dEffectiveDamping;  // 6.4.0부터 삭제
	void Initialize()
	{
		dStiffness = 0.0;
		dYieldStrength = 0.0;
		dPostYieldStiffRatio = 0.1;
		dLoopParamA = 0.5;
		dLoopParamB = 0.5;    
	}
	void Convert632(const T_NLLP_LRBI_632& rData)
	{
		dStiffness = rData.dStiffness;
		dYieldStrength = rData.dYieldStrength;
		dPostYieldStiffRatio = rData.dPostYieldStiffRatio;    
		dLoopParamA  = rData.dLoopParamA ;
		dLoopParamB  = rData.dLoopParamB ;    
	}
};
struct T_NLLP_FPSI_632
{
	double dStiffness;
	double dFricCoeffSlow;
	double dFricCoeffFast;
	double dRateParam;
	double dRadius;
	double dLoopParamA;  // auto calc (1.0 - |b|)
	double dLoopParamB;
	double dEffectiveDamping;  // 6.4.0부터 삭제
};
struct T_NLLP_FPSI
{
	double dStiffness;
	double dFricCoeffSlow;
	double dFricCoeffFast;
	double dRateParam;
	double dRadius;
	double dLoopParamA;  // auto calc (1.0 - |b|)
	double dLoopParamB;
//  double dEffectiveDamping;  // 6.4.0부터 삭제
	void Initialize()
	{
		dStiffness = 0.0;
		dFricCoeffSlow = 0.0;
		dFricCoeffFast = 0.0;
		dRateParam = 0.0;
		dRadius = 0.0;
		dLoopParamA = 0.5;
		dLoopParamB = 0.5;    
	}
	void Convert632(const T_NLLP_FPSI_632& rData)
	{
		dStiffness = rData.dStiffness;
		dFricCoeffSlow = rData.dFricCoeffSlow;
		dFricCoeffFast = rData.dFricCoeffFast;    
		dRateParam   = rData.dRateParam ;
		dRadius      = rData.dRadius ;    
		dLoopParamA  = rData.dLoopParamA ;
		dLoopParamB  = rData.dLoopParamB ;    
	}
};
union T_NLLP_PROP_630
{
	T_NLLP_VD_630   VD;
	T_NLLP_GAP_632  GAP;
	T_NLLP_HOOK_632 HOOK;
	T_NLLP_HS_632   HS;
	T_NLLP_LRBI_632 LRBI;
	T_NLLP_FPSI_632 FPSI;
};
union T_NLLP_PROP_632
{
	T_NLLP_VD_632   VD;     // Visco-elastic Damper
	T_NLLP_GAP_632  GAP;    // Gap
	T_NLLP_HOOK_632 HOOK;   // Hook
	T_NLLP_HS_632   HS;     // Hysteretic System
	T_NLLP_LRBI_632 LRBI;   // Lead Rubber Bearing Isolator
	T_NLLP_FPSI_632 FPSI;   // Friction Pendulum System Isolator
};
union T_NLLP_PROP
{
	T_NLLP_VD   VD;     // Visco-elastic Damper
	T_NLLP_GAP  GAP;    // Gap
	T_NLLP_HOOK HOOK;   // Hook
	T_NLLP_HS   HS;     // Hysteretic System
	T_NLLP_LRBI LRBI;   // Lead Rubber Bearing Isolator
	T_NLLP_FPSI FPSI;   // Friction Pendulum System Isolator
	void Convert630(const T_NLLP_PROP_630& rData, int nType, int ix)
	{
		switch(nType)
		{
		case 0: VD.Convert630(rData.VD); break;
		case 1: GAP   = rData.GAP; break;
		case 2: HOOK  = rData.HOOK; break;
		case 3: HS    = rData.HS; break;
		case 4: 
			if (ix == 0 || ix == 3 || ix == 4 || ix == 5) GAP = rData.GAP;
			else LRBI  = rData.LRBI; 
			break;
		case 5: 
			if (ix == 0 || ix == 3 || ix == 4 || ix == 5) GAP = rData.GAP;
			else FPSI  = rData.FPSI; 
			break;
		}
	}
	void Convert632(const T_NLLP_PROP_632& rData, int nType, int ix)
	{
		switch(nType)
		{
		case 0: VD.Convert632(rData.VD); break;
		case 1: GAP.Convert632(rData.GAP); break;
		case 2: HOOK.Convert632(rData.HOOK); break;
		case 3: HS.Convert632(rData.HS); break;
		case 4: 
			if (ix == 0 || ix == 3 || ix == 4 || ix == 5) GAP.Convert632(rData.GAP);
			else LRBI.Convert632(rData.LRBI); 
			break;
		case 5: 
			if (ix == 0 || ix == 3 || ix == 4 || ix == 5) GAP.Convert632(rData.GAP);
			else FPSI.Convert632(rData.FPSI);
			break;
		}
	}
};

struct T_NLLP_D
{
	CString     PropName;
	int         nType;            // 0:Visco, 1:Gap, 2:Hook, 3:Hysteretic, 4:Lead Rubber, 5:Friction Pendulum
	char        LDP_cDir[8];      // '0':꺼짐, '1':켜짐 : 6개만 사용(dx, dy, dz, rx, ry, rz)
	double      LDP_dEffStiff[6];
	char        NDP_cDir[8];      // '0':꺼짐, '1':켜짐 : 6개만 사용(dx, dy, dz, rx, ry, rz)
	T_NLLP_PROP NDP_Prop[6];
	BOOL        bShearSprLoc;
	double      dDistRatioU2;
	double      dDistRatioU3;
	double      dTotalWeight;
	double      dTotalMass;      // 6.0.0 추가
	BOOL        bUseMass;        // 6.0.0 추가
	CString     Description;
	unsigned int nSeq;    // 순번
	int         nApplicationType;   // 6.4.0추가:  0=Element, 1=Force
	int         nElemType;          // 6.4.0추가:  0=SPG(Spring), 1=DSP(Linear Dashpot), 2=SLD(Spring and Linear Dashpot)
																	//             3=SND1(Spring and Nonlinear Dashpot2), 4=SND1(Spring and Nonlinear Dashpot2) 
	double      LDP_dEffDamping[6]; // 6.4.0추가:
	void Initialize()
	{
		PropName.Empty();
		nApplicationType = 1;
		nType = 0;
		nElemType = 0;
		for (int i = 0; i < 8; i++)
			LDP_cDir[i] = NDP_cDir[i] = '0';
		for (i = 0; i < 6; i++) 
		{
			LDP_dEffStiff[i] = 0.0;
			LDP_dEffDamping[i] = 0.0;
			NDP_Prop[i].VD.Initialize();
		}
		bShearSprLoc = FALSE;
		dDistRatioU2 = 0.5;
		dDistRatioU3 = 0.5;
		dTotalWeight = 0.0;
		dTotalMass = 0.0;
		bUseMass = FALSE;
		Description.Empty();
		nSeq=0;
	}
};
struct T_NLLP_D_CH_590
{
	char        PropName[20];
	int         nType;            // 0:Visco, 1:Gap, 2:Hook, 3:Hysteretic, 4:Lead Rubber, 5:Friction Pendulum
	char        LDP_cDir[8];      // '0':꺼짐, '1':켜짐 : 6개만 사용(dx, dy, dz, rx, ry, rz)
	double      LDP_dEffStiff[6];
	char        NDP_cDir[8];      // '0':꺼짐, '1':켜짐 : 6개만 사용(dx, dy, dz, rx, ry, rz)
	T_NLLP_PROP_630 NDP_Prop[6];
	BOOL        bShearSprLoc;
	double      dDistRatioU2;
	double      dDistRatioU3;
	double      dTotalWeight;
	char        Description[80];
	unsigned int nSeq;    // 순번
};
struct T_NLLP_D_CH_630
{
	char        PropName[20];
	int         nType;            // 0:Visco, 1:Gap, 2:Hook, 3:Hysteretic, 4:Lead Rubber, 5:Friction Pendulum
	char        LDP_cDir[8];      // '0':꺼짐, '1':켜짐 : 6개만 사용(dx, dy, dz, rx, ry, rz)
	double      LDP_dEffStiff[6];
	char        NDP_cDir[8];      // '0':꺼짐, '1':켜짐 : 6개만 사용(dx, dy, dz, rx, ry, rz)
	T_NLLP_PROP_630 NDP_Prop[6];
	BOOL        bShearSprLoc;
	double      dDistRatioU2;
	double      dDistRatioU3;
	double      dTotalWeight;  
	double      dTotalMass;      // 6.0.0 추가
	BOOL        bUseMass;        // 6.0.0 추가  
	char        Description[80];
	unsigned int nSeq;    // 순번
};
struct T_NLLP_D_CH_632
{
	char        PropName[20];
	int         nType;
	char        LDP_cDir[8];
	double      LDP_dEffStiff[6];
	char        NDP_cDir[8];
	T_NLLP_PROP_632 NDP_Prop[6];
	BOOL        bShearSprLoc;
	double      dDistRatioU2;
	double      dDistRatioU3;
	double      dTotalWeight;  
	double      dTotalMass;       // 6.0.0 추가
	BOOL        bUseMass;         // 6.0.0 추가
	char        Description[80];
	unsigned int nSeq;
	int         n631Mark;         // 6.3.1에서 추가(Size 구분용)
};
struct T_NLLP_D_CH
{
	char        PropName[20];
	int         nType;
	char        LDP_cDir[8];
	double      LDP_dEffStiff[6];
	char        NDP_cDir[8];
	T_NLLP_PROP NDP_Prop[6];
	BOOL        bShearSprLoc;
	double      dDistRatioU2;
	double      dDistRatioU3;
	double      dTotalWeight;  
	double      dTotalMass;       // 6.0.0 추가
	BOOL        bUseMass;         // 6.0.0 추가
	char        Description[80];
	unsigned int nSeq;
	int         n631Mark;         // 6.3.1에서 추가(Size 구분용)
	int         nApplicationType;   // 6.4.0추가:  0=Element, 1=Force
	int         nElemType;          // 6.4.0추가:  0=SPG(Spring), 1=DSP(Linear Dashpot), 2=SLD(Spring and Linear Dashpot)
																	//             3=SND1(Spring and Nonlinear Dashpot2), 4=SND1(Spring and Nonlinear Dashpot2) 
	double      LDP_dEffDamping[6]; // 6.4.0추가:
	void ConvertToString(T_NLLP_D& rData)
	{
		ConvertCharStr(PropName, rData.PropName, sizeof(PropName));
		rData.nType = nType;
		memcpy(rData.LDP_cDir, LDP_cDir, sizeof(LDP_cDir));
		memcpy(rData.LDP_dEffStiff, LDP_dEffStiff, sizeof(LDP_dEffStiff));
		memcpy(rData.NDP_cDir, NDP_cDir, sizeof(NDP_cDir));
		memcpy(rData.NDP_Prop, NDP_Prop, sizeof(NDP_Prop));
		rData.bShearSprLoc = bShearSprLoc;
		rData.dDistRatioU2 = dDistRatioU2;
		rData.dDistRatioU3 = dDistRatioU3;
		rData.dTotalWeight = dTotalWeight;
		rData.dTotalMass   = dTotalMass;
		rData.bUseMass = bUseMass;
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.nSeq=nSeq;
		rData.nApplicationType = nApplicationType;
		rData.nElemType = nElemType;
		memcpy(rData.LDP_dEffDamping, LDP_dEffDamping, sizeof(LDP_dEffDamping));
	}
	void ConvertToChar(T_NLLP_D& rData)
	{
		ConvertStrChar(rData.PropName, PropName, sizeof(PropName));
		nType = rData.nType;
		memcpy(LDP_cDir, rData.LDP_cDir, sizeof(LDP_cDir));
		memcpy(LDP_dEffStiff, rData.LDP_dEffStiff, sizeof(LDP_dEffStiff));
		memcpy(NDP_cDir, rData.NDP_cDir, sizeof(NDP_cDir));
		memcpy(NDP_Prop, rData.NDP_Prop, sizeof(NDP_Prop));
		bShearSprLoc = rData.bShearSprLoc;
		dDistRatioU2 = rData.dDistRatioU2;
		dDistRatioU3 = rData.dDistRatioU3;
		dTotalWeight = rData.dTotalWeight;
		dTotalMass   = rData.dTotalMass;
		bUseMass = rData.bUseMass;
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		nSeq=rData.nSeq;
		n631Mark = 0;
		nApplicationType = rData.nApplicationType;
		nElemType = rData.nElemType;
		memcpy(LDP_dEffDamping, rData.LDP_dEffDamping, sizeof(LDP_dEffDamping));
	}
	void Convert590(T_NLLP_D_CH_590& rData)
	{
		memcpy(PropName, rData.PropName, sizeof(PropName));    
		nType = rData.nType;
		memcpy(LDP_cDir, rData.LDP_cDir, sizeof(LDP_cDir));
		memcpy(LDP_dEffStiff, rData.LDP_dEffStiff, sizeof(LDP_dEffStiff));
		memcpy(NDP_cDir, rData.NDP_cDir, sizeof(NDP_cDir));
		for (int i = 0; i < 6; i++)
			NDP_Prop[i].Convert630(rData.NDP_Prop[i], nType, i);
		bShearSprLoc = rData.bShearSprLoc;
		dDistRatioU2 = rData.dDistRatioU2;
		dDistRatioU3 = rData.dDistRatioU3;
		dTotalWeight = rData.dTotalWeight;
		dTotalMass   = 0.;
		bUseMass = FALSE;
		memcpy(Description, rData.Description, sizeof(Description));
		nSeq=rData.nSeq;
		nApplicationType = 1;
		nElemType = 0;
		for(i = 0; i < 6; i++) LDP_dEffDamping[i] = 0.;
	}
	void Convert630(T_NLLP_D_CH_630& rData)
	{
		memcpy(PropName, rData.PropName, sizeof(PropName));    
		nType = rData.nType;
		memcpy(LDP_cDir, rData.LDP_cDir, sizeof(LDP_cDir));
		memcpy(LDP_dEffStiff, rData.LDP_dEffStiff, sizeof(LDP_dEffStiff));
		memcpy(NDP_cDir, rData.NDP_cDir, sizeof(NDP_cDir));
		for (int i = 0; i < 6; i++)
			NDP_Prop[i].Convert630(rData.NDP_Prop[i], nType, i);
		bShearSprLoc = rData.bShearSprLoc;
		dDistRatioU2 = rData.dDistRatioU2;
		dDistRatioU3 = rData.dDistRatioU3;
		dTotalWeight = rData.dTotalWeight;
		dTotalMass   = rData.dTotalMass;
		bUseMass     = rData.bUseMass;
		memcpy(Description, rData.Description, sizeof(Description));
		nSeq=rData.nSeq;
		nApplicationType = 1;
		nElemType = 0;
		for(i = 0; i < 6; i++) LDP_dEffDamping[i] = 0.;
	}
	void Convert632(T_NLLP_D_CH_632& rData)
	{
		memcpy(PropName, rData.PropName, sizeof(PropName));    
		nType = rData.nType;
		memcpy(LDP_cDir, rData.LDP_cDir, sizeof(LDP_cDir));
		memcpy(LDP_dEffStiff, rData.LDP_dEffStiff, sizeof(LDP_dEffStiff));
		memcpy(NDP_cDir, rData.NDP_cDir, sizeof(NDP_cDir));
		for (int i = 0; i < 6; i++)
			NDP_Prop[i].Convert632(rData.NDP_Prop[i], nType, i);
		bShearSprLoc = rData.bShearSprLoc;
		dDistRatioU2 = rData.dDistRatioU2;
		dDistRatioU3 = rData.dDistRatioU3;
		dTotalWeight = rData.dTotalWeight;
		dTotalMass   = rData.dTotalMass;
		bUseMass     = rData.bUseMass;
		memcpy(Description, rData.Description, sizeof(Description));
		nSeq=rData.nSeq; 
		nApplicationType = 1;
		nElemType = 0;
		for(i = 0; i < 6; i++) LDP_dEffDamping[i] = 0.;
	}
};
struct T_NLLP_UDRD_D
{
	T_NLLP_K key;
	T_NLLP_D data;
};
struct T_NLLP_UDRD_D_CH_590
{
	T_NLLP_K key;
	T_NLLP_D_CH_590 data;
};
struct T_NLLP_UDRD_D_CH_630
{
	T_NLLP_K key;
	T_NLLP_D_CH_630 data;
};
struct T_NLLP_UDRD_D_CH_632
{
	T_NLLP_K key;
	T_NLLP_D_CH_632 data;
};
struct T_NLLP_UDRD_D_CH
{
	T_NLLP_K key;
	T_NLLP_D_CH data;
};
struct T_NLLP_UNIT
{
	T_NLLP_UNIT()
	{
		LDP_dEffStiff[0] = D_UNITSYS_BASE_SPRING;
		LDP_dEffStiff[1] = D_UNITSYS_BASE_SPRING;
		LDP_dEffStiff[2] = D_UNITSYS_BASE_SPRING;
		LDP_dEffStiff[3] = D_UNITSYS_BASE_ROSPRING;
		LDP_dEffStiff[4] = D_UNITSYS_BASE_ROSPRING;
		LDP_dEffStiff[5] = D_UNITSYS_BASE_ROSPRING;
		LDP_dEffDamping[0] = D_UNITSYS_BASE_FSEC_L;
		LDP_dEffDamping[1] = D_UNITSYS_BASE_FSEC_L;
		LDP_dEffDamping[2] = D_UNITSYS_BASE_FSEC_L;
		LDP_dEffDamping[3] = D_UNITSYS_BASE_FLSEC_R;
		LDP_dEffDamping[4] = D_UNITSYS_BASE_FLSEC_R;
		LDP_dEffDamping[5] = D_UNITSYS_BASE_FLSEC_R;
		dTotalWeight = D_UNITSYS_BASE_WEIGHT;
		dTotalMass = D_UNITSYS_BASE_MASS;
		dStiffnessD = D_UNITSYS_BASE_SPRING;
		dStiffnessR = D_UNITSYS_BASE_ROSPRING;
		//dDampingD = D_UNITSYS_BASE_FSEC_L;
		//dDampingR = D_UNITSYS_BASE_FLSEC_R;
		dDampingD = D_UNITSYS_BASE_FORCE;     // 6.3.1에서 변경
		dDampingR = D_UNITSYS_BASE_MOMENT;    // 6.3.1에서 변경
		dOpenD = D_UNITSYS_BASE_LENGTH;
		dOpenR = D_UNITSYS_BASE_RADIAN;
		dYieldStrengthD = D_UNITSYS_BASE_FORCE;
		dYieldStrengthR = D_UNITSYS_BASE_MOMENT;
		dRadius = D_UNITSYS_BASE_LENGTH;
//    dEffectiveDampingD = D_UNITSYS_BASE_FSEC_L;      // 6.4.0부터 사용 않음
//    dEffectiveDampingR = D_UNITSYS_BASE_FLSEC_R;     // 6.4.0부터 사용 않음
		dRefVelocityD      = D_UNITSYS_BASE_VELOCITY;
		dRefVelocityR      = D_UNITSYS_BASE_FREQUENCY;
		dRateParam         = D_UNITSYS_BASE_SEC_L;
	}
	int LDP_dEffStiff[6];
	int LDP_dEffDamping[6];
	int dTotalWeight;
	int dTotalMass;
	int dStiffnessD;
	int dStiffnessR;
	int dDampingD;
	int dDampingR;
	int dOpenD;
	int dOpenR;
	int dYieldStrengthD;
	int dYieldStrengthR;
	int dRadius;
//  int dEffectiveDampingD;
//  int dEffectiveDampingR;
	int dRefVelocityD;
	int dRefVelocityR;
	int dRateParam;
};
#define HASHSIZENLLP 11

// General Link
#define T_NLNK_K unsigned int
struct T_NLNK_D
{
	T_NODE_K Node1;
	T_NODE_K Node2;
	T_NLLP_K PropKey;
	double dBetaAngle;
	T_BNGR_K GroupKey;
	void Initialize()
	{
		Node1=0;
		Node2=0;
		PropKey=0;
		dBetaAngle=0.0;
		GroupKey = 0;
	}
};
struct T_NLNK_UDRD_D
{
	T_NLNK_K key;
	T_NLNK_D data;
};
struct T_NLNK_UNIT
{
	T_NLNK_UNIT()
	{
		dBetaAngle=D_UNITSYS_BASE_DEGREE;
	}
	int dBetaAngle;
};
#define HASHSIZENLNK 1001

#pragma pack(pop)

#endif  // __DB_ST_DT_BNDR_H__
