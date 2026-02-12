#ifndef __DB_ST_DT_GMOD_H__
#define __DB_ST_DT_GMOD_H__

#define MIDAS_SDK

#pragma pack(push, 1)

// Main Girder
#define T_MGDR_K unsigned int
struct T_MGDR_BASE
{
	T_NODE_K KeyNode;
	void Initialize()
	{
		KeyNode=0;
	}
};
#define T_MGDR_DT CArray<T_MGDR_BASE, T_MGDR_BASE&>
struct T_MGDR_D
{
	CString Name;
	T_MGDR_DT aData;
	unsigned int nSeq;
	void Initialize()
	{
		Name.Empty();
		aData.RemoveAll();
		nSeq = 0;
	}
	T_MGDR_D()
	{
	}
	T_MGDR_D(T_MGDR_D& src)
	{
		*this = src;
	}
	T_MGDR_D& operator=(T_MGDR_D& src)
	{
		Name = src.Name;
		aData.Copy(src.aData);
		nSeq = src.nSeq;
		return *this;
	}
};
struct T_MGDR_D_CH
{
	char Name[40];
	T_MGDR_DT aData;
	unsigned int nSeq;
	void ConvertToString(T_MGDR_D& rData)
	{
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.aData.Copy(aData);
		rData.nSeq = nSeq;
	}
	void ConvertToChar(T_MGDR_D& rData)
	{
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		aData.Copy(rData.aData);
		nSeq = rData.nSeq;
	}
};
struct T_MGDR_UDRD_D
{
	T_MGDR_K key;
	T_MGDR_D data;
};
struct T_MGDR_UDRD_D_CH
{
	T_MGDR_K key;
	T_MGDR_D_CH data;
};
struct T_MGDR_UDRD_D_CH_RW
{
	T_MGDR_K key;
	char Name[40];
	unsigned int nSeq;
	unsigned int nCountData;
	void GetMgdr(T_MGDR_K& rKey, T_MGDR_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.Name, Name, sizeof(Name));
		rData.nSeq = nSeq;
	}
	void SetMgdr(const T_MGDR_K Key, const T_MGDR_D_CH& rData)
	{
		key = Key;
		memcpy(Name, rData.Name, sizeof(Name));
		nSeq = rData.nSeq;
		nCountData = (int)rData.aData.GetSize();
	}
};
struct T_MGDR_UNIT
{
	T_MGDR_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEMGDR 11
#define D_MGDR_MINNUM       1   // 입력가능한 최소 번호
#define D_MGDR_MAXNUM    9999   // 입력가능한 최대 번호

// Cross Beam
#define T_CBEM_K unsigned int
struct T_CBEM_BASE_940
{
	T_NODE_K KeyNode;
	void Initialize()
	{
		KeyNode = 0;
	}
};

struct T_CBEM_BASE
{
	T_NODE_K KeyNode;
	BOOL bSkip;
	void Initialize()
	{
		KeyNode=0;
		bSkip=FALSE;
	}
	void Convert940(T_CBEM_BASE_940& rData)
	{
		KeyNode = rData.KeyNode;
		bSkip = FALSE;
	}
};

#define T_CBEM_DT CArray<T_CBEM_BASE, T_CBEM_BASE&>
struct T_CBEM_D
{
	CString Name;
	T_CBEM_DT aData;
	unsigned int nSeq;
	void Initialize()
	{
		Name.Empty();
		aData.RemoveAll();
		nSeq = 0;
	}
	T_CBEM_D() {Initialize();}
	T_CBEM_D(T_CBEM_D& src) { *this = src; }
	T_CBEM_D& operator=(T_CBEM_D& src)
	{
		Name = src.Name;
		aData.Copy(src.aData);
		nSeq = src.nSeq;
		return *this;
	}
};

#define T_CBEM_DT_940 CArray<T_CBEM_BASE_940, T_CBEM_BASE_940&>
struct T_CBEM_D_940
{
	CString Name;
	T_CBEM_DT_940 aData;
	unsigned int nSeq;
	void Initialize()
	{
		Name.Empty();
		aData.RemoveAll();
		nSeq = 0;
	}
	T_CBEM_D_940() { Initialize(); }
	T_CBEM_D_940(T_CBEM_D_940& src) { *this = src; }
	T_CBEM_D_940& operator=(T_CBEM_D_940& src)
	{
		Name = src.Name;
		aData.Copy(src.aData);
		nSeq = src.nSeq;
		return *this;
	}

