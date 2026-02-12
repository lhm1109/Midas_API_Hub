#ifndef __DB_ST_DN_JUD_H__
#define __DB_ST_DN_JUD_H__

#define MIDAS_SDK

#include "DB_ST_DT.h"
#include "DB_ST_DT_UNIT.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)

#define T_LLCD_K unsigned int
struct T_LLCD_D // 수평하중의 방향 지정
{ 
	int nDirType; // 0=지정안함, 1=X, 2=Y
	void Initialize()
	{    
		nDirType = 0;
	}
	
	T_LLCD_D() { Initialize(); };
	T_LLCD_D(T_LLCD_D& src) { *this = src; }
	T_LLCD_D& operator=(const T_LLCD_D& src)
	{ 
		nDirType = src.nDirType;
		return *this;
	}	
};
struct T_LLCD_UDRD_D
{
	T_LLCD_K key;
	T_LLCD_D data;
};
#define HASHSIZELLCD HASHSIZESTLD

#define T_SLBS_K unsigned int
struct T_SLBS_D // 슬래브에 의한 보 강성 할증
{ 
	CString  strName     ;
	T_MATL_K nMatlKey    ;
	T_THIK_K nThikKey    ;
	int      nInputMethod;               
	int      nCalcMethod ;               
	double   dOneSide    ;
	double   dBothSide   ;  
	CArray<T_NODE_K,T_NODE_K> arKeyNode; // 절점 리스트    
	CArray<T_ELEM_K,T_ELEM_K> arKeyElem; // 부재 리스트  
	void Initialize()
	{
		strName      = "" ;
		nMatlKey     = 0  ;
		nThikKey     = 0  ;    
		nInputMethod = 0  ;    // 입력방법 : 0=영역, 1=직접지정-편측, 2-직접지정-양측          
		nCalcMethod  = 1  ;    // 슬래브 증대율 : 0=자동, 1=직접입력           
		dOneSide     = 1.5;    // 편측 증대율
		dBothSide    = 2.0;    // 양측 증대율
		arKeyNode.RemoveAll(); // 절점 리스트    
		arKeyElem.RemoveAll(); // 부재 리스트  
	}  
	T_SLBS_D() { Initialize(); };
	T_SLBS_D(T_SLBS_D& src) { *this = src; }
	T_SLBS_D& operator=(const T_SLBS_D& src)
	{ 
		strName      = src.strName     ;
		nMatlKey     = src.nMatlKey    ;
		nThikKey     = src.nThikKey    ;    
		nInputMethod = src.nInputMethod;    // 입력방법 : 0=영역, 1=직접지정-편측, 2-직접지정-양측          
		nCalcMethod  = src.nCalcMethod ;    // 슬래브 증대율 : 0=자동, 1=직접입력           
		dOneSide     = src.dOneSide    ;    // 편측 증대율
		dBothSide    = src.dBothSide   ;    // 양측 증대율
		arKeyNode.Copy(src.arKeyNode); 
		arKeyElem.Copy(src.arKeyElem); 
		return *this;
	}	
};
struct T_SLBS_D_CH
{
	char     chName[80]  ;
	T_MATL_K nMatlKey    ;
	T_THIK_K nThikKey    ;
	int      nInputMethod;               
	int      nCalcMethod ;               
	double   dOneSide    ;
	double   dBothSide   ;  
	CArray<T_NODE_K,T_NODE_K> arKeyNode;     
	CArray<T_ELEM_K,T_ELEM_K> arKeyElem; 
	
	void ConvertToString(T_SLBS_D& rData)
	{
		ConvertCharStr(chName, rData.strName, sizeof(chName));
		rData.nMatlKey     = nMatlKey    ; 
		rData.nThikKey     = nThikKey    ; 
		rData.nInputMethod = nInputMethod; 
		rData.nCalcMethod  = nCalcMethod ;       
		rData.dOneSide     = dOneSide    ; 
		rData.dBothSide    = dBothSide   ;   
		rData.arKeyNode.Copy(arKeyNode)  ;
		rData.arKeyElem.Copy(arKeyElem)  ;
	}
	void ConvertToChar(T_SLBS_D& rData)
	{
		ConvertStrChar(rData.strName, chName, sizeof(chName));
		nMatlKey     = rData.nMatlKey    ; 
		nThikKey     = rData.nThikKey    ; 
		nInputMethod = rData.nInputMethod; 
		nCalcMethod  = rData.nCalcMethod ;       
		dOneSide     = rData.dOneSide    ; 
		dBothSide    = rData.dBothSide   ;   
		arKeyNode.Copy(rData.arKeyNode)  ;
		arKeyElem.Copy(rData.arKeyElem)  ;
	}
};
struct T_SLBS_UDRD_D
{
	T_SLBS_K key;
	T_SLBS_D data;
};
struct T_SLBS_ITEM_UDRD_D
{
	T_SLBS_K key;
	T_NODE_K nodekey;
	T_ELEM_K elemkey;
};
struct T_SLBS_UDRD_D_CH
{
	T_SLBS_K key;
	T_SLBS_D_CH data;
};
struct T_SLBS_UDRD_D_CH_RW
{ 
	T_SLBS_K key;
	char     chName[80]   ; 
	T_MATL_K nMatlKey     ;
	T_THIK_K nThikKey     ;
	int      nInputMethod ;               
	int      nCalcMethod  ;               
	double   dOneSide     ;
	double   dBothSide    ;  
	int      nCountNodeKey;
	int      nCountElemKey;
	void GetSlbs(T_SLBS_K& rKey, T_SLBS_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.chName, chName, sizeof(chName));
		rData.nMatlKey     = nMatlKey    ; 
		rData.nThikKey     = nThikKey    ; 
		rData.nInputMethod = nInputMethod; 
		rData.nCalcMethod  = nCalcMethod ;       
		rData.dOneSide     = dOneSide    ; 
		rData.dBothSide    = dBothSide   ;  
	}
	void SetSlbs(const T_SLBS_K Key, const T_SLBS_D_CH& rData)
	{
		key=Key;
		memcpy(chName, rData.chName, sizeof(chName));
		nMatlKey      = rData.nMatlKey    ; 
		nThikKey      = rData.nThikKey    ; 
		nInputMethod  = rData.nInputMethod; 
		nCalcMethod   = rData.nCalcMethod ;       
		dOneSide      = rData.dOneSide    ; 
		dBothSide     = rData.dBothSide   ;     
		nCountNodeKey = (int)rData.arKeyNode.GetSize();
		nCountElemKey = (int)rData.arKeyElem.GetSize();
	}
};
#define HASHSIZESLBS 101
#define HASHSIZESLBSITEM HASHSIZEELEM

#define T_IRST_K unsigned int  
struct T_IRST_D // 비정형 스토리 Irregular Story
{ 
	T_GRUP_K nReportGrupK; // 계산서 출력용 층 그룹 Key : Group의 nPlaneType=3 인 것만 가능 (X-Y 평면)
	T_GRUP_K nEccenGrupK ; // 편심율 계산용 층 그룹 Key : Group의 nPlaneType=3 인 것만 가능 (X-Y 평면)

	int      nReportType; // 계산서출력용         : 0=층 따라감, 1=출력안함            , 2=층 그룹 사용
	int      nEccenType ; // 강성/편심율 계산용   : 0=층 따라감, 1=계산서 출력용 따라감, 2=층 그룹 사용
	double   dLevel     ; // 강성/편심율용 높이   : nReportType이 2이고 nEccenType이 1일 경우 
												//                        또는 nEccenType이 2일 경우 사용
	void Initialize()
	{
		nReportGrupK = 0 ; 
		nEccenGrupK  = 0 ; 
		nReportType  = 0 ; 
		nEccenType   = 1 ; 
		dLevel       = 0.; 
	}  
	T_IRST_D() { Initialize(); };
	T_IRST_D(T_IRST_D& src) { *this = src; }
	T_IRST_D& operator=(const T_IRST_D& src)
	{ 
		nReportGrupK = src.nReportGrupK; 
		nEccenGrupK  = src.nEccenGrupK; 
		nReportType  = src.nReportType; 
		nEccenType   = src.nEccenType ; 
		dLevel       = src.dLevel     ; 
		return *this;
	}	
};
struct T_IRST_UDRD_D
{
	T_IRST_K key;
	T_IRST_D data;
};
struct T_IRST_UNIT
{
	T_IRST_UNIT()
	{
		dLevel = D_UNITSYS_BASE_LENGTH;    
	}
	int dLevel;  
};
#define HASHSIZEIRST 101

#define T_BLCF_K unsigned int
struct T_BLCF_D // 보 횡보강지정
{  
	UINT nGroupId        ;               // ID
	BOOL bSlabRestriction;               // 슬래브 구속 여부
	BOOL bLateralConf    ;               // 횡보강 입력
	int  nSlab           ;               // 0=구속, 1=비구속
	int  nLateral        ;               // 0=등간격, 1=간격직접입력
	int  nNumSpace       ;               // nLateral=0일 경우, 등간격 갯수
	CArray<T_ELEM_K,T_ELEM_K> arKeyElem; // 부재 리스트  
	CArray<double,double> aInputSpace;   // nLateral=1일 경우, 간격 List  
	void Initialize()
	{
		nGroupId         = 0;
		bSlabRestriction = TRUE;            
		bLateralConf     = TRUE;            
		nSlab            = 0;            
		nLateral         = 0;   
		nNumSpace        = 2;    
		arKeyElem.RemoveAll();     
		aInputSpace.RemoveAll();
	}  
	T_BLCF_D() { Initialize(); };
	T_BLCF_D(T_BLCF_D& src) { *this = src; }
	T_BLCF_D& operator=(const T_BLCF_D& src)
	{ 
		nGroupId         = src.nGroupId;
		bSlabRestriction = src.bSlabRestriction;            
		bLateralConf     = src.bLateralConf;            
		nSlab            = src.nSlab;            
		nLateral         = src.nLateral;   
		nNumSpace        = src.nNumSpace;    
		arKeyElem.Copy(src.arKeyElem);    
		aInputSpace.Copy(src.aInputSpace);
		return *this;
	}	
};
struct T_BLCF_UDRD_D
{
	T_BLCF_K key;
	T_BLCF_D data;
};
struct T_BLCF_ITEM_UDRD_D
{
	T_BLCF_K key;
	T_ELEM_K elemkey;
};
struct T_BLCF_UDRD_D_RW
{ 
	T_BLCF_K key;
	UINT nGroupId        ;
	BOOL bSlabRestriction;             
	BOOL bLateralConf    ;             
	int  nSlab           ;             
	int  nLateral        ;             
	int  nNumSpace       ;           
	char dummyArray_32bit_1[20];   // 하위 버전 호환용 size dummy 크기. CArray<T_ELEM_K,T_ELEM_K> arKeyElem;    
	char dummyArray_32bit_2[20];   // 하위 버전 호환용 size dummy 크기. CArray<double,double> aInputSpace; 
	int nNumElem;
	int nNumInputSpace;
	void GetBlcf(T_BLCF_K& rKey, T_BLCF_D& rData)
	{
		rKey = key;
		rData.nGroupId         = nGroupId        ;
		rData.bSlabRestriction = bSlabRestriction;
		rData.bLateralConf     = bLateralConf    ; 
		rData.nSlab            = nSlab           ; 
		rData.nLateral         = nLateral        ; 
		rData.nNumSpace        = nNumSpace       ; 
		//rData.arKeyElem.Copy(arKeyElem);    
		//rData.aInputSpace.Copy(aInputSpace);
	}
	void SetBlcf(const T_BLCF_K Key, const T_BLCF_D& rData)
	{ 
		key = Key;
		nGroupId         = rData.nGroupId        ;
		bSlabRestriction = rData.bSlabRestriction;
		bLateralConf     = rData.bLateralConf    ; 
		nSlab            = rData.nSlab           ; 
		nLateral         = rData.nLateral        ; 
		nNumSpace        = rData.nNumSpace       ;     
		nNumElem         = (int)rData.arKeyElem.GetSize();    
		nNumInputSpace   = (int)rData.aInputSpace.GetSize();    
	}  
};
// 이전에 CArray를 직접 저장하던 데이터 때문에 발생할 문제 해결용.
// CArray는 32bit(=20 Bytes)와 64bit(=40 Bytes)가 다른 크기를 갖는다.
struct T_BLCF_UDRD_D_dummy64bit_RW
{ 
	T_BLCF_K key;
	UINT nGroupId        ;
	BOOL bSlabRestriction;             
	BOOL bLateralConf    ;             
	int  nSlab           ;             
	int  nLateral        ;             
	int  nNumSpace       ;           
	char dummyArray_64bit_1[40];   // 하위 버전 호환용 size dummy 크기. CArray<T_ELEM_K,T_ELEM_K> arKeyElem;    
	char dummyArray_64bit_2[40];   // 하위 버전 호환용 size dummy 크기. CArray<double,double> aInputSpace; 
	int nNumElem;
	int nNumInputSpace;  
	void Convert(T_BLCF_UDRD_D_RW& TData)
	{
		TData.key = key;
		TData.nGroupId         = nGroupId        ;
		TData.bSlabRestriction = bSlabRestriction;
		TData.bLateralConf     = bLateralConf    ;
		TData.nSlab            = nSlab           ;
		TData.nLateral         = nLateral        ;
		TData.nNumSpace        = nNumSpace       ;
		TData.nNumElem         = nNumElem        ;
		TData.nNumInputSpace   = nNumInputSpace  ;
	}
};
struct T_BLCF_UNIT
{
	T_BLCF_UNIT()
	{
		dInputSpace  = D_UNITSYS_BASE_LENGTH;    
	}
	int dInputSpace;  
};
#define HASHSIZEBLCF HASHSIZEELEM

//////////////////////////////////////////////////////////////////////////
struct T_JUD_FES_DS_D
{ 
	T_STOR_K StorK         ; // Story Key
	double   dFes          ; // Fes
	double   dDs[2]        ; // Ds : [0]=정가력, [1]=부가력
	double   dExternalForce; // 외력
	double   dQud          ; // Qud
	void Initialize()
	{ 
		StorK          = 0 ; 
		dFes           = 0.; 
		dDs[0]         = 0.; 
		dDs[1]         = 0.;
		dExternalForce = 0.; 
		dQud           = 0.;
	}                  
	T_JUD_FES_DS_D() { Initialize(); };
	T_JUD_FES_DS_D(T_JUD_FES_DS_D& src) { *this = src; }
	T_JUD_FES_DS_D& operator=(const T_JUD_FES_DS_D& src)
	{                            
		StorK          = src.StorK         ; 
		dFes           = src.dFes          ; 
		dDs[0]         = src.dDs[0]        ; 
		dDs[1]         = src.dDs[1]        ;
		dExternalForce = src.dExternalForce;  
		dQud           = src.dQud          ; 
		return *this;
	}	  
};

#define T_JDFD_KEY unsigned int  
union T_JDFD_K
{
	T_JDFD_KEY keymap;
	struct
	{
		unsigned int entity : 20; // LoadCase Key
		unsigned int serial : 12; // Type : 1=Fes, 2=Ds, 3=외력분포(Ds), 4=외력분포(보유수평내력), 5=Qud
	} key;
};
struct T_JDFD_D 
{
	int nForceType; // T_JDFD_K의 key serial이 3 or 4일 경우 사용 - 외력지정 : 0=층전단력에 입력, 1=수평외력에 입력  
	CArray<T_JUD_FES_DS_D, T_JUD_FES_DS_D&> aJUDFesDsData; 
	
	void Initialize()
	{
		nForceType = 0;
		aJUDFesDsData.RemoveAll();      
	}
	T_JDFD_D() {}
	T_JDFD_D(T_JDFD_D& Data)	{*this = Data;}
	T_JDFD_D& operator = (const T_JDFD_D& Data)
	{    
		nForceType = Data.nForceType; 
		aJUDFesDsData.Copy(Data.aJUDFesDsData);    
		return *this;
	}    
};
struct T_JDFD_UDRD_D
{
	T_JDFD_K key;
	T_JDFD_D data;
};
struct T_JDFD_UDRD_D_RW
{ 
	T_JDFD_K key;
	int nForceType;
	char dummyArray_32bit[20];    // 하위 버전 호환용 size dummy 크기. CArray<T_JUD_FES_DS_D, T_JUD_FES_DS_D&> aJUDFesDsData;  
	int nNumJUDFesDsData;    
	
	void GetJDFD(T_JDFD_K& rKey, T_JDFD_D& rData)
	{
		rKey.keymap = key.keymap;
		rData.nForceType = nForceType;
		//rData.aJUDFesDsData.Copy(aJUDFesDsData);
	}
	void SetJDFD(const T_JDFD_K Key, const T_JDFD_D& rData)
	{    
		key.keymap = Key.keymap;
		nForceType = rData.nForceType;    
		nNumJUDFesDsData = (int)rData.aJUDFesDsData.GetSize();    
	}  
};
// 이전에 CArray를 직접 저장하던 데이터 때문에 발생할 문제 해결용.
// CArray는 32bit(=20 Bytes)와 64bit(=40 Bytes)가 다른 크기를 갖는다.
struct T_JDFD_UDRD_D_dummy64bit_RW
{ 
	T_JDFD_K key;
	int nForceType;
	char dummyArray_64bit[40];    // 하위 버전 호환용 size dummy 크기. CArray<T_JUD_FES_DS_D, T_JUD_FES_DS_D&> aJUDFesDsData;  
	int nNumJUDFesDsData;    
	void Convert(T_JDFD_UDRD_D_RW& TData)
	{
		TData.key = key;
		TData.nForceType = nForceType;
		TData.nNumJUDFesDsData = nNumJUDFesDsData;
	}
};
struct T_JDFD_UNIT
{
	T_JDFD_UNIT()
	{
		dExternalForce = D_UNITSYS_BASE_FORCE;  
		dQud           = D_UNITSYS_BASE_FORCE; 
	}
	int dExternalForce;  
	int dQud          ;
};
#define HASHSIZEJDFD HASHSIZESTLD

#define T_JDDP_K unsigned int
struct T_JDDP_D // 다이아프램 형식
{   
	int nDiaphragmType; // 0=내다이아프램, 1=통다이아프램, 2=외다이아프램, 3=기타
	void Initialize()
	{
		nDiaphragmType = 0;
	}  
	T_JDDP_D() { Initialize(); };
	T_JDDP_D(T_JDDP_D& src) { *this = src; }
	T_JDDP_D& operator=(const T_JDDP_D& src)
	{ 
		nDiaphragmType = src.nDiaphragmType;
		return *this;
	}	
};
struct T_JDDP_UDRD_D
{
	T_JDDP_K key;
	T_JDDP_D data;
};
#define HASHSIZEJDDP HASHSIZENODE

#define T_JDJP_K unsigned int
struct T_JDJP_D // 접합부 패널의 보강
{   
	double   dThick; // 보강재의 두께
	T_MATL_K MatlK ; // 보강재의 종류

	void Initialize()
	{
		dThick = 0.; 
		MatlK  = 0 ; 
	}  
	T_JDJP_D() { Initialize(); };
	T_JDJP_D(T_JDJP_D& src) { *this = src; }
	T_JDJP_D& operator=(const T_JDJP_D& src)
	{ 
		dThick = src.dThick; 
		MatlK  = src.MatlK ; 
		return *this;
	}	
};
struct T_JDJP_UDRD_D
{
	T_JDJP_K key;
	T_JDJP_D data;
};
struct T_JDJP_UNIT
{
	T_JDJP_UNIT()
	{
		dThick = D_UNITSYS_BASE_LENGTH;    
	}
	int dThick;  
};
#define HASHSIZEJDJP HASHSIZENODE

#define T_JRNK_K unsigned int
struct T_JRNK_D // 부재종별 직접지정
{   
	int nRank; // 부재종별 1:A, 2:B, 3:C, 4:D
	 
	void Initialize()
	{
		nRank = 1; 
	}  
	T_JRNK_D() { Initialize(); };
	T_JRNK_D(T_JRNK_D& src) { *this = src; }
	T_JRNK_D& operator=(const T_JRNK_D& src)
	{ 
		nRank = src.nRank; 
		return *this;
	}	
};
struct T_JRNK_UDRD_D
{
	T_JRNK_K key;
	T_JRNK_D data;
};
#define HASHSIZEJRNK HASHSIZEELEM

#define T_JDHO_K unsigned int  
struct T_JDHO_D // 일본2차설계 자동생성 Hinge & Offset
{  
	CArray<T_PHGT_K, T_PHGT_K> aJUDPhgtKey;
	CArray<T_PHGE_K, T_PHGE_K> aJUDPhgeKey;    
	CArray<T_OFFS_K, T_OFFS_K> aJUDOffsKey;  
	
	void Initialize()
	{
		aJUDPhgtKey.RemoveAll();     
		aJUDPhgeKey.RemoveAll(); 
		aJUDOffsKey.RemoveAll();
	}
	T_JDHO_D() {}
	T_JDHO_D(T_JDHO_D& Data)	{*this = Data;}
	T_JDHO_D& operator = (const T_JDHO_D& Data)
	{
		aJUDPhgtKey.Copy(Data.aJUDPhgtKey);        
		aJUDPhgeKey.Copy(Data.aJUDPhgeKey);  
		aJUDOffsKey.Copy(Data.aJUDOffsKey);
		return *this;
	}    
};
struct T_JDHO_UDRD_D
{
	T_JDHO_D data;
};
struct T_JDHO_UDRD_D_RW
{ 
	char aJUDPhgtKey[20];				//64비트 vs 32비트 모델 호환 처리
	char aJUDPhgeKey[20];
	char aJUDOffsKey[20];  
	int nNumJUDPhgtKey;  
	int nNumJUDPhgeKey;
	int nNumJUDOffsKey; 
	
	void GetJdho(T_JDHO_D& rData)
	{
//     rData.aJUDPhgtKey.Copy(aJUDPhgtKey);
//     rData.aJUDPhgeKey.Copy(aJUDPhgeKey);
//     rData.aJUDOffsKey.Copy(aJUDOffsKey);    
	}
	void SetJdho(const T_JDHO_D& rData)
	{     
		nNumJUDPhgtKey = (int)rData.aJUDPhgtKey.GetSize();    
		nNumJUDPhgeKey = (int)rData.aJUDPhgeKey.GetSize();    
		nNumJUDOffsKey = (int)rData.aJUDOffsKey.GetSize();    
	}  
};
#define HASHSIZEJDHO 1

#define T_JDPL_K unsigned int  
struct T_JDPL_D // 일본2차설계 자동생성 Pushover LC
{
	int nPushoverType; // 마지막으로 수행한 Pushover Mode : 0=일반, 1=2차설계

	CArray<UINT2, UINT2> aJUDPolcKey; // i1=Ds, i2=보유수평내력  
	
	void Initialize()
	{
		nPushoverType = 0;
		aJUDPolcKey.RemoveAll();      
	}
	T_JDPL_D() {}
	T_JDPL_D(T_JDPL_D& Data)	{*this = Data;}
	T_JDPL_D& operator = (const T_JDPL_D& Data)
	{
		nPushoverType = Data.nPushoverType;
		aJUDPolcKey.Copy(Data.aJUDPolcKey);    
		return *this;
	}    
};
struct T_JDPL_UDRD_D
{
	T_JDPL_D data;
};
struct T_JDPL_UDRD_D_RW
{  
	int nPushoverType;
	char aJUDPolcKey[20];	//CArray<UINT2, UINT2> aJUDPolcKey;  
	int nNumJUDPolcKey;    
	
	void GetJdpl(T_JDPL_D& rData)
	{
		rData.nPushoverType = nPushoverType;				
		rData.aJUDPolcKey.RemoveAll();	//rData.aJUDPolcKey.Copy(aJUDPolcKey);    
	}
	void SetJdpl(const T_JDPL_D& rData)
	{    
		nPushoverType = rData.nPushoverType;
		nNumJUDPolcKey = (int)rData.aJUDPolcKey.GetSize();    
	}  
};
#define HASHSIZEJDPL 1

#define T_JDSH_K unsigned int
struct T_JDSH_D // 의장고
{ 
	BOOL   bAuto      ; // Auto Check
	double dUserHeight; // Input Height
	double dSlabArea  ; // Input Slab Area  
	void Initialize()
	{
		bAuto       = TRUE;
		dUserHeight = 0.  ;
		dSlabArea   = 0.  ;
	}
	
	T_JDSH_D() { Initialize(); };
	T_JDSH_D(T_JDSH_D& src) { *this = src; }
	T_JDSH_D& operator=(const T_JDSH_D& src)
	{ 
		bAuto       = src.bAuto      ;
		dUserHeight = src.dUserHeight;  
		dSlabArea   = src.dSlabArea  ;
		return *this;
	}	
};
struct T_JDSH_UDRD_D
{
	T_JDSH_K key;
	T_JDSH_D data;
};
struct T_JDSH_UNIT
{
	T_JDSH_UNIT()
	{
		dUserHeight = D_UNITSYS_BASE_LENGTH;  
		dSlabArea   = D_UNITSYS_BASE_AREA  ;
	}
	int dUserHeight;  
	int dSlabArea  ;
};
#define HASHSIZEJDSH 101

#define T_JDRT_K unsigned int
struct T_JDRT_D // 루트 판정용 데이터
{           
	double dHeight    ; // 건물 높이 
	double dWidthX    ; // 폭 (X-방향) 
	double dWidthY    ; // 폭 (Y-방향)
	double dEaves     ; // 처마높이
	double dSpan      ; // 스팬길이
	double dArea      ; // 연면적  
	double dParapet   ; // 파라펫 높이
	
	void Initialize()
	{
		dHeight     = 0.  ;
		dWidthX     = 0.  ;
		dWidthY     = 0.  ;
		dEaves      = 0.  ;
		dSpan       = 0.  ;
		dArea       = 0.  ;
		dParapet    = 0.  ;    
	}
	
	T_JDRT_D() { Initialize(); };
	T_JDRT_D(T_JDRT_D& src) { *this = src; }
	T_JDRT_D& operator=(const T_JDRT_D& src)
	{   
		dHeight     = src.dHeight    ;    
		dWidthX     = src.dWidthX    ;
		dWidthY     = src.dWidthY    ;
		dEaves      = src.dEaves     ;
		dSpan       = src.dSpan      ;
		dArea       = src.dArea      ;  
		dParapet    = src.dParapet   ;    
		return *this;
	}	
};
struct T_JDRT_UDRD_D
{
	T_JDRT_D data;
};
struct T_JDRT_UNIT
{
	T_JDRT_UNIT()
	{
		dHeight  = D_UNITSYS_BASE_LENGTH;
		dWidthX  = D_UNITSYS_BASE_LENGTH;
		dWidthY  = D_UNITSYS_BASE_LENGTH;
		dEaves   = D_UNITSYS_BASE_LENGTH;
		dSpan    = D_UNITSYS_BASE_LENGTH;
		dArea    = D_UNITSYS_BASE_AREA  ;
		dParapet = D_UNITSYS_BASE_LENGTH;
	}
	int dHeight ;
	int dWidthX ;
	int dWidthY ;
	int dEaves  ;
	int dSpan   ;
	int dArea   ;
	int dParapet;
};
#define HASHSIZEJDRT 1

#define T_JDMS_K unsigned int
struct T_JDMS_D // 계산정보 - 모델&강성
{ 
	// nRoot는 nStructType이 RC일 때와 STL일때가 구분된다.
	// RC  : 0=자동, 1=루트1  , 2=루트2-1, 3=루트2-2, 4=루트2-3, 5=루트3
	// STL : 0=자동, 1=루트1-1, 2=루트1-2, 3=루트2  , 4=루트3
	int    nStructType   ; // 구조종별 지정 : 0=RC, 1=SRC, 2=STEEL  
	int    nRootX        ; // X방향 루트 
	int    nRootY        ; // Y방향 루트
	double dLimitAngle   ; // 층간 변형각 제한치
	//int    nFrameAxis    ; // 보, 기둥 강성 산정 시 주변부재 고려-축강성 : 2=해당부재 고려
	//int    nFrameBend    ; //                                     휨강성 : 2=해당부재 고려             
	//BOOL   bConsiderRebar; 
	//BOOL   bConsiderSteel;   
	BOOL   bIgnoreStiff  ; // 철골, RC 비틀림 강성 무시
	
	void Initialize()
	{
		nStructType    = 2   ;
		nRootX         = 4   ;
		nRootY         = 4   ;
		dLimitAngle    = 200 ;
		//nFrameAxis     = 2   ;
		//nFrameBend     = 2   ;                   
		//bConsiderRebar = TRUE;
		//bConsiderSteel = TRUE;
		bIgnoreStiff   = TRUE;
	}
	
	T_JDMS_D() { Initialize(); };
	T_JDMS_D(T_JDMS_D& src) { *this = src; }
	T_JDMS_D& operator=(const T_JDMS_D& src)
	{   
		nStructType    = src.nStructType   ;    
		nRootX         = src.nRootX        ;
		nRootY         = src.nRootY        ;
		dLimitAngle    = src.dLimitAngle   ;
		//nFrameAxis     = src.nFrameAxis    ;
		//nFrameBend     = src.nFrameBend    ;
		//bConsiderRebar = src.bConsiderRebar;
		//bConsiderSteel = src.bConsiderSteel;
		bIgnoreStiff   = src.bIgnoreStiff  ;
		return *this;
	}	
};
struct T_JDMS_UDRD_D
{
	T_JDMS_D data;
};
#define HASHSIZEJDMS 1

#define T_JDLS_K unsigned int
struct T_JDLS_D_800 // 계산정보 - 하중&응력
{
	int    nDomain_RC    ; // 강역 고려-RC  : 0=강역, 1=축심, 2=면
	BOOL   bColmStiff    ; // [사용안함] 기둥 축력에 의한 변형 미고려
	BOOL   bSnowFactor   ; // 다설지역시 단면산정 고려 계수
	double dSnowFactor_LT; // 장기
	double dSnowFactor_EL; // 단기(지진하중시)
	double dSnowFactor_WL; // 단기(풍하중시)  
	int    nShortLcom    ; // 단기하중케이스의 조합법 : 0=선형, 1=비선형  
	int    nStrsXPos     ; // 수평하중의 방향-X : 0=정방향, 1=부방향, 2=양방향
	int    nStrsYPos     ; // 수평하중의 방향-Y 
	int    nStfnCalc     ; // 편심율-강심위치 : 0=이론식, 1=기술기준 해설서 
	int    nStoryAngle   ; // 층간변형각 : 0=최대치(가력방향), 1=최대치(변형방향), 2=각프레임의 평균(변형방향)
	int    nStoryStfn    ; // 강성율 계산시층각변형각 : 0=강심위치(가력방향), 1=각프레임의 평균(변형방향)
};

struct T_JDLS_D // 계산정보 - 하중&응력
{           
	int    nDomain_RC    ; // 강역 고려-RC  : 0=강역, 1=축심, 2=면
	BOOL   bColmStiff    ; // [사용안함] 기둥 축력에 의한 변형 미고려
	BOOL   bSnowFactor   ; // 다설지역시 단면산정 고려 계수
	double dSnowFactor_LT; // 장기
	double dSnowFactor_EL; // 단기(지진하중시)
	double dSnowFactor_WL; // 단기(풍하중시)  
	int    nShortLcom    ; // 단기하중케이스의 조합법 : 0=선형, 1=비선형  
	int    nStrsXPos     ; // 수평하중의 방향-X : 0=정방향, 1=부방향, 2=양방향
	int    nStrsYPos     ; // 수평하중의 방향-Y 
	int    nStfnCalc     ; // 편심율-강심위치 : 0=이론식, 1=기술기준 해설서 
	int    nStoryAngle   ; // 층간변형각 : 0=최대치(가력방향), 1=최대치(변형방향), 2=각프레임의 평균(변형방향)
	int    nStoryStfn    ; // 강성율 계산시층각변형각 : 0=강심위치(가력방향), 1=각프레임의 평균(변형방향)
	int    nUpLifting    ; // 떠오름 고려 : 0=한다, 1=안한다  // 8.0.5
	//int    nStrsSeis     ;  
	//BOOL   bTensionOnly  ;
	//BOOL   bCompOnly     ;	
	
	void Initialize()
	{
		nDomain_RC     = 0    ;
		bColmStiff     = FALSE;
		bSnowFactor    = FALSE;
		dSnowFactor_LT = 0.7  ;
		dSnowFactor_EL = 0.35 ;
		dSnowFactor_WL = 0.35 ;
		nShortLcom     = 0    ;    
		nStrsXPos      = 2    ;
		nStrsYPos      = 2    ;
		nStfnCalc      = 0    ;
		nStoryAngle    = 0    ;
		nStoryStfn     = 0    ;
		nUpLifting     = 1    ;
	}
	
	void Convert800(T_JDLS_D_800& rData)
	{
		nDomain_RC     = rData.nDomain_RC    ;
		bColmStiff     = rData.bColmStiff    ;
		bSnowFactor    = rData.bSnowFactor   ;
		dSnowFactor_LT = rData.dSnowFactor_LT;
		dSnowFactor_EL = rData.dSnowFactor_EL;
		dSnowFactor_WL = rData.dSnowFactor_WL;
		nShortLcom     = rData.nShortLcom    ;
		nStrsXPos      = rData.nStrsXPos     ;
		nStrsYPos      = rData.nStrsYPos     ;
		nStfnCalc      = rData.nStfnCalc     ;
		nStoryAngle    = rData.nStoryAngle   ;
		nStoryStfn     = rData.nStoryStfn    ;
		nUpLifting     = 1                   ;
	}
};
struct T_JDLS_UDRD_D
{
	T_JDLS_D data;
};
#define HASHSIZEJDLS 1

#define T_JDSC_K unsigned int
struct T_JDSC_D // 계산정보 - 단면산정 
{ 
	int     nBeamEndLocRC    ; // 단부위치 설정 RC/SRC-보  : 0=강역, 1=축심, 2=면
	int     nColmEndLocRC    ; //                     -기둥 
	int     nPedtEndLocRC    ; //                     -주각 
	int     nBeamEndLocS     ; // 단부위치 설정 S/CFT-보 
	int     nColmEndLocS     ; //                    -기둥 
	int     nPedtEndLocS     ; //                    -주각 
	int     nBeamEndLocRC_ST ; // 단기
	int     nColmEndLocRC_ST ; // 단기                    
	int     nPedtEndLocRC_ST ; // 단기                    
	int     nBeamEndLocS_ST  ; // 단기 
	int     nColmEndLocS_ST  ; // 단기                    
	int     nPedtEndLocS_ST  ; // 단기
	int     nSpan            ; // 내법 치수 설정방법 : 0=강역단간, 1=순 스팬
	int     nColmQualify_RC  ; // 기둥 단면검정방법 RC/SRC : 0=1축휨, 1=2축휨(축력), 2=2축휨(편심)
	int     nColmQualify_SCFT; // 기둥 단면검정방법 S/CFT                           
	BOOL    bUseRootAs       ; // Pt, Pw의 최소/최대 철근비 지정 : TRUE=루트 판정시 Data 사용

	double	dFrameStfn       ; // 사용안함(jdub로 옮김) 기준강도의 할증률-주철근(보,기둥)
	double	dSteelStfn       ; // 사용안함(jdub로 옮김)                  -철골

	//double	dMinLength_Beam  ; // 부재의 최소치수-보
	//double	dMinLength_Colm  ; //                -기둥
	double	dRCBeamPt[2]     ; // Pt-RC보 : [0]:max. [1]:min
	double	dRCColmPt[2]     ; //   -RC기둥
	double	dRCBeamPw[2]     ; // pw-RC보
	double	dRCColmPw[2]     ; //   -RC기둥 
	BOOL    bBeamOrtho       ; // 직교 가력시 보의 단면산정
	//double	dBeamFactorRC    ; // 큰 보의 처짐검토 변형증대계수-RC 
	//double	dBeamFactorSteel ; //                              -STL
	
