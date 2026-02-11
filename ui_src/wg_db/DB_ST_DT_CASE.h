#ifndef __DB_ST_DT_CASE_H__
#define __DB_ST_DT_CASE_H__

#define MIDAS_SDK

#pragma pack(push, 1)

// #if defined(_CIVIL)
// #define D_SGLD_KEY_DL   1
// #define D_SGLD_KEY_EL   2
// #define D_SGLD_KEY_TP   3
// #define D_SGLD_KEY_TS   4
// #define D_SGLD_KEY_CP   5
// #define D_SGLD_KEY_CS   6
// #define D_SGLD_KEY_SP   7
// #define D_SGLD_KEY_SS   8
// #define D_SGLD_KEY_SUM  9
// #else
// #define D_SGLD_KEY_DL   1 
// #define D_SGLD_KEY_LL   2
// #define D_SGLD_KEY_TP   3
// #define D_SGLD_KEY_TS   4
// #define D_SGLD_KEY_CR   5
// #define D_SGLD_KEY_SH   6
// #define D_SGLD_KEY_SUM  7
// #endif

#define T_STLD_K unsigned int
typedef CArray<T_STLD_K, T_STLD_K> T_STLD_K_LIST;

// struct T_STLD_D
// {
//   enum DIRC
//   {
//     GLOBAL_X = 1;
//     GLOBAL_Y,
//   };
//   
//   CString LoadCaseName;
//   CString LoadCaseType;    // dead, live, wind ...
//   CString Description;
//   unsigned int LoadCaseId;    // 순번
//   
//   //iGen 기초 모듈에서 사용하는 데이터
//   BOOL     bUseHfop   ; //하중 케이스
//   int      nDirection ; //방향
//   
//   void Initialize()
//   {
//     LoadCaseName.Empty();
//     LoadCaseType.Empty();
//     Description.Empty();
//     LoadCaseId  = 0;
//     bUseHfop    = FALSE;
//     nDirection  = GLOBAL_X;
//   }
// };

#define D_STLD_NAME_SIZE 40
struct T_STLD_D
{
	CString LoadCaseName;
	CString LoadCaseType;    // dead, live, wind ...
	CString Description;
	unsigned int LoadCaseId;    // 순번
	void Initialize()
	{
		LoadCaseName.Empty();
		LoadCaseType.Empty();
		Description.Empty();
		LoadCaseId=0;
	}
};

struct T_STLD_D_CH_861
{
	char LoadCaseName[20];
	char LoadCaseType[8];
	char Description[80];
	unsigned int LoadCaseId;
};

struct T_STLD_D_CH
{
	char LoadCaseName[D_STLD_NAME_SIZE];
	char LoadCaseType[8];
	char Description[80];
	unsigned int LoadCaseId;

	void Convert861(T_STLD_D_CH_861& rData)
	{
		memset(LoadCaseName,' ',D_STLD_NAME_SIZE);
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(rData.LoadCaseName));  // 이전버전 변수 크기만큼만.

		memcpy(LoadCaseType, rData.LoadCaseType, sizeof(LoadCaseType));
		memcpy(Description,  rData.Description,  sizeof(Description));
		LoadCaseId = rData.LoadCaseId;
	}

};
struct T_STLD_UDRD_D
{
	T_STLD_K key;
	T_STLD_D data;
};

struct T_STLD_UDRD_D_CH_861
{
	T_STLD_K key;
	T_STLD_D_CH_861 data;
};

struct T_STLD_UDRD_D_CH
{
	T_STLD_K key;
	T_STLD_D_CH data;
};
struct T_STLD_UNIT
{
	T_STLD_UNIT()
	{
		nCount=4;
		for(int i=0; i<nCount; i++)nUnit[i]=D_UNITSYS_NONE;
		LoadCaseName=nUnit[0];
		LoadCaseType=nUnit[1];
		Description=nUnit[2];
		LoadCaseId=nUnit[3];
	}
	int LoadCaseName;
	int LoadCaseType;
	int Description;
	int LoadCaseId;
	int nCount;
	int nUnit[4];
};
#define HASHSIZESTLD 10
#define D_STLD_MINNUM       1   // 입력가능한 최소 정적하중조건번호
#define D_STLD_MAXNUM    9999   // 입력가능한 최대 정적하중조건번호

///////////////// Mec Solver ////////////////////////////////
// 
// Analysis Case Type - Mec DB 에서 사용
enum EN_MEC_ANALCASE
{
	D_MEC_ANALCASE_BEGIN = 1,

	D_MEC_ANALCASE_STAGE = D_MEC_ANALCASE_BEGIN,
	D_MEC_ANALCASE_P_DELTA,
	D_MEC_ANALCASE_STATIC,
	D_MEC_ANALCASE_BUCKLING,
	D_MEC_ANALCASE_PRE_COMPOSITE,
	D_MEC_ANALCASE_MOVING,
	D_MEC_ANALCASE_SETTLE,
	D_MEC_ANALCASE_SUSPENSION,
	D_MEC_ANALCASE_EIGEN,
	D_MEC_ANALCASE_SPECTRUM,
	D_MEC_ANALCASE_LINEAR_HISTORY,
	D_MEC_ANALCASE_NONLINEAR_HISTORY,
	D_MEC_ANALCASE_PUSHOVER,
	D_MEC_ANALCASE_HYDRATION,
	D_MEC_ANALCASE_APP_TEN_LOSS,

	D_MEC_ANALCASE_STAGEPOST_STATIC,
	D_MEC_ANALCASE_STAGEPOST_BUCKLING,

	D_MEC_ANALCASE_END, // it should always be last.

	// analysis case last index
	D_MEC_ANALCASE_LASTIDX = D_MEC_ANALCASE_END - 1
};

// Sub Case Type - Mec DB 에서 사용
enum EN_MEC_SUBCASE
{
	D_MEC_SUBCASE_BEGIN = 1,

	D_MEC_SUBCASE_LINEAR_STATIC = D_MEC_SUBCASE_BEGIN,
	D_MEC_SUBCASE_NONLINEAR_STATIC,
	D_MEC_SUBCASE_GEOMETRY_NONLINEAR,
	D_MEC_SUBCASE_MATERIAL_NONLINEAR,
	D_MEC_SUBCASE_GEO_MAT_NONLINEAR,
	D_MEC_SUBCASE_BUCKLING,
	D_MEC_SUBCASE_P_DELTA,
	D_MEC_SUBCASE_LINEAR_STATIC_PRE_COMPOSITE,
	D_MEC_SUBCASE_NONLINEAR_STATIC_PRE_COMPOSITE,
	D_MEC_SUBCASE_EIGEN,
	D_MEC_SUBCASE_SPECTRUM,
	D_MEC_SUBCASE_MOVING,
	D_MEC_SUBCASE_SETTLE,
	D_MEC_SUBCASE_SUSPENSION,
	D_MEC_SUBCASE_TIME_LINEAR_MODAL_TRANSIENT,
	D_MEC_SUBCASE_TIME_LINEAR_MODAL_PERIODIC,
	D_MEC_SUBCASE_TIME_LINEAR_DIRECT_TRANSIENT,
	D_MEC_SUBCASE_TIME_NONLINEAR_MODAL,
	D_MEC_SUBCASE_TIME_NONLINEAR_DIRECT,
	D_MEC_SUBCASE_TIME_NONLINEAR_DIRECT_LARGE_DISP,
	D_MEC_SUBCASE_TIME_NONLINEAR_DIRECT_P_DELTA,
	D_MEC_SUBCASE_TIME_NONLINEAR_STATIC_LOAD,
	D_MEC_SUBCASE_TIME_NONLINEAR_STATIC_DISP,
	D_MEC_SUBCASE_TIME_NONLINEAR_STATIC_LARGE_DISP_LOAD,
	D_MEC_SUBCASE_TIME_NONLINEAR_STATIC_LARGE_DISP_DISP,
	D_MEC_SUBCASE_TIME_NONLINEAR_STATIC_P_DELTA_LOAD,
	D_MEC_SUBCASE_TIME_NONLINEAR_STATIC_P_DELTA_DISP,
	D_MEC_SUBCASE_PUSHOVER_NONLINEAR_STATIC,
	D_MEC_SUBCASE_PUSHOVER_GEOMETRY_NONLINEAR,
	D_MEC_SUBCASE_PUSHOVER_P_DELTA,
	D_MEC_SUBCASE_HYDRATION,
	D_MEC_SUBCASE_LINEAR_STATIC_APP_TEN_LOSS,
	D_MEC_SUBCASE_LINEAR_STATIC_APP_TEN_LOSS_TEN_LOAD,
	D_MEC_SUBCASE_LINEAR_STATIC_APP_TEN_LOSS_APTL,
	D_MEC_SUBCASE_NONLINEAR_STATIC_APP_TEN_LOSS,
	D_MEC_SUBCASE_NONLINEAR_STATIC_APP_TEN_LOSS_TEN_LOAD,
	D_MEC_SUBCASE_NONLINEAR_STATIC_APP_TEN_LOSS_APTL,