	void ConvertFromCurrent(T_CBEM_D& rData)
	{
		Name = rData.Name;
		nSeq = rData.nSeq;

		int nSize = rData.aData.GetSize();
		aData.SetSize(nSize);
		for (int i = 0; i < nSize; i++)
		{
			aData[i].KeyNode = rData.aData[i].KeyNode;
		}
	}
};

struct T_CBEM_D_CH
{
	char Name[40];
	T_CBEM_DT aData;
	unsigned int nSeq;
	void ConvertToString(T_CBEM_D& rData)
	{
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.aData.Copy(aData);
		rData.nSeq = nSeq;
	}
	void ConvertToChar(T_CBEM_D& rData)
	{
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		aData.Copy(rData.aData);
		nSeq = rData.nSeq;
	}
};
struct T_CBEM_UDRD_D
{
	T_CBEM_K key;
	T_CBEM_D data;
};
struct T_CBEM_UDRD_D_CH
{
	T_CBEM_K key;
	T_CBEM_D_CH data;
};
struct T_CBEM_UDRD_D_CH_RW
{
	T_CBEM_K key;
	char Name[40];
	unsigned int nSeq;
	unsigned int nCountData;
	void GetCbem(T_CBEM_K& rKey, T_CBEM_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.Name, Name, sizeof(Name));
		rData.nSeq = nSeq;
	}
	void SetCbem(const T_CBEM_K Key, const T_CBEM_D_CH& rData)
	{
		key = Key;
		memcpy(Name, rData.Name, sizeof(Name));
		nSeq = rData.nSeq;
		nCountData = (int)rData.aData.GetSize();
	}
};
struct T_CBEM_UNIT
{
	T_CBEM_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZECBEM 11
#define D_CBEM_MINNUM       1   // 입력가능한 최소 번호
#define D_CBEM_MAXNUM    9999   // 입력가능한 최대 번호

// Load Line
#define T_LDLN_K unsigned int
struct T_LDLN_BASE
{
	double x, y, z;
	void Initialize()
	{
		x = y = z = 0.0;
	}
};
#define T_LDLN_DT CArray<T_LDLN_BASE, T_LDLN_BASE&>
struct T_LDLN_D
{
	CString Name;
	T_LDLN_DT aData;
	unsigned int nSeq;
	void Initialize()
	{
		Name.Empty();
		aData.RemoveAll();
		nSeq = 0;
	}
	T_LDLN_D()
	{
	}
	T_LDLN_D(T_LDLN_D& src)
	{
		*this = src;
	}
	T_LDLN_D& operator=(T_LDLN_D& src)
	{
		Name = src.Name;
		aData.Copy(src.aData);
		nSeq = src.nSeq;
		return *this;
	}
};
struct T_LDLN_D_CH
{
	char Name[40];
	T_LDLN_DT aData;
	unsigned int nSeq;
	void ConvertToString(T_LDLN_D& rData)
	{
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.aData.Copy(aData);
		rData.nSeq = nSeq;
	}
	void ConvertToChar(T_LDLN_D& rData)
	{
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		aData.Copy(rData.aData);
		nSeq = rData.nSeq;
	}
};
struct T_LDLN_UDRD_D
{
	T_LDLN_K key;
	T_LDLN_D data;
};
struct T_LDLN_UDRD_D_CH
{
	T_LDLN_K key;
	T_LDLN_D_CH data;
};
struct T_LDLN_UDRD_D_CH_RW
{
	T_LDLN_K key;
	char Name[40];
	unsigned int nSeq;
	unsigned int nCountData;
	void GetLdln(T_LDLN_K& rKey, T_LDLN_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.Name, Name, sizeof(Name));
		rData.nSeq = nSeq;
	}
	void SetLdln(const T_LDLN_K Key, const T_LDLN_D_CH& rData)
	{
		key = Key;
		memcpy(Name, rData.Name, sizeof(Name));
		nSeq = rData.nSeq;
		nCountData = (int)rData.aData.GetSize();
	}
};
struct T_LDLN_UNIT
{
	T_LDLN_UNIT()
	{
		xyz = D_UNITSYS_BASE_LENGTH;
	}
	int xyz;
};
#define HASHSIZELDLN 11
#define D_LDLN_MINNUM       1   // 입력가능한 최소 번호
#define D_LDLN_MAXNUM    9999   // 입력가능한 최대 번호

// Grid Influence Line Load Case
#define D_GILC_TYPE_DEAD     0
#define D_GILC_TYPE_DEADOTHR 1
#define D_GILC_TYPE_LIVEAUTO 2
#define D_GILC_TYPE_CROWD    3
#define D_GILC_TYPE_LIVEOTHR 4
#define D_GILC_TYPE_TLOAD    5

