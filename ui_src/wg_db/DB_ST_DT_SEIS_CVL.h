#ifndef __DB_ST_DT_SEIS_CVL_H__
#define __DB_ST_DT_SEIS_CVL_H__

#define MIDAS_SDK

#pragma pack(push, 1)

// Seismic Evaluation Code
#define T_BSED_K unsigned int
struct T_BSED_D
{
	UINT nDesignCode;

	void Initialize()
	{
		nDesignCode = 0;
	}
};
struct T_BSED_UDRD_D
{
	T_BSED_K key;
	T_BSED_D data;
};
struct T_BSED_UNIT
{
	T_BSED_UNIT()
	{
		nNoUnit=D_UNITSYS_NONE;
	}
	int nNoUnit;
};
#define HASHSIZEBSED 1

// Evaluation Group
#define T_EVGP_K UINT
struct T_EVGP_COLM_D
{
	CString strName;
	CArray<T_ELEM_K, T_ELEM_K> aColumn;

	T_EVGP_COLM_D()  { }
	T_EVGP_COLM_D(const T_EVGP_COLM_D& src) {*this = src;}
	T_EVGP_COLM_D& operator=(const T_EVGP_COLM_D& src)
	{
		strName = src.strName;
		aColumn.Copy(src.aColumn);
		return *this;
	}  

	void Initialize()
	{
		strName.Empty();
		aColumn.RemoveAll();
	}	
};

struct T_EVGP_D
{
	CString strName;
	int  nType; // 0:Pier, 1:Abutment
	int  nAbutPos; // 0:Start, 1:End
	BOOL bPierCap;
	CArray<T_ELEM_K, T_ELEM_K> aPierCap;
	CArray<T_EVGP_COLM_D, T_EVGP_COLM_D&> aEvgpColm;
	BOOL bLink;
	int  nLink; // 0:Elastic Link, 1:Support
	CArray<T_NODE_K, T_NODE_K> aLink;
	CArray<T_NODE_K, T_NODE_K> aSupport;

	T_EVGP_D()  { }
	T_EVGP_D(const T_EVGP_D& src) {*this = src;}
	T_EVGP_D& operator=(const T_EVGP_D& src)
	{
		strName = src.strName;
		nType = src.nType;
		nAbutPos = src.nAbutPos;
		bPierCap = src.bPierCap;
		aPierCap.Copy(src.aPierCap);
		aEvgpColm.Copy(src.aEvgpColm);
		bLink = src.bLink;    
		nLink = src.nLink;
		aLink.Copy(src.aLink);
		aSupport.Copy(src.aSupport);		
		return *this;
	}  

	void Initialize()
	{
		strName.Empty();
		nType = 0;
		nAbutPos = 0;
		bPierCap = FALSE;
		aPierCap.RemoveAll();
		aEvgpColm.RemoveAll();
		bLink = TRUE;
		nLink = 0;
		aLink.RemoveAll();
		aSupport.RemoveAll();		
	}	
};

struct T_EVGP_COLM_D_CH
{
	char strName[60];
	CArray<T_ELEM_K, T_ELEM_K> aColumn;

	void ConvertToString(T_EVGP_COLM_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.aColumn.Copy(aColumn);	
	}
	void ConvertToChar(T_EVGP_COLM_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		aColumn.Copy(rData.aColumn);
	}
};

struct T_EVGP_D_CH
{
	char strName[60];
	int  nType; // 0:Pier, 1:Abutment
	int  nAbutPos; // 0:Start, 1:End
	BOOL bPierCap;
	CArray<T_ELEM_K, T_ELEM_K> aPierCap;
	CArray<T_EVGP_COLM_D_CH, T_EVGP_COLM_D_CH&> aEvgpColm;
	BOOL bLink;
	int  nLink; // 0:Elastic Link, 1:Support
	CArray<T_NODE_K, T_NODE_K> aLink;
	CArray<T_NODE_K, T_NODE_K> aSupport;	

	void ConvertToString(T_EVGP_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nType = nType;
		rData.nAbutPos = nAbutPos;
		rData.bPierCap = bPierCap;
		rData.aPierCap.Copy(aPierCap);

		int nCount = (int)aEvgpColm.GetSize();
		rData.aEvgpColm.SetSize(nCount);
		for (int i = 0; i < nCount; i++) 
		{
			aEvgpColm[i].ConvertToString(rData.aEvgpColm[i]);
		}
		
		rData.bLink = bLink;
		rData.nLink = nLink;
		rData.aLink.Copy(aLink);
		rData.aSupport.Copy(aSupport);		
	}
	void ConvertToChar(T_EVGP_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		nType = rData.nType;
		nAbutPos = rData.nAbutPos;
		bPierCap = rData.bPierCap;
		aPierCap.Copy(rData.aPierCap);

		int nCount = (int)rData.aEvgpColm.GetSize();
		aEvgpColm.SetSize(nCount);
		for (int i = 0; i < nCount; i++) 
		{
			aEvgpColm[i].ConvertToChar(rData.aEvgpColm[i]);
		}
		
		bLink = rData.bLink;
		nLink = rData.nLink;
		aLink.Copy(rData.aLink);
		aSupport.Copy(rData.aSupport);		
	}
};

struct T_EVGP_UDRD_D
{
	T_EVGP_K key;
	T_EVGP_D data;
};

struct T_EVGP_UDRD_D_CH
{
	T_EVGP_K key;
	T_EVGP_D_CH data;
};

struct T_EVGP_COLM_D_CH_RW
{
	char strName[60];
	int nCountColumn;

	void GetEvgp(T_EVGP_COLM_D_CH& rData)
	{
		memcpy(rData.strName, strName, sizeof(strName));
	}
	void SetEvgp(const T_EVGP_COLM_D_CH& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));
		nCountColumn = (int)rData.aColumn.GetSize();		
	}
};

struct T_EVGP_UDRD_D_CH_RW_885
{
	T_EVGP_K key;
	char strName[60];
	BOOL bPierCap;
	int nCountPierCap;
	int nCountColumn;
	BOOL bLink;
	int nCountLink;

	void GetEvgp(T_EVGP_K& rKey, T_EVGP_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.strName, strName, sizeof(strName));
		rData.nType = 0;
		rData.nAbutPos = 0; // 930 추가
		rData.bPierCap = bPierCap;
		rData.bLink = bLink;    
	}
};

struct T_EVGP_UDRD_D_CH_RW_925
{
	T_EVGP_K key;
	char strName[60];
	int  nType; // 0:Pier, 1:Abutment
	BOOL bPierCap;
	int nCountPierCap;
	int nCountEvgpColm;
	BOOL bLink;
	int nLink;
	int nCountLink;
	int nCountSupport;
};

struct T_EVGP_UDRD_D_CH_RW
{
	T_EVGP_K key;
	char strName[60];
	int  nType; // 0:Pier, 1:Abutment
	int  nAbutPos; // 0:Start, 1:End
	BOOL bPierCap;
	int nCountPierCap;
	int nCountEvgpColm;
	BOOL bLink;
	int nLink;
	int nCountLink;
	int nCountSupport;	

	void GetEvgp(T_EVGP_K& rKey, T_EVGP_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.strName, strName, sizeof(strName));
		rData.nType = nType;
		rData.nAbutPos = nAbutPos;
		rData.bPierCap = bPierCap;
		rData.bLink = bLink;    
		rData.nLink = nLink;
	}
	void SetEvgp(const T_EVGP_K Key, const T_EVGP_D_CH& rData)
	{
		key = Key;
		memcpy(strName, rData.strName, sizeof(strName));
		nType = rData.nType;
		nAbutPos = rData.nAbutPos;
		bPierCap = rData.bPierCap;
		nCountPierCap = (int)rData.aPierCap.GetSize();
		nCountEvgpColm = (int)rData.aEvgpColm.GetSize();		
		bLink = rData.bLink;
		nLink = rData.nLink;
		nCountLink = (int)rData.aLink.GetSize();
		nCountSupport = (int)rData.aSupport.GetSize();		
	}
	void Convert925(T_EVGP_UDRD_D_CH_RW_925& rData)
	{
		key = rData.key;
		memcpy(strName, rData.strName, sizeof(strName));
		nType = rData.nType;
		nAbutPos = 0;
		bPierCap = rData.bPierCap;
		nCountPierCap = rData.nCountPierCap;
		nCountEvgpColm = rData.nCountEvgpColm;
		bLink = rData.bLink;
		nLink = rData.nLink;
		nCountLink = rData.nCountLink;
		nCountSupport = rData.nCountSupport;
	}
};

struct T_EVGP_UNIT
{
	T_EVGP_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEEVGP 1001
#define HASHSIZELISTEVGP 11

// Evaluation Parameter
#define T_EVPM_K_885 T_EVGP_K
#define T_EVPM_K unsigned int

struct T_EVPM_D_885
{
	int    nBehavL; // 0:라멘거동, 1:캔틸레버거동
	double dHe1L;
	double dHe2L;
	int    nBehavT; // 0:라멘거동, 1:캔틸레버거동
	double dHe1T;
	double dHe2T;
	double dAeRat;
	int    nBH;     // 종횡비 판단 방법
	int    nSplice; // 철근 상세
	double dLp;     // 횡방향 철근비
	double dTs;     // 통제 주기
};

struct T_EVPM_D
{
	int    nBehavL;   // 0:라멘거동, 1:캔틸레버거동
	double dHe1L;
	double dHe2L;
	int    nBehavT;   // 0:라멘거동, 1:캔틸레버거동
	double dHe1T;
	double dHe2T;
	double dAeRat;
	int    nBH;       // 종횡비 판단 방법
	int    nSplice;   // 철근 상세
	double dLp;       // 횡방향 철근비
	double dTs;       // 통제 주기
	T_EVGP_K EvgpK;   // Evaluation Group
	int    nEvgpColm; // Evaluation Group Column Index

	T_EVPM_D()  { }
	T_EVPM_D(T_EVPM_D& src) {*this = src;}
	T_EVPM_D& operator=(T_EVPM_D& src)
	{
		nBehavL = src.nBehavL;
		dHe1L = src.dHe1L;
		dHe2L = src.dHe2L;
		nBehavT = src.nBehavT;
		dHe1T = src.dHe1T;
		dHe2T = src.dHe2T;
		dAeRat = src.dAeRat;
		nBH = src.nBH;
		nSplice = src.nSplice;
		dLp = src.dLp;
		dTs = src.dTs;
		EvgpK = src.EvgpK;
		nEvgpColm = src.nEvgpColm;
		return *this;
	}  
	
	void Initialize()
	{
		nBehavL = 0;
		dHe1L = 0.0;
		dHe2L = 0.0;
		nBehavT = 0;
		dHe1T = 0.0;
		dHe2T = 0.0;
		dAeRat = 0.8;
		nBH = 0;
		nSplice = 0;
		dLp = 0.0;
		dTs = 0.0;
		EvgpK = 0;
		nEvgpColm = 0;
	}	

	void Convert885(T_EVPM_D_885& rData)
	{
		nBehavL = rData.nBehavL;
		dHe1L = rData.dHe1L;
		dHe2L = rData.dHe2L;
		nBehavT = rData.nBehavT;
		dHe1T = rData.dHe1T;
		dHe2T = rData.dHe2T;
		dAeRat = rData.dAeRat;
		nBH = rData.nBH;
		nSplice = rData.nSplice;
		dLp = rData.dLp;
		dTs = rData.dTs;
		EvgpK = 0;
		nEvgpColm = 0;
	}
};

struct T_EVPM_UDRD_D_885
{
	T_EVPM_K key;
	T_EVPM_D_885 data;
};

struct T_EVPM_UDRD_D
{
	T_EVPM_K key;
	T_EVPM_D data;
};

struct T_EVPM_UNIT
{
	T_EVPM_UNIT()
	{
		dHe1L = D_UNITSYS_BASE_LENGTH;
		dHe2L = D_UNITSYS_BASE_LENGTH;
		dHe1T = D_UNITSYS_BASE_LENGTH;
		dHe2T = D_UNITSYS_BASE_LENGTH;
		dTs = D_UNITSYS_BASE_TIME;
	}
	int dHe1L;
	int dHe2L;
	int dHe1T;
	int dHe2T;
	int dTs;
};
#define HASHSIZEEVPM HASHSIZEEVGP

// Bridge Information
#define T_SBRI_K unsigned int

struct T_SBRI_D
{
	CString strBrgName; // 교량명
	CString strLocation; // 위치
	CString strManager; // 관리주체
	CString strLineNum; // 노선번호
	CString strOrgLoc; // 소재지
	int nBuildYear; // 준공년도
	int nSeismicClass; // 내진등급 0:특, 1:1, 2:2
	int nEarthquake; // 지진구역 0:1, 1:2
	int nMvCode; // 설계하중
	int nMvhlStd;
	CString strMvhlType;
	CString strMvhlName; // 이게 Blank 이면 strMvhlType 가 의미 있는 이름이 됨
	double dTotalLen; // 총연장
	CArray<double, double> aSpan; // 연속경간형식
	double dBrgWidth; // 교폭
	int nUpper; // 상부형식  0:None, 1:PSC-I Girder, 2:PSC-Box Girder, 3:RC Slab, 4:Steel-I Girder, 5:Steel-Box Girder
	CString strUpper;
	int nAbutmentType;  //교대형식 0:None, 1:역T형, 2:중력식, 3:부벽식, 4:박스형, 5:라멘식, 6:중간이음식
	CString strAbutmentType;
	int nAbutmentBase; // 교대기초 0:None, 1:직접기초, 2:Mass기초, 3:말뚝기초(PHC), 4:말뚝기초(강관), 5:말뚝기초(현장타설), 6:우물통기초, 7:확대기초
	CString strAbutmentBase;
	int nPierType; // 교각형식 0:None, 1:T형, 2:문형, 3:횟불형, 4:라멘식
	CString strPierType;
	int nPierBase; // 교각기초 0:None, 1:직접기초, 2:Mass기초, 3:말뚝기초(PHC), 4:말뚝기초(강관), 5:말뚝기초(현장타설), 6:우물통기초, 7:확대기초
	CString strPierBase;
	int nSupportMatl; //받침제원 0:None, 1:고력황동받침, 2:탄성받침, 3:포트받침
	CString strSupportMatl; 
	double dSupportCapaAbut; // 받침용량(교대)
	double dSupportCapaPier; // 받침용량(교각)
	int nPierSect; // 교각단면 0:None, 1:원형, 2:사각형, 3:트랙형
	CString strPierSect;
	UINT SpfcK; // 응답스펙트럼
	