	D_MEC_SUBCASE_END, // it should always be last.

	// subcase last index
	D_MEC_SUBCASE_LASTIDX = D_MEC_SUBCASE_END - 1
};

//*^^* LOADCASE 추가
#define D_LCOM_STATIC         1
#define D_LCOM_SPECTRUM       2
#define D_LCOM_HISTORY        3
#define D_LCOM_MOVING         4
#define D_LCOM_SETTLEMENT     5
#define D_LCOM_ENVELOPE       6
#define D_LCOM_CB_GENERAL     7
#define D_LCOM_CB_STEEL       8
#define D_LCOM_CB_CONCRETE    9
#define D_LCOM_CB_FOOTING     10
#define D_LCOM_CB_SRC         11
#define D_LCOM_PUSHOVER       12
#define D_LCOM_STAGE          13
#define D_LCOM_ECCENSPEC      14    // 우발 편심 때문에 Case 추가
#define D_LCOM_GRIDMODEL      15
#define D_LCOM_THEX           16    // 동해석 관련(후처리에서만 생기는 LoadCase)
#define D_LCOM_THAM           17    // 동해석 관련(후처리에서만 생기는 LoadCase)
#define D_LCOM_THPO           18    // 동해석 관련(후처리에서만 생기는 LoadCase)
#define D_LCOM_CRANE          19    // 중국 gen crane 하중
#define D_LCOM_CB_ALUMINUM    20    // 국내 알루미늄 하중조합 추가 // PMS:XXXX-THBAE-20130923
#define D_LCOM_CB_STLCOMP     21
#define D_LCOM_CB_CFSTEEL     22
#define D_LCOM_CB_SEISMIC     23
#define D_LCOM_CB_HISTORY     24
#define D_LCOM_CB_LINEAR_EVAL 25
#define D_LCOM_SPECTRUM_ND    26  // Non-Dissipative 용 SPECTRUM
#define D_LCOM_ECCENSPEC_ND   27  // Non-Dissipative 용 ECCENSPEC
#define D_LCOM_CB_JSONFILE    28
#define D_LCOM_NL_HISTORY	  29
#define D_LCOM_STAGPOST_STATIC  30

#define D_LCOM_LAST_IDX     D_LCOM_STAGPOST_STATIC    // 하중 타입 추가시 변경 부탁드립니다.
///////////////////////////////////////////////////////////////////////////////////////////

#define D_LCOMTYPE_GENERAL     1
#define D_LCOMTYPE_STEEL       2
#define D_LCOMTYPE_CONCRETE    3
#define D_LCOMTYPE_FDN         4
#define D_LCOMTYPE_SRC         5
#define D_LCOMTYPE_ALUMINUM    6
#define D_LCOMTYPE_STLCOMP     7
#define D_LCOMTYPE_CFSTEEL     8
#define D_LCOMTYPE_SEISMIC     9
#define D_LCOMTYPE_LINEAR_EVAL 10  // 추가후 Stagctrl.cpp에서 D_LCOMTYPE_GENERAL 검색해서 같은 방식으로 처리 해야 함.
                                   // 추가하지 않으면 pre에서 입력한 하중조합이 post에서 사라짐.
#define D_LCOMTYPE_MAX        D_LCOMTYPE_LINEAR_EVAL // LCOMTYPE이 추가되면 같이 수정해야 함.

#define D_LCOM_NUMCOMB_850  150
#define D_LCOM_NUMCOMB      999
#define T_LCOM_K unsigned int
typedef CArray<T_LCOM_K, T_LCOM_K> T_LCOM_K_LIST;
struct T_LCOM_BASE
{
	unsigned int AnalType;   //D_LCOM_???
	unsigned int LoadCaseKey;
	double Factor;

    T_LCOM_BASE()
    {
        Initialize();
    }

    T_LCOM_BASE(UINT unType, UINT nLcasK, double dFact)
    {
        AnalType = unType;
        LoadCaseKey = nLcasK;
        Factor = dFact;
    }

	int T_LCOM_BASE::operator == (const T_LCOM_BASE& rData)	
	{
		if(AnalType != rData.AnalType) return FALSE;
		if(LoadCaseKey != rData.LoadCaseKey) return FALSE;
		if(Factor != rData.Factor) return FALSE;

		return TRUE;
	}
	void Initialize()
	{
		AnalType =0;
		LoadCaseKey =0;
		Factor = 0.0;
	}
};

typedef CArray<T_LCOM_BASE, T_LCOM_BASE&> T_LCOM_BASE_LIST;

enum EN_EVAL_ACTIVE
{
	EN_EVAL_NONE = 0,
	EN_EVAL_GENE,
	EN_EVAL_VERT,
	EN_EVAL_END
};

enum EN_LNEVAL_ACTIVE
{
	EN_LNEVAL_NONE = 0,
	EN_LNEVAL_GRAV,
	EN_LNEVAL_SEIS,
	EN_LNEVAL_SPEC,
	EN_LNEVAL_VERT,
	EN_LNEVAL_END
};

enum EN_GEN_ACTIVE
{
	EN_GEN_NONE = 0,
	EN_GEN_ACT,
	EN_GEN_END,
};

enum EN_DGN_ACTIVE
{
	EN_DGN_NONE = 0,
	EN_DGN_STRN,
	EN_DGN_SERV,
	EN_DGN_SPEC,
	EN_DGN_VERT,
    EN_DGN_STRE, /// Strength (Elastic)
	EN_DGN_UG_STRN,
	EN_DGN_UG_SERV,
	EN_DGN_UG_SPEC,
	EN_DGN_ACTIVE_END
};

enum EN_DGN_LCOMTYPE
{
	EN_DGN_ADD = 0,
	EN_DGN_ENV,
	EN_DGN_ABS,
	EN_DGN_SRSS
};