	void Initialize()
	{    
		nBeamEndLocRC = 1;    
		nColmEndLocRC = 1;       
		nPedtEndLocRC = 1;       
		nBeamEndLocS  = 1;        
		nColmEndLocS  = 1;        
		nPedtEndLocS  = 2;     

		nBeamEndLocRC_ST = 0;    
		nColmEndLocRC_ST = 0;       
		nPedtEndLocRC_ST = 0;       
		nBeamEndLocS_ST  = 1;        
		nColmEndLocS_ST  = 1;        
		nPedtEndLocS_ST  = 2;     
	 
		nSpan             = 0   ;
		nColmQualify_RC   = 0   ; 
		nColmQualify_SCFT = 1   ;
		bUseRootAs        = TRUE;
		dFrameStfn        = 1   ;
		dSteelStfn        = 1   ;                
		//dMinLength_Beam   = 0.25;
		//dMinLength_Colm   = 0.4 ;
		dRCBeamPt[0]      = 2   ;
		dRCBeamPt[1]      = 0   ;    
		dRCColmPt[0]      = 2   ;
		dRCColmPt[1]      = 0   ;
		dRCBeamPw[0]      = 1.2 ;
		dRCBeamPw[1]      = 0.2 ;    
		dRCColmPw[0]      = 1.2 ;
		dRCColmPw[1]      = 0.2 ;
		bBeamOrtho        = FALSE;
		//dBeamFactorRC     = 8   ;  
		//dBeamFactorSteel  = 1   ;
	}
	
	T_JDSC_D() { Initialize(); };
	T_JDSC_D(T_JDSC_D& src) { *this = src; }
	T_JDSC_D& operator=(const T_JDSC_D& src)
	{      
		nBeamEndLocRC = src.nBeamEndLocRC;    
		nColmEndLocRC = src.nColmEndLocRC;       
		nPedtEndLocRC = src.nPedtEndLocRC;       
		nBeamEndLocS  = src.nBeamEndLocS ;        
		nColmEndLocS  = src.nColmEndLocS ;        
		nPedtEndLocS  = src.nPedtEndLocS ;     

		nBeamEndLocRC_ST = src.nBeamEndLocRC_ST;    
		nColmEndLocRC_ST = src.nColmEndLocRC_ST;       
		nPedtEndLocRC_ST = src.nPedtEndLocRC_ST;       
		nBeamEndLocS_ST  = src.nBeamEndLocS_ST ;        
		nColmEndLocS_ST  = src.nColmEndLocS_ST ;        
		nPedtEndLocS_ST  = src.nPedtEndLocS_ST ;   

		nSpan             = src.nSpan            ;
		nColmQualify_RC   = src.nColmQualify_RC  ; 
		nColmQualify_SCFT = src.nColmQualify_SCFT;
		bUseRootAs        = src.bUseRootAs       ;
		dFrameStfn        = src.dFrameStfn       ;
		dSteelStfn        = src.dSteelStfn       ;                
		//dMinLength_Beam   = src.dMinLength_Beam  ;
		//dMinLength_Colm   = src.dMinLength_Colm  ;
		bBeamOrtho        = src.bBeamOrtho       ;
		//dBeamFactorRC     = src.dBeamFactorRC    ;  
		//dBeamFactorSteel  = src.dBeamFactorSteel ;
		for (int i=0; i<2; i++)
		{
			dRCBeamPt[i] = src.dRCBeamPt[i];
			dRCColmPt[i] = src.dRCColmPt[i];
			dRCBeamPw[i] = src.dRCBeamPw[i];
			dRCColmPw[i] = src.dRCColmPw[i];        
		}                                        
		return *this;
	}	
};
struct T_JDSC_UDRD_D
{
	T_JDSC_D data;
};
// struct T_JDSC_UNIT
// {
//   T_JDSC_UNIT()
//   {
//     dMinLength_Beam=D_UNITSYS_BASE_LENGTH;
//     dMinLength_Colm=D_UNITSYS_BASE_LENGTH;
//   }
//   int dMinLength_Beam;
//   int dMinLength_Colm;
// };
#define HASHSIZEJDSC 1

#define T_JDRC_K unsigned int
struct T_JDRC_D // 계산정보 - 단면RC 
{           
	int    nQd           ; // QD 결정방법 : 0=MIN(Qo+Qy, QL+n·QE), 1=Qo+Qy, 2=QL+n·QE
	double dQdExtra_N    ; // nQd가 2일 경우 할증률(n)
	//double dQdExtra_Alpha; // 루트2-3인 경우 할증률(α)
	int    nShear        ; // 전단강도식 : 0=허용전단, 1=종국전단
	int    nUltimateShear; // 종국전단 : 0=0.053, 1=0.068
	//int    nUrban        ; 
	//double dCrackWidth   ;  
	BOOL   bColmMainRebar; // 주철근 개수 결정시 0.8% 적용 
	//BOOL   bColmSSWF     ;  
	//BOOL   bColmSCWB     ;  
	BOOL   bColmAdStrs   ; // 부착응력도의 검토 (RC규준 1991년)  
	int    nColmMu       ; // 기둥Mu산정식-인장철근 : 0=at반영식, 1=ag반영식 
	double dBeamMinRatio ; // 보-최소복근비
	double dBeamCentRebar; // 보-중앙부 상단배근/단부최대배근 
	//int    nBeamRebarArea; 
	BOOL   bBeamSSWF     ; // 강전단 약휨
	BOOL   bBeamAdStrs   ; // 부착응력도와 필요 정착길이 검토 (RC규준 1991년)
	BOOL   bBeamAdLength ; // [사용안함] 필요부착길이 검토 (RC규준 1999년) 
	int    nBeamAdLength ; // [사용안함] 0=존재응력도, 1=항복강도
	double dBeamBndrRebar; // 단부중앙경계선
	int    nBeamFixEnd   ; // 정착길이-단부
	int    nBeamFixCent  ; //         -중앙부
	int    nBeamHook     ; // 양단후크의 유무 : 0=없음, 1-있음
	
	void Initialize()
	{
		nQd            = 0    ;   
		dQdExtra_N     = 1.5  ;
		//dQdExtra_Alpha = 1.1  ;
		nShear         = 0    ;   
		nUltimateShear = 1    ;
		//nUrban         = 0    ;   
		//dCrackWidth    = 0.2  ;
		bColmMainRebar = TRUE ;   
		//bColmSSWF      = TRUE ;   
		//bColmSCWB      = TRUE ;  
		bColmAdStrs    = FALSE;  
		nColmMu        = 0    ;  
		dBeamMinRatio  = 0.3  ;
		dBeamCentRebar = 0.25 ;
		//nBeamRebarArea = 0    ;   
		bBeamSSWF      = TRUE ;  
		bBeamAdStrs    = TRUE ;  
		bBeamAdLength  = FALSE;
		nBeamAdLength  = 0    ;
		dBeamBndrRebar = 0.25 ;   
		nBeamFixEnd    = 15   ;
		nBeamFixCent   = 20   ;    
		nBeamHook      = 0    ;
	}
	
	T_JDRC_D() { Initialize(); };
	T_JDRC_D(T_JDRC_D& src) { *this = src; }
	T_JDRC_D& operator=(const T_JDRC_D& src)
	{   
		nQd            = src.nQd           ;   
		dQdExtra_N     = src.dQdExtra_N    ;
		//dQdExtra_Alpha = src.dQdExtra_Alpha;
		nShear         = src.nShear        ;   
		nUltimateShear = src.nUltimateShear;
		//nUrban         = src.nUrban        ;   
		//dCrackWidth    = src.dCrackWidth   ;
		bColmMainRebar = src.bColmMainRebar;   
		//bColmSSWF      = src.bColmSSWF     ;   
		//bColmSCWB      = src.bColmSCWB     ;  
		bColmAdStrs    = src.bColmAdStrs   ;  
		nColmMu        = src.nColmMu       ;  
		dBeamMinRatio  = src.dBeamMinRatio ;
		dBeamCentRebar = src.dBeamCentRebar;
		//nBeamRebarArea = src.nBeamRebarArea;   
		bBeamSSWF      = src.bBeamSSWF     ;  
		bBeamAdStrs    = src.bBeamAdStrs   ;  
		bBeamAdLength  = src.bBeamAdLength ;
		nBeamAdLength  = src.nBeamAdLength ;
		dBeamBndrRebar = src.dBeamBndrRebar;
		nBeamFixEnd    = src.nBeamFixEnd   ;
		nBeamFixCent   = src.nBeamFixCent  ;
		nBeamHook      = src.nBeamHook     ;
		return *this;
	}	
};
struct T_JDRC_UDRD_D
{
	T_JDRC_D data;
};
#define HASHSIZEJDRC 1

#define T_JDST_K unsigned int
struct T_JDST_D_805 // 계산정보 - 단면S
{           
	int    nBeamSlabRe   ; // 상단 플랜지에 대한 슬래브의 구속 : 0=구속, 1=비구속
	BOOL   bBeamReinforce; // 휨보강의 검토 (루트1-1의 경우)
	BOOL   bBendingCoeff ; // Bending Coeff
	double dColmBuckling ; // Alpha
	double dInDiapBCP    ; // 내다이아프램 - BCP
	double dInDiapBCR    ; //              - BCR
	double dInDiapSTKR   ; //              - STKR
	double dDiapBCP      ; // 통다이아프램 - BCP
	double dDiapBCR      ; //              - BCR
	double dDiapSTKR     ; //              - STKR
	double dOutDiapBCP   ; // 외다이아프램 - BCP
	double dOutDiapBCR   ; //              - BCR
	double dOutDiapSTKR  ; //              - STKR
	double dEtcDiapBCP   ; // 기타 - BCP
	double dEtcDiapBCR   ; //      - BCR
	double dEtcDiapSTKR  ; //      - STKR
	T_STOR_K nMaxStory   ; // 최상층 Story Key : 0=Auto
	T_STOR_K nMinStory   ; // 최하층 Story Key
	BOOL   bWebBeam      ; // 빔 웨브휨내력 고려    // 8.0.5
	BOOL   bWebColm      ; // 기둥 웨브휨내력 고려  // 8.0.5
};
struct T_JDST_D_800 // 계산정보 - 단면S
{           
	int    nBeamSlabRe   ; // 상단 플랜지에 대한 슬래브의 구속 : 0=구속, 1=비구속
	BOOL   bBeamReinforce; // 휨보강의 검토 (루트1-1의 경우)
	BOOL   bBendingCoeff ; // Bending Coeff
	double dColmBuckling ; // Alpha
	double dInDiapBCP    ; // 내다이아프램 - BCP
	double dInDiapBCR    ; //              - BCR
	double dInDiapSTKR   ; //              - STKR
	double dDiapBCP      ; // 통다이아프램 - BCP
	double dDiapBCR      ; //              - BCR
	double dDiapSTKR     ; //              - STKR
	double dOutDiapBCP   ; // 외다이아프램 - BCP
	double dOutDiapBCR   ; //              - BCR
	double dOutDiapSTKR  ; //              - STKR
	double dEtcDiapBCP   ; // 기타 - BCP
	double dEtcDiapBCR   ; //      - BCR
	double dEtcDiapSTKR  ; //      - STKR
	T_STOR_K nMaxStory   ; // 최상층 Story Key : 0=Auto
	T_STOR_K nMinStory   ; // 최하층 Story Key
};

struct T_JDST_D // 계산정보 - 단면S
{           
	int    nBeamSlabRe   ; // 상단 플랜지에 대한 슬래브의 구속 : 0=구속, 1=비구속
	BOOL   bBeamReinforce; // 휨보강의 검토 (루트1-1의 경우)
	BOOL   bBendingCoeff ; // Bending Coeff
	double dColmBuckling ; // Alpha
	double dInDiapBCP    ; // 내다이아프램 - BCP
	double dInDiapBCR    ; //              - BCR
	double dInDiapSTKR   ; //              - STKR
	double dDiapBCP      ; // 통다이아프램 - BCP
	double dDiapBCR      ; //              - BCR
	double dDiapSTKR     ; //              - STKR
	double dOutDiapBCP   ; // 외다이아프램 - BCP
	double dOutDiapBCR   ; //              - BCR
	double dOutDiapSTKR  ; //              - STKR
	double dEtcDiapBCP   ; // 기타 - BCP
	double dEtcDiapBCR   ; //      - BCR
	double dEtcDiapSTKR  ; //      - STKR
	T_STOR_K nMaxStory   ; // 최상층 Story Key : 0=Auto
	T_STOR_K nMinStory   ; // 최하층 Story Key
	BOOL   bWebBeam      ; // 빔 웨브휨내력 고려    // 8.0.5
	BOOL   bWebColm      ; // 기둥 웨브휨내력 고려  // 8.0.5
	BOOL   bFlexualYieldBrace; // 주각 휨항복 판정시 brace 축력 고려  // 8.1.0

	void Initialize()
	{
		nBeamSlabRe    = 1    ;   
		bBeamReinforce = FALSE;
		bBendingCoeff  = TRUE ;
		dColmBuckling  = 0.7  ;
		dInDiapBCP     = 1.1  ;
		dInDiapBCR     = 1.2  ;
		dInDiapSTKR    = 1.3  ;
		dDiapBCP       = 1.2  ;
		dDiapBCR       = 1.3  ;
		dDiapSTKR      = 1.4  ;
		dOutDiapBCP    = 1.2  ;
		dOutDiapBCR    = 1.3  ;
		dOutDiapSTKR   = 1.4  ;
		dEtcDiapBCP    = 1.0  ;
		dEtcDiapBCR    = 1.0  ;
		dEtcDiapSTKR   = 1.0  ;
		nMaxStory      = 0    ;
		nMinStory      = 0    ;
		bWebBeam       = TRUE ;
		bWebColm       = TRUE ;
		bFlexualYieldBrace = TRUE;
	}                       
	
	T_JDST_D() { Initialize(); };
	T_JDST_D(T_JDST_D& src) { *this = src; }
	T_JDST_D& operator=(const T_JDST_D& src)
	{
		nBeamSlabRe    = src.nBeamSlabRe   ;
		bBeamReinforce = src.bBeamReinforce;
		bBendingCoeff  = src.bBendingCoeff ;
		dColmBuckling  = src.dColmBuckling ;
		dInDiapBCP     = src.dInDiapBCP    ;
		dInDiapBCR     = src.dInDiapBCR    ;
		dInDiapSTKR    = src.dInDiapSTKR   ;
		dDiapBCP       = src.dDiapBCP      ;
		dDiapBCR       = src.dDiapBCR      ;
		dDiapSTKR      = src.dDiapSTKR     ;
		dOutDiapBCP    = src.dOutDiapBCP   ;
		dOutDiapBCR    = src.dOutDiapBCR   ;
		dOutDiapSTKR   = src.dOutDiapSTKR  ;
		dEtcDiapBCP    = src.dEtcDiapBCP   ;
		dEtcDiapBCR    = src.dEtcDiapBCR   ;
		dEtcDiapSTKR   = src.dEtcDiapSTKR  ;
		nMaxStory      = src.nMaxStory     ;
		nMinStory      = src.nMinStory     ;
		bWebBeam       = src.bWebBeam      ;
		bWebColm       = src.bWebColm      ;
		bFlexualYieldBrace = src.bFlexualYieldBrace      ;
		return *this;
	}
	void Convert805(T_JDST_D_805& rData)
	{
		nBeamSlabRe    = rData.nBeamSlabRe   ;
		bBeamReinforce = rData.bBeamReinforce;
		bBendingCoeff  = rData.bBendingCoeff ;
		dColmBuckling  = rData.dColmBuckling ;
		dInDiapBCP     = rData.dInDiapBCP    ;
		dInDiapBCR     = rData.dInDiapBCR    ;
		dInDiapSTKR    = rData.dInDiapSTKR   ;
		dDiapBCP       = rData.dDiapBCP      ;
		dDiapBCR       = rData.dDiapBCR      ;
		dDiapSTKR      = rData.dDiapSTKR     ;
		dOutDiapBCP    = rData.dOutDiapBCP   ;
		dOutDiapBCR    = rData.dOutDiapBCR   ;
		dOutDiapSTKR   = rData.dOutDiapSTKR  ;
		dEtcDiapBCP    = rData.dEtcDiapBCP   ;
		dEtcDiapBCR    = rData.dEtcDiapBCR   ;
		dEtcDiapSTKR   = rData.dEtcDiapSTKR  ;
		nMaxStory      = rData.nMaxStory     ;
		nMinStory      = rData.nMinStory     ;
		bWebBeam       = rData.bWebBeam      ;
		bWebColm       = rData.bWebColm      ;
		bFlexualYieldBrace = TRUE;
	}
	void Convert800(T_JDST_D_800& rData)
	{
		nBeamSlabRe    = rData.nBeamSlabRe   ;
		bBeamReinforce = rData.bBeamReinforce;
		bBendingCoeff  = rData.bBendingCoeff ;
		dColmBuckling  = rData.dColmBuckling ;
		dInDiapBCP     = rData.dInDiapBCP    ;
		dInDiapBCR     = rData.dInDiapBCR    ;
		dInDiapSTKR    = rData.dInDiapSTKR   ;
		dDiapBCP       = rData.dDiapBCP      ;
		dDiapBCR       = rData.dDiapBCR      ;
		dDiapSTKR      = rData.dDiapSTKR     ;
		dOutDiapBCP    = rData.dOutDiapBCP   ;
		dOutDiapBCR    = rData.dOutDiapBCR   ;
		dOutDiapSTKR   = rData.dOutDiapSTKR  ;
		dEtcDiapBCP    = rData.dEtcDiapBCP   ;
		dEtcDiapBCR    = rData.dEtcDiapBCR   ;
		dEtcDiapSTKR   = rData.dEtcDiapSTKR  ;
		nMaxStory      = rData.nMaxStory     ;
		nMinStory      = rData.nMinStory     ;
		bWebBeam       = TRUE                ;
		bWebColm       = TRUE                ;
		bFlexualYieldBrace = TRUE;
	}
};
struct T_JDST_UDRD_D
{
	T_JDST_D data;
};
#define HASHSIZEJDST 1

#define T_JDJO_K unsigned int
struct T_JDJO_D // 계산정보 - 접합부
{           
	//BOOL   bRCShort            ;    
	//int    nRCShortShearX      ; 
	//int    nRCShortShearY      ; 
	//BOOL   bRCSlab             ;  
	//BOOL   bRCShearExtra       ;  
	//BOOL   bRCShearReduction   ;  
	//BOOL   bUltimate           ;   
	//int    nUltimate           ; 
	//BOOL   bColmHeight         ;   
	BOOL   bSteelShort         ; //단기허용응력도 설계(S부재) 
	//BOOL   bSteelColm          ; 
	//int    nSteelColm          ;   
	//BOOL   bSteelBeam          ; 
	//int    nSteelBeam          ;   
	//double dRCShearExtra       ;     
	//double dRCShearReduction   ;
	//double dRCStrsExtra        ;
	//double dSRCStrsExtra       ;
	//double dSRCShearReinforce_T;
	//double dSRCShearReinforce_L;
	//double dColmHeight         ;
	
	void Initialize()
	{
		//bRCShort             = TRUE ;       
		//nRCShortShearX       = 0    ; 
		//nRCShortShearY       = 0    ; 
		//bRCSlab              = FALSE;  
		//bRCShearExtra        = FALSE;  
		//bRCShearReduction    = FALSE;  
		//bUltimate            = FALSE;   
		//nUltimate            = 0    ; 
		//bColmHeight          = FALSE;   
		bSteelShort          = TRUE ; 
		//bSteelColm           = FALSE; 
		//nSteelColm           = 0    ;   
		//bSteelBeam           = 0    ; 
		//nSteelBeam           = FALSE;       
		//dRCShearExtra        = 1    ;
		//dRCShearReduction    = 1    ;
		//dRCStrsExtra         = 1    ;
		//dSRCStrsExtra        = 1    ;
		//dSRCShearReinforce_T = 0.1  ;
		//dSRCShearReinforce_L = 0.1  ;
		//dColmHeight          = 0.75 ;  
	}
	
	T_JDJO_D() { Initialize(); };
	T_JDJO_D(T_JDJO_D& src) { *this = src; }
	T_JDJO_D& operator=(const T_JDJO_D& src)
	{   
		//bRCShort             = src.bRCShort            ;       
		//nRCShortShearX       = src.nRCShortShearX      ; 
		//nRCShortShearY       = src.nRCShortShearY      ; 
		//bRCSlab              = src.bRCSlab             ;  
		//bRCShearExtra        = src.bRCShearExtra       ;  
		//bRCShearReduction    = src.bRCShearReduction   ;  
		//bUltimate            = src.bUltimate           ;   
		//nUltimate            = src.nUltimate           ; 
		//bColmHeight          = src.bColmHeight         ;   
		bSteelShort          = src.bSteelShort         ; 
		//bSteelColm           = src.bSteelColm          ; 
		//nSteelColm           = src.nSteelColm          ;   
		//bSteelBeam           = src.bSteelBeam          ; 
		//nSteelBeam           = src.nSteelBeam          ;       
		//dRCShearExtra        = src.dRCShearExtra       ;
		//dRCShearReduction    = src.dRCShearReduction   ;
		//dRCStrsExtra         = src.dRCStrsExtra        ;
		//dSRCStrsExtra        = src.dSRCStrsExtra       ;
		//dSRCShearReinforce_T = src.dSRCShearReinforce_T;
		//dSRCShearReinforce_L = src.dSRCShearReinforce_L;
		//dColmHeight          = src.dColmHeight         ;  
		return *this;
	}	
};
struct T_JDJO_UDRD_D
{
	T_JDJO_D data;
};
#define HASHSIZEJDJO 1

#define T_JDCR_K unsigned int
struct T_JDCR_D // 내력정보 - 균열
{  
	BOOL   bFrameBending; // 보,기둥 휨 
	BOOL   bFrameShear  ; // 보,기둥 전단
	int    nBeamYield   ; // 항복시 휨강성 저감률-보 : 0=a/D를 이용한 Method-1 또는 2식 사용, 1=Method-1식 사용
	int    nColmYield   ; //                     -기둥
	int    nMcOpt       ; // Mc 산정 : 0=√σB, 1=σB
	double dMC          ; // 산정계수
	
	void Initialize()
	{
		bFrameBending = TRUE;
		bFrameShear   = TRUE;    
		nBeamYield    = 0   ;
		nColmYield    = 0   ;   
		nMcOpt        = 0   ;
		dMC           = 0.56;    
	}
	
	T_JDCR_D() { Initialize(); };
	T_JDCR_D(T_JDCR_D& src) { *this = src; }
	T_JDCR_D& operator=(const T_JDCR_D& src)
	{   
		bFrameBending = src.bFrameBending;    
		bFrameShear   = src.bFrameShear  ;    
		nBeamYield    = src.nBeamYield   ;
		nColmYield    = src.nColmYield   ;
		nMcOpt        = src.nMcOpt       ;
		dMC           = src.dMC          ;
		return *this;
	}	
};
struct T_JDCR_UDRD_D
{
	T_JDCR_D data;
};
#define HASHSIZEJDCR 1

#define T_JDUB_K unsigned int
struct T_JDUB_D_805 // 내력정보 - 종국내력 - 축/휨
{  
	int    nSteelMU          ; // Steel 기둥의 Mu산정식 : 0=이론식, 1=간략식
	double dCollaStrsExtra   ; // 휨용 응력 할증률 
	double dCollaSigmaMU     ; // ∑CMu / ∑GMu
	double dCollaMU          ; // cMu/cMm
	double dCollaNComp       ; // N/Nu-압축측
	double dCollaNTens       ; //     -인장측
	BOOL   bMuBuckling       ; // 철골보 Mu 산정시 횡좌굴 고려       
	BOOL   bFlexualYieldBrace; // 주각 휨항복 판정시 brace 축력 고려  
	double dRCRectAlpha      ; // 기둥 항복곡면 산정식(강축/약축)-RC-각형  
	double dRCCirAlpha       ; //                                   -원형
	double dSteelCirAlpha    ; //                                -STL-원형
	double dSteelRectAlpha   ; //                                    -각형
	double dSteelEtcAlpha    ; //                                    -기타
	double dFrameStfn        ; // 기준강도의 할증률-주철근(보,기둥)
	double dSteelStfn        ; //                  -철골
};

struct T_JDUB_D_BACKUP // 내력정보 - 종국내력 - 축/휨
{  
	//int    nRCMU             ; 
	int    nSteelMU          ; // Steel 기둥의 Mu산정식 : 0=이론식, 1=간략식
	double dCollaStrsExtra   ; // 휨용 응력 할증률 
	double dCollaSigmaMU     ; // ∑CMu / ∑GMu
	double dCollaMU          ; // cMu/cMm
	double dCollaNComp       ; // N/Nu-압축측
	double dCollaNTens       ; //     -인장측
	//BOOL   bWeb              ; // 웨브휨내력 고려
	BOOL   bMuBuckling       ; // 철골보 Mu 산정시 횡좌굴 고려       
	BOOL   bFlexualYieldBrace; // 주각 휨항복 판정시 brace 축력 고려  
	double dRCRectAlpha      ; // 기둥 항복곡면 산정식(강축/약축)-RC-각형  
	double dRCCirAlpha       ; //                                   -원형
	double dSteelCirAlpha    ; //                                -STL-원형
	double dSteelRectAlpha   ; //                                    -각형
	double dSteelEtcAlpha    ; //                                    -기타
};

struct T_JDUB_D_800 // 내력정보 - 종국내력 - 축/휨
{  
	int    nSteelMU          ; // Steel 기둥의 Mu산정식 : 0=이론식, 1=간략식
	double dCollaStrsExtra   ; // 휨용 응력 할증률 
	double dCollaSigmaMU     ; // ∑CMu / ∑GMu
	double dCollaMU          ; // cMu/cMm
	double dCollaNComp       ; // N/Nu-압축측
	double dCollaNTens       ; //     -인장측
	BOOL   bMuBuckling       ; // 철골보 Mu 산정시 횡좌굴 고려       
	BOOL   bFlexualYieldBrace; // 주각 휨항복 판정시 brace 축력 고려  
	double dRCRectAlpha      ; // 기둥 항복곡면 산정식(강축/약축)-RC-각형  
	double dRCCirAlpha       ; //                                   -원형
	double dSteelCirAlpha    ; //                                -STL-원형
	double dSteelRectAlpha   ; //                                    -각형
	double dSteelEtcAlpha    ; //                                    -기타
	double dFrameStfn        ; // 기준강도의 할증률-주철근(보,기둥)
	double dSteelStfn        ; //                  -철골
};

struct T_JDUB_D // 내력정보 - 종국내력 - 축/휨
{  
	//int    nRCMU             ; 
	int    nSteelMU          ; // Steel 기둥의 Mu산정식 : 0=이론식, 1=간략식
	double dCollaStrsExtra   ; // 휨용 응력 할증률 
	double dCollaSigmaMU     ; // ∑CMu / ∑GMu
	double dCollaMU          ; // cMu/cMm
	double dCollaNComp       ; // N/Nu-압축측
	double dCollaNTens       ; //     -인장측
	BOOL   bMuBuckling       ; // 철골보 Mu 산정시 횡좌굴 고려       
//   BOOL   bFlexualYieldBrace; // 주각 휨항복 판정시 brace 축력 고려  
	double dRCRectAlpha      ; // 기둥 항복곡면 산정식(강축/약축)-RC-각형  
	double dRCCirAlpha       ; //                                   -원형
	double dSteelCirAlpha    ; //                                -STL-원형
	double dSteelRectAlpha   ; //                                    -각형
	double dSteelEtcAlpha    ; //                                    -기타
	double dFrameStfn        ; // 기준강도의 할증률-주철근(보,기둥)
	double dSteelStfn        ; //                  -철골

	void Initialize()
	{
		//nRCMU              = 1; // 초기값은 구조규정 ag 식 
		nSteelMU           = 1   ;
		dCollaStrsExtra    = 1.1 ;
		dCollaSigmaMU      = 1.2 ;
		dCollaMU           = 1.2 ;
		dCollaNComp        = 0.4 ;
		dCollaNTens        = 0.6 ;
		bMuBuckling        = TRUE;        
//     bFlexualYieldBrace = TRUE;   
		dRCRectAlpha       = 1   ;
		dRCCirAlpha        = 2   ;
		dSteelCirAlpha     = 2   ;
		dSteelRectAlpha    = 1   ;   
		dSteelEtcAlpha     = 1   ;
		dFrameStfn         = 1   ;
		dSteelStfn         = 1   ;
	}                    
	void ConvertBackup(T_JDUB_D_BACKUP& rData)
	{
		nSteelMU           = rData.nSteelMU          ;
		dCollaStrsExtra    = rData.dCollaStrsExtra   ;
		dCollaSigmaMU      = rData.dCollaSigmaMU     ;
		dCollaMU           = rData.dCollaMU          ;
		dCollaNComp        = rData.dCollaNComp       ;
		dCollaNTens        = rData.dCollaNTens       ;
		bMuBuckling        = rData.bMuBuckling       ;        
//     bFlexualYieldBrace = rData.bFlexualYieldBrace;   
		dRCRectAlpha       = rData.dRCRectAlpha      ;
		dRCCirAlpha        = rData.dRCCirAlpha       ;
		dSteelCirAlpha     = rData.dSteelCirAlpha    ;
		dSteelRectAlpha    = rData.dSteelRectAlpha   ;   
		dSteelEtcAlpha     = rData.dSteelEtcAlpha    ;
		dFrameStfn         = 1.;
		dSteelStfn         = 1.;
	}
	void Convert805(T_JDUB_D_805& rData)
	{
		nSteelMU           = rData.nSteelMU          ;
		dCollaStrsExtra    = rData.dCollaStrsExtra   ;
		dCollaSigmaMU      = rData.dCollaSigmaMU     ;
		dCollaMU           = rData.dCollaMU          ;
		dCollaNComp        = rData.dCollaNComp       ;
		dCollaNTens        = rData.dCollaNTens       ;
		bMuBuckling        = rData.bMuBuckling       ;        
		dRCRectAlpha       = rData.dRCRectAlpha      ;
		dRCCirAlpha        = rData.dRCCirAlpha       ;
		dSteelCirAlpha     = rData.dSteelCirAlpha    ;
		dSteelRectAlpha    = rData.dSteelRectAlpha   ;   
		dSteelEtcAlpha     = rData.dSteelEtcAlpha    ;
		dFrameStfn         = rData.dFrameStfn;
		dSteelStfn         = rData.dSteelStfn;
	}
	void Convert800(T_JDUB_D_800& rData)
	{
		nSteelMU           = rData.nSteelMU          ;
		dCollaStrsExtra    = rData.dCollaStrsExtra   ;
		dCollaSigmaMU      = rData.dCollaSigmaMU     ;
		dCollaMU           = rData.dCollaMU          ;
		dCollaNComp        = rData.dCollaNComp       ;
		dCollaNTens        = rData.dCollaNTens       ;
		bMuBuckling        = rData.bMuBuckling       ;        
//     bFlexualYieldBrace = rData.bFlexualYieldBrace;   
		dRCRectAlpha       = rData.dRCRectAlpha      ;
		dRCCirAlpha        = rData.dRCCirAlpha       ;
		dSteelCirAlpha     = rData.dSteelCirAlpha    ;
		dSteelRectAlpha    = rData.dSteelRectAlpha   ;   
		dSteelEtcAlpha     = rData.dSteelEtcAlpha    ;
		dFrameStfn         = rData.dFrameStfn;
		dSteelStfn         = rData.dSteelStfn;
	}

	T_JDUB_D() { Initialize(); };
	T_JDUB_D(T_JDUB_D& src) { *this = src; }
	T_JDUB_D& operator=(const T_JDUB_D& src)
	{   
		//nRCMU              = src.nRCMU             ; 
		nSteelMU           = src.nSteelMU          ;
		dCollaStrsExtra    = src.dCollaStrsExtra   ;
		dCollaSigmaMU      = src.dCollaSigmaMU     ;
		dCollaMU           = src.dCollaMU          ;
		dCollaNComp        = src.dCollaNComp       ;
		dCollaNTens        = src.dCollaNTens       ;
		bMuBuckling        = src.bMuBuckling       ;        
//     bFlexualYieldBrace = src.bFlexualYieldBrace;   
		dRCRectAlpha       = src.dRCRectAlpha      ;
		dRCCirAlpha        = src.dRCCirAlpha       ;
		dSteelCirAlpha     = src.dSteelCirAlpha    ;
		dSteelRectAlpha    = src.dSteelRectAlpha   ;   
		dSteelEtcAlpha     = src.dSteelEtcAlpha    ;
		dFrameStfn         = src.dFrameStfn        ;
		dSteelStfn         = src.dSteelStfn        ;
		return *this;
	}	
};
struct T_JDUB_UDRD_D
{
	T_JDUB_D data;
};
#define HASHSIZEJDUB 1

#define T_JDUS_K unsigned int
struct T_JDUS_D // 내력정보 - 종국내력 - 전단
{  
	//int    nQU           ;
	BOOL   bAlphaM       ; // 전단 걸계용 응력할증 여유도 αM을 고려
	double dAlphaM       ; // 할증률 상한치 
	BOOL   bOrtho        ; // 직교방향 프레임 부재의 전단설계

	double dCollaStrs    ; // 전단용 응력 할증률   
	//int    nRCStrsType   ;
	//double dUserBeamHinge;
	//double dUserBeamEtc  ;
	//double dUserColmHinge;
	//double dUserColmEtc  ;
	int    nQURCBeam     ; // RC 부재 Qu 산정식 : 0=0.053, 1=0.068)    
	//int    nQURCColm     ;     
	//double dRCBeamRP_Beam;
	//double dRCBeamRP_Colm;
	//double dRCColmRP_Beam;
	//double dRCColmRP_Colm;
	//double dBeamAngle    ;
	//double dColmAngle    ;
	//double dColmAxial    ;
	
	void Initialize()
	{
		//nQU            = 0    ; 
		bAlphaM        = FALSE;
		dAlphaM        = 1    ;
		bOrtho         = FALSE;
		dCollaStrs     = 1.1  ;
		//nRCStrsType    = 0    ;
		//dUserBeamHinge = 1.1  ;
		//dUserBeamEtc   = 1.2  ;        
		//dUserColmHinge = 1.1  ;   
		//dUserColmEtc   = 1.25 ;
		nQURCBeam      = 1    ;
		//nQURCColm      = 0    ;
		//dRCBeamRP_Beam = 50   ;
		//dRCBeamRP_Colm = 50   ;
		//dRCColmRP_Beam = 50   ;
		//dRCColmRP_Colm = 50   ;
		//dBeamAngle     = 1.5  ;
		//dColmAngle     = 1.5  ;
		//dColmAxial     = 1.5  ;
	}                    
	
	T_JDUS_D() { Initialize(); };
	T_JDUS_D(T_JDUS_D& src) { *this = src; }
	T_JDUS_D& operator=(const T_JDUS_D& src)
	{   
		//nQU            = src.nQU           ; 
		bAlphaM        = src.bAlphaM       ;
		dAlphaM        = src.dAlphaM       ;
		bOrtho         = src.bOrtho        ;    
		dCollaStrs     = src.dCollaStrs    ;
		//nRCStrsType    = src.nRCStrsType   ;
		//dUserBeamHinge = src.dUserBeamHinge;
		//dUserBeamEtc   = src.dUserBeamEtc  ;        
		//dUserColmHinge = src.dUserColmHinge;   
		//dUserColmEtc   = src.dUserColmEtc  ;
		nQURCBeam      = src.nQURCBeam     ;
		//nQURCColm      = src.nQURCColm     ;
		//dRCBeamRP_Beam = src.dRCBeamRP_Beam;
		//dRCBeamRP_Colm = src.dRCBeamRP_Colm;
		//dRCColmRP_Beam = src.dRCColmRP_Beam;
		//dRCColmRP_Colm = src.dRCColmRP_Colm;
		//dBeamAngle     = src.dBeamAngle    ;
		//dColmAngle     = src.dColmAngle    ;
		//dColmAxial     = src.dColmAxial    ;    
		return *this;
	}	
};
struct T_JDUS_UDRD_D
{
	T_JDUS_D data;
};
#define HASHSIZEJDUS 1

