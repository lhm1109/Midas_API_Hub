#pragma once

#pragma region // T_MPST_REBAR (변수 구조체) : M-Phi 계산 파라미터(철근)
struct T_MPST_SSM_REBAR_ITEM
{
	double dEpsilon_st;		// 허용 인장변형율 εst 직접 지정
	double dLongDia_Est;	// εst산출용 축방향 철근 직경 φ
	double dLongDia_Lp;		// Lp 산출용 축방향 철근 직경 φ'
	int nNs;				// 압축측 축방향 철근 개수 ns
	double dD2;				// 횡구속 철근 유효길이 d'
	double dLongDia_c0;		// 축방향 철근의 순 피복두께 c0
	double dHingeLim;		// 소성힌지 길이 Lp의 제한값

	BOOL bLimitCompPos;		//한계압축 변형율 평가위치 적용 유무
	UINT nLimitCompPos;		//한계압축 변형율 평가위치 타입 T_STIRRUP_ITEM::eLimitCompPos
	double dLimitCompPos;	//한계압축 변형율 평가위치 거리 (입력 값 ≤ 단면 높이/2)

	void Initialize()
	{
		dEpsilon_st = 0;
		dLongDia_Est = 0;
		dLongDia_Lp = 0;
		nNs = 0;
		dD2 = 0;
		dLongDia_c0 = 0;
		dHingeLim = 0;
		bLimitCompPos = FALSE;
		nLimitCompPos = 0;
		dLimitCompPos = 0;
	}

	T_MPST_SSM_REBAR_ITEM() {}
	T_MPST_SSM_REBAR_ITEM(const T_MPST_SSM_REBAR_ITEM& src) { *this = src; }
	T_MPST_SSM_REBAR_ITEM& operator=(const T_MPST_SSM_REBAR_ITEM& src)
	{
		if (this != &src) {
			dEpsilon_st = src.dEpsilon_st;
			dLongDia_Est = src.dLongDia_Est;
			dLongDia_Lp = src.dLongDia_Lp;
			nNs = src.nNs;
			dD2 = src.dD2;
			dLongDia_c0 = src.dLongDia_c0;
			dHingeLim = src.dHingeLim;
			bLimitCompPos = src.bLimitCompPos;
			nLimitCompPos = src.nLimitCompPos;
			dLimitCompPos = src.dLimitCompPos;
		}
		return *this;
	}

	enum eLimitCompPos
	{
		kUnkowun,
		kOutmostSect,	//단면 최외각
		kOutmostRebar,	//철근 최외각
		kOutUser,	// 유저 입력
	};
};

struct T_MPST_SSM_REBAR
{
	enum eDir
	{
		kPlus = 0,
		kMinus,
		kSizeDir,
	};

	BOOL bEpsilon_st;		// 허용 인장변형율 εst 직접 지정 여부
	BOOL bLongDia_Est;		// εst산출용 축방향 철근 직경 φ 직접 지정 여부
	BOOL bLongDia_Lp;		// Lp 산출용 축방향 철근 직경 φ' 직접 지정 여부
	BOOL bLongDia_c0;		// 축방향 철근의 순 피복두께 c0 직접 지정 여부
	BOOL bHingeLim;			// 소성힌지 길이 Lp의 제한값 직접 지정 여부

	T_MPST_SSM_REBAR_ITEM LocalY[eDir::kSizeDir];
	T_MPST_SSM_REBAR_ITEM LocalZ[eDir::kSizeDir];

	void Initialize()
	{
		bEpsilon_st = FALSE;
		bLongDia_Est = FALSE;
		bLongDia_Lp = FALSE;
		bLongDia_c0 = FALSE;
		bHingeLim = FALSE;

		LocalY[eDir::kPlus].Initialize();
		LocalY[eDir::kMinus].Initialize();
		LocalZ[eDir::kPlus].Initialize();
		LocalZ[eDir::kMinus].Initialize();
	}

	T_MPST_SSM_REBAR() {}
	T_MPST_SSM_REBAR(const T_MPST_SSM_REBAR& src) { *this = src; }
	T_MPST_SSM_REBAR& operator=(const T_MPST_SSM_REBAR& src)
	{
		if (this != &src) {
			bEpsilon_st = src.bEpsilon_st;
			bLongDia_Est = src.bLongDia_Est;
			bLongDia_Lp = src.bLongDia_Lp;
			bLongDia_c0 = src.bLongDia_c0;
			bHingeLim = src.bHingeLim;

			LocalY[eDir::kPlus] = src.LocalY[eDir::kPlus];
			LocalY[eDir::kMinus] = src.LocalY[eDir::kMinus];
			LocalZ[eDir::kPlus] = src.LocalZ[eDir::kPlus];
			LocalZ[eDir::kMinus] = src.LocalZ[eDir::kMinus];
		}
		return *this;
	}
};
#pragma endregion

#define T_MPST_SSM_K unsigned int
struct T_MPST_SSM_D
{
	enum ePos
	{
		k_I = 0,
		k_M,
		k_J,
		kSizePos,
	};

	T_MPST_SSM_REBAR Rebar[kSizePos];		// 철근 관련 계수

	void Initialize()
	{
		Rebar[ePos::k_I].Initialize();
		Rebar[ePos::k_M].Initialize();
		Rebar[ePos::k_J].Initialize();
	}

	T_MPST_SSM_D() { Initialize(); };
	T_MPST_SSM_D(T_MPST_SSM_D& src) { *this = src; };
	T_MPST_SSM_D& operator=(const T_MPST_SSM_D& src)
	{
		Rebar[ePos::k_I] = src.Rebar[ePos::k_I];
		Rebar[ePos::k_M] = src.Rebar[ePos::k_M];
		Rebar[ePos::k_J] = src.Rebar[ePos::k_J];
		return *this;
	}
};

struct T_MPST_SSM_UNIT
{
	T_MPST_SSM_UNIT()
	{
		Length = D_UNITSYS_BASE_LENGTH;
	}
	int Length;
};

struct T_MPST_SSM_UDRD_D
{
	T_MPST_SSM_K key;
	T_MPST_SSM_D data;
};
#define HASHSIZEMPSTSSM HASHSIZESECT