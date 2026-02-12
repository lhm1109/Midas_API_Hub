#pragma once

#define T_MPHG_K T_MATL_K
struct T_MPHG_D_856
{
	char cDir[4];          // '0':꺼짐, '1':켜짐 : 3개만 사용(PMM, My, Mz)
	int  nSectionNum[3];   // Number of Section
	int  nHysModel[3];     // HYSTERESIS MODEL
	int nStrsStrnCurve;    // 0=Concrete Standard Spec. 1=Municipal V, 2=Municipal III(Sigma_ck), 3=Municipal III(0.85Sigma_ck)
	int nConsiderSigbt;    // 0=Consider, 1=Do not Consider

	void Initialize()
	{
		cDir[0] = '0';
		cDir[1] = '1';
		cDir[2] = '1';
		for (int i = 0; i < 3; i++) nSectionNum[i] = 3;
		nHysModel[0] = 0;    // Kinematic Hardening(Normal Trilinear)
		nHysModel[1] = 6;    // Modified Takeda Trilinear
		nHysModel[2] = 6;    // Modified Takeda Trilinear
		nStrsStrnCurve = 1;
		nConsiderSigbt = 0;
	}
};
struct T_MPHG_D_885
{
	char cDir[4];          // '0':꺼짐, '1':켜짐 : 3개만 사용(PMM, My, Mz)
	int  nSectionNum[3];   // Number of Section
	int  nHysModel[3];     // HYSTERESIS MODEL
	int nStrsStrnCurve;    // 0=Concrete Standard Spec. 1=Municipal V, 2=Municipal III(Sigma_ck), 3=Municipal III(0.85Sigma_ck)
	int nConsiderSigbt;    // 0=Consider, 1=Do not Consider
	int nBrokenLine1;      // 1차 꺽인점.. [RC/PSC일 때 0:균열, 1:항복], [Steel/SRC일 때 0:압축쪽 항목, 1:인장쪽 항목]
	int nBrokenLine2;      // 2차 꺽인점.. 0:항복, 종국
	int nLimitState;       // 교량의 중요도.. 0:A종(내진성능3), 1:B종(내진성능2)
	int nStrenthLimit;      // 한계 압축 변형율 이후의 강성.. 0:하강 강성 유지, 1:강성 0으로 처리

	void Initialize()
	{
		cDir[0] = '0';
		cDir[1] = '1';
		cDir[2] = '1';
		for (int i = 0; i < 3; i++) nSectionNum[i] = 3;
		nHysModel[0] = 0;    // Kinematic Hardening(Normal Trilinear)
		nHysModel[1] = 6;    // Modified Takeda Trilinear
		nHysModel[2] = 6;    // Modified Takeda Trilinear
		nStrsStrnCurve = 1;
		nConsiderSigbt = 0;
		nBrokenLine1 = 0;
		nBrokenLine2 = 0;
		nLimitState = 1;
		nStrenthLimit = 0;
	}
};

struct T_MPHG_D_960
{
	char cDir[4];
	int  nSectionNum[3];
	int  nHysModel[3];
	int nStrsStrnCurve;
	int nConsiderSigbt;
	int nBrokenLine1;
	int nBrokenLine2;
	int nLimitState;
	int nStrenthLimit;
	int nCode;
	int nEarthquakeType;

	void Initialize()
	{
		cDir[0] = '0';
		cDir[1] = '1';
		cDir[2] = '1';
		for (int i = 0; i < 3; i++) nSectionNum[i] = 3;
		nHysModel[0] = 0;
		nHysModel[1] = 6;
		nHysModel[2] = 6;
		nStrsStrnCurve = 1;
		nConsiderSigbt = 0;
		nBrokenLine1 = 0;
		nBrokenLine2 = 0;
		nLimitState = 1;
		nStrenthLimit = 0;
		nCode = 4;
		nEarthquakeType = 1;
	}
};

#define D_MPHG_SECTION_NUM_MAX 20  /// nSectionNum의 최대값 제한.

struct T_MPHG_D
{
	char cDir[4];          // '0':꺼짐, '1':켜짐 : 3개만 사용(PMM, My, Mz)
	int  nSectionNum[3];   // Number of Section
	int  nHysModel[3];     // HYSTERESIS MODEL
	//  0: Kinematic Hardening(Normal Trilinear), 
	//  1: Origin-oriented Trilinear,     2: Peak-oriented Trilinear,
	//  3: Clough Bilinear,               4: Degrading Trilinear, 
	//  5: Original Takeda Trilinear,     6: Modified Takeda Trilinear     
	// 10: Normal Bilinear               11: Elastic Bilinear
	// 12: Elastic Trilinear             13: Elastic Tetralinear
	// 14: Takeda Tetralinear            15: Modified Takeda Tetralinear
// ISOLATOR
	   // 51: LRB Isolator Bilinear Type   52: LRB Isolator Trilinear Type
	   // 53: High Damping Rubber Isolator 
	int nStrsStrnCurve;		// 0=Concrete Standard Spec. 1=Municipal V, 2=Municipal III(Sigma_ck), 3=Municipal III(0.85Sigma_ck)
	int nConsiderSigbt;		// 0=Consider, 1=Do not Consider
	int nBrokenLine1;		// 1차 꺽인점.. [RC/PSC일 때 0:균열, 1:항복], [Steel/SRC일 때 0:압축쪽 항목, 1:인장쪽 항목]
	int nBrokenLine2;		// 2차 꺽인점.. 0:항복, 종국
	int nLimitState;		// 교량의 중요도.. 0:A종(내진성능3), 1:B종(내진성능2)
	int nStrenthLimit;      // 한계 압축 변형율 이후의 강성.. 0:하강 강성 유지, 1:강성 0으로 처리
	int nCode;              // 0:"콘크리트표준시방서", 1:"도시V (평성14년)", 2:"도시III (평성14년, σc,σck)", 3:"도시III (평성14년, σc,0.85σck)", 4:"도시V (평성24년)", 5:"도시IV (평성14/24년, 말뚝기초)"
	int nEarthquakeType;	// 0=Type1, 1=Type2
	int nExamTarget;		// 검토대상 : T_MPHG_D::eExamTarget v965추가
	BOOL bPassYbPoint;		// 골격곡선이 Yb점을 통과 v965추가
	int nSectionConsider;  // 단면 제원의 취급 0: A＝Ac＋n・As 1:A＝Ac＋ (n-1)・As