#define T_JDAC_K unsigned int
struct T_JDAC_D // 내력정보 - 해석제어
{  
	int      nIncrementMethod; // Increment Method : 0=Load, 1=Displacement
	int      nStepCtrlOption ; // Stepping Control : 0=Auto, 1=Equal
	int      nDispCtrlOption ; // Displacement : 0=Global, 1=Master Node
	double   dOverallMaxDisp ; // Max Translational Displacement 

	double   dLoad_Pos_X     ; // LC Scale Factor-정가력-X방향
	double   dLoad_Pos_Y     ; //                       -Y방향
	double   dLoad_Neg_X     ; //                -부가력-X방향 
	double   dLoad_Neg_Y     ; //                       -Y방향

	double   dStiff_RC       ; // Default Stiff Reduction Ratio-RC
	double   dStiff_STL      ; //                              -STL 

	double   dDisp_Pos_X     ; // Max Disp-정가력-X방향
	double   dDisp_Pos_Y     ; //                -Y방향
	double   dDisp_Neg_X     ; //         -부가력-X방향
	double   dDisp_Neg_Y     ; //                -Y방향

	T_NODE_K MasterNode      ; // Master Node Key
	CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad; // Initial LC
	int      nOutputOption   ; // 해석결과출력옵션 : 0=최종스텝, 1=전체스텝
	void Initialize()
	{
		nIncrementMethod = 0    ;
		nStepCtrlOption  = 0    ;
		nDispCtrlOption  = 0    ;
		dOverallMaxDisp  = 0.0  ;  
		dLoad_Pos_X      = 5.0  ;
		dLoad_Pos_Y      = 5.0  ;
		dLoad_Neg_X      = 5.0  ;
		dLoad_Neg_Y      = 5.0  ;
		dStiff_RC        = 1000.0;
		dStiff_STL       = 1000.0;  
		dDisp_Pos_X      = 0.0  ;
		dDisp_Pos_Y      = 0.0  ;
		dDisp_Neg_X      = 0.0  ;
		dDisp_Neg_Y      = 0.0  ;  
		MasterNode       = 0    ; 
		aInitLoad.RemoveAll();
		nOutputOption    = 1    ;
	}                    
	
	T_JDAC_D() { Initialize(); };
	T_JDAC_D(T_JDAC_D& src) { *this = src; }
	T_JDAC_D& operator=(const T_JDAC_D& src)
	{   
		nIncrementMethod = src.nIncrementMethod;
		nStepCtrlOption  = src.nStepCtrlOption ;
		nDispCtrlOption  = src.nDispCtrlOption ;
		dOverallMaxDisp  = src.dOverallMaxDisp ;  
		dLoad_Pos_X      = src.dLoad_Pos_X     ;
		dLoad_Pos_Y      = src.dLoad_Pos_Y     ;
		dLoad_Neg_X      = src.dLoad_Neg_X     ;
		dLoad_Neg_Y      = src.dLoad_Neg_Y     ;
		dStiff_RC        = src.dStiff_RC       ;
		dStiff_STL       = src.dStiff_STL      ;
		dDisp_Pos_X      = src.dDisp_Pos_X     ;
		dDisp_Pos_Y      = src.dDisp_Pos_Y     ;
		dDisp_Neg_X      = src.dDisp_Neg_X     ;
		dDisp_Neg_Y      = src.dDisp_Neg_Y     ;  
		MasterNode       = src.MasterNode      ; 
		aInitLoad.Copy(src.aInitLoad);
		nOutputOption    = src.nOutputOption   ;
		return *this;
	}	
};
struct T_JDAC_UDRD_D
{
	T_JDAC_D data;
};
struct T_JDAC_UDRD_D_RW
{
	int      nIncrementMethod; 
	int      nStepCtrlOption ; 
	int      nDispCtrlOption ; 
	double   dOverallMaxDisp ; 
	double   dLoad_Pos_X     ; 
	double   dLoad_Pos_Y     ; 
	double   dLoad_Neg_X     ; 
	double   dLoad_Neg_Y     ; 
	double   dStiff_RC       ; 
	double   dStiff_STL      ; 
	double   dDisp_Pos_X     ; 
	double   dDisp_Pos_Y     ; 
	double   dDisp_Neg_X     ; 
	double   dDisp_Neg_Y     ; 
	T_NODE_K MasterNode      ; 
	char aSize_for_aInitLoad[20];  //CArray<T_PVLC_BASE, T_PVLC_BASE&> aInitLoad; // 잘못 들어간 CArray 를 하위호환성을 위해 크기만 맞춰줌(32bit에서 CArray = 20bytes)
	int      nCountInitLoad  ;     
	int      nOutputOption   ;
	void GetJdac(T_JDAC_D& rData)
	{
		rData.nIncrementMethod = nIncrementMethod;
		rData.nStepCtrlOption  = nStepCtrlOption ;
		rData.nDispCtrlOption  = nDispCtrlOption ;
		rData.dOverallMaxDisp  = dOverallMaxDisp ;  
		rData.dLoad_Pos_X      = dLoad_Pos_X     ;
		rData.dLoad_Pos_Y      = dLoad_Pos_Y     ;
		rData.dLoad_Neg_X      = dLoad_Neg_X     ;
		rData.dLoad_Neg_Y      = dLoad_Neg_Y     ;
		rData.dStiff_RC        = dStiff_RC       ;
		rData.dStiff_STL       = dStiff_STL      ;
		rData.dDisp_Pos_X      = dDisp_Pos_X     ;
		rData.dDisp_Pos_Y      = dDisp_Pos_Y     ;
		rData.dDisp_Neg_X      = dDisp_Neg_X     ;
		rData.dDisp_Neg_Y      = dDisp_Neg_Y     ;  
		rData.MasterNode       = MasterNode      ; 
		//rData.aInitLoad.Copy(aInitLoad);    
		rData.nOutputOption    = nOutputOption   ;
	}
	void SetJdac(T_JDAC_D& rData)
	{
		nIncrementMethod = rData.nIncrementMethod;
		nStepCtrlOption  = rData.nStepCtrlOption ;
		nDispCtrlOption  = rData.nDispCtrlOption ;
		dOverallMaxDisp  = rData.dOverallMaxDisp ;  
		dLoad_Pos_X      = rData.dLoad_Pos_X     ;
		dLoad_Pos_Y      = rData.dLoad_Pos_Y     ;
		dLoad_Neg_X      = rData.dLoad_Neg_X     ;
		dLoad_Neg_Y      = rData.dLoad_Neg_Y     ;
		dStiff_RC        = rData.dStiff_RC       ;
		dStiff_STL       = rData.dStiff_STL      ;
		dDisp_Pos_X      = rData.dDisp_Pos_X     ;
		dDisp_Pos_Y      = rData.dDisp_Pos_Y     ;
		dDisp_Neg_X      = rData.dDisp_Neg_X     ;
		dDisp_Neg_Y      = rData.dDisp_Neg_Y     ;  
		MasterNode       = rData.MasterNode      ; 
		nCountInitLoad   = (int)rData.aInitLoad.GetSize();
		nOutputOption    = rData.nOutputOption   ;
	}
};
struct T_JDAC_UNIT
{
	T_JDAC_UNIT()
	{
		dOverallMaxDisp=D_UNITSYS_BASE_LENGTH; 
		dDisp_Pos_X    =D_UNITSYS_BASE_LENGTH; 
		dDisp_Pos_Y    =D_UNITSYS_BASE_LENGTH; 
		dDisp_Neg_X    =D_UNITSYS_BASE_LENGTH; 
		dDisp_Neg_Y    =D_UNITSYS_BASE_LENGTH; 
	}
	int dOverallMaxDisp;  
	int dDisp_Pos_X    ;  
	int dDisp_Pos_Y    ;  
	int dDisp_Neg_X    ;  
	int dDisp_Neg_Y    ;  
};
#define HASHSIZEJDAC 1

#define T_JDDS_K unsigned int
struct T_JDDS_D_800 // 내력정보 - Ds
{  
	// [0]:X방향, [1]:Y방향
	
	// 0=Method-1, 1=Method-2
	int    nBeamShear_RC[2]; // RC 보/기둥-전단 
	int    nBeamAxis_RC [2]; //           -축압축 
	int    nBeamShear_ST[2]; // STL 보/기둥-전단 
	int    nBeamAxis_ST [2]; //            -축압축
	int    nWallShear_RC[2]; // RC 벽-전단  
	int    nWallAxis_RC [2]; //      -축압축
	int    nBraceAxis_ST[2]; // STL Brace-축압축  
	double dCentAngle   [2]; // 층간변형각-중심
	double dMaxAngle    [2]; //           -최대
	double dAvgAngle    [2]; //           -평균
	int    nMaxStepPos  [2]; // 전체스텝수-정가력
	int    nMaxStepNeg  [2]; //            부가력
	int    nStopStepPos [2]; // 중지스텝수-정가력
	int    nStopStepNeg [2]; //            부가력
	BOOL   bStopStep    [2]; // 중지 유무
};

struct T_JDDS_D // 내력정보 - Ds
{  
	// [0]:X방향, [1]:Y방향

	// 0=Method-1, 1=Method-2
	int    nBeamShear_RC[2]; // RC 보/기둥-전단 
	int    nBeamAxis_RC [2]; //           -축압축 
	int    nBeamShear_ST[2]; // STL 보/기둥-전단 
	int    nBeamAxis_ST [2]; //            -축압축

	//int    nColmShear_RC[2];  
	//int    nColmAxis_RC [2];   
	//int    nColmShear_ST[2];  
	//int    nColmAxis_ST [2]; 
	int    nWallShear_RC[2]; // RC 벽-전단  
	int    nWallAxis_RC [2]; //      -축압축
	int    nBraceAxis_ST[2]; // STL Brace-축압축  
	double dCentAngle   [2]; // 층간변형각-중심
	double dMaxAngle    [2]; //           -최대
	double dAvgAngle    [2]; //           -평균
	int    nMaxStepPos  [2]; // 전체스텝수-정가력
	int    nMaxStepNeg  [2]; //            부가력
	int    nStopStepPos [2]; // 중지스텝수-정가력
	int    nStopStepNeg [2]; //            부가력
	BOOL   bStopStep    [2]; // 중지 유무
	int    nPointLift   [2]; // 지점 - 떠오름                   // 8.0.5
	int    nPointAxial  [2]; //        압괴                     // 8.0.5
	int    nUpLifting      ; // 떠오름 고려 : 0=한다, 1=안한다  // 8.0.5
	
	void Initialize()
	{
		for (int i=0; i<2; i++)
		{
			nBeamShear_RC[i] = 0 ;
			nBeamAxis_RC [i] = 0 ;
			nBeamShear_ST[i] = 0 ;
			nBeamAxis_ST [i] = 0 ;
			nWallShear_RC[i] = 0 ;
			nWallAxis_RC [i] = 0 ;
			nBraceAxis_ST[i] = 0 ;
			dCentAngle   [i] = 50.;
			dMaxAngle    [i] = 50.;
			dAvgAngle    [i] = 50.;
			nMaxStepPos  [i] = 50;
			nMaxStepNeg  [i] = 50;
			nStopStepPos [i] = 50;
			nStopStepNeg [i] = 50;
			bStopStep    [i] = FALSE;
			nPointLift   [i] = 0;
			nPointAxial  [i] = 0;
		}
		nUpLifting = 1;
	}

	void Convert800(T_JDDS_D_800& rData)
	{
		for (int i=0; i<2; i++)
		{
			nBeamShear_RC[i] = rData.nBeamShear_RC[i];
			nBeamAxis_RC [i] = rData.nBeamAxis_RC [i];
			nBeamShear_ST[i] = rData.nBeamShear_ST[i];
			nBeamAxis_ST [i] = rData.nBeamAxis_ST [i];
			nWallShear_RC[i] = rData.nWallShear_RC[i];
			nWallAxis_RC [i] = rData.nWallAxis_RC [i];
			nBraceAxis_ST[i] = rData.nBraceAxis_ST[i];
			dCentAngle   [i] = rData.dCentAngle   [i];
			dMaxAngle    [i] = rData.dMaxAngle    [i];
			dAvgAngle    [i] = rData.dAvgAngle    [i];
			nMaxStepPos  [i] = rData.nMaxStepPos  [i];
			nMaxStepNeg  [i] = rData.nMaxStepNeg  [i];
			nStopStepPos [i] = rData.nStopStepPos [i];
			nStopStepNeg [i] = rData.nStopStepNeg [i];
			bStopStep    [i] = rData.bStopStep    [i];
			nPointLift   [i] = 0;
			nPointAxial  [i] = 0;
		}
		nUpLifting = 1;
	}
};
struct T_JDDS_UDRD_D
{
	T_JDDS_D data;
};
#define HASHSIZEJDDS 1

#define T_JDHL_K unsigned int
struct T_JDHL_D_800 // 내력정보 - 보유수평내력
{
	BOOL   bUseDS; // Ds 산정시 보유수평내력 사용
	
	int    nBeamShear_RC[2];
	int    nBeamAxis_RC [2];
	int    nBeamShear_ST[2];
	int    nBeamAxis_ST [2];
	int    nWallShear_RC[2];
	int    nWallAxis_RC [2];
	int    nBraceAxis_ST[2];
	double dCentAngle   [2];
	double dMaxAngle    [2];
	double dAvgAngle    [2];
	int    nMaxStepPos  [2];
	int    nMaxStepNeg  [2];
	int    nStopStepPos [2];
	int    nStopStepNeg [2];
	BOOL   bStopStep    [2];
};

struct T_JDHL_D // 내력정보 - 보유수평내력
{  
	BOOL   bUseDS; // Ds 산정시 보유수평내력 사용

	int    nBeamShear_RC[2]; 
	int    nBeamAxis_RC [2];  
	int    nBeamShear_ST[2]; 
	int    nBeamAxis_ST [2];
	//int    nColmShear_RC[2];  
	//int    nColmAxis_RC [2]; 
	//int    nColmShear_ST[2];  
	//int    nColmAxis_ST [2]; 
	int    nWallShear_RC[2];   
	int    nWallAxis_RC [2];
	int    nBraceAxis_ST[2];   
	double dCentAngle   [2];
	double dMaxAngle    [2];
	double dAvgAngle    [2];
	int    nMaxStepPos  [2];
	int    nMaxStepNeg  [2];
	int    nStopStepPos [2];
	int    nStopStepNeg [2];
	BOOL   bStopStep    [2];
	int    nPointLift   [2]; // 지점 - 떠오름                   // 8.0.5
	int    nPointAxial  [2]; //        압괴                     // 8.0.5
	int    nUpLifting      ; // 떠오름 고려 : 0=한다, 1=안한다  // 8.0.5
	
	void Initialize()
	{
		bUseDS = FALSE;
		for (int i=0; i<2; i++)
		{
			nBeamShear_RC[i] = 0 ;
			nBeamAxis_RC [i] = 0 ;
			nBeamShear_ST[i] = 0 ;
			nBeamAxis_ST [i] = 0 ;
			nWallShear_RC[i] = 0 ;
			nWallAxis_RC [i] = 0 ;
			nBraceAxis_ST[i] = 0 ;
			dCentAngle   [i] = 100.;
			dMaxAngle    [i] = 100.;
			dAvgAngle    [i] = 100.;
			nMaxStepPos  [i] = 50;
			nMaxStepNeg  [i] = 50;
			nStopStepPos [i] = 50;
			nStopStepNeg [i] = 50;
			bStopStep    [i] = FALSE;
			nPointLift   [i] = 0;
			nPointAxial  [i] = 0;
		}
		nUpLifting = 1;
	}
	
	void Convert800(T_JDHL_D_800& rData)
	{
		bUseDS = rData.bUseDS;
		for (int i=0; i<2; i++)
		{
			nBeamShear_RC[i] = rData.nBeamShear_RC[i];
			nBeamAxis_RC [i] = rData.nBeamAxis_RC [i];
			nBeamShear_ST[i] = rData.nBeamShear_ST[i];
			nBeamAxis_ST [i] = rData.nBeamAxis_ST [i];
			nWallShear_RC[i] = rData.nWallShear_RC[i];
			nWallAxis_RC [i] = rData.nWallAxis_RC [i];
			nBraceAxis_ST[i] = rData.nBraceAxis_ST[i];
			dCentAngle   [i] = rData.dCentAngle   [i];
			dMaxAngle    [i] = rData.dMaxAngle    [i];
			dAvgAngle    [i] = rData.dAvgAngle    [i];
			nMaxStepPos  [i] = rData.nMaxStepPos  [i];
			nMaxStepNeg  [i] = rData.nMaxStepNeg  [i];
			nStopStepPos [i] = rData.nStopStepPos [i];
			nStopStepNeg [i] = rData.nStopStepNeg [i];
			bStopStep    [i] = rData.bStopStep    [i];
			nPointLift   [i] = 0;
			nPointAxial  [i] = 0;
		}
		nUpLifting = 1;
	}

	void SetFromDs(T_JDDS_D& JddsD)
	{
		for (int i=0; i<2; i++)
		{
			nBeamShear_RC[i] = JddsD.nBeamShear_RC[i];
			nBeamAxis_RC [i] = JddsD.nBeamAxis_RC [i];
			nBeamShear_ST[i] = JddsD.nBeamShear_ST[i];
			nBeamAxis_ST [i] = JddsD.nBeamAxis_ST [i];
			nWallShear_RC[i] = JddsD.nWallShear_RC[i];
			nWallAxis_RC [i] = JddsD.nWallAxis_RC [i];
			nBraceAxis_ST[i] = JddsD.nBraceAxis_ST[i];
			dCentAngle   [i] = JddsD.dCentAngle   [i];
			dMaxAngle    [i] = JddsD.dMaxAngle    [i];
			dAvgAngle    [i] = JddsD.dAvgAngle    [i];
			nMaxStepPos  [i] = JddsD.nMaxStepPos  [i];
			nMaxStepNeg  [i] = JddsD.nMaxStepNeg  [i];
			nStopStepPos [i] = JddsD.nStopStepPos [i];
			nStopStepNeg [i] = JddsD.nStopStepNeg [i];
			bStopStep    [i] = JddsD.bStopStep    [i];
			nPointLift   [i] = JddsD.nPointLift   [i];
			nPointAxial  [i] = JddsD.nPointAxial  [i];
		}

		nUpLifting = JddsD.nUpLifting;
	}
};
struct T_JDHL_UDRD_D
{
	T_JDHL_D data;
};
#define HASHSIZEJDHL 1

#define T_JDEL_K unsigned int
struct T_JDEL_D // 내력정보 - 부재종별 판정
{ 
	BOOL bNonColla   ; // 미붕괴 부재 여유도에 의한 파괴모드 판정
	//BOOL bCosiderQD  ;
	//BOOL bCosiderPT  ;
	BOOL bCosiderD   ; // D부재의 고려 
	BOOL bOrtho      ; // 직교방향 프레임 고려  
	int  nDecisionOpt; // 보,기둥 종별 결정 : 0=기둥, 1=힌지

	//int  nTauu       ;
	int  nNGElem     ; // 횡보강검토 : 0=고려하지 않음, 1=D로 취급 
	
	void Initialize()
	{
		bNonColla    = TRUE ;
		//bCosiderQD   = TRUE ;
		//bCosiderPT   = TRUE ;
		bCosiderD    = TRUE ;  
		bOrtho       = TRUE ;    
		nDecisionOpt = 0;
		//nTauu        = 1    ;
		nNGElem      = 1    ;    
	}                    
	
	T_JDEL_D() { Initialize(); };
	T_JDEL_D(T_JDEL_D& src) { *this = src; }
	T_JDEL_D& operator=(const T_JDEL_D& src)
	{   
		bNonColla    = src.bNonColla   ; 
		//bCosiderQD   = src.bCosiderQD  ;
		//bCosiderPT   = src.bCosiderPT  ;
		bCosiderD    = src.bCosiderD   ;
		bOrtho       = src.bOrtho      ;    
		//nTauu        = src.nTauu       ;
		nNGElem      = src.nNGElem     ;    
		nDecisionOpt = src.nDecisionOpt;
		return *this;
	}	
};
struct T_JDEL_UDRD_D
{
	T_JDEL_D data;
};
#define HASHSIZEJDEL 1

#define T_JDBD_K unsigned int


struct T_JDBD_D_805 // 일괄설계수행
{
	BOOL bFirstDesign ; // 해석 & 1차 설계
	BOOL bSecondDesign; // 2차 설계 
	BOOL bSectCalc    ; // 단면선정가능부재만 적용
	BOOL bAll         ;
	BOOL bPosX        ;
	BOOL bNegX        ;
	BOOL bPosY        ;
	BOOL bNegY        ;
	int  nAutoRebar   ; // 자동배근 : 0=전체부재, 1=OK부재, 2=적용하지않음
	
	void Initialize()
	{    
		bFirstDesign  = TRUE; 
		bSecondDesign = TRUE; 
		bSectCalc     = TRUE; 
		bAll          = TRUE;
		bPosX         = TRUE;
		bNegX         = TRUE;
		bPosY         = TRUE;
		bNegY         = TRUE;
		nAutoRebar    = 0   ; 
	}
	T_JDBD_D_805() { Initialize(); };
	T_JDBD_D_805(T_JDBD_D_805& src) { *this = src; }
//   T_JDBD_D_805& operator=(const T_JDBD_D& src)
//   {                                 
//     bFirstDesign  = src.bFirstDesign ; 
//     bSecondDesign = src.bSecondDesign; 
//     bSectCalc     = src.bSectCalc    ; 
//     bAll          = src.bAll         ;
//     bPosX         = src.bPosX        ;
//     bNegX         = src.bNegX        ;
//     bPosY         = src.bPosY        ;
//     bNegY         = src.bNegY        ;
//     nAutoRebar    = src.nAutoRebar   ;        
//     return *this;
//   }	
};

struct T_JDBD_D // 일괄설계수행
{
	BOOL bFirstDesign ; // 해석 & 1차 설계
	BOOL bSecondDesign; // 2차 설계 
	BOOL bSectCalc    ; // 단면선정가능부재만 적용
	BOOL bFoundDesign ; // 기초 모듈을 이용한 기초 설계
	BOOL bAll         ;
	BOOL bPosX        ;
	BOOL bNegX        ;
	BOOL bPosY        ;
	BOOL bNegY        ;
	int  nAutoRebar   ; // 자동배근 : 0=전체부재, 1=OK부재, 2=적용하지않음

	void Initialize()
	{    
		bFirstDesign  = TRUE; 
		bSecondDesign = TRUE; 
		bSectCalc     = TRUE;
		bFoundDesign  = FALSE;  
		bAll          = TRUE;
		bPosX         = TRUE;
		bNegX         = TRUE;
		bPosY         = TRUE;
		bNegY         = TRUE;
		nAutoRebar    = 0   ; 
	}
	T_JDBD_D() { Initialize(); };
	T_JDBD_D(T_JDBD_D& src) { *this = src; }
	T_JDBD_D& operator=(const T_JDBD_D& src)
	{                                 
		bFirstDesign  = src.bFirstDesign ; 
		bSecondDesign = src.bSecondDesign; 
		bSectCalc     = src.bSectCalc    ; 
		bFoundDesign  = src.bFoundDesign ;
		bAll          = src.bAll         ;
		bPosX         = src.bPosX        ;
		bNegX         = src.bNegX        ;
		bPosY         = src.bPosY        ;
		bNegY         = src.bNegY        ;
		nAutoRebar    = src.nAutoRebar   ;        
		return *this;
	}	
	void Convert805(T_JDBD_D_805& rData)
	{
		bFirstDesign  = rData.bFirstDesign ; 
		bSecondDesign = rData.bSecondDesign; 
		bSectCalc     = rData.bSectCalc    ; 
		bFoundDesign  = TRUE;
		bAll          = rData.bAll         ;
		bPosX         = rData.bPosX        ;
		bNegX         = rData.bNegX        ;
		bPosY         = rData.bPosY        ;
		bNegY         = rData.bNegY        ;
		nAutoRebar    = rData.nAutoRebar   ;   
	}
};

struct T_JDBD_UDRD_D
{
	T_JDBD_D data;
};
#define HASHSIZEJDBD 1

#define T_BPOP_K unsigned int
// 일본 2차설계 S조노출형 주각 옵션
struct T_BPOP_D
{
	int  nCheck          ; // 앵커볼트 검토식 0: 강철 구조설계 규준(6.5)식, 1: 강관 구조설계 시공지침(4.89)식
	BOOL bConcCorner     ; // 입상부 벗겨져 떨어짐 검토여부
	BOOL bConcComp       ; // 입상부 압괴 검토여부
	BOOL bBoltFixComp    ; // 앵커볼트 정착부의 압괴 검토여부
	BOOL bBoltSideComp   ; // 앵커볼트 주변부의 압렬 검토여부
	BOOL bShearBoltSide  ; // 전단에 의한 앵커볼트 주변의 압괴 검토여부
	BOOL bShearConcCorner; // 전단에 의한 입상부의 벗겨져 떨어짐 검토여부
	BOOL bUltStressChk   ; // 종국시 응력에 의한 단면검토여부
	BOOL bPlateBroken    ; // 베이스 플래이트의 파단 검토여부
	void Initialize()
	{
		nCheck           = 0;
		bConcCorner      = FALSE;
		bConcComp        = FALSE;
		bBoltFixComp     = FALSE;
		bBoltSideComp    = FALSE;
		bShearBoltSide   = FALSE;
		bShearConcCorner = FALSE;
		bUltStressChk    = FALSE;
		bPlateBroken     = FALSE;
	}
	T_BPOP_D() { Initialize(); };
	T_BPOP_D(T_BPOP_D& src) { *this = src; }
	T_BPOP_D& operator=(const T_BPOP_D& src)
	{   
		nCheck           = src.nCheck          ;
		bConcCorner      = src.bConcCorner     ;      
		bConcComp        = src.bConcComp       ;        
		bBoltFixComp     = src.bBoltFixComp    ;     
		bBoltSideComp    = src.bBoltSideComp   ;    
		bShearBoltSide   = src.bShearBoltSide  ;   
		bShearConcCorner = src.bShearConcCorner; 
		bUltStressChk    = src.bUltStressChk   ;    
	  bPlateBroken     = src.bPlateBroken    ;     
		return *this;
	}	
};
struct T_BPOP_UDRD_D
{
	T_BPOP_D data;
};
#define HASHSIZEBPOP 1

#define T_BPLT_K unsigned int
// 일본 2차설계 주각정보(Elem.에 연계) PMS:4215
struct T_BPLT_D
{	
	int      nType       ; // 주각의 형식 (BPLT_TYPE_XXX)
	T_ELEM_K ElemK_Rigid ; // 강역구간으로 생성한 Elem Key(강역구간으로 연결될 Elem가 없을 경우 0)
	double   dRigidLength; // 강역구간의 길이

	// 주각형식이 DB형식일때 (BPLT_TYPE_HibaseEco이후 모두)
	CString  strDBName   ; // 주각형식의 DB이름(nType이 DB형식일 경우에만)
	int      nEccType    ; // 편심형식 (0:편심無, 1:왼쪽, 2:오른쪽, 3:위쪽, 4:아래쪽)
	
	// 주각형식이 사용자 정의 일때 (nType이 S조 노출형일때)
	double  dBP_Dy  ; // BasePlate의 y방향 길이(Unit:L)
	double  dBP_Dz  ; // BasePlate의 z방향 길이(Unit:L)
	double  dBP_t   ; // BasePlate의 두께(Unit:L)
	double  dBP_dty ; // 앵커볼트 위치로 부터 BasePlate의 가장자리 끝까지의 y방향 거리(Unit:L)
	double  dBP_dtz ; // 앵커볼트 위치로 부터 BasePlate의 가장자리 끝까지의 z방향 거리(Unit:L)
	double  dBP_Fy  ; // BasePlate의 항복강도(0일때에는 기둥의 값 사용)(Unit:F/L^2)
	double  dBP_Fu  ; // BasePlate의 극한인장강도(Unit:F/L^2)
	double  dBP_HDia; // 앵커볼트 구멍 직경(Unit:L)

	CString strAB_MatlName; // 앵커볼트 재료이름
	CString strAB_DBName  ; // 앵커볼트 DB이름
	int     nAB_yNum      ; // 앵커볼트 y방향 갯수 (최소 2이상 - 전체 앵커개수  dAB_yNum*2 + (dAB_zNum-2)*2
	int     nAB_zNum      ; // 앵커볼트 z방향 갯수 (최소 2이상)
	double  dAB_FixL      ; // 앵커볼트의 길이(Unit:L)
	double  dAB_FixA      ; // 앵커볼트 정착철물의 면적(Unit:L^2)
									
	double  dAC_By; // 기둥주의 y방향 길이(Unit:L)
	double  dAC_Bz; // 기둥주의 z방향 길이(Unit:L)
	double  dAC_h ; // 기둥주의 첫시작높이(앵커볼트의 길이 이하)(Unit:L) ???
 
	double  dProjArea; // 투영면적(앵커볼트 주변의 압괴를 검토할때의 투영면적)(Unit:L^2)
	void Initialize()
	{
		nType        = 0;
		strDBName    = "";
		nEccType     = 0;
		ElemK_Rigid  = 0;
		dRigidLength = 0.0;
		dBP_Dy       = 0.0;
		dBP_Dz       = 0.0;
		dBP_t        = 0.0;
		dBP_dty      = 0.0;
		dBP_dtz      = 0.0;
		dBP_Fy       = 0.0;
		dBP_Fu       = 0.0;
		dBP_HDia     = 0.0;
		strAB_MatlName = "";
		strAB_DBName = "";
		nAB_yNum     = 0;
		nAB_zNum     = 0;
		dAB_FixL     = 0.0; 
		dAB_FixA     = 0.0; 
		dAC_By       = 0.0;
		dAC_Bz       = 0.0;
		dAC_h        = 0.0;
		dProjArea    = 0.0;
	}
	T_BPLT_D() { Initialize(); };
	T_BPLT_D(T_BPLT_D& src) { *this = src; }
	T_BPLT_D& operator=(const T_BPLT_D& src)
	{   
		nType          = src.nType         ;
		strDBName      = src.strDBName     ;
		nEccType       = src.nEccType      ;
		ElemK_Rigid    = src.ElemK_Rigid   ;
		dRigidLength   = src.dRigidLength  ;
		dBP_Dy         = src.dBP_Dy        ;
		dBP_Dz         = src.dBP_Dz        ;
		dBP_t          = src.dBP_t         ;
		dBP_dty        = src.dBP_dty       ;
		dBP_dtz        = src.dBP_dtz       ;
		dBP_Fy         = src.dBP_Fy        ;
		dBP_Fu         = src.dBP_Fu        ;
		dBP_HDia       = src.dBP_HDia      ;
		strAB_MatlName = src.strAB_MatlName;
		strAB_DBName   = src.strAB_DBName  ;
		nAB_yNum       = src.nAB_yNum      ;
		nAB_zNum       = src.nAB_zNum      ;
		dAB_FixL       = src.dAB_FixL      ; 
		dAB_FixA       = src.dAB_FixA      ; 
		dAC_By         = src.dAC_By        ;
		dAC_Bz         = src.dAC_Bz        ;
		dAC_h          = src.dAC_h         ;
		dProjArea      = src.dProjArea     ;   
		return *this;
	}	
};
struct T_BPLT_D_CH
{
	int      nType        ; 
	char     strDBName[80]; 
	int      nEccType     ; 
	T_ELEM_K ElemK_Rigid  ; 
	double   dRigidLength ;   
												
	double dBP_Dy  ; 
	double dBP_Dz  ; 
	double dBP_t   ; 
	double dBP_dty ; 
	double dBP_dtz ; 
	double dBP_Fy  ; 
	double dBP_Fu  ; 
	double dBP_HDia; 
	
	char   strAB_MatlName[80]; 
	char   strAB_DBName  [80]; 
	int    nAB_yNum; 
	int    nAB_zNum; 
	double dAB_FixL; 
	double dAB_FixA; 
								 
	double  dAC_By; 
	double  dAC_Bz; 
	double  dAC_h ; 
	
	double  dProjArea; 

	void ConvertToString(T_BPLT_D& rData)
	{
		rData.nType        = nType       ; 
		rData.nEccType     = nEccType    ; 
		rData.ElemK_Rigid  = ElemK_Rigid ; 
		rData.dRigidLength = dRigidLength;       
		rData.dBP_Dy       = dBP_Dy      ; 
		rData.dBP_Dz       = dBP_Dz      ; 
		rData.dBP_t        = dBP_t       ; 
		rData.dBP_dty      = dBP_dty     ; 
		rData.dBP_dtz      = dBP_dtz     ; 
		rData.dBP_Fy       = dBP_Fy      ; 
		rData.dBP_Fu       = dBP_Fu      ; 
		rData.dBP_HDia     = dBP_HDia    ;   
		rData.nAB_yNum     = nAB_yNum    ; 
		rData.nAB_zNum     = nAB_zNum    ; 
		rData.dAB_FixL     = dAB_FixL    ; 
		rData.dAB_FixA     = dAB_FixA    ;     
		rData.dAC_By       = dAC_By      ; 
		rData.dAC_Bz       = dAC_Bz      ; 
		rData.dAC_h        = dAC_h       ; 
		rData.dProjArea    = dProjArea   ; 
		ConvertCharStr(strDBName     , rData.strDBName     , sizeof(strDBName     ));
		ConvertCharStr(strAB_MatlName, rData.strAB_MatlName, sizeof(strAB_MatlName));
		ConvertCharStr(strAB_DBName  , rData.strAB_DBName  , sizeof(strAB_DBName  ));
	}
	void ConvertToChar(T_BPLT_D& rData)
	{
		nType        = rData.nType       ; 
		nEccType     = rData.nEccType    ; 
		ElemK_Rigid  = rData.ElemK_Rigid ; 
		dRigidLength = rData.dRigidLength;       
		dBP_Dy       = rData.dBP_Dy      ; 
		dBP_Dz       = rData.dBP_Dz      ; 
		dBP_t        = rData.dBP_t       ; 
		dBP_dty      = rData.dBP_dty     ; 
		dBP_dtz      = rData.dBP_dtz     ; 
		dBP_Fy       = rData.dBP_Fy      ; 
		dBP_Fu       = rData.dBP_Fu      ; 
		dBP_HDia     = rData.dBP_HDia    ;   
		nAB_yNum     = rData.nAB_yNum    ; 
		nAB_zNum     = rData.nAB_zNum    ; 
		dAB_FixL     = rData.dAB_FixL    ; 
		dAB_FixA     = rData.dAB_FixA    ;     
		dAC_By       = rData.dAC_By      ; 
		dAC_Bz       = rData.dAC_Bz      ; 
		dAC_h        = rData.dAC_h       ; 
	  dProjArea    = rData.dProjArea   ; 
		ConvertStrChar(rData.strDBName     , strDBName     , sizeof(strDBName     ));
		ConvertStrChar(rData.strAB_MatlName, strAB_MatlName, sizeof(strAB_MatlName));
		ConvertStrChar(rData.strAB_DBName  , strAB_DBName  , sizeof(strAB_DBName  ));    
	}
};
struct T_BPLT_UDRD_D
{
	T_BPLT_K key;
	T_BPLT_D data;
};
struct T_BPLT_UDRD_D_CH
{
	T_BPLT_K key;
	T_BPLT_D_CH data;
};
struct T_BPLT_UNIT
{
	T_BPLT_UNIT()
	{
		dRigidLength = D_UNITSYS_BASE_LENGTH; 
		dBP_Dy       = D_UNITSYS_BASE_LENGTH; 
		dBP_Dz       = D_UNITSYS_BASE_LENGTH; 
		dBP_t        = D_UNITSYS_BASE_LENGTH; 
		dBP_dty      = D_UNITSYS_BASE_LENGTH; 
		dBP_dtz      = D_UNITSYS_BASE_LENGTH; 
		dBP_Fy       = D_UNITSYS_BASE_ELAST ; 
		dBP_Fu       = D_UNITSYS_BASE_ELAST ; 
	  dBP_HDia     = D_UNITSYS_BASE_LENGTH; 
		dAB_FixL     = D_UNITSYS_BASE_LENGTH; 
		dAB_FixA     = D_UNITSYS_BASE_AREA  ; 
		dAC_By       = D_UNITSYS_BASE_LENGTH; 
		dAC_Bz       = D_UNITSYS_BASE_LENGTH; 
		dAC_h        = D_UNITSYS_BASE_LENGTH; 
	  dProjArea    = D_UNITSYS_BASE_AREA  ; 
	}
	int dRigidLength; 
	int dBP_Dy      ; 
	int dBP_Dz      ; 
	int dBP_t       ; 
	int dBP_dty     ; 
	int dBP_dtz     ; 
	int dBP_Fy      ; 
	int dBP_Fu      ; 
	int dBP_HDia    ; 
	int dAB_FixL    ; 
	int dAB_FixA    ; 
	int dAC_By      ; 
	int dAC_Bz      ; 
	int dAC_h       ; 
	int dProjArea   ; 
};
#define HASHSIZEBPLT 1001