#define T_GILC_K unsigned int
struct T_GILC_D
{
	CString LoadCaseName;
	int     nLoadCaseType;    // 0:Dead, 1:Other Dead Load, 
														// 2:Vehicle Live Load, 3:Crowded Line Load, 4:Other Live Load (2,3,4는 일본 버전에만)
	CString Description;
	unsigned int LoadCaseId;    // 순번
	void Initialize()
	{
		LoadCaseName.Empty();
		nLoadCaseType = 0;
		Description.Empty();
		LoadCaseId=0;
	}
};
struct T_GILC_D_CH
{
	char LoadCaseName[20];
	int  nLoadCaseType;
	char Description[80];
	unsigned int LoadCaseId;
	void ConvertToString(T_GILC_D& rData)
	{
		ConvertCharStr(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		rData.nLoadCaseType = nLoadCaseType;
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.LoadCaseId = LoadCaseId;
	}
	void ConvertToChar(T_GILC_D& rData)
	{
		ConvertStrChar(rData.LoadCaseName, LoadCaseName, sizeof(LoadCaseName));
		nLoadCaseType = rData.nLoadCaseType;
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		LoadCaseId = rData.LoadCaseId;
	}
};
struct T_GILC_UDRD_D
{
	T_GILC_K key;
	T_GILC_D data;
};
struct T_GILC_UDRD_D_CH
{
	T_GILC_K key;
	T_GILC_D_CH data;
};
struct T_GILC_UNIT
{
	T_GILC_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEGILC 11
#define D_GILC_MINNUM       1   // 입력가능한 최소 정적하중조건번호
#define D_GILC_MAXNUM    9999   // 입력가능한 최대 정적하중조건번호

// Grid Influence Line Load
#define T_GILD_K T_GILC_K
struct T_GILD_BASE
{
	int nLoadType;  // 0:Point, 1:Line, 2:Area  : Dead Load, Other Dead Load에서만 사용
	T_LDLN_K CenterLineK;
	BOOL bStartGirder;
	BOOL bEndGirder;
	UINT StartGirderLineK; // bStartGirder==TRUE면 MgdrK, FALSE이면 LdlnK;
	UINT EndGirderLineK;   // bEndGirder==TRUE면 MgdrK, FALSE이면 LdlnK
	T_CBEM_K StartCrossK;
	T_CBEM_K EndCrossK;
	T_LDLN_K StartMedialK;    // Vehicle Live Load만 사용 (0 허용)
	T_LDLN_K EndMedialK;      // Vehicle Live Load만 사용 (0 허용)
	double dStartLoad;        // Dead Load, Other Dead Load만 사용
	double dEndLoad;          // Dead Load, Other Dead Load만 사용
	double dMainLoadingRange; // Vehicle Live Load만 사용
	double dLoad;             // Other Live Load만 사용
	void Initialize()
	{
		nLoadType = 0;
		CenterLineK = 0;
		bStartGirder = TRUE;
		bEndGirder = TRUE;
		StartGirderLineK = 0;
		EndGirderLineK = 0;
		StartCrossK = 0;
		EndCrossK = 0;
		StartMedialK = 0;
		EndMedialK = 0;
		dStartLoad = 0.0;
		dEndLoad = 0.0;
		dMainLoadingRange = 0.0;
		dLoad = 0.0;
	}
};

struct T_GILD_D
{
	int nType;  // 0:Dead Load, 1:Other Dead Load, 2:Vehicle Live Load, 3:Crowded Live Load, 4:Other Live Load
							// 2, 3, 4는 일본 버전에만
	int nVehicleLoadType; //             0:L-load type-A, 1:L-load type-B, 2:T-load,   3:Honsu-Shikoku Bridge Authority
												// 변경(881)-> 0:L-load type-A, 1:L-load type-B, 2:T-load-A, 4:T-load-B
	int nCrowdedLoadType; //             0:Japan Highway public corporation, 1:Honsu-Shikoku Bridge Authority
												// 변경(881)-> 0:Japan Highway public corporation
	CArray<T_GILD_BASE, T_GILD_BASE&> aData;
	void Initialize()
	{
		nType = 0;
		nVehicleLoadType = 0;
		nCrowdedLoadType = 0;
		aData.RemoveAll();
	}
	T_GILD_D() {}
	T_GILD_D(T_GILD_D& src) { *this = src; }
	T_GILD_D& operator=(T_GILD_D& src)
	{
		nType=src.nType;
		nVehicleLoadType = src.nVehicleLoadType;
		nCrowdedLoadType = src.nCrowdedLoadType;
		aData.Copy(src.aData);
		return *this;
	}
};

struct T_GILD_UDRD_D
{
	T_GILD_K key;
	T_GILD_D data;
};

struct T_GILD_UDRD_D_RW
{
	T_GILD_K key;
	int nType;
	int nVehicleLoadType;
	int nCrowdedLoadType;
	unsigned int nCountData;
	void GetGild(T_GILD_K& rKey, T_GILD_D& rData)
	{
		rKey = key;
		rData.nType = nType;
		rData.nVehicleLoadType = nVehicleLoadType;
		rData.nCrowdedLoadType = nCrowdedLoadType;
	}
	void SetGild(const T_GILD_K Key, const T_GILD_D& rData)
	{
		key = Key;
		nType = rData.nType;
		nVehicleLoadType = rData.nVehicleLoadType;
		nCrowdedLoadType = rData.nCrowdedLoadType;
		nCountData = (int)rData.aData.GetSize();
	}
};

struct T_GILD_UNIT
{
	T_GILD_UNIT()
	{
		dPointLoad = D_UNITSYS_BASE_FORCE;
		dLineLoad = D_UNITSYS_BASE_UNITFORCE;
		dAreaLoad = D_UNITSYS_BASE_STRESS;
		dMainLoadingRange = D_UNITSYS_BASE_LENGTH;
	}
	int dPointLoad;
	int dLineLoad;
	int dAreaLoad;
	int dMainLoadingRange;
};
#define HASHSIZEGILD HASHSIZEGILC

/// analysis control ///
#define T_BRGT_K unsigned int
struct T_BRGT_D
{
	int nKindOfBridge;  // 0:Steel, 1:PC, 2:RC
	int nTypeOfBridge;  // 0:Simple, Continuous Girder, 1:Gerber, 2:Rahmen
	int nDirectLoad;    // 0:Direct Load, 1:Indirect Load
	void Initialize()
	{
		nKindOfBridge = 0;
		nTypeOfBridge = 0;
		nDirectLoad = 0;
	}
};
struct T_BRGT_UDRD_D
{
	T_BRGT_D data;
};
struct T_BRGT_UNIT
{
	T_BRGT_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEBRGT 1


//Japan Grid Analysis Control Data ///
#define T_GACD_K unsigned int
struct T_GACD_D
{
	int nSubDivide;  
	double dDistance;