// 540에서 bActive가 nActive로 변경됨
// Size 변경 없고 값 변환 필요 없음
struct T_LCOM_D
{
	CString LoadCombName;
	int     LoadCombType;       // 0,1,2
	CString Description;
	int     nActive;            // 0:inactive, 1:active or strength/stress(CIVIL RC), 2:serviceability(CIVIL RC), 5:strength(Elastic), 6:U.G_Strength , 7:U.G_serviceability, 8:U.G_Special
	BOOL    bElasticStage;      // 탄성단계 : 6.7.1에서 추가
	unsigned int LoadCombId;
	CArray<T_LCOM_BASE, T_LCOM_BASE&>aCombination;
	//T_LCOM_BASE  Combination[D_LCOM_NUMCOMB];
	int     nServLcomType;      //(2009.11.17) Add by Unsang :: Serviceability load combination type!
	int     nSubLcomType;       // 설계용 하중조합 구분 (Strength, Service, Fatigue ...)
	int     nSeisLcomType;      // 국내내진설계용 하중조합 구분 (Seismic Load Combination Type)

	double  dLcomFactor;        //(v950) 일본 Steel 설계

	void Initialize()
	{
		LoadCombName.Empty();
		LoadCombType=0;
		Description.Empty();
		nActive=1;
		bElasticStage = FALSE;
		LoadCombId=0;
		aCombination.RemoveAll();
//     for(int i=0; i<D_LCOM_NUMCOMB; i++)
//     {
//       Combination[i].AnalType=0;  // D_LCOM_???
//       Combination[i].LoadCaseKey=0;
//       Combination[i].Factor=0.;
//     }
		nServLcomType = 0;
		nSubLcomType = 0;
		nSeisLcomType = 0;
		dLcomFactor = 1.0;
	}
	T_LCOM_D()  {Initialize(); }
	T_LCOM_D(const T_LCOM_D& src) { *this = src; }
	T_LCOM_D& operator=(const T_LCOM_D& src)
	{
		LoadCombName = src.LoadCombName;
		LoadCombType = src.LoadCombType;
		Description  = src.Description;
		nActive      = src.nActive;
		bElasticStage= src.bElasticStage;
		LoadCombId   = src.LoadCombId;
		aCombination.Copy(src.aCombination);
		nServLcomType = src.nServLcomType;
		nSubLcomType  = src.nSubLcomType;
		nSeisLcomType = src.nSeisLcomType;
		dLcomFactor   = src.dLcomFactor;
		return *this;
	}
};
struct T_LCOM_D_UL  // Normalize 용
{
	CString LoadCombName;
	int     LoadCombType;       // 0,1,2
	CString Description;
	int     nActive;
	BOOL    bElasticStage;      // 탄성단계 : 6.7.1에서 추가
	unsigned int LoadCombId;
	CArray<T_LCOM_BASE, T_LCOM_BASE&>Combination;
	int     nServLcomType;      //(2009.11.17) Add by Unsang :: Serviceability load combination type!
	int     nSubLcomType;
	int     nSeisLcomType;
	double  dLcomFactor;

	T_LCOM_D_UL(){}
	T_LCOM_D_UL(const T_LCOM_D_UL& rSrc) { *this = rSrc; }
	T_LCOM_D_UL(const T_LCOM_D& rSrc) { *this = rSrc; }
	T_LCOM_D_UL& operator=(const T_LCOM_D_UL& rData)
	{
		LoadCombName = rData.LoadCombName;
		LoadCombType = rData.LoadCombType;
		Description = rData.Description;
		nActive = rData.nActive;
		bElasticStage = rData.bElasticStage;
		LoadCombId = rData.LoadCombId;
		Combination.Copy(rData.Combination);
		nServLcomType = rData.nServLcomType;
		nSubLcomType = rData.nSubLcomType;
		nSeisLcomType = rData.nSeisLcomType;
		dLcomFactor = rData.dLcomFactor;

		return *this;
	}
	T_LCOM_D_UL& operator=(const T_LCOM_D& rData)
	{
		LoadCombName = rData.LoadCombName;
		LoadCombType = rData.LoadCombType;
		Description = rData.Description;
		nActive = rData.nActive;
		bElasticStage = rData.bElasticStage;
		LoadCombId = rData.LoadCombId;
		Combination.RemoveAll();
		Combination.Copy(rData.aCombination);
//     for(int i=0; i<D_LCOM_NUMCOMB; i++)
//     {
//       if (rData.Combination[i].LoadCaseKey == 0) break;
//       Combination.Add((T_LCOM_BASE&)rData.Combination[i]);
//     }
		nServLcomType = rData.nServLcomType;
		nSubLcomType = rData.nSubLcomType;
		nSeisLcomType = rData.nSeisLcomType;
		dLcomFactor = rData.dLcomFactor;

		return *this;
	}
	void Initialize()
	{
		LoadCombName.Empty();
		LoadCombType=0;
		Description.Empty();
		nActive=1;
		bElasticStage = FALSE;
		LoadCombId=0;
		Combination.RemoveAll();
		nServLcomType = 0;
		nSubLcomType = 0;
		nSeisLcomType = 0;
		dLcomFactor = 1.0;
	}
};
struct T_LCOM_D_CH_40
{
	char LoadCombName[20];
	int LoadCombType;
	char Description[80];
	unsigned int LoadCombId;
	T_LCOM_BASE Combination[50];
};
struct T_LCOM_D_CH_520_ALPHA
{
	char LoadCombName[20];
	int LoadCombType;
	char Description[80];
	BOOL bActive;
	unsigned int LoadCombId;
	T_LCOM_BASE Combination[50];
};
struct T_LCOM_D_CH_670
{
	char LoadCombName[20];
	int LoadCombType;
	char Description[80];
	int nActive;
	unsigned int LoadCombId;
	T_LCOM_BASE Combination[150];
};
struct T_LCOM_D_CH_780
{
	char LoadCombName[20];
	int LoadCombType;
	char Description[80];
	int nActive;
	BOOL bElasticStage;
	unsigned int LoadCombId;
	T_LCOM_BASE Combination[D_LCOM_NUMCOMB_850];
};
struct T_LCOM_D_CH_785
{
	char LoadCombName[20];
	int LoadCombType;
	char Description[80];
	int nActive;
	BOOL bElasticStage;
	unsigned int LoadCombId;
	T_LCOM_BASE Combination[D_LCOM_NUMCOMB_850];
	int nServLcomType;      //(2009.11.17) Add by Unsang :: Serviceability load combination type!
};
struct T_LCOM_D_CH_850
{
	char LoadCombName[20];
	int LoadCombType;
	char Description[80];
	int nActive;
	BOOL bElasticStage;
	unsigned int LoadCombId;
	T_LCOM_BASE Combination[D_LCOM_NUMCOMB_850];
	int nServLcomType;      //(2009.11.17) Add by Unsang :: Serviceability load combination type!]
	int nSubLcomType;
};
struct T_LCOM_D_CH_856
{
	char LoadCombName[20];
	int LoadCombType;
	char Description[80];
	int nActive;
	BOOL bElasticStage;
	unsigned int LoadCombId;
	T_LCOM_BASE Combination[D_LCOM_NUMCOMB];
	int nServLcomType;      //(2009.11.17) Add by Unsang :: Serviceability load combination type!]
	int nSubLcomType;
};
struct T_LCOM_D_CH_881
{
	char LoadCombName[20];
	int LoadCombType;
	char Description[80];
	int nActive;
	BOOL bElasticStage;
	unsigned int LoadCombId;
	CArray<T_LCOM_BASE, T_LCOM_BASE&> aCombination;
	int nServLcomType;      //(2009.11.17) Add by Unsang :: Serviceability load combination type!]
	int nSubLcomType;
};
struct T_LCOM_D_CH_890
{
	char LoadCombName[20];
	int LoadCombType;
	char Description[80];
	int nActive;
	BOOL bElasticStage;
	unsigned int LoadCombId;
	CArray<T_LCOM_BASE, T_LCOM_BASE&> aCombination;
	int nServLcomType;      //(2009.11.17) Add by Unsang :: Serviceability load combination type!]
	int nSubLcomType;
	int nSeisLcomType;
};
#define D_LCOM_DESC_SIZE 100
struct T_LCOM_D_CH_945
{
	char LoadCombName[20];
	int LoadCombType;
	char Description[D_LCOM_DESC_SIZE];
	int nActive;
	BOOL bElasticStage;
	unsigned int LoadCombId;
	CArray<T_LCOM_BASE, T_LCOM_BASE&> aCombination;
	int nServLcomType;      //(2009.11.17) Add by Unsang :: Serviceability load combination type!]
	int nSubLcomType;
	int nSeisLcomType;
};
struct T_LCOM_D_CH
{
	char LoadCombName[20];
	int LoadCombType;
	char Description[D_LCOM_DESC_SIZE];
	int nActive;
	BOOL bElasticStage;
	unsigned int LoadCombId;
	CArray<T_LCOM_BASE, T_LCOM_BASE&> aCombination;
	int nServLcomType;      //(2009.11.17) Add by Unsang :: Serviceability load combination type!]
	int nSubLcomType;
	int nSeisLcomType;
	double dLcomFactor;