#define T_JDCF_K unsigned int
struct T_JDCF_D // 냉간성형강의 내력저감  Cold-Formed Steel : CFS (냉간성형강)
{ 
	BOOL bConsider;   // True: 내력저감 고려, False: 미고려
	BOOL bPlastic ;
	
	double a_dRatioBCP[4];   // [0]:내다이어프램, [1]:통다이어프램, [2]:외다이어프램, [3]:기타
	double a_dRatioBCR[4];
	double a_dRatioForce[4];
	
	void Initialize()
	{
		bConsider = TRUE;
		bPlastic  = TRUE;
		
		a_dRatioBCP[0] = 0.85;
		a_dRatioBCP[1] = 0.80;
		a_dRatioBCP[2] = 0.80;
		a_dRatioBCP[3] = 1.00;
		
		a_dRatioBCR[0] = 0.80;
		a_dRatioBCR[1] = 0.75;
		a_dRatioBCR[2] = 0.75;
		a_dRatioBCR[3] = 1.00;
		
		a_dRatioForce[0] = 1.00;
		a_dRatioForce[1] = 1.00;
		a_dRatioForce[2] = 1.00;
		a_dRatioForce[3] = 1.00;
	}
	
	T_JDCF_D() { Initialize(); };
	T_JDCF_D(T_JDCF_D& src) { *this = src; }
	T_JDCF_D& operator=(const T_JDCF_D& src)
	{   
		bConsider = src.bConsider;
		bPlastic  = src.bPlastic;
		for (int i=0; i<4; i++)    
		{
			a_dRatioBCP[i] = src.a_dRatioBCP[i];
			a_dRatioBCR[i] = src.a_dRatioBCR[i];
			a_dRatioForce[i] = src.a_dRatioForce[i];
		}
		 return *this;
	}	
};
struct T_JDCF_UDRD_D
{
	T_JDCF_D data;
};
#define HASHSIZEJDCF 1
//////////////////////////////////////////////////////////////////////////

#define D_JUD_RANK_A 1
#define D_JUD_RANK_B 2
#define D_JUD_RANK_C 3
#define D_JUD_RANK_D 4

#define D_JUD_STRUCT_TYPE_FRAME      1
#define D_JUD_STRUCT_TYPE_WALL       2
#define D_JUD_STRUCT_TYPE_FRAME_WALL 3

#define D_JUD_STRUCT_MATL_TYPE_STL 1
#define D_JUD_STRUCT_MATL_TYPE_CON 2
#define D_JUD_STRUCT_MATL_TYPE_SRC 3

#define D_JUD_HINGE_STATUS_ELASTIC     0
#define D_JUD_HINGE_STATUS_CRACK       1
#define D_JUD_HINGE_STATUS_YIELD       2

#define D_JUD_POSI_I 0
#define D_JUD_POSI_J 1
#define D_JUD_POSI_C 2

#define D_JUD_REMARK_OK 1
#define D_JUD_REMARK_NG 2

#define D_JUD_ROOT_NON     0  // Root 판별불가

#define D_JUD_ROOT_STL_1_1 111
#define D_JUD_ROOT_STL_1_2 112
#define D_JUD_ROOT_STL_2   120
#define D_JUD_ROOT_STL_3   130

#define D_JUD_ROOT_CON_1   210
#define D_JUD_ROOT_CON_2_1 221
#define D_JUD_ROOT_CON_2_2 222
#define D_JUD_ROOT_CON_2_3 223
#define D_JUD_ROOT_CON_3   230

#define D_JUD_ROOT_SRC_1   310
#define D_JUD_ROOT_SRC_2_1 321
#define D_JUD_ROOT_SRC_2_2 322
#define D_JUD_ROOT_SRC_2_3 323
#define D_JUD_ROOT_SRC_3   330

#define _JUD_ELEM_LOAD_KEY unsigned int
union _JUD_ELEM_LOAD_K
{
	_JUD_ELEM_LOAD_KEY KeyMap;
	struct
	{
		unsigned int ElemK : 24; // Element Key.            (0~16,777,216)
		unsigned int PolcK :  8; // Pushover Load Case Key. (0~255)
	}Key;
};

#define  _JUD_STOR_LOAD_KEY unsigned int
union _JUD_STOR_LOAD_K
{
	_JUD_STOR_LOAD_KEY KeyMap;
	struct
	{
		unsigned int StorK : 24;  // Story Key.              (0~16,777,216)
		unsigned int PolcK :  8;  // Pushover Load Case Key. (0~255)
	}Key;
};

// 설계 결과 구조체 (1차설계) ////////////////////////////////////////////////////////////////////////

// 일본 2차설계 - 접합부 1차설계 결과 PMS:4215
struct _JUD_RJNT_D
{// Steel에서 접합부관련 설계 결과

	BOOL   bCheck;
	T_ELEM_K ElemK_Up;   // 上柱 ElemK
	T_ELEM_K ElemK_Down; // 下柱 ElemK
	double ddb[2]; // 대들보 플랜지의 판두께 중심간 거리 (Unit:L)
	double ddc[2]; // 접합부 플랜지의 판두께 중심간 거리 (Unit:L)
	double dtp[2]; // 접합부 패널의 판두께 (Unit:L)
	double dVe[2]; // 접합부 패널의 유효체적 (Unit:L^3)
	double dk[2];  // 전단에 관한 형상계수 
	double dP[2];  // 축하중
	double dn[2];  // n = P/(Fy*Area)
	BOOL   bIsRes[2];// 해당 방향의 결과값 유무([0]:yy에 대한 모메트 검토결과 유무, [1]:zz에 대한 모메트 검토결과 유무)
	double dbML[2]; // 좌측에 붙은 보의 모멘트합(접합각도에 대한 보정 고려)
	double dbMR[2]; // 좌측에 붙은 보의 모멘트합(접합각도에 대한 보정 고려)
	double dcQU[2]; // 상단의 기둥에 대한 전단력(기둥간의 축회전각에 대한 보정 고려)
	double dcQL[2]; // 하단의 기둥에 대한 전단력(기둥간의 축회전각에 대한 보정 고려)
	double dpM[2];  // 해당 접합부의 Panel Moment
	double dpMy[2]; // 해당 접합부의 항복 Panel Moment

	// 최대값을 보관시에만 사용합니다.
	T_LCOM_K LcomK[2]; // 최대값 발생 Lcom Key
	
	void Initialize()
	{
		bCheck     = FALSE;
		ElemK_Up   = 0  ; 
		ElemK_Down = 0  ; 
		for(int i=0 ; i<2 ; i++)
		{
			ddb[i]    = 0.0; 
			ddc[i]    = 0.0; 
			dtp[i]    = 0.0; 
			dVe[i]    = 0.0; 
			dk[i]     = 0.0; 
			dP[i]     = 0.0;
			dn[i]     = 0.0;
			bIsRes[i] = FALSE;
			dbML[i]   = 0.0;
			dbMR[i]   = 0.0;
			dcQU[i]   = 0.0;
			dcQL[i]   = 0.0;
			dpM[i]    = 0.0;
			dpMy[i]   = 0.0;

			LcomK[i]  = 0;
		}
	}
	_JUD_RJNT_D() {}
	_JUD_RJNT_D(const _JUD_RJNT_D& src) { *this = src; }
	_JUD_RJNT_D& operator= (const _JUD_RJNT_D& src)
	{
		bCheck     = src.bCheck    ;
		ElemK_Up   = src.ElemK_Up  ; 
		ElemK_Down = src.ElemK_Down; 
		for(int i=0 ; i<2 ; i++)
		{
			ddb[i]    = src.ddb[i]   ; 
			ddc[i]    = src.ddc[i]   ; 
			dtp[i]    = src.dtp[i]   ; 
			dVe[i]    = src.dVe[i]   ; 
			dk[i]     = src.dk[i]    ; 
			dP[i]     = src.dP[i]    ;
			dn[i]     = src.dn[i]    ;
			bIsRes[i] = src.bIsRes[i];
			dbML[i]   = src.dbML[i]  ;
			dbMR[i]   = src.dbMR[i]  ;
			dcQU[i]   = src.dcQU[i]  ;
			dcQL[i]   = src.dcQL[i]  ;
			dpM[i]    = src.dpM[i]   ;
			dpMy[i]   = src.dpMy[i]  ;

			LcomK[i]  = src.LcomK[i] ;
		}
		return *this;
	}
};
struct JUD_RJNT_UNIT
{
	JUD_RJNT_UNIT()
	{
		ddb   =D_UNITSYS_BASE_LENGTH;
		ddc   =D_UNITSYS_BASE_FORCE ;
		dtp   =D_UNITSYS_BASE_FORCE ;
	  dVe   =D_UNITSYS_BASE_VOLUME; 
		dP    =D_UNITSYS_BASE_FORCE ; 
		dbML  =D_UNITSYS_BASE_MOMENT; 
		dbMR  =D_UNITSYS_BASE_MOMENT; 
		dcQU  =D_UNITSYS_BASE_FORCE ; 
		dcQL  =D_UNITSYS_BASE_FORCE ; 
		dpM   =D_UNITSYS_BASE_MOMENT; 
	  dpMy  =D_UNITSYS_BASE_MOMENT;     
	}
	int ddb ;
	int ddc ;
	int dtp ;
	int dVe ;
	int dP  ;
	int dbML;
	int dbMR;
	int dcQU;
	int dcQL;
	int dpM ;
	int dpMy;  
};

// 일본 2차설계 - 내력비 1차설계 결과 PMS:4215
struct _JUD_RJSR_DIR_D
{
	BOOL   bIsRes;//결과 존재여부
	double dRatio;//내력비
	// 휨강도
	double dMcu[2];// Up, Low
	double dMbu[2];// Local축의 +방향, -방향
	// 해당 방향 결과 유무(Elem존재여부)
	BOOL   bMcu[2];// Up, Low
	BOOL   bMbu[2];// Local축의 +방향, -방향
	void Initialize()
	{
		bIsRes = FALSE;
		dRatio  = 0.0;
		for(int i=0 ; i<2 ; i++)
		{
			dMcu[i] = 0.0;
			dMbu[i] = 0.0;
			bMcu[i] = FALSE;
			bMbu[i] = FALSE;
		}
	}
	_JUD_RJSR_DIR_D() {}
	_JUD_RJSR_DIR_D(_JUD_RJSR_DIR_D& rData) {*this = rData;}
	_JUD_RJSR_DIR_D& operator = (_JUD_RJSR_DIR_D& rData)
	{
		bIsRes  = rData.bIsRes;
		dRatio  = rData.dRatio;
		for(int i=0 ; i<2 ; i++)
		{
			dMcu[i] = rData.dMcu[i];
			dMbu[i] = rData.dMbu[i];
			bMcu[i] = rData.bMcu[i];
			bMbu[i] = rData.bMbu[i];
		}
		return *this;
	}
};

struct JUD_RJSR_DIR_UNIT
{
	JUD_RJSR_DIR_UNIT()
	{
		dMcu =D_UNITSYS_BASE_MOMENT; 
	  dMbu =D_UNITSYS_BASE_MOMENT;     
	}
	int dMcu;
	int dMbu;
};

struct _JUD_RJSR_D
{
	BOOL     bCheck;
	T_ELEM_K ElemK; // ResD의 기준의 되는 기둥 Elem. Key (ResD의 Mbu값은 해당 Elem의 Local축방향을 기준으로 계산된 결과 입니다.
	_JUD_RJSR_DIR_D ResD[2];// 0:Myy방향 결과,  1:Mzz방향 결과
	void Initialize()
	{
		bCheck = FALSE;
		ElemK = 0;
		for(int i=0 ; i<2 ; i++) { ResD[i].Initialize(); }
	}
	_JUD_RJSR_D() {}
	_JUD_RJSR_D(_JUD_RJSR_D& rData) {*this = rData;}
	_JUD_RJSR_D& operator = (_JUD_RJSR_D& rData)
	{
		bCheck = rData.bCheck;
		ElemK = rData.ElemK;
		for(int i=0 ; i<2 ; i++) { ResD[i] = rData.ResD[i]; }
		return *this;
	}
};

struct _JUD_RJSR_UNIT
{
	_JUD_RJSR_UNIT()  
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};

// 일본 2차설계 - 주각 1차설계 결과 PMS:4215
struct _JUD_RSBL_D
{
	BOOL   bCheck;
	int nType; // 출력형식(0:메이커제, 1:S조 노출형(루트1-1 or 루트3), 2:S조 노출형(루트1-2 or 루트2))
	// 모멘트 [0]:yy [1]:zz
	// 전단력 [0]:z  [1]:y
	double dP;        //
	double dM[2];     // 
	double dQ[2];     //

	double dMa[2];    // 허용모멘트(메이커제만)
	double dQa[2];    // 마찰에 의한 주각의 허용전단력

	double de[2];     // 축력의 편심거리
	double dXn[2];    // 베이스플레이트 하면의 중심축 위치(인장영역에서의 거리)
	double dSigmac[2];// 압축구간의 콘크리트의 최대 압축응력
	double dT[2];     // 인장측 엥커볼트들의 인장력
	double dSigmat[2];// 엥커볼트의 인장응력
	double dtau[2];   // 엥커볼트의 전단응력
	double dfc[2];    // 콘크리트의 설계기준강도
	double dfto[2];   // 엥커 볼트의 허용 인장응력도(장기： 0.5 F, 단기： 장기의 1.5배)
	double dfts[2];   // 전단력을 동시에 받는 엥커 볼트의 허용 인장 응력도 
	double dfs[2];    // 엥커 볼트의 허용 전단 응력도
	double dTb[2];
	double dQb[2]; 
	double dRateComb[2];
	
	// 아래 항목은 2차설계까지 수행되면 경우에 따라(루트1-2 or 루트2) 1차설계에 추가적으로 계산함
	BOOL   bIsSub; // 지진력에 의해서 생기는 응력을γ배 시킨 계산 결과의 유무	
	double dAlpha;  // 계수 default = 1.0
	double dGamma;  // 계수 default = 1.0
	double dSub_P; 
	double dSub_Q[2];
	double dSub_M[2];
	double dSub_e[2];
	double dSub_Xn[2];
	double dSub_Sigmac[2];
	double dSub_Tb[2];
	double dSub_Pb[2];
	double dSub_bQa[2];
	double dSub_Qy[2];		

	void Initialize()
	{
		bCheck    = FALSE;
		nType     = 0;
		dP        = 0.0;    
		bIsSub    = FALSE;
		dAlpha    = 1.0;  
	  dGamma    = 1.0; 
		dSub_P    = 0.0;
		for(int i=0 ; i<2 ; i++)
		{
			dM[i]          = 0.0;
			dQ[i]          = 0.0;
			dMa[i]         = 0.0;
			dQa[i]         = 0.0;
			de[i]          = 0.0;
			dXn[i]         = 0.0;
			dSigmac[i]     = 0.0;
			dT[i]          = 0.0;
			dSigmat[i]     = 0.0;
			dtau[i]        = 0.0;
			dfc[i]         = 0.0;
			dfto[i]        = 0.0;
			dfts[i]        = 0.0;
			dfs[i]         = 0.0;
			dTb[i]         = 0.0;
			dQb[i]         = 0.0; 
			dRateComb[i]   = 0.0;
			dSub_Q[i]      = 0.0;
			dSub_M[i]      = 0.0;
			dSub_e[i]      = 0.0;
			dSub_Xn[i]     = 0.0;
			dSub_Sigmac[i] = 0.0;
			dSub_Tb[i]     = 0.0;
			dSub_Pb[i]     = 0.0;
			dSub_bQa[i]    = 0.0;
			dSub_Qy[i]     = 0.0;
		}
	}
	_JUD_RSBL_D() {}
	_JUD_RSBL_D(const _JUD_RSBL_D& src) { *this = src; }
	_JUD_RSBL_D& operator= (const _JUD_RSBL_D& src)
	{
		bCheck    = src.bCheck;
		nType     = src.nType ;
		dP        = src.dP    ;
		bIsSub    = src.bIsSub;
		dAlpha    = src.dAlpha;  
	  dGamma    = src.dGamma; 
		dSub_P    = src.dSub_P;
		for(int i=0 ; i<2 ; i++)
		{
			dM[i]          = src.dM[i]         ;
			dQ[i]          = src.dQ[i]         ;
			dMa[i]         = src.dMa[i]        ;
			dQa[i]         = src.dQa[i]        ;
			de[i]          = src.de[i]         ;
			dXn[i]         = src.dXn[i]        ;
			dSigmac[i]     = src.dSigmac[i]    ;
			dT[i]          = src.dT[i]         ;
			dSigmat[i]     = src.dSigmat[i]    ;
			dtau[i]        = src.dtau[i]       ;
			dfc[i]         = src.dfc[i]        ;
			dfto[i]        = src.dfto[i]       ;
			dfts[i]        = src.dfts[i]       ;
			dfs[i]         = src.dfs[i]        ;
			dTb[i]         = src.dTb[i]        ;
			dQb[i]         = src.dQb[i]        ; 
			dRateComb[i]   = src.dRateComb[i]  ;
			dSub_Q[i]      = src.dSub_Q[i]     ;
			dSub_M[i]      = src.dSub_M[i]     ;
			dSub_e[i]      = src.dSub_e[i]     ;
			dSub_Xn[i]     = src.dSub_Xn[i]    ;
			dSub_Sigmac[i] = src.dSub_Sigmac[i];
			dSub_Tb[i]     = src.dSub_Tb[i]    ;
			dSub_Pb[i]     = src.dSub_Pb[i]    ;
			dSub_bQa[i]    = src.dSub_bQa[i]   ;
			dSub_Qy[i]     = src.dSub_Qy[i]    ;
		}
		return *this;
	}
};
struct JUD_RSBL_UNIT
{
	JUD_RSBL_UNIT()
	{
		dP         =D_UNITSYS_BASE_FORCE ;
		dM         =D_UNITSYS_BASE_MOMENT;
		dQ         =D_UNITSYS_BASE_FORCE ;    
		dMa        =D_UNITSYS_BASE_MOMENT;    
		dQa        =D_UNITSYS_BASE_FORCE ;     
		de         =D_UNITSYS_BASE_LENGTH;
		dXn        =D_UNITSYS_BASE_LENGTH;
		dSigmac    =D_UNITSYS_BASE_STRESS;
		dT         =D_UNITSYS_BASE_FORCE ; 
		dSigmat    =D_UNITSYS_BASE_STRESS;    
		dtau       =D_UNITSYS_BASE_STRESS;
		dfc        =D_UNITSYS_BASE_STRESS;
		dfto       =D_UNITSYS_BASE_STRESS;
		dfts       =D_UNITSYS_BASE_STRESS;
		dfs        =D_UNITSYS_BASE_STRESS;
		dTb        =D_UNITSYS_BASE_FORCE ;
		dQb        =D_UNITSYS_BASE_FORCE ;                                         
		dSub_P     =D_UNITSYS_BASE_FORCE ; 
		dSub_M     =D_UNITSYS_BASE_MOMENT;
		dSub_Q     =D_UNITSYS_BASE_FORCE ;    
		dSub_e     =D_UNITSYS_BASE_LENGTH;
		dSub_Xn    =D_UNITSYS_BASE_LENGTH;
		dSub_Sigmac=D_UNITSYS_BASE_STRESS;
		dSub_Tb    =D_UNITSYS_BASE_FORCE ;
		dSub_Pb    =D_UNITSYS_BASE_FORCE ;
		dSub_bQa   =D_UNITSYS_BASE_FORCE ;
		dSub_Qy    =D_UNITSYS_BASE_FORCE ;		
	}                                  
	int dP         ;
	int dM         ;
	int dQ         ;    
	int dMa        ;    
	int dQa        ;    
	int de         ;
	int dXn        ;
	int dSigmac    ;
	int dT         ; 
	int dSigmat    ;
	int dtau       ;
	int dfc        ;
	int dfto       ;
	int dfts       ;
	int dfs        ;
	int dTb        ;
	int dQb        ;  
	int dSub_P     ; 
	int dSub_Q     ;
	int dSub_M     ;
	int dSub_e     ;
	int dSub_Xn    ;
	int dSub_Sigmac;
	int dSub_Tb    ;
	int dSub_Pb    ;
	int dSub_bQa   ;
	int dSub_Qy    ;		
};


// 일본 2차설계 - Steel 횡보강 1차설계 결과 PMS:4215
struct _JUD_RLCF_D
{
	BOOL bCheck;
	// 등간격으로 횡보강을 마련하는 방법
	int nLcfNum;     // 횡보강수

	CArray<double,double> aLenYList; // 횡보강 간격 (Unit : L)
	double dLength;  // 부재 길이 (Unit : L)
	double dLamda;   // 세장비(무보강시)
	
	int nLcfNum_Req; // 필요 횡보강수

	BOOL bNumChk;    // 횡보강수 검토 결과


	// 단부에 횡보강을 마련하는 방법
	BOOL bIsLbChk;  // 단부 횡보강 검토 여부
	double dLb_Max; // 실제 횡보강 간격(최대) (Unit : L)
	double dLb_End; // 실제 횡보강 간격(양단부의 횡보강 거리중 큰값) (Unit : L)
	double dLb_ReqMax; // 최대 횡보강 간격(허용가능한 최대 횡보강 거리) (Unit : L)
	double dLb_ReqEnd; // 최대 횡보강 간격(허용가능한 단부 횡보강 거리) (Unit : L)
	BOOL bLbChk;    // 단부 횡보강 거리 검토 결과


	void Initialize()
	{
		bCheck      = FALSE;
		nLcfNum     = 0;
		aLenYList.RemoveAll();
		dLength     = 0.0;
		dLamda      = 0.0;
		nLcfNum_Req = 0;
		bNumChk     = FALSE;
		bIsLbChk    = FALSE;
		dLb_Max     = 0.0;
		dLb_End     = 0.0;
		dLb_ReqMax  = 0.0;
		dLb_ReqEnd  = 0.0;
		bLbChk      = FALSE;
	}
	_JUD_RLCF_D() {}
	_JUD_RLCF_D(const _JUD_RLCF_D& src) { *this = src; }
	_JUD_RLCF_D& operator= (const _JUD_RLCF_D& src)
	{
		bCheck      = src.bCheck     ;
		nLcfNum     = src.nLcfNum    ;
		aLenYList.Copy(src.aLenYList);
		dLength     = src.dLength    ;
		dLamda      = src.dLamda     ;
		nLcfNum_Req = src.nLcfNum_Req;
		bNumChk     = src.bNumChk    ;
		bIsLbChk    = src.bIsLbChk   ;
		dLb_Max     = src.dLb_Max    ;
		dLb_End     = src.dLb_End    ;
		dLb_ReqMax  = src.dLb_ReqMax ;
		dLb_ReqEnd  = src.dLb_ReqEnd ;
		bLbChk      = src.bLbChk     ;
		
		return *this;
	}
	BOOL IsOK()
	{
		if(!bNumChk) return FALSE;
		if(bIsLbChk && !bLbChk) return FALSE;
		return TRUE;
	}
};
struct _JUD_RLCF_D_RW
{
	BOOL   bCheck      ;
	int    nLcfNum     ; 
	double dLength     ; 
	double dLamda      ; 
	int    nLcfNum_Req ; 
	BOOL   bNumChk     ; 
	BOOL   bIsLbChk    ; 
	double dLb_Max     ; 
	double dLb_End     ; 
	double dLb_ReqMax  ; 
	double dLb_ReqEnd  ; 
	BOOL   bLbChk      ; 
	int    nNumLenYList;
	void Initialize()
	{
		bCheck      = FALSE;
		nLcfNum     = 0;
		dLength     = 0.0;
		dLamda      = 0.0;
		nLcfNum_Req = 0;
		bNumChk     = FALSE;
		bIsLbChk    = FALSE;
		dLb_Max     = 0.0;
		dLb_End     = 0.0;
		dLb_ReqMax  = 0.0;
		dLb_ReqEnd  = 0.0;
		bLbChk      = FALSE;
		nNumLenYList = 0;
	}
	void GetRLCF(_JUD_RLCF_D& rData)
	{ 
		rData.bCheck      = bCheck     ;
		rData.nLcfNum     = nLcfNum    ;
		rData.dLength     = dLength    ;
		rData.dLamda      = dLamda     ;
		rData.nLcfNum_Req = nLcfNum_Req;
		rData.bNumChk     = bNumChk    ;
		rData.bIsLbChk    = bIsLbChk   ;
		rData.dLb_Max     = dLb_Max    ;
		rData.dLb_End     = dLb_End    ;
		rData.dLb_ReqMax  = dLb_ReqMax ;
		rData.dLb_ReqEnd  = dLb_ReqEnd ;
		rData.bLbChk      = bLbChk     ;
	}
	void SetRLCF(const _JUD_RLCF_D& rData)
	{
		bCheck      = rData.bCheck     ;
		nLcfNum     = rData.nLcfNum    ;
		dLength     = rData.dLength    ;
		dLamda      = rData.dLamda     ;
		nLcfNum_Req = rData.nLcfNum_Req;
		bNumChk     = rData.bNumChk    ;
		bIsLbChk    = rData.bIsLbChk   ;
		dLb_Max     = rData.dLb_Max    ;
		dLb_End     = rData.dLb_End    ;
		dLb_ReqMax  = rData.dLb_ReqMax ;
		dLb_ReqEnd  = rData.dLb_ReqEnd ;
		bLbChk      = rData.bLbChk     ;
		nNumLenYList = (int)rData.aLenYList.GetSize();
	}  
};
struct JUD_RLCF_UNIT
{
	JUD_RLCF_UNIT()
	{
		dLenY     =D_UNITSYS_BASE_LENGTH;
		dLength   =D_UNITSYS_BASE_LENGTH;
		dLb_Max   =D_UNITSYS_BASE_LENGTH; 
		dLb_End   =D_UNITSYS_BASE_LENGTH; 
		dLb_ReqMax=D_UNITSYS_BASE_LENGTH; 
		dLb_ReqEnd=D_UNITSYS_BASE_LENGTH;     
	}
	int dLenY     ;
	int dLength   ;
	int dLb_Max   ;
	int dLb_End   ;
	int dLb_ReqMax;
	int dLb_ReqEnd;  
};
// 설계 결과 구조체 (2차설계) ////////////////////////////////////////////////////////////////////////

struct _JUD_ROOT_CHK_STL_BASE
{
	int nNumStor;           // 층수. (Only Steel)
	double dTotalFloorArea; // 연면적.
	double dHeightEaves;    // 처마높이. (Only Steel)
	double dSpan;           // 최장Span. (Only Steel)
	BOOL   bCBRatio;        // 기둥과 보의 내력비 검토여부
	double dCBRatio;        // 기둥과 보의 내력비. (Only Steel)

	void Initialize()
	{
		nNumStor        = 0;
		dTotalFloorArea = 0.0;
		dHeightEaves    = 0.0;
		dSpan           = 0.0;
		bCBRatio        = FALSE;
		dCBRatio        = 0.0;
	}

	_JUD_ROOT_CHK_STL_BASE() {}
	_JUD_ROOT_CHK_STL_BASE(const _JUD_ROOT_CHK_STL_BASE& src) { *this = src; }
	_JUD_ROOT_CHK_STL_BASE& operator= (const _JUD_ROOT_CHK_STL_BASE& src)
	{
		nNumStor        = src.nNumStor;
		dTotalFloorArea = src.dTotalFloorArea;
		dHeightEaves    = src.dHeightEaves;
		dSpan           = src.dSpan;
		bCBRatio        = src.bCBRatio;
		dCBRatio        = src.dCBRatio;
		return *this;
	}  
};
struct _JUD_ROOT_CHK_STL_BASE_UNIT
{
	_JUD_ROOT_CHK_STL_BASE_UNIT()
	{
		dTotalFloorArea=D_UNITSYS_BASE_AREA;
		dHeightEaves=D_UNITSYS_BASE_LENGTH;
		dSpan=D_UNITSYS_BASE_LENGTH;
	}   
	int dTotalFloorArea;
	int dHeightEaves;   
	int dSpan;          
};

struct _JUD_ROOT_CHK_CON_BASE
{
	double d25Aw07Ac_ZWAi;       // Con : 2.5*Alpha*Aw + 0.7*Alpha*Ac. (SRC : 2.5*Alpha*Aw + 1.0*Alpha*Ac) (UNIT : ???)
	double d18Aw18Ac_ZWAi;       // Con : 1.8*Alpha*Aw + 1.8*Alpha*Ac. (SRC : 2.0*Alpha*Aw + 2.0*Alpha*Ac) (UNIT : ???)

	void Initialize()
	{
		d25Aw07Ac_ZWAi = 0.0;
		d18Aw18Ac_ZWAi = 0.0;
	}

	_JUD_ROOT_CHK_CON_BASE() {}
	_JUD_ROOT_CHK_CON_BASE(const _JUD_ROOT_CHK_CON_BASE& src) { *this = src; }
	_JUD_ROOT_CHK_CON_BASE& operator= (const _JUD_ROOT_CHK_CON_BASE& src)
	{
		d25Aw07Ac_ZWAi = src.d25Aw07Ac_ZWAi;
		d18Aw18Ac_ZWAi = src.d18Aw18Ac_ZWAi;
		return *this;
	}
};


struct _JUD_ROOT_CHK_CON_BASE_UNIT
{
	_JUD_ROOT_CHK_CON_BASE_UNIT()
	{
		d25Aw07Ac_ZWAi=D_UNITSYS_BASE_RSTRESS;
		d18Aw18Ac_ZWAi=D_UNITSYS_BASE_RSTRESS;
	}                                  
	int d25Aw07Ac_ZWAi;                        
	int d18Aw18Ac_ZWAi;
};

struct _JUD_ROOT_CHK_D
{  
	int nStructMatlType;    // 주구조 Type : D_JUD_STRUCT_MATL_TYPE_XXX.
	int nRoot;              // 계산정보 옵션에 따라 최종 확정된 Root
	int nRoot_Cal;          // 계산된 Root
			
	double dHeight;         // 건물높이.   (UNIT : L)
	double dHBRatio;        // 탑상비.
	BOOL   bDriftAngle;
	double dDriftAngle;     // 층간변형각. (Disp/Height)
	BOOL   bStffRatio; 
	double dStffRatio;      // 강성율.
	double dEccnRatio;      // 편심율.
	BOOL   bQuQunRatio;
	double dQuQunRatio;     // 보유수평내력비.

	_JUD_ROOT_CHK_STL_BASE StlChkBase;
	_JUD_ROOT_CHK_CON_BASE ConChkBase;
	
	void Initialize()
	{    
		nStructMatlType = 0;
		nRoot           = 0;
		nRoot_Cal       = 0;

		dHeight         = 0.0;
		dHBRatio        = 0.0;
		bDriftAngle     = FALSE;
		dDriftAngle     = 0.0;
		bStffRatio      = FALSE;
		dStffRatio      = 0.0;
		dEccnRatio      = 0.0;
		bQuQunRatio     = FALSE;
		dQuQunRatio     = 0.0;

		StlChkBase.Initialize();
		ConChkBase.Initialize();
	}

	_JUD_ROOT_CHK_D() {}
	_JUD_ROOT_CHK_D(const _JUD_ROOT_CHK_D& src) { *this = src; }
	_JUD_ROOT_CHK_D& operator= (const _JUD_ROOT_CHK_D& src)
	{ 
		nStructMatlType = src.nStructMatlType;
		nRoot           = src.nRoot;
		nRoot_Cal       = src.nRoot_Cal;

		dHeight         = src.dHeight    ;
		dHBRatio        = src.dHBRatio   ;
		bDriftAngle     = src.bDriftAngle;
		dDriftAngle     = src.dDriftAngle;
		bStffRatio      = src.bStffRatio ;
		dStffRatio      = src.dStffRatio ;
		dEccnRatio      = src.dEccnRatio ;
		bQuQunRatio     = src.bQuQunRatio;
		dQuQunRatio     = src.dQuQunRatio;

		StlChkBase      = src.StlChkBase;
		ConChkBase      = src.ConChkBase;
		
		return *this;
	}  
};


struct _JUD_ROOT_CHK_UNIT
{
	_JUD_ROOT_CHK_UNIT()
	{
		dHeight = D_UNITSYS_BASE_LENGTH;
	}                                  
	int dHeight;
};


struct _JUD_MEMB_RANK_COLLAPSE_REDN
{
	double dSumGMuy;
	double dSumGMuz;
	double dSumCMuy;
	double dSumCMuz;
	double dSumGMmy;
	double dSumGMmz;
	double dSumCMmy;
	double dSumCMmz;
	double dSumMmy;
	double dSumMmz;
	double dQuy;
	double dQuz;
	double dQmy;
	double dQmz;
	double dQmuy;//(Qm = sum(Mu_I+Mu_J)/Length.)
	double dQmuz;
	double dQ0y;
	double dQ0z;
	void Initialize()
	{
		dSumGMuy = 0.0;
		dSumGMuz = 0.0;
		dSumCMuy = 0.0;
		dSumCMuz = 0.0;
		dSumGMmy = 0.0;
		dSumGMmz = 0.0;
		dSumCMmy = 0.0;
		dSumCMmz = 0.0;
		dSumMmy  = 0.0;
		dSumMmz  = 0.0;
		dQuy     = 0.0;
		dQuz     = 0.0;
		dQmy     = 0.0;
		dQmz     = 0.0;
		dQmuy    = 0.0;
		dQmuz    = 0.0;
		dQ0y     = 0.0;
		dQ0z     = 0.0;
	}
};
struct _JUD_MEMB_RANK_COLLAPSE_REDN_UNIT
{
	_JUD_MEMB_RANK_COLLAPSE_REDN_UNIT()
	{
		dSumGMuy = D_UNITSYS_BASE_MOMENT;
		dSumGMuz = D_UNITSYS_BASE_MOMENT;
		dSumCMuy = D_UNITSYS_BASE_MOMENT;
		dSumCMuz = D_UNITSYS_BASE_MOMENT;
		dSumGMmy = D_UNITSYS_BASE_MOMENT;
		dSumGMmz = D_UNITSYS_BASE_MOMENT;
		dSumCMmy = D_UNITSYS_BASE_MOMENT;
		dSumCMmz = D_UNITSYS_BASE_MOMENT;
		dSumMmy  = D_UNITSYS_BASE_MOMENT;
		dSumMmz  = D_UNITSYS_BASE_MOMENT;
		dQuy     = D_UNITSYS_BASE_FORCE;
		dQuz     = D_UNITSYS_BASE_FORCE;
		dQmy     = D_UNITSYS_BASE_FORCE;
		dQmz     = D_UNITSYS_BASE_FORCE;
		dQmuy    = D_UNITSYS_BASE_FORCE;
		dQmuz    = D_UNITSYS_BASE_FORCE;
		dQ0y     = D_UNITSYS_BASE_FORCE;
		dQ0z     = D_UNITSYS_BASE_FORCE;
	}
	int dSumGMuy;
	int dSumGMuz;
	int dSumCMuy;
	int dSumCMuz;
	int dSumGMmy;
	int dSumGMmz;
	int dSumCMmy;
	int dSumCMmz;
	int dSumMmy ;
	int dSumMmz ;
	int dQuy    ;
	int dQuz    ;
	int dQmy    ;
	int dQmz    ;
	int dQmuy   ;
	int dQmuz   ;
	int dQ0y    ;
	int dQ0z    ;
};