	BOOL bReaction;
	int nReacFilter;  // 0=All 1=Group;
	T_GRUP_K ReacGrup;
	BOOL bDisplacement;
	int nDispFilter;  // 0=All 1=Group;
	T_GRUP_K DispGrup;
	BOOL bForce;
	int nForcFilter;  // 0=All 1=Group;
	T_GRUP_K ForcGrup;
	BOOL bLink;
	int nLinkFilter;  // 0=All 1=Group;
	T_BNGR_K LinkGrup;

	void Initialize()
	{
		nSubDivide = 1;
		dDistance  = 1.0;
		bReaction = TRUE;
		nReacFilter = 0;  // 0=All 1=Group;
		ReacGrup = 0;
		bDisplacement = TRUE;
		nDispFilter = 0;  // 0=All 1=Group;
		DispGrup = 0;
		bForce = TRUE;
		nForcFilter = 0;  // 0=All 1=Group;
		ForcGrup = 0;
		bLink = TRUE;
		nLinkFilter = 0;  // 0=All 1=Group;
		LinkGrup = 0;
	}
};
struct T_GACD_UDRD_D
{
	T_GACD_D data;
};
struct T_GACD_UNIT
{
	T_GACD_UNIT()
	{
		dDistance=D_UNITSYS_BASE_LENGTH;
	}
	int dDistance;
};
#define HASHSIZEGACD 1

// Span Length
#define T_BSPN_K unsigned int
struct T_BSPN_D
{
	T_CBEM_K StartCrossK; // Cross Beam Key
	T_CBEM_K EndCrossK; 
	double dSpanLength;
	double dLoadingLength;
	int nSpanType;          // 0:Girder, 1:Rahmen, 2:Cantilever, 3:Hanger Beam
	int nConnectionType;    // 0:Both Fix, 1:Left Pin, 2:Right Pin, 3:Both Pin
	unsigned int nSeq;    // 순번
	void Initialize()
	{
		StartCrossK = 0;
		EndCrossK = 0;
		dSpanLength = 0.0;
		dLoadingLength = 0.0;
		nSpanType = 0;
		nConnectionType = 0;
		nSeq=0;
	}
};

struct T_BSPN_UDRD_D
{
	T_BSPN_K key;
	T_BSPN_D data;
};
struct T_BSPN_UNIT
{
	T_BSPN_UNIT()
	{
		dSpanLength = D_UNITSYS_BASE_LENGTH;
		dLoadingLength = D_UNITSYS_BASE_LENGTH;
	}
	int dSpanLength;
	int dLoadingLength;
};
#define HASHSIZEBSPN 31

// Impact Factor in Grid Model
#define T_GIMP_K unsigned int
struct T_GIMP_D
{
	int nTgtType;         // 0=Element, 1=Support
	UINT nTgtKey;         // Element Key : nTgtType = 0 인 경우
												// Node Key    : nTgtType = 1 인 경우
	int nFactorType;      // 0:Use Span Length of Span Data, 1: Use Span Length 75 % of Span Data
												// 2:Define Span Length Directory, 3:Define Impact Factor Directly
	double dSpanLength;
	double dImpactFactor;  
	void Initialize()
	{
		nTgtType = 0;
		nTgtKey = 0;
		nFactorType = 0;
		dSpanLength = 0.0;
		dImpactFactor = 0.0;    
	}  
};
struct T_GIMP_UDRD_D
{
	T_GIMP_K key;
	T_GIMP_D data;
};
struct T_GIMP_UNIT
{
	T_GIMP_UNIT()
	{
		dSpanLength = D_UNITSYS_BASE_LENGTH;
	}
	int dSpanLength;
};
#define HASHSIZEGIMP 1001

// Structure Type in Grid Model
#define T_GSTY_K unsigned int
struct T_GSTY_D
{
	int  nTgtType;         // 0=Element
	UINT nTgtKey;          // nTgtType = 0 : Element Key                        
	int  nElemType;        // 0=Girder, 1=Rahem, 2=Cantilever, 3=Hanger Beam
	void Initialize()
	{
		nTgtType = 0;
		nTgtKey = 0;
		nElemType = 0;
	}  
};
struct T_GSTY_UDRD_D
{
	T_GSTY_K key;
	T_GSTY_D data;
};
struct T_GSTY_UNIT
{
	T_GSTY_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEGSTY 1001

// Cross Reference for Grid Model
#define T_CREF_K unsigned int
struct T_CREF_BASE
{
	UINT ElemK;
	int  nCalcPosition;   // 0=I, 1=J, 2=Both
	void Initialize()
	{
		ElemK = 0;
		nCalcPosition = 2;
	}
};
struct T_CREF_D
{
	CString Name;
	int nCalcItem;  // 0:Reaction, 1:Diplacement, 2:Member Force 3:Link Force
	int nCalcType;  // 0:Matrix, 1:Max
	UINT MasterK;   // NodeK (nCalcItem==0 or nCalcItem==1)
									// ElemK (nCalcItem==2)
	CArray<UINT, UINT> aList;                    // NodeK : nCalcItem==0 or nCalcItem==1 인 경우 사용
	CArray<T_CREF_BASE, T_CREF_BASE&> aElemBase; // 6.5.0 추가 :nCalcItem==2, 3 인 경우 사용  
	BOOL bIncImpFactor;  
	int nCalcPositionMaster;  // 6.5.0 추가 : 0=I, 1=J, 2=Both
	unsigned int nSeq;
	void Initialize()
	{
		Name.Empty();
		nCalcItem = 0;
		nCalcType = 0;
		MasterK = 0;
		aList.RemoveAll();
		aElemBase.RemoveAll();
		bIncImpFactor = TRUE;    
		nCalcPositionMaster = 2;
		nSeq = 0;
	}
	T_CREF_D()
	{
	}
	T_CREF_D(T_CREF_D& src)
	{
		*this = src;
	}
	T_CREF_D& operator=(T_CREF_D& src)
	{
		Name = src.Name;
		nCalcItem = src.nCalcItem;
		nCalcType = src.nCalcType;
		MasterK = src.MasterK;
		aList.Copy(src.aList);
		aElemBase.Copy(src.aElemBase);
		bIncImpFactor = src.bIncImpFactor;    
		nCalcPositionMaster = src.nCalcPositionMaster;
		nSeq = src.nSeq;
		return *this;
	}
};
struct T_CREF_D_CH
{
	char Name[40];
	int nCalcItem;
	int nCalcType;
	UINT MasterK;
	CArray<UINT, UINT> aList;
	CArray<T_CREF_BASE, T_CREF_BASE&> aElemBase; // 6.5.0 추가 :nCalcItem==2 인 경우 사용  
	BOOL bIncImpFactor;  
	int nCalcPositionMaster;  // 6.5.0 추가 : 0=I, 1=J, 2=Both
	unsigned int nSeq;
	void ConvertToString(T_CREF_D& rData)
	{
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.nCalcItem     = nCalcItem;
		rData.nCalcType     = nCalcType;
		rData.MasterK       = MasterK;
		rData.aList.Copy(aList);
		rData.aElemBase.Copy(aElemBase);
		rData.bIncImpFactor = bIncImpFactor;    
		rData.nCalcPositionMaster = nCalcPositionMaster;
		rData.nSeq          = nSeq;
	}
	void ConvertToChar(T_CREF_D& rData)
	{
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		nCalcItem     = rData.nCalcItem;
		nCalcType     = rData.nCalcType;
		MasterK       = rData.MasterK;
		aList.Copy(rData.aList);
		aElemBase.Copy(rData.aElemBase);
		bIncImpFactor = rData.bIncImpFactor;    
		nCalcPositionMaster = rData.nCalcPositionMaster;
		nSeq          = rData.nSeq;
	}
};
struct T_CREF_UDRD_D
{
	T_CREF_K key;
	T_CREF_D data;
};
struct T_CREF_UDRD_D_CH
{
	T_CREF_K key;
	T_CREF_D_CH data;
};
struct T_CREF_UDRD_D_CH_RW
{
	T_CREF_K key;
	char Name[40];
	int nCalcItem;
	int nCalcType;
	UINT MasterK;
	BOOL bIncImpFactor;  
	int nCalcPositionMaster;  // 6.5.0 추가 : 0=I, 1=J, 2=Both
	unsigned int nSeq;
	unsigned int nCountList;
	unsigned int nCountElemBase;
	void GetCref(T_CREF_K& rKey, T_CREF_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.Name, Name, sizeof(Name));
		rData.nCalcItem     = nCalcItem;
		rData.nCalcType     = nCalcType;
		rData.MasterK       = MasterK;
		rData.bIncImpFactor = bIncImpFactor;    
		rData.nCalcPositionMaster = nCalcPositionMaster;
		rData.nSeq          = nSeq;
	}
	void SetCref(const T_CREF_K Key, const T_CREF_D_CH& rData)
	{
		key = Key;
		memcpy(Name, rData.Name, sizeof(Name));
		nCalcItem     = rData.nCalcItem;
		nCalcType     = rData.nCalcType;
		MasterK       = rData.MasterK;
		bIncImpFactor = rData.bIncImpFactor;    
		nCalcPositionMaster = rData.nCalcPositionMaster;
		nSeq          = rData.nSeq;
		nCountList = (int)rData.aList.GetSize();
		nCountElemBase = (int)rData.aElemBase.GetSize();
	}
};
struct T_CREF_UNIT
{
	T_CREF_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZECREF 11
#define D_CREF_MINNUM       1   // 입력가능한 최소 번호
#define D_CREF_MAXNUM    9999   // 입력가능한 최대 번호

// P1 Load Factor Type
#define T_P1LT_K unsigned int
struct T_P1LT_D
{
	CString Name;
	int nLoadItem;          // 0:Reaction, 1:Displacement, 2:Member Force
	int nLoadType[6];        // 0:Bending Memoment, 1:Shear Force
	unsigned int nSeq;  // 순번
	void Initialize()
	{
		Name.Empty();
		nLoadItem = 0;
		for (int i = 0; i < 6; i++) nLoadType[i] = 1;
		nSeq=0;
	}
};
struct T_P1LT_D_CH
{
	char Name[20];
	int nLoadItem;
	int nLoadType[6];
	unsigned int nSeq;
	void ConvertToString(T_P1LT_D& rData)
	{
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.nLoadItem      = nLoadItem;
		for (int i = 0; i < 6; i++)
			rData.nLoadType[i] = nLoadType[i];
		rData.nSeq           = nSeq;
	}
	void ConvertToChar(T_P1LT_D& rData)
	{
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		nLoadItem      = rData.nLoadItem;
		for (int i = 0; i < 6; i++)
			nLoadType[i] = rData.nLoadType[i];
		nSeq           = rData.nSeq;
	}
};
struct T_P1LT_UDRD_D
{
	T_P1LT_K key;
	T_P1LT_D data;
};
struct T_P1LT_UDRD_D_CH
{
	T_P1LT_K key;
	T_P1LT_D_CH data;
};
struct T_P1LT_UNIT
{
	T_P1LT_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEP1LT 101

// P1 Load Factor
//#define T_P1LA_K T_NODE_K
#define T_P1LA_K unsigned int
struct T_P1LA_D
{
	T_P1LT_K TypeKey;
	UINT NodeElemK;  // P1LT의 nLoadItem에 따라 Node/Elem 결정
	int  nLoadItem;   // P1LT의 nLoadItem 사본 저장(내부 사용용)
	void Initialize()
	{
		TypeKey = 0;
		NodeElemK = 0;
		nLoadItem = 0;
	}
};
struct T_P1LA_UDRD_D
{
	T_P1LA_K key;
	T_P1LA_D data;
};
struct T_P1LA_UNIT
{
	T_P1LA_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEP1LA 997

// Impact/P1 Load Factor for Link Data(Elastic, Nonlinear)
#define T_LNKF_K unsigned int
struct T_LNKF_D_640
{
	int  nLinkType;        // 0:Elastic Link, 1:General Link
	UINT LinkKey;
	int  nFactorKind;      // 0:Impact Factor, 1:P1 Load Factor
	int  nFactorType;      // nFactorKind == 0인 경우 사용  
												 // 0:Use Span Length of Span Data, 
												 // 1: Use Span Length 75 % of Span Data
												 // 2:Define Span Length Directory, 
												 // 3:Define Impact Factor Directly
	double dSpanLength;    // nFactorKind == 0인 경우 사용  
	double dImpactFactor;  // nFactorKind == 0인 경우 사용  
	T_P1LT_K TypeKey;      // nFactorKind == 1인 경우 사용
};
struct T_LNKF_D
{
	int  nLinkType;        // 0:Elastic Link, 1:General Link
	UINT LinkKey;
	int  nFactorKind;      // 0:Impact Factor, 1:P1 Load Factor, 2:Element Structure Type(6.5.0에서 추가)
	int  nFactorType;      // nFactorKind == 0인 경우 사용  
												 // 0:Use Span Length of Span Data, 
												 // 1: Use Span Length 75 % of Span Data
												 // 2:Define Span Length Directory, 
												 // 3:Define Impact Factor Directly
	double dSpanLength;    // nFactorKind == 0인 경우 사용  
	double dImpactFactor;  // nFactorKind == 0인 경우 사용  
	T_P1LT_K TypeKey;      // nFactorKind == 1인 경우 사용
	int    nElemType;      // nFactorKind == 2인 경우 사용  // 6.5.0에서 추가 
												 // 0=Girder, 1=Rahem, 2=Cantilever, 3=Hanger Beam  
	void Initialize()
	{
		nLinkType = 0;
		LinkKey = 0;
		nFactorKind = 0;
		nFactorType = 0;
		dSpanLength = 0.0;
		dImpactFactor = 0.0;
		TypeKey = 0;
		nElemType = 0;
	}
	void Convert640(T_LNKF_D_640& rData)
	{
		nLinkType     = rData.nLinkType    ;
		LinkKey       = rData.LinkKey      ;
		nFactorKind   = rData.nFactorKind  ;
		nFactorType   = rData.nFactorType  ;
		dSpanLength   = rData.dSpanLength  ;
		dImpactFactor = rData.dImpactFactor;
		TypeKey       = rData.TypeKey      ;
		nElemType     = 0                  ;
	}
};
struct T_LNKF_UDRD_D_640
{
	T_LNKF_K key;
	T_LNKF_D_640 data;
};
struct T_LNKF_UDRD_D
{
	T_LNKF_K key;
	T_LNKF_D data;
};
struct T_LNKF_UNIT
{
	T_LNKF_UNIT()
	{
		dSpanLength=D_UNITSYS_BASE_LENGTH;
	}
	int dSpanLength;
};
#define HASHSIZELNKF 101

// Grid Mass Converter (Load to Mass for Grid Model)
#define D_GLTM_NUMGLTM  15
#define T_GLTM_K unsigned int
struct T_GLTM_BASE
{
	T_GILC_K LoadCaseKey;
	double dblFactor;
	void Initialize()
	{
		LoadCaseKey=0;
		dblFactor=0;
	}
};
struct T_GLTM_D
{  
	BOOL bMassDirX;
	BOOL bMassDirY;
	BOOL bMassDirZ;  
	BOOL bFishBoneModel;
	T_MGDR_K MgdrKey;
	double dGravity;
	int nNumGltmCase;
	T_GLTM_BASE GltmCase[D_GLTM_NUMGLTM];
	void Initialize()
	{    
		bMassDirX = TRUE;
		bMassDirY = TRUE;
		bMassDirZ = FALSE; 
		bFishBoneModel = FALSE;
		MgdrKey = 0;
		dGravity = 0.;
		nNumGltmCase = 0;
		for(int i=0; i<D_GLTM_NUMGLTM; i++) GltmCase[i].Initialize();
	}
};
struct T_GLTM_UDRD_D
{
	T_GLTM_D data;
};
struct T_GLTM_UNIT
{
	T_GLTM_UNIT()
	{
		dGravity=D_UNITSYS_BASE_GRAVITY;    
	}
	int dGravity;  
};
#define HASHSIZEGLTM 1

// Nodal Mass
#define T_GMAS_K T_NODE_K
struct T_GMAS_D
{
	double Mass[3];
	void Initialize()
	{    
		memset(Mass,0,sizeof(Mass));    
	}  
};
struct T_GMAS_UDRD_D
{
	T_GMAS_K key;
	T_GMAS_D data;
};
struct T_GMAS_UNIT
{
	T_GMAS_UNIT()
	{
		nCount=3;
		nUnit[0]=D_UNITSYS_BASE_MASS;
		nUnit[1]=D_UNITSYS_BASE_MASS;
		nUnit[2]=D_UNITSYS_BASE_MASS;
		for(int j=0; j<nCount; j++)Mass[j]=nUnit[j];
	}
	int Mass[3];
	int nCount;  
	int nUnit[3];
};
#define HASHSIZEGMAS HASHSIZENODE

struct T_GRIDINFO_MASSLOAD
{
	T_NODE_K NodeK;
	T_GILC_K GilcK;
	double dNodalLoad;
	void Initialize()
	{
		NodeK=0;
		GilcK=0;
		dNodalLoad=0.;
	}
};

// Span Information
#define T_SPAN_K unsigned int
typedef std::pair<unsigned int, UINT> SpanPairK;
typedef CArray<SpanPairK, SpanPairK> ArrSpanPairKey;
struct T_SPAN_BASE
{
	UINT ElemK;
	BOOL nSupport; // 0:No, 1:I, 2:J
	void Initialize()
	{
		ElemK = 0;
		nSupport = 0;
	}
};

struct T_SPAN_D
{
	CString Name; // Girder Name
	BOOL    bExactSpan;
	CArray<double, double> aSpan;
	CArray<T_SPAN_BASE, T_SPAN_BASE&> aElemBase;
	int     nDirection; // 0:(-)Local-y, 1:(+)Local-y, 2:both
	unsigned int nSeq;
	int nSectType; // 0: PSC Bridge, 1: Composite Bridge, 2:Steel Girder