	void ConvertToString(T_LCOM_D& rData)
	{
		ConvertCharStr(LoadCombName, rData.LoadCombName, sizeof(LoadCombName));
		rData.LoadCombType=LoadCombType;
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.nActive=nActive;
		rData.bElasticStage=bElasticStage;
		rData.LoadCombId=LoadCombId;
		rData.aCombination.Copy(aCombination);
		rData.nServLcomType=nServLcomType;
		rData.nSubLcomType=nSubLcomType;
		rData.nSeisLcomType=nSeisLcomType;
		rData.dLcomFactor = dLcomFactor;
	}
	void ConvertToChar(T_LCOM_D& rData)
	{
		ConvertStrChar(rData.LoadCombName, LoadCombName, sizeof(LoadCombName));
		LoadCombType=rData.LoadCombType;
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		nActive=rData.nActive;
		bElasticStage = rData.bElasticStage;
		LoadCombId=rData.LoadCombId;
		aCombination.Copy(rData.aCombination);
		nServLcomType=rData.nServLcomType;
		nSubLcomType=rData.nSubLcomType;
		nSeisLcomType=rData.nSeisLcomType;
		dLcomFactor = rData.dLcomFactor;
	}
	void Convert40(T_LCOM_D_CH_40& rData)
	{
		memcpy(LoadCombName, rData.LoadCombName, __min(sizeof(LoadCombName), sizeof(rData.LoadCombName)));
		LoadCombType=rData.LoadCombType;
		memset(Description,' ',D_LCOM_DESC_SIZE);
		memcpy(Description, rData.Description, __min(sizeof(Description), sizeof(rData.Description)));
		nActive=1;
		bElasticStage = FALSE;
		LoadCombId=rData.LoadCombId;
		for(int i=0; i<50; i++)
		{
			if(rData.Combination[i].LoadCaseKey==0) break;
			aCombination.Add(rData.Combination[i]);
		}
//     for(; i < D_LCOM_NUMCOMB; i++) 
//     {
//       Combination[i].AnalType=0;  // D_LCOM_???
//       Combination[i].LoadCaseKey=0;
//       Combination[i].Factor=0.;
//     }
		nServLcomType = 0;
		nSubLcomType = 0;
		nSeisLcomType = 0;
		dLcomFactor = 1.0;
	}
	void Convert520Alpha(T_LCOM_D_CH_520_ALPHA& rData)
	{
		memcpy(LoadCombName, rData.LoadCombName, sizeof(LoadCombName));
		LoadCombType=rData.LoadCombType;
		memset(Description,' ',D_LCOM_DESC_SIZE);
		memcpy(Description, rData.Description, __min(sizeof(Description), sizeof(rData.Description)));
		nActive=rData.bActive;
		bElasticStage = FALSE;
		LoadCombId=rData.LoadCombId;
		for(int i=0; i<50; i++)
		{
			if(rData.Combination[i].LoadCaseKey==0) break;
			aCombination.Add(rData.Combination[i]);
		}

//     for(int i=0; i<50; i++)Combination[i]=rData.Combination[i];
//     for(; i < D_LCOM_NUMCOMB; i++) 
//     {
//       Combination[i].AnalType=0;  // D_LCOM_???
//       Combination[i].LoadCaseKey=0;
//       Combination[i].Factor=0.;
//     }
		nServLcomType = 0;
		nSubLcomType = 0;
		nSeisLcomType = 0;
		dLcomFactor = 1.0;
	}
	void Convert670(T_LCOM_D_CH_670& rData)
	{
		memcpy(LoadCombName, rData.LoadCombName, sizeof(LoadCombName));
		LoadCombType=rData.LoadCombType;
		memset(Description,' ',D_LCOM_DESC_SIZE);
		memcpy(Description, rData.Description, __min(sizeof(Description), sizeof(rData.Description)));
		nActive=rData.nActive;
		bElasticStage = FALSE;
		LoadCombId=rData.LoadCombId;
		for(int i=0; i<150; i++)
		{
			if(rData.Combination[i].LoadCaseKey==0) break;
			aCombination.Add(rData.Combination[i]);
		}

		//memcpy(Combination, rData.Combination, sizeof(Combination));
		nServLcomType = 0;
		nSubLcomType = 0;
		nSeisLcomType = 0;
		dLcomFactor = 1.0;
	}
	void Convert780(T_LCOM_D_CH_780& rData)
	{
		memcpy(LoadCombName, rData.LoadCombName, sizeof(LoadCombName));
		LoadCombType=rData.LoadCombType;
		memset(Description,' ',D_LCOM_DESC_SIZE);
		memcpy(Description, rData.Description, __min(sizeof(Description), sizeof(rData.Description)));
		nActive=rData.nActive;

#if defined(_MGEN)
		if(rData.bElasticStage == 0 || rData.bElasticStage == 1) 
		{
			bElasticStage = rData.bElasticStage;
			nServLcomType = 0;  
		}
		else if(rData.bElasticStage == 2)
		{
			bElasticStage = FALSE;
			nServLcomType = 1;  
		}
		else if(rData.bElasticStage == 3)
		{
			bElasticStage = FALSE;
			nServLcomType = 2;
		}
		else if(rData.bElasticStage == 4)
		{
			bElasticStage = FALSE;
			nServLcomType = 3;
		}
#elif defined(_CIVIL)
		bElasticStage = rData.bElasticStage;
		nServLcomType = 0;
#endif

		LoadCombId=rData.LoadCombId;
		for(int i=0; i<D_LCOM_NUMCOMB_850; i++)
		{
			if(rData.Combination[i].LoadCaseKey==0) break;
			aCombination.Add(rData.Combination[i]);
		}
		//memcpy(Combination, rData.Combination, sizeof(Combination));
		nSubLcomType = 0;
		nSeisLcomType = 0;
		dLcomFactor = 1.0;
	}
	void Convert785(T_LCOM_D_CH_785& rData)
	{
		memcpy(LoadCombName, rData.LoadCombName, sizeof(LoadCombName));
		LoadCombType=rData.LoadCombType;
		memset(Description,' ',D_LCOM_DESC_SIZE);
		memcpy(Description, rData.Description, __min(sizeof(Description), sizeof(rData.Description)));
		nActive=rData.nActive;
		bElasticStage=rData.bElasticStage;
		LoadCombId=rData.LoadCombId;
		for(int i=0; i<D_LCOM_NUMCOMB_850; i++)
		{
			if(rData.Combination[i].LoadCaseKey==0) break;
			aCombination.Add(rData.Combination[i]);
		}
		//memcpy(Combination, rData.Combination, sizeof(Combination));
		nServLcomType=rData.nServLcomType;
		nSubLcomType = 0;
		nSeisLcomType = 0;
		dLcomFactor = 1.0;
	}
	void Convert850(T_LCOM_D_CH_850& rData)
	{
		memcpy(LoadCombName, rData.LoadCombName, sizeof(LoadCombName));
		LoadCombType=rData.LoadCombType;
		memset(Description,' ',D_LCOM_DESC_SIZE);
		memcpy(Description, rData.Description, __min(sizeof(Description), sizeof(rData.Description)));
		nActive=rData.nActive;
		bElasticStage=rData.bElasticStage;
		LoadCombId=rData.LoadCombId;
		for(int i=0; i<D_LCOM_NUMCOMB_850; i++)
		{
			if(rData.Combination[i].LoadCaseKey==0) break;
			aCombination.Add(rData.Combination[i]);
		}
// 		for(int i=0; i<D_LCOM_NUMCOMB; i++)
// 		{
// 			if(i < D_LCOM_NUMCOMB_850) 
// 			{
// 				Combination[i] = rData.Combination[i];
// 			}
// 			else
// 			{
// 				Combination[i].AnalType=0;
// 				Combination[i].LoadCaseKey=0;
// 				Combination[i].Factor=0.0;
// 			}
// 		}
		nServLcomType=rData.nServLcomType;
		nSubLcomType = rData.nSubLcomType;
		nSeisLcomType = 0;
		dLcomFactor = 1.0;
	}
	void Convert856(T_LCOM_D_CH_856& rData)
	{
		memcpy(LoadCombName, rData.LoadCombName, sizeof(LoadCombName));
		LoadCombType=rData.LoadCombType;
		memset(Description,' ',D_LCOM_DESC_SIZE);
		memcpy(Description, rData.Description, __min(sizeof(Description), sizeof(rData.Description)));
		nActive=rData.nActive;
		bElasticStage=rData.bElasticStage;
		LoadCombId=rData.LoadCombId;
		for(int i=0; i<D_LCOM_NUMCOMB; i++)
		{
			if(rData.Combination[i].LoadCaseKey==0) break;
			aCombination.Add(rData.Combination[i]);
		}
		nServLcomType=rData.nServLcomType;
		nSubLcomType = rData.nSubLcomType;
		nSeisLcomType = 0;
		dLcomFactor = 1.0;
	}
	void Convert881(T_LCOM_D_CH_881& rData)
	{
		memcpy(LoadCombName, rData.LoadCombName, sizeof(LoadCombName));
		LoadCombType=rData.LoadCombType;
		memset(Description,' ',D_LCOM_DESC_SIZE);
		memcpy(Description, rData.Description, __min(sizeof(Description), sizeof(rData.Description)));
		nActive=rData.nActive;
		bElasticStage=rData.bElasticStage;
		LoadCombId=rData.LoadCombId;
		aCombination.Copy(rData.aCombination);
		nServLcomType=rData.nServLcomType;
		nSubLcomType = rData.nSubLcomType;
		nSeisLcomType = 0;
		dLcomFactor = 1.0;
	}
	void Convert890(T_LCOM_D_CH_890& rData)
	{
		memcpy(LoadCombName, rData.LoadCombName, sizeof(LoadCombName));
		LoadCombType=rData.LoadCombType;
		memset(Description,' ',D_LCOM_DESC_SIZE);
		memcpy(Description, rData.Description, __min(sizeof(Description), sizeof(rData.Description)));
		nActive=rData.nActive;
		bElasticStage=rData.bElasticStage;
		LoadCombId=rData.LoadCombId;
		aCombination.Copy(rData.aCombination);
		nServLcomType=rData.nServLcomType;
		nSubLcomType = rData.nSubLcomType;
		nSeisLcomType = rData.nSeisLcomType;
		dLcomFactor = 1.0;
	}
	void Convert945(T_LCOM_D& rData)
	{
		memcpy(LoadCombName, rData.LoadCombName, sizeof(LoadCombName));
		LoadCombType = rData.LoadCombType;
		memset(Description, ' ', D_LCOM_DESC_SIZE);
		memcpy(Description, rData.Description, __min(sizeof(Description), sizeof(rData.Description)));
		nActive = rData.nActive;
		bElasticStage = rData.bElasticStage;
		LoadCombId = rData.LoadCombId;
		aCombination.Copy(rData.aCombination);
		nServLcomType = rData.nServLcomType;
		nSubLcomType = rData.nSubLcomType;
		nSeisLcomType = rData.nSeisLcomType;
		dLcomFactor = 1.0;
	}

};
struct T_LCOM_UDRD_D
{
	T_LCOM_K key;
	T_LCOM_D data;
};
struct T_LCOM_UDRD_D_CH_40
{
	T_LCOM_K key;
	T_LCOM_D_CH_40 data;
};
struct T_LCOM_UDRD_D_CH_520_ALPHA
{
	T_LCOM_K key;
	T_LCOM_D_CH_520_ALPHA data;
};
struct T_LCOM_UDRD_D_CH_670
{
	T_LCOM_K key;
	T_LCOM_D_CH_670 data;
};
struct T_LCOM_UDRD_D_CH_780
{
	T_LCOM_K key;
	T_LCOM_D_CH_780 data;
};
struct T_LCOM_UDRD_D_CH_785
{
	T_LCOM_K key;
	T_LCOM_D_CH_785 data;
};
struct T_LCOM_UDRD_D_CH_850
{
	T_LCOM_K key;
	T_LCOM_D_CH_850 data;
};
struct T_LCOM_UDRD_D_CH_856
{
	T_LCOM_K key;
	T_LCOM_D_CH_856 data;
};
struct T_LCOM_UDRD_D_CH_881
{
	T_LCOM_K key;
	T_LCOM_D_CH_881 data;
};
struct T_LCOM_UDRD_D_CH_890
{
	T_LCOM_K key;
	T_LCOM_D_CH_890 data;
};
struct T_LCOM_UDRD_D_CH_945
{
	T_LCOM_K key;
	T_LCOM_D_CH_945 data;
};
struct T_LCOM_UDRD_D_CH
{
	T_LCOM_K key;
	T_LCOM_D_CH data;
};
struct T_LCOM_UDRD_D_CH_RW_860_x86
{
	T_LCOM_K key;
	char LoadCombName[20];
	int LoadCombType;
	char Description[80];
	int nActive;
	BOOL bElasticStage;
	unsigned int LoadCombId;
	//CArray<T_LCOM_BASE, T_LCOM_BASE&> aCombination;
	char Dummy[20];
	int nServLcomType;      //(2009.11.17) Add by Unsang :: Serviceability load combination type!]
	int nSubLcomType;
	int nCountCombination;
};
struct T_LCOM_UDRD_D_CH_RW_860_x64
{
	T_LCOM_K key;
	char LoadCombName[20];
	int LoadCombType;
	char Description[80];
	int nActive;
	BOOL bElasticStage;
	unsigned int LoadCombId;
	//CArray<T_LCOM_BASE, T_LCOM_BASE&> aCombination;
	char Dummy[40];
	int nServLcomType;      //(2009.11.17) Add by Unsang :: Serviceability load combination type!]
	int nSubLcomType;
	int nCountCombination;
};
struct T_LCOM_UDRD_D_CH_RW_881
{
	T_LCOM_K key;
	char LoadCombName[20];
	int LoadCombType;
	char Description[80];
	int nActive;
	BOOL bElasticStage;
	unsigned int LoadCombId;
	int nServLcomType;      //(2009.11.17) Add by Unsang :: Serviceability load combination type!]
	int nSubLcomType;
	int nCountCombination;
};
struct T_LCOM_UDRD_D_CH_RW_890
{
	T_LCOM_K key;
	char LoadCombName[20];
	int LoadCombType;
	char Description[80];
	int nActive;
	BOOL bElasticStage;
	unsigned int LoadCombId;
	int nServLcomType;      //(2009.11.17) Add by Unsang :: Serviceability load combination type!]
	int nSubLcomType;
	int nCountCombination;
	int nSeisLcomType;
};
struct T_LCOM_UDRD_D_CH_RW_945
{
	T_LCOM_K key;
	char LoadCombName[20];
	int LoadCombType;
	char Description[D_LCOM_DESC_SIZE];
	int nActive;
	BOOL bElasticStage;
	unsigned int LoadCombId;
	int nServLcomType;      //(2009.11.17) Add by Unsang :: Serviceability load combination type!]
	int nSubLcomType;
	int nCountCombination;
	int nSeisLcomType;
};
struct T_LCOM_UDRD_D_CH_RW
{
	T_LCOM_K key;
	char LoadCombName[20];
	int LoadCombType;
	char Description[D_LCOM_DESC_SIZE];
	int nActive;
	BOOL bElasticStage;
	unsigned int LoadCombId;
	int nServLcomType;      //(2009.11.17) Add by Unsang :: Serviceability load combination type!]
	int nSubLcomType;
	int nCountCombination;
	int nSeisLcomType;
	double dLcomFactor;