struct _JUD_MEMB_RANK_D
{
	int nRank;    // 부재종별(D_JUD_RANK_?) : 기호표시형식(1번째 부재종류(Beam-Column : F, Brace : B, Wall : W) 2번째 종별(A, B, C, D))
	int nRankCal; 
	int nRankOrg;
	int nMembType;// 부재종류(D_MBTP_?)

	BOOL bFailQ;     // 전단파괴모드여부
	BOOL bFailM;     // 휨파괴모드여부
	
	BOOL   bIsData[3];  // Position별 Data존재여부
	double dTauFc[3];   // τu/fc (RC(Beam,Column))
	double dSigmaFc[3]; // σ0/fc (RC(Column))
	double dPt[3];      // 인장철근비 (RC(Column))
	double dHDRatio[3]; // h0/D (RC(Column))
	double dBTR[3];     // 플랜지 폭두깨비(B/t) (Steel(All))
	double dDTR[3];     // 웨브   폭두깨비(H/t) (Steel(All))
	double dLambda;     // 세장비(Steel(Brace))

	
	BOOL   bCollapseChk;
	int    nFType;	// 파괴모드(0:취성파괴이외, 1:전단파괴, 2:휨파괴, 11:전단파괴위험, 12:휨파괴위험)
	double daM;
	double daMy;
	double daMz;
	double daQ;
	double daQy;
	double daQz;
	_JUD_MEMB_RANK_COLLAPSE_REDN CollapseRednI;
	_JUD_MEMB_RANK_COLLAPSE_REDN CollapseRednJ;


	void Initialize()
	{    
		nRank    = 0;
		nRankCal = 0;
		nRankOrg = 0;
		nMembType= 0;
		bFailQ   = FALSE;
		bFailM   = FALSE;
		for(int i=0 ; i<3 ; i++)
		{
			bIsData[i]  = FALSE;
			dTauFc[i]   = 0.0;
			dSigmaFc[i] = 0.0;
			dPt[i]      = 0.0;
			dHDRatio[i] = 0.0;
			dBTR[i]     = 0.0;
			dDTR[i]     = 0.0;
		}
		dLambda  = 0.0;

		bCollapseChk = FALSE;
		nFType = 0;	
		daM  = 0.0;
		daMy = 0.0;
		daMz = 0.0;
		daQ  = 0.0;
		daQy = 0.0;
		daQz = 0.0;
		CollapseRednI.Initialize();
		CollapseRednJ.Initialize();
	}

	_JUD_MEMB_RANK_D() {}
	_JUD_MEMB_RANK_D(const _JUD_MEMB_RANK_D& src) { *this = src; }
	_JUD_MEMB_RANK_D& operator= (const _JUD_MEMB_RANK_D& src)
	{
		nRank    = src.nRank;
		nRankCal = src.nRankCal;
		nRankOrg = src.nRankOrg;
		nMembType= src.nMembType;
		bFailQ   = src.bFailQ;
		bFailM   = src.bFailM;
		for(int i=0 ; i<3 ; i++)
		{			
			bIsData[i]  = src.bIsData[i] ;
			dTauFc[i]   = src.dTauFc[i]  ;
			dSigmaFc[i] = src.dSigmaFc[i];
			dPt[i]      = src.dPt[i]     ;
			dHDRatio[i] = src.dHDRatio[i];
			dBTR[i]     = src.dBTR[i]    ;
			dDTR[i]     = src.dDTR[i]    ;
		}
		dLambda  = src.dLambda;
	
		bCollapseChk  = src.bCollapseChk ;
		nFType        = src.nFType       ;	
		daM           = src.daM          ;
		daMy          = src.daMy         ;
		daMz          = src.daMz         ;
		daQ           = src.daQ          ;
		daQy          = src.daQy         ;
		daQz          = src.daQz         ;
		CollapseRednI = src.CollapseRednI;
		CollapseRednJ = src.CollapseRednJ;

		return *this;
	}
};

struct _JUD_MEMB_RANK_UNIT
{
	_JUD_MEMB_RANK_UNIT()  
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};

struct _JUD_HGST_BASE  // Hinge Status Base Data.
{
	BOOL bDOF;
	int nStepCrack;
	int nStepYield;
	int nStatus;
	
	void Initialize()
	{
		bDOF = FALSE;
		nStepCrack = 0;
		nStepYield = 0;
		nStatus    = 0;
	}

	_JUD_HGST_BASE() {}
	_JUD_HGST_BASE(const _JUD_HGST_BASE& src) { *this = src; }
	_JUD_HGST_BASE& operator= (const _JUD_HGST_BASE& src)
	{
		bDOF       = src.bDOF;
		nStepCrack = src.nStepCrack;
		nStepYield = src.nStepYield;
		nStatus    = src.nStatus;

		return *this;
	}
};

struct _JUD_HGST_D
{
	_JUD_HGST_BASE HingeStausI[6];
	_JUD_HGST_BASE HingeStausC[6];
	_JUD_HGST_BASE HingeStausJ[6];

	void Initialize()
	{
		for(int i=0; i<6; i++)
		{
			HingeStausI[i].Initialize();
			HingeStausC[i].Initialize();
			HingeStausJ[i].Initialize();
		}
	}

	_JUD_HGST_D() {}
	_JUD_HGST_D(const _JUD_HGST_D& src) { *this = src; }
	_JUD_HGST_D& operator= (const _JUD_HGST_D& src)
	{
		for(int i=0; i<6; i++)
		{
			HingeStausI[i] = src.HingeStausI[i];
			HingeStausC[i] = src.HingeStausC[i];
			HingeStausJ[i] = src.HingeStausJ[i];
		}
		return *this;
	}
};

struct _JUD_HGST_UNIT
{
	_JUD_HGST_UNIT()  
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};

struct _JUD_REDN_BASE
{
	// by SHIN
	// - d?U   : 해당 Elem의 힌지속성에서 종국시의 강도값([0]:+, [1]:-)
	// - d?    : 해석결과 상에서 Crack또는 Yield시(Crack우선)의 Step시의 해당 Elem의 부재력
	// - d?0   : 초기하중의 해당 Elem의 부재력
	// - d?_max: 해당 결과 마지막 Step까지의 최대 부재력
	BOOL   bIsN;
	double dNU[2];
	double dN;
	double dN0;
	double dN_max; 
	BOOL   bIsMy;
	double dMUy_Inter[2]; // 일반형식일때 모멘트 방향의 종국강도, N-M-M형식일때 2축휨에 대한 보관작업이 이루어진 종국강도
	double dMUy[2];
	double dMUy_max[2]; // P-M전체에서 최대값 
	double dMy;
	double dM0y;
	double dMy_max;
	BOOL   bIsMz;
	double dMUz_Inter[2]; // 일반형식일때 모멘트 방향의 종국강도, N-M-M형식일때 2축휨에 대한 보관작업이 이루어진 종국강도
	double dMUz[2];
	double dMUz_max[2]; // P-M전체에서 최대값 
	double dMz;
	double dM0z;
	double dMz_max;
	BOOL   bIsQy;
	double dQUy[2];
	double dQy;
	double dQ0y;
	double dQy_max;
	BOOL   bIsQz;
	double dQUz[2];
	double dQz;
	double dQ0z;
	double dQz_max;
	double dQMy[2];  // 전단내력(연성파괴) 여유도 계산용 : 모멘트 강도에 의해 필요한 전단력. (Qm = sum(Mu_I+Mu_J)/Length.)
	double dQMz[2];  // 전단내력(연성파괴) 여유도 계산용 : 모멘트 강도에 의해 필요한 전단력. (Qm = sum(Mu_I+Mu_J)/Length.)
	double dQUQMy[2];// 전단내력(연성파괴) 여유도 : (Qu-Q0)/Qm 
	double dQUQMz[2];// 전단내력(연성파괴) 여유도 : (Qu-Q0)/Qm
	// 소성율	(최종 Step에서의 값)
	double dDDx;     // D/Dx
	double dDDy;     // D/Dy
	double dDDz;     // D/Dz
	double dRRy;     // R/Ry
	double dRRz;     // R/Rz

	void Initialize()
	{
		dNU[0]  = dNU[1]               = dN      = dN0      = dN_max  = 0.0;
		dMUy[0] = dMUy[1]              = dMy     = dM0y     = dMy_max = 0.0;
		dMUy_Inter[0] = dMUy_Inter[1] = 0.0;
		dMUy_max[0] = dMUy_max[1] = 0.0;
		dMUz[0] = dMUz[1]              = dMz     = dM0z     = dMz_max = 0.0;
		dMUz_Inter[0] = dMUz_Inter[1] = 0.0;
		dMUz_max[0] = dMUz_max[1] = 0.0;
		dQUy[0] = dQUy[1]              = dQy     = dQ0y     = dQy_max = 0.0;
		dQUz[0] = dQUz[1]              = dQz     = dQ0z     = dQz_max = 0.0;
		dQMy[0] = dQMy[1] = dQMz[0] = dQMz[1] = 0.0;
		dQUQMy[0] = dQUQMy[1] = dQUQMz[0] = dQUQMz[1] = 0.0;

		bIsN  = bIsMy = bIsMz = bIsQy = bIsQz = FALSE;
		dDDx = dDDy = dDDz = dRRy = dRRz = 0.0;
	}

	_JUD_REDN_BASE() {}
	_JUD_REDN_BASE(const _JUD_REDN_BASE& src) { *this = src; }
	_JUD_REDN_BASE& operator= (const _JUD_REDN_BASE& src)
	{
		bIsN    = src.bIsN ;
		bIsMy   = src.bIsMy;
		bIsMz   = src.bIsMz;
		bIsQy   = src.bIsQy;
		bIsQz   = src.bIsQz;
		dN      = src.dN ;
		dMy     = src.dMy;
		dMz     = src.dMz;
		dQy     = src.dQy;
		dQz     = src.dQz;
		dN0     = src.dN0;
		dM0y    = src.dM0y;
		dM0z    = src.dM0z;
		dQ0y    = src.dQ0y;
		dQ0z    = src.dQ0z;
		dN_max  = src.dN_max ;
		dMy_max = src.dMy_max;
		dMz_max = src.dMz_max;
		dQy_max = src.dQy_max;
		dQz_max = src.dQz_max;
		dDDx    = src.dDDx;
		dDDy    = src.dDDy;
		dDDz    = src.dDDz;
		dRRy    = src.dRRy;
		dRRz    = src.dRRz;

		for(int i=0 ; i<2 ; i++)
		{
			dNU[i]  = src.dNU[i];
			dMUy[i] = src.dMUy[i];
			dMUz[i] = src.dMUz[i];
			dMUy_Inter[i] = src.dMUy_Inter[i];
			dMUz_Inter[i] = src.dMUz_Inter[i];
			dMUy_max[i] = src.dMUy_max[i];
			dMUz_max[i] = src.dMUz_max[i];
			dQUy[i] = src.dQUy[i];
			dQUz[i] = src.dQUz[i];
			dQMy[i] = src.dQMy[i];
			dQMz[i] = src.dQMz[i];
			dQUQMy[i] = src.dQUQMy[i];
			dQUQMz[i] = src.dQUQMz[i];
		}

		return *this;
	}
};

struct _JUD_REDN_BASE_UNIT
{
	_JUD_REDN_BASE_UNIT()  
	{
		dNU     = D_UNITSYS_BASE_FORCE;
		dN      = D_UNITSYS_BASE_FORCE;
		dN0     = D_UNITSYS_BASE_FORCE;
		dN_max  = D_UNITSYS_BASE_FORCE;
		dMUy_Inter = D_UNITSYS_BASE_MOMENT;
		dMUy    = D_UNITSYS_BASE_MOMENT;
		dMUy_max= D_UNITSYS_BASE_MOMENT;
		dMy     = D_UNITSYS_BASE_MOMENT;
		dM0y    = D_UNITSYS_BASE_MOMENT;
		dMy_max = D_UNITSYS_BASE_MOMENT;
		dMUz_Inter = D_UNITSYS_BASE_MOMENT;
		dMUz    = D_UNITSYS_BASE_MOMENT;
		dMUz_max= D_UNITSYS_BASE_MOMENT;
		dMz     = D_UNITSYS_BASE_MOMENT;
		dM0z    = D_UNITSYS_BASE_MOMENT;
		dMz_max = D_UNITSYS_BASE_MOMENT;
		dQUy    = D_UNITSYS_BASE_FORCE;
		dQy     = D_UNITSYS_BASE_FORCE;
		dQ0y    = D_UNITSYS_BASE_FORCE;
		dQy_max = D_UNITSYS_BASE_FORCE;
		dQUz    = D_UNITSYS_BASE_FORCE;
		dQz     = D_UNITSYS_BASE_FORCE;
		dQ0z    = D_UNITSYS_BASE_FORCE;
		dQz_max = D_UNITSYS_BASE_FORCE;
		dQMy    = D_UNITSYS_BASE_FORCE;
		dQMz    = D_UNITSYS_BASE_FORCE;
	}
	int dNU;
	int dN;
	int dN0;
	int dN_max;
	int dMUy_Inter;
	int dMUy;
	int dMUy_max;
	int dMy;
	int dM0y;
	int dMy_max;
	int dMUz_Inter;
	int dMUz;
	int dMUz_max;
	int dMz;
	int dM0z;
	int dMz_max;
	int dQUy;
	int dQy;
	int dQ0y;
	int dQy_max;
	int dQUz;
	int dQz;
	int dQ0z;
	int dQz_max;
	int dQMy;
	int dQMz;
};


struct _JUD_REDN_D
{
	_JUD_REDN_BASE RednBaseI;   // I단
	_JUD_REDN_BASE RednBaseIM;  // 1/4 
	_JUD_REDN_BASE RednBaseM;   // M단
	_JUD_REDN_BASE RednBaseMJ;  // 3/4
	_JUD_REDN_BASE RednBaseJ;   // J단
	

	void Initialize()
	{
		RednBaseI.Initialize();
		RednBaseIM.Initialize();
		RednBaseM.Initialize();
		RednBaseMJ.Initialize();
		RednBaseJ.Initialize();
	}

	_JUD_REDN_D() {}
	_JUD_REDN_D(const _JUD_REDN_D& src) { *this = src; }
	_JUD_REDN_D& operator= (const _JUD_REDN_D& src)
	{
		RednBaseI  = src.RednBaseI;
		RednBaseIM = src.RednBaseIM;
		RednBaseM  = src.RednBaseM;
		RednBaseMJ = src.RednBaseMJ;
		RednBaseJ  = src.RednBaseJ;
		return *this;
	}
};

struct _JUD_REDN_UNIT
{
	_JUD_REDN_UNIT()  
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};

struct _JUD_GRUP_RANK_D
{
	BOOL bCheck;
	double dQA;
	double dQB;
	double dQC;
	double dQD;
	double dQTotal;

	int nRankCal;
	int nRank;

	void Initialize()
	{
		bCheck = FALSE;

		dQA = 0.0;
		dQB = 0.0;
		dQC = 0.0;
		dQD = 0.0;
		dQTotal = 0.0;

		nRankCal = 0;
		nRank    = 0;
	}

	_JUD_GRUP_RANK_D() {}
	_JUD_GRUP_RANK_D(const _JUD_GRUP_RANK_D& src)
	{
		*this = src;
	}
	_JUD_GRUP_RANK_D& operator= (const _JUD_GRUP_RANK_D& src)
	{
		bCheck = src.bCheck;

		dQA = src.dQA;
		dQB = src.dQB;
		dQC = src.dQC;
		dQD = src.dQD;
		dQTotal = src.dQTotal;

		nRankCal = src.nRankCal;
		nRank    = src.nRank;

		return *this;
	}  
};

struct _JUD_GRUP_RANK_UNIT
{
	_JUD_GRUP_RANK_UNIT()  
	{
		dQA=D_UNITSYS_BASE_FORCE;
		dQB=D_UNITSYS_BASE_FORCE;
		dQC=D_UNITSYS_BASE_FORCE;
		dQD=D_UNITSYS_BASE_FORCE;
		dQTotal=D_UNITSYS_BASE_FORCE;
	}
	int dQA;
	int dQB;
	int dQC;
	int dQD;
	int dQTotal;
};


struct _JUD_QU_RES_FRAME_D
{
	// 가력방향 결과
	double dQu;    
	double dQu_Ds;	
	// XY방향 결과
	double dQuX;
	double dQuY;
	double dQuX_Ds;
	double dQuY_Ds;
	CArray<UINT2, UINT2> arOverlapFrameK; // 해당 Frame과 공유하는 Elem가 있는 인접 Frame;
	
	void Initialize()
	{
		dQu     = 0.0;
		dQu_Ds  = 0.0;
		dQuX    = 0.0;
		dQuY    = 0.0;
		dQuX_Ds = 0.0;
		dQuY_Ds = 0.0;
		arOverlapFrameK.RemoveAll();
	}
	_JUD_QU_RES_FRAME_D() {}
	_JUD_QU_RES_FRAME_D(const _JUD_QU_RES_FRAME_D& src) { *this = src; }
	_JUD_QU_RES_FRAME_D& operator= (const _JUD_QU_RES_FRAME_D& src)
	{
		dQu     = src.dQu   ;
		dQu_Ds  = src.dQu_Ds;
		dQuX    = src.dQu   ;
		dQuY    = src.dQu   ;
		dQuX_Ds = src.dQu_Ds;
		dQuY_Ds = src.dQu_Ds;
		arOverlapFrameK.Copy(src.arOverlapFrameK);
		return *this;
	}
};

struct _JUD_QU_RES_FRAME_UNIT
{
	_JUD_QU_RES_FRAME_UNIT()  
	{
		dQu    =D_UNITSYS_BASE_FORCE;
		dQu_Ds =D_UNITSYS_BASE_FORCE;
		dQuX   =D_UNITSYS_BASE_FORCE;
		dQuY   =D_UNITSYS_BASE_FORCE;
		dQuX_Ds=D_UNITSYS_BASE_FORCE;
		dQuY_Ds=D_UNITSYS_BASE_FORCE;
	} 
	int dQu;
	int dQu_Ds;
	int dQuX;
	int dQuY;
	int dQuX_Ds;
	int dQuY_Ds;
};

struct _JUD_QU_RES_D
{
	_JUD_GRUP_RANK_D FramRankD; // Frame요소 정보
	_JUD_GRUP_RANK_D BracRankD; // Brace요소 정보 
	_JUD_GRUP_RANK_D WallRankD; // Wall요소 정보 

	CMapEx<UINT2,UINT2&,_JUD_QU_RES_FRAME_D,_JUD_QU_RES_FRAME_D&> arFrameD; // Frame(표현 Elem Group)

	double dStorForce; // 해석결과 상의 층전단력
	double dAvgDrift;
	double dDriftAngle;

	double dRs;
	double dQTotal;
	double dBetau;
	double dDs;
	double dFe;
	double dFs;
	double dFes;
	double dQud;
	double dQun;
	double dQu;
	double dQu_Ds;
	double dRatio;
	int nRemark;

	void Initialize()
	{
		FramRankD.Initialize();
		BracRankD.Initialize();
		WallRankD.Initialize();

		arFrameD.RemoveAll();
		
		dStorForce  = 0.0; 
		dAvgDrift   = 0.0;
		dDriftAngle = 0.0;

		dRs     = 0.0;
		dQTotal = 0.0;
		dBetau  = 0.0;
		dDs     = 0.0;
		dFe     = 0.0;
		dFs     = 0.0;
		dFes    = 0.0;
		dQud    = 0.0;
		dQun    = 0.0;
		dQu     = 0.0;
		dQu_Ds  = 0.0;
		dRatio  = 0.0;
		nRemark = 0;
	}

	_JUD_QU_RES_D() {}
	_JUD_QU_RES_D(const _JUD_QU_RES_D& src) { *this = src; }
	_JUD_QU_RES_D& operator= (const _JUD_QU_RES_D& src)
	{
		FramRankD = src.FramRankD;
		BracRankD = src.BracRankD;
		WallRankD = src.WallRankD;

		_JUD_QU_RES_FRAME_D FrameData;
		UINT2 Index;  Index.i1 = 0;  Index.i2 = 0;
		POSITION Pos = src.arFrameD.GetStartPosition();
		while(Pos)
		{
			FrameData.Initialize();
			src.arFrameD.GetNextAssoc(Pos, Index, FrameData);
			arFrameD.SetAt(Index, FrameData);
		}
				
		dStorForce  = src.dStorForce ; 
		dAvgDrift   = src.dAvgDrift  ;
		dDriftAngle = src.dDriftAngle;

		dRs     = src.dRs;
		dQTotal = src.dQTotal;
		dBetau  = src.dBetau;
		dDs     = src.dDs;
		dFe     = src.dFe;
		dFs     = src.dFs;
		dFes    = src.dFes;
		dQud    = src.dQud;
		dQun    = src.dQun;
		dQu     = src.dQu;
		dQu_Ds  = src.dQu_Ds;
		dRatio  = src.dRatio;
		nRemark = src.nRemark;

		return *this;
	}  
};

struct _JUD_QU_RES_UNIT
{
	_JUD_QU_RES_UNIT()  
	{
		dStorForce=D_UNITSYS_BASE_FORCE;
		dAvgDrift=D_UNITSYS_BASE_LENGTH;
		dQTotal=D_UNITSYS_BASE_FORCE;
		dQud=D_UNITSYS_BASE_FORCE;
		dQun=D_UNITSYS_BASE_FORCE;
		dQu=D_UNITSYS_BASE_FORCE;
		dQu_Ds=D_UNITSYS_BASE_FORCE;
	}  
	int dStorForce;
	int dAvgDrift;
	int dQTotal;
	int dQud;
	int dQun;
	int dQu;
	int dQu_Ds;
};

struct _JUD_AC_AW_D // 주량, 벽량 데이터.
{
	UINT   StorK; // Story Key
	int nStructMatlType;
	double dSumAc;
	double dSumAw;  
	double dSumAwPrime;  
	double dSumAlphaAc;
	double dSumAlphaAw;
	double dSumAlphaAwPrime;
	double dEq1; // [RC] Σ2.5αAw＋Σ0.7αAc＋Σ0.7αAw'
	double dEq2; // [RC] Σ1.8αAw＋Σ1.8αAc
	double dZWAi;

	void Initialize()
	{
		StorK = 0;
		nStructMatlType = 0;
		dSumAw = 0.0;
		dSumAc = 0.0;
		dSumAwPrime = 0.0;
		dSumAlphaAw = 0.0;
		dSumAlphaAc = 0.0;
		dSumAlphaAwPrime = 0.0;
		dEq1 = 0.0;
		dEq2 = 0.0;
		dZWAi = 0.0;
	}

	_JUD_AC_AW_D() {}
	_JUD_AC_AW_D(const _JUD_AC_AW_D& src) { *this = src; }
	_JUD_AC_AW_D& operator= (const _JUD_AC_AW_D& src)
	{
		StorK = src.StorK;
		nStructMatlType = src.nStructMatlType;
		dSumAc = src.dSumAc;
		dSumAw = src.dSumAw;
		dSumAwPrime = src.dSumAwPrime;
		dSumAlphaAc = src.dSumAlphaAc;
		dSumAlphaAw = src.dSumAlphaAw;
		dSumAlphaAwPrime = src.dSumAlphaAwPrime;
		dEq1 = src.dEq1;
		dEq2 = src.dEq2;
		dZWAi = src.dZWAi;

		return *this;
	}
};

struct _JUD_AC_AW_UNIT
{
	_JUD_AC_AW_UNIT()  
	{
		dSumAc           = D_UNITSYS_BASE_AREA;
		dSumAw           = D_UNITSYS_BASE_AREA;  
		dSumAwPrime      = D_UNITSYS_BASE_AREA;  
		dSumAlphaAc      = D_UNITSYS_BASE_AREA;
		dSumAlphaAw      = D_UNITSYS_BASE_AREA;
		dSumAlphaAwPrime = D_UNITSYS_BASE_AREA;
		dEq1             = D_UNITSYS_BASE_AREA;
		dEq2             = D_UNITSYS_BASE_AREA;
		dZWAi            = D_UNITSYS_BASE_FORCE;
	} 
	int dSumAc;
	int dSumAw;  
	int dSumAwPrime;  
	int dSumAlphaAc;
	int dSumAlphaAw;
	int dSumAlphaAwPrime;
	int dEq1;
	int dEq2;
	int dZWAi;
};

struct _JUD_STOR_DRFT_D
{
	UINT   NodeK;   // Node Key
	double dHeight; // 층고.
	double dDriftX; // 층간변위 (Global X)
	double dDriftY; // 층간변위 (Global Y)
	double dDrift;  // 층간변위 (가력방향)

	void Initialize()
	{
		NodeK = 0;
		dHeight = 0.0;
		dDriftX = 0.0;
		dDriftY = 0.0;
		dDrift  = 0.0;
	}

	_JUD_STOR_DRFT_D() {}
	_JUD_STOR_DRFT_D(const _JUD_STOR_DRFT_D& src) { *this = src; }
	_JUD_STOR_DRFT_D& operator= (const _JUD_STOR_DRFT_D& src)
	{
		NodeK = src.NodeK;
		dHeight = src.dHeight;
		dDriftX = src.dDriftX;
		dDriftY = src.dDriftY;
		dDrift  = src.dDrift;

		return *this;
	}
};

struct _JUD_STOR_DRFT_UNIT
{
	_JUD_STOR_DRFT_UNIT()  
	{
		dHeight = D_UNITSYS_BASE_LENGTH;
		dDriftX = D_UNITSYS_BASE_LENGTH;  
		dDriftY = D_UNITSYS_BASE_LENGTH;  
		dDrift  = D_UNITSYS_BASE_LENGTH;
	} 
	int dHeight;
	int dDriftX;
	int dDriftY;
	int dDrift; 
};

struct _JUD_BASEPLATE_RES_BASE
{	
	// Mpc (p)
	double dMpc;
	// Mu
	double dnt; // 인장 볼트수

	double dab; // 인장 볼트면적

	double ddt; // 인장 볼트 거리
	double dNu;
	double dTu;
	double dMu;
	// Qu
	double dSu;
	double dQfu;
	double dQsu;	
	double dQu;    // 주각의 극한전단력
	// My
	double dbSigmay;
	double ddty;
	double ddc;
	double dMy;
	// Qy
	double dQa;
	double dbQa;
	double dQy;


	BOOL bConcCorner;      // 입상부 벗겨져 떨어짐 검토               결과여부
	BOOL bConcComp;        // 입상부 압괴 검토                        결과여부
	BOOL bBoltFixComp;     // 앵커볼트 정착부의 압괴 검토             결과여부
	BOOL bBoltSideComp;    // 앵커볼트 주변부의 압렬 검토             결과여부
	BOOL bShearBoltSide;   // 전단에 의한 앵커볼트 주변의 압괴 검토   결과여부
	BOOL bShearConcCorner; // 전단에 의한 입상부의 벗겨져 떨어짐 검토 결과여부
	BOOL bUltStressChk;    // 종국시 응력에 의한 단면검토             결과여부
	BOOL bPlateBroken;     // 베이스 플래이트의 파단 검토             결과여부
	
	// 앵커볼트의 파단검토

	double dc1;            // 입상부 벗겨져 떨어짐 검토 (Stress)
	double dc2;            // 입상부 압괴 검토 (Stress)
	double dc3;            // 앵커볼트 정착부의 압괴 검토 (Stress)
	double dc4;            // 앵커볼트 주변부의 압렬 검토 (Stress)
	double de1;            // 전단에 의한 앵커볼트 주변의 압괴 검토 (Length)
	double de2;            // 전단에 의한 입상부의 벗겨져 떨어짐 검토 (Length)
	double dAa;            // 정착철물의 유효지압면적

	double dAc;            // 기초 콘크리트의 할렬면의 수평 투영 면적

	// Check Result : Value < Allowable = OK
	BOOL bOKConcCorner;      // 입상부 벗겨져 떨어짐 검토               
	BOOL bOKConcComp;        // 입상부 압괴 검토                        
	BOOL bOKBoltFixComp;     // 앵커볼트 정착부의 압괴 검토             
	BOOL bOKBoltSideComp;    // 앵커볼트 주변부의 압렬 검토             
	BOOL bOKShearBoltSide;   // 전단에 의한 앵커볼트 주변의 압괴 검토   
	BOOL bOKShearConcCorner; // 전단에 의한 입상부의 벗겨져 떨어짐 검토 
	BOOL bOKPlateBrokenPu1;  // 베이스 플래이트의 파단 검토
	BOOL bOKPlateBrokenPu2;  // 베이스 플래이트의 파단 검토
	BOOL bOKPlateBrokentau;  // 베이스 플래이트의 파단 검토

	// 종국시 응력에 의한 단면검토

	double dXn_Length;     // 하단에서 부터 중립축까지의 길이
	double dSigma_c;       // 콘크리트 응력      
	double dFc;            // 콘크리트 허용응력
	double dSigma_y;       // 앵커볼트 응력
	double dbFy;           // 앵커볼트 허용응력
	// 베이스 플래이트의 파단 검토

	double dAlpha_Q;       
	double dPu1;
	double dPu2;
	double dAlpha_t;
	double dtau;

	double dAe; // Base Plate의 유효 단면적

	double dAl; // 앵커볼트 구멍 주위의 단면적


	void Initialize()
	{
		dMpc = 0.0;
		
		dnt = 0.0;
		dab = 0.0;
		ddt = 0.0;
		dNu = 0.0;
		dTu = 0.0;
		dMu = 0.0;
		
		dSu = 0.0;
		dQfu = 0.0;
		dQsu = 0.0;	
		dQu = 0.0;
		
		dbSigmay = 0.0;
		ddty = 0.0;
		ddc = 0.0;
		dMy = 0.0;
		
		dQa = 0.0;
		dbQa = 0.0;
		dQy = 0.0;


		bConcCorner = FALSE;      
		bConcComp = FALSE;        
		bBoltFixComp = FALSE;     
		bBoltSideComp = FALSE;    
		bShearBoltSide = FALSE;   
		bShearConcCorner = FALSE; 
		bUltStressChk = FALSE;    
		bPlateBroken = FALSE;     
		
		dc1 = 0.0;
		dc2 = 0.0;
		dc3 = 0.0;
		dc4 = 0.0;
		de1 = 0.0;
		de2 = 0.0;
		dAa = 0.0;
		dAc = 0.0;

		bOKConcCorner      = FALSE;     
		bOKConcComp        = FALSE;       
		bOKBoltFixComp     = FALSE;    
		bOKBoltSideComp    = FALSE;   
		bOKShearBoltSide   = FALSE;  
		bOKShearConcCorner = FALSE;
		bOKPlateBrokenPu1  = FALSE; 
		bOKPlateBrokenPu2  = FALSE; 
		bOKPlateBrokentau  = FALSE; 

		dXn_Length = 0.0;
		dSigma_c = 0.0;  
		dFc = 0.0;       
		dSigma_y = 0.0;  
		dbFy = 0.0;   

		dAlpha_Q = 0.0;       
		dPu1 = 0.0;
		dPu2 = 0.0;
		dAlpha_t = 0.0;
		dtau = 0.0;

		dAe = 0.0;
		dAl = 0.0;
	}
};

struct _JUD_BASEPLATE_RES_BASE_UNIT
{
	_JUD_BASEPLATE_RES_BASE_UNIT()  
	{		
		dMpc       = D_UNITSYS_BASE_MOMENT;		
		dab        = D_UNITSYS_BASE_AREA;
		ddt        = D_UNITSYS_BASE_LENGTH;
		dNu        = D_UNITSYS_BASE_FORCE;
		dTu        = D_UNITSYS_BASE_FORCE;
		dMu        = D_UNITSYS_BASE_MOMENT;		
		dSu        = D_UNITSYS_BASE_FORCE;
		dQfu       = D_UNITSYS_BASE_FORCE;
		dQsu       = D_UNITSYS_BASE_FORCE;	
		dQu        = D_UNITSYS_BASE_FORCE;		
		dbSigmay   = D_UNITSYS_BASE_STRESS;
		ddty       = D_UNITSYS_BASE_LENGTH;
		ddc        = D_UNITSYS_BASE_LENGTH;
		dMy        = D_UNITSYS_BASE_MOMENT;		
		dQa        = D_UNITSYS_BASE_FORCE;
		dbQa       = D_UNITSYS_BASE_FORCE;
		dQy        = D_UNITSYS_BASE_FORCE;		
		dc1        = D_UNITSYS_BASE_STRESS;
		dc2        = D_UNITSYS_BASE_STRESS;
		dc3        = D_UNITSYS_BASE_STRESS;
		dc4        = D_UNITSYS_BASE_STRESS;
		de1        = D_UNITSYS_BASE_LENGTH;
		de2        = D_UNITSYS_BASE_LENGTH;
		dAa        = D_UNITSYS_BASE_AREA;
		dAc        = D_UNITSYS_BASE_AREA;
		dXn_Length = D_UNITSYS_BASE_LENGTH; 
		dSigma_c   = D_UNITSYS_BASE_STRESS; 
		dFc        = D_UNITSYS_BASE_STRESS; 
		dSigma_y   = D_UNITSYS_BASE_STRESS; 
		dbFy       = D_UNITSYS_BASE_STRESS; 
		dAlpha_Q   = D_UNITSYS_BASE_FORCE;  
		dPu1       = D_UNITSYS_BASE_FORCE;
		dPu2       = D_UNITSYS_BASE_FORCE;
		dAlpha_t   = D_UNITSYS_BASE_STRESS;
		dtau       = D_UNITSYS_BASE_STRESS;
		dAe        = D_UNITSYS_BASE_AREA; 
		dAl        = D_UNITSYS_BASE_AREA; 
	}
	int dMpc;

	int dab;
	int ddt;
	int dNu;
	int dTu;
	int dMu;
	
	int dSu;
	int dQfu;
	int dQsu;	
	int dQu;
	
	int dbSigmay;
	int ddty;
	int ddc;
	int dMy;
	
	int dQa;
	int dbQa;
	int dQy;
	
	int dc1;
	int dc2;
	int dc3;
	int dc4;
	int de1;
	int de2;
	int dAa;
	int dAc;

	int dXn_Length;
	int dSigma_c;
	int dFc;
	int dSigma_y;
	int dbFy;

	int dAlpha_Q;
	int dPu1;
	int dPu2;
	int dAlpha_t;
	int dtau;

	int dAe;
	int dAl;
};

struct _JUD_BASEPLATE_RES_D
{// 주각 2차설계 결과

	int nType; // 출력형식(0:메이커제, 1~12:S조 노출형(플로우번호 1,3,4,5,8,9,11,12))
	           //  0 : dP, dM, dQ, Data[i].dMu, Data[i].dQu
	           //  1, 3,4,5,8,11 : dP, dM, dQ, Data[i]의 Mpc, Mu, My, Qy 계열
	           //  9 : dP, dM, dQ, Data[i]의 Mpc, Mu, Qu, 앵커볼트의 파단검토, 베이스 플래이트의 파단 검토

	           // 12 : dP, dM, dQ, Data[i]의 Mpc, Mu, My, Qy, 종국시 응력에 의한 단면검토, 베이스 플래이트의 파단 검토


	// 모멘트 [0]:yy [1]:zz
	// 전단력 [0]:z  [1]:y
	double dP;        //
	double dM[2];     // 
	double dQ[2];     //
	
	double dAlpha;
	double dGamma;

	_JUD_BASEPLATE_RES_BASE Data[2];
	
	void Initialize()
	{
		nType  = 0;

		dAlpha = 1.0;
		dGamma = 1.0;
		dP     = 0.0;
		for(int i=0 ; i<2 ; i++)
		{
			dM[i] = 0.0;
			dQ[i] = 0.0;
			Data[i].Initialize();
		}
	}

	_JUD_BASEPLATE_RES_D() {}
	_JUD_BASEPLATE_RES_D(const _JUD_BASEPLATE_RES_D& src) { *this = src; }
	_JUD_BASEPLATE_RES_D& operator= (const _JUD_BASEPLATE_RES_D& src)
	{
		nType  = src.nType ;
		
		dAlpha = src.dAlpha;
		dGamma = src.dGamma;
		dP     = src.dP    ;
		for(int i=0 ; i<2 ; i++)
		{
			dM[i]   = src.dM[i]  ;
			dQ[i]   = src.dQ[i]  ;
			Data[i] = src.Data[i];
		}
		return *this;
	}
};
struct _JUD_BASEPLATE_RES_UNIT
{
	_JUD_BASEPLATE_RES_UNIT()  
	{
		dP = D_UNITSYS_BASE_FORCE;
		dM = D_UNITSYS_BASE_MOMENT;  
		dQ = D_UNITSYS_BASE_FORCE;  
	} 
	int dP;
	int dM;
	int dQ;
};

