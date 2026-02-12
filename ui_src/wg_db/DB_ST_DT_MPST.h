#pragma once

#define T_MPST_K T_SECT_K

struct T_MPST_D_856
{
	double dDim;           // dY
	double dDimz;          // dZ
	double dAlpha;         // alpha X
	double dBeta;          // Beta X
	int    nPosition;      // 1=I, 2=M, 3=J
	double da;             // space of longitudinal stiffner, a Y
	BOOL   bUse;

	void Initialize()
	{
		dDim = 0.;
		dDimz = 0.;
		dAlpha = 0.;
		dBeta = 0.;
		nPosition = 1;
		da = 0.;
		bUse = FALSE;
	}
};

struct T_MPST_D_880
{
	double dDim;           // dY
	double dDimz;          // dZ
	double dAlpha;         // alpha X
	double dBeta;          // Beta X
	int    nPosition;      // 1=I, 2=M, 3=J
	double da;             // space of longitudinal stiffner, a Y
	BOOL   bUse;

	double dAlphaZ;        // Alpha Z
	double dBetaZ;         // Beta Z
	double dHingeLimY;     // 0.15h Y
	double dHingeLimZ;     // 0.15h Z
	int nnsY;              // ns Y
	int nnsZ;              // ns Z
	double dd2Y;           // d' Y
	double dd2Z;           // d' Z
	double dAngY;          // Ang. Y (전단철근과 연직축의 각도)
	double dAngZ;          // Ang. Z
	double dlY;            // l Y (L임..)
	double dlZ;            // l Z
	double daZ;            // a Z

	void Initialize()
	{
		dDim = 0.;
		dDimz = 0.;
		dAlpha = 0.;
		dBeta = 0.;
		nPosition = 1;
		da = 0.;
		bUse = FALSE;

		dAlphaZ = 0.0;
		dBetaZ = 0.0;
		dHingeLimY = 0.0;
		dHingeLimZ = 0.0;
		nnsY = 0;
		nnsZ = 0;
		dd2Y = 0.0;
		dd2Z = 0.0;
		dAngY = 90.0;
		dAngZ = 90.0;
		dlY = 0.0;
		dlZ = 0.0;
		daZ = 0.0;
	}
};

struct T_MPST_D_960
{
	double dDim;
	double dDimz;
	double dAlpha;
	double dBeta;
	int    nPosition;
	double da;
	BOOL   bUse;

	double dAlphaZ;
	double dBetaZ;
	double dHingeLimY;
	double dHingeLimZ;
	int nnsY;
	int nnsZ;
	double dd2Y;
	double dd2Z;
	double dAngY;
	double dAngZ;
	double dlY;
	double dlZ;
	double daZ;
	double dc;

	void Initialize()
	{
		dDim = 0.;
		dDimz = 0.;
		dAlpha = 0.;
		dBeta = 0.;
		nPosition = 1;
		da = 0.;
		bUse = FALSE;

		dAlphaZ = 0.0;
		dBetaZ = 0.0;
		dHingeLimY = 0.0;
		dHingeLimZ = 0.0;
		nnsY = 0;
		nnsZ = 0;
		dd2Y = 0.0;
		dd2Z = 0.0;
		dAngY = 90.0;
		dAngZ = 90.0;
		dlY = 0.0;
		dlZ = 0.0;
		daZ = 0.0;
		dc = 0.0;
	}
};

struct T_MPST_STEEL_ITEM
{
	double dEpsilon_a;		// 허용 인장변형율 εa 직접 지정
	double dL;				// 유효 좌굴길이 L
	double da;				// 종방향 보강재 간격 a
	double dRF;				// 폭 두께비 파라미터 RF(Rt)
	double dRR;				// 폭 두께비 파라미터 RR

	void Initialize()
	{
		dEpsilon_a = 0;
		dL = 0;
		da = 0;
		dRF = 0;
		dRR = 0;
	}

	T_MPST_STEEL_ITEM() {}
	T_MPST_STEEL_ITEM(const T_MPST_STEEL_ITEM& src) { *this = src; }
	T_MPST_STEEL_ITEM& operator=(const T_MPST_STEEL_ITEM& src)
	{
		if (this != &src) {
			dEpsilon_a = src.dEpsilon_a;
			dL = src.dL;
			da = src.da;
			dRF = src.dRF;
			dRR = src.dRR;
		}
		return *this;
	}
};

struct T_MPST_STEEL
{
	BOOL bEpsilon_a;		// 허용 인장변형율 εa 직접 지정 여부
	T_MPST_STEEL_ITEM LocalY;
	T_MPST_STEEL_ITEM LocalZ;

	void Initialize()
	{
		bEpsilon_a = FALSE;
		LocalY.Initialize();
		LocalZ.Initialize();
	}