	void Initialize()
	{
		strBrgName=""; 
		strLocation=""; 
		strManager=""; 
		strLineNum=""; 
		strOrgLoc=""; 
		nBuildYear=0; 
		nSeismicClass=1; 
		nEarthquake=0; 
		nMvCode=0; 
		nMvhlStd=0;
		strMvhlType="";
		strMvhlName="";
		dTotalLen=300.0;
		aSpan.RemoveAll(); 
		dBrgWidth=13.0;
		nUpper=1; 
		strUpper="";
		nAbutmentType=1;  
		strAbutmentType="";
		nAbutmentBase=1; 
		strAbutmentBase="";
		nPierType=1; 
		strPierType="";
		nPierBase=3; 
		strPierBase="";
		nSupportMatl=1; 
		strSupportMatl=""; 
		dSupportCapaAbut=1000.0;
		dSupportCapaPier=1000.0;
		nPierSect=1; 
		strPierSect="";
		SpfcK=NULL; 
	}

	T_SBRI_D() {}
	T_SBRI_D(const T_SBRI_D& rData) { *this = rData; }
	T_SBRI_D& operator=(const T_SBRI_D& rData)
	{
		strBrgName = rData.strBrgName;
		strLocation = rData.strLocation;
		strManager = rData.strManager;
		strLineNum = rData.strLineNum;
		strOrgLoc = rData.strOrgLoc;
		nBuildYear = rData.nBuildYear;
		nSeismicClass = rData.nSeismicClass;
		nEarthquake = rData.nEarthquake;
		nMvCode = rData.nMvCode;
		nMvhlStd = rData.nMvhlStd;
		strMvhlType = rData.strMvhlType;
		strMvhlName = rData.strMvhlName;
		dTotalLen = rData.dTotalLen;
		aSpan.Copy(rData.aSpan);
		dBrgWidth = rData.dBrgWidth;
		nUpper = rData.nUpper;
		strUpper = rData.strUpper;
		nAbutmentType = rData.nAbutmentType;
		strAbutmentType = rData.strAbutmentType;
		nAbutmentBase = rData.nAbutmentBase;
		strAbutmentBase = rData.strAbutmentBase;
		nPierType = rData.nPierType;
		strPierType = rData.strPierType;
		nPierBase = rData.nPierBase;
		strPierBase = rData.strPierBase;
		nSupportMatl = rData.nSupportMatl;
		strSupportMatl = rData.strSupportMatl;
		dSupportCapaAbut = rData.dSupportCapaAbut;
		dSupportCapaPier = rData.dSupportCapaPier;
		nPierSect = rData.nPierSect;
		strPierSect = rData.strPierSect;
		SpfcK = rData.SpfcK;

		return *this;
	}
};

struct T_SBRI_D_CH
{
	char strBrgName[100]; // 교량명
	char strLocation[200]; // 위치
	char strManager[100]; // 관리주체
	char strLineNum[100]; // 노선번호
	char strOrgLoc[200]; // 소재지
	int nBuildYear; // 준공년도
	int nSeismicClass; // 내진등급 0:특, 1:1, 2:2
	int nEarthquake; // 지진구역 0:1, 1:2
	int nMvCode; // 설계하중
	int nMvhlStd;
	char strMvhlType[60];
	char strMvhlName[60];
	double dTotalLen; // 총연장
	CArray<double, double> aSpan; // 연속경간형식
	double dBrgWidth; // 교폭
	int nUpper; // 상부형식  0:None, 1:PSC-I Girder, 2:PSC-Box Girder, 3:RC Slab, 4:Steel-I Girder, 5:Steel-Box Girder
	char strUpper[100];
	int nAbutmentType;  //교대형식 0:None, 1:역T형, 2:중력식, 3:부벽식, 4:박스형, 5:라멘식, 6:중간이음식
	char strAbutmentType[100];
	int nAbutmentBase; // 교대기초 0:None, 1:직접기초, 2:Mass기초, 3:말뚝기초(PHC), 4:말뚝기초(강관), 5:말뚝기초(현장타설), 6:우물통기초, 7:확대기초
	char strAbutmentBase[100];
	int nPierType; // 교각형식 0:None, 1:T형, 2:문형, 3:횟불형, 4:라멘식
	char strPierType[100];
	int nPierBase; // 교각기초 0:None, 1:직접기초, 2:Mass기초, 3:말뚝기초(PHC), 4:말뚝기초(강관), 5:말뚝기초(현장타설), 6:우물통기초, 7:확대기초
	char strPierBase[100];
	int nSupportMatl; //받침제원 0:None, 1:고력황동받침, 2:탄성받침, 3:포트받침
	char strSupportMatl[100]; 
	double dSupportCapaAbut; // 받침용량(교대)
	double dSupportCapaPier; // 받침용량(교각)
	int nPierSect; // 교각단면 0:None, 1:원형, 2:사각형, 3:트랙형
	char strPierSect[100];
	UINT SpfcK; // 응답스펙트럼

	void ConvertToString(T_SBRI_D& rData)
	{
		ConvertCharStr(strBrgName, rData.strBrgName, sizeof(strBrgName));
		ConvertCharStr(strLocation, rData.strLocation, sizeof(strLocation));
		ConvertCharStr(strManager, rData.strManager, sizeof(strManager));
		ConvertCharStr(strLineNum, rData.strLineNum, sizeof(strLineNum));
		ConvertCharStr(strOrgLoc, rData.strOrgLoc, sizeof(strOrgLoc));
		rData.nBuildYear = nBuildYear;
		rData.nSeismicClass = nSeismicClass;
		rData.nEarthquake = nEarthquake;
		rData.nMvCode = nMvCode;
		rData.nMvhlStd = nMvhlStd;
		ConvertCharStr(strMvhlType, rData.strMvhlType, sizeof(strMvhlType));
		ConvertCharStr(strMvhlName, rData.strMvhlName, sizeof(strMvhlName));
		rData.dTotalLen = dTotalLen;
		rData.aSpan.Copy(aSpan);
		rData.dBrgWidth = dBrgWidth;
		rData.nUpper = nUpper;
		ConvertCharStr(strUpper, rData.strUpper, sizeof(strUpper));
		rData.nAbutmentType = nAbutmentType;
		ConvertCharStr(strAbutmentType, rData.strAbutmentType, sizeof(strAbutmentType));
		rData.nAbutmentBase = nAbutmentBase;
		ConvertCharStr(strAbutmentBase, rData.strAbutmentBase, sizeof(strAbutmentBase));
		rData.nPierType = nPierType;
		ConvertCharStr(strPierType, rData.strPierType, sizeof(strPierType));
		rData.nPierBase = nPierBase;
		ConvertCharStr(strPierBase, rData.strPierBase, sizeof(strPierBase));
		rData.nSupportMatl = nSupportMatl;
		ConvertCharStr(strSupportMatl, rData.strSupportMatl, sizeof(strSupportMatl));
		rData.dSupportCapaAbut = dSupportCapaAbut;
		rData.dSupportCapaPier = dSupportCapaPier;
		rData.nPierSect = nPierSect;
		ConvertCharStr(strPierSect, rData.strPierSect, sizeof(strPierSect));
		rData.SpfcK = SpfcK;
	}
	void ConvertToChar(T_SBRI_D& rData)
	{
		ConvertStrChar(rData.strBrgName, strBrgName, sizeof(strBrgName));
		ConvertStrChar(rData.strLocation, strLocation, sizeof(strLocation));
		ConvertStrChar(rData.strManager, strManager, sizeof(strManager));
		ConvertStrChar(rData.strLineNum, strLineNum, sizeof(strLineNum));
		ConvertStrChar(rData.strOrgLoc, strOrgLoc, sizeof(strOrgLoc));
		nBuildYear = rData.nBuildYear;
		nSeismicClass = rData.nSeismicClass;
		nEarthquake = rData.nEarthquake;
		nMvCode = rData.nMvCode;
		nMvhlStd = rData.nMvhlStd;
		ConvertStrChar(rData.strMvhlType, strMvhlType, sizeof(strMvhlType));    
		ConvertStrChar(rData.strMvhlName, strMvhlName, sizeof(strMvhlName));
		dTotalLen = rData.dTotalLen;
		aSpan.Copy(rData.aSpan);
		dBrgWidth = rData.dBrgWidth;
		nUpper = rData.nUpper;
		ConvertStrChar(rData.strUpper, strUpper, sizeof(strUpper));
		nAbutmentType = rData.nAbutmentType;
		ConvertStrChar(rData.strAbutmentType, strAbutmentType, sizeof(strAbutmentType));
		nAbutmentBase = rData.nAbutmentBase;
		ConvertStrChar(rData.strAbutmentBase, strAbutmentBase, sizeof(strAbutmentBase));
		nPierType = rData.nPierType;
		ConvertStrChar(rData.strPierType, strPierType, sizeof(strPierType));
		nPierBase = rData.nPierBase;
		ConvertStrChar(rData.strPierBase, strPierBase, sizeof(strPierBase));
		nSupportMatl = rData.nSupportMatl;
		ConvertStrChar(rData.strSupportMatl, strSupportMatl, sizeof(strSupportMatl));
		dSupportCapaAbut = rData.dSupportCapaAbut;
		dSupportCapaPier = rData.dSupportCapaPier;
		nPierSect = rData.nPierSect;
		ConvertStrChar(rData.strPierSect, strPierSect, sizeof(strPierSect));
		SpfcK = rData.SpfcK;
	}
};

struct T_SBRI_UDRD_D
{
	T_SBRI_D data;
};

struct T_SBRI_UDRD_D_CH
{
	T_SBRI_K key;
	T_SBRI_D_CH data;
};

struct T_SBRI_UDRD_D_CH_RW
{
	char strBrgName[100]; // 교량명
	char strLocation[200]; // 위치
	char strManager[100]; // 관리주체
	char strLineNum[100]; // 노선번호
	char strOrgLoc[200]; // 소재지
	int nBuildYear; // 준공년도
	int nSeismicClass; // 내진등급 0:특, 1:1, 2:2
	int nEarthquake; // 지진구역 0:1, 1:2
	int nMvCode; // 설계하중
	int nMvhlStd;
	char strMvhlType[60];
	char strMvhlName[60];
	double dTotalLen; // 총연장
	int nCountSpan; // 연속경간형식
	double dBrgWidth; // 교폭
	int nUpper; // 상부형식  0:None, 1:PSC-I Girder, 2:PSC-Box Girder, 3:RC Slab, 4:Steel-I Girder, 5:Steel-Box Girder
	char strUpper[100];
	int nAbutmentType;  //교대형식 0:None, 1:역T형, 2:중력식, 3:부벽식, 4:박스형, 5:라멘식, 6:중간이음식
	char strAbutmentType[100];
	int nAbutmentBase; // 교대기초 0:None, 1:직접기초, 2:Mass기초, 3:말뚝기초(PHC), 4:말뚝기초(강관), 5:말뚝기초(현장타설), 6:우물통기초, 7:확대기초
	char strAbutmentBase[100];
	int nPierType; // 교각형식 0:None, 1:T형, 2:문형, 3:횟불형, 4:라멘식
	char strPierType[100];
	int nPierBase; // 교각기초 0:None, 1:직접기초, 2:Mass기초, 3:말뚝기초(PHC), 4:말뚝기초(강관), 5:말뚝기초(현장타설), 6:우물통기초, 7:확대기초
	char strPierBase[100];
	int nSupportMatl; //받침제원 0:None, 1:고력황동받침, 2:탄성받침, 3:포트받침
	char strSupportMatl[100]; 
	double dSupportCapaAbut; // 받침용량(교대)
	double dSupportCapaPier; // 받침용량(교각)
	int nPierSect; // 교각단면 0:None, 1:원형, 2:사각형, 3:트랙형
	char strPierSect[100];
	UINT SpfcK; // 응답스펙트럼