// 구조계산서 입력정보 데이터
struct T_JUDR_RPOP_FONT
{
	CString   csFontName;
	int       nFontSize;
	BOOL      bBold;
	BOOL      bItalic;
	BOOL      bUnderLine;
	BOOL      bStrikeOut;
	COLORREF  rgbFontColor;

	void Initialize()
	{
#if defined(_JP)
		csFontName   = "Meiryo UI";
#else
		csFontName   = "MS PGothic";
#endif
		nFontSize    = 9;
		bBold        = FALSE;
		bItalic      = FALSE;
		bUnderLine   = FALSE;
		bStrikeOut   = FALSE;
		rgbFontColor = RGB(0,0,0);
	}
};

struct T_JUDR_RPOP_FONT_UDRD_D
{
	T_JUDR_RPOP_FONT data;
};

struct T_JUDR_RPOP_FONT_CH
{
	char      csFontName[80];
	int       nFontSize;
	BOOL      bBold;
	BOOL      bItalic;
	BOOL      bUnderLine;
	BOOL      bStrikeOut;
	COLORREF  rgbFontColor;

	void ConvertToString(T_JUDR_RPOP_FONT& rData)
	{
		ConvertCharStr(csFontName, rData.csFontName, sizeof(csFontName));
		rData.nFontSize=nFontSize;
		rData.bBold=bBold;
		rData.bItalic=bItalic;
		rData.bUnderLine=bUnderLine;
		rData.bStrikeOut=bStrikeOut;
		rData.rgbFontColor=rgbFontColor;
	}
	void ConvertToChar(T_JUDR_RPOP_FONT& rData)
	{
		ConvertStrChar(rData.csFontName, csFontName, sizeof(csFontName));
		nFontSize=rData.nFontSize;
		bBold=rData.bBold;
		bItalic=rData.bItalic;
		bUnderLine=rData.bUnderLine;
		bStrikeOut=rData.bStrikeOut;
		rgbFontColor=rData.rgbFontColor;
	}
};

struct T_JUDR_RPOP_FONT_UNIT
{
	T_JUDR_RPOP_FONT_UNIT()
	{
		csFontName = D_UNITSYS_NONE;
		nFontSize = D_UNITSYS_NONE;
		bBold = D_UNITSYS_NONE;
		bItalic = D_UNITSYS_NONE;
		bUnderLine = D_UNITSYS_NONE;
		bStrikeOut = D_UNITSYS_NONE;
		rgbFontColor = D_UNITSYS_NONE;
	}
	int csFontName;
	int nFontSize;
	int bBold;
	int bItalic;
	int bUnderLine;
	int bStrikeOut;
	int rgbFontColor;
};

struct T_JUDR_RPOP_PAPER  // 용지여백

{
	// 단위계 - mm (fixed)

	double dMarginTop;      // 여백 : 상, 하, 좌, 우

	double dMarginBot;
	double dMarginLeft;
	double dMarginRight;

	double dPaperSizeX;     // Paper Size
	double dPaperSizeY; 
	
	//double dBindTop;      // 제본 - 위쪽
	//double dBindLeft;     // 제본 - 왼쪽
	
	void Initialize()
	{              
		dMarginTop   = 20.0;  
		dMarginBot   = 15.0;
		dMarginLeft  = 30.0;
		dMarginRight = 30.0;

		dPaperSizeX  = 210.0;
		dPaperSizeY  = 297.0;
		
		//dBindTop     = 0.0;    
		//dBindLeft    = 0.0; 
	}
};

struct T_JUDR_RPOP_PAPER_UDRD_D
{
	T_JUDR_RPOP_PAPER data;
};

struct T_JUDR_RPOP_PAPER_UNIT
{
	T_JUDR_RPOP_PAPER_UNIT()
	{
		dMarginTop = D_UNITSYS_NONE;
		dMarginBot = D_UNITSYS_NONE;
		dMarginLeft = D_UNITSYS_NONE;
		dMarginRight = D_UNITSYS_NONE;
		dPaperSizeX = D_UNITSYS_NONE;
		dPaperSizeY = D_UNITSYS_NONE;
		//dBindTop = D_UNITSYS_NONE;
		//dBindLeft = D_UNITSYS_NONE;
	}
	int dMarginTop;
	int dMarginBot;
	int dMarginLeft;
	int dMarginRight;
	int dPaperSizeX;
	int dPaperSizeY;
	//int dBindTop;
	//int dBindLeft;
};

struct T_JUDR_RPOP_800
{
	// 설정
	T_JUDR_RPOP_FONT  TextFont;      // 텍스트 폰트
	T_JUDR_RPOP_FONT  ImageFont;     // 이미지 폰트
	T_JUDR_RPOP_PAPER Paper;         // 용지여백
	
	double dLineSpace ;              // 행간격 (텍스트 폰트에 대한 %)
		
	int  nDivideHor_Input;           // 그림 분할개수
	int  nDivideVer_Input;        // 0:1, 1:2=, 2:2||, 3:4, 4:6
	
	int  nDivideHor_Output;           
	int  nDivideVer_Output;
};

struct T_JUDR_RPOP_805
{
	// 설정
	T_JUDR_RPOP_FONT  TextFont;      // 텍스트 폰트
	T_JUDR_RPOP_FONT  ImageFont;     // 이미지 폰트
	T_JUDR_RPOP_PAPER Paper;         // 용지여백
	
	
	double dLineSpace ;              // 행간격 (텍스트 폰트에 대한 %)
	
	//   double dScaleHor_Input;          // 그림의 Scale, 단위 %
	//   double dScaleVer_Input;
	//   double dScaleHor_Output;
	//   double dScaleVer_Output;
	
	int  nDivideHor_Input;           // 그림 분할개수
	int  nDivideVer_Input_YZ;        // 0:1, 1:2=, 2:2||, 3:4, 4:6
	int  nDivideVer_Input_XZ;        // 0:1, 1:2=, 2:2||, 3:4, 4:6
	
	int  nDivideHor_Output;           
	int  nDivideVer_Output_YZ;
	int  nDivideVer_Output_XZ;
	
	// 레이아웃.
	int  nLayout_Hor_Pos;
	int  nLayout_Hor_Num;
	
	int  nLayout_Ver_YZ_Pos;
	int  nLayout_Ver_YZ_Num;
	int  nLayout_Ver_XZ_Pos;
	int  nLayout_Ver_XZ_Num;
	
	// 그림 옵션.
	BOOL bDrawRect;
};

#define A3_COUNT 30
#define A3_COUNT_JFD 10 // 기초 모듈 관련
struct T_JUDR_RPOP
{
	// 설정
	T_JUDR_RPOP_FONT  TextFont;      // 텍스트 폰트
	T_JUDR_RPOP_FONT  ImageFont;     // 이미지 폰트
	T_JUDR_RPOP_PAPER Paper;         // 용지여백


	double dLineSpace ;              // 행간격 (텍스트 폰트에 대한 %)
	
//   double dScaleHor_Input;          // 그림의 Scale, 단위 %
//   double dScaleVer_Input;
//   double dScaleHor_Output;
//   double dScaleVer_Output;

	int  nDivideHor_Input;           // 그림 분할개수
	int  nDivideVer_Input_YZ;        // 0:1, 1:2=, 2:2||, 3:4, 4:6
	int  nDivideVer_Input_XZ;        // 0:1, 1:2=, 2:2||, 3:4, 4:6

	int  nDivideHor_Output;           
	int  nDivideVer_Output_YZ;
	int  nDivideVer_Output_XZ;

	// 레이아웃.
	int  nLayout_Hor_Pos;
	int  nLayout_Hor_Num;

	int  nLayout_Ver_YZ_Pos;
	int  nLayout_Ver_YZ_Num;
	int  nLayout_Ver_XZ_Pos;
	int  nLayout_Ver_XZ_Num;
	
	// 그림 옵션.
	BOOL bDrawRect;

	BOOL bUseAutoFont;
	int  nFontSize[A3_COUNT];
	int  nAutoSize[A3_COUNT];
	BOOL bPrintA3[A3_COUNT];

	int  nFontSize_JFD[A3_COUNT_JFD];
	int  nAutoSize_JFD[A3_COUNT_JFD];
	BOOL bPrintA3_JFD[A3_COUNT_JFD];

	void Initialize()
	{
		TextFont.Initialize();
		ImageFont.Initialize();
		Paper.Initialize();

		dLineSpace  = 150.0; 
		
//     dScaleHor_Input  = 100.0; 
//     dScaleVer_Input  = 100.0;
//     dScaleHor_Output = 100.0;
//     dScaleVer_Output = 100.0;

		nDivideHor_Input  = 0;
		nDivideVer_Input_YZ  = 0;
		nDivideVer_Input_XZ  = 0;
		nDivideHor_Output = 0;
		nDivideVer_Output_YZ = 0;
		nDivideVer_Output_XZ = 0;

		nLayout_Hor_Pos = 0;
		nLayout_Hor_Num = 2;

		nLayout_Ver_YZ_Pos = 0;
		nLayout_Ver_YZ_Num = 2;
		nLayout_Ver_XZ_Pos = 0;
		nLayout_Ver_XZ_Num = 2;

		bDrawRect = TRUE;
		bUseAutoFont = TRUE;
		for(int i=0; i<A3_COUNT; i++)
		{
			nFontSize[i]=9;
			nAutoSize[i]=0;
			bPrintA3[i]=FALSE;
		}

		for(int i=0; i<A3_COUNT_JFD; i++)
		{
			nFontSize_JFD[i]=9;
			nAutoSize_JFD[i]=0;
			bPrintA3_JFD[i]=FALSE;
		}
	}

	void Convert800(T_JUDR_RPOP_800& rData)
	{
		TextFont      = rData.TextFont    ;
		ImageFont     = rData.ImageFont    ;
		Paper         = rData.Paper   ;
	 
		dLineSpace    = rData.dLineSpace;

		nDivideHor_Input     = rData.nDivideHor_Input;
		nDivideVer_Input_YZ  = rData.nDivideVer_Input;
		nDivideVer_Input_XZ  = rData.nDivideVer_Input;
		nDivideHor_Output    = rData.nDivideHor_Output;
		nDivideVer_Output_YZ = rData.nDivideVer_Output;
		nDivideVer_Output_XZ = rData.nDivideVer_Output;

		nLayout_Hor_Pos = 0;
		nLayout_Hor_Num = 2;
		
		nLayout_Ver_YZ_Pos = 0;
		nLayout_Ver_YZ_Num = 2;
		nLayout_Ver_XZ_Pos = 0;
		nLayout_Ver_XZ_Num = 2;

		bDrawRect = TRUE;
		bUseAutoFont = TRUE;
		for(int i=0; i<A3_COUNT; i++)
		{
			nFontSize[i]=9;
			nAutoSize[i]=0;
			bPrintA3[i]=FALSE;
		}

		for(int i=0; i<A3_COUNT_JFD; i++)
		{
			nFontSize_JFD[i]=9;
			nAutoSize_JFD[i]=0;
			bPrintA3_JFD[i]=FALSE;
		}
	}

	void Convert805(T_JUDR_RPOP_805& rData)
	{
		TextFont      = rData.TextFont ;
		ImageFont     = rData.ImageFont;
		Paper         = rData.Paper    ;
		
		dLineSpace    = rData.dLineSpace;
		
		nDivideHor_Input     = rData.nDivideHor_Input;
		nDivideVer_Input_YZ  = rData.nDivideVer_Input_YZ;
		nDivideVer_Input_XZ  = rData.nDivideVer_Input_XZ;
		nDivideHor_Output    = rData.nDivideHor_Output;
		nDivideVer_Output_YZ = rData.nDivideVer_Output_YZ;
		nDivideVer_Output_XZ = rData.nDivideVer_Output_XZ;
		
		nLayout_Hor_Pos = rData.nLayout_Hor_Pos;
		nLayout_Hor_Num = rData.nLayout_Hor_Num;
		
		nLayout_Ver_YZ_Pos = rData.nLayout_Ver_YZ_Pos;
		nLayout_Ver_YZ_Num = rData.nLayout_Ver_YZ_Num;
		nLayout_Ver_XZ_Pos = rData.nLayout_Ver_XZ_Pos;
		nLayout_Ver_XZ_Num = rData.nLayout_Ver_XZ_Num;
		
		bDrawRect = rData.bDrawRect;
		bUseAutoFont = TRUE;
		for(int i=0; i<A3_COUNT; i++)
		{
			nFontSize[i]=9;
			nAutoSize[i]=0;
			bPrintA3[i]=FALSE;
		}

		for(int i=0; i<A3_COUNT_JFD; i++)
		{
			nFontSize_JFD[i]=9;
			nAutoSize_JFD[i]=0;
			bPrintA3_JFD[i]=FALSE;
		}
	}
};

struct T_JUDR_RPOP_UDRD_D
{
	T_JUDR_RPOP data;
};

struct T_JUDR_RPOP_UNIT
{
	T_JUDR_RPOP_UNIT()
	{
		dLineSpace       = D_UNITSYS_NONE;
//     dScaleHor_Input  = D_UNITSYS_NONE;
//     dScaleVer_Input  = D_UNITSYS_NONE;
//     dScaleHor_Output = D_UNITSYS_NONE;
//     dScaleVer_Output = D_UNITSYS_NONE;
		nDivideHor_Input  = D_UNITSYS_NONE;
		nDivideVer_Input  = D_UNITSYS_NONE;
		nDivideHor_Output = D_UNITSYS_NONE;
		nDivideVer_Output = D_UNITSYS_NONE;
	}
	int dLineSpace ;
//   int dScaleHor_Input; 
//   int dScaleVer_Input;
//   int dScaleHor_Output;
//   int dScaleVer_Output;
	int nDivideHor_Input;
	int nDivideVer_Input;
	int nDivideHor_Output;
	int nDivideVer_Output;
};

struct T_JUDR_RPOP_CH_800
{
	T_JUDR_RPOP_FONT_CH  TextFont; 
	T_JUDR_RPOP_FONT_CH  ImageFont;
	T_JUDR_RPOP_PAPER Paper;
	
	double dLineSpace ;
		
	int  nDivideHor_Input;        // 그림 분할개수
	int  nDivideVer_Input;        // 0:1, 1:2=, 2:2||, 3:4, 4:6
	
	int  nDivideHor_Output;           
	int  nDivideVer_Output;
};

struct T_JUDR_RPOP_CH_805
{
	T_JUDR_RPOP_FONT_CH  TextFont; 
	T_JUDR_RPOP_FONT_CH  ImageFont;
	T_JUDR_RPOP_PAPER Paper;
	
	double dLineSpace ;
	
	int  nDivideHor_Input;           // 그림 분할개수
	int  nDivideVer_Input_XY;        // 0:1, 1:2=, 2:2||, 3:4, 4:6
	int  nDivideVer_Input_XZ;        // 0:1, 1:2=, 2:2||, 3:4, 4:6
	
	int  nDivideHor_Output;           
	int  nDivideVer_Output_XY;
	int  nDivideVer_Output_XZ;
	
	// 레이아웃.
	int  nLayout_Hor_Pos;
	int  nLayout_Hor_Num;
	
	int  nLayout_Ver_XY_Pos;
	int  nLayout_Ver_XY_Num;
	int  nLayout_Ver_XZ_Pos;
	int  nLayout_Ver_XZ_Num;
	
	BOOL bDrawRect;
};

struct T_JUDR_RPOP_CH
{
	T_JUDR_RPOP_FONT_CH  TextFont; 
	T_JUDR_RPOP_FONT_CH  ImageFont;
	T_JUDR_RPOP_PAPER Paper;
	
	double dLineSpace ;
	
	int  nDivideHor_Input;           // 그림 분할개수
	int  nDivideVer_Input_XY;        // 0:1, 1:2=, 2:2||, 3:4, 4:6
	int  nDivideVer_Input_XZ;        // 0:1, 1:2=, 2:2||, 3:4, 4:6
	
	int  nDivideHor_Output;           
	int  nDivideVer_Output_XY;
	int  nDivideVer_Output_XZ;
	
	// 레이아웃.
	int  nLayout_Hor_Pos;
	int  nLayout_Hor_Num;
	
	int  nLayout_Ver_XY_Pos;
	int  nLayout_Ver_XY_Num;
	int  nLayout_Ver_XZ_Pos;
	int  nLayout_Ver_XZ_Num;

	BOOL bDrawRect;

	BOOL bUseAutoFont;
	int  nFontSize[A3_COUNT];
	int  nAutoSize[A3_COUNT];
	BOOL bPrintA3[A3_COUNT];
	int  nFontSize_JFD[A3_COUNT_JFD];
	int  nAutoSize_JFD[A3_COUNT_JFD];
	BOOL bPrintA3_JFD[A3_COUNT_JFD];


	void ConvertToString(T_JUDR_RPOP& rData)
	{
		TextFont.ConvertToString(rData.TextFont);
		ImageFont.ConvertToString(rData.ImageFont);
		rData.Paper = Paper;
		rData.dLineSpace  = dLineSpace ;

		rData.nDivideHor_Input    = nDivideHor_Input;
		rData.nDivideVer_Input_YZ = nDivideVer_Input_XY;
		rData.nDivideVer_Input_XZ = nDivideVer_Input_XZ;
		rData.nDivideHor_Output   = nDivideHor_Output;
		rData.nDivideVer_Output_YZ= nDivideVer_Output_XY;
		rData.nDivideVer_Output_XZ= nDivideVer_Output_XZ;

		rData.nLayout_Hor_Pos    = nLayout_Hor_Pos;
		rData.nLayout_Hor_Num    = nLayout_Hor_Num;
		rData.nLayout_Ver_YZ_Pos = nLayout_Ver_XY_Pos;
		rData.nLayout_Ver_YZ_Num = nLayout_Ver_XY_Num;
		rData.nLayout_Ver_XZ_Pos = nLayout_Ver_XZ_Pos;
		rData.nLayout_Ver_XZ_Num = nLayout_Ver_XZ_Num;
		
		rData.bDrawRect          = bDrawRect;
		rData.bUseAutoFont       = bUseAutoFont;
		memcpy(rData.nFontSize, nFontSize, sizeof(nFontSize));
		memcpy(rData.nAutoSize, nAutoSize, sizeof(nAutoSize));
		memcpy(rData.bPrintA3, bPrintA3, sizeof(bPrintA3));

		memcpy(rData.nFontSize_JFD, nFontSize_JFD, sizeof(nFontSize_JFD));
		memcpy(rData.nAutoSize_JFD, nAutoSize_JFD, sizeof(nAutoSize_JFD));
		memcpy(rData.bPrintA3_JFD, bPrintA3_JFD, sizeof(bPrintA3_JFD));
	}
	void ConvertToChar(T_JUDR_RPOP& rData)
	{
		TextFont.ConvertToChar(rData.TextFont);
		ImageFont.ConvertToChar(rData.ImageFont);
		Paper = rData.Paper;
		dLineSpace  = rData.dLineSpace ;

		nDivideHor_Input    = rData.nDivideHor_Input;
		nDivideVer_Input_XY = rData.nDivideVer_Input_YZ;
		nDivideVer_Input_XZ = rData.nDivideVer_Input_XZ;
		nDivideHor_Output   = rData.nDivideHor_Output;
		nDivideVer_Output_XY= rData.nDivideVer_Output_YZ;
		nDivideVer_Output_XZ= rData.nDivideVer_Output_XZ;
		
		nLayout_Hor_Pos    = rData.nLayout_Hor_Pos;
		nLayout_Hor_Num    = rData.nLayout_Hor_Num;
		nLayout_Ver_XY_Pos = rData.nLayout_Ver_YZ_Pos;
		nLayout_Ver_XY_Num = rData.nLayout_Ver_YZ_Num;
		nLayout_Ver_XZ_Pos = rData.nLayout_Ver_XZ_Pos;
		nLayout_Ver_XZ_Num = rData.nLayout_Ver_XZ_Num;

		bDrawRect          = rData.bDrawRect;
		bUseAutoFont       = rData.bUseAutoFont;
		memcpy(nFontSize, rData.nFontSize, sizeof(nFontSize));
		memcpy(nAutoSize, rData.nAutoSize, sizeof(nAutoSize));
		memcpy(bPrintA3, rData.bPrintA3, sizeof(bPrintA3));

		memcpy(nFontSize_JFD, rData.nFontSize_JFD, sizeof(nFontSize_JFD));
		memcpy(nAutoSize_JFD, rData.nAutoSize_JFD, sizeof(nAutoSize_JFD));
		memcpy(bPrintA3_JFD , rData.bPrintA3_JFD, sizeof(bPrintA3_JFD));
	}

	void Convert800(T_JUDR_RPOP_CH_800& rData)
	{
		memcpy(TextFont.csFontName, rData.TextFont.csFontName, sizeof(TextFont.csFontName));
		TextFont.nFontSize = rData.TextFont.nFontSize;
		TextFont.bBold     = rData.TextFont.bBold;
		TextFont.bItalic   = rData.TextFont.bItalic;
		TextFont.bUnderLine= rData.TextFont.bUnderLine;
		TextFont.bStrikeOut= rData.TextFont.bStrikeOut;
		TextFont.rgbFontColor= rData.TextFont.rgbFontColor;

		memcpy(ImageFont.csFontName, rData.ImageFont.csFontName, sizeof(ImageFont.csFontName));
		ImageFont.nFontSize = rData.ImageFont.nFontSize;
		ImageFont.bBold     = rData.ImageFont.bBold;
		ImageFont.bItalic   = rData.ImageFont.bItalic;
		ImageFont.bUnderLine= rData.ImageFont.bUnderLine;
		ImageFont.bStrikeOut= rData.ImageFont.bStrikeOut;
		ImageFont.rgbFontColor= rData.ImageFont.rgbFontColor;

		nDivideHor_Input    = rData.nDivideHor_Input;
		nDivideVer_Input_XY = rData.nDivideVer_Input;
		nDivideVer_Input_XZ = rData.nDivideVer_Input;
		nDivideHor_Output   = rData.nDivideHor_Output;
		nDivideVer_Output_XY= rData.nDivideVer_Output;
		nDivideVer_Output_XZ= rData.nDivideVer_Output;
		
		nLayout_Hor_Pos    = 0;
		nLayout_Hor_Num    = 2;
		nLayout_Ver_XY_Pos = 0;
		nLayout_Ver_XY_Num = 2;
		nLayout_Ver_XZ_Pos = 0;
		nLayout_Ver_XZ_Num = 2;
		bDrawRect          = TRUE;
		bUseAutoFont       = TRUE;
		for(int i=0; i<A3_COUNT; i++)
		{
			nFontSize[i]=9;
			nAutoSize[i]=0;
			bPrintA3[i]=FALSE;
		}
		for(int i=0; i<A3_COUNT_JFD; i++)
		{
			nFontSize_JFD[i]=9;
			nAutoSize_JFD[i]=0;
			bPrintA3_JFD[i]=FALSE;
		}
	}
	void Convert805(T_JUDR_RPOP_CH_805& rData)
	{
		memcpy(TextFont.csFontName, rData.TextFont.csFontName, sizeof(TextFont.csFontName));
		TextFont.nFontSize = rData.TextFont.nFontSize;
		TextFont.bBold     = rData.TextFont.bBold;
		TextFont.bItalic   = rData.TextFont.bItalic;
		TextFont.bUnderLine= rData.TextFont.bUnderLine;
		TextFont.bStrikeOut= rData.TextFont.bStrikeOut;
		TextFont.rgbFontColor= rData.TextFont.rgbFontColor;
		
		memcpy(ImageFont.csFontName, rData.ImageFont.csFontName, sizeof(ImageFont.csFontName));
		ImageFont.nFontSize = rData.ImageFont.nFontSize;
		ImageFont.bBold     = rData.ImageFont.bBold;
		ImageFont.bItalic   = rData.ImageFont.bItalic;
		ImageFont.bUnderLine= rData.ImageFont.bUnderLine;
		ImageFont.bStrikeOut= rData.ImageFont.bStrikeOut;
		ImageFont.rgbFontColor= rData.ImageFont.rgbFontColor;
		
		nDivideHor_Input    = rData.nDivideHor_Input;
		nDivideVer_Input_XY = rData.nDivideVer_Input_XY;
		nDivideVer_Input_XZ = rData.nDivideVer_Input_XZ;
		nDivideHor_Output   = rData.nDivideHor_Output;
		nDivideVer_Output_XY= rData.nDivideVer_Output_XY;
		nDivideVer_Output_XZ= rData.nDivideVer_Output_XZ;
		
		nLayout_Hor_Pos    = rData.nLayout_Hor_Pos    ;
		nLayout_Hor_Num    = rData.nLayout_Hor_Num    ;
		nLayout_Ver_XY_Pos = rData.nLayout_Ver_XY_Pos ;
		nLayout_Ver_XY_Num = rData.nLayout_Ver_XY_Num ;
		nLayout_Ver_XZ_Pos = rData.nLayout_Ver_XZ_Pos ;
		nLayout_Ver_XZ_Num = rData.nLayout_Ver_XZ_Num ;
		bDrawRect          = rData.bDrawRect;
		bUseAutoFont       = TRUE;
		for(int i=0; i<A3_COUNT; i++)
		{
			nFontSize[i]=9;
			nAutoSize[i]=0;
			bPrintA3[i]=FALSE;
		}
		for(int i=0; i<A3_COUNT_JFD; i++)
		{
			nFontSize_JFD[i]=9;
			nAutoSize_JFD[i]=0;
			bPrintA3_JFD[i]=FALSE;
		}
	}
};

struct T_JUDR_USER_INFORMATION
{
	CString strName;      // 설계사무소 명칭
	CString strUser;      // 담당자
	CString strRegistry;  // 건축사 등록번호
	CString strAddress;   // 연락처 (주소)
	CString strPhone;     // Tel
	
	void Initialize()
	{
		strName.Empty();
		strUser.Empty();
		strRegistry.Empty();
		strAddress.Empty();
		strPhone.Empty();
	}
};

struct T_JUDR_USER_INFORMATION_UDRD_D
{
	T_JUDR_USER_INFORMATION data;
};

struct T_JUDR_USER_INFORMATION_UNIT
{
	T_JUDR_USER_INFORMATION_UNIT()
	{
		strName = D_UNITSYS_NONE;      
		strUser = D_UNITSYS_NONE;      
		strRegistry = D_UNITSYS_NONE;  
		strAddress = D_UNITSYS_NONE;   
		strPhone = D_UNITSYS_NONE; 
	}
	int strName;      
	int strUser;      
	int strRegistry;  
	int strAddress;   
	int strPhone;     
};

struct T_JUDR_USER_INFORMATION_CH
{
	char strName[80];      
	char strUser[80];      
	char strRegistry[80];  
	char strAddress[80];   
	char strPhone[80]; 

	void ConvertToString(T_JUDR_USER_INFORMATION& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strUser, rData.strUser, sizeof(strUser));
		ConvertCharStr(strRegistry, rData.strRegistry, sizeof(strRegistry));
		ConvertCharStr(strAddress, rData.strAddress, sizeof(strAddress));
		ConvertCharStr(strPhone, rData.strPhone, sizeof(strPhone));
	}
	void ConvertToChar(T_JUDR_USER_INFORMATION& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strUser, strUser, sizeof(strUser));
		ConvertStrChar(rData.strRegistry, strRegistry, sizeof(strRegistry));
		ConvertStrChar(rData.strAddress, strAddress, sizeof(strAddress));
		ConvertStrChar(rData.strPhone, strPhone, sizeof(strPhone));
	}
};

struct T_JUDR_COVER
{
	// 0.표지

	CString strProject;
	CString strInfoOS;

	T_JUDR_USER_INFORMATION Designer;         // 구조설계사무소
	T_JUDR_USER_INFORMATION Cooperator;    // 구조설계협력사무소

	void Initialize()
	{
		strProject.Empty();
		strInfoOS.Empty();
		Designer.Initialize();
		Cooperator.Initialize();
	}
};

struct T_JUDR_COVER_UDRD_D
{
	T_JUDR_COVER data;
};

struct T_JUDR_COVER_UNIT
{
	T_JUDR_COVER_UNIT()
	{
		strProject = D_UNITSYS_NONE;
		strInfoOS = D_UNITSYS_NONE;
	}
	int strProject;
	int strInfoOS;
};

struct T_JUDR_COVER_CH
{
	char strProject[80];
	char strInfoOS[80];

	T_JUDR_USER_INFORMATION_CH Designer;
	T_JUDR_USER_INFORMATION_CH Cooperator;
	
	void ConvertToString(T_JUDR_COVER& rData)
	{
		ConvertCharStr(strProject, rData.strProject, sizeof(strProject));
		ConvertCharStr(strInfoOS, rData.strInfoOS, sizeof(strInfoOS));
		Designer.ConvertToString(rData.Designer);
		Cooperator.ConvertToString(rData.Cooperator);
	}
	void ConvertToChar(T_JUDR_COVER& rData)
	{
		ConvertStrChar(rData.strProject, strProject, sizeof(strProject));
		ConvertStrChar(rData.strInfoOS, strInfoOS, sizeof(strInfoOS));
		Designer.ConvertToChar(rData.Designer);
		Cooperator.ConvertToChar(rData.Cooperator);
	}
	void Convert800(T_JUDR_COVER_CH& rData)
	{
		memcpy(strProject, rData.strProject, sizeof(strProject));
		memcpy(strInfoOS, rData.strInfoOS, sizeof(strInfoOS));

		memcpy(Designer.strName, rData.Designer.strName , sizeof(Designer.strName));     
		memcpy(Designer.strUser, rData.Designer.strUser , sizeof(Designer.strUser));
		memcpy(Designer.strRegistry, rData.Designer.strRegistry , sizeof(Designer.strRegistry));
		memcpy(Designer.strAddress, rData.Designer.strAddress , sizeof(Designer.strAddress));
		memcpy(Designer.strPhone, rData.Designer.strPhone , sizeof(Designer.strPhone));

		memcpy(Cooperator.strName, rData.Cooperator.strName , sizeof(Cooperator.strName));     
		memcpy(Cooperator.strUser, rData.Cooperator.strUser , sizeof(Cooperator.strUser));
		memcpy(Cooperator.strRegistry, rData.Cooperator.strRegistry , sizeof(Cooperator.strRegistry));
		memcpy(Cooperator.strAddress, rData.Cooperator.strAddress , sizeof(Cooperator.strAddress));
		memcpy(Cooperator.strPhone, rData.Cooperator.strPhone , sizeof(Cooperator.strPhone));
	}
};

struct T_JUDR_GENERAL
{
	// 1.일반사항
	CString strSite;        // 건축장소
	CString strUse;         // 용도
	CString strType;        // 구조종별

	//double dBuildingArea;   // 건축면적

	double dTotalArea;      // 총연면적

	double dGroundLevel;    // GL
	double dEaves;          // 처마높이
	double dHeight;         // 건축물 높이
	//double dDepth;          // 기초바닥깊이
	double dParapet;        // 파라펫 높이

	CString strUpperX;      // 상부구조형식 : X방향

	CString strUpperY;      // 상부구조형식 : X방향

	int     nConstType;     // 공사종별  0:신축 , 1:증축 
	BOOL    bExtend;        // 증축예정  T:있음 , F:없음
	CString strExtStory;    // 증축예정 층수


	CString strBaseType;    // 기초구조형식
	CString strFinish;      // 마무리
	BOOL    bRoofAcc;       // 옥상부속물 유무 - T:있음 , F:없음
	CString strRoofAcc;     // 옥상부속물 

	void Initialize()
	{
		strSite.Empty();      
		strUse.Empty();       
		strType.Empty();      
		
		//dBuildingArea = 0.0; 
		dTotalArea = 0.0;    
		dGroundLevel = 0.0;  
		dEaves = 0.0;        
		dHeight = 0.0;       
		//dDepth = 0.0;        
		dParapet = 0.0;      
		
		strUpperX.Empty();    
		strUpperY.Empty();    
		nConstType = 0;;   
		bExtend = FALSE;      
		strExtStory.Empty();    
		
		strBaseType.Empty();
		strFinish.Empty();    
		bRoofAcc = FALSE;     
		strRoofAcc.Empty();   
	}
};

struct T_JUDR_GENERAL_UDRD_D
{
	T_JUDR_GENERAL data;
};

struct T_JUDR_GENERAL_UNIT
{
	T_JUDR_GENERAL_UNIT()
	{
		strSite = D_UNITSYS_NONE;
		strUse = D_UNITSYS_NONE;
		strType = D_UNITSYS_NONE;
		//dBuildingArea = D_UNITSYS_BASE_AREA;
		dTotalArea = D_UNITSYS_BASE_AREA;
		dGroundLevel = D_UNITSYS_BASE_LENGTH;
		dEaves = D_UNITSYS_BASE_LENGTH;
		dHeight = D_UNITSYS_BASE_LENGTH;
		//dDepth = D_UNITSYS_BASE_LENGTH;
		dParapet = D_UNITSYS_BASE_LENGTH;
		strUpperX = D_UNITSYS_NONE;
		strUpperY = D_UNITSYS_NONE;
		nConstType = D_UNITSYS_NONE;
		bExtend = D_UNITSYS_NONE;
		strExtStory = D_UNITSYS_NONE;
		strBaseType = D_UNITSYS_NONE;
		strFinish = D_UNITSYS_NONE;
		bRoofAcc = D_UNITSYS_NONE;
		strRoofAcc = D_UNITSYS_NONE;
	}
	int strSite;
	int strUse;
	int strType;
	//int dBuildingArea;
	int dTotalArea;
	int dGroundLevel;
	int dEaves;
	int dHeight;
	//int dDepth;
	int dParapet;
	int strUpperX;
	int strUpperY;
	int nConstType;
	int bExtend;
	int strExtStory;
	int strBaseType;
	int strFinish;
	int bRoofAcc;
	int strRoofAcc;
};

struct T_JUDR_GENERAL_CH
{
	char strSite[80];      
	char strUse[80];      
	char strType[80];     
	
	//double dBuildingArea;
	double dTotalArea;   
	double dGroundLevel; 
	double dEaves;       
	double dHeight;      
	//double dDepth;       
	double dParapet;     
	
	char strUpperX[80];   
	char strUpperY[80];   
	int  nConstType;  
	BOOL bExtend;     
	char strExtStory[20]; 
	
	char strBaseType[8192]; 
	char strFinish[8192];   
	BOOL bRoofAcc;    
	char strRoofAcc[8192];  