	void GetLcom(T_LCOM_K& rKey, T_LCOM_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.LoadCombName, LoadCombName, sizeof(LoadCombName));
		memcpy(rData.Description, Description, sizeof(Description));
		rData.LoadCombType  = LoadCombType;
		rData.nActive       = nActive;
		rData.bElasticStage = bElasticStage;
		rData.LoadCombId    = LoadCombId;
		rData.nServLcomType = nServLcomType;
		rData.nSubLcomType  = nSubLcomType;
		rData.nSeisLcomType = nSeisLcomType;
		rData.dLcomFactor   = dLcomFactor;
		//rData.aCombination.Copy(aCombination);    
	}
	void SetLcom(const T_LCOM_K Key, const T_LCOM_D_CH& rData)
	{
		key=Key;
		memcpy(LoadCombName, rData.LoadCombName, sizeof(LoadCombName));
		memcpy(Description,  rData.Description, sizeof(Description));
		LoadCombType  = rData.LoadCombType;
		nActive       = rData.nActive;
		bElasticStage = rData.bElasticStage;
		LoadCombId    = rData.LoadCombId;
		nServLcomType = rData.nServLcomType;
		nSubLcomType  = rData.nSubLcomType;
		nCountCombination = (int)rData.aCombination.GetSize();
		nSeisLcomType = rData.nSeisLcomType;
		dLcomFactor   = rData.dLcomFactor;
	}
	void Convert860_x86(T_LCOM_UDRD_D_CH_RW_860_x86& rData860)
	{
		key = rData860.key;
		memcpy(LoadCombName, rData860.LoadCombName, sizeof(LoadCombName));
		memset(Description,' ',D_LCOM_DESC_SIZE);
		memcpy(Description,  rData860.Description, sizeof(rData860.Description));
		LoadCombType      = rData860.LoadCombType;
		nActive           = rData860.nActive;
		bElasticStage     = rData860.bElasticStage;
		LoadCombId        = rData860.LoadCombId;
		nServLcomType     = rData860.nServLcomType;
		nSubLcomType      = rData860.nSubLcomType;
		nCountCombination = rData860.nCountCombination;
		nSeisLcomType = 0;
		dLcomFactor   = 1.0;
	}
	void Convert860_x64(T_LCOM_UDRD_D_CH_RW_860_x64& rData860)
	{
		key = rData860.key;
		memcpy(LoadCombName, rData860.LoadCombName, sizeof(LoadCombName));
		memset(Description,' ',D_LCOM_DESC_SIZE);
		memcpy(Description,  rData860.Description, sizeof(rData860.Description));
		LoadCombType      = rData860.LoadCombType;
		nActive           = rData860.nActive;
		bElasticStage     = rData860.bElasticStage;
		LoadCombId        = rData860.LoadCombId;
		nServLcomType     = rData860.nServLcomType;
		nSubLcomType      = rData860.nSubLcomType;
		nCountCombination = rData860.nCountCombination;
		nSeisLcomType = 0;
		dLcomFactor   = 1.0;
	}
	void Convert881(T_LCOM_UDRD_D_CH_RW_881& rData)
	{
		key = rData.key;
		memcpy(LoadCombName, rData.LoadCombName, sizeof(LoadCombName));
		memset(Description,' ',D_LCOM_DESC_SIZE);
		memcpy(Description,  rData.Description, sizeof(rData.Description));
		LoadCombType      = rData.LoadCombType;
		nActive           = rData.nActive;
		bElasticStage     = rData.bElasticStage;
		LoadCombId        = rData.LoadCombId;
		nServLcomType     = rData.nServLcomType;
		nSubLcomType      = rData.nSubLcomType;
		nCountCombination = rData.nCountCombination;
		nSeisLcomType = 0;
		dLcomFactor   = 1.0;
	}
	void Convert890(T_LCOM_UDRD_D_CH_RW_890& rData)
	{
		key = rData.key;
		memcpy(LoadCombName, rData.LoadCombName, sizeof(LoadCombName));
		memset(Description,' ',D_LCOM_DESC_SIZE);
		memcpy(Description,  rData.Description, sizeof(rData.Description));
		LoadCombType      = rData.LoadCombType;
		nActive           = rData.nActive;
		bElasticStage     = rData.bElasticStage;
		LoadCombId        = rData.LoadCombId;
		nServLcomType     = rData.nServLcomType;
		nSubLcomType      = rData.nSubLcomType;
		nCountCombination = rData.nCountCombination;
		nSeisLcomType = rData.nSeisLcomType;
		dLcomFactor   = 1.0;
	}
	void Convert945(T_LCOM_UDRD_D_CH_RW_945& rData)
	{
		key = rData.key;
		memcpy(LoadCombName, rData.LoadCombName, sizeof(LoadCombName));
		memcpy(Description,  rData.Description, sizeof(Description));
		LoadCombType      = rData.LoadCombType;
		nActive           = rData.nActive;
		bElasticStage     = rData.bElasticStage;
		LoadCombId        = rData.LoadCombId;
		nServLcomType     = rData.nServLcomType;
		nSubLcomType      = rData.nSubLcomType;
		nCountCombination = rData.nCountCombination;
		nSeisLcomType     = rData.nSeisLcomType;
		dLcomFactor       = 1.0;
	}
};
struct T_LCOM_UNIT
{
	T_LCOM_UNIT()
	{
		LoadCombName=D_UNITSYS_NONE;
		LoadCombType=D_UNITSYS_NONE;
		Description=D_UNITSYS_NONE;
		nActive=D_UNITSYS_NONE;
		LoadCombId=D_UNITSYS_NONE;
		Combination=D_UNITSYS_NONE;
	}
	int LoadCombName;
	int LoadCombType;
	int Description;
	int nActive;
	int LoadCombId;
	int Combination;
};