	void GetSbri(T_SBRI_K& rKey, T_SBRI_D_CH& rData)
	{
		memcpy(rData.strBrgName, strBrgName, sizeof(strBrgName));
		memcpy(rData.strLocation, strLocation, sizeof(strLocation));
		memcpy(rData.strManager, strManager, sizeof(strManager));
		memcpy(rData.strLineNum, strLineNum, sizeof(strLineNum));
		memcpy(rData.strOrgLoc, strOrgLoc, sizeof(strOrgLoc));
		rData.nBuildYear = nBuildYear;    
		rData.nSeismicClass = nSeismicClass;
		rData.nEarthquake = nEarthquake;
		rData.nMvCode = nMvCode;
		rData.nMvhlStd = nMvhlStd;
		memcpy(rData.strMvhlType, strMvhlType, sizeof(strMvhlType));
		memcpy(rData.strMvhlName, strMvhlName, sizeof(strMvhlName));
		rData.dTotalLen = dTotalLen;
		//rData.aSpan.Copy(aSpan);
		rData.dBrgWidth = dBrgWidth;
		rData.nUpper = nUpper;
		memcpy(rData.strUpper, strUpper, sizeof(strUpper));
		rData.nAbutmentType = nAbutmentType;
		memcpy(rData.strAbutmentType, strAbutmentType, sizeof(strAbutmentType));
		rData.nAbutmentBase = nAbutmentBase;
		memcpy(rData.strAbutmentBase, strAbutmentBase, sizeof(strAbutmentBase));
		rData.nPierType = nPierType;
		memcpy(rData.strPierType, strPierType, sizeof(strPierType));
		rData.nPierBase = nPierBase;
		memcpy(rData.strPierBase, strPierBase, sizeof(strPierBase));
		rData.nSupportMatl = nSupportMatl;
		memcpy(rData.strSupportMatl, strSupportMatl, sizeof(strSupportMatl));    
		rData.dSupportCapaAbut = dSupportCapaAbut;
		rData.dSupportCapaPier = dSupportCapaPier;
		rData.nPierSect = nPierSect;
		memcpy(rData.strPierSect, strPierSect, sizeof(strPierSect));
		rData.SpfcK = SpfcK;
	}
	void SetSbri(const T_SBRI_K rKey, const T_SBRI_D_CH& rData)
	{
		memcpy(strBrgName, rData.strBrgName, sizeof(strBrgName));
		memcpy(strLocation, rData.strLocation, sizeof(strLocation));
		memcpy(strManager, rData.strManager, sizeof(strManager));
		memcpy(strLineNum, rData.strLineNum, sizeof(strLineNum));
		memcpy(strOrgLoc, rData.strOrgLoc, sizeof(strOrgLoc));
		nBuildYear = rData.nBuildYear;
		nSeismicClass = rData.nSeismicClass;
		nEarthquake = rData.nEarthquake;
		nMvCode = rData.nMvCode;
		nMvhlStd = rData.nMvhlStd;
		memcpy(strMvhlType, rData.strMvhlType, sizeof(strMvhlType));
		memcpy(strMvhlName, rData.strMvhlName, sizeof(strMvhlName));
		dTotalLen = rData.dTotalLen;
		nCountSpan = (int)rData.aSpan.GetSize();
		dBrgWidth = rData.dBrgWidth;
		nUpper = rData.nUpper;
		memcpy(strUpper, rData.strUpper, sizeof(strUpper));
		nAbutmentType = rData.nAbutmentType;
		memcpy(strAbutmentType, rData.strAbutmentType, sizeof(strAbutmentType));
		nAbutmentBase = rData.nAbutmentBase;
		memcpy(strAbutmentBase, rData.strAbutmentBase, sizeof(strAbutmentBase));
		nPierType = rData.nPierType;
		memcpy(strPierType, rData.strPierType, sizeof(strPierType));
		nPierBase = rData.nPierBase;
		memcpy(strPierBase, rData.strPierBase, sizeof(strPierBase));
		nSupportMatl = rData.nSupportMatl;
		memcpy(strSupportMatl, rData.strSupportMatl, sizeof(strSupportMatl));
		dSupportCapaAbut = rData.dSupportCapaAbut;
		dSupportCapaPier = rData.dSupportCapaPier;
		nPierSect = rData.nPierSect;
		memcpy(strPierSect, rData.strPierSect, sizeof(strPierSect));
		SpfcK = rData.SpfcK;
	}
};

struct T_SBRI_UNIT
{
	T_SBRI_UNIT()
	{ 
		dTotalLen = D_UNITSYS_BASE_LENGTH;
		aSpan = D_UNITSYS_BASE_LENGTH;
		dBrgWidth = D_UNITSYS_BASE_LENGTH;
		dSupportCapaAbut = D_UNITSYS_BASE_FORCE;
		dSupportCapaPier= D_UNITSYS_BASE_FORCE;
	}
	int dTotalLen;
	int aSpan;
	int dBrgWidth;
	int dSupportCapaAbut;
	int dSupportCapaPier;
};
#define HASHSIZESBRI 1

// Bearing Evaluation
#define T_BREV_K UINT
struct T_BREV_D
{
	CString  strName;
	T_EVGP_K EvgpK;
	int      nBearing;    // v910 부터 T_BRPR_D 에서 사용, 0:강제받침, 1:탄성받침
	int      nStlBearing; // v910 부터 T_BRPR_D 에서 사용, 0:고력황동받침, 1:포트받침(소켓매몰X) 2:포트받침(소켓매몰O)  : 
	double   dds;         // v910 부터 T_BRPR_D 에서 사용,
	double   dCapaL;      // v910 부터 T_BRPR_D 에서 사용,
	double   dDispL;      // v910 부터 T_BRPR_D 에서 사용,
	double   dCapaT;      // v910 부터 T_BRPR_D 에서 사용,
	double   dDispT;      // v910 부터 T_BRPR_D 에서 사용,
	int      n2wFixed;    // 양방향 고정단
	int      n2wMovable;  // 양방향 가동단
	int      n1wLMovable; // 일방향 (교축) 가동단
	int      n1wTMovable; // 일방향 (교직) 가동단

	T_BREV_D()  { }
	T_BREV_D(const T_BREV_D& src) {*this = src;}
	T_BREV_D& operator=(const T_BREV_D& src)
	{
		strName = src.strName;
		EvgpK = src.EvgpK;
		nBearing = src.nBearing;
		nStlBearing = src.nStlBearing;
		dds = src.dds;
		dCapaL = src.dCapaL;
		dDispL = src.dDispL;
		dCapaT = src.dCapaT;
		dDispT = src.dDispT;
		n2wFixed = src.n2wFixed;
		n2wMovable = src.n2wMovable;
		n1wLMovable = src.n1wLMovable;
		n1wTMovable = src.n1wTMovable;

		return *this;
	}  

	void Initialize()
	{
		strName.Empty();
		EvgpK = 0;
		nBearing = 0;   
		nStlBearing = 0;
		dds = 0.0;
		dCapaL = 0.0;
		dDispL = 0.0;
		dCapaT = 0.0;
		dDispT = 0.0;
		n2wFixed = 0;
		n2wMovable = 0;
		n1wLMovable = 0;
		n1wTMovable = 0;
	}	
};

struct T_BREV_D_CH
{
	char strName[60];
	T_EVGP_K EvgpK;
	int      nBearing;    // 0:강제받침, 1:탄성받침
	int      nStlBearing; // 0:고력황동받침, 1:포트받침(소켓매몰X) 2:포트받침(소켓매몰O)
	double   dds;
	double   dCapaL;
	double   dDispL;
	double   dCapaT;
	double   dDispT;
	int      n2wFixed;    // 양방향 고정단
	int      n2wMovable;  // 양방향 가동단
	int      n1wLMovable; // 일방향 (교축) 가동단
	int      n1wTMovable; // 일방향 (교직) 가동단

	void ConvertToString(T_BREV_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.EvgpK = EvgpK;
		rData.nBearing = nBearing;
		rData.nStlBearing = nStlBearing;
		rData.dds = dds;
		rData.dCapaL = dCapaL;
		rData.dDispL = dDispL;
		rData.dCapaT = dCapaT;
		rData.dDispT = dDispT;
		rData.n2wFixed = n2wFixed;
		rData.n2wMovable = n2wMovable;
		rData.n1wLMovable = n1wLMovable;
		rData.n1wTMovable = n1wTMovable;
	}
	void ConvertToChar(T_BREV_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		EvgpK = rData.EvgpK;
		nBearing = rData.nBearing;
		nStlBearing = rData.nStlBearing;
		dds = rData.dds;
		dCapaL = rData.dCapaL;
		dDispL = rData.dDispL;
		dCapaT = rData.dCapaT;
		dDispT = rData.dDispT;
		n2wFixed = rData.n2wFixed;
		n2wMovable = rData.n2wMovable;
		n1wLMovable = rData.n1wLMovable;
		n1wTMovable = rData.n1wTMovable;
	}
};

struct T_BREV_UDRD_D
{
	T_BREV_K key;
	T_BREV_D data;
};

struct T_BREV_UDRD_D_CH
{
	T_BREV_K key;
	T_BREV_D_CH data;
};

struct T_BREV_UNIT
{
	T_BREV_UNIT()
	{
		dds = D_UNITSYS_BASE_LENGTH;
		dCapaL = D_UNITSYS_BASE_FORCE;
		dDispL = D_UNITSYS_BASE_LENGTH;
		dCapaT = D_UNITSYS_BASE_FORCE;
		dDispT = D_UNITSYS_BASE_LENGTH;
	}
	int dds;
	int dCapaL;
	int dDispL;
	int dCapaT;
	int dDispT;
};
#define HASHSIZEBREV 1001

// Bearing Property
#define T_BRPR_K UINT
struct T_BRPR_D
{
	CString strName;
	int nInstall; // 0:Cast In Place, 1:Post Installed
	int nAnchor;
	double dda;
	double dhef;
	double dfy;
	double dfuta;
	double ddh;
	double deh;
	double dNp;
	BOOL bContiWeld; // 연속 용접
	double dSx;
	int    nN1;
	double dS1;
	double dSy;
	int    nN2;
	double dS2;	

	int      nBearing;    // v910, 0:강제받침, 1:탄성받침
	int      nStlBearing; // v910,0:고력황동받침, 1:포트받침(소켓매몰X) 2:포트받침(소켓매몰O)  
	double   dds;         // v910
	double   dCapaL;
	double   dDispL;
	double   dCapaT;
	double   dDispT;

	T_BRPR_D()  { }
	T_BRPR_D(const T_BRPR_D& src) {*this = src;}
	T_BRPR_D& operator=(const T_BRPR_D& src)
	{
		strName = src.strName;
		nInstall = src.nInstall;
		nAnchor = src.nAnchor;
		dda = src.dda;
		dhef = src.dhef;
		dfy = src.dfy;
		dfuta = src.dfuta;
		ddh = src.ddh;
		deh = src.deh;
		dNp = src.dNp;		
		bContiWeld = src.bContiWeld;
		dSx = src.dSx;
		nN1 = src.nN1;
		dS1 = src.dS1;
		dSy = src.dSy;
		nN2 = src.nN2;
		dS2 = src.dS2;

		nBearing = src.nBearing;
		nStlBearing = src.nStlBearing;
		dds    = src.dds;
		dCapaL = src.dCapaL;
		dDispL = src.dDispL;
		dCapaT = src.dCapaT;
		dDispT = src.dDispT;
		return *this;
	}  

	void Initialize()
	{
		strName.Empty();
		nInstall = 0;
		nAnchor = 0;
		dda = 0.0;
		dhef = 0.0;
		dfy = 0.0;
		dfuta = 0.0;
		ddh = 0.0;
		deh = 0.0;
		dNp = 0.0;		
		bContiWeld = FALSE;
		dSx = 0.0;
		nN1 = 0;
		dS1 = 0.0;
		dSy = 0.0;
		nN2 = 0;
		dS2 = 0.0;

		nBearing = 0;
		nStlBearing = 0;
		dds = 0.0;
		dCapaL = 0.0;
		dDispL = 0.0;
		dCapaT = 0.0;
		dDispT = 0.0;
	}
};

struct T_BRPR_D_CH_905
{
	char strName[60];
	int nInstall; // 0:Cast In Place, 1:Post Installed
	int nAnchor;
	double dda;
	double dhef;
	double dfy;
	double dfuta;
	double deh;
	double ddh;
	double dNp;
	BOOL bContiWeld; // 연속 용접
	double dSx;
	int    nN1;
	double dS1;
	double dSy;
	int    nN2;
	double dS2;
};

struct T_BRPR_D_CH
{
	char strName[60];
	int nInstall; // 0:Cast In Place, 1:Post Installed
	int nAnchor;
	double dda;
	double dhef;
	double dfy;
	double dfuta;
	double deh;
	double ddh;
	double dNp;
	BOOL bContiWeld; // 연속 용접
	double dSx;
	int    nN1;
	double dS1;
	double dSy;
	int    nN2;
	double dS2;	

	int      nBearing;    // v910
	int      nStlBearing; // v910
	double   dds;         // v910
	double   dCapaL;      // v910
	double   dDispL;      // v910
	double   dCapaT;      // v910
	double   dDispT;      // v910