	T_MPST_STEEL() {}
	T_MPST_STEEL(const T_MPST_STEEL& src) { *this = src; }
	T_MPST_STEEL& operator=(const T_MPST_STEEL& src)
	{
		if (this != &src) {
			bEpsilon_a = src.bEpsilon_a;
			LocalY = src.LocalY;
			LocalZ = src.LocalZ;
		}
		return *this;
	}
};

struct T_MPST_D
{
#pragma region // Don't use it in JP (T_STIRRUP_ITEM::dHingeLim 대체)
	double dDim;			// dY
	double dDimz;			// dZ
#pragma endregion
	double dAlpha;			// 단면 보정계수 Alpha (Local Y)
	double dBeta;			// 단면 보정계수 Beta  (Local Y)
	int    nPosition;		// 1=I, 2=M, 3=J <- v965 부터 사용하지 않음
#pragma region // Don't use it in JP (T_MPST_STEEL_ITEM::da 대체)
	double da;				// space of longitudinal stiffner, a Y
#pragma endregion
	BOOL   bUse;			// 계산여부
	double dAlphaZ;			// 단면 보정계수 Alpha (Local Z)
	double dBetaZ;			// 단면 보정계수 Beta  (Local Z)
#pragma region // Don't use it in JP (T_MPST_SSM_REBAR_ITEM::dHingeLim, nNs, dD2 대체)
	double dHingeLimY;		// 0.15h Y
	double dHingeLimZ;		// 0.15h Z
	int nnsY;				// ns Y
	int nnsZ;				// ns Z
	double dd2Y;			// d' Y
	double dd2Z;			// d' Z
#pragma endregion
	double dAngY;			// 전단철근과 연직축의 각도 Ang (Local Y) 한국내진설계에서 사용
	double dAngZ;			// 전단철근과 연직축의 각도 Ang (Local Z) 한국내진설계에서 사용
#pragma region // Don't use it in JP (T_MPST_STEEL_ITEM::dL, da 대체)
	double dlY;				// l Y (L임..)
	double dlZ;				// l Z
	double daZ;				// a Z
#pragma endregion
	double dc;				// pipe pile 부식대(0≤c≤10mm)

	T_MPST_STEEL Steel;		// 철골 관련 계수 v965추가

	void Initialize()
	{
		dDim = 0.;
		dDimz = 0.;
		dAlpha = 0.;
		dBeta = 0.;
		nPosition = 2; // 2025.09.04 chlim Civil NX JP v965 부터 UI에서 삭제되고 Mphi 계산시 항상 i,j단 평균값으로 해서 초기값을 Middle 로 함.
		da = 0.;
		bUse = FALSE;
		dAlphaZ = 0.0;
		dBetaZ = 0.0;
		dHingeLimY = 0.0;
		dHingeLimZ = 0.0;
		nnsY = 0;
		nnsZ = 0;
		dd2Y = 0.0;
		dd2Z = 0.0;
		dAngY = 90.0;
		dAngZ = 90.0;
		dlY = 0.0;
		dlZ = 0.0;
		daZ = 0.0;
		dc = 0.0;
		Steel.Initialize();
	}

	T_MPST_D() { Initialize(); };
	T_MPST_D(T_MPST_D& src) { *this = src; };
	T_MPST_D& operator=(const T_MPST_D& src)
	{
		dDim = src.dDim;           // dY
		dDimz = src.dDimz;          // dZ
		dAlpha = src.dAlpha;         // alpha X
		dBeta = src.dBeta;          // Beta X
		nPosition = src.nPosition;      // 1=I, 2=M, 3=J
		da = src.da;             // space of longitudinal stiffner, a Y
		bUse = src.bUse;
		dAlphaZ = src.dAlphaZ;        // Alpha Z
		dBetaZ = src.dBetaZ;         // Beta Z
		dHingeLimY = src.dHingeLimY;     // 0.15h Y
		dHingeLimZ = src.dHingeLimZ;     // 0.15h Z
		nnsY = src.nnsY;              // ns Y
		nnsZ = src.nnsZ;              // ns Z
		dd2Y = src.dd2Y;           // d' Y
		dd2Z = src.dd2Z;           // d' Z
		dAngY = src.dAngY;          // Ang. Y (전단철근과 연직축의 각도)
		dAngZ = src.dAngZ;          // Ang. Z
		dlY = src.dlY;            // l Y (L임..)
		dlZ = src.dlZ;            // l Z
		daZ = src.daZ;            // a Z
		dc = src.dc;             // pipe pile 부식대(0≤c≤10mm)
		Steel = src.Steel;
		return *this;
	};
	void Convert856(T_MPST_D_856 rData)
	{
		dDim = rData.dDim;
		dDimz = rData.dDimz;
		dAlpha = rData.dAlpha;
		dBeta = rData.dBeta;
		nPosition = rData.nPosition;
		da = rData.da;
		bUse = rData.bUse;
		dAlphaZ = 0.0;
		dBetaZ = 0.0;
		dHingeLimY = 0.0;
		dHingeLimZ = 0.0;
		nnsY = 0;
		nnsZ = 0;
		dd2Y = 0.0;
		dd2Z = 0.0;
		dAngY = 90.0;
		dAngZ = 90.0;
		dlY = 0.0;
		dlZ = 0.0;
		daZ = 0.0;
		dc = 0.0;
		
		Steel.LocalY.da = rData.da;
	}