// Load Combination Type
#define T_LCMT_K unsigned int
struct T_LCMT_BASE_830
{
	CArray<T_LCOM_K, T_LCOM_K&> aStrength;
	CArray<T_LCOM_K, T_LCOM_K&> aService;
	CArray<T_LCOM_K, T_LCOM_K&> aUltimate;
	CArray<T_LCOM_K, T_LCOM_K&> aFatigue;
};

struct T_LCMT_BASE
{
	CArray<T_LCOM_K, T_LCOM_K&> aStrength;
	CArray<T_LCOM_K, T_LCOM_K&> aService;
	CArray<T_LCOM_K, T_LCOM_K&> aUltimate;
	CArray<T_LCOM_K, T_LCOM_K&> aFatigue;
	CArray<T_LCOM_K, T_LCOM_K&> aFatigue2;
	
	void Initialize()
	{
		aStrength.RemoveAll();
		aService.RemoveAll();
		aUltimate.RemoveAll();
		aFatigue.RemoveAll();
		aFatigue2.RemoveAll();
	}
};

struct T_LCMT_D_830
{
	T_LCMT_BASE_830 Base[2]; // 0:Steel Design 1:Concrete Design
};

struct T_LCMT_D
{
	T_LCMT_BASE Base[3]; // 0:Steel Design 1:Concrete Design 2:Steel Composite