	void ConvertToString(T_JUDR_GENERAL& rData)
	{
		ConvertCharStr(strSite    , rData.strSite    , sizeof(strSite    ));
		ConvertCharStr(strUse     , rData.strUse     , sizeof(strUse     ));
		ConvertCharStr(strType    , rData.strType    , sizeof(strType    ));
		ConvertCharStr(strUpperX  , rData.strUpperX  , sizeof(strUpperX  ));
		ConvertCharStr(strUpperY  , rData.strUpperY  , sizeof(strUpperY  ));
		ConvertCharStr(strExtStory, rData.strExtStory, sizeof(strExtStory));
		ConvertCharStr(strBaseType, rData.strBaseType, sizeof(strBaseType));
		ConvertCharStr(strFinish  , rData.strFinish  , sizeof(strFinish  ));
		ConvertCharStr(strRoofAcc , rData.strRoofAcc , sizeof(strRoofAcc ));

		//rData.dBuildingArea = dBuildingArea;
		rData.dTotalArea    = dTotalArea;
		rData.dGroundLevel  = dGroundLevel;
		rData.dEaves        = dEaves;
		rData.dHeight       = dHeight;
		//rData.dDepth        = dDepth;
		rData.dParapet      = dParapet;
		rData.nConstType    = nConstType;
		rData.bExtend       = bExtend;
		rData.bRoofAcc      = bRoofAcc;
	}                     
	void ConvertToChar(T_JUDR_GENERAL& rData)
	{
		ConvertStrChar(rData.strSite    , strSite    , sizeof(strSite    ));
		ConvertStrChar(rData.strUse     , strUse     , sizeof(strUse     ));
		ConvertStrChar(rData.strType    , strType    , sizeof(strType    ));
		ConvertStrChar(rData.strUpperX  , strUpperX  , sizeof(strUpperX  ));
		ConvertStrChar(rData.strUpperY  , strUpperY  , sizeof(strUpperY  ));
		ConvertStrChar(rData.strExtStory, strExtStory, sizeof(strExtStory));
		ConvertStrChar(rData.strBaseType, strBaseType, sizeof(strBaseType));
		ConvertStrChar(rData.strFinish  , strFinish  , sizeof(strFinish  ));
		ConvertStrChar(rData.strRoofAcc , strRoofAcc , sizeof(strRoofAcc ));

		//dBuildingArea = rData.dBuildingArea;
		dTotalArea    = rData.dTotalArea;
		dGroundLevel  = rData.dGroundLevel;
		dEaves        = rData.dEaves;
		dHeight       = rData.dHeight;
		//dDepth        = rData.dDepth;
		dParapet      = rData.dParapet;
		nConstType    = rData.nConstType;
		bExtend       = rData.bExtend;
		bRoofAcc      = rData.bRoofAcc;
	}

	void Convert800(T_JUDR_GENERAL_CH& rData)
	{
		memcpy(strSite    , rData.strSite    , sizeof(strSite    ));
		memcpy(strUse     , rData.strUse     , sizeof(strUse     ));
		memcpy(strType    , rData.strType    , sizeof(strType    ));
		memcpy(strUpperX  , rData.strUpperX  , sizeof(strUpperX  ));
		memcpy(strUpperY  , rData.strUpperY  , sizeof(strUpperY  ));
		memcpy(strExtStory, rData.strExtStory, sizeof(strExtStory));
		memcpy(strBaseType, rData.strBaseType, sizeof(strBaseType));
		memcpy(strFinish  , rData.strFinish  , sizeof(strFinish  ));
		memcpy(strRoofAcc , rData.strRoofAcc , sizeof(strRoofAcc ));
		
		//dBuildingArea = rData.dBuildingArea;
		dTotalArea    = rData.dTotalArea;
		dGroundLevel  = rData.dGroundLevel;
		dEaves        = rData.dEaves;
		dHeight       = rData.dHeight;
		//dDepth        = rData.dDepth;
		dParapet      = rData.dParapet;
		nConstType    = rData.nConstType;
		bExtend       = rData.bExtend;
		bRoofAcc      = rData.bRoofAcc;
	}
};

struct T_JUDR_POLICY
{
	// 2.설계방침과 사용재료

	CString strDesignUpper;   // 구조설계방침 - 상부구조
	CString strDesignBase;    // 구조설계방침 - 기초구조
	CString strCalcUpper;     // 구조계산방침 - 상부구조
	CString strCalcBase;      // 구조계산방침 - 기초구조
	CString strTools;         // 사용프로그램 외
	CString strGuide;         // 지침/규준
	CString strResearch;      // 조사/연구결과

	CString strReference;     // 참고문헌


	void Initialize()
	{
		strDesignUpper.Empty();
		strDesignBase.Empty();
		strCalcUpper.Empty();
		strCalcBase.Empty();
		strTools.Empty();
		strGuide.Empty();
		strResearch.Empty();
		strResearch.Empty();
	}
};

struct T_JUDR_POLICY_UDRD_D
{
	T_JUDR_POLICY data;
};

struct T_JUDR_POLICY_UNIT
{
	T_JUDR_POLICY_UNIT()
	{
		strDesignUpper = D_UNITSYS_NONE;
		strDesignBase = D_UNITSYS_NONE; 
		strCalcUpper = D_UNITSYS_NONE;  
		strCalcBase = D_UNITSYS_NONE;   
		strTools = D_UNITSYS_NONE;      
		strGuide = D_UNITSYS_NONE;      
		strResearch = D_UNITSYS_NONE;   
		strReference = D_UNITSYS_NONE;  
	}
	int strDesignUpper;
	int strDesignBase; 
	int strCalcUpper;  
	int strCalcBase;   
	int strTools;      
	int strGuide;      
	int strResearch;   
	int strReference;  
};

struct T_JUDR_POLICY_CH
{
	char strDesignUpper[8192];
	char strDesignBase[8192]; 
	char strCalcUpper[8192];  
	char strCalcBase[8192];   
	char strTools[8192];      
	char strGuide[8192];      
	char strResearch[8192];   
	char strReference[8192]; 

	void ConvertToString(T_JUDR_POLICY& rData)
	{
		ConvertCharStr(strDesignUpper, rData.strDesignUpper, sizeof(strDesignUpper));
		ConvertCharStr(strDesignBase , rData.strDesignBase , sizeof(strDesignBase ));
		ConvertCharStr(strCalcUpper  , rData.strCalcUpper  , sizeof(strCalcUpper  ));
		ConvertCharStr(strCalcBase   , rData.strCalcBase   , sizeof(strCalcBase   ));
		ConvertCharStr(strTools      , rData.strTools      , sizeof(strTools      ));
		ConvertCharStr(strGuide      , rData.strGuide      , sizeof(strGuide      ));
		ConvertCharStr(strResearch   , rData.strResearch   , sizeof(strResearch   ));
		ConvertCharStr(strReference  , rData.strReference  , sizeof(strReference  ));
	}                     
	void ConvertToChar(T_JUDR_POLICY& rData)
	{
		ConvertStrChar(rData.strDesignUpper, strDesignUpper, sizeof(strDesignUpper));
		ConvertStrChar(rData.strDesignBase , strDesignBase , sizeof(strDesignBase ));
		ConvertStrChar(rData.strCalcUpper  , strCalcUpper  , sizeof(strCalcUpper  ));
		ConvertStrChar(rData.strCalcBase   , strCalcBase   , sizeof(strCalcBase   ));
		ConvertStrChar(rData.strTools      , strTools      , sizeof(strTools      ));
		ConvertStrChar(rData.strGuide      , strGuide      , sizeof(strGuide      ));
		ConvertStrChar(rData.strResearch   , strResearch   , sizeof(strResearch   ));
		ConvertStrChar(rData.strReference  , strReference  , sizeof(strReference  ));
	}

	void Convert800(T_JUDR_POLICY_CH& rData)
	{
		memcpy(strDesignUpper, rData.strDesignUpper, sizeof(strDesignUpper));
		memcpy(strDesignBase , rData.strDesignBase , sizeof(strDesignBase ));
		memcpy(strCalcUpper  , rData.strCalcUpper  , sizeof(strCalcUpper  ));
		memcpy(strCalcBase   , rData.strCalcBase   , sizeof(strCalcBase   ));
		memcpy(strTools      , rData.strTools      , sizeof(strTools      ));
		memcpy(strGuide      , rData.strGuide      , sizeof(strGuide      ));
		memcpy(strResearch   , rData.strResearch   , sizeof(strResearch   ));
		memcpy(strReference  , rData.strReference  , sizeof(strReference  ));
	} 
};

struct T_JUDR_PROGRAM
{
	// 3.프로그램사용현황
	CString strMessage;
	CString strComment;   // 설계자로서의 생각 
	CString strETC;       // 기타

	void Initialize()
	{
		strMessage.Empty();
		strComment.Empty();
		strETC.Empty();
	}
};

struct T_JUDR_PROGRAM_UDRD_D
{
	T_JUDR_PROGRAM data;
};

struct T_JUDR_PROGRAM_UNIT
{
	T_JUDR_PROGRAM_UNIT()
	{ 
		strMessage = D_UNITSYS_NONE;
		strComment = D_UNITSYS_NONE;
		strETC = D_UNITSYS_NONE;
	}
	int strMessage;
	int strComment;
	int strETC;
};

struct T_JUDR_PROGRAM_CH
{
	char strMessage[8192];
	char strComment[8192];
	char strETC[8192];

	void ConvertToString(T_JUDR_PROGRAM& rData)
	{
		ConvertCharStr(strMessage, rData.strMessage, sizeof(strMessage));
		ConvertCharStr(strComment, rData.strComment, sizeof(strComment));
		ConvertCharStr(strETC    , rData.strETC    , sizeof(strETC    ));
	}                     
	void ConvertToChar(T_JUDR_PROGRAM& rData)
	{
		ConvertStrChar(rData.strMessage, strMessage, sizeof(strMessage));
		ConvertStrChar(rData.strComment, strComment, sizeof(strComment));
		ConvertStrChar(rData.strETC    , strETC    , sizeof(strETC    ));
	}

	void Convert800(T_JUDR_PROGRAM_CH& rData)
	{
		memcpy(strMessage, rData.strMessage, sizeof(strMessage));
		memcpy(strComment, rData.strComment, sizeof(strComment));
		memcpy(strETC    , rData.strETC    , sizeof(strETC    ));
	} 
};

struct T_JUDR_LOAD
{
	// 4.하중/외력
	CString strCalcBasis;   // 적재하중시행령85조이외의 산출근거
	CString strSpecial;     // 특수하중
	CString strETC;     // 기타하중
	int     nPrintSpecial;     // 특수하중배치도의 출력 - 0:정리출력, 1:하중케이스마다 출력 
		
	void Initialize()
	{
		strCalcBasis.Empty();
		strSpecial.Empty();
		strETC.IsEmpty();
		nPrintSpecial = 0;
	}
};

struct T_JUDR_LOAD_UDRD_D
{
	T_JUDR_LOAD data;
};

struct T_JUDR_LOAD_UNIT
{
	T_JUDR_LOAD_UNIT()
	{
		strCalcBasis  = D_UNITSYS_NONE; 
		strSpecial    = D_UNITSYS_NONE;
		strETC        = D_UNITSYS_NONE;
		nPrintSpecial = D_UNITSYS_NONE;
	}
	int strCalcBasis; 
	int strSpecial; 
	int strETC;
	int nPrintSpecial;
};

struct T_JUDR_LOAD_CH
{
	char strCalcBasis[8192];
	char strSpecial[8192];
	char strETC[8192];
	int  nPrintSpecial;

	void ConvertToString(T_JUDR_LOAD& rData)
	{
		ConvertCharStr(strCalcBasis, rData.strCalcBasis, sizeof(strCalcBasis));
		ConvertCharStr(strSpecial  , rData.strSpecial  , sizeof(strSpecial  ));
		ConvertCharStr(strETC      , rData.strETC      , sizeof(strETC      ));
	}                     
	void ConvertToChar(T_JUDR_LOAD& rData)
	{
		ConvertStrChar(rData.strCalcBasis, strCalcBasis, sizeof(strCalcBasis));
		ConvertStrChar(rData.strSpecial  , strSpecial  , sizeof(strSpecial  ));
		ConvertStrChar(rData.strETC      , strETC      , sizeof(strETC      ));
	}

	void Convert800(T_JUDR_LOAD_CH& rData)
	{
		memcpy(strCalcBasis, rData.strCalcBasis, sizeof(strCalcBasis));
		memcpy(strSpecial  , rData.strSpecial  , sizeof(strSpecial  ));
		memcpy(strETC      , rData.strETC      , sizeof(strETC      ));
		nPrintSpecial = rData.nPrintSpecial;
	} 
};

struct T_JUDR_ANALYSIS
{
	// 6.응력해석
	BOOL bIncludeCross;   // 직교방향 프레임 - T:포함 , F:미포함
	
	BOOL bExistWindLoad;
	BOOL bExistSeisLoad;

	// 출력방향 - 0:정방향, 1:부방향, 3:양방향

	int  nSeisX;
	int  nSeisY;
	int  nWindX;
	int  nWindY;

	CString strETC;   // 기타 코멘트
	
	void Initialize()
	{
		bIncludeCross  = FALSE;
		bExistWindLoad = FALSE;
		bExistSeisLoad = FALSE;
		nSeisX = nSeisY = nWindX = nWindY = 0;
		strETC.Empty();
	}
};

struct T_JUDR_ANALYSIS_UDRD_D
{
	T_JUDR_ANALYSIS data;
};

struct T_JUDR_ANALYSIS_UNIT
{
	T_JUDR_ANALYSIS_UNIT()
	{
		bIncludeCross = D_UNITSYS_NONE;
		bExistWindLoad = D_UNITSYS_NONE;
		bExistSeisLoad = D_UNITSYS_NONE;
		nSeisX = D_UNITSYS_NONE;
		nSeisY = D_UNITSYS_NONE;
		nWindX = D_UNITSYS_NONE;
		nWindY = D_UNITSYS_NONE;
		strETC = D_UNITSYS_NONE;
	}
	int bIncludeCross;
	int bExistWindLoad;
	int bExistSeisLoad;
	int nSeisX;
	int nSeisY;
	int nWindX;
	int nWindY;
	int strETC;
};

struct T_JUDR_ANALYSIS_CH
{
	BOOL bIncludeCross;
	BOOL bExistWindLoad;
	BOOL bExistSeisLoad;
	int  nSeisX;
	int  nSeisY;
	int  nWindX;
	int  nWindY;
	char strETC[8192]; 
	
	void ConvertToString(T_JUDR_ANALYSIS& rData)
	{
		ConvertCharStr(strETC, rData.strETC, sizeof(strETC));
		rData.bIncludeCross  = bIncludeCross;
		rData.bExistWindLoad = bExistWindLoad;
		rData.bExistSeisLoad = bExistSeisLoad;
		rData.nSeisX         = nSeisX;
		rData.nSeisY         = nSeisY;
		rData.nWindX         = nWindX;
		rData.nWindY         = nWindY;

	}                     
	void ConvertToChar(T_JUDR_ANALYSIS& rData)
	{
		ConvertStrChar(rData.strETC, strETC, sizeof(strETC));
		bIncludeCross  = rData.bIncludeCross;
		bExistWindLoad = rData.bExistWindLoad;
		bExistSeisLoad = rData.bExistSeisLoad;
		nSeisX         = rData.nSeisX;
		nSeisY         = rData.nSeisY;
		nWindX         = rData.nWindX;
		nWindY         = rData.nWindY;
	}

	void Convert800(T_JUDR_ANALYSIS_CH& rData)
	{
		memcpy(strETC, rData.strETC, sizeof(strETC));
		bIncludeCross  = rData.bIncludeCross;
		bExistWindLoad = rData.bExistWindLoad;
		bExistSeisLoad = rData.bExistSeisLoad;
		nSeisX         = rData.nSeisX;
		nSeisY         = rData.nSeisY;
		nWindX         = rData.nWindX;
		nWindY         = rData.nWindY;
	}  
};

struct T_JUDR_HORLOAD
{
	// 11.보유수평내력
	BOOL bMajorX;         // X정방향

	BOOL bMinorX;         // X부방향

	BOOL bMajorY;         // Y정방향

	BOOL bMinorY;         // Y부방향

	BOOL bIncludeCross;   // 직교방향 프레임 - T:포함 , F:미포함
	CString strComment;   // 구조계산방침
	
	void Initialize()
	{
		bMajorX = bMinorX = bMajorY = bMinorY = TRUE;
		bIncludeCross = FALSE;
		strComment.Empty();
	}
};

struct T_JUDR_HORLOAD_UDRD_D
{
	T_JUDR_HORLOAD data;
};

struct T_JUDR_HORLOAD_UNIT
{
	T_JUDR_HORLOAD_UNIT()
	{
		bMajorX = D_UNITSYS_NONE;
		bMinorX = D_UNITSYS_NONE;
		bMajorY = D_UNITSYS_NONE;
		bMinorY = D_UNITSYS_NONE;
		bIncludeCross = D_UNITSYS_NONE;
		strComment = D_UNITSYS_NONE;
	}
	int bMajorX;
	int bMinorX;
	int bMajorY;
	int bMinorY;
	int bIncludeCross;
	int strComment;
};

struct T_JUDR_HORLOAD_CH
{
	BOOL bMajorX;
	BOOL bMinorX;
	BOOL bMajorY;
	BOOL bMinorY;
	BOOL bIncludeCross;
	char strComment[8192];

	void ConvertToString(T_JUDR_HORLOAD& rData)
	{
		ConvertCharStr(strComment, rData.strComment, sizeof(strComment));
		rData.bMajorX       = bMajorX;
		rData.bMinorX       = bMinorX;
		rData.bMajorY       = bMajorY;
		rData.bMinorY       = bMinorY;
		rData.bIncludeCross = bIncludeCross;
	}                     
	void ConvertToChar(T_JUDR_HORLOAD& rData)
	{
		ConvertStrChar(rData.strComment, strComment, sizeof(strComment));
		bMajorX       = rData.bMajorX;
		bMinorX       = rData.bMinorX;
		bMajorY       = rData.bMajorY;
		bMinorY       = rData.bMinorY;
		bIncludeCross = rData.bIncludeCross;
	}
	void Convert800(T_JUDR_HORLOAD_CH& rData)
	{
		memcpy(strComment, rData.strComment, sizeof(strComment));
		bMajorX       = rData.bMajorX;
		bMinorX       = rData.bMinorX;
		bMajorY       = rData.bMajorY;
		bMinorY       = rData.bMinorY;
		bIncludeCross = rData.bIncludeCross;
	}
};

#define T_JUDR_K unsigned int
struct T_JUDR_D 
{
	T_JUDR_RPOP     PageOption;   // 설정
	T_JUDR_COVER    Cover;        // 0.표지

	T_JUDR_GENERAL  General;      // 1.일반사항
	T_JUDR_POLICY   Policy;       // 2.설계방침
	T_JUDR_PROGRAM  Program;      // 3.프로그램
	T_JUDR_LOAD     Load;         // 4.하중/외력
	T_JUDR_ANALYSIS Analysis;     // 6.응력해석
	T_JUDR_HORLOAD  HorLoad;      // 11.보유수평내력

	CString         strPreCalc;      // 5.준비계산

	CString         strSection;      // 7.단면검정
	int             nSectOption;     // 7.단면검정 < 출력옵션  0 : 전부재출력 1 : 불리한부재만출력
	CString         strTotalOpinion; // 14.종합소견 
	
	void Initialize()
	{
		PageOption.Initialize();
		Cover.Initialize();
		General.Initialize();
		Policy.Initialize();   
		Program.Initialize();
		Load.Initialize();
		Analysis.Initialize();
		HorLoad.Initialize();
		
		strPreCalc.Empty();
		strSection.Empty();
		strTotalOpinion.Empty();

		nSectOption = 1;
	}
};

struct T_JUDR_UNIT
{
	T_JUDR_UNIT()
	{
		strPreCalc = D_UNITSYS_NONE;
		strSection = D_UNITSYS_NONE;
		strTotalOpinion = D_UNITSYS_NONE;
		nSectOption = D_UNITSYS_NONE;
	}
	int strPreCalc;
	int strSection;
	int strTotalOpinion;
	int nSectOption;
};

struct T_JUDR_D_CH_805
{
	T_JUDR_RPOP_CH_805 PageOption;
	T_JUDR_COVER_CH    Cover;
	T_JUDR_GENERAL_CH  General;
	T_JUDR_POLICY_CH   Policy;
	T_JUDR_PROGRAM_CH  Program;
	T_JUDR_LOAD_CH     Load;
	T_JUDR_ANALYSIS_CH Analysis;
	T_JUDR_HORLOAD_CH  HorLoad;
	
	char strPreCalc[8192];
	char strSection[8192];
	char strTotalOpinion[8192];
	int  nSectOption;
}; 

struct T_JUDR_D_CH_800
{
	T_JUDR_RPOP_CH_800 PageOption;
	T_JUDR_COVER_CH    Cover;
	T_JUDR_GENERAL_CH  General;
	T_JUDR_POLICY_CH   Policy;
	T_JUDR_PROGRAM_CH  Program;
	T_JUDR_LOAD_CH     Load;
	T_JUDR_ANALYSIS_CH Analysis;
	T_JUDR_HORLOAD_CH  HorLoad;
	
	char strPreCalc[8192];
	char strSection[8192];
	char strTotalOpinion[8192];
	int  nSectOption;
}; 

struct T_JUDR_D_CH
{
	T_JUDR_RPOP_CH     PageOption;
	T_JUDR_COVER_CH    Cover;
	T_JUDR_GENERAL_CH  General;
	T_JUDR_POLICY_CH   Policy;
	T_JUDR_PROGRAM_CH  Program;
	T_JUDR_LOAD_CH     Load;
	T_JUDR_ANALYSIS_CH Analysis;
	T_JUDR_HORLOAD_CH  HorLoad;
	
	char strPreCalc[8192];
	char strSection[8192];
	char strTotalOpinion[8192];
	int  nSectOption;
	
	void ConvertToString(T_JUDR_D& rData)
	{
		PageOption.ConvertToString(rData.PageOption);
		ConvertCharStr(strPreCalc, rData.strPreCalc, sizeof(strPreCalc));
		ConvertCharStr(strSection, rData.strSection, sizeof(strSection));
		ConvertCharStr(strTotalOpinion, rData.strTotalOpinion, sizeof(strTotalOpinion));
		Cover.ConvertToString(rData.Cover);
		General.ConvertToString(rData.General);
		Policy.ConvertToString(rData.Policy);
		Program.ConvertToString(rData.Program);
		Load.ConvertToString(rData.Load);
		Analysis.ConvertToString(rData.Analysis);
		HorLoad.ConvertToString(rData.HorLoad);
		rData.nSectOption = nSectOption;
	}
	void ConvertToChar(T_JUDR_D& rData)
	{
		PageOption.ConvertToChar(rData.PageOption);
		ConvertStrChar(rData.strPreCalc, strPreCalc, sizeof(strPreCalc));
		ConvertStrChar(rData.strSection, strSection, sizeof(strSection));
		ConvertStrChar(rData.strTotalOpinion, strTotalOpinion, sizeof(strTotalOpinion));
		Cover.ConvertToChar(rData.Cover);
		General.ConvertToChar(rData.General);
		Policy.ConvertToChar(rData.Policy);
		Program.ConvertToChar(rData.Program);
		Load.ConvertToChar(rData.Load);
		Analysis.ConvertToChar(rData.Analysis);
		HorLoad.ConvertToChar(rData.HorLoad);
		nSectOption = rData.nSectOption;
	}

	void Convert800(T_JUDR_D_CH_800& rData)
	{
		PageOption.Convert800(rData.PageOption);

		Cover.Convert800(rData.Cover);
		General.Convert800(rData.General);
		Policy.Convert800(rData.Policy);
		Program.Convert800(rData.Program);
		Load.Convert800(rData.Load);
		Analysis.Convert800(rData.Analysis);
		HorLoad.Convert800(rData.HorLoad);

		memcpy(strPreCalc, rData.strPreCalc, sizeof(strPreCalc));
		memcpy(strSection, rData.strSection, sizeof(strSection));
		memcpy(strTotalOpinion, rData.strTotalOpinion, sizeof(strTotalOpinion));
		nSectOption = rData.nSectOption;
	}

	void Convert805(T_JUDR_D_CH_805& rData)
	{
		PageOption.Convert805(rData.PageOption);
		
		Cover.Convert800(rData.Cover);
		General.Convert800(rData.General);
		Policy.Convert800(rData.Policy);
		Program.Convert800(rData.Program);
		Load.Convert800(rData.Load);
		Analysis.Convert800(rData.Analysis);
		HorLoad.Convert800(rData.HorLoad);
		
		memcpy(strPreCalc, rData.strPreCalc, sizeof(strPreCalc));
		memcpy(strSection, rData.strSection, sizeof(strSection));
		memcpy(strTotalOpinion, rData.strTotalOpinion, sizeof(strTotalOpinion));
		nSectOption = rData.nSectOption;
	}
}; 

struct T_JUDR_UDRD_D
{
	T_JUDR_D data;
};

struct T_JUDR_UDRD_D_CH_800
{
	T_JUDR_K key;
	T_JUDR_D_CH_800 data;
};

struct T_JUDR_UDRD_D_CH_805
{
	T_JUDR_K key;
	T_JUDR_D_CH_805 data;
};
struct T_JUDR_UDRD_D_CH
{
	T_JUDR_K key;
	T_JUDR_D_CH data;
	void Convert800(T_JUDR_UDRD_D_CH_800& rData)
	{
		key = rData.key;
		data.Convert800(rData.data);
	}
	void Convert805(T_JUDR_UDRD_D_CH_805& rData)
	{
		key = rData.key;
		data.Convert805(rData.data);
	}
};

//////////////////////////////////////////////////////////////////////////
// JFD Desing Result
// concrete column/brace
struct T_RJFD_D
{
	int	 res_nCheck;        // 설계결과 유무 0:설계결과 없음, 1:Design, 2:Checking
	int	 res_nFndType;      // DB_ST_DN_JFD.h    FounType과 같다.
	BOOL res_bShort;        // Ture:short term, False;Long-term
	BOOL res_bAddStress;    // TRUE : 기초판이 휨 부담. FALSE:지중보가 부담.
	BOOL res_bIsValidLoad;	// FALSE면 설계불가능 단면 - Overturning에 의해서 계산이 불가능한 경우
	int	 res_nMaxLcomK[11];  // 불리한 하중조합
													 // 직접기초  0:접지압, 1:휨x, 2:휨y, 3:1면전단x, 4:1면전단y, 5:2면전단, 6:부착응력x1, 7:부착응력x2, 8:부착응력y1, 9:부착응력y2
													 // 줄기초    0:접지압, 1:휨x, 2:1면전단x, 3:부착응력x1, 4:부착응력x2
													 // 파일기초  0:압축지지력, 1:인장지지력, 2:휨x, 3:휨y, 4:1면전단x, 5:1면전단y, 6:파일펀칭, 7:부착응력x1, 8:부착응력x2, 9:부착응력y1, 10:부착응력y2
													 // 매트기초  0:접지압
	
	int	 nCheckResult;    // 1=OK  2>=NG

	double	res_dLx;        // x방향 길이
	double	res_dLy;        // y방향 길이
	double	res_dThick;     // 두께
	double	res_dEccenx;    // x방향 편심
	double	res_dEcceny;    // y방향 편심
	int	    res_iUpperElem; // 상부 부재 번호
	double	res_dB;         // 기둥 폭
	double	res_dH;         // 기둥 깊이
	int     res_nPileNo;    // 파일 갯수
	
	double	res_dPu[11];	// 축력
	double	res_dMux[11];	// 휨x
	double	res_dMuy[11];	// 휨y
	double	res_dSelfW[11];	// 자중
	double	res_dAddPu[11];	// 부가축력 : 지중보 부담시.
	double	res_dAddMux1[11];	// 부가휨 : 직접-편심, 말뚝-말뚝편심
	double	res_dAddMuy1[11];	// 부가휨 : 직접-편심, 말뚝-말뚝편심
	double	res_dAddMux2[11];	// 부가휨 : 직접/말뚝-접지압 계산용 지점전단 휨
	double	res_dAddMuy2[11];	// 부가휨 : 직접/말뚝-접지압 계산용 지점전단 휨
	double	res_dAddMux3[11];	// 부가휨 : 직접/말뚝-기초판 설계용 지점전단 휨
	double	res_dAddMuy3[11];	// 부가휨 : 직접/말뚝-기초판 설계용 지점전단 휨
	double	res_dAddMux4[11];	// 부가휨 : 말뚝-말뚝머리 휨.
	double	res_dAddMuy4[11];	// 부가휨 : 말뚝-말뚝머리 휨.
	double	res_dAddMux5[11];	// 부가휨 : 말뚝-시공오차에 의한 휨.
	double	res_dAddMuy5[11];	// 부가휨 : 말뚝-시공오차에 의한 휨.
	double	res_dAddMux6[11];	// 부가휨 : 말뚝-말뚝머리 전단에 의한 휨(기초판 설계용).
	double	res_dAddMuy6[11];	// 부가휨 : 말뚝-말뚝머리 전단에 의한 휨(기초판 설계용).
	
	// common result		
	double res_dSigmaPhi[2];  // 철근둘레 길이의 합
	double res_ddt[2];  // 피복두께
	double res_dd[2];// 유효깊이

	// Bending
	double	res_dMfx; // 기초판 휨 부재력(y-dir 배근)
	double	res_dMax;	// 허용휨응력

	double	res_dMfy; // 기초판 휨 부재력(x-dir 배근)
	double	res_dMay;	// 허용휨응력

	// Shear
	double	res_dQfx; // 기초판 1면 전단(x-dir)
	double	res_dQax; // 허용전단응력
										
	double	res_dQfy; // 기초판 1면 전단(y-dir)
	double	res_dQay;	// 허용전단응력

	// adhessive
	double  res_dSigmat[2]; // 철근의 존재응력도　σ_t=M_f/(a_t?j)
	double  res_ddb[2]; // 주근직경（명칭이름의 수치）
	double  res_dld[2]; // 부착길이（기둥면으로부터 기초단까지의 거리－피복두께）
	double	res_dTau_a1x;	// 부착응력도(허용 fa)
	double	res_dTau_a2x; // 부착응력도(허용 0.8*fa)
	double	res_dTau_a1y; // 부착응력도(허용 fa)
	double	res_dTau_a2y; // 부착응력도(허용 0.8*fa)
	double	res_dfa1x;    // 허용부착응력도
	double	res_dfa2x;    // 허용부착응력도
	double	res_dfa1y;    // 허용부착응력도
	double	res_dfa2y;    // 허용부착응력도
	
	// 지내력 기초 only (독립,줄,매트)		
	double	res_dSB;	// 접지압(독릭/줄,파일기초)
	double	res_dfe;	// 허용지내력(독릭/줄,파일기초)

	// 접지압
	double res_dTopLeft;   // 1
	double res_dTopRight;  // 2
	double res_dBotLeft;   // 3
	double res_dBotRight;  // 4
	double res_dZeroPnt_TLR;  // 12
	double res_dZeroPnt_BLR;  // 32
	double res_dZeroPnt_LTB;  // 13
	double res_dZeroPnt_RTB;  // 24

	// 2면전단
	double	res_db0; // 펀칭 전단면의 총 폭

	// 직접
	double	res_dA0; // 펀칭 전단면의 총 폭에 의해 둘러싸인 면적

	double	res_dQpd;	// 2면전단력
	double	res_dQpa;	// 2면 허용전단응력

	// pile		
	double	res_dBp; // 말뚝직경
	double	res_dlp; // 인접한 말뚝의 최소 간격

	double	res_dRc;    // 파일 반력(압축)
	double	res_dRac;   // 파일 허용지지력(압축)
											
	double	res_dRt;    // 파일 반력(인장)
	double	res_dRat;   // 파일 허용지지력(인장)
											
	double	res_dRi;    // 파일펀칭
	double	res_dQpai;	// 파일펀칭 허용전단응력
	
	// Rebar		
	CString	res_strXBarName;
	int	res_iXBarNum;
	double	res_dXBarSpace; // only line

	CString	res_strYBarName;	
	int	res_iYBarNum;	
	
	// mat property data.
	double res_dMat_Area;
	double res_dMat_Iyy;
	double res_dMat_Izz;
	double res_dMat_YBar;
	double res_dMat_ZBar;
	double res_dMat_GX;
	double res_dMat_GY;

	double res_dAddPbyMx[10]; // 휨을 기초판에서 부담할 경우 말뚝 부가 휨 축력
	double res_dAddPbyMy[10]; // 휨을 기초판에서 부담할 경우 말뚝 부가 휨 축력
	
	// material
	double res_dat[2];
	double res_dft[2];
	double res_dfc;
	double res_dfs;

	void Initialize()
	{
		res_nCheck = 0;
		res_nFndType = 0;
		res_bShort = FALSE;
		res_bAddStress = TRUE;
		res_bIsValidLoad = TRUE;
		memset(res_nMaxLcomK,0,sizeof(res_nMaxLcomK));
			
		res_dLx = 0.0;
		res_dLy = 0.0;
		res_dThick = 0.0;
		res_dEccenx = 0.0;
		res_dEcceny = 0.0;
		res_iUpperElem = 0;
		res_dB = 0.0;
		res_dH = 0.0;
		res_nPileNo = 0;
		
		memset(res_dPu,0,sizeof(res_dPu));
		memset(res_dMux,0,sizeof(res_dMux));
		memset(res_dMuy,0,sizeof(res_dMuy));
		memset(res_dAddPu,0,sizeof(res_dAddPu));
		memset(res_dSelfW,0,sizeof(res_dSelfW));
		memset(res_dAddMux1,0,sizeof(res_dAddMux1));
		memset(res_dAddMuy1,0,sizeof(res_dAddMuy1));
		memset(res_dAddMux2,0,sizeof(res_dAddMux2));
		memset(res_dAddMuy2,0,sizeof(res_dAddMuy2));
		memset(res_dAddMux3,0,sizeof(res_dAddMux3));
		memset(res_dAddMuy3,0,sizeof(res_dAddMuy3));
		memset(res_dAddMux4,0,sizeof(res_dAddMux4));
		memset(res_dAddMuy4,0,sizeof(res_dAddMuy4));
		memset(res_dAddMux5,0,sizeof(res_dAddMux5));
		memset(res_dAddMuy5,0,sizeof(res_dAddMuy5));
		memset(res_dAddMux6,0,sizeof(res_dAddMux6));
		memset(res_dAddMuy6,0,sizeof(res_dAddMuy6));
		memset(res_dAddPbyMx,0,sizeof(res_dAddPbyMx));
		memset(res_dAddPbyMy,0,sizeof(res_dAddPbyMy));
				
		res_dMfx = 0.0;
		res_dMax = 0.0;

		res_dMfy = 0.0;
		res_dMay = 0.0;

		res_dQfx = 0.0;
		res_dQax = 0.0;

		res_dQfy = 0.0;
		res_dQay = 0.0;

		res_dTau_a1x = 0.0;
		res_dTau_a2x = 0.0;
		res_dTau_a1y = 0.0;
		res_dTau_a2y = 0.0;
		res_dfa1x = 0.0;
		res_dfa2x = 0.0;
		res_dfa1y = 0.0;
		res_dfa2y = 0.0;
				
		res_dQpd = 0.0;
		res_dQpa = 0.0;

		res_dSB = 0.0;
		res_dfe = 0.0;

		res_dTopLeft = 0.0;   // 1
		res_dTopRight = 0.0;  // 2
		res_dBotLeft = 0.0;   // 3
		res_dBotRight = 0.0;  // 4
		res_dZeroPnt_TLR = 0.0;  // 12
		res_dZeroPnt_BLR = 0.0;  // 32
		res_dZeroPnt_LTB = 0.0;  // 13
		res_dZeroPnt_RTB = 0.0;  // 24
				
		res_dRt = 0.0;
		res_dRat = 0.0;

		res_dRc = 0.0;
		res_dRac = 0.0;

		res_dRi = 0.0;
		res_dQpai = 0.0;
				
		res_strXBarName = "";
		res_iXBarNum = 0;
		res_dXBarSpace = 0.0;

		res_strYBarName = "";
		res_iYBarNum = 0;

		memset(res_dSigmaPhi,0,sizeof(res_dSigmaPhi));
		memset(res_ddt,0,sizeof(res_ddt));
		memset(res_dd,0,sizeof(res_dd));

		memset(res_dSigmat,0,sizeof(res_dSigmat));
		memset(res_ddb,0,sizeof(res_ddb));
		memset(res_dld,0,sizeof(res_dld));
		
		res_db0=0.0;
		
		res_dBp=0.0;
		res_dlp=0.0;
		
		res_dA0=0.0;

		res_dMat_Area=0.0;
		res_dMat_Iyy=0.0;
		res_dMat_Izz=0.0;
		res_dMat_YBar=0.0;
		res_dMat_ZBar=0.0;
		res_dMat_GX=0.0;
		res_dMat_GY=0.0;

		// material
		memset(res_dat,0,sizeof(res_dat));
		memset(res_dft,0,sizeof(res_dft));
		res_dfc=0.0;
		res_dfs=0.0;
	}
/*
	double	res_dPu[11];	// 축력
	double	res_dMux[11];	// 휨x
	double	res_dMuy[11];	// 휨y
	double	res_dSelfW[11];	// 자중
	double	res_dAddPu[11];	// 부가축력 : 지중보 부담시.
	double	res_dAddMux1[11];	// 부가휨 : 직접-편심, 말뚝-말뚝편심
	double	res_dAddMuy1[11];	// 부가휨 : 직접-편심, 말뚝-말뚝편심
	double	res_dAddMux2[11];	// 부가휨 : 직접/말뚝-접지압 계산용 지점전단 휨
	double	res_dAddMuy2[11];	// 부가휨 : 직접/말뚝-접지압 계산용 지점전단 휨
	double	res_dAddMux3[11];	// 부가휨 : 직접/말뚝-기초판 설계용 지점전단 휨
	double	res_dAddMuy3[11];	// 부가휨 : 직접/말뚝-기초판 설계용 지점전단 휨
	double	res_dAddMux4[11];	// 부가휨 : 말뚝-말뚝머리 휨.
	double	res_dAddMuy4[11];	// 부가휨 : 말뚝-말뚝머리 휨.
	double	res_dAddMux5[11];	// 부가휨 : 말뚝-시공오차에 의한 휨.
	double	res_dAddMuy5[11];	// 부가휨 : 말뚝-시공오차에 의한 휨.
	double	res_dAddMux6[11];	// 부가휨 : 말뚝-말뚝머리 전단에 의한 휨(기초판 설계용).
	double	res_dAddMuy6[11];	// 부가휨 : 말뚝-말뚝머리 전단에 의한 휨(기초판 설계용).
	*/
	double Get_Pu_Stress()
	{
		return res_dPu[0]+res_dSelfW[0]+res_dAddPu[0];
	}
	double Get_Pu_Plate()
	{
		return res_dPu[0]+res_dAddPu[0];    
	}
	double Get_Mux_Stress() // 1,2,4,5
	{
		return res_dMux[0]+res_dAddMux1[0]+res_dAddMux2[0]+res_dAddMux4[0]+res_dAddMux5[0];
	}
	double Get_Mux_Plate() // 1,3,4,5,6
	{
		return res_dMux[0]+res_dAddMux1[0]+res_dAddMux3[0]+res_dAddMux4[0]+res_dAddMux5[0]+res_dAddMux6[0];
	}
	double Get_Muy_Stress() // 1,2,4,5
	{
		return res_dMuy[0]+res_dAddMuy1[0]+res_dAddMuy2[0]+res_dAddMuy4[0]+res_dAddMuy5[0];
	}
	double Get_Muy_Plate() // 1,3,4,5,6
	{
		return res_dMuy[0]+res_dAddMuy1[0]+res_dAddMuy3[0]+res_dAddMuy4[0]+res_dAddMuy5[0]+res_dAddMuy6[0];
	}
	double Get_Mex()
	{
		return res_dAddMux1[0];
	}
	double Get_Mey()
	{
		return res_dAddMuy1[0];
	}
	double Get_Mpx()
	{
		return res_dAddMux4[0];
	}
	double Get_Mpy()
	{
		return res_dAddMux4[0];
	}
	double Get_Mrx()
	{
		return res_dMux[0]+res_dAddMux2[0];
	}
	double Get_Mry()
	{
		return res_dMuy[0]+res_dAddMuy2[0];
	}
	double Get_Mcx()
	{
		return res_dAddMuy5[0];
	}
	double Get_Mcy()
	{
		return res_dAddMuy5[0];
	}
};