	void ConvertToString(T_BRPR_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nInstall = nInstall;
		rData.nAnchor = nAnchor;
		rData.dda = dda;
		rData.dhef = dhef;
		rData.dfy = dfy;
		rData.dfuta = dfuta;
		rData.ddh = ddh;
		rData.deh = deh;
		rData.dNp = dNp;		
		rData.bContiWeld = bContiWeld;
		rData.dSx = dSx;
		rData.nN1 = nN1;
		rData.dS1 = dS1;
		rData.dSy = dSy;
		rData.nN2 = nN2;
		rData.dS2 = dS2;

		rData.nBearing = nBearing;
		rData.nStlBearing = nStlBearing;
		rData.dds    = dds;
		rData.dCapaL = dCapaL;
		rData.dDispL = dDispL;
		rData.dCapaT = dCapaT;
		rData.dDispT = dDispT;
	}
	void ConvertToChar(T_BRPR_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		nInstall = rData.nInstall;
		nAnchor = rData.nAnchor;
		dda = rData.dda;
		dhef = rData.dhef;
		dfy = rData.dfy;
		dfuta = rData.dfuta;
		ddh = rData.ddh;
		deh = rData.deh;
		dNp = rData.dNp;		
		bContiWeld = rData.bContiWeld;
		dSx = rData.dSx;
		nN1 = rData.nN1;
		dS1 = rData.dS1;
		dSy = rData.dSy;
		nN2 = rData.nN2;
		dS2 = rData.dS2;

		nBearing = rData.nBearing;
		nStlBearing = rData.nStlBearing;
		dds = rData.dds;
		dCapaL = rData.dCapaL;
		dDispL = rData.dDispL;
		dCapaT = rData.dCapaT;
		dDispT = rData.dDispT;
	}
	void Convert905(T_BRPR_D_CH_905& rData)
	{
		memcpy(strName, rData.strName, sizeof(strName));
		nInstall = rData.nInstall;
		nAnchor = rData.nAnchor;
		dda = rData.dda;
		dhef = rData.dhef;
		dfy = rData.dfy;
		dfuta = rData.dfuta;
		ddh = rData.ddh;
		deh = rData.deh;
		dNp = rData.dNp;
		bContiWeld = rData.bContiWeld;
		dSx = rData.dSx;
		nN1 = rData.nN1;
		dS1 = rData.dS1;
		dSy = rData.dSy;
		nN2 = rData.nN2;
		dS2 = rData.dS2;

		// 데이터 체크를 넘기 위한 초기값
		nBearing = 0;
		nStlBearing = 0;
		dds = 1.0;
		dCapaL = 1.0;
		dDispL = 1.0;
		dCapaT = 1.0;
		dDispT = 1.0;
	}
};

struct T_BRPR_UDRD_D
{
	T_BRPR_K key;
	T_BRPR_D data;
};

struct T_BRPR_UDRD_D_CH_905
{
	T_BRPR_K key;
	T_BRPR_D_CH_905 data;
};

struct T_BRPR_UDRD_D_CH
{
	T_BRPR_K key;
	T_BRPR_D_CH data;
};

struct T_BRPR_UNIT
{
	T_BRPR_UNIT()
	{
		dda = D_UNITSYS_BASE_LENGTH;
		dhef = D_UNITSYS_BASE_LENGTH;
		dfy = D_UNITSYS_BASE_STRESS;
		dfuta = D_UNITSYS_BASE_STRESS;
		ddh = D_UNITSYS_BASE_LENGTH;
		deh = D_UNITSYS_BASE_LENGTH;
		dNp = D_UNITSYS_BASE_FORCE;		
		dSx = D_UNITSYS_BASE_LENGTH;
		dS1 = D_UNITSYS_BASE_LENGTH;
		dSy = D_UNITSYS_BASE_LENGTH;		
		dS2 = D_UNITSYS_BASE_LENGTH;

		dds = D_UNITSYS_BASE_LENGTH;
		dCapaL = D_UNITSYS_BASE_FORCE;
		dDispL = D_UNITSYS_BASE_LENGTH;
		dCapaT = D_UNITSYS_BASE_FORCE;
		dDispT = D_UNITSYS_BASE_LENGTH;

	}
	int dda;
	int dhef;
	int dfy;
	int dfuta;
	int ddh;
	int deh;
	int dNp;	
	int dSx;
	int dS1;
	int dSy;	
	int dS2;

	int dds;         // v910
	int dCapaL;      // v910
	int dDispL;      // v910
	int dCapaT;      // v910
	int dDispT;      // v910
};
#define HASHSIZEBRPR 1001

// Anchor Evaluation
#define T_ANEV_K UINT
struct T_ANEV_REBAR_D
{
	BOOL bTension;
	double dFyat;
	double ddsat;
	BOOL bShear;
	double dFyasL;
	double ddsasL;
	int nNasL;
	double dFyasT;
	double ddsasT;
	int nNasT;

	T_ANEV_REBAR_D()  { }
	T_ANEV_REBAR_D(const T_ANEV_REBAR_D& src) {*this = src;}
	T_ANEV_REBAR_D& operator=(const T_ANEV_REBAR_D& src)
	{
		bTension = src.bTension;
		dFyat = src.dFyat;
		ddsat = src.ddsat;
		bShear = src.bShear;
		dFyasL = src.dFyasL;
		ddsasL = src.ddsasL;
		nNasL = src.nNasL;
		dFyasT = src.dFyasT;
		ddsasT = src.ddsasT;
		nNasT = src.nNasT;
		return *this;
	}  

	void Initialize()
	{
		bTension = FALSE;
		dFyat = 0.0;
		ddsat = 0.0;
		bShear = FALSE;
		dFyasL = 0.0;
		ddsasL = 0.0;
		nNasL = 0;
		dFyasT = 0.0;
		ddsasT = 0.0;
		nNasT = 0;
	}	
};

struct T_ANEV_FAIL_SEL_LAYER_D
{
	CArray<BOOL, BOOL> aFailSel;

	void Initialize()
	{
		aFailSel.RemoveAll();
	}
	T_ANEV_FAIL_SEL_LAYER_D() { Initialize();}
	T_ANEV_FAIL_SEL_LAYER_D(const T_ANEV_FAIL_SEL_LAYER_D& src) { *this = src; }
	T_ANEV_FAIL_SEL_LAYER_D& operator=(const T_ANEV_FAIL_SEL_LAYER_D& src)
	{
		aFailSel.Copy(src.aFailSel);
		return *this;
	}
};

struct T_ANEV_FAIL_SEL_LAYER_D_RW
{
	int nCountFailSel;

	void Get(T_ANEV_FAIL_SEL_LAYER_D& rData)
	{
		rData.aFailSel.SetSize(nCountFailSel);
	}
	void Set(const T_ANEV_FAIL_SEL_LAYER_D& rData)
	{
		nCountFailSel = (int)rData.aFailSel.GetSize();
	}
};

struct T_ANEV_CONC_FAIL_D_925
{
	int nNum;
	double dAVc;
	double dAVco;
	double dHa;
	double dCa1;
	double dCa1_q;
	double dCa2;
	double dCa2_q;
};

struct T_ANEV_CONC_FAIL_D
{
	int nNum; 
	double dAVc;
	double dAVco;
	double dHa;
	double dCa1;
	double dCa1_q;
	double dCa2;
	double dCa2_q;

	CArray<T_ANEV_FAIL_SEL_LAYER_D, T_ANEV_FAIL_SEL_LAYER_D> aFailSelLayer;

	T_ANEV_CONC_FAIL_D()  { }
	T_ANEV_CONC_FAIL_D(const T_ANEV_CONC_FAIL_D& src) {*this = src;}
	T_ANEV_CONC_FAIL_D& operator=(const T_ANEV_CONC_FAIL_D& src)
	{
		nNum = src.nNum;
		dAVc = src.dAVc;
		dAVco = src.dAVco;
		dHa = src.dHa;
		dCa1 = src.dCa1;
		dCa1_q = src.dCa1_q;
		dCa2 = src.dCa2;
		dCa2_q = src.dCa2_q;
		aFailSelLayer.Copy(src.aFailSelLayer);
		return *this;
	}  

	void Initialize()
	{
		nNum = 0;
		dAVc = 0.0;
		dAVco = 0.0;
		dHa = 0.0;
		dCa1 = 0.0;
		dCa1_q = 0.0;
		dCa2 = 0.0;
		dCa2_q = 0.0;
		aFailSelLayer.RemoveAll();
	}	
};

struct T_ANEV_CONC_FAIL_D_RW
{
	int nNum;
	double dAVc;
	double dAVco;
	double dHa;
	double dCa1;
	double dCa1_q;
	double dCa2;
	double dCa2_q;
	int nCountFailSelLayer;

	void Get(T_ANEV_CONC_FAIL_D& rData)
	{
		rData.nNum = nNum;
		rData.dAVc = dAVc;
		rData.dAVco = dAVco;
		rData.dHa = dHa;
		rData.dCa1 = dCa1;
		rData.dCa1_q = dCa1_q;
		rData.dCa2 = dCa2;
		rData.dCa2_q = dCa2_q;
		rData.aFailSelLayer.SetSize(nCountFailSelLayer);
	}
	void Set(const T_ANEV_CONC_FAIL_D& rData)
	{
		nNum = rData.nNum;
		dAVc = rData.dAVc;
		dAVco = rData.dAVco;
		dHa = rData.dHa;
		dCa1 = rData.dCa1;
		dCa1_q = rData.dCa1_q;
		dCa2 = rData.dCa2;
		dCa2_q = rData.dCa2_q;
		nCountFailSelLayer = (int)rData.aFailSelLayer.GetSize();
	}
	void Convert925(T_ANEV_CONC_FAIL_D_925& rData)
	{
		nNum = rData.nNum;
		dAVc = rData.dAVc;
		dAVco = rData.dAVco;
		dHa = rData.dHa;
		dCa1 = rData.dCa1;
		dCa1_q = rData.dCa1_q;
		dCa2 = rData.dCa2;
		dCa2_q = rData.dCa2_q;
		nCountFailSelLayer = 0;
	}
};

struct T_ANEV_PRYOUT_D_925
{
	int nNum;
	double dANc;
	double dANco;
	double dHef;
	double dCa1;
	double dCa1_q;
	double dCa2;
	double dCa2_q;
};

struct T_ANEV_PRYOUT_D
{
	int nNum; 
	double dANc;
	double dANco;
	double dHef;
	double dCa1;
	double dCa1_q;
	double dCa2;
	double dCa2_q;

	CArray<T_ANEV_FAIL_SEL_LAYER_D, T_ANEV_FAIL_SEL_LAYER_D> aFailSelLayer;

	T_ANEV_PRYOUT_D()  { }
	T_ANEV_PRYOUT_D(const T_ANEV_PRYOUT_D& src) {*this = src;}
	T_ANEV_PRYOUT_D& operator=(const T_ANEV_PRYOUT_D& src)
	{
		nNum = src.nNum;
		dANc = src.dANc;
		dANco = src.dANco;
		dHef = src.dHef;
		dCa1 = src.dCa1;
		dCa1_q = src.dCa1_q;
		dCa2 = src.dCa2;
		dCa2_q = src.dCa2_q;
		aFailSelLayer.Copy(src.aFailSelLayer);
		return *this;
	}  

	void Initialize()
	{
		nNum = 0;
		dANc = 0.0;
		dANco = 0.0;
		dHef = 0.0;
		dCa1 = 0.0;
		dCa1_q = 0.0;
		dCa2 = 0.0;
		dCa2_q = 0.0;
		aFailSelLayer.RemoveAll();
	}	
};

struct T_ANEV_PRYOUT_D_RW
{
	int nNum;
	double dANc;
	double dANco;
	double dHef;
	double dCa1;
	double dCa1_q;
	double dCa2;
	double dCa2_q;
	int nCountFailSelLayer;

	void Get(T_ANEV_PRYOUT_D& rData)
	{
		rData.nNum = nNum;
		rData.dANc = dANc;
		rData.dANco = dANco;
		rData.dHef = dHef;
		rData.dCa1 = dCa1;
		rData.dCa1_q = dCa1_q;
		rData.dCa2 = dCa2;
		rData.dCa2_q = dCa2_q;
		rData.aFailSelLayer.SetSize(nCountFailSelLayer);
	}
	void Set(const T_ANEV_PRYOUT_D& rData)
	{
		nNum = rData.nNum;
		dANc = rData.dANc;
		dANco = rData.dANco;
		dHef = rData.dHef;
		dCa1 = rData.dCa1;
		dCa1_q = rData.dCa1_q;
		dCa2 = rData.dCa2;
		dCa2_q = rData.dCa2_q;
		nCountFailSelLayer = (int)rData.aFailSelLayer.GetSize();
	}
	void Convert925(T_ANEV_PRYOUT_D_925& rData)
	{
		nNum = rData.nNum;
		dANc = rData.dANc;
		dANco = rData.dANco;
		dHef = rData.dHef;
		dCa1 = rData.dCa1;
		dCa1_q = rData.dCa1_q;
		dCa2 = rData.dCa2;
		dCa2_q = rData.dCa2_q;
		nCountFailSelLayer = 0;
	}
};

struct T_ANEV_FAIL_D_925
{
	BOOL bUser[2]; // 0:콘크리트 파괴, 1:프라이아웃 파괴
	T_ANEV_CONC_FAIL_D_925 ConcFailL;
	T_ANEV_CONC_FAIL_D_925 ConcFailT;
	T_ANEV_PRYOUT_D_925 PryoutL;
	T_ANEV_PRYOUT_D_925 PryoutT;
};

struct T_ANEV_FAIL_D
{
	BOOL bUser[2]; // 0:콘크리트 파괴, 1:프라이아웃 파괴
	T_ANEV_CONC_FAIL_D ConcFailL;
	T_ANEV_CONC_FAIL_D ConcFailT;
	T_ANEV_PRYOUT_D PryoutL;
	T_ANEV_PRYOUT_D PryoutT;
	