	void Initialize()
	{
		for(int i=0; i<3; ++i)
		{
			Base[i].Initialize();
		}
	}

	T_LCMT_D() {}
	T_LCMT_D(const T_LCMT_D& rData) { *this = rData; }
	T_LCMT_D& operator=(const T_LCMT_D& rData)
	{
		for(int i=0; i<3; ++i)
		{
			Base[i].aStrength.Copy(rData.Base[i].aStrength);
			Base[i].aService.Copy(rData.Base[i].aService);
			Base[i].aUltimate.Copy(rData.Base[i].aUltimate);
			Base[i].aFatigue.Copy(rData.Base[i].aFatigue);
			Base[i].aFatigue2.Copy(rData.Base[i].aFatigue2);
		}

		return *this;
	}
};

struct T_LCMT_D_RW_830
{
	int nCountStrength[2];
	int nCountService[2];
	int nCountUltimate[2];
	int nCountFatigue[2];
};

struct T_LCMT_D_RW
{
	int nCountStrength[3];
	int nCountService[3];
	int nCountUltimate[3];
	int nCountFatigue[3];
	int nCountFatigue2[3];

	void Get(T_LCMT_D& rData)
	{
		for(int i=0; i<3; ++i)
		{
			rData.Base[i].aStrength.SetSize(nCountStrength[i]);
			rData.Base[i].aService.SetSize(nCountService[i]);
			rData.Base[i].aUltimate.SetSize(nCountUltimate[i]);
			rData.Base[i].aFatigue.SetSize(nCountFatigue[i]);
			rData.Base[i].aFatigue2.SetSize(nCountFatigue2[i]);
		}
	}
	void Set(T_LCMT_D Data)
	{	
		for(int i=0; i<3; ++i)
		{
			nCountStrength[i] = (int)Data.Base[i].aStrength.GetSize();
			nCountService[i]  = (int)Data.Base[i].aService.GetSize();
			nCountUltimate[i] = (int)Data.Base[i].aUltimate.GetSize();
			nCountFatigue[i]  = (int)Data.Base[i].aFatigue.GetSize();
			nCountFatigue2[i] = (int)Data.Base[i].aFatigue2.GetSize();
		}
	}
	void Convert830(T_LCMT_D_RW_830& rData)
	{
		for(int i=0; i<2; ++i)
		{
			nCountStrength[i] = rData.nCountStrength[i];
			nCountService[i]  = rData.nCountService[i];
			nCountUltimate[i] = rData.nCountUltimate[i];
			nCountFatigue[i]  = rData.nCountFatigue[i];
			nCountFatigue2[i] = 0;
		}

		nCountStrength[2] = 0;
		nCountService[2]  = 0;
		nCountUltimate[2] = 0;
		nCountFatigue[2]  = 0;
		nCountFatigue2[2] = 0;
	}
};

struct T_LCMT_UDRD_D
{
	T_LCMT_D data;
};

struct T_LCMT_UDRD_D_RW_830
{
	T_LCMT_D_RW_830 data;
};

struct T_LCMT_UDRD_D_RW
{
	T_LCMT_D_RW data;

	void GetLcmt(T_LCMT_D &rData)
	{
		data.Get(rData);
	}
	void SetLcmt(const T_LCMT_D &rData)
	{
		data.Set(rData);
	}
	void Convert830(T_LCMT_UDRD_D_RW_830& rData)
	{
		data.Convert830(rData.data);
	}
};

struct T_LCMT_UNIT
{
	T_LCMT_UNIT()
	{ 
		// Nothing
	}
};
#define HASHSIZELCMT 1

#define HASHSIZELCOM 10
#define D_LCOM_MINNUM       1   // 입력가능한 최소 하중조합번호
#define D_LCOM_MAXNUM    9999   // 입력가능한 최대 하중조합번호

#define T_ENVL_K unsigned int
struct T_ENVL_BASE
{
	unsigned int AnalType;   //D_LCOM_???
	unsigned int LoadCaseKey;
};

// 더이상 사용하지 않는 기능으로 분석됨. by Hsshim. 2015-05-13
struct T_ENVL_D
{
	CString EnvelopeName;
	CString Description;
	unsigned int EnvelopeId;
	CArray<T_ENVL_BASE,T_ENVL_BASE&>arEnvelope;
	void Initialize()
	{
		EnvelopeName.Empty();
		Description.Empty();
		EnvelopeId=0;
		arEnvelope.RemoveAll();
	}
	T_ENVL_D()
	{
	}
	T_ENVL_D(T_ENVL_D& src)
	{
		*this = src;
	}
	T_ENVL_D& operator=(T_ENVL_D& src)
	{
		EnvelopeName=src.EnvelopeName;
		Description=src.Description;
		EnvelopeId=src.EnvelopeId;
		arEnvelope.Copy(src.arEnvelope);
		return *this;
	}
};
struct T_ENVL_D_CH
{
	char EnvelopeName[40];
	char Description[80];
	unsigned int EnvelopeId;
	CArray<T_ENVL_BASE,T_ENVL_BASE&>arEnvelope;
	void ConvertToString(T_ENVL_D& rData)
	{
		ConvertCharStr(EnvelopeName, rData.EnvelopeName, sizeof(EnvelopeName));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.EnvelopeId=EnvelopeId;
		rData.arEnvelope.Copy(arEnvelope);
	}
	void ConvertToChar(T_ENVL_D& rData)
	{
		ConvertStrChar(rData.EnvelopeName, EnvelopeName, sizeof(EnvelopeName));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		EnvelopeId=rData.EnvelopeId;
		arEnvelope.Copy(rData.arEnvelope);
	}
};
struct T_ENVL_UDRD_D
{
	T_ENVL_K key;
	T_ENVL_D data;
};
struct T_ENVL_UDRD_D_CH
{
	T_ENVL_K key;
	T_ENVL_D_CH data;
};
// 더이상 사용하지 않는 기능으로 분석됨. by Hsshim. 2015-05-13
struct T_ENVL_UDRD_D_CH_RW
{
	T_ENVL_K key;
	char EnvelopeName[40];
	char Description[80];
	unsigned int EnvelopeId;
	char dummyArray_32bit[20];   // 하위 버전 호환용 size dummy 크기. CArray<T_ENVL_BASE,T_ENVL_BASE&>arEnvelope;
	int nCountEnvelope;
	void GetEnvl(T_ENVL_K& rKey, T_ENVL_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.EnvelopeName, EnvelopeName, sizeof(EnvelopeName));
		memcpy(rData.Description, Description, sizeof(Description));
		rData.EnvelopeId=EnvelopeId;
		//rData.arEnvelope.Copy(arEnvelope);    
	}
	void SetEnvl(const T_ENVL_K Key, const T_ENVL_D_CH& rData)
	{
		key=Key;
		memcpy(EnvelopeName, rData.EnvelopeName, sizeof(EnvelopeName));
		memcpy(Description, rData.Description, sizeof(Description));
		EnvelopeId=rData.EnvelopeId;
		nCountEnvelope=(int)rData.arEnvelope.GetSize();
	}
};
struct T_ENVL_UNIT
{
	T_ENVL_UNIT()
	{
		EnvelopeName=D_UNITSYS_NONE;
		Description=D_UNITSYS_NONE;
		EnvelopeId=D_UNITSYS_NONE;
		AnalType=D_UNITSYS_NONE;
		LoadCaseKey=D_UNITSYS_NONE;
	}
	int EnvelopeName;
	int Description;
	int EnvelopeId;
	int AnalType;
	int LoadCaseKey;
};
#define HASHSIZEENVL 10
#define D_ENVL_MINNUM       1   // 입력가능한 최소 Envelope번호
#define D_ENVL_MAXNUM    9999   // 입력가능한 최대 Envelope번호

