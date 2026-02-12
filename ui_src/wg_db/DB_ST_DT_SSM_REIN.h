#ifndef __DB_ST_DT_REIN_SSM_H__
#define __DB_ST_DT_REIN_SSM_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#pragma region // T_REIN_SSM_D (변수 구조체) : 철근콘크리트 단면보강 정보
struct T_REIN_RC_OPT
{
	double dThick; //RC피복 두께
	
	void Initialize()
	{
		dThick = 0.0;
	}

	T_REIN_RC_OPT() { }
	T_REIN_RC_OPT(const T_REIN_RC_OPT& src) { *this = src; }
	T_REIN_RC_OPT& operator=(const T_REIN_RC_OPT& src)
	{
		if (this != &src) {
			dThick = src.dThick;
		}
		return *this;
	}
};

struct T_REIN_SSM_RC
{
	BOOL bUse;		// 철근콘크리트 단면보강 사용여부
	T_REIN_RC_OPT LocalY;	// Local Y
	T_REIN_RC_OPT LocalZ;	// Local Z

	T_REIN_SSM_RC() { }
	T_REIN_SSM_RC(const T_REIN_SSM_RC& src) { *this = src; }
	T_REIN_SSM_RC& operator=(const T_REIN_SSM_RC& src)
	{
		if (this != &src) {
			bUse = src.bUse;
			LocalY = src.LocalY;
			LocalZ = src.LocalZ;
		}
		return *this;
	}

	void Initialize()
	{
		bUse = false;
		LocalY.Initialize();
		LocalZ.Initialize();
	}
};
#pragma endregion

#pragma region // T_REIN_SSM_D (변수 구조체) : 강판 단면보강 정보
struct T_REIN_SP_OPT
{
	double dThick;	// 강판피복 두께
	BOOL bConvertStrrupSect; // 횡구속 철근 환산 여부 - 기설부
	BOOL bConvertStrrupRein; // 횡구속 철근 환산 여부 - RC보강부
	BOOL bPsLengthStrrup; //  횡구속 철근 - 콘크리트 단면의 ps 에 강판 유효길이 고려 여부
	double dLengthStrrup; // 횡구속 철근 강판 유효길이

	void Initialize()
	{
		dThick = 0.0;
		bConvertStrrupSect = FALSE;
		bConvertStrrupRein = FALSE;
		bPsLengthStrrup = FALSE;
		dLengthStrrup = 0.0;
	}

	T_REIN_SP_OPT() { }
	T_REIN_SP_OPT(const T_REIN_SP_OPT& src) { *this = src; }
	T_REIN_SP_OPT& operator=(const T_REIN_SP_OPT& src)
	{
		if (this != &src) {
			dThick = src.dThick;
			bConvertStrrupSect = src.bConvertStrrupSect;
			bConvertStrrupRein = src.bConvertStrrupRein;
			bPsLengthStrrup = src.bPsLengthStrrup;
			dLengthStrrup = src.dLengthStrrup;
		}
		return *this;
	}
};

struct T_REIN_SSM_SP
{
	BOOL bUse;		// 강판 단면보강 사용여부
	BOOL bConsiderWithoutAnker; // 축방향 저항효과를 앵커철근 배치와 상관없이 항상 고려
	T_REIN_SP_OPT LocalY;	// Local Y
	T_REIN_SP_OPT LocalZ;	// Local Z

	T_REIN_SSM_SP() { }
	T_REIN_SSM_SP(const T_REIN_SSM_SP& src) { *this = src; }
	T_REIN_SSM_SP& operator=(const T_REIN_SSM_SP& src)
	{
		if (this != &src) {
			bUse = src.bUse;
			bConsiderWithoutAnker = src.bConsiderWithoutAnker;
			LocalY = src.LocalY;
			LocalZ = src.LocalZ;
		}
		return *this;
	}

	void Initialize()
	{
		bUse = false;
		bConsiderWithoutAnker = false;
		LocalY.Initialize();
		LocalZ.Initialize();
	}
};
#pragma endregion

#pragma region // T_REIN_SSM_D (변수 구조체) : 섬유피복 단면보강 정보
struct T_REIN_CF_OPT
{
	UINT nBendNum;			// 휨 보강 시트 매수
	UINT nShearNum;		// 전단 보강 시트 매수
	UINT nToughNumSect;	// 인성보강 시트 매수 - 기설부
	UINT nToughNumRein;	// 인성보강 시트 매수 - RC보강부
	double dToughLenSect;	// 인성보강 유효길이 - 기설부
	double dToughLenRein;	// 인성보강 유효길이 - RC보강부