	void Initialize()
	{
		cDir[0] = '0';
		cDir[1] = '1';
		cDir[2] = '1';
		for (int i = 0; i < 3; i++) nSectionNum[i] = 3;
		nHysModel[0] = 0;    // Kinematic Hardening(Normal Trilinear)
		nHysModel[1] = 6;    // Modified Takeda Trilinear
		nHysModel[2] = 6;    // Modified Takeda Trilinear
		nStrsStrnCurve = 1;
		nConsiderSigbt = 0;
		nBrokenLine1 = 0;
		nBrokenLine2 = 0;
		nLimitState = 1;
		nStrenthLimit = 0;
		nCode = 4;
		nEarthquakeType = 1;
		nExamTarget = eExamTarget::kExistingSection;
		bPassYbPoint = FALSE;
		nSectionConsider = 0;
	}
	void Convert856(T_MPHG_D_856 rData)
	{
		for (int i = 0; i < 4; i++) cDir[i] = rData.cDir[i];
		for (int i = 0; i < 3; i++) nSectionNum[i] = rData.nSectionNum[i];
		nHysModel[0] = rData.nHysModel[0];
		nHysModel[1] = rData.nHysModel[1];
		nHysModel[2] = rData.nHysModel[2];
		nStrsStrnCurve = rData.nStrsStrnCurve;
		nConsiderSigbt = rData.nConsiderSigbt;
		nBrokenLine1 = 0;
		nBrokenLine2 = 0;
		nLimitState = 1;
		nStrenthLimit = 0;
		nCode = 4;
		nEarthquakeType = 1;
		nExamTarget = eExamTarget::kExistingSection;
		bPassYbPoint = FALSE;
		nSectionConsider = 0;
	}
	void Convert885(T_MPHG_D_885 rData)
	{
		for (int i = 0; i < 4; i++) cDir[i] = rData.cDir[i];
		for (int i = 0; i < 3; i++) nSectionNum[i] = rData.nSectionNum[i];
		nHysModel[0] = rData.nHysModel[0];
		nHysModel[1] = rData.nHysModel[1];
		nHysModel[2] = rData.nHysModel[2];
		nStrsStrnCurve = rData.nStrsStrnCurve;
		nConsiderSigbt = rData.nConsiderSigbt;
		nBrokenLine1 = rData.nBrokenLine1;
		nBrokenLine2 = rData.nBrokenLine2;
		nLimitState = rData.nLimitState;
		nStrenthLimit = rData.nStrenthLimit;
		nCode = 4;
		nEarthquakeType = 1;
		nExamTarget = eExamTarget::kExistingSection;
		bPassYbPoint = FALSE;
		nSectionConsider = 0;
	}
	void Convert960(T_MPHG_D_960 rData)
	{
		for (int i = 0; i < 4; i++) cDir[i] = rData.cDir[i];
		for (int i = 0; i < 3; i++) nSectionNum[i] = rData.nSectionNum[i];
		nHysModel[0] = rData.nHysModel[0];
		nHysModel[1] = rData.nHysModel[1];
		nHysModel[2] = rData.nHysModel[2];
		nStrsStrnCurve = rData.nStrsStrnCurve;
		nConsiderSigbt = rData.nConsiderSigbt;
		nBrokenLine1 = rData.nBrokenLine1;
		nBrokenLine2 = rData.nBrokenLine2;
		nLimitState = rData.nLimitState;
		nStrenthLimit = rData.nStrenthLimit;
		nCode = rData.nCode;
		nEarthquakeType = rData.nEarthquakeType;
		nExamTarget = eExamTarget::kExistingSection;
		bPassYbPoint = FALSE;
		nSectionConsider = 0;
	}

	enum eExamTarget
	{
		kExistingSection,//기설단면
		kReinforceSection,//기설+보강 단면
	};

	enum eSectionConsider
	{
		kNAs,			//A＝Ac＋n・As
		kNminusAs,	//A＝Ac＋ (n-1)・As
	};
};
struct T_MPHG_UDRD_D_856
{
	T_MPHG_K key;
	T_MPHG_D_856 data;
};
struct T_MPHG_UDRD_D_885
{
	T_MPHG_K key;
	T_MPHG_D_885 data;
};
struct T_MPHG_UDRD_D_960
{
	T_MPHG_K key;
	T_MPHG_D_960 data;
};
struct T_MPHG_UDRD_D
{
	T_MPHG_K key;
	T_MPHG_D data;
};
struct T_MPHG_UNIT
{
	T_MPHG_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;
	}
	int nNoUnit;
};
#define HASHSIZEMPHG HASHSIZEMATL