// Loadcases for Reciprocal Stress
struct T_LCRS_BASE
{
	unsigned int AnalType;   //D_LCOM_???
	unsigned int LoadCaseKey;
	void Initialize()
	{
		AnalType    = 0;
		LoadCaseKey = 0;
	}
};

#define T_LCRS_K unsigned int
struct T_LCRS_D
{
	CArray<T_LCRS_BASE, T_LCRS_BASE&> aCase;
	void Initialize()
	{
		aCase.RemoveAll();
	}
	T_LCRS_D()
	{
	}
	T_LCRS_D(T_LCRS_D& src)
	{
		*this = src;
	}
	T_LCRS_D& operator=(T_LCRS_D& src)
	{    
		aCase.Copy(src.aCase);
		return *this;
	}  
};
struct T_LCRS_UDRD_D
{  
	T_LCRS_D data;
};
struct T_LCRS_UDRD_D_RW
{  
	int nCountCase;  
	void GetLcrs(T_LCRS_D& rData)
	{
	}
	void SetLcrs(T_LCRS_D& rData)
	{
		nCountCase = (int)rData.aCase.GetSize();
	}  
};

struct T_LCRS_UNIT
{
	T_LCRS_UNIT()
	{
		nNoUnit=D_UNITSYS_NONE;    
	}
	int nNoUnit;  
};
#define HASHSIZELCRS 1

// Load Contribution for NL Load Case 
#define T_LCTB_K unsigned int
struct T_LCTB_BASE
{
	UINT LoadCaseKey;
	int  iAnalType;   // D_LCOM_STATIC
	double dFactor;
	void Initialize()
	{
		LoadCaseKey = 0;
		iAnalType = 0;
		dFactor = 0.;
	}
};
struct T_LCTB_D
{
	CString strLctbName;
	CString Description;
	T_STLD_K StldKey;
	unsigned int LoadContributionId;
	CArray<T_LCTB_BASE, T_LCTB_BASE&> aLctbBase;  
	void Initialize()
	{
		strLctbName.Empty();
		Description.Empty();
		StldKey = 0;
		LoadContributionId = 0;
		aLctbBase.RemoveAll();    
	}
	T_LCTB_D()  { }
	T_LCTB_D(T_LCTB_D& src) {*this = src;}
	T_LCTB_D& operator=(T_LCTB_D& src)
	{
		strLctbName = src.strLctbName;
		Description=src.Description;
		StldKey = src.StldKey;
		LoadContributionId = src.LoadContributionId;
		aLctbBase.Copy(src.aLctbBase);    
		return *this;
	}  
};
struct T_LCTB_D_CH
{
	char strLctbName[40];  
	char Description[80];
	T_STLD_K StldKey;
	unsigned int LoadContributionId;
	CArray<T_LCTB_BASE, T_LCTB_BASE&> aLctbBase;
	void ConvertToString(T_LCTB_D& rData)
	{
		ConvertCharStr(strLctbName, rData.strLctbName, sizeof(strLctbName));   
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.StldKey = StldKey;
		rData.LoadContributionId = LoadContributionId;
		rData.aLctbBase.Copy(aLctbBase);
	}
	void ConvertToChar(T_LCTB_D& rData)
	{
		ConvertStrChar(rData.strLctbName, strLctbName, sizeof(strLctbName)); 
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		StldKey = rData.StldKey;
		LoadContributionId=rData.LoadContributionId;
		aLctbBase.Copy(rData.aLctbBase);
	}
};
struct T_LCTB_UDRD_D
{
	T_LCTB_K key;
	T_LCTB_D data;
};
struct T_LCTB_UDRD_D_CH
{
	T_LCTB_K key;
	T_LCTB_D_CH data;
};
struct T_LCTB_UDRD_D_CH_RW
{
	T_LCTB_K key; 
	char strLctbName[40];
	char Description[80];
	T_STLD_K StldKey;
	unsigned int LoadContributionId;
	unsigned int nCountLctbBase;
	void GetLctb(T_LCTB_K& rKey, T_LCTB_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strLctbName, strLctbName, sizeof(strLctbName));
		memcpy(rData.Description, Description, sizeof(Description));
		rData.StldKey = StldKey;
		rData.LoadContributionId = LoadContributionId;
	}
	void SetLctb(const T_LCTB_K Key, const T_LCTB_D_CH& rData)
	{
		key=Key;    
		memcpy(strLctbName, rData.strLctbName, sizeof(strLctbName));
		memcpy(Description, rData.Description, sizeof(Description));
		StldKey = rData.StldKey;
		LoadContributionId = rData.LoadContributionId;
		nCountLctbBase=(int)rData.aLctbBase.GetSize();
	}
};
struct T_LCTB_UNIT
{
	T_LCTB_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZELCTB 10
#define D_LCTB_MINNUM       1   // 입력가능한 최소 Envelope번호
#define D_LCTB_MAXNUM    9999   // 입력가능한 최대 Envelope번호

// Analysis Option
#define T_ANOP_K unsigned int

struct T_ANOP_D
{
	double dStiffnessDiagonal;
	double dSpringConstant;

	T_ANOP_D() { Initialize(); }

	T_ANOP_D(T_ANOP_D& src)
	{
		*this = src;
	}

	void Initialize()
	{
		dStiffnessDiagonal = 1e-13;
		dSpringConstant = 1e-10;
	}

	T_ANOP_D& operator=(T_ANOP_D& src)
	{
		dStiffnessDiagonal = src.dStiffnessDiagonal;
		dSpringConstant = src.dSpringConstant;
		return *this;
	}
};

struct T_ANOP_UDRD_D
{
	T_ANOP_D data;
};

#define HASHSIZEANOP 1

#pragma pack(pop)

#endif  // __DB_ST_DT_CASE_H__