	void Initialize()
	{
		nBendNum = 0;
		nShearNum = 0;
		nToughNumSect = 0;
		nToughNumRein = 0;
		dToughLenSect = 0.0;
		dToughLenRein = 0.0;
	}

	T_REIN_CF_OPT() { }
	T_REIN_CF_OPT(const T_REIN_CF_OPT& src) { *this = src; }
	T_REIN_CF_OPT& operator=(const T_REIN_CF_OPT& src)
	{
		if (this != &src) {
			nBendNum = src.nBendNum;
			nShearNum = src.nShearNum;
			nToughNumSect = src.nToughNumSect;
			nToughNumRein = src.nToughNumRein;
			dToughLenSect = src.dToughLenSect;
			dToughLenRein = src.dToughLenRein;
		}
		return *this;
	}
};

struct T_REIN_SSM_CF
{
	BOOL bUse;				// 섬유피복 단면보강 사용여부
	UINT nFormulaType;		// 섬유피복 보강 계산식 T_SECT_REIN_CF::eFormulaType
	T_REIN_CF_OPT LocalY;	// Local Y
	T_REIN_CF_OPT LocalZ;	// Local Z

	T_REIN_SSM_CF() { Initialize(); }
	T_REIN_SSM_CF(const T_REIN_SSM_CF& src) { *this = src; }
	T_REIN_SSM_CF& operator=(const T_REIN_SSM_CF& src)
	{
		if (this != &src) {
			bUse = src.bUse;
			nFormulaType = src.nFormulaType;
			LocalY = src.LocalY;
			LocalZ = src.LocalZ;
		}
		return *this;
	}

	void Initialize()
	{
		bUse = false;
		nFormulaType = 1;
		LocalY.Initialize();
		LocalZ.Initialize();
	}

	enum eFormulaType
	{
		kUnKouwn,
		kCalcuFormula1,
		kCalcuFormula2,
	};
};
#pragma endregion

//Reinforcement Section - Section Manager
#define T_REIN_SSM_K unsigned int
struct T_REIN_SSM_D
{
	T_REIN_SSM_RC ReinSectRC;	// 철근콘크리트(Rebar Concrete) 보강단면
	T_REIN_SSM_SP ReinSectSP;	// 강판(Steel Plate) 보강단면
	T_REIN_SSM_CF ReinSectCF;	// 탄소섬유(Carbon fiber) 보강단면

	T_REIN_SSM_D() { Initialize(); }
	T_REIN_SSM_D(const T_REIN_SSM_D& src) { *this = src; }
	T_REIN_SSM_D& operator=(const T_REIN_SSM_D& src)
	{
		if (this != &src) {
			ReinSectRC = src.ReinSectRC;
			ReinSectSP = src.ReinSectSP;
			ReinSectCF = src.ReinSectCF;
		}
		return *this;
	}

	void Initialize()
	{
		ReinSectRC.Initialize();
		ReinSectSP.Initialize();
		ReinSectCF.Initialize();
	}
};

struct T_REIN_SSM_UNIT
{
	T_REIN_SSM_UNIT()
	{
		Length = D_UNITSYS_BASE_LENGTH;
		Thickness = D_UNITSYS_BASE_LENGTH;
	}
	int Length;
	int Thickness;
};

struct T_REIN_SSM_UDRD_D
{
	T_REIN_SSM_K key;
	T_REIN_SSM_D data;
};

#define HASHSIZEREINSSM HASHSIZESECT

#pragma region //T_SECT_SSM_D (DB 구조체) - 요소별 보강단면 정보
#define T_SECT_SSM_K unsigned int
struct T_SECT_SSM_D
{
	T_ELEM_K kElem;				// T_ELEM_D 와 1:1 관계
	T_REIN_SSM_K kReinSsm;		// T_REIN_SSM_D 와 1:N 관계

	T_SECT_SSM_D() { Initialize(); }
	T_SECT_SSM_D(const T_SECT_SSM_D& src) { *this = src; }
	T_SECT_SSM_D& operator=(const T_SECT_SSM_D& src)
	{
		if (this != &src) {
			kElem = src.kElem;
			kReinSsm = src.kReinSsm;
		}
		return *this;
	}

	void Initialize()
	{
		kElem = 0;
		kReinSsm = 0;
	}
};

struct T_SECT_SSM_UNIT
{
	T_SECT_SSM_UNIT()
	{
	}
};

struct T_SECT_SSM_UDRD_D
{
	T_SECT_SSM_K key;
	T_SECT_SSM_D data;
};

#define HASHSIZESECTSSM HASHSIZEELEM
#pragma endregion

#pragma pack(pop)

#endif  // __DB_ST_DT_REIN_SSM_H__