	void Initialize()
	{
		Name.Empty();
		bExactSpan = FALSE;
		aSpan.RemoveAll();
		aElemBase.RemoveAll();
		nDirection = 0;
		nSeq = 0;
		nSectType = 0;
	}
	T_SPAN_D()
	{
		Initialize();
	}
	T_SPAN_D(const T_SPAN_D& src)
	{
		*this = src;
	}
	T_SPAN_D& operator=(const T_SPAN_D& src)
	{
		Name = src.Name;
		bExactSpan = src.bExactSpan;
		aSpan.Copy(src.aSpan);
		aElemBase.Copy(src.aElemBase);
		nSeq = src.nSeq;
		nDirection = src.nDirection;
		nSectType = src.nSectType;
		return *this;
	}
};

struct T_SPAN_D_CH
{
	char Name[40];
	BOOL bExactSpan;
	CArray<double, double> aSpan;
	CArray<T_SPAN_BASE, T_SPAN_BASE&> aElemBase;
	int  nDirection;
	unsigned int nSeq;
	int nSectType; // 0: PSC Bridge, 1: Composite Bridge, 2:Steel Girder
	void ConvertToString(T_SPAN_D& rData)
	{
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		rData.bExactSpan = bExactSpan;
		rData.aSpan.Copy(aSpan);
		rData.aElemBase.Copy(aElemBase);
		rData.nDirection = nDirection;
		rData.nSeq = nSeq;
		rData.nSectType = nSectType;
	}
	void ConvertToChar(T_SPAN_D& rData)
	{
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		bExactSpan = rData.bExactSpan;
		aSpan.Copy(rData.aSpan);
		aElemBase.Copy(rData.aElemBase);
		nDirection = rData.nDirection;
		nSeq = rData.nSeq;
		nSectType = rData.nSectType;
	}
};
struct T_SPAN_UDRD_D
{
	T_SPAN_K key;
	T_SPAN_D data;
};
struct T_SPAN_UDRD_D_CH
{
	T_SPAN_K key;
	T_SPAN_D_CH data;
};

struct T_SPAN_UDRD_D_CH_RW_825
{
	T_SPAN_K key;
	char Name[40];
	BOOL bExactSpan;
	unsigned int nCountSpan;
	unsigned int nCountElemBase;
	int  nDirection;
	unsigned int nSeq;
};

struct T_SPAN_UDRD_D_CH_RW
{
	T_SPAN_K key;
	char Name[40];
	BOOL bExactSpan;
	unsigned int nCountSpan;
	unsigned int nCountElemBase;
	int  nDirection;
	unsigned int nSeq;
	int nSectType; // 0: PSC Bridge, 1: Composite Bridge, 2:Steel Girder
	void GetSpan(T_SPAN_K& rKey, T_SPAN_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.Name, Name, sizeof(Name));
		rData.bExactSpan = bExactSpan;
		rData.nDirection = nDirection;
		rData.nSeq       = nSeq;
		rData.nSectType  = nSectType;
	}
	void SetSpan(const T_SPAN_K Key, const T_SPAN_D_CH& rData)
	{
		key = Key;
		memcpy(Name, rData.Name, sizeof(Name));
		bExactSpan = rData.bExactSpan;
		nCountSpan = (int)rData.aSpan.GetSize();
		nCountElemBase = (int)rData.aElemBase.GetSize();
		nDirection = rData.nDirection;
		nSeq       = rData.nSeq;
		nSectType  = rData.nSectType;
	}
	void Convert825(T_SPAN_UDRD_D_CH_RW_825& src)
	{
		key = src.key;
		memcpy(Name, src.Name, sizeof(Name));
		bExactSpan = src.bExactSpan;
		nCountSpan = src.nCountSpan;
		nCountElemBase = src.nCountElemBase;
		nDirection = src.nDirection;
		nSeq       = src.nSeq;
		nSectType = 0;
	}
};
struct T_SPAN_UNIT
{
	T_SPAN_UNIT()
	{
		dSpan = D_UNITSYS_BASE_LENGTH;
	}
	int dSpan;
};
#define HASHSIZESPAN 3


// Span Information - virtual beam
#define T_SPAV_K T_SPAN_K
typedef T_SPAN_D  T_SPAV_D;

struct T_SPAV_UDRD_D_CH
{
	T_SPAN_K key;
	T_SPAN_D_CH data;
};

struct T_SPAV_UDRD_D
{
	T_SPAV_K key;
	T_SPAV_D data;
};

struct T_SPAV_UNIT
{
	T_SPAV_UNIT()
	{
		dSpan = D_UNITSYS_BASE_LENGTH;
	}
	int dSpan;
};
#define HASHSIZESPAV 3

#pragma pack(pop)

#endif  // __DB_ST_DT_GMOD_H__