	T_ANEV_FAIL_D()  { }
	T_ANEV_FAIL_D(const T_ANEV_FAIL_D& src) {*this = src;}
	T_ANEV_FAIL_D& operator=(const T_ANEV_FAIL_D& src)
	{
		bUser[0] = src.bUser[0];
		bUser[1] = src.bUser[1];
		ConcFailL = src.ConcFailL;
		ConcFailT = src.ConcFailT;
		PryoutL = src.PryoutL;
		PryoutT = src.PryoutT;
		return *this;
	}  

	void Initialize()
	{
		bUser[0] = FALSE;
		bUser[1] = FALSE;
		ConcFailL.Initialize();
		ConcFailT.Initialize();
		PryoutL.Initialize();
		PryoutT.Initialize();
	}	
};

struct T_ANEV_FAIL_D_RW
{
	BOOL bUser[2]; // 0:콘크리트 파괴, 1:프라이아웃 파괴
	T_ANEV_CONC_FAIL_D_RW ConcFailL;
	T_ANEV_CONC_FAIL_D_RW ConcFailT;
	T_ANEV_PRYOUT_D_RW PryoutL;
	T_ANEV_PRYOUT_D_RW PryoutT;

	void Get(T_ANEV_FAIL_D& rData)
	{
		rData.bUser[0] = bUser[0];
		rData.bUser[1] = bUser[1];
		ConcFailL.Get(rData.ConcFailL);
		ConcFailT.Get(rData.ConcFailT);
		PryoutL.Get(rData.PryoutL);
		PryoutT.Get(rData.PryoutT);
	}
	void Set(const T_ANEV_FAIL_D& rData)
	{
		bUser[0] = rData.bUser[0];
		bUser[1] = rData.bUser[1];
		ConcFailL.Set(rData.ConcFailL);
		ConcFailT.Set(rData.ConcFailT);
		PryoutL.Set(rData.PryoutL);
		PryoutT.Set(rData.PryoutT);
	}
	void Convert925(T_ANEV_FAIL_D_925& rData)
	{
		bUser[0] = rData.bUser[0];
		bUser[1] = rData.bUser[1];
		ConcFailL.Convert925(rData.ConcFailL);
		ConcFailT.Convert925(rData.ConcFailT);
		PryoutL.Convert925(rData.PryoutL);
		PryoutT.Convert925(rData.PryoutT);
	}
};

struct T_ANEV_REG_SUPP_LAYER_D
{
	CArray<int, int> aBrevType; // 0:고정단, 1:양방향 가동단, 2:교축 가동단, 3:교직 가동단
								// -1 : 비어있음
	void Initialize()
	{
		aBrevType.RemoveAll();
	}
	T_ANEV_REG_SUPP_LAYER_D() { Initialize(); }
	T_ANEV_REG_SUPP_LAYER_D(const T_ANEV_REG_SUPP_LAYER_D& src) {*this = src;}
	T_ANEV_REG_SUPP_LAYER_D& operator=(const T_ANEV_REG_SUPP_LAYER_D& src)
	{
		aBrevType.Copy(src.aBrevType);
		return *this;
	}  


};

struct T_ANEV_REG_SUPP_LAYER_D_RW
{
	int nCountBrevType;

	void Get(T_ANEV_REG_SUPP_LAYER_D& rData)
	{
		rData.aBrevType.SetSize(nCountBrevType);
	}
	void Set(const T_ANEV_REG_SUPP_LAYER_D& rData)
	{
		nCountBrevType = (int)rData.aBrevType.GetSize();
	}
};

struct T_ANEV_REG_SUPP_TYPE_D
{
	CArray<T_ANEV_REG_SUPP_LAYER_D, T_ANEV_REG_SUPP_LAYER_D&> aLayer;

	T_ANEV_REG_SUPP_TYPE_D()  { }
	T_ANEV_REG_SUPP_TYPE_D(const T_ANEV_REG_SUPP_TYPE_D& src) {*this = src;}
	T_ANEV_REG_SUPP_TYPE_D& operator=(const T_ANEV_REG_SUPP_TYPE_D& src)
	{
		aLayer.Copy(src.aLayer);
		return *this;
	}  

	void Initialize()
	{
		aLayer.RemoveAll();
	}	
};

struct T_ANEV_REG_SUPP_TYPE_D_RW
{
	int nCountLayer;

	void Set(const T_ANEV_REG_SUPP_TYPE_D& rData)
	{
		nCountLayer = (int)rData.aLayer.GetSize();
	}
};

struct T_ANEV_BRPR_KEYS_D
{
	CArray<T_BRPR_K, T_BRPR_K> aBrprK;  // 0 : 비어있음

	void Initialize()
	{
		aBrprK.RemoveAll();
	}
	T_ANEV_BRPR_KEYS_D() { Initialize();}
	T_ANEV_BRPR_KEYS_D(const T_ANEV_BRPR_KEYS_D& src) {*this = src;}
	T_ANEV_BRPR_KEYS_D& operator=(const T_ANEV_BRPR_KEYS_D& src)
	{
		aBrprK.Copy(src.aBrprK);
		return *this;
	}  
};

struct T_ANEV_BRPR_KEYS_D_RW
{
	int nCountBrprK;

	void Get(T_ANEV_BRPR_KEYS_D& rData)
	{
		rData.aBrprK.SetSize(nCountBrprK);
	}
	void Set(const T_ANEV_BRPR_KEYS_D& rData)
	{
		nCountBrprK = (int)rData.aBrprK.GetSize();
	}
};

struct T_ANEV_BRPR_D
{
	CArray<T_ANEV_BRPR_KEYS_D, T_ANEV_BRPR_KEYS_D&> aBrprKs;

	T_ANEV_BRPR_D()  { }
	T_ANEV_BRPR_D(const T_ANEV_BRPR_D& src) {*this = src;}
	T_ANEV_BRPR_D& operator=(const T_ANEV_BRPR_D& src)
	{
		aBrprKs.Copy(src.aBrprKs);
		return *this;
	}  

	void Initialize()
	{
		aBrprKs.RemoveAll();
	}	
};

struct T_ANEV_BRPR_D_RW
{
	int nCountBrprKs;

	void Get(T_ANEV_BRPR_D& rData)
	{
		rData.aBrprKs.SetSize(nCountBrprKs);
	}
	void Set(const T_ANEV_BRPR_D& rData)
	{
		nCountBrprKs = (int)rData.aBrprKs.GetSize();
	}
};


struct T_ANEV_ELNK_NODE_D
{
	CArray<T_NODE_K, T_NODE_K> aNodeK1;  // 0 : 비어있음
	CArray<T_NODE_K, T_NODE_K> aNodeK2;  

	T_ANEV_ELNK_NODE_D()  { }
	T_ANEV_ELNK_NODE_D(const T_ANEV_ELNK_NODE_D& src) {*this = src;}
	T_ANEV_ELNK_NODE_D& operator=(const T_ANEV_ELNK_NODE_D& src)
	{
		aNodeK1.Copy(src.aNodeK1);
		aNodeK2.Copy(src.aNodeK2);

		return *this;
	}  

	void Initialize()
	{
		aNodeK1.RemoveAll();
		aNodeK2.RemoveAll();
	}	
};

struct T_ANEV_ELNK_NODE_D_RW
{
	int nCountNodeK1;
	//int nCountNodeK2; 어차피 nCountNodeK1 와 nCountNodeK2 는 같으니 1번만 쓰자..

	void Get(T_ANEV_ELNK_NODE_D& rData)
	{
		rData.aNodeK1.SetSize(nCountNodeK1);
		rData.aNodeK2.SetSize(nCountNodeK1);
	}
	void Set(const T_ANEV_ELNK_NODE_D& rData)
	{
		nCountNodeK1 = (int)rData.aNodeK1.GetSize();
		//nCountNodeK2 = (int)rData.aNodeK2.GetSize();
	}
};

struct T_ANEV_TENS_CHK_D
{
	CArray<T_ANEV_ELNK_NODE_D, T_ANEV_ELNK_NODE_D&> aElnkNodeKs;

	T_ANEV_TENS_CHK_D()  { }
	T_ANEV_TENS_CHK_D(const T_ANEV_TENS_CHK_D& src) {*this = src;}
	T_ANEV_TENS_CHK_D& operator=(const T_ANEV_TENS_CHK_D& src)
	{
		aElnkNodeKs.Copy(src.aElnkNodeKs);
		return *this;
	}  

	void Initialize()
	{
		aElnkNodeKs.RemoveAll();
	}	
};

struct T_ANEV_TENS_CHK_D_RW
{
	int nCountElnkNodeKs;

	void Set(const T_ANEV_TENS_CHK_D& rData)
	{
		nCountElnkNodeKs = (int)rData.aElnkNodeKs.GetSize();
	}
};

struct T_ANEV_LAYER_D
{
	double dTheta_q1; // θ' (q1:Quotes1)
	double dTheta_q2; // θ'' (q2:Quotes2)
	double dD1;
	double dD1_q;
	CArray<double, double> aSax;
	double dD2;
	double dD2_q;
	double dSay;

	void Initialize()
	{
		dTheta_q1 = 0.0;
		dTheta_q2 = 0.0;
		dD1 = 0.0;
		dD1_q = 0.0;
		aSax.RemoveAll();
		dD2 = 0.0;
		dD2_q = 0.0;
		dSay = 0.0;
	}

	T_ANEV_LAYER_D() { Initialize(); }
	T_ANEV_LAYER_D(const T_ANEV_LAYER_D& src) { *this = src; }
	T_ANEV_LAYER_D& operator=(const T_ANEV_LAYER_D& src)
	{
		dTheta_q1 = src.dTheta_q1;
		dTheta_q2 = src.dTheta_q2;
		dD1 = src.dD1;
		dD1_q = src.dD1_q;
		aSax.Copy(src.aSax);
		dD2 = src.dD2;
		dD2_q = src.dD2_q;
		dSay = src.dSay;

		return *this;
	}
};

struct T_ANEV_LAYER_D_RW
{
	double dTheta_q1; // θ' (q1:Quotes1)
	double dTheta_q2; // θ'' (q2:Quotes2)
	double dD1;
	double dD1_q;
	int nCountSax;
	double dD2;
	double dD2_q;
	double dSay;

	void Get(T_ANEV_LAYER_D& rData)
	{
		rData.dTheta_q1 = dTheta_q1;
		rData.dTheta_q2 = dTheta_q2;
		rData.dD1 = dD1;
		rData.dD1_q = dD1_q;
		rData.aSax.SetSize(nCountSax);
		rData.dD2 = dD2;
		rData.dD2_q = dD2_q;
		rData.dSay = dSay;
	}
	void Set(const T_ANEV_LAYER_D& rData)
	{
		dTheta_q1 = rData.dTheta_q1;
		dTheta_q2 = rData.dTheta_q2;
		dD1 = rData.dD1;
		dD1_q = rData.dD1_q;
		nCountSax = (int)rData.aSax.GetSize();
		dD2 = rData.dD2;
		dD2_q = rData.dD2_q;
		dSay = rData.dSay;
	}
};

struct T_ANEV_D
{
	CString strName;
	CString strDescription;
	T_BREV_K BrevK;
	double dfck;
	double dHcop;
	T_BRPR_K BrprK; // 905 부터 사용 안함 BrprKeys 로 대체
	BOOL bRebar;
	T_ANEV_REBAR_D Rebar;
	double dTheta1;
	double dTheta2;
	double dTheta_q1; // θ' (q1:Quotes1) 930 부터 사용 안함 aLayer.dTheta_q1 로 대체
	double dTheta_q2; // θ'' (q2:Quotes2) 930 부터 사용 안함 aLayer.dTheta_q2 로 대체
	double dD1;       // 930 부터 사용 안함 aLayer.dD1 로 대체
	double dD1_q;     // 930 부터 사용 안함 aLayer.dD1_q 로 대체
	int    nN1;       // 891 부터 사용 안함 aSax 로 대체
	double dSax;      // 891 부터 사용 안함 aSax 로 대체 
	CArray<double, double> aSax; // 930 부터 사용 안함 aLayer.aSax 로 대체
	double dD2;       // 930 부터 사용 안함 aLayer.dD2 로 대체
	double dD2_q;     // 930 부터 사용 안함 aLayer.dD2_q 로 대체
	int    nN2;       // 891 부터 사용 안함 aSay 로 대체 
	double dSay;      // 891 부터 사용 안함 aSay 로 대체  
	CArray<double, double> aSay; // 930 부터 사용 안함 aLayer.dSay 로 대체
	T_ANEV_FAIL_D Fail;
	BOOL   bSuppType; // 사용 안함
	T_ANEV_REG_SUPP_TYPE_D SuppType;
	BOOL bRelAngle;             // 910, FALSE : Auto,  True : User
	double dRelAngle;           // 910 상대각도
	T_ANEV_BRPR_D BearingProp;	// 910
	BOOL bTensChk;				// 910
	T_ANEV_TENS_CHK_D TensChk;	// 910
	double dB;                  // 930
	double dH;                  // 930   
	double dCopingAngle;        // 930   
	CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D> aLayer; // 930   

	BOOL bShearFactor;          // v950 -전단강도 수정계수
	int  nShearFactorType;      // v950 
	BOOL bPryout;               // v950 -프라이아웃 설계검토
	BOOL bPryoutRow;            // v950 -전열+후열검토