struct T_RJFD_D_CH
{
	int	 res_nCheck;        // 설계결과 유무 0:설계결과 없음, 1:Design, 2:Checking
	int	 res_nFndType;      // 기초 타입 1:독립기초, 2:줄기초, 3:파일기초, 4:매트기초
	BOOL res_bShort;        // Ture:short term, False;Long-term
	BOOL res_bAddStress;    // TRUE : 기초판이 휨 부담. FALSE:지중보가 부담.
	BOOL res_bIsValidLoad;	// FALSE면 설계불가능 단면 - Overturning에 의해서 계산이 불가능한 경우
	int	 res_nMaxLcomK[11];  // 불리한 하중조합 0:접지압, 1:휨, 2:1면전단, 3:2면전단, 4:부착
		
	double	res_dLx;        // x방향 길이
	double	res_dLy;        // y방향 길이
	double	res_dThick;     // 두께
	double	res_dEccenx;    // x방향 편심
	double	res_dEcceny;    // y방향 편심
	int	    res_iUpperElem; // 상부 부재 번호
	double	res_dB;         // 기둥 폭
	double	res_dH;         // 기둥 깊이
	int     res_nPileNo;
	
	double	res_dPu[11];      // 축력 - 지점반력
	double	res_dMux[11];     // 휨x - 지점반력
	double	res_dMuy[11];     // 휨y - 지점반력
	double	res_dSelfW[11];   // 자중
	double	res_dAddPu[11];   // 부가축력 : 지중보 부담시.
	double	res_dAddMux1[11];	// 부가휨 : 직접-편심, 말뚝-말뚝편심
	double	res_dAddMuy1[11];	// 부가휨 : 직접-편심, 말뚝-말뚝편심
	double	res_dAddMux2[11];	// 부가휨 : 직접/말뚝-접지압 계산용 지점전단 휨
	double	res_dAddMuy2[11];	// 부가휨 : 직접/말뚝-접지압 계산용 지점전단 휨
	double	res_dAddMux3[11];	// 부가휨 : 직접/말뚝-기초판 설계용 지점전단 휨
	double	res_dAddMuy3[11];	// 부가휨 : 직접/말뚝-기초판 설계용 지점전단 휨
	double	res_dAddMux4[11];	// 부가휨 : 말뚝-말뚝머리 휨.
	double	res_dAddMuy4[11];	// 부가휨 : 말뚝-말뚝머리 휨.
	double	res_dAddMux5[11];	// 부가휨 : 말뚝-시공오차에 의한 휨.
	double	res_dAddMuy5[11];	// 부가휨 : 말뚝-시공오차에 의한 휨.
	double	res_dAddMux6[11];	// 부가휨 : 말뚝-말뚝머리 전단에 의한 휨(기초판 설계용).
	double	res_dAddMuy6[11];	// 부가휨 : 말뚝-말뚝머리 전단에 의한 휨(기초판 설계용).
	
	// common result		
	double res_dSigmaPhi[2];  // 철근둘레 길이의 합
	double res_ddt[2];  // 피복두께
	double res_dd[2];// 유효깊이

	double	res_dMfx;     // 기초판 휨 부재력(y-dir 배근)
	double	res_dMax;	// 허용휨응력

	double	res_dMfy;     // 기초판 휨 부재력(x-dir 배근)
	double	res_dMay;	// 허용휨응력

	double	res_dQfx;     // 기초판 1면 전단(x-dir)
	double	res_dQax;	// 허용전단응력

	double	res_dQfy;     // 기초판 1면 전단(y-dir)
	double	res_dQay;	// 허용전단응력

	double  res_dSigmat[2]; // 철근의 존재응력도　σ_t=M_f/(a_t?j)
	double  res_ddb[2]; // 주근직경（명칭이름의 수치）
	double  res_dld[2]; // 부착길이（기둥면으로부터 기초단까지의 거리－피복두께）
	double	res_dTau_a1x;	// 부착응력도(허용 fa)
	double	res_dTau_a2x;  // 부착응력도(허용 0.8*fa)
	double	res_dTau_a1y;	// 부착응력도(허용 fa)
	double	res_dTau_a2y;  // 부착응력도(허용 0.8*fa)
	double	res_dfa1x;	// 허용부착응력도
	double	res_dfa2x;	// 허용부착응력도
	double	res_dfa1y;	// 허용부착응력도
	double	res_dfa2y;	// 허용부착응력도
	
	double	res_db0; // 펀칭 전단면의 총 폭

	// 지내력 기초 only (독립,줄,매트)	
	double	res_dA0; // 펀칭 전단면의 총 폭에 의해 둘러싸인 면적
	double	res_dQpd;	// 2면전단력
	double	res_dQpa;	// 2면 허용전단응력

	double	res_dSB;	// 접지압(독릭/줄,파일기초)
	double	res_dfe;	// 허용지내력(독릭/줄,파일기초)

	// 접지압
	double res_dTopLeft;   // 1
	double res_dTopRight;  // 2
	double res_dBotLeft;   // 3
	double res_dBotRight;  // 4
	double res_dZeroPnt_TLR;  // 12
	double res_dZeroPnt_BLR;  // 32
	double res_dZeroPnt_LTB;  // 13
	double res_dZeroPnt_RTB;  // 24
	
	// pile only		
	double	res_dBp; // 말뚝직경
	double	res_dlp; // 인접한 말뚝의 최소 간격

	double	res_dRt;	// 파일 반력(인장)
	double	res_dRat;	  // 파일 허용지지력(인장)
	
	double	res_dRc;	// 파일 반력(압축)
	double	res_dRac;	  // 파일 허용지지력(압축)

	double	res_dRi;	// 파일펀칭
	double	res_dQpai;	// 파일펀칭 허용전단응력
	
	// Rebar		
	char	res_strXBarName[8];
	int	res_iXBarNum;
	double	res_dXBarSpace; // only line
	
	char	res_strYBarName[8];	
	int	res_iYBarNum;	

	// mat property data.
	double res_dMat_Area;
	double res_dMat_Iyy;
	double res_dMat_Izz;
	double res_dMat_YBar;
	double res_dMat_ZBar;
	double res_dMat_GX;
	double res_dMat_GY;

	double res_dAddPbyMx[10];
	double res_dAddPbyMy[10];

	// material
	double res_dat[2];
	double res_dft[2];
	double res_dfc;
	double res_dfs;

	void Initialize()
	{
		res_nCheck = 0;
		res_nFndType = 0;
		res_bShort = FALSE;
		res_bAddStress = TRUE;
		res_bIsValidLoad = TRUE;
		memset(res_nMaxLcomK,0,sizeof(res_nMaxLcomK));
		
		res_dLx = 0.0;
		res_dLy = 0.0;
		res_dThick = 0.0;
		res_dEccenx = 0.0;
		res_dEcceny = 0.0;
		res_iUpperElem = 0;
		res_dB = 0.0;
		res_dH = 0.0;
		res_nPileNo = 0;
		
		memset(res_dPu ,0,sizeof(res_dPu ));
		memset(res_dMux,0,sizeof(res_dMux));
		memset(res_dMuy,0,sizeof(res_dMuy));
		memset(res_dSelfW,0,sizeof(res_dSelfW));
		memset(res_dAddPu,0,sizeof(res_dAddPu));
		memset(res_dAddMux1,0,sizeof(res_dAddMux1));
		memset(res_dAddMuy1,0,sizeof(res_dAddMuy1));
		memset(res_dAddMux2,0,sizeof(res_dAddMux2));
		memset(res_dAddMuy2,0,sizeof(res_dAddMuy2));
		memset(res_dAddMux3,0,sizeof(res_dAddMux3));
		memset(res_dAddMuy3,0,sizeof(res_dAddMuy3));
		memset(res_dAddMux4,0,sizeof(res_dAddMux4));
		memset(res_dAddMuy4,0,sizeof(res_dAddMuy4));
		memset(res_dAddMux5,0,sizeof(res_dAddMux5));
		memset(res_dAddMuy5,0,sizeof(res_dAddMuy5));
		memset(res_dAddMux6,0,sizeof(res_dAddMux6));
		memset(res_dAddMuy6,0,sizeof(res_dAddMuy6));
		memset(res_dAddPbyMx,0,sizeof(res_dAddPbyMx));
		memset(res_dAddPbyMy,0,sizeof(res_dAddPbyMy));

		res_dMfx = 0.0;
		res_dMax = 0.0;

		res_dMfy = 0.0;
		res_dMay = 0.0;

		res_dQfx = 0.0;
		res_dQax = 0.0;
		
		res_dQfy = 0.0;
		res_dQay = 0.0;

		res_dTau_a1x = 0.0;
		res_dTau_a2x = 0.0;
		res_dTau_a1y = 0.0;
		res_dTau_a2y = 0.0;
		res_dfa1x = 0.0;
		res_dfa2x = 0.0;
		res_dfa1y = 0.0;
		res_dfa2y = 0.0;
		
		res_dQpd = 0.0;
		res_dQpa = 0.0;

		res_dSB = 0.0;
		res_dfe = 0.0;

		res_dTopLeft = 0.0;   // 1
		res_dTopRight = 0.0;  // 2
		res_dBotLeft = 0.0;   // 3
		res_dBotRight = 0.0;  // 4
		res_dZeroPnt_TLR = 0.0;  // 12
		res_dZeroPnt_BLR = 0.0;  // 32
		res_dZeroPnt_LTB = 0.0;  // 13
		res_dZeroPnt_RTB = 0.0;  // 24
		
		res_dRt = 0.0;
		res_dRat = 0.0;

		res_dRc = 0.0;
		res_dRac = 0.0;

		res_dRi = 0.0;
		res_dQpai = 0.0;    
		
		memset(res_strXBarName,0,sizeof(res_strXBarName));
		res_iXBarNum = 0;
		res_dXBarSpace = 0.0;
		
		memset(res_strYBarName,0,sizeof(res_strYBarName));
		res_iYBarNum = 0;

		memset(res_dSigmaPhi,0,sizeof(res_dSigmaPhi));
		memset(res_ddt,0,sizeof(res_ddt));
		memset(res_dd,0,sizeof(res_dd));
		
		memset(res_dSigmat,0,sizeof(res_dSigmat));
		memset(res_ddb,0,sizeof(res_ddb));
		memset(res_dld,0,sizeof(res_dld));
		
		res_db0=0.0;
		
		res_dBp=0.0;
		res_dlp=0.0;
		
		res_dA0=0.0;

		res_dMat_Area=0.0;
		res_dMat_Iyy=0.0;
		res_dMat_Izz=0.0;
		res_dMat_YBar=0.0;
		res_dMat_ZBar=0.0;
		res_dMat_GX=0.0;
		res_dMat_GY=0.0;

		// material
		memset(res_dat,0,sizeof(res_dat));
		memset(res_dft,0,sizeof(res_dft));
		res_dfc=0.0;
		res_dfs=0.0;
	}
	void ConvertToString(T_RJFD_D& rData)
	{
		rData.res_nCheck = res_nCheck;
		rData.res_nFndType = res_nFndType;
		rData.res_bShort = res_bShort;
		rData.res_bAddStress = res_bAddStress;
		rData.res_bIsValidLoad = res_bIsValidLoad;
		memcpy(rData.res_nMaxLcomK, res_nMaxLcomK, sizeof(res_nMaxLcomK));
		
		rData.res_dLx = res_dLx;
		rData.res_dLy = res_dLy;
		rData.res_dThick = res_dThick;
		rData.res_dEccenx = res_dEccenx;
		rData.res_dEcceny = res_dEcceny;
		rData.res_iUpperElem = res_iUpperElem;
		rData.res_dB = res_dB;
		rData.res_dH = res_dH;
		rData.res_nPileNo = res_nPileNo;
		
		memcpy(rData.res_dPu , res_dPu , sizeof(res_dPu ));
		memcpy(rData.res_dMux, res_dMux, sizeof(res_dMux));
		memcpy(rData.res_dMuy, res_dMuy, sizeof(res_dMuy));
		memcpy(rData.res_dAddPu, res_dAddPu, sizeof(res_dAddPu));
		memcpy(rData.res_dSelfW, res_dSelfW, sizeof(res_dSelfW));
		memcpy(rData.res_dAddMux1,res_dAddMux1,sizeof(res_dAddMux1));
		memcpy(rData.res_dAddMuy1,res_dAddMuy1,sizeof(res_dAddMuy1));
		memcpy(rData.res_dAddMux2,res_dAddMux2,sizeof(res_dAddMux2));
		memcpy(rData.res_dAddMuy2,res_dAddMuy2,sizeof(res_dAddMuy2));
		memcpy(rData.res_dAddMux3,res_dAddMux3,sizeof(res_dAddMux3));
		memcpy(rData.res_dAddMuy3,res_dAddMuy3,sizeof(res_dAddMuy3));
		memcpy(rData.res_dAddMux4,res_dAddMux4,sizeof(res_dAddMux4));
		memcpy(rData.res_dAddMuy4,res_dAddMuy4,sizeof(res_dAddMuy4));
		memcpy(rData.res_dAddMux5,res_dAddMux5,sizeof(res_dAddMux5));
		memcpy(rData.res_dAddMuy5,res_dAddMuy5,sizeof(res_dAddMuy5));
		memcpy(rData.res_dAddMux6,res_dAddMux6,sizeof(res_dAddMux6));
		memcpy(rData.res_dAddMuy6,res_dAddMuy6,sizeof(res_dAddMuy6));
		memcpy(rData.res_dAddPbyMx,res_dAddPbyMx,sizeof(res_dAddPbyMx));
		memcpy(rData.res_dAddPbyMy,res_dAddPbyMy,sizeof(res_dAddPbyMy));
		
		rData.res_dMfx = res_dMfx;
		rData.res_dMax = res_dMax;
		
		rData.res_dMfy = res_dMfy;
		rData.res_dMay = res_dMay;

		rData.res_dQfx = res_dQfx;
		rData.res_dQax = res_dQax;
		
		rData.res_dQfy = res_dQfy;
		rData.res_dQay = res_dQay;

		rData.res_dTau_a1x = res_dTau_a1x;
		rData.res_dTau_a2x = res_dTau_a2x;
		rData.res_dTau_a1y = res_dTau_a1y;
		rData.res_dTau_a2y = res_dTau_a2y;
		rData.res_dfa1x = res_dfa1x;
		rData.res_dfa2x = res_dfa2x;
		rData.res_dfa1y = res_dfa1y;
		rData.res_dfa2y = res_dfa2y;
		
		rData.res_dQpd = res_dQpd;
		rData.res_dQpa = res_dQpa;

		rData.res_dSB = res_dSB;
		rData.res_dfe = res_dfe;

		rData.res_dTopLeft     = res_dTopLeft    ;   // 1
		rData.res_dTopRight    = res_dTopRight   ;  // 2
		rData.res_dBotLeft     = res_dBotLeft    ;   // 3
		rData.res_dBotRight    = res_dBotRight   ;  // 4
		rData.res_dZeroPnt_TLR = res_dZeroPnt_TLR;  // 12
		rData.res_dZeroPnt_BLR = res_dZeroPnt_BLR;  // 32
		rData.res_dZeroPnt_LTB = res_dZeroPnt_LTB;  // 13
		rData.res_dZeroPnt_RTB = res_dZeroPnt_RTB;  // 24
		
		rData.res_dRt = res_dRt;
		rData.res_dRat = res_dRat;
		
		rData.res_dRc = res_dRc;
		rData.res_dRac = res_dRac;

		rData.res_dRi = res_dRi;
		rData.res_dQpai = res_dQpai;
		
		ConvertCharStr(res_strXBarName, rData.res_strXBarName, sizeof(res_strXBarName));
		rData.res_iXBarNum = res_iXBarNum;
		rData.res_dXBarSpace = res_dXBarSpace;

		ConvertCharStr(res_strYBarName, rData.res_strYBarName, sizeof(res_strYBarName));
		rData.res_iYBarNum = res_iYBarNum;

		memcpy(rData.res_dSigmaPhi,res_dSigmaPhi,sizeof(res_dSigmaPhi));
		memcpy(rData.res_ddt,res_ddt,sizeof(res_ddt));
		memcpy(rData.res_dd,res_dd,sizeof(res_dd));
		
		memcpy(rData.res_dSigmat,res_dSigmat,sizeof(res_dSigmat));
		memcpy(rData.res_ddb,res_ddb,sizeof(res_ddb));
		memcpy(rData.res_dld,res_dld,sizeof(res_dld));
		
		rData.res_db0=res_db0;
		
		rData.res_dBp=res_dBp;
		rData.res_dlp=res_dlp;
		
		rData.res_dA0=res_dA0;

		rData.res_dMat_Area=res_dMat_Area;
		rData.res_dMat_Iyy =res_dMat_Iyy ;
		rData.res_dMat_Izz =res_dMat_Izz ;
		rData.res_dMat_YBar=res_dMat_YBar;
		rData.res_dMat_ZBar=res_dMat_ZBar;
		rData.res_dMat_GX  =res_dMat_GX  ;
		rData.res_dMat_GY  =res_dMat_GY  ;

		// material
		memcpy(rData.res_dat,res_dat,sizeof(res_dat));
		memcpy(rData.res_dft,res_dft,sizeof(res_dft));
		rData.res_dfc=res_dfc;
		rData.res_dfs=res_dfs;
	}
	void ConvertToChar(T_RJFD_D& rData)
	{
		res_nCheck = rData.res_nCheck;
		res_nFndType = rData.res_nFndType;
		res_bShort = rData.res_bShort;
		res_bAddStress = rData.res_bAddStress;
		res_bIsValidLoad = rData.res_bIsValidLoad;
		memcpy(res_nMaxLcomK, rData.res_nMaxLcomK, sizeof(res_nMaxLcomK));
		
		res_dLx = rData.res_dLx;
		res_dLy = rData.res_dLy;
		res_dThick = rData.res_dThick;
		res_dEccenx = rData.res_dEccenx;
		res_dEcceny = rData.res_dEcceny;
		res_iUpperElem = rData.res_iUpperElem;
		res_dB = rData.res_dB;
		res_dH = rData.res_dH;
		res_nPileNo = rData.res_nPileNo;
		
		memcpy(res_dPu , rData.res_dPu, sizeof(res_dPu ));
		memcpy(res_dMux, rData.res_dMux, sizeof(res_dMux));
		memcpy(res_dMuy, rData.res_dMuy, sizeof(res_dMuy));
		memcpy(res_dAddPu, rData.res_dAddPu, sizeof(res_dAddPu));
		memcpy(res_dSelfW, rData.res_dSelfW, sizeof(res_dSelfW));
		memcpy(res_dAddMux1,rData.res_dAddMux1,sizeof(res_dAddMux1));
		memcpy(res_dAddMuy1,rData.res_dAddMuy1,sizeof(res_dAddMuy1));
		memcpy(res_dAddMux2,rData.res_dAddMux2,sizeof(res_dAddMux2));
		memcpy(res_dAddMuy2,rData.res_dAddMuy2,sizeof(res_dAddMuy2));
		memcpy(res_dAddMux3,rData.res_dAddMux3,sizeof(res_dAddMux3));
		memcpy(res_dAddMuy3,rData.res_dAddMuy3,sizeof(res_dAddMuy3));
		memcpy(res_dAddMux4,rData.res_dAddMux4,sizeof(res_dAddMux4));
		memcpy(res_dAddMuy4,rData.res_dAddMuy4,sizeof(res_dAddMuy4));
		memcpy(res_dAddMux5,rData.res_dAddMux5,sizeof(res_dAddMux5));
		memcpy(res_dAddMuy5,rData.res_dAddMuy5,sizeof(res_dAddMuy5));
		memcpy(res_dAddMux6,rData.res_dAddMux6,sizeof(res_dAddMux6));
		memcpy(res_dAddMuy6,rData.res_dAddMuy6,sizeof(res_dAddMuy6));
		memcpy(res_dAddPbyMx,rData.res_dAddPbyMx,sizeof(res_dAddPbyMx));
		memcpy(res_dAddPbyMy,rData.res_dAddPbyMy,sizeof(res_dAddPbyMy));
		
		res_dMfx = rData.res_dMfx;
		res_dMax = rData.res_dMax;
		
		res_dMfy = rData.res_dMfy;
		res_dMay = rData.res_dMay;

		res_dQfx = rData.res_dQfx;
		res_dQax = rData.res_dQax;
		
		res_dQfy = rData.res_dQfy;
		res_dQay = rData.res_dQay;

		res_dTau_a1x = rData.res_dTau_a1x;
		res_dTau_a2x = rData.res_dTau_a2x;
		res_dTau_a1y = rData.res_dTau_a1y;
		res_dTau_a2y = rData.res_dTau_a2y;
		res_dfa1x = rData.res_dfa1x;
		res_dfa2x = rData.res_dfa2x;
		res_dfa1y = rData.res_dfa1y;
		res_dfa2y = rData.res_dfa2y;
		
		res_dQpd = rData.res_dQpd;
		res_dQpa = rData.res_dQpa;

		res_dSB = rData.res_dSB;
		res_dfe = rData.res_dfe;
		
		res_dTopLeft     = rData.res_dTopLeft    ;   // 1
		res_dTopRight    = rData.res_dTopRight   ;  // 2
		res_dBotLeft     = rData.res_dBotLeft    ;   // 3
		res_dBotRight    = rData.res_dBotRight   ;  // 4
		res_dZeroPnt_TLR = rData.res_dZeroPnt_TLR;  // 12
		res_dZeroPnt_BLR = rData.res_dZeroPnt_BLR;  // 32
		res_dZeroPnt_LTB = rData.res_dZeroPnt_LTB;  // 13
		res_dZeroPnt_RTB = rData.res_dZeroPnt_RTB;  // 24

		res_dRt = rData.res_dRt;
		res_dRat = rData.res_dRat;
		
		res_dRc = rData.res_dRc;
		res_dRac = rData.res_dRac;

		res_dRi = rData.res_dRi;
		res_dQpai = rData.res_dQpai;
		
		ConvertStrChar(rData.res_strXBarName, res_strXBarName, sizeof(res_strXBarName));
		res_iXBarNum = rData.res_iXBarNum;
		res_dXBarSpace = rData.res_dXBarSpace;

		ConvertStrChar(rData.res_strYBarName, res_strYBarName, sizeof(res_strYBarName));
		res_iYBarNum = rData.res_iYBarNum;

		memcpy(res_dSigmaPhi,rData.res_dSigmaPhi,sizeof(res_dSigmaPhi));
		memcpy(res_ddt,rData.res_ddt,sizeof(res_ddt));
		memcpy(res_dd,rData.res_dd,sizeof(res_dd));
		
		memcpy(res_dSigmat,rData.res_dSigmat,sizeof(res_dSigmat));
		memcpy(res_ddb,rData.res_ddb,sizeof(res_ddb));
		memcpy(res_dld,rData.res_dld,sizeof(res_dld));
		
		res_db0=rData.res_db0;
		
		res_dBp=rData.res_dBp;
		res_dlp=rData.res_dlp;
		
		res_dA0=rData.res_dA0;

		res_dMat_Area=rData.res_dMat_Area;
		res_dMat_Iyy =rData.res_dMat_Iyy ;
		res_dMat_Izz =rData.res_dMat_Izz ;
		res_dMat_YBar=rData.res_dMat_YBar;
		res_dMat_ZBar=rData.res_dMat_ZBar;
		res_dMat_GX  =rData.res_dMat_GX  ;
		res_dMat_GY  =rData.res_dMat_GY  ;

		// material
		memcpy(res_dat,rData.res_dat,sizeof(res_dat));
		memcpy(res_dft,rData.res_dft,sizeof(res_dft));
		res_dfc=rData.res_dfc;
		res_dfs=rData.res_dfs;
	}
};

struct T_RJFD_UNIT
{
	T_RJFD_UNIT()
	{
		res_nCheck=D_UNITSYS_NONE;
		res_nFndType=D_UNITSYS_NONE;
		res_bShort=D_UNITSYS_NONE;
		res_bAddStress=D_UNITSYS_NONE;
		res_bIsValidLoad=D_UNITSYS_NONE;
		res_nMaxLcomK=D_UNITSYS_NONE;
		
		res_dLx=D_UNITSYS_BASE_LENGTH;
		res_dLy=D_UNITSYS_BASE_LENGTH;
		res_dThick=D_UNITSYS_BASE_LENGTH;
		res_dEccenx=D_UNITSYS_BASE_LENGTH;
		res_dEcceny=D_UNITSYS_BASE_LENGTH;
		res_iUpperElem=D_UNITSYS_NONE;
		res_dB=D_UNITSYS_BASE_LENGTH;
		res_dH=D_UNITSYS_BASE_LENGTH;
		res_nPileNo=D_UNITSYS_NONE;
		
		res_dPu=D_UNITSYS_BASE_FORCE;
		res_dMux=D_UNITSYS_BASE_MOMENT;
		res_dMuy=D_UNITSYS_BASE_MOMENT;
		res_dAddPu=D_UNITSYS_BASE_FORCE;
		res_dSelfW=D_UNITSYS_BASE_FORCE;

		res_dAddMux1=D_UNITSYS_BASE_MOMENT;
		res_dAddMuy1=D_UNITSYS_BASE_MOMENT;
		res_dAddMux2=D_UNITSYS_BASE_MOMENT;
		res_dAddMuy2=D_UNITSYS_BASE_MOMENT;
		res_dAddMux3=D_UNITSYS_BASE_MOMENT;
		res_dAddMuy3=D_UNITSYS_BASE_MOMENT;
		res_dAddMux4=D_UNITSYS_BASE_MOMENT;
		res_dAddMuy4=D_UNITSYS_BASE_MOMENT;
		res_dAddMux5=D_UNITSYS_BASE_MOMENT;
		res_dAddMuy5=D_UNITSYS_BASE_MOMENT;
		res_dAddMux6=D_UNITSYS_BASE_MOMENT;
		res_dAddMuy6=D_UNITSYS_BASE_MOMENT;
		
		res_dMfx=D_UNITSYS_BASE_MOMENT;
		res_dMfy=D_UNITSYS_BASE_MOMENT;
		res_dQfx=D_UNITSYS_BASE_FORCE;
		res_dQfy=D_UNITSYS_BASE_FORCE;
		res_dTau_a1x=D_UNITSYS_BASE_STRESS;
		res_dTau_a2x=D_UNITSYS_BASE_STRESS;
		res_dTau_a1y=D_UNITSYS_BASE_STRESS;
		res_dTau_a2y=D_UNITSYS_BASE_STRESS;
		
		res_dMax=D_UNITSYS_BASE_MOMENT;
		res_dMay=D_UNITSYS_BASE_MOMENT;
		res_dQax=D_UNITSYS_BASE_FORCE;
		res_dQay=D_UNITSYS_BASE_FORCE;
		res_dfa1x=D_UNITSYS_BASE_STRESS;
		res_dfa2x=D_UNITSYS_BASE_STRESS;
		res_dfa1y=D_UNITSYS_BASE_STRESS;
		res_dfa2y=D_UNITSYS_BASE_STRESS;
		
		res_dQpd=D_UNITSYS_BASE_FORCE;
		res_dQpa=D_UNITSYS_BASE_FORCE;
		res_dSB=D_UNITSYS_BASE_STRESS;
		res_dfe=D_UNITSYS_BASE_STRESS;

		res_dTopLeft     = D_UNITSYS_BASE_STRESS;
		res_dTopRight    = D_UNITSYS_BASE_STRESS;
		res_dBotLeft     = D_UNITSYS_BASE_STRESS;
		res_dBotRight    = D_UNITSYS_BASE_STRESS;
		res_dZeroPnt_TLR = D_UNITSYS_BASE_LENGTH;
		res_dZeroPnt_BLR = D_UNITSYS_BASE_LENGTH;
		res_dZeroPnt_LTB = D_UNITSYS_BASE_LENGTH;
		res_dZeroPnt_RTB = D_UNITSYS_BASE_LENGTH;
		
		res_dRt=D_UNITSYS_BASE_FORCE;
		res_dRc=D_UNITSYS_BASE_FORCE;
		res_dRi=D_UNITSYS_BASE_FORCE;
		
		res_dRat=D_UNITSYS_BASE_FORCE;
		res_dRac=D_UNITSYS_BASE_FORCE;
		res_dQpai=D_UNITSYS_BASE_FORCE;
		
		res_strXBarName=D_UNITSYS_NONE;
		res_strYBarName=D_UNITSYS_NONE;
		res_iXBarNum=D_UNITSYS_NONE;
		res_dXBarSpace=D_UNITSYS_BASE_LENGTH;
		res_iYBarNum=D_UNITSYS_NONE;

		res_dSigmaPhi=D_UNITSYS_BASE_LENGTH;
		res_ddt=D_UNITSYS_BASE_LENGTH;
		res_dd=D_UNITSYS_BASE_LENGTH;
		
		res_dSigmat=D_UNITSYS_BASE_STRESS;
		res_ddb=D_UNITSYS_BASE_LENGTH;
		res_dld=D_UNITSYS_BASE_LENGTH;
		
		res_db0=D_UNITSYS_BASE_LENGTH;
		
		res_dBp=D_UNITSYS_BASE_LENGTH;
		res_dlp=D_UNITSYS_BASE_LENGTH;
		
		res_dA0=D_UNITSYS_BASE_AREA;

		res_dMat_Area=D_UNITSYS_BASE_AREA;
		res_dMat_Iyy =D_UNITSYS_BASE_STIF;
		res_dMat_Izz =D_UNITSYS_BASE_STIF;
		res_dMat_YBar=D_UNITSYS_BASE_LENGTH;
		res_dMat_ZBar=D_UNITSYS_BASE_LENGTH;
		res_dMat_GX  =D_UNITSYS_BASE_LENGTH;
		res_dMat_GY  =D_UNITSYS_BASE_LENGTH;

		res_dAddPbyMx = D_UNITSYS_BASE_FORCE;
		res_dAddPbyMy = D_UNITSYS_BASE_FORCE;

		res_dat = D_UNITSYS_BASE_AREA;
		res_dft = D_UNITSYS_BASE_STRESS;
		res_dfc = D_UNITSYS_BASE_STRESS;
		res_dfs = D_UNITSYS_BASE_STRESS;
	}
	int res_nCheck;
	int res_nFndType;
	int res_bShort;
	int res_bAddStress;
	int res_bIsValidLoad;
	int res_nMaxLcomK;
		
	int res_dLx;
	int res_dLy;
	int res_dThick;
	int res_dEccenx;
	int res_dEcceny;
	int res_iUpperElem;
	int res_dB;
	int res_dH;
	int res_nPileNo;
	
	int res_dPu;
	int res_dMux;
	int res_dMuy;
	int res_dAddPu;
	int res_dSelfW;

	int res_dAddMux1;
	int res_dAddMuy1;
	int res_dAddMux2;
	int res_dAddMuy2;
	int res_dAddMux3;
	int res_dAddMuy3;
	int res_dAddMux4;
	int res_dAddMuy4;
	int res_dAddMux5;
	int res_dAddMuy5;
	int res_dAddMux6;
	int res_dAddMuy6;

	int res_dMfx;
	int res_dMfy;
	int res_dQfx;
	int res_dQfy;
	int res_dTau_a1x;
	int res_dTau_a2x;
	int res_dTau_a1y;
	int res_dTau_a2y;
	
	int res_dMax;
	int res_dMay;
	int res_dQax;
	int res_dQay;
	int res_dfa1x;
	int res_dfa2x;
	int res_dfa1y;
	int res_dfa2y;
		
	int res_dQpd;
	int res_dQpa;
	int res_dSB;
	int res_dfe;

	int res_dTopLeft;
	int res_dTopRight;
	int res_dBotLeft;
	int res_dBotRight;
	int res_dZeroPnt_TLR;
	int res_dZeroPnt_BLR;
	int res_dZeroPnt_LTB;
	int res_dZeroPnt_RTB;
		
	int res_dRt;
	int res_dRc;
	int res_dRi;
	
	int res_dRat;
	int res_dRac;
	int res_dQpai;
		
	int res_strXBarName;
	int res_strYBarName;
	int res_iXBarNum;
	int res_dXBarSpace;
	int res_iYBarNum;

	int res_dSigmaPhi;
	int res_ddt;
	int res_dd;
	
	int res_dSigmat;
	int res_ddb;
	int res_dld;
	
	int res_db0;
	
	int res_dBp;
	int res_dlp;
	
	int res_dA0;

	int res_dMat_Area;
	int res_dMat_Iyy ;
	int res_dMat_Izz ;
	int res_dMat_YBar;
	int res_dMat_ZBar;
	int res_dMat_GX  ;
	int res_dMat_GY  ;

	int res_dAddPbyMx;
	int res_dAddPbyMy;

	int res_dat;
	int res_dft;
	int res_dfc;
	int res_dfs;
};



#define HASESIZEJUDR 1

//////////////////////////////////////////////////////////////////////////


#pragma pack()

#endif