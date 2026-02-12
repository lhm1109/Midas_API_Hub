#ifndef __DB_ST_DT_TOWER_H__
#define __DB_ST_DT_TOWER_H__

#define MIDAS_SDK

#pragma pack(push, 1)

// MTBG : Member Type by Group (1개만 입력 가능)
#define T_MTBG_K unsigned int // always 1
struct T_MTBG_D
{
	T_GRUP_K MainPost;      // 주주재
	T_GRUP_K MainBracing;   // 복재
	T_GRUP_K ArmPost;       // 암주재
	T_GRUP_K ArmDiagonal;   // 암대각재
	T_GRUP_K ArmTie;        // 암조재
	T_GRUP_K Redundant;     // 보조재
	void Initialize()
	{
		MainPost    = 0;
		MainBracing = 0;
		ArmPost     = 0;
		ArmDiagonal = 0;
		ArmTie      = 0;    
		Redundant   = 0;
	}
};
struct T_MTBG_UDRD_D
{
	T_MTBG_D data;
};
struct T_MTBG_UNIT
{
	T_MTBG_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEMTBG 1

// Design Variables for Member Type
#define T_DVMT_K unsigned int // always 1
struct T_DVMT_D_100
{
	double dKyKz[5];          // MainPost, MainBracing, ArmPost, ArmDiagonal, ArmTie
	int    nJoint[5];         // 0:Butt Joint, 1:Lap Joint, 2:No Joint
	double dSafetyFactor[6];
	int    nEccent[5];        // 0:Small, 1:Normal, 2:Large
	double dLimitSR[5];
	void Initialize()
	{
		dKyKz[0] = 0.9;
		dKyKz[1] = 0.8;
		dKyKz[2] = 0.8;
		dKyKz[3] = 0.8;
		dKyKz[4] = 0.8;
		nJoint[0] = 0;  // Main Post = Butt Joint
		nJoint[1] = 2;  // Main Bracing = No Joint
		nJoint[2] = 1;  // Arm Post = Lap Joint
		nJoint[3] = 2;  // Arm Diagonal = No Joint
		nJoint[4] = 2;  // Arm Tie = No Joint
		dSafetyFactor[0] = 1.02;
		dSafetyFactor[1] = 1.02;
		dSafetyFactor[2] = 1.2;
		dSafetyFactor[3] = 1.2;
		dSafetyFactor[4] = 1.2;
		dSafetyFactor[5] = 1.05;
		nEccent[0] = 1; // Main Post = normal
		nEccent[1] = 2; // Main Bracing = large
		nEccent[2] = 1; // Arm Post = normal
		nEccent[3] = 2; // Arm Diagonal = large
		nEccent[4] = 2; // Arm Tie = large
		dLimitSR[0] = 200;
		dLimitSR[1] = 220;
		dLimitSR[2] = 200;
		dLimitSR[3] = 220;
		dLimitSR[4] = 220;
	}
};
struct T_DVMT_D
{
	double dKyKz[6];          // MainPost, MainBracing, ArmPost, ArmDiagonal, ArmTie, Redundant
	int    nJoint[6];         // 0:Butt Joint, 1:Lap Joint, 2:No Joint
	double dSafetyFactor[6];
	double dBoltSafetyFactor[6];
	int    nEccent[6];        // 0:Small, 1:Normal, 2:Large
	double dLimitSR[6];
	void Initialize()
	{
		dKyKz[0] = 0.9;
		dKyKz[1] = 0.8;
		dKyKz[2] = 0.8;
		dKyKz[3] = 0.8;
		dKyKz[4] = 0.8;
		dKyKz[5] = 0.8;
		nJoint[0] = 0;  // Main Post = Butt Joint
		nJoint[1] = 2;  // Main Bracing = No Joint
		nJoint[2] = 1;  // Arm Post = Lap Joint
		nJoint[3] = 2;  // Arm Diagonal = No Joint
		nJoint[4] = 2;  // Arm Tie = No Joint
		nJoint[5] = 2;  // Redundant = No Joint
		dSafetyFactor[0] = 1.02;
		dSafetyFactor[1] = 1.02;
		dSafetyFactor[2] = 1.2;
		dSafetyFactor[3] = 1.2;
		dSafetyFactor[4] = 1.2;
		dSafetyFactor[5] = 1.02;
		dBoltSafetyFactor[0] = 1.05;
		dBoltSafetyFactor[1] = 1.05;
		dBoltSafetyFactor[2] = 1.05;
		dBoltSafetyFactor[3] = 1.05;
		dBoltSafetyFactor[4] = 1.05;
		dBoltSafetyFactor[5] = 1.05;
		nEccent[0] = 1; // Main Post = normal
		nEccent[1] = 2; // Main Bracing = large
		nEccent[2] = 1; // Arm Post = normal
		nEccent[3] = 2; // Arm Diagonal = large
		nEccent[4] = 2; // Arm Tie = large
		nEccent[5] = 2; // Redundant = large
		dLimitSR[0] = 200;
		dLimitSR[1] = 220;
		dLimitSR[2] = 200;
		dLimitSR[3] = 220;
		dLimitSR[4] = 220;
		dLimitSR[5] = 220;
	}
	void Convert100(T_DVMT_D_100& rData)
	{
		for (int i = 0; i < 5; i++)
		{
			dKyKz[i] = rData.dKyKz[i];
			nJoint[i] = rData.nJoint[i];
			dSafetyFactor[i] = rData.dSafetyFactor[i];
			dBoltSafetyFactor[i] = rData.dSafetyFactor[5];
			nEccent[i] = rData.nEccent[i];
			dLimitSR[i] = rData.dLimitSR[i];
		}
		// Redundant Data
		dKyKz[5] = 0.8;
		nJoint[5] = 2;
		dSafetyFactor[5] = 1.02;
		dBoltSafetyFactor[5] = 1.05;
		nEccent[5] = 2;
		dLimitSR[5] = 220;
	}
};
struct T_DVMT_UDRD_D_100
{
	T_DVMT_D_100 data;
};
struct T_DVMT_UDRD_D
{
	T_DVMT_D data;
};
struct T_DVMT_UNIT
{
	T_DVMT_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEDVMT 1

// Bolt 
#define D_BOLT_M12  1
#define D_BOLT_M16  2
#define D_BOLT_M20  3
#define D_BOLT_M22  4
#define D_BOLT_M24  5
#define D_BOLT_M30  6
#define D_BOLT_M36  7

#define T_BOLT_K unsigned int // always 1
struct T_BOLT_D_100 // 하위 데이타 호환성 없음(Binary File Reading용)
{
	// MainPost, MainBracing, ArmPost, ArmDiagonal, ArmTie
	int nDesignation[5];      // D_BOLT_???
	int nFlangeConnection[5]; // 0:Single, 1:Double
	int nRowOfHole[5];
};
struct T_BOLT_BASE
{
	double dThickness;
	int    nFlangeConnection;
	int    nRowOfHole;
};
struct T_BOLT_D
{
	T_BOLT_BASE Post[7];
	T_BOLT_BASE Etc[7];
};
struct T_BOLT_UDRD_D_100
{
	T_BOLT_D_100 data;
};
struct T_BOLT_UDRD_D
{
	T_BOLT_D data;
};
struct T_BOLT_UNIT
{
	T_BOLT_UNIT()
	{
		dThickness = D_UNITSYS_BASE_LENGTH;
	}
	int dThickness;
};
#define HASHSIZEBOLT 1

// Tower Load Generator Data
#define T_TLGD_K unsigned int // always 1
struct T_TLGD_TOWER_INFO_100  // 철탑제원
{
	int nType;  // 0:직선형, 1:각도형, 2:내장형, 3:인류형
	double dCenterX;
	double dCenterY;
	CArray<T_NODE_K, T_NODE_K> aGLineNodes; // 가공지선 절점
	CArray<T_NODE_K, T_NODE_K> aELineNodes; // 전선 절점
};
struct T_TLGD_TOWER_INFO  // 철탑제원
{
	int nType;  // 0:직선형, 1:각도형, 2:내장형, 3:인류형
	double dGLineFactor[2];
	double dELineFactor[2];
	double dCenterX;
	double dCenterY;
	CArray<T_NODE_K, T_NODE_K> aGLineNodes; // 가공지선 절점
	CArray<T_NODE_K, T_NODE_K> aELineNodes; // 전선 절점

	T_TLGD_TOWER_INFO() {}
	T_TLGD_TOWER_INFO(const T_TLGD_TOWER_INFO& src) { *this = src; }
	T_TLGD_TOWER_INFO& operator=(const T_TLGD_TOWER_INFO& src)
	{
		nType = src.nType;
		memcpy(dGLineFactor, src.dGLineFactor, sizeof(dGLineFactor));
		memcpy(dELineFactor, src.dELineFactor, sizeof(dELineFactor));
		dCenterX = src.dCenterX;
		dCenterY = src.dCenterY;
		aGLineNodes.Copy(src.aGLineNodes);
		aELineNodes.Copy(src.aELineNodes);
		return *this;
	}
	void Initialize()
	{
		nType = 0;
		dGLineFactor[0] = dGLineFactor[1] = 0.0;
		dELineFactor[0] = dELineFactor[1] = 0.0;
		dCenterX = 0.0;
		dCenterY = 0.0;
		aGLineNodes.RemoveAll();
		aELineNodes.RemoveAll();
	}
	void Convert100(T_TLGD_TOWER_INFO_100& rData)
	{
		nType = rData.nType;
		dGLineFactor[0] = dGLineFactor[1] = 0.0;
		dELineFactor[0] = dELineFactor[1] = 0.0;
		dCenterX = rData.dCenterX;
		dCenterY = rData.dCenterY;
		aGLineNodes.Copy(rData.aGLineNodes);
		aELineNodes.Copy(rData.aELineNodes);
	}
};
struct T_TLGD_TOWER_INFO_RW_100
{
	int nType;
	double dCenterX;
	double dCenterY;
	int nGLineNodesNum;
	int nELineNodesNum;
};
struct T_TLGD_TOWER_INFO_RW
{
	int nType;
	double dGLineFactor[2];
	double dELineFactor[2];
	double dCenterX;
	double dCenterY;
	int nGLineNodesNum;
	int nELineNodesNum;
	void SetTlgdTowerInfo(T_TLGD_TOWER_INFO& rData)
	{
		nType = rData.nType;
		memcpy(dGLineFactor, rData.dGLineFactor, sizeof(dGLineFactor));
		memcpy(dELineFactor, rData.dELineFactor, sizeof(dELineFactor));
		dCenterX = rData.dCenterX;
		dCenterY = rData.dCenterY;
		nGLineNodesNum = (int)rData.aGLineNodes.GetSize();
		nELineNodesNum = (int)rData.aELineNodes.GetSize();
	}
	void GetTlgdTowerInfo(T_TLGD_TOWER_INFO& rData)
	{
		rData.nType = nType;
		memcpy(rData.dGLineFactor, dGLineFactor, sizeof(dGLineFactor));
		memcpy(rData.dELineFactor, dELineFactor, sizeof(dELineFactor));
		rData.dCenterX = dCenterX;
		rData.dCenterY = dCenterY;
	}
	void Convert100(T_TLGD_TOWER_INFO_RW_100& rData)
	{
		nType = rData.nType;
		dGLineFactor[0] = dGLineFactor[1] = 0.0;
		dELineFactor[0] = dELineFactor[1] = 0.0;
		dCenterX = rData.dCenterX;
		dCenterY = rData.dCenterY;
		nGLineNodesNum = rData.nGLineNodesNum;
		nELineNodesNum = rData.nELineNodesNum;
	}
};
struct T_TLGD_TOWER_INFO_UNIT
{
	T_TLGD_TOWER_INFO_UNIT()
	{
		dCenterX = D_UNITSYS_BASE_LENGTH;
		dCenterY = D_UNITSYS_BASE_LENGTH;
	}
	int dCenterX;
	int dCenterY;
};
struct T_TLGD_GASUB_LINE_100  // 가섭선
{
	double dGLineMaxTension[2];  // 최대상정 장력(가공지선) : [0]고온계, [1]저온계
	double dGLineOuterDiameter;  // 가섭선 외경(가공지선)
	double dGLineUnitWgt;        // 가섭선 단위중량(가공지선)
	double dELineMaxTension[2];  // 최대상정 장력(전선) : [0]고온계, [1]저온계
	double dELineOuterDiameter;  // 가섭선 외경(전선)
	double dELineUnitWgt;        // 가섭선 단위중량(전선)
	int    nSmallConductor;      // 1상당 소도체수(전선)
	double dPibingThik;          // 피빙 두께
	double dPibingSpGrav;        // 피빙 비중
};
struct T_TLGD_GASUB_LINE  // 가섭선
{
	double dGLineMaxTension[2];  // 최대상정 장력(가공지선) : [0]고온계, [1]저온계
	double dGLineOuterDiameter;  // 가섭선 외경(가공지선)
	double dGLineUnitWgt;        // 가섭선 단위중량(가공지선)
	double dELineMaxTension[2];  // 최대상정 장력(전선) : [0]고온계, [1]저온계
	double dELineOuterDiameter;  // 가섭선 외경(전선)
	double dELineUnitWgt;        // 가섭선 단위중량(전선)
	int    nSmallConductor;      // 1상당 소도체수(전선)
	int    nDansunJosoo;         // 단선조수(전선)                
	double dPibingThik;          // 피빙 두께
	double dPibingSpGrav;        // 피빙 비중
	void Initialize()
	{
		dGLineMaxTension[0] = dGLineMaxTension[1] = 0.0;
		dGLineOuterDiameter = 0.0;
		dGLineUnitWgt = 0.0;
		dELineMaxTension[0] = dELineMaxTension[1] = 0.0;
		dELineOuterDiameter = 0.0;
		dELineUnitWgt = 0.0;
		nSmallConductor = 4;
		nDansunJosoo = 2;                   
		dPibingThik = 0.0;
		dPibingSpGrav = 0.0;
	}
	void Convert100(T_TLGD_GASUB_LINE_100& rData)
	{
		dGLineMaxTension[0] = rData.dGLineMaxTension[0];
		dGLineMaxTension[1] = rData.dGLineMaxTension[1];
		dGLineOuterDiameter = rData.dGLineOuterDiameter;
		dGLineUnitWgt       = rData.dGLineUnitWgt      ;
		dELineMaxTension[0] = rData.dELineMaxTension[0];
		dELineMaxTension[1] = rData.dELineMaxTension[1];
		dELineOuterDiameter = rData.dELineOuterDiameter;
		dELineUnitWgt       = rData.dELineUnitWgt      ;
		nSmallConductor     = rData.nSmallConductor    ;
		nDansunJosoo        = 2                        ;     
		dPibingThik         = rData.dPibingThik        ;
		dPibingSpGrav       = rData.dPibingSpGrav      ;
	}
};
struct T_TLGD_GASUB_LINE_UNIT
{
	T_TLGD_GASUB_LINE_UNIT()
	{
		dGLineMaxTension = D_UNITSYS_BASE_FORCE;
		dGLineOuterDiameter = D_UNITSYS_BASE_LENGTH;
		dGLineUnitWgt = D_UNITSYS_BASE_UNITLOAD;
		dELineMaxTension = D_UNITSYS_BASE_FORCE;
		dELineOuterDiameter = D_UNITSYS_BASE_LENGTH;
		dELineUnitWgt = D_UNITSYS_BASE_UNITLOAD;
		dPibingThik = D_UNITSYS_BASE_LENGTH;
		dPibingSpGrav = D_UNITSYS_BASE_DENSITY;
	}
	int dGLineMaxTension;
	int dGLineOuterDiameter;
	int dGLineUnitWgt;
	int dELineMaxTension;
	int dELineOuterDiameter;
	int dELineUnitWgt;
	int dPibingThik;
	int dPibingSpGrav;
};
struct T_TLGD_VERT_LOAD_100   // 수직하중
{
	double dLoadSpan;       // 수직하중 경간
	double dVertAngleLoadGLine;  // 수직각도 하중(가공지선)
	double dVertAngleLoadELine;  // 수직각도 하중(전선)
	double dInsulatorWgt;   // 애자장치 중량
	double dWorkerWgt;      // 작업원 중량
	double dSignBoardWgt;   // 항공장애 표지구 중량  
};
struct T_TLGD_VERT_LOAD   // 수직하중
{
	double dLoadSpan;             // 수직하중 경간(정상시)
	double dLoadSpanDansunFactor; // 수직하중 경간(단선시) : Factor
	double dELineLoadSpan;            // 전선 수직하중경간(정상시)
	double dELineLoadSpanDansunFactor; // 전선 수직하중경간(단선시) : Factor
	double dVertAngleLoadGLine;  // 수직각도 하중(가공지선)
	double dVertAngleLoadELine;  // 수직각도 하중(전선)
	double dInsulatorWgt;   // 애자장치 중량
	double dWorkerWgt;      // 작업원 중량
	double dSignBoardWgt;   // 항공장애 표지구 중량
	double dEtcLoadGLine;   // 기타 수직하중(가공지선)
	double dEtcLoadELine;   // 기타 수직하중(전선)
	void Initialize()
	{
		dLoadSpan = 0.0;
		dLoadSpanDansunFactor = 0.5;
		dELineLoadSpan = 0.0;
		dELineLoadSpanDansunFactor = 0.5;
		dVertAngleLoadGLine = 0.0;
		dVertAngleLoadELine = 0.0;
		dInsulatorWgt = 0.0;
		dWorkerWgt = 0.0;
		dSignBoardWgt = 0.0;
		dEtcLoadGLine = 0.0;
		dEtcLoadELine = 0.0;
	}
	void Convert100(T_TLGD_VERT_LOAD_100& rData)
	{
		dLoadSpan             = rData.dLoadSpan            ;
		dLoadSpanDansunFactor = 0.5                        ;
		dELineLoadSpan        = rData.dLoadSpan            ;
		dELineLoadSpanDansunFactor = 0.5                   ;
		dVertAngleLoadGLine   = rData.dVertAngleLoadGLine  ;
		dVertAngleLoadELine   = rData.dVertAngleLoadELine  ;
		dInsulatorWgt         = rData.dInsulatorWgt        ;
		dWorkerWgt            = rData.dWorkerWgt           ;
		dSignBoardWgt         = rData.dSignBoardWgt        ;
		dEtcLoadGLine         = 0.0                        ;
		dEtcLoadELine         = 0.0                        ;
	}
};
struct T_TLGD_VERT_LOAD_UNIT
{
	T_TLGD_VERT_LOAD_UNIT()
	{
		dLoadSpan = D_UNITSYS_BASE_LENGTH;
		dELineLoadSpan = D_UNITSYS_BASE_LENGTH;
		dVertAngleLoadGLine = D_UNITSYS_BASE_FORCE;
		dVertAngleLoadELine = D_UNITSYS_BASE_FORCE;
		dInsulatorWgt = D_UNITSYS_BASE_WEIGHT;
		dWorkerWgt = D_UNITSYS_BASE_WEIGHT;
		dSignBoardWgt = D_UNITSYS_BASE_WEIGHT;
		dEtcLoadGLine = D_UNITSYS_BASE_WEIGHT;
		dEtcLoadELine = D_UNITSYS_BASE_WEIGHT;
	}
	int dLoadSpan;
	int dELineLoadSpan;
	int dVertAngleLoadGLine;
	int dVertAngleLoadELine;
	int dInsulatorWgt;
	int dWorkerWgt;
	int dSignBoardWgt;
	int dEtcLoadGLine;
	int dEtcLoadELine;
};
struct T_TLGD_HORZ_LOAD_100       // 수평하중
{
	double dLoadSpan;             // 수평하중 경간
	double dBodyWindPres[2];      // 철탑본체 풍압력 [0]고온계, [1]저온계
	double dArmWindPres[2];       // 철탑암 풍압력 [0]고온계, [1]저온계
	double dGLineWindPres[2];     // 가공지선 풍압력 [0]고온계, [1]저온계
	double dELineWindPres[2];     // 전선 풍압력 [0]고온계, [1]저온계
	double dInsulatorWindLoad[2]; // 애자장치 풍하중 [0]고온계, [1]저온계
	double dSignBoardWindLoad[2]; // 항공장애 표지구 풍하중 [0]고온계, [1]저온계
	double dHorzAngle;            // 수평각
};
struct T_TLGD_HORZ_LOAD       // 수평하중
{
	double dLoadSpan;             // 수평하중 경간(정상시)
	double dLoadSpanDansunFactor; // 수평하중 경간(단선시) : Factor
	double dBodyWindPres[2];      // 철탑본체 풍압력 [0]고온계, [1]저온계
	double dArmWindPres[2];       // 철탑암 풍압력 [0]고온계, [1]저온계
	double dGLineWindPres[2];     // 가공지선 풍압력 [0]고온계, [1]저온계
	double dELineWindPres[2];     // 전선 풍압력 [0]고온계, [1]저온계
	double dInsulatorWindLoad[2]; // 애자장치 풍하중 [0]고온계, [1]저온계
	double dSignBoardWindLoad[2]; // 항공장애 표지구 풍하중 [0]고온계, [1]저온계
	double dHorzAngle;            // 수평각
	void Initialize()
	{
		dLoadSpan = 0.0;
		dLoadSpanDansunFactor = 0.5;
		dBodyWindPres[0] = dBodyWindPres[1] = 0.0;
		dArmWindPres[0] = dArmWindPres[1] = 0.0;
		dGLineWindPres[0] = dGLineWindPres[1] = 0.0;
		dELineWindPres[0] = dELineWindPres[1] = 0.0;
		dInsulatorWindLoad[0] = dInsulatorWindLoad[1] = 0.0;
		dSignBoardWindLoad[0] = dSignBoardWindLoad[1] = 0.0;
		dHorzAngle = 0.0;
	}
	void Convert100(T_TLGD_HORZ_LOAD_100& rData)
	{
		dLoadSpan             = rData.dLoadSpan            ;
		dLoadSpanDansunFactor = 0.5                        ;
		dBodyWindPres[0]      = rData.dBodyWindPres[0]     ;
		dBodyWindPres[1]      = rData.dBodyWindPres[1]     ;
		dArmWindPres[0]       = rData.dArmWindPres[0]      ;
		dArmWindPres[1]       = rData.dArmWindPres[1]      ;
		dGLineWindPres[0]     = rData.dGLineWindPres[0]    ;
		dGLineWindPres[1]     = rData.dGLineWindPres[1]    ;
		dELineWindPres[0]     = rData.dELineWindPres[0]    ;
		dELineWindPres[1]     = rData.dELineWindPres[1]    ;
		dInsulatorWindLoad[0] = rData.dInsulatorWindLoad[0];
		dInsulatorWindLoad[1] = rData.dInsulatorWindLoad[1];
		dSignBoardWindLoad[0] = rData.dSignBoardWindLoad[0];
		dSignBoardWindLoad[1] = rData.dSignBoardWindLoad[1];
		dHorzAngle            = rData.dHorzAngle           ;
	}
};
struct T_TLGD_HORZ_LOAD_UNIT
{
	T_TLGD_HORZ_LOAD_UNIT()
	{
		dLoadSpan = D_UNITSYS_BASE_LENGTH;
		dBodyWindPres = D_UNITSYS_BASE_PRESSURE;
		dArmWindPres = D_UNITSYS_BASE_PRESSURE;
		dGLineWindPres = D_UNITSYS_BASE_PRESSURE;
		dELineWindPres = D_UNITSYS_BASE_PRESSURE;
		dInsulatorWindLoad = D_UNITSYS_BASE_FORCE;
		dSignBoardWindLoad = D_UNITSYS_BASE_FORCE;
		dHorzAngle = D_UNITSYS_BASE_DEGREE;
	}
	int dLoadSpan;
	int dBodyWindPres;
	int dArmWindPres;
	int dGLineWindPres;
	int dELineWindPres;
	int dInsulatorWindLoad;
	int dSignBoardWindLoad;
	int dHorzAngle;
};
struct T_TLGD_D_100
{
	T_TLGD_TOWER_INFO_100 TowerInfo;  // 철탑제원
	T_TLGD_GASUB_LINE_100 GasubLine;  // 가섭선
	T_TLGD_VERT_LOAD_100  VertLoad;   // 수직하중
	T_TLGD_HORZ_LOAD_100  HorzLoad;   // 수평하중
};
struct T_TLGD_D
{
	T_TLGD_TOWER_INFO TowerInfo;  // 철탑제원
	T_TLGD_GASUB_LINE GasubLine;  // 가섭선
	T_TLGD_VERT_LOAD  VertLoad;   // 수직하중
	T_TLGD_HORZ_LOAD  HorzLoad;   // 수평하중
	void Initialize()
	{
		TowerInfo.Initialize();
		GasubLine.Initialize();
		VertLoad.Initialize();
		HorzLoad.Initialize();
	}
	void Convert100(T_TLGD_D_100& rData)
	{
		TowerInfo.Convert100(rData.TowerInfo);
		GasubLine.Convert100(rData.GasubLine);
		VertLoad.Convert100(rData.VertLoad);
		HorzLoad.Convert100(rData.HorzLoad);
	}
};
struct T_TLGD_D_RW_100
{
	T_TLGD_TOWER_INFO_RW_100 TowerInfo;  // 철탑제원
	T_TLGD_GASUB_LINE_100 GasubLine;  // 가섭선
	T_TLGD_VERT_LOAD_100  VertLoad;   // 수직하중
	T_TLGD_HORZ_LOAD_100  HorzLoad;   // 수평하중
};
struct T_TLGD_D_RW
{
	T_TLGD_TOWER_INFO_RW TowerInfo;  // 철탑제원
	T_TLGD_GASUB_LINE GasubLine;  // 가섭선
	T_TLGD_VERT_LOAD  VertLoad;   // 수직하중
	T_TLGD_HORZ_LOAD  HorzLoad;   // 수평하중
	void SetTlgd(T_TLGD_D& rData)
	{
		TowerInfo.SetTlgdTowerInfo(rData.TowerInfo);
		GasubLine = rData.GasubLine;
		VertLoad = rData.VertLoad;
		HorzLoad = rData.HorzLoad;
	}
	void GetTlgd(T_TLGD_D& rData)
	{
		TowerInfo.GetTlgdTowerInfo(rData.TowerInfo);
		rData.GasubLine = GasubLine;
		rData.VertLoad = VertLoad;
		rData.HorzLoad = HorzLoad;
	}
	void Convert100(T_TLGD_D_RW_100& rData)
	{
		TowerInfo.Convert100(rData.TowerInfo);
		GasubLine.Convert100(rData.GasubLine);
		VertLoad.Convert100(rData.VertLoad);
		HorzLoad.Convert100(rData.HorzLoad);
	}
};
struct T_TLGD_UDRD_D
{
	T_TLGD_D data;
};
struct T_TLGD_UNIT
{
	T_TLGD_TOWER_INFO_UNIT TowerInfo;
	T_TLGD_GASUB_LINE_UNIT GasubLine;
	T_TLGD_VERT_LOAD_UNIT  VertLoad;
	T_TLGD_HORZ_LOAD_UNIT  HorzLoad;
};
#define HASHSIZETLGD 1

#pragma pack(pop)

#endif  // __DB_ST_DT_TOWER_H__