	T_ANEV_D()  { }
	T_ANEV_D(const T_ANEV_D& src) {*this = src;}
	T_ANEV_D& operator=(const T_ANEV_D& src)
	{
		strName = src.strName;
		strDescription = src.strDescription;
		BrevK = src.BrevK;
		dfck = src.dfck;
		dHcop = src.dHcop;
		BrprK = src.BrprK;
		bRebar = src.bRebar;
		Rebar = src.Rebar;
		dTheta1 = src.dTheta1;
		dTheta2 = src.dTheta2;
		dTheta_q1 = src.dTheta_q1;
		dTheta_q2 = src.dTheta_q2;
		dD1 = src.dD1;
		dD1_q = src.dD1_q;
		nN1 = src.nN1;
		dSax = src.dSax;
		aSax.Copy(src.aSax);
		dD2 = src.dD2;
		dD2_q = src.dD2_q;
		nN2 = src.nN2;
		dSay = src.dSay;
		aSay.Copy(src.aSay);
		Fail = src.Fail;
		bSuppType = src.bSuppType;
		SuppType = src.SuppType;
		bRelAngle = src.bRelAngle;
		dRelAngle = src.dRelAngle;
		BearingProp = src.BearingProp;
		bTensChk = src.bTensChk;
		TensChk = src.TensChk;
		dB = src.dB;
		dH = src.dH;
		dCopingAngle = src.dCopingAngle;
		aLayer.Copy(src.aLayer);

		bShearFactor     = src.bShearFactor;
		nShearFactorType = src.nShearFactorType;
		bPryout          = src.bPryout;
		bPryoutRow       = src.bPryoutRow;
		return *this;
	}  

	void Initialize()
	{
		strName.Empty();
		strDescription.Empty();
		BrevK = 0;
		dfck = 0.0;
		dHcop = 0.0;
		BrprK = 0;
		bRebar = FALSE;
		Rebar.Initialize();
		dTheta1 = 0.0;
		dTheta2 = 0.0;
		dTheta_q1 = 0.0;
		dTheta_q2 = 0.0;
		dD1 = 0.0;
		dD1_q = 0.0;
		nN1 = 0;
		dSax = 0.0;
		aSax.RemoveAll();
		dD2 = 0.0;
		dD2_q = 0.0;
		nN2 = 0;
		dSay = 0.0;
		aSay.RemoveAll();
		Fail.Initialize();
		bSuppType = FALSE;
		SuppType.Initialize();
		bRelAngle = FALSE; // Auto
		dRelAngle = 0.0;
		BearingProp.Initialize();
		bTensChk = FALSE;
		TensChk.Initialize();
		dB = 0.0;
		dH = 0.0;
		dCopingAngle = 0.0;
		aLayer.RemoveAll();
		bShearFactor = FALSE;
		nShearFactorType = 0;
		bPryout = TRUE;
		bPryoutRow = FALSE;
	}	
};

struct T_ANEV_D_CH_890
{
	char strName[60];
	T_BREV_K BrevK;
	double dfck;
	double dHcop;
	T_BRPR_K BrprK;
	BOOL bRebar;
	T_ANEV_REBAR_D Rebar;
	double dTheta1;
	double dTheta2;
	double dTheta_q1; // θ' (q1:Quotes1)
	double dTheta_q2; // θ'' (q2:Quotes2)
	double dD1;
	double dD1_q;
	int    nN1;
	double dSax;
	double dD2;
	double dD2_q;
	int    nN2;
	double dSay;
	T_ANEV_FAIL_D_925 Fail;
};

struct T_ANEV_D_CH_905
{
	char strName[60];
	T_BREV_K BrevK;
	double dfck;
	double dHcop;
	T_BRPR_K BrprK;
	BOOL bRebar;
	T_ANEV_REBAR_D Rebar;
	double dTheta1;
	double dTheta2;
	double dTheta_q1; // θ' (q1:Quotes1)
	double dTheta_q2; // θ'' (q2:Quotes2)
	double dD1;
	double dD1_q;
	int    nN1;  // 891 부터 사용 안함 aSax 로 대체
	double dSax; // 891 부터 사용 안함 aSax 로 대체 
	CArray<double, double> aSax;
	double dD2;
	double dD2_q;
	int    nN2;  // 891 부터 사용 안함 aSay 로 대체 
	double dSay; // 891 부터 사용 안함 aSay 로 대체 
	CArray<double, double> aSay;
	T_ANEV_FAIL_D_925 Fail;
	BOOL   bSuppType;
	T_ANEV_REG_SUPP_TYPE_D SuppType;	
};

struct T_ANEV_D_CH_925
{
	char strName[60];
	char strDescription[120];
	T_BREV_K BrevK;
	double dfck;
	double dHcop;
	T_BRPR_K BrprK;
	BOOL bRebar;
	T_ANEV_REBAR_D Rebar;
	double dTheta1;
	double dTheta2;
	double dTheta_q1; // θ' (q1:Quotes1)
	double dTheta_q2; // θ'' (q2:Quotes2)
	double dD1;
	double dD1_q;
	int    nN1;  // 891 부터 사용 안함 aSax 로 대체
	double dSax; // 891 부터 사용 안함 aSax 로 대체 
	CArray<double, double> aSax;
	double dD2;
	double dD2_q;
	int    nN2;  // 891 부터 사용 안함 aSay 로 대체 
	double dSay; // 891 부터 사용 안함 aSay 로 대체 
	CArray<double, double> aSay;
	T_ANEV_FAIL_D_925 Fail;
	BOOL   bSuppType;
	T_ANEV_REG_SUPP_TYPE_D SuppType;
	BOOL   bRelAngle;           // 900  
	double dRelAngle;           // 910
	T_ANEV_BRPR_D BearingProp;	// 910
	BOOL bTensChk;				// 910
	T_ANEV_TENS_CHK_D TensChk;	// 910
};

struct T_ANEV_D_CH
{
	char strName[60];
	char strDescription[120];
	T_BREV_K BrevK;
	double dfck;
	double dHcop;
	T_BRPR_K BrprK;
	BOOL bRebar;
	T_ANEV_REBAR_D Rebar;
	double dTheta1;
	double dTheta2;
	double dTheta_q1; // θ' (q1:Quotes1)
	double dTheta_q2; // θ'' (q2:Quotes2)
	double dD1;
	double dD1_q;
	int    nN1;  // 891 부터 사용 안함 aSax 로 대체
	double dSax; // 891 부터 사용 안함 aSax 로 대체 
	CArray<double, double> aSax;
	double dD2;
	double dD2_q;
	int    nN2;  // 891 부터 사용 안함 aSay 로 대체 
	double dSay; // 891 부터 사용 안함 aSay 로 대체 
	CArray<double, double> aSay;
	T_ANEV_FAIL_D Fail;
	BOOL   bSuppType;
	T_ANEV_REG_SUPP_TYPE_D SuppType;
	BOOL   bRelAngle;           // 900  
	double dRelAngle;           // 910
	T_ANEV_BRPR_D BearingProp;	// 910
	BOOL bTensChk;				// 910
	T_ANEV_TENS_CHK_D TensChk;	// 910
	double dB;                  // 930
	double dH;                  // 930   
	double dCopingAngle;        // 930   
	CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D> aLayer; // 930

	BOOL bShearFactor;          // v950
	int  nShearFactorType;      // v950
	BOOL bPryout;               // v950
	BOOL bPryoutRow;            // v950

	void ConvertToString(T_ANEV_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDescription, rData.strDescription, sizeof(strDescription));	
		rData.BrevK = BrevK;
		rData.dfck = dfck;
		rData.dHcop = dHcop;
		rData.BrprK = BrprK;
		rData.bRebar = bRebar;
		rData.Rebar = Rebar;
		rData.dTheta1 = dTheta1;
		rData.dTheta2 = dTheta2;
		rData.dTheta_q1 = dTheta_q1;
		rData.dTheta_q2 = dTheta_q2;
		rData.dD1 = dD1;
		rData.dD1_q = dD1_q;
		rData.nN1 = nN1;
		rData.dSax = dSax;
		rData.aSax.Copy(aSax);
		rData.dD2 = dD2;
		rData.dD2_q = dD2_q;
		rData.nN2 = nN2;
		rData.dSay = dSay;
		rData.aSay.Copy(aSay);
		rData.Fail = Fail;
		rData.bSuppType = bSuppType;
		rData.SuppType = SuppType;
		rData.bRelAngle = bRelAngle;
		rData.dRelAngle = dRelAngle;
		rData.BearingProp = BearingProp;
		rData.bTensChk = bTensChk;
		rData.TensChk = TensChk;
		rData.dB = dB;
		rData.dH = dH;
		rData.dCopingAngle = dCopingAngle;
		rData.aLayer.Copy(aLayer);

		rData.bShearFactor = bShearFactor;        
		rData.nShearFactorType = nShearFactorType;
		rData.bPryout = bPryout;
		rData.bPryoutRow = bPryoutRow;
	}
	void ConvertToChar(T_ANEV_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDescription, strDescription, sizeof(strDescription));	
		BrevK = rData.BrevK;
		dfck = rData.dfck;
		dHcop = rData.dHcop;
		BrprK = rData.BrprK;
		bRebar = rData.bRebar;
		Rebar = rData.Rebar;
		dTheta1 = rData.dTheta1;
		dTheta2 = rData.dTheta2;
		dTheta_q1 = rData.dTheta_q1;
		dTheta_q2 = rData.dTheta_q2;
		dD1 = rData.dD1;
		dD1_q = rData.dD1_q;
		nN1 = rData.nN1;
		dSax = rData.dSax;
		aSax.Copy(rData.aSax);
		dD2 = rData.dD2;
		dD2_q = rData.dD2_q;
		nN2 = rData.nN2;
		dSay = rData.dSay;
		aSay.Copy(rData.aSay);
		Fail = rData.Fail;
		bSuppType = rData.bSuppType;
		SuppType = rData.SuppType;
		bRelAngle = rData.bRelAngle;
		dRelAngle = rData.dRelAngle;
		BearingProp = rData.BearingProp;
		bTensChk = rData.bTensChk;
		TensChk = rData.TensChk;
		dB = rData.dB;
		dH = rData.dH;
		dCopingAngle = rData.dCopingAngle;
		aLayer.Copy(rData.aLayer);

		bShearFactor = rData.bShearFactor;
		nShearFactorType = rData.nShearFactorType;
		bPryout = rData.bPryout;
		bPryoutRow = rData.bPryoutRow;
	}
};

struct T_ANEV_UDRD_D
{
	T_ANEV_K key;
	T_ANEV_D data;
};

struct T_ANEV_UDRD_D_CH_890
{
	T_ANEV_K key;
	T_ANEV_D_CH_890 data;
};

struct T_ANEV_UDRD_D_CH_905
{
	T_ANEV_K key;
	T_ANEV_D_CH_905 data;
};

struct T_ANEV_UDRD_D_CH_925
{
	T_ANEV_K key;
	T_ANEV_D_CH_925 data;
};

struct T_ANEV_UDRD_D_CH
{
	T_ANEV_K key;
	T_ANEV_D_CH data;
};

struct T_ANEV_UDRD_D_CH_RW_905
{
	T_ANEV_K key;
	char strName[60];
	T_BREV_K BrevK;
	double dfck;
	double dHcop;
	T_BRPR_K BrprK;
	BOOL bRebar;
	T_ANEV_REBAR_D Rebar;
	double dTheta1;
	double dTheta2;
	double dTheta_q1; // θ' (q1:Quotes1)
	double dTheta_q2; // θ'' (q2:Quotes2)
	double dD1;
	double dD1_q;
	int    nN1;  // 891 부터 사용 안함 aSax 로 대체
	double dSax; // 891 부터 사용 안함 aSax 로 대체 
	int nCountSax;
	double dD2;
	double dD2_q;
	int    nN2;  // 891 부터 사용 안함 aSay 로 대체
	double dSay; // 891 부터 사용 안함 aSay 로 대체 
	int nCountSay;
	T_ANEV_FAIL_D_925 Fail;
	BOOL   bSuppType;
	T_ANEV_REG_SUPP_TYPE_D_RW SuppType;
};

struct T_ANEV_UDRD_D_CH_RW_TEMP
{
	T_ANEV_K key;
	char strName[60];
	char strDescription[120];
	T_BREV_K BrevK;
	double dfck;
	double dHcop;
	T_BRPR_K BrprK;
	BOOL bRebar;
	T_ANEV_REBAR_D Rebar;
	double dTheta1;
	double dTheta2;
	double dTheta_q1; // θ' (q1:Quotes1)
	double dTheta_q2; // θ'' (q2:Quotes2)
	double dD1;
	double dD1_q;
	int    nN1;  // 891 부터 사용 안함 aSax 로 대체
	double dSax; // 891 부터 사용 안함 aSax 로 대체 
	int nCountSax;
	double dD2;
	double dD2_q;
	int    nN2;  // 891 부터 사용 안함 aSay 로 대체
	double dSay; // 891 부터 사용 안함 aSay 로 대체 
	int nCountSay;
	T_ANEV_FAIL_D_925 Fail;
	BOOL   bSuppType;
	T_ANEV_REG_SUPP_TYPE_D_RW SuppType;
//	BOOL  bRelAngle;                  // 900
	double dRelAngle;           	  // 910 
	T_ANEV_BRPR_D_RW BearingProp;	  // 910
	BOOL bTensChk;                    // 910
	T_ANEV_TENS_CHK_D_RW TensChk;     // 910
};