	void Convert880(T_MPST_D_880 rData)
	{
		dDim = rData.dDim;
		dDimz = rData.dDimz;
		dAlpha = rData.dAlpha;
		dBeta = rData.dBeta;
		nPosition = rData.nPosition;
		da = rData.da;
		bUse = rData.bUse;
		dAlphaZ = rData.dAlphaZ;
		dBetaZ = rData.dBetaZ;
		dHingeLimY = rData.dHingeLimY;
		dHingeLimZ = rData.dHingeLimZ;
		nnsY = rData.nnsY;
		nnsZ = rData.nnsZ;
		dd2Y = rData.dd2Y;
		dd2Z = rData.dd2Z;
		dAngY = rData.dAngY;
		dAngZ = rData.dAngZ;
		dlY = rData.dlY;
		dlZ = rData.dlZ;
		daZ = rData.daZ;
		dc = 0.0;
		
		Steel.LocalY.dL = rData.dlY;
		Steel.LocalZ.dL = rData.dlZ;
		Steel.LocalY.da = rData.da;
		Steel.LocalZ.da = rData.daZ;
	}

	void Convert960(T_MPST_D_960 rData)
	{
		dDim = rData.dDim;
		dDimz = rData.dDimz;
		dAlpha = rData.dAlpha;
		dBeta = rData.dBeta;
		nPosition = rData.nPosition;
		da = rData.da;
		bUse = rData.bUse;
		dAlphaZ = rData.dAlphaZ;
		dBetaZ = rData.dBetaZ;
		dHingeLimY = rData.dHingeLimY;
		dHingeLimZ = rData.dHingeLimZ;
		nnsY = rData.nnsY;
		nnsZ = rData.nnsZ;
		dd2Y = rData.dd2Y;
		dd2Z = rData.dd2Z;
		dAngY = rData.dAngY;
		dAngZ = rData.dAngZ;
		dlY = rData.dlY;
		dlZ = rData.dlZ;
		daZ = rData.daZ;
		dc = rData.dc;

		Steel.LocalY.dL = rData.dlY;
		Steel.LocalZ.dL = rData.dlZ;
		Steel.LocalY.da = rData.da;
		Steel.LocalZ.da = rData.daZ;
	}
};
struct T_MPST_UDRD_D_856
{
	T_MPST_K key;
	T_MPST_D_856 data;
};
struct T_MPST_UDRD_D_880
{
	T_MPST_K key;
	T_MPST_D_880 data;
};
struct T_MPST_UDRD_D_960
{
	T_MPST_K key;
	T_MPST_D_960 data;
};
struct T_MPST_UDRD_D
{
	T_MPST_K key;
	T_MPST_D data;
};
struct T_MPST_UNIT
{
	T_MPST_UNIT()
	{
		dDim = D_UNITSYS_BASE_LENGTH;
		dDimz = D_UNITSYS_BASE_LENGTH;
		da = D_UNITSYS_BASE_LENGTH;
		dHingeLimY = D_UNITSYS_BASE_LENGTH;
		dHingeLimZ = D_UNITSYS_BASE_LENGTH;
		dd2Y = D_UNITSYS_BASE_LENGTH;
		dd2Z = D_UNITSYS_BASE_LENGTH;
		dlY = D_UNITSYS_BASE_LENGTH;
		dlZ = D_UNITSYS_BASE_LENGTH;
		daZ = D_UNITSYS_BASE_LENGTH;
		dc = D_UNITSYS_BASE_LENGTH;
		dEpsilon = D_UNITSYS_BASE_STRESS;
		dLength = D_UNITSYS_BASE_LENGTH;
	}
	int dDim;
	int dDimz;
	int da;
	int dHingeLimY;
	int dHingeLimZ;
	int dd2Y;
	int dd2Z;
	int dlY;
	int dlZ;
	int daZ;
	int dc;
	int dEpsilon;
	int dLength;
};
#define HASHSIZEMPST HASHSIZESECT