struct T_ANEV_UDRD_D_CH_RW_925
{
	T_ANEV_K key;
	char strName[60];
	char strDescription[120];
	T_BREV_K BrevK;
	double dfck;
	double dHcop;
	T_BRPR_K BrprK;
	BOOL bRebar;
	T_ANEV_REBAR_D Rebar;
	double dTheta1;
	double dTheta2;
	double dTheta_q1; // θ' (q1:Quotes1)
	double dTheta_q2; // θ'' (q2:Quotes2)
	double dD1;
	double dD1_q;
	int    nN1;  // 891 부터 사용 안함 aSax 로 대체
	double dSax; // 891 부터 사용 안함 aSax 로 대체 
	int nCountSax;
	double dD2;
	double dD2_q;
	int    nN2;  // 891 부터 사용 안함 aSay 로 대체
	double dSay; // 891 부터 사용 안함 aSay 로 대체 
	int nCountSay;
	T_ANEV_FAIL_D_925 Fail;
	BOOL   bSuppType;
	T_ANEV_REG_SUPP_TYPE_D_RW SuppType;
	BOOL  bRelAngle;                  // 900
	double dRelAngle;           	  // 910 
	T_ANEV_BRPR_D_RW BearingProp;	  // 910
	BOOL bTensChk;                    // 910
	T_ANEV_TENS_CHK_D_RW TensChk;     // 910
};

struct T_ANEV_UDRD_D_CH_RW_945
{
	T_ANEV_K key;
	char strName[60];
	char strDescription[120];
	T_BREV_K BrevK;
	double dfck;
	double dHcop;
	T_BRPR_K BrprK;
	BOOL bRebar;
	T_ANEV_REBAR_D Rebar;
	double dTheta1;
	double dTheta2;
	double dTheta_q1; // θ' (q1:Quotes1)
	double dTheta_q2; // θ'' (q2:Quotes2)
	double dD1;
	double dD1_q;
	int    nN1;  // 891 부터 사용 안함 aSax 로 대체
	double dSax; // 891 부터 사용 안함 aSax 로 대체 
	int nCountSax;
	double dD2;
	double dD2_q;
	int    nN2;  // 891 부터 사용 안함 aSay 로 대체
	double dSay; // 891 부터 사용 안함 aSay 로 대체 
	int nCountSay;
	T_ANEV_FAIL_D_RW Fail;
	BOOL   bSuppType;
	T_ANEV_REG_SUPP_TYPE_D_RW SuppType;
	BOOL  bRelAngle;                  // 900
	double dRelAngle;           	  // 910 
	T_ANEV_BRPR_D_RW BearingProp;	  // 910
	BOOL bTensChk;                    // 910
	T_ANEV_TENS_CHK_D_RW TensChk;     // 910
	double dB;                        // 930
	double dH;                        // 930   
	double dCopingAngle;              // 930   
	int nCountLayer;                  // 930
};

struct T_ANEV_UDRD_D_CH_RW_950_Temp
{
	T_ANEV_K key;
	char strName[60];
	char strDescription[120];
	T_BREV_K BrevK;
	double dfck;
	double dHcop;
	T_BRPR_K BrprK;
	BOOL bRebar;
	T_ANEV_REBAR_D Rebar;
	double dTheta1;
	double dTheta2;
	double dTheta_q1; // θ' (q1:Quotes1)
	double dTheta_q2; // θ'' (q2:Quotes2)
	double dD1;
	double dD1_q;
	int    nN1;  // 891 부터 사용 안함 aSax 로 대체
	double dSax; // 891 부터 사용 안함 aSax 로 대체 
	int nCountSax;
	double dD2;
	double dD2_q;
	int    nN2;  // 891 부터 사용 안함 aSay 로 대체
	double dSay; // 891 부터 사용 안함 aSay 로 대체 
	int nCountSay;
	T_ANEV_FAIL_D_RW Fail;
	BOOL   bSuppType;
	T_ANEV_REG_SUPP_TYPE_D_RW SuppType;
	BOOL  bRelAngle;                  // 900
	double dRelAngle;           	  // 910 
	T_ANEV_BRPR_D_RW BearingProp;	  // 910
	BOOL bTensChk;                    // 910
	T_ANEV_TENS_CHK_D_RW TensChk;     // 910
	double dB;                        // 930
	double dH;                        // 930   
	double dCopingAngle;              // 930   
	int nCountLayer;                  // 930

	BOOL bShearFactor;                // 950
	int nShearFactorType;             // 950 
};

struct T_ANEV_UDRD_D_CH_RW_950_Temp2
{
	T_ANEV_K key;
	char strName[60];
	char strDescription[120];
	T_BREV_K BrevK;
	double dfck;
	double dHcop;
	T_BRPR_K BrprK;
	BOOL bRebar;
	T_ANEV_REBAR_D Rebar;
	double dTheta1;
	double dTheta2;
	double dTheta_q1; // θ' (q1:Quotes1)
	double dTheta_q2; // θ'' (q2:Quotes2)
	double dD1;
	double dD1_q;
	int    nN1;  // 891 부터 사용 안함 aSax 로 대체
	double dSax; // 891 부터 사용 안함 aSax 로 대체 
	int nCountSax;
	double dD2;
	double dD2_q;
	int    nN2;  // 891 부터 사용 안함 aSay 로 대체
	double dSay; // 891 부터 사용 안함 aSay 로 대체 
	int nCountSay;
	T_ANEV_FAIL_D_RW Fail;
	BOOL   bSuppType;
	T_ANEV_REG_SUPP_TYPE_D_RW SuppType;
	BOOL  bRelAngle;                  // 900
	double dRelAngle;           	  // 910 
	T_ANEV_BRPR_D_RW BearingProp;	  // 910
	BOOL bTensChk;                    // 910
	T_ANEV_TENS_CHK_D_RW TensChk;     // 910
	double dB;                        // 930
	double dH;                        // 930   
	double dCopingAngle;              // 930   
	int nCountLayer;                  // 930

	BOOL bShearFactor;                // 950
	int nShearFactorType;             // 950 
	BOOL bPryout;                     // 950
};

struct T_ANEV_UDRD_D_CH_RW
{
	T_ANEV_K key;
	char strName[60];
	char strDescription[120];
	T_BREV_K BrevK;
	double dfck;
	double dHcop;
	T_BRPR_K BrprK;
	BOOL bRebar;
	T_ANEV_REBAR_D Rebar;
	double dTheta1;
	double dTheta2;
	double dTheta_q1; // θ' (q1:Quotes1)
	double dTheta_q2; // θ'' (q2:Quotes2)
	double dD1;
	double dD1_q;
	int    nN1;  // 891 부터 사용 안함 aSax 로 대체
	double dSax; // 891 부터 사용 안함 aSax 로 대체 
	int nCountSax;
	double dD2;
	double dD2_q;
	int    nN2;  // 891 부터 사용 안함 aSay 로 대체
	double dSay; // 891 부터 사용 안함 aSay 로 대체 
	int nCountSay;
	T_ANEV_FAIL_D_RW Fail;
	BOOL   bSuppType;
	T_ANEV_REG_SUPP_TYPE_D_RW SuppType;	
	BOOL  bRelAngle;                  // 900
	double dRelAngle;           	  // 910 
	T_ANEV_BRPR_D_RW BearingProp;	  // 910
	BOOL bTensChk;                    // 910
	T_ANEV_TENS_CHK_D_RW TensChk;     // 910
	double dB;                        // 930
	double dH;                        // 930   
	double dCopingAngle;              // 930   
	int nCountLayer;                  // 930

	BOOL bShearFactor;                // 950
	int nShearFactorType;             // 950 
	BOOL bPryout;                     // 950
	BOOL bPryoutRow;                  // 950
	
	void GetAnev(T_ANEV_K& rKey, T_ANEV_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strDescription, strDescription, sizeof(strDescription));		
		rData.BrevK = BrevK;
		rData.dfck = dfck;
		rData.dHcop = dHcop;
		rData.BrprK = BrprK;
		rData.bRebar = bRebar;
		rData.Rebar = Rebar;
		rData.dTheta1 = dTheta1;
		rData.dTheta2 = dTheta2;
		rData.dTheta_q1 = dTheta_q1;
		rData.dTheta_q2 = dTheta_q2;
		rData.dD1 = dD1;
		rData.dD1_q = dD1_q;
		rData.nN1 = nN1;
		rData.dSax = dSax;
		// nCountSax
		rData.dD2 = dD2;
		rData.dD2_q = dD2_q;
		rData.nN2 = nN2;
		rData.dSay = dSay;
		// nCountSay
		Fail.Get(rData.Fail);
		rData.bSuppType = bSuppType;
		// SuppType.nCountLayer
		rData.bRelAngle = bRelAngle;
		rData.dRelAngle = dRelAngle;
		//BearingProp.nCountBrprKs;	
		rData.bTensChk = bTensChk;
		//TensChk.nCountElnkNodeKs ;
		rData.dB = dB;
		rData.dH = dH;
		rData.dCopingAngle = dCopingAngle;
		// nCountLayer
		rData.bShearFactor = bShearFactor;        
		rData.nShearFactorType = nShearFactorType;
		rData.bPryout = bPryout;
		rData.bPryoutRow = bPryoutRow;
	}
	void SetAnev(const T_ANEV_K Key, const T_ANEV_D_CH& rData)
	{
		key = Key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));		
		BrevK = rData.BrevK;
		dfck = rData.dfck;
		dHcop = rData.dHcop;
		BrprK = rData.BrprK;
		bRebar = rData.bRebar;
		Rebar = rData.Rebar;
		dTheta1 = rData.dTheta1;
		dTheta2 = rData.dTheta2;
		dTheta_q1 = rData.dTheta_q1;
		dTheta_q2 = rData.dTheta_q2;
		dD1 = rData.dD1;
		dD1_q = rData.dD1_q;
		nN1 = rData.nN1;
		dSax = rData.dSax;
		nCountSax = (int)rData.aSax.GetSize();
		dD2 = rData.dD2;
		dD2_q = rData.dD2_q;
		nN2 = rData.nN2;
		dSay = rData.dSay;
		nCountSay = (int)rData.aSay.GetSize();
		Fail.Set(rData.Fail);
		bSuppType = rData.bSuppType;
		SuppType.Set(rData.SuppType);
		bRelAngle = rData.bRelAngle;
		dRelAngle = rData.dRelAngle;
		BearingProp.Set(rData.BearingProp);
		bTensChk = rData.bTensChk;
		TensChk.Set(rData.TensChk);
		dB = rData.dB;
		dH = rData.dH;
		dCopingAngle = rData.dCopingAngle;
		nCountLayer = (int)rData.aLayer.GetSize();

		bShearFactor     = rData.bShearFactor;
		nShearFactorType = rData.nShearFactorType;
		bPryout          = rData.bPryout;
		bPryoutRow       = rData.bPryoutRow;
	}
	void Convert950_Temp2(T_ANEV_UDRD_D_CH_RW_950_Temp2& rData)
	{
		key = rData.key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		BrevK = rData.BrevK;
		dfck = rData.dfck;
		dHcop = rData.dHcop;
		BrprK = rData.BrprK;
		bRebar = rData.bRebar;
		Rebar = rData.Rebar;
		dTheta1 = rData.dTheta1;
		dTheta2 = rData.dTheta2;
		dTheta_q1 = rData.dTheta_q1;
		dTheta_q2 = rData.dTheta_q2;
		dD1 = rData.dD1;
		dD1_q = rData.dD1_q;
		nN1 = rData.nN1;
		dSax = rData.dSax;
		nCountSax = rData.nCountSax;
		dD2 = rData.dD2;
		dD2_q = rData.dD2_q;
		nN2 = rData.nN2;
		dSay = rData.dSay;
		nCountSay = rData.nCountSay;
		Fail = rData.Fail;
		bSuppType = rData.bSuppType;
		SuppType = rData.SuppType;
		bRelAngle = rData.bRelAngle;
		dRelAngle = rData.dRelAngle;
		BearingProp = rData.BearingProp;
		bTensChk = rData.bTensChk;
		TensChk = rData.TensChk;
		dB = rData.dB;
		dH = rData.dH;
		dCopingAngle = rData.dCopingAngle;
		nCountLayer = rData.nCountLayer;

		bShearFactor = rData.bShearFactor;
		nShearFactorType = rData.nShearFactorType;
		bPryout = rData.bPryout;
		bPryoutRow = FALSE;
	}
	void Convert950_Temp(T_ANEV_UDRD_D_CH_RW_950_Temp& rData)
	{
		key = rData.key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		BrevK = rData.BrevK;
		dfck = rData.dfck;
		dHcop = rData.dHcop;
		BrprK = rData.BrprK;
		bRebar = rData.bRebar;
		Rebar = rData.Rebar;
		dTheta1 = rData.dTheta1;
		dTheta2 = rData.dTheta2;
		dTheta_q1 = rData.dTheta_q1;
		dTheta_q2 = rData.dTheta_q2;
		dD1 = rData.dD1;
		dD1_q = rData.dD1_q;
		nN1 = rData.nN1;
		dSax = rData.dSax;
		nCountSax = rData.nCountSax;
		dD2 = rData.dD2;
		dD2_q = rData.dD2_q;
		nN2 = rData.nN2;
		dSay = rData.dSay;
		nCountSay = rData.nCountSay;
		Fail = rData.Fail;
		bSuppType = rData.bSuppType;
		SuppType = rData.SuppType;
		bRelAngle = rData.bRelAngle;
		dRelAngle = rData.dRelAngle;
		BearingProp = rData.BearingProp;
		bTensChk = rData.bTensChk;
		TensChk = rData.TensChk;
		dB = rData.dB;
		dH = rData.dH;
		dCopingAngle = rData.dCopingAngle;
		nCountLayer = rData.nCountLayer;

		bShearFactor = rData.bShearFactor;
		nShearFactorType = rData.nShearFactorType;
		bPryout = TRUE;
		bPryoutRow = FALSE;
	}
	void Convert945(T_ANEV_UDRD_D_CH_RW_945& rData)
	{
		key = rData.key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		BrevK = rData.BrevK;
		dfck = rData.dfck;
		dHcop = rData.dHcop;
		BrprK = rData.BrprK;
		bRebar = rData.bRebar;
		Rebar = rData.Rebar;
		dTheta1 = rData.dTheta1;
		dTheta2 = rData.dTheta2;
		dTheta_q1 = rData.dTheta_q1;
		dTheta_q2 = rData.dTheta_q2;
		dD1 = rData.dD1;
		dD1_q = rData.dD1_q;
		nN1 = rData.nN1;
		dSax = rData.dSax;
		nCountSax = rData.nCountSax;
		dD2 = rData.dD2;
		dD2_q = rData.dD2_q;
		nN2 = rData.nN2;
		dSay = rData.dSay;
		nCountSay = rData.nCountSay;
		Fail = rData.Fail;
		bSuppType = rData.bSuppType;
		SuppType = rData.SuppType;
		bRelAngle = rData.bRelAngle;
		dRelAngle = rData.dRelAngle;
		BearingProp = rData.BearingProp;
		bTensChk = rData.bTensChk;
		TensChk = rData.TensChk;
		dB = rData.dB;
		dH = rData.dH;
		dCopingAngle = rData.dCopingAngle;
		nCountLayer = rData.nCountLayer;

		bShearFactor = FALSE;
		nShearFactorType = 0;
		bPryout = TRUE;
		bPryoutRow = FALSE;
	}
	void Convert925(T_ANEV_UDRD_D_CH_RW_925& rData) // 삭제 예정
	{
		key = rData.key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		BrevK = rData.BrevK;
		dfck = rData.dfck;
		dHcop = rData.dHcop;
		BrprK = rData.BrprK;
		bRebar = rData.bRebar;
		Rebar = rData.Rebar;
		dTheta1 = rData.dTheta1;
		dTheta2 = rData.dTheta2;
		dTheta_q1 = rData.dTheta_q1;
		dTheta_q2 = rData.dTheta_q2;
		dD1 = rData.dD1;
		dD1_q = rData.dD1_q;
		nN1 = rData.nN1;
		dSax = rData.dSax;
		nCountSax = rData.nCountSax;
		dD2 = rData.dD2;
		dD2_q = rData.dD2_q;
		nN2 = rData.nN2;
		dSay = rData.dSay;
		nCountSay = rData.nCountSay;
		Fail.Convert925(rData.Fail);
		bSuppType = rData.bSuppType;
		SuppType.nCountLayer = rData.SuppType.nCountLayer;
		bRelAngle = rData.bRelAngle;
		dRelAngle = rData.dRelAngle;
		BearingProp.nCountBrprKs = rData.BearingProp.nCountBrprKs;
		bTensChk = rData.bTensChk;
		TensChk.nCountElnkNodeKs = rData.TensChk.nCountElnkNodeKs;
		dB = 0.0;
		dH = 0.0;
		dCopingAngle = 0.0;
		nCountLayer = 0;
		bShearFactor = FALSE;
		nShearFactorType = 0;
		bPryout = TRUE;
		bPryoutRow = FALSE;
	}
	void ConvertTEMP(T_ANEV_UDRD_D_CH_RW_TEMP& rData) // 삭제 예정
	{
		key = rData.key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		BrevK = rData.BrevK;
		dfck = rData.dfck;
		dHcop = rData.dHcop;
		BrprK = rData.BrprK;
		bRebar = rData.bRebar;
		Rebar = rData.Rebar;
		dTheta1 = rData.dTheta1;
		dTheta2 = rData.dTheta2;
		dTheta_q1 = rData.dTheta_q1;
		dTheta_q2 = rData.dTheta_q2;
		dD1 = rData.dD1;
		dD1_q = rData.dD1_q;
		nN1 = rData.nN1;
		dSax = rData.dSax;
		nCountSax = rData.nCountSax; 
		dD2 = rData.dD2;
		dD2_q = rData.dD2_q;
		nN2 = rData.nN2;
		dSay = rData.dSay;
		nCountSay = rData.nCountSay; 
		Fail.Convert925(rData.Fail);
		bSuppType = rData.bSuppType;
		SuppType.nCountLayer = rData.SuppType.nCountLayer; 
		bRelAngle = FALSE;
		dRelAngle = rData.dRelAngle;
		BearingProp.nCountBrprKs = rData.BearingProp.nCountBrprKs; 
		bTensChk = rData.bTensChk;
		TensChk.nCountElnkNodeKs = rData.TensChk.nCountElnkNodeKs;
		dB = 0.0;
		dH = 0.0;
		dCopingAngle = 0.0;
		nCountLayer = 0;
		bShearFactor = FALSE;
		nShearFactorType = 0;
		bPryout = TRUE;
		bPryoutRow = FALSE;
	}
	void Convert905(T_ANEV_UDRD_D_CH_RW_905& rData)
	{
		key = rData.key;
		memcpy(strName, rData.strName, sizeof(strName));
		BrevK = rData.BrevK;
		dfck = rData.dfck;
		dHcop = rData.dHcop;
		BrprK = rData.BrprK;
		bRebar = rData.bRebar;
		Rebar = rData.Rebar;
		dTheta1 = rData.dTheta1;
		dTheta2 = rData.dTheta2;
		dTheta_q1 = rData.dTheta_q1;
		dTheta_q2 = rData.dTheta_q2;
		dD1 = rData.dD1;
		dD1_q = rData.dD1_q;
		nN1 = rData.nN1;
		dSax = rData.dSax;
		nCountSax = rData.nCountSax; // 나중에 Convert 
		dD2 = rData.dD2;
		dD2_q = rData.dD2_q;
		nN2 = rData.nN2;
		dSay = rData.dSay;
		nCountSay = rData.nCountSay; // 나중에 Convert 
		Fail.Convert925(rData.Fail);
		bSuppType = rData.bSuppType;
		SuppType.nCountLayer = rData.SuppType.nCountLayer; // 나중에 Convert 
		bRelAngle = TRUE;
		dRelAngle = 0.0;
		BearingProp.nCountBrprKs = 0; // 나중에 Convert
		bTensChk = FALSE;
		TensChk.nCountElnkNodeKs = 0; // 나중에 Convert
		dB = 0.0;
		dH = 0.0;
		dCopingAngle = 0.0;
		nCountLayer = 0;
		bShearFactor = FALSE;
		nShearFactorType = 0;
		bPryout = TRUE;
		bPryoutRow = FALSE;
	}
	void Convert890(T_ANEV_K Key, T_ANEV_D_CH_890& rData) // 890 이하 버전에서는 배열이 없었음
	{
		key = Key;
		memcpy(strName, rData.strName, sizeof(strName));
		BrevK = rData.BrevK;
		dfck = rData.dfck;
		dHcop = rData.dHcop;
		BrprK = rData.BrprK;
		bRebar = rData.bRebar;
		Rebar = rData.Rebar;
		dTheta1 = rData.dTheta1;
		dTheta2 = rData.dTheta2;
		dTheta_q1 = rData.dTheta_q1;
		dTheta_q2 = rData.dTheta_q2;
		dD1 = rData.dD1;
		dD1_q = rData.dD1_q;
		nN1 = rData.nN1;
		dSax = rData.dSax;
		nCountSax = 0; // 나중에 Convert 
		dD2 = rData.dD2;
		dD2_q = rData.dD2_q;
		nN2 = rData.nN2;
		dSay = rData.dSay;
		nCountSay = 0; // 나중에 Convert 
		Fail.Convert925(rData.Fail);
		bSuppType = FALSE;
		SuppType.nCountLayer = 0;  // 나중에 Convert 
		bRelAngle = TRUE;
		dRelAngle = 0.0;
		BearingProp.nCountBrprKs = 0; // 나중에 Convert
		bTensChk = FALSE;
		TensChk.nCountElnkNodeKs = 0; // 나중에 Convert
		dB = 0.0;
		dH = 0.0;
		dCopingAngle = 0.0;
		nCountLayer = 0; // 나중에 Convert 
		bShearFactor = FALSE;
		nShearFactorType = 0;
		bPryout = TRUE;
		bPryoutRow = FALSE;
	}
};

struct T_ANEV_UNIT
{
	T_ANEV_UNIT()
	{
		dfck = D_UNITSYS_BASE_STRESS;
		dHcop = D_UNITSYS_BASE_LENGTH;
		dTheta1 = D_UNITSYS_BASE_DEGREE;
		dTheta2 = D_UNITSYS_BASE_DEGREE;
		dTheta_q1 = D_UNITSYS_BASE_DEGREE;
		dTheta_q2 = D_UNITSYS_BASE_DEGREE;
		dD1 = D_UNITSYS_BASE_LENGTH;
		dD1_q = D_UNITSYS_BASE_LENGTH;
		dSax = D_UNITSYS_BASE_LENGTH;
		dD2 = D_UNITSYS_BASE_LENGTH;
		dD2_q = D_UNITSYS_BASE_LENGTH;
		dSay = D_UNITSYS_BASE_LENGTH;
		dFyat = D_UNITSYS_BASE_STRESS;
		ddsat = D_UNITSYS_BASE_LENGTH;
		dFyasL = D_UNITSYS_BASE_STRESS;
		ddsasL = D_UNITSYS_BASE_LENGTH;		
		dFyasT = D_UNITSYS_BASE_STRESS;
		ddsasT = D_UNITSYS_BASE_LENGTH;
		dAVc = D_UNITSYS_BASE_AREA;
		dAVco = D_UNITSYS_BASE_AREA;
		dHa = D_UNITSYS_BASE_LENGTH;
		dCa1 = D_UNITSYS_BASE_LENGTH;
		dCa1_q = D_UNITSYS_BASE_LENGTH;
		dCa2 = D_UNITSYS_BASE_LENGTH;
		dCa2_q = D_UNITSYS_BASE_LENGTH;
		dANc = D_UNITSYS_BASE_AREA;
		dANco = D_UNITSYS_BASE_AREA;
		dHef = D_UNITSYS_BASE_LENGTH;
		dRelAngle = D_UNITSYS_BASE_DEGREE;
		dB = D_UNITSYS_BASE_LENGTH;
		dH = D_UNITSYS_BASE_LENGTH;
		dCopingAngle = D_UNITSYS_BASE_DEGREE;
	}
	int dfck;
	int dHcop;
	int dTheta1;
	int dTheta2;
	int dTheta_q1;
	int dTheta_q2;
	int dD1;
	int dD1_q;
	int dSax;
	int dD2;
	int dD2_q;
	int dSay;
	int dFyat;
	int ddsat;	
	int dFyasL;
	int ddsasL;
	int dFyasT;
	int ddsasT;
	int dAVc;
	int dAVco;
	int dHa;
	int dCa1;
	int dCa1_q;
	int dCa2;
	int dCa2_q;
	int dANc;
	int dANco;
	int dHef;
	int dRelAngle;
	int dB;
	int dH;
	int dCopingAngle;
};
#define HASHSIZEANEV 1001

// Support Length
#define T_SUPL_K UINT
struct T_SUPL_D
{
	CString strName;
	T_ANEV_K AnevK;
	double dNc;
	double dL;
	double dH;
	double dTheta;

	T_SUPL_D()  { }
	T_SUPL_D(const T_SUPL_D& src) {*this = src;}
	T_SUPL_D& operator=(const T_SUPL_D& src)
	{
		strName = src.strName;
		AnevK = src.AnevK;
		dNc = src.dNc;
		dL = src.dL;
		dH = src.dH;
		dTheta = src.dTheta;
		return *this;
	}  

	void Initialize()
	{
		strName.Empty();
		AnevK = 0;
		dNc = 0.0;
		dL = 0.0;
		dH = 0.0;
		dTheta = 0.0;
	}	
};

struct T_SUPL_D_CH
{
	char strName[60];
	T_ANEV_K AnevK;
	double dNc;
	double dL;
	double dH;
	double dTheta;

	void ConvertToString(T_SUPL_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.AnevK = AnevK;
		rData.dNc = dNc;
		rData.dL = dL;
		rData.dH = dH;
		rData.dTheta = dTheta;
	}
	void ConvertToChar(T_SUPL_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		AnevK = rData.AnevK;
		dNc = rData.dNc;
		dL = rData.dL;
		dH = rData.dH;
		dTheta = rData.dTheta;
	}
};

struct T_SUPL_UDRD_D
{
	T_SUPL_K key;
	T_SUPL_D data;
};

struct T_SUPL_UDRD_D_CH
{
	T_SUPL_K key;
	T_SUPL_D_CH data;
};

struct T_SUPL_UNIT
{
	T_SUPL_UNIT()
	{
		dNc = D_UNITSYS_BASE_LENGTH;
		dL = D_UNITSYS_BASE_LENGTH;
		dH = D_UNITSYS_BASE_LENGTH;
		dTheta = D_UNITSYS_BASE_DEGREE;
	}
	int dNc;
	int dL;
	int dH;
	int dTheta;
};
#define HASHSIZESUPL 1001

#pragma pack(pop)

#endif  // __DB_ST_DT_SEIS_CVL_H__
