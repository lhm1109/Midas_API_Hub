#ifndef __DB_ST_DN_JFD_H__
#define __DB_ST_DN_JFD_H__

#define MIDAS_SDK

#include "DB_ST_DT.h"
#include "DB_ST_DT_UNIT.h"
#include "DBCodeDef.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma pack(1)

#define  NAME_LENGTH 16

#define D_PILE_SECT_SHAPE_REG_RND       "Round"   // Round
#define D_PILE_SECT_SHAPE_REG_PIPE      "Pipe"    // Pipe
#define D_PILE_SECT_SHAPE_REG_PHC       "PHC"     // PHC 말뚝
#define D_PILE_SECT_SHAPE_REG_STL_PHC   "SPHC"    // 철부 PHC 말뚝
#define D_PILE_SECT_SHAPE_REG_ST        "ST"      // ST 말뚝
#define D_PILE_SECT_SHAPE_REG_STL_PC    "SPC"     // 육각 파이프형 철부 PC 말뚝
#define D_PILE_SECT_SHAPE_REG_SC        "SC"      // SC 말뚝
#define D_PILE_SECT_SHAPE_REG_PRC       "PRC"     // PRC 말뚝
#define D_PILE_SECT_SHAPE_REG_STL_PRC   "SPRC"    // 철부 PRC 말뚝
#define D_PILE_SECT_SHAPE_REG_CPRC      "CPRC"    // CPRC 말뚝
#define D_PILE_SECT_SHAPE_REG_PRCS      "PRCS"    // PRC 말뚝 (전단보강)
#define D_PILE_SECT_SHAPE_REG_SPRCS     "SPRCS"   // 철부 PRC 말뚝 (전단보강)

enum FounType
{
	FOUN_DIR=0,   // 직접(지내력)기초
	FOUN_STRP,    // 줄기초
	FOUN_PILE,    // 말뚝기초
	FOUN_MAT,     // 매트기초
};

enum PileType
{
	PILE_PLACE=0, // 현장타설
	PILE_PRECAST, // 기성
	PILE_STL_PIPE,// 강관
};

// MNET:XXXX-CJJEONG-20121026 (2088) : 변경시 주의
enum Precast
{
	PILE_PHC  =0,  // PHC 말뚝
	PILE_NPHC   ,  // 절부 PHC 말뚝
	PILE_ST     ,  // ST 말뚝
	PILE_NPRC   ,  // 절부 PRC 말뚝
	PILE_CPRC   ,  // CPRC 말뚝
	PILE_PRCR   ,  // PRC 말뚝 (전단보강)
	PILE_NPRCR  ,  // 절부 PRC 말뚝 (전단보강)
	PILE_SC     ,  // SC 말뚝
};

#define T_FGRP_K unsigned int
typedef CArray<T_FGRP_K, T_FGRP_K> T_FGRP_K_LIST;

#define DGN_VALUE_NUM 88 //기초 단면 변수 갯수

struct T_FOUN_DGN
{
	UINT GrupKey;
	UINT Key;                           // NodeK : 직접기초, 말뚝기초, 줄기초
	double dDgnValue[DGN_VALUE_NUM];    // 설계중간변수 : 지내력기초/줄기초, 말뚝기초 공통 사용
	BOOL bDgnValueAuto[DGN_VALUE_NUM];  // 설계중간변수 자동계산 여부
	void Initialize()
	{
		GrupKey = NULL;
		Key = NULL;
		for(int i=0; i<DGN_VALUE_NUM; i++)
		{
			dDgnValue[i]=0.;
			bDgnValueAuto[i]=TRUE;
		}
	}
};

#define LIQ_VALUE_NUM 22 //액상화 변수 갯수

struct T_FOUN_LIQ  //액상화 변수
{
	double dLiquValue[LIQ_VALUE_NUM];  //설계 중간 변수 액상화 판정 결과
	BOOL   bLiquAutoF[LIQ_VALUE_NUM];  //자동계산 여부
	void Initialize()
	{
		for(int i = 0; i < LIQ_VALUE_NUM; i++ )
		{
			dLiquValue[i] = 0;
			bLiquAutoF[i] = TRUE;
		}
	}
	T_FOUN_LIQ() { Initialize(); };
	T_FOUN_LIQ& operator=(const T_FOUN_LIQ& src)
	{ 
		for(int i = 0; i < LIQ_VALUE_NUM; i++ )
		{
			dLiquValue[i] = src.dLiquValue[i];
			bLiquAutoF[i] = src.bLiquAutoF[i];
		}
		return *this;
	}	
};

struct T_FGRP_D
{
	CString strGrupName;
	int nFounType;
	CString strHfopGrup;
	CArray<UINT,UINT> aKey; // NodeK(직접기초, 말뚝기초, 매트기초), ElemK(줄기초)
	CArray<T_FOUN_DGN, T_FOUN_DGN&> aKeyDgnValue;
	CArray<T_FOUN_LIQ, T_FOUN_LIQ&> aLiquValue; //액상화 판정 결과
	void Initialize()
	{
		strGrupName=_T("");
		nFounType=0;
		strHfopGrup=_T("Group1");
		aKey.RemoveAll();
		aKeyDgnValue.RemoveAll();
		aLiquValue.RemoveAll();
	}
	T_FGRP_D() { Initialize(); };
	T_FGRP_D(T_FGRP_D& src) { *this = src; }
	T_FGRP_D& operator=(const T_FGRP_D& src)
	{ 
		strGrupName = src.strGrupName;
		nFounType   = src.nFounType;
		strHfopGrup = src.strHfopGrup;
		aKey.Copy(src.aKey);
		aKeyDgnValue.Copy(src.aKeyDgnValue);
		aLiquValue.Copy(src.aLiquValue);
		return *this;
	}	
};
struct T_FGRP_D_CH
{
	char  chGrupName[80];
	int   nFounType;
	char  strHfopGrup[NAME_LENGTH];
	CArray<UINT,UINT> aKey;
	CArray<T_FOUN_DGN, T_FOUN_DGN&> aKeyDgnValue;
	CArray<T_FOUN_LIQ, T_FOUN_LIQ&> aLiquValue;
	
	void ConvertToString(T_FGRP_D& rData)
	{
		ConvertCharStr(chGrupName, rData.strGrupName, sizeof(chGrupName));
		rData.nFounType = nFounType;
		ConvertCharStr(strHfopGrup, rData.strHfopGrup, sizeof(strHfopGrup));
		rData.aKey.Copy(aKey);
		rData.aKeyDgnValue.Copy(aKeyDgnValue);
		rData.aLiquValue.Copy(aLiquValue);
	}
	void ConvertToChar(T_FGRP_D& rData)
	{
		ConvertStrChar(rData.strGrupName, chGrupName, sizeof(chGrupName));
		nFounType = rData.nFounType; 
		ConvertStrChar(rData.strHfopGrup, strHfopGrup, sizeof(strHfopGrup));
		aKey.Copy(rData.aKey);
		aKeyDgnValue.Copy(rData.aKeyDgnValue);
		aLiquValue.Copy(rData.aLiquValue);
	}
};
struct T_FGRP_UDRD_D
{
	T_FGRP_K key;
	T_FGRP_D data;
};
struct T_FGRP_UDRD_D_CH
{
	T_FGRP_K key;
	T_FGRP_D_CH data;
};
struct T_FGRP_UDRD_D_CH_RW
{ 
	T_FGRP_K  key;
	char      chGrupName[80]; 
	int       nFounType;
	char      strHfopGrup[NAME_LENGTH];
	int       nCountKey;
	int       nCountKeyDgnValue;
	int       nCountLiquValue;
	void GetFgrp(T_FGRP_K& rKey, T_FGRP_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.chGrupName, chGrupName, sizeof(chGrupName));
		rData.nFounType=nFounType;
		memcpy(rData.strHfopGrup, strHfopGrup, sizeof(strHfopGrup));
	}
	void SetFgrp(const T_FGRP_K Key, const T_FGRP_D_CH& rData)
	{
		key=Key;
		memcpy(chGrupName, rData.chGrupName, sizeof(chGrupName));
		nFounType = rData.nFounType;    
		memcpy(strHfopGrup, rData.strHfopGrup, sizeof(strHfopGrup));
		nCountKey = (int)rData.aKey.GetSize();
		nCountKeyDgnValue = (int)rData.aKeyDgnValue.GetSize();
		nCountLiquValue = (int)rData.aLiquValue.GetSize();
	}
};
struct T_FGRP_UNIT
{
	T_FGRP_UNIT()
	{
		strGrupName=D_UNITSYS_NONE;
		nFounType=D_UNITSYS_NONE;
	}
	int strGrupName;
	int nFounType;
};
#define HASHSIZEFGRP 11

struct T_FOUN_DGN_UNIT
{
	T_FOUN_DGN_UNIT(int nFounType, int nUnitSize)
	{
		for(int i=0; i<DGN_VALUE_NUM; i++)
		{
			dDgnValue[i] = D_UNITSYS_NONE;
		}

		int k = 0;
		//\JFDCalcSWS.h , enum JFD_PIFO_T_D ~ JFD_DIRF_NUM
		//enum Type을 따로 모아서 네임스페이스로 분리 하는게 좋겠네요.
		//여기서 각 타입별로 다르게 사용되는 단위계들 처리
		if(FOUN_PILE == nFounType)
		{
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_WEIGHT;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_ELAST ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_WEIGHT;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_ELAST ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_WEIGHT;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_ELAST ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_AREA  ;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_FORCE ;
			dDgnValue[k++] = D_UNITSYS_BASE_FORCE ;
			dDgnValue[k++] = D_UNITSYS_BASE_FORCE ;
			dDgnValue[k++] = D_UNITSYS_BASE_FORCE ;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_FORCE ;
			dDgnValue[k++] = D_UNITSYS_BASE_FORCE ;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_WEIGHT;
			dDgnValue[k++] = D_UNITSYS_BASE_FORCE ;
			dDgnValue[k++] = D_UNITSYS_BASE_FORCE ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_WEIGHT;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_ELAST ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_WEIGHT;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_ELAST ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_WEIGHT;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_NONE       ;
			dDgnValue[k++] = D_UNITSYS_BASE_ELAST ;
			dDgnValue[k++] = D_UNITSYS_BASE_LENGTH;
			dDgnValue[k++] = D_UNITSYS_BASE_FORCE ;
			dDgnValue[k++] = D_UNITSYS_BASE_FORCE ;
			dDgnValue[k++] = D_UNITSYS_BASE_FORCE ;
			dDgnValue[k++] = D_UNITSYS_BASE_WEIGHT;
			dDgnValue[k++] = D_UNITSYS_BASE_FORCE ;
			dDgnValue[k++] = D_UNITSYS_BASE_FORCE ;
			ASSERT(nUnitSize==k);
		}
		else
		{
			dDgnValue[k++] =  D_UNITSYS_BASE_LENGTH ;
			dDgnValue[k++] =  D_UNITSYS_BASE_LENGTH ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_BASE_ELAST  ;
			dDgnValue[k++] =  D_UNITSYS_BASE_ELAST  ;
			dDgnValue[k++] =  D_UNITSYS_BASE_DENSITY;
			dDgnValue[k++] =  D_UNITSYS_BASE_DENSITY;
			dDgnValue[k++] =  D_UNITSYS_BASE_LENGTH ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_BASE_ELAST  ;
			dDgnValue[k++] =  D_UNITSYS_BASE_ELAST  ;
			dDgnValue[k++] =  D_UNITSYS_BASE_LENGTH ;
			dDgnValue[k++] =  D_UNITSYS_BASE_LENGTH ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_BASE_ELAST  ;
			dDgnValue[k++] =  D_UNITSYS_BASE_ELAST  ;
			dDgnValue[k++] =  D_UNITSYS_BASE_DENSITY;
			dDgnValue[k++] =  D_UNITSYS_BASE_DENSITY;
			dDgnValue[k++] =  D_UNITSYS_BASE_LENGTH ;
			dDgnValue[k++] =  D_UNITSYS_NONE        ;
			dDgnValue[k++] =  D_UNITSYS_BASE_ELAST  ;
			dDgnValue[k++] =  D_UNITSYS_BASE_ELAST  ;
			ASSERT(nUnitSize==k);
		}
	}
	int dDgnValue[DGN_VALUE_NUM];
};

struct T_FOUN_LIQ_UNIT
{
	T_FOUN_LIQ_UNIT()
	{
		for(int i=0; i<LIQ_VALUE_NUM; i++)
		{
			dLiquValue[i] = D_UNITSYS_NONE;
		}
		int k = 0;
		//\JFDCalcSWS.h , enum JFD_LIQUE_LEVEL ~ JFD_LIQUE_NUM
		//enum Type을 따로 모아서 네임스페이스로 분리 하는게 좋겠네요.
		dLiquValue[k++] = D_UNITSYS_BASE_LENGTH ;
		dLiquValue[k++] = D_UNITSYS_BASE_LENGTH ;
		dLiquValue[k++] = D_UNITSYS_NONE        ;
		dLiquValue[k++] = D_UNITSYS_BASE_DENSITY;
		dLiquValue[k++] = D_UNITSYS_NONE        ;
		dLiquValue[k++] = D_UNITSYS_NONE        ;
		dLiquValue[k++] = D_UNITSYS_BASE_STRESS ;
		dLiquValue[k++] = D_UNITSYS_BASE_STRESS ;
		dLiquValue[k++] = D_UNITSYS_NONE        ;
		dLiquValue[k++] = D_UNITSYS_NONE        ;
		dLiquValue[k++] = D_UNITSYS_BASE_STRESS ;
		dLiquValue[k++] = D_UNITSYS_NONE        ;
		dLiquValue[k++] = D_UNITSYS_NONE        ;
		dLiquValue[k++] = D_UNITSYS_NONE        ;
		dLiquValue[k++] = D_UNITSYS_NONE        ;
		dLiquValue[k++] = D_UNITSYS_NONE        ;
		dLiquValue[k++] = D_UNITSYS_BASE_STRESS ;
		dLiquValue[k++] = D_UNITSYS_NONE        ;
		dLiquValue[k++] = D_UNITSYS_NONE        ;
		dLiquValue[k++] = D_UNITSYS_NONE        ;
		dLiquValue[k++] = D_UNITSYS_NONE        ;
		dLiquValue[k++] = D_UNITSYS_NONE        ;
		ASSERT(LIQ_VALUE_NUM == k);  
	}
	int dLiquValue[LIQ_VALUE_NUM];
};

struct T_FOUN_PDSTL
{
	UINT Key;
	double dDx;
	double dDy;
	BOOL bDxUser;
	BOOL bDyUser;

	void Initialize()
	{
		Key=NULL;
		dDx=0.;
		dDy=0.;
		bDxUser=FALSE;
		bDyUser=FALSE;
	}
};

struct T_FOUN_PDSTL_UNIT
{
	T_FOUN_PDSTL_UNIT()
	{
		dDx=D_UNITSYS_BASE_LENGTH;
		dDy=D_UNITSYS_BASE_LENGTH;
	}
	int dDx;
	int dDy;
};

struct T_FOUN_MATL_ARBAR
{
	int    nLX          ;
	CString strLXGrade  ;
	double dSpaceX      ;  //Only Use to Line Type Founcation
	int    nLY          ;
	CString strLYGrade  ;
	double dProtectThick;

	CString strCodeName ;  //철근 재질 코드
	CString strLXMatl   ;  // x 방향 철근 재질 - 줄기초도 사용..
	CString strLYMatl   ;  // y 방향 철근 재질
												 
	void Initialize()
	{
		nLX              =0;          
		strLXGrade       =_T("D22");
		dSpaceX          =0.;
		nLY              =0;
		strLYGrade       =_T("D22");
		dProtectThick    =0.;

		strCodeName  = _T("JIS(RC)");
		strLXMatl    = _T("SD295A");
		strLYMatl    = _T("SD295A");
	}
	T_FOUN_MATL_ARBAR() { Initialize(); };
	T_FOUN_MATL_ARBAR(T_FOUN_MATL_ARBAR& src) { *this = src; }
	T_FOUN_MATL_ARBAR& operator=(const T_FOUN_MATL_ARBAR& src)
	{                                  
		nLX           = src.nLX          ;
		strLXGrade    = src.strLXGrade   ;
		dSpaceX       = src.dSpaceX      ;
		nLY           = src.nLY          ;
		strLYGrade    = src.strLYGrade   ;
		dProtectThick = src.dProtectThick;
		strCodeName   = src.strCodeName  ;
		strLXMatl     = src.strLXMatl    ;
		strLYMatl     = src.strLYMatl    ;
		return *this;
	}	
};

struct T_FOUN_MATL_ARBAR_CH
{
	int     nLX;
	char    LXGrade[NAME_LENGTH];
	char    LYGrade[NAME_LENGTH];
	double  dSpaceX;
	int     nLY;
	double  dProtectThick;
	char strCodeName[NAME_LENGTH] ; 
	char strLXMatl[NAME_LENGTH]   ; 
	char strLYMatl[NAME_LENGTH]   ; 
	
	void ConvertToString(T_FOUN_MATL_ARBAR& rData)
	{
		rData.nLX = nLX;
		ConvertCharStr(LXGrade, rData.strLXGrade, sizeof(LXGrade));
		ConvertCharStr(LYGrade, rData.strLYGrade, sizeof(LYGrade));
		rData.dSpaceX = dSpaceX;
		rData.nLY = nLY;
		rData.dProtectThick = dProtectThick;
		ConvertCharStr(strCodeName, rData.strCodeName, sizeof(strCodeName));
		ConvertCharStr(strLXMatl  , rData.strLXMatl  , sizeof(strLXMatl  ));
		ConvertCharStr(strLYMatl  , rData.strLYMatl  , sizeof(strLYMatl  ));
	}
	void ConvertToChar(T_FOUN_MATL_ARBAR& rData)
	{
		nLX = rData.nLX;
		ConvertStrChar(rData.strLXGrade, LXGrade, sizeof(LXGrade));
		ConvertStrChar(rData.strLYGrade, LYGrade, sizeof(LYGrade));
		dSpaceX = rData.dSpaceX;
		nLY = rData.nLY;
		dProtectThick = rData.dProtectThick;
		ConvertStrChar(rData.strCodeName, strCodeName, sizeof(strCodeName));
		ConvertStrChar(rData.strLXMatl  , strLXMatl  , sizeof(strLXMatl  ));
		ConvertStrChar(rData.strLYMatl  , strLYMatl  , sizeof(strLYMatl  ));
	}
};

struct T_FOUN_MATL_ARBAR_UNIT
{
	T_FOUN_MATL_ARBAR_UNIT()
	{
		nLX=D_UNITSYS_NONE;
		strLXGrade=D_UNITSYS_NONE;
		dSpaceX=D_UNITSYS_BASE_LENGTH;
		nLY=D_UNITSYS_NONE;
		strLYGrade=D_UNITSYS_NONE;
		dProtectThick=D_UNITSYS_BASE_LENGTH;
	}
	int nLX;
	int strLXGrade;
	int dSpaceX;  //Only Use to Line Type Founcation
	int nLY;
	int strLYGrade;
	int dProtectThick;
};

struct T_FOUN_MATL
{
	CString strCodeName;
	CString strConcName;
	int nDirRebarType;
	BOOL bRebarInfo;
	T_FOUN_MATL_ARBAR RebarInfo;
	void Initialize()
	{
		strCodeName  = _T("JIS(RC)");
		strConcName  = _T("Fc18");
		nDirRebarType=0;
		bRebarInfo=FALSE;
		RebarInfo.Initialize();
	}
	T_FOUN_MATL() { Initialize(); };
	T_FOUN_MATL(T_FOUN_MATL& src) { *this = src; }
	T_FOUN_MATL& operator=(const T_FOUN_MATL& src)
	{ 
		strCodeName   = src.strCodeName  ;
		strConcName   = src.strConcName  ;
		nDirRebarType = src.nDirRebarType;
		bRebarInfo    = src.bRebarInfo   ;
		RebarInfo     = src.RebarInfo    ;
		return *this;
	}	
};

struct T_FOUN_MATL_CH
{
	char strCodeName[NAME_LENGTH];
	char strConcName[NAME_LENGTH];

	int nDirRebarType;
	BOOL bRebarInfo;
	T_FOUN_MATL_ARBAR_CH RebarInfo;
	void ConvertToString(T_FOUN_MATL& rData)
	{
		ConvertCharStr(strCodeName , rData.strCodeName , sizeof(strCodeName));
		ConvertCharStr(strConcName , rData.strConcName , sizeof(strConcName));
		rData.nDirRebarType=nDirRebarType;
		rData.bRebarInfo=bRebarInfo;
		RebarInfo.ConvertToString(rData.RebarInfo);
	}
	void ConvertToChar(T_FOUN_MATL& rData)
	{
		ConvertStrChar(rData.strCodeName , strCodeName , sizeof(strCodeName ));
		ConvertStrChar(rData.strConcName , strConcName , sizeof(strConcName ));
		nDirRebarType=rData.nDirRebarType;
		bRebarInfo=rData.bRebarInfo;
		RebarInfo.ConvertToChar(rData.RebarInfo);
	}
};

struct T_FOUN_SOIL
{
	double dAddLoad_DIRC;//타입별로 사용하는 단위계가 달라 분리
	double dAddLoad_STRP;
	double dAddLoad_PILE;
	double dAddLoad_MAT ; //매트 기초에서 현재 사용되지 않는다.
	int nPressure;
	double dPressHeight;
	double dUnitWeight;
	void Initialize()
	{
		dAddLoad_DIRC=0.;
		dAddLoad_STRP=0.;
		dAddLoad_PILE=0.;
		dAddLoad_MAT =0.;
		nPressure=0;
		dPressHeight=0.6;
		dUnitWeight=18.;
	}

	static void GetAddLoadbyType(const int nFounType, const T_FOUN_SOIL* pFndSoilD,double* const dAddLoad)
	{
		if(NULL == pFndSoilD) return;
		if(FOUN_DIR == nFounType)
		{
			*dAddLoad = pFndSoilD->dAddLoad_DIRC;
		}
		else if(FOUN_STRP == nFounType)
		{
			*dAddLoad = pFndSoilD->dAddLoad_STRP;
		}
		else if(FOUN_PILE == nFounType)
		{
			*dAddLoad = pFndSoilD->dAddLoad_PILE;
		}
		else if(FOUN_MAT == nFounType)
		{
			ASSERT(FALSE);//매트는 현재 사용하지 않는다.
			*dAddLoad = pFndSoilD->dAddLoad_MAT;
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	static void SetAddLoadbyType(const int nFounType, T_FOUN_SOIL* const pFndSoilD, const double dAddLoad)
	{
		if(NULL == pFndSoilD) return;
		if(FOUN_DIR == nFounType)
		{
			pFndSoilD->dAddLoad_DIRC = dAddLoad;
		}
		else if(FOUN_STRP == nFounType)
		{
			pFndSoilD->dAddLoad_STRP = dAddLoad;
		}
		else if(FOUN_PILE == nFounType)
		{
			pFndSoilD->dAddLoad_PILE = dAddLoad;
		}
		else if(FOUN_MAT == nFounType)
		{
			ASSERT(FALSE);//매트는 현재 사용하지 않는다.
			pFndSoilD->dAddLoad_MAT = dAddLoad;
		}
		else
		{
			ASSERT(FALSE);
		}
	}
};

struct T_FOUN_SOIL_UNIT
{
	T_FOUN_SOIL_UNIT()
	{
		dAddLoad_DIRC=D_UNITSYS_BASE_WEIGHT;
		dAddLoad_STRP=D_UNITSYS_BASE_UNITLOAD;
		dAddLoad_PILE=D_UNITSYS_BASE_WEIGHT;
		dAddLoad_MAT=D_UNITSYS_NONE;
		nPressure=D_UNITSYS_NONE;
		dPressHeight=D_UNITSYS_BASE_LENGTH;
		dUnitWeight=D_UNITSYS_BASE_DENSITY;
	}
	int dAddLoad_DIRC;
	int dAddLoad_STRP;
	int dAddLoad_PILE;
	int dAddLoad_MAT; //매트 기초 단위는 사용되지 않는다.
	int nPressure;
	int dPressHeight;  //Only Use to Line Type Founcation
	int dUnitWeight;
};

#define T_FOUN_K T_FGRP_K
struct T_FOUN_D
{
	double dB;
	double dL;
	double dH;
	double d_h1; //h1 - 근입깊이 ; 절점에서 기초저면까지 길이
	double d_h2; //h2 - 지중보 중심에서 기초저면까지의 길이
	double d_h3; //h3 - 파일기초의 파일묻힘 깊이
	double dDf;
	double dEccenX;
	double dEccenY;
	double dDistance;
	double dAllowableSoilPressure;
	CArray<T_FOUN_PDSTL, T_FOUN_PDSTL&> aPdstl;
	T_FOUN_MATL Matl;
	T_FOUN_SOIL Soil;
	void Initialize()
	{
		dB=3.;
		dL=3.;
		dH=0.6;
		d_h1 = 1.2;
		d_h2 = 1.2;
		d_h3 = 0.3;
		dDf=1.2;
		dEccenX=0.;
		dEccenY=0.;
		dDistance=0.3;
		dAllowableSoilPressure=300.;
		aPdstl.RemoveAll();
		Matl.Initialize();
		Soil.Initialize();
	}
	T_FOUN_D() { Initialize(); };
	T_FOUN_D(T_FOUN_D& src) { *this = src; }
	T_FOUN_D& operator=(const T_FOUN_D& src)
	{ 
		dB = src.dB;
		dL = src.dL;
		dH = src.dH;
		d_h1 = src.d_h1;
		d_h2 = src.d_h2;
		d_h3 = src.d_h3;
		dDf = src.dDf;
		dEccenX = src.dEccenX;
		dEccenY = src.dEccenY;
		dDistance = src.dDistance;
		dAllowableSoilPressure = src.dAllowableSoilPressure;
		aPdstl.Copy(src.aPdstl);
		Matl = src.Matl;
		Soil = src.Soil;
		return *this;
	}	
};

struct T_FOUN_D_CH
{
	double dB;
	double dL;
	double dH;
	double d_h1;
	double d_h2;
	double d_h3;
	double dDf;
	double dEccenX;
	double dEccenY;
	double dDistance;
	double dAllowableSoilPressure;
	CArray<T_FOUN_PDSTL, T_FOUN_PDSTL&> aPdstl;
	T_FOUN_MATL_CH Matl;
	T_FOUN_SOIL Soil;

	void ConvertToString(T_FOUN_D& rData)
	{
		rData.dB = dB;
		rData.dL = dL;
		rData.dH = dH;
		rData.d_h1 = d_h1;  
		rData.d_h2 = d_h2;  
		rData.d_h3 = d_h3;  
		rData.dDf = dDf;
		rData.dEccenX = dEccenX;
		rData.dEccenY = dEccenY;
		rData.dDistance = dDistance;
		rData.dAllowableSoilPressure = dAllowableSoilPressure;
		rData.aPdstl.Copy(aPdstl);
		Matl.ConvertToString(rData.Matl);
		rData.Soil = Soil;
	}
	void ConvertToChar(T_FOUN_D& rData)
	{
		dB=rData.dB;
		dL=rData.dL;
		dH=rData.dH;
		d_h1 = rData.d_h1;
		d_h2 = rData.d_h2;
		d_h3 = rData.d_h3;
		dDf=rData.dDf;
		dEccenX=rData.dEccenX;
		dEccenY=rData.dEccenY;
		dDistance=rData.dDistance;
		dAllowableSoilPressure=rData.dAllowableSoilPressure;
		aPdstl.Copy(rData.aPdstl);
		Matl.ConvertToChar(rData.Matl);
		Soil=rData.Soil;
	}
};

struct T_FOUN_UDRD_D
{
	T_FOUN_K key;
	T_FOUN_D data;
};

struct T_FOUN_UDRD_D_CH
{
	T_FOUN_K key;
	T_FOUN_D_CH data;
};

struct T_FOUN_UDRD_D_CH_RW
{
	T_FOUN_K key;
	double dB;
	double dL;
	double dH;
	double d_h1;
	double d_h2;
	double d_h3;
	double dDf;
	double dEccenX;
	double dEccenY;
	double dDistance;
	double dAllowableSoilPressure;
	int nPdstl;
	T_FOUN_MATL_CH Matl;
	T_FOUN_SOIL Soil;
	
	void GetFoun(T_FOUN_K& rKey, T_FOUN_D_CH& rData)
	{
		rKey=key;
		rData.dB=dB;
		rData.dL=dL;
		rData.dH=dH;
		rData.d_h1 = d_h1;
		rData.d_h2 = d_h2;
		rData.d_h3 = d_h3;
		rData.dDf=dDf;
		rData.dEccenX=dEccenX;
		rData.dEccenY=dEccenY;
		rData.dDistance=dDistance;
		rData.dAllowableSoilPressure=dAllowableSoilPressure;
		rData.aPdstl.SetSize(nPdstl);
		rData.Matl = Matl;
		rData.Soil = Soil;
	}
	void SetFoun(const T_FOUN_K& rKey, const T_FOUN_D_CH& rData)
	{
		key = rKey;
		dB = rData.dB;
		dL = rData.dL;
		dH = rData.dH;
		d_h1 = rData.d_h1;
		d_h2 = rData.d_h2;
		d_h3 = rData.d_h3;
		dDf = rData.dDf;
		dEccenX = rData.dEccenX;
		dEccenY = rData.dEccenY;
		dDistance = rData.dDistance;
		dAllowableSoilPressure = rData.dAllowableSoilPressure;
		nPdstl = (int)rData.aPdstl.GetSize();
		Matl = rData.Matl;
		Soil = rData.Soil;
	}
};

struct T_FOUN_UNIT
{
	T_FOUN_UNIT()
	{
		dB=D_UNITSYS_BASE_LENGTH;
		dL=D_UNITSYS_BASE_LENGTH;
		dH=D_UNITSYS_BASE_LENGTH;
		d_h1 = D_UNITSYS_BASE_LENGTH;
		d_h2 = D_UNITSYS_BASE_LENGTH;
		d_h3 = D_UNITSYS_BASE_LENGTH;
		dDf=D_UNITSYS_BASE_LENGTH;
		dEccenX=D_UNITSYS_BASE_LENGTH;
		dEccenY=D_UNITSYS_BASE_LENGTH;
		dDistance=D_UNITSYS_BASE_LENGTH;
		dAllowableSoilPressure=D_UNITSYS_BASE_STRESS;
	}
	int dB;
	int dL;
	int dH;
	int d_h1;
	int d_h2;
	int d_h3;
	int dDf;
	int dEccenX;
	int dEccenY;
	int dDistance;
	int dAllowableSoilPressure;
};

#define HASHSIZEFOUN 11

struct T_GRND_N
{
	double dLevel;
	double dNValue;
	void Initialize()
	{
		dLevel=0.;
		dNValue=0.;
	}
	T_GRND_N() { Initialize(); };
};

struct T_GRND_N_UNIT
{
	T_GRND_N_UNIT()
	{
		dLevel=D_UNITSYS_BASE_LENGTH;
		dNValue=D_UNITSYS_NONE;
	}
	int dLevel;
	int dNValue;
};

struct T_GRND_STRATUM
{
	enum SOIL{
		SURFACE_SOIL = 0,  //표토
		REFILL_SOIL     ,  //되메임 흙
		LOAM            ,  //롬
		SILT            ,  //실트
		CLAY            ,  //점토
		SANDY_SOIL      ,  //사질토
		SAND_VERY_FINE  ,  //매우 가는 모래
		SAND_FINE       ,  //가는 모래
		SAND_MIDDLE     ,  //중간 모래
		SAND_BOLD       ,  //굵은 모래
		SAND_WITH_GRAVEL,  //모래자갈(역칠토)
		GRAVEL          ,  //자갈
		GROUT           ,  //고결토 ??
		ROCK            ,  //암반
		MOLD            ,  //부식토
		WHASANG_ROCK    ,  //화상석
	};
	double dLevel;
	int nType;
	double dWetUWeight;
	double dSatUWeight;
	double dCohesion;
	double dAngleInter;
	int nPYE;
	double dE0;
	double dAlpha;
//  double dKh0;
//  double dKh;
	double dFC;
	double dCC;

	BOOL bCohesionAuto;
	BOOL bAngleInterAuto;
	BOOL bE0Auto;
	void Initialize()
	{
		dLevel=0.;
		nType=0;
		dWetUWeight=0.;
		dSatUWeight=0.;
		dCohesion=0.;
		dAngleInter=0.;
		nPYE=0;
		dE0=0.;
		dAlpha=0.;
//    dKh0=0.;
//    dKh=0.;
		dFC = 0.;
		dCC = 0.;

		bCohesionAuto=TRUE;
		bAngleInterAuto=TRUE;
		bE0Auto=TRUE;
	}
	T_GRND_STRATUM() { Initialize(); };
};

struct T_GRND_STRATUM_UNIT
{
	T_GRND_STRATUM_UNIT()
	{
		dLevel=D_UNITSYS_BASE_LENGTH;
		nType=D_UNITSYS_NONE;
		dWetUWeight=D_UNITSYS_BASE_DENSITY;
		dSatUWeight=D_UNITSYS_BASE_DENSITY;
		dCohesion=D_UNITSYS_BASE_ELAST;
		dAngleInter=D_UNITSYS_BASE_DEGREE;
		nPYE=D_UNITSYS_NONE;
		dE0=D_UNITSYS_BASE_ELAST;
		dAlpha=D_UNITSYS_NONE;
//    dKh0=D_UNITSYS_BASE_DENSITY;
//    dKh=D_UNITSYS_BASE_DENSITY;
		dFC=D_UNITSYS_NONE;
		dCC=D_UNITSYS_NONE;
	}
	int dLevel;
	int nType;
	int dWetUWeight;
	int dSatUWeight;
	int dCohesion;
	int dAngleInter;
	int nPYE;
	int dE0;
	int dAlpha;
//  int dKh0;
//  int dKh;
	int dFC;
	int dCC;
};

typedef CArray<T_GRND_STRATUM, T_GRND_STRATUM&> T_GRND_STRATUM_LIST;

#define T_GRND_K T_FGRP_K
struct T_GRND_D
{
	double dLogStart;     // 주상도 시작 심도 G.L
	BOOL   bAlluvium;
	double dAlluvium;     // 충적층 심도 G.L
	BOOL bGroundWater;    // 지하수위 사용여부
	double dGroundWater;  // 지하수위 G.L
	CArray<T_GRND_N, T_GRND_N&> aNValue;                // 표준관입시험
	CArray<T_GRND_STRATUM, T_GRND_STRATUM&> aStratum;   // 지층구성
	int nAutoCalCohesion; // 점착력 옵션
	int nAutoCalcAngle;   // 내부마찰각 옵션
	void Initialize()
	{
		dLogStart=0.;
		bAlluvium = FALSE;
		dAlluvium=0.;
		bGroundWater=FALSE;
		dGroundWater=0.;
		aNValue.RemoveAll();
		aStratum.RemoveAll();
		nAutoCalCohesion=0;
		nAutoCalcAngle=0;
	}
	T_GRND_D() { Initialize(); };
	T_GRND_D(T_GRND_D& src) { *this = src; }
	T_GRND_D& operator=(const T_GRND_D& src)
	{ 
		dLogStart = src.dLogStart;
		bAlluvium = src.bAlluvium;
		dAlluvium = src.dAlluvium;
		bGroundWater = src.bGroundWater;
		dGroundWater = src.dGroundWater;
		aNValue.Copy(src.aNValue);
		aStratum.Copy(src.aStratum);
		nAutoCalCohesion = src.nAutoCalCohesion;
		nAutoCalcAngle = src.nAutoCalcAngle;
		return *this;
	}	
};

struct T_GRND_UDRD_D
{
	T_GRND_K key;
	T_GRND_D data;
};

struct T_GRND_UDRD_D_RW
{ 
	T_GRND_K  key;
	double dLogStart;
	BOOL   bAlluvium;
	double dAlluvium;
	BOOL bGroundWater;
	double dGroundWater;
	int nAutoCalCohesion;
	int nAutoCalcAngle;

	int nNumNValue;
	int nNumStratum;

	void GetGrnd(T_GRND_K& rKey, T_GRND_D& rData)
	{
		rKey=key;
		rData.dLogStart=dLogStart;
		rData.bAlluvium=bAlluvium;
		rData.dAlluvium=dAlluvium;
		rData.bGroundWater=bGroundWater;
		rData.dGroundWater=dGroundWater;
		rData.nAutoCalCohesion=nAutoCalCohesion;
		rData.nAutoCalcAngle=nAutoCalcAngle;
	}
	void SetGrnd(const T_GRND_K& rKey, const T_GRND_D& rData)
	{
		key = rKey;
		dLogStart = rData.dLogStart;
		bAlluvium=rData.bAlluvium;
		dAlluvium = rData.dAlluvium;
		bGroundWater = rData.bGroundWater;
		dGroundWater = rData.dGroundWater;
		nAutoCalCohesion = rData.nAutoCalCohesion;
		nAutoCalcAngle = rData.nAutoCalcAngle;

		nNumNValue = (int)rData.aNValue.GetSize();
		nNumStratum = (int)rData.aStratum.GetSize();
	}
};

struct T_GRND_UNIT
{
	T_GRND_UNIT()
	{
		dLogStart=D_UNITSYS_BASE_LENGTH;
		dAlluvium=D_UNITSYS_BASE_LENGTH;
		dGroundWater=D_UNITSYS_BASE_LENGTH;
	}
	int dLogStart;
	int dAlluvium;
	int dGroundWater;
};

#define HASHSIZEGRND 11

struct T_PILE_SECT_PROP
{ 
	enum PROP_SECT
	{
		SECT_D = 0,
		SECT_D1,
		SECT_D2,
		SECT_tw,
		SECT_tw_pipe,
		SECT_Dw,
		SECT_NUMBER,
	};

	enum PROP_STIF
	{
		STIF_Ae = 0,
		STIF_A,
		STIF_Ie,
		STIF_I,
		STIF_PS, 
		STIF_NUMBER,
	};

	double dSect[SECT_NUMBER]; 
	double dStif[STIF_NUMBER]; 

	CString strRebarDia;  //철근 단면
	int     nRebarNumb ;  //개수
	double  dPCStlDia;  //PC강 꼬임선 단면
	int     nPCstlNumb ;  //개수
	double  dShRbDia ;  //전단 보강근
	double  dShRbDist  ;  //간격
	int     nShFormula ; //

	void Initialize()
	{      
		for(int i = 0; i < SECT_NUMBER ; i++)
		{
			dSect[i] = 0.0;
		}

		for(int j = 0; j < STIF_NUMBER ;  j++)
		{
			dStif[j] = 0.0;
		}

		strRebarDia = _T("");
		nRebarNumb  = 0;
		dPCStlDia = 0.;
		nPCstlNumb  = 0;
		dShRbDia  = 0.;
		dShRbDist   = 0.;
		nShFormula = 0;
	}
	T_PILE_SECT_PROP(){ Initialize(); };
	T_PILE_SECT_PROP(T_PILE_SECT_PROP& src) { *this = src; }
	T_PILE_SECT_PROP& operator=(const T_PILE_SECT_PROP& src)
	{
		memcpy(dSect, src.dSect, sizeof(dSect));
		memcpy(dStif, src.dStif, sizeof(dStif));
		strRebarDia = src.strRebarDia;
		nRebarNumb  = src.nRebarNumb ;
		dPCStlDia = src.dPCStlDia;
		nPCstlNumb  = src.nPCstlNumb ;
		dShRbDia  = src.dShRbDia ;
		dShRbDist   = src.dShRbDist  ;
		nShFormula  = src.nShFormula ;
		return *this;  
	}
};


struct T_PILE_SECT_PROP_CH
{ 
	double dSect[T_PILE_SECT_PROP::SECT_NUMBER]; 
	double dStif[T_PILE_SECT_PROP::STIF_NUMBER]; 
	
	char    strRebarDia[NAME_LENGTH];  //철근 단면
	int     nRebarNumb ;  //개수
	double  dPCStlDia;  //PC강 꼬임선 단면
	int     nPCstlNumb ;  //개수
	double  dShRbDia;  //전단 보강근
	double  dShRbDist  ;  //간격
	int     nShFormula ; //
	void ConvertToString(T_PILE_SECT_PROP& rData)
	{
		memcpy(rData.dSect, dSect, sizeof(rData.dSect));
		memcpy(rData.dStif, dStif, sizeof(rData.dStif));
		ConvertCharStr(strRebarDia     , rData.strRebarDia     , sizeof(strRebarDia     ));    
		rData.nRebarNumb        =nRebarNumb        ;
		rData.dPCStlDia       =dPCStlDia        ;
		rData.nPCstlNumb        =nPCstlNumb        ;
		rData.dShRbDia        =dShRbDia         ;
		rData.dShRbDist         =dShRbDist         ;
		rData.nShFormula        = nShFormula;
	}

	void ConvertToChar(T_PILE_SECT_PROP& rData)
	{
		memcpy(dSect, rData.dSect, sizeof(dSect));
		memcpy(dStif, rData.dStif, sizeof(dStif));
		ConvertStrChar(rData.strRebarDia , strRebarDia , sizeof(strRebarDia));
		nRebarNumb =rData.nRebarNumb ;
		dPCStlDia =rData.dPCStlDia ;
		nPCstlNumb =rData.nPCstlNumb ;
		dShRbDia =rData.dShRbDia ;
		dShRbDist =rData.dShRbDist ;
		nShFormula = rData.nShFormula;
	}
};

struct T_PILE_SECT_PROP_UNIT
{
	T_PILE_SECT_PROP_UNIT()
	{
		for(int i = 0; i < T_PILE_SECT_PROP::SECT_NUMBER ; i++)
		{
			dSect[i] = D_UNITSYS_BASE_LENGTH;
		}

		dStif[T_PILE_SECT_PROP::STIF_Ae] = D_UNITSYS_BASE_AREA;
		dStif[T_PILE_SECT_PROP::STIF_A] = D_UNITSYS_BASE_AREA;
		dStif[T_PILE_SECT_PROP::STIF_Ie] = D_UNITSYS_BASE_STIF;
		dStif[T_PILE_SECT_PROP::STIF_I]  = D_UNITSYS_BASE_STIF;
		dStif[T_PILE_SECT_PROP::STIF_PS] = D_UNITSYS_BASE_ELAST     ;

		nRebarNumb = D_UNITSYS_NONE     ;
		dPCStlDia = D_UNITSYS_BASE_LENGTH;
		nPCstlNumb = D_UNITSYS_NONE     ;
		dShRbDia = D_UNITSYS_BASE_LENGTH;
		dShRbDist  = D_UNITSYS_BASE_LENGTH;        
	}

	int dSect[T_PILE_SECT_PROP::SECT_NUMBER]; //0:D, D1  1:D2  2:tw  3:Dw
	int dStif[T_PILE_SECT_PROP::STIF_NUMBER]; //0:Ae  1:Ie   2:I  3:Ps
	
	int nRebarNumb ;  //개수
	int dPCStlDia;  //PC강 꼬임선 단면
	int nPCstlNumb ;  //개수
	int dShRbDia ;  //전단 보강근
	int  dShRbDist  ;  //간격

};


struct T_PILE_DETAIL
{
	enum TYPE{ TOP = 1, BOTTOM = 3 };
	double dLevel        ; //깊이
	double dDia         ; //직경
	int    nExtend       ; //확두/확저
	double dExtDia      ; //확두부 직경
	double dExtAngle     ; //확두부 각도
	double dDt1          ; //dt1
	double dExt_HeadLen  ; //머리부 직선 길이
	int    nHeadCondition; //두부조건
	double dHeadKTheta   ; //k
	int    nEndCondition ; //선단조건
	CString strCodeName  ; //철근 코드
	CString strMainMatl  ; //주근 재질
	CString strMainDia   ; //주근 직경
	int    nMainNum      ; //주근 개수
	CString strStripMatl ; //후프근 재질
	CString strStripDia  ; //후프근 직경
	double dStripSpace   ; //후프근 간격 
	double dProtectThick ; //피복 두께
	double dCorLen       ; 
	double dShearRatio   ;
	double dAlpha        ; //alpha

	int nSectShape;     // 형상
	int nSectType;      //단면

	CString strConcCodeName ;
	CString strConcName     ;
	CString strStelCodeName ;
	CString strStelName     ;

	T_PILE_SECT_PROP SectProp;

	void Initialize()
	{
		dLevel=0.0;
		dDia=0.0;
		nExtend=0;
		dExtDia=0.0;
		dExtAngle=12.0;
		dDt1=0.05;
		dExt_HeadLen=0.0;
		nHeadCondition=0;
		dHeadKTheta=0.0;
		nEndCondition=0;
		strCodeName = _T("JIS(RC)");
		strMainMatl = _T("SD345"  );
		strMainDia  = _T("D22"    );
		nMainNum=0;
		strStripMatl= _T("SD295A");
		strStripDia = _T("D13"   );
		dStripSpace=0.0;
		dProtectThick=0.0;
		dCorLen=0.0;
		dShearRatio=2;
		dAlpha=1.5;

		nSectShape=0;
		nSectType=0;

		strConcCodeName =_T("JIS(RC)");
		strConcName     =_T("");
		strStelCodeName = MATLCODE_STL_JIS;
		strStelName     =_T("");

		SectProp.Initialize();
	}
	T_PILE_DETAIL() { Initialize(); };
	T_PILE_DETAIL(T_PILE_DETAIL& src) { *this = src; }
	T_PILE_DETAIL& operator=(const T_PILE_DETAIL& src)
	{ 
		dLevel=src.dLevel;
		dDia=src.dDia;
		nExtend=src.nExtend;
		dExtDia=src.dExtDia;
		dExtAngle=src.dExtAngle;
		dDt1=src.dDt1;
		dExt_HeadLen=src.dExt_HeadLen;
		nHeadCondition=src.nHeadCondition;
		dHeadKTheta=src.dHeadKTheta;
		nEndCondition=src.nEndCondition;
		strCodeName=src.strCodeName;
		strMainMatl=src.strMainMatl;
		strMainDia =src.strMainDia ;
		nMainNum=src.nMainNum;
		strStripMatl=src.strStripMatl;
		strStripDia =src.strStripDia ;
		dStripSpace=src.dStripSpace;
		dProtectThick=src.dProtectThick;
		dCorLen=src.dCorLen;
		dShearRatio=src.dShearRatio;
		dAlpha = src.dAlpha;

		nSectShape = src.nSectShape;
		nSectType = src.nSectType;

		strConcCodeName = src.strConcCodeName;
		strConcName     = src.strConcName    ;
		strStelCodeName = src.strStelCodeName;
		strStelName     = src.strStelName    ;    

		SectProp = src.SectProp;
		return *this;
	}	
};

struct T_PILE_DETAIL_CH
{
	double dLevel        ; //깊이
	double dDia          ; //직경
	int    nExtend       ; //확두/확저
	double dExtDia       ; //확두부 직경
	double dExtAngle     ; //확두부 각도
	double dDt1  ; //사용안함
	double dExt_HeadLen  ; //머리부 직선 길이
	int    nHeadCondition; //두부조건
	double dHeadKTheta   ; //k
	int    nEndCondition ; //선단조건
	char   strCodeName[NAME_LENGTH]  ; //철근 코드
	char   strMainMatl[NAME_LENGTH]  ; //주근 재질
	char   strMainDia [NAME_LENGTH]  ; //주근 직경
	int    nMainNum      ; //주근 개수
	char   strStripMatl[NAME_LENGTH] ; //후프근 재질
	char   strStripDia[NAME_LENGTH]  ; //후프근 직경
	double dStripSpace   ; //후프근 간격 
	double dProtectThick ; //피복 두께
	double dCorLen       ; 
	double dShearRatio   ;
	int nSectShape;
	int nSectType;
	char strConcCodeName[NAME_LENGTH];
	char strConcName    [NAME_LENGTH];
	char strStelCodeName[NAME_LENGTH];
	char strStelName    [NAME_LENGTH];

	T_PILE_SECT_PROP_CH SectProp;

	//[Check before Test...!!] Hjang:검증 끝나면 위치 옮길 것
	double  dAlpha      ;//검증 파일 변환 해줘야 해서 임시로 맨 마지막에
	
	void ConvertToString(T_PILE_DETAIL& rData)
	{
		rData.dLevel        =dLevel        ;
		rData.dDia          =dDia          ;
		rData.nExtend       =nExtend       ;
		rData.dExtDia       =dExtDia       ;
		rData.dExtAngle     =dExtAngle     ;
		rData.dDt1  =dDt1  ;
		rData.dExt_HeadLen  =dExt_HeadLen  ;
		rData.nHeadCondition=nHeadCondition;
		rData.dHeadKTheta   =dHeadKTheta   ;
		rData.nEndCondition =nEndCondition ;
		ConvertCharStr(strCodeName, rData.strCodeName, sizeof(strCodeName));
		ConvertCharStr(strMainMatl, rData.strMainMatl, sizeof(strMainMatl));
		ConvertCharStr(strMainDia , rData.strMainDia , sizeof(strMainDia ));
		rData.nMainNum      =nMainNum      ;
		ConvertCharStr(strStripMatl , rData.strStripMatl , sizeof(strStripMatl ));
		ConvertCharStr(strStripDia  , rData.strStripDia , sizeof(strStripDia ));
		rData.dStripSpace   =dStripSpace   ;
		rData.dProtectThick =dProtectThick ;
		rData.dCorLen       =dCorLen       ;
		rData.dShearRatio   =dShearRatio   ;
		rData.dAlpha        =dAlpha        ;

		rData.nSectShape = nSectShape;
		rData.nSectType = nSectType;   
		ConvertCharStr(strConcCodeName, rData.strConcCodeName, sizeof(strConcCodeName));
		ConvertCharStr(strConcName    , rData.strConcName    , sizeof(strConcName    ));
		ConvertCharStr(strStelCodeName, rData.strStelCodeName, sizeof(strStelCodeName));
		ConvertCharStr(strStelName     , rData.strStelName     , sizeof(strStelName     ));  

		SectProp.ConvertToString(rData.SectProp);
	}

	void ConvertToChar(T_PILE_DETAIL& rData)
	{
		dLevel        =rData.dLevel        ;
		dDia          =rData.dDia          ;
		nExtend       =rData.nExtend       ;
		dExtDia       =rData.dExtDia       ;
		dExtAngle     =rData.dExtAngle     ;
		dDt1  =rData.dDt1  ;
		dExt_HeadLen  =rData.dExt_HeadLen  ;
		nHeadCondition=rData.nHeadCondition;
		dHeadKTheta   =rData.dHeadKTheta   ;
		nEndCondition =rData.nEndCondition ;
		ConvertStrChar(rData.strCodeName, strCodeName, sizeof(strCodeName));
		ConvertStrChar(rData.strMainMatl, strMainMatl, sizeof(strMainMatl));
		ConvertStrChar(rData.strMainDia , strMainDia , sizeof(strMainDia ));
		nMainNum      =rData.nMainNum      ;
		ConvertStrChar(rData.strStripMatl, strStripMatl, sizeof(strStripMatl));
		ConvertStrChar(rData.strStripDia , strStripDia , sizeof(strStripDia ));
		dStripSpace   =rData.dStripSpace   ;
		dProtectThick =rData.dProtectThick ;
		dCorLen       =rData.dCorLen       ;
		dShearRatio   =rData.dShearRatio   ;
		dAlpha        =rData.dAlpha        ;

		nSectShape=rData.nSectShape;
		nSectType=rData.nSectType;
		ConvertStrChar(rData.strConcCodeName, strConcCodeName, sizeof(strConcCodeName));
		ConvertStrChar(rData.strConcName    , strConcName    , sizeof(strConcName    ));
		ConvertStrChar(rData.strStelCodeName, strStelCodeName, sizeof(strStelCodeName));
		ConvertStrChar(rData.strStelName    , strStelName    , sizeof(strStelName    ));

		SectProp.ConvertToChar(rData.SectProp);
	}
};

struct T_PILE_DETAIL_UNIT
{
	T_PILE_DETAIL_UNIT()
	{
		dLevel = D_UNITSYS_BASE_LENGTH;
		dDia = D_UNITSYS_BASE_LENGTH;
		dExtArea = D_UNITSYS_BASE_LENGTH;
		dExtAngle =  D_UNITSYS_BASE_DEGREE;  
		dDt1 = D_UNITSYS_BASE_LENGTH;
		dExt_HeadLen = D_UNITSYS_BASE_LENGTH;
		dHeadKTheta = D_UNITSYS_BASE_ELAST;
		dStripSpace = D_UNITSYS_BASE_LENGTH;
		dProtectThick = D_UNITSYS_BASE_LENGTH;
		dCorLen = D_UNITSYS_BASE_LENGTH;
	}
	int dLevel;
	int dDia;
	int dExtArea;
	int dExtAngle;
	int dDt1;
	int dExt_HeadLen;
	int dHeadKTheta;
	int dStripSpace;
	int dProtectThick;
	int dCorLen;
};

typedef CArray<T_PILE_DETAIL, T_PILE_DETAIL&> T_PILE_DETAIL_LIST;

struct T_PILE_ARRANGE
{
	double dX;
	double dY;

	T_PILE_ARRANGE() { Initialize(); }
	void Initialize() { memset(this, 0, sizeof(T_PILE_ARRANGE)); }  
};

struct T_PILE_ARRANGE_UNIT
{
	T_PILE_ARRANGE_UNIT()
	{
		dX = D_UNITSYS_BASE_LENGTH;
		dY = D_UNITSYS_BASE_LENGTH;
	}
	int dX;
	int dY;
};

struct T_PUBLIC_DETAIL
{
	enum CALC_SAND //사질토 계산 방법
	{
		b = 0,     //b = P1
		b_NsP,     //b*Ns~ = P
		b_NsP_P2,  //b*Ns~ P1*Ns~+P2
	};
	//사질토 
	int    nSandCalcWay; //계산 방법
	double dSandP1     ;
	double dSandP2     ;
	double dNsValUpLimt;
	double dNsPrmDnLimt;

	enum CALC_COHE //점성토 계산 방법
	{
		y = 0,    // y = P1
		y_quP,    // y*qu~=P1
		y_quP_Nc, // y*qu~=P1*Nc~+P2
		y_quP_quP,  // y*qu~=P1*qu~+P2

	};
	//점성토
	int    nCoheCalcWay; //계산 방법
	double dCoheP1     ;
	double dCoheP2     ;
	double dQuValUpLimt;
	double dQuPrmDnLimt;

	void Initialize()
	{  
		nSandCalcWay = CALC_SAND::b;
		dSandP1      =  2.0;
		dSandP2      =  0.0;
		dNsValUpLimt =  0.0;
		dNsPrmDnLimt = 25.0;
		
		nCoheCalcWay = CALC_COHE::y;
		dCoheP1      =   0.5;
		dCoheP2      =   0.0;
		dQuValUpLimt =   0.0;
		dQuPrmDnLimt = 100.0;
	}
};

struct T_PUBLIC_DETAIL_UNIT
{
	T_PUBLIC_DETAIL_UNIT()
	{
		nSandCalcWay = D_UNITSYS_NONE; 
		dSandP1      = D_UNITSYS_NONE;
		dSandP2      = D_UNITSYS_NONE;
		dNsValUpLimt = D_UNITSYS_NONE;
		dNsPrmDnLimt = D_UNITSYS_NONE;
		nCoheCalcWay = D_UNITSYS_NONE; 
		dCoheP1      = D_UNITSYS_NONE;
		dCoheP2      = D_UNITSYS_NONE;
		dQuValUpLimt = D_UNITSYS_BASE_STRESS;
		dQuPrmDnLimt = D_UNITSYS_BASE_STRESS;
	}
	int nSandCalcWay; 
	int dSandP1     ;
	int dSandP2     ;
	int dNsValUpLimt;
	int dNsPrmDnLimt;

	int nCoheCalcWay; 
	int dCoheP1     ;
	int dCoheP2     ;
	int dQuValUpLimt;
	int dQuPrmDnLimt;
};

struct T_PILE_PUBLIC
{
	double dBearCoef; //선단 지지력
	double dNRangeDN; //선단 N치의 범위 - 아래쪽
	double dNRangeUP; //선단 N치의 범위 - 위쪽
	double dNValLimt; //N치의 상한치
	double nNprmLimt; //N~치의 상한치
	
	//기성강관말뚝의 선단유효면적 : 강관 말뚝의 경우에만 사용
	int nValidArCalc; //계산 방법
	double nValidCoef_e; //계수

	T_PUBLIC_DETAIL top;
	BOOL            bMid;
	T_PUBLIC_DETAIL mid;
	BOOL            bBot;
	T_PUBLIC_DETAIL bot;
	void Initialize()
	{
		dBearCoef = 250.0;
		dNRangeDN =   1.0;
		dNRangeUP =   4.0;
		dNValLimt = 100.0;
		nNprmLimt =  60.0;

		nValidArCalc = 0;
		nValidCoef_e = 1.2;
		
		top.Initialize();
		bMid = FALSE;
		mid.Initialize();
		bBot = FALSE;
		bot.Initialize();
	}
};


struct T_PILE_PUBLIC_UNIT
{
	T_PILE_PUBLIC_UNIT()
	{
		dBearCoef = D_UNITSYS_NONE; 
		dNRangeDN = D_UNITSYS_NONE; 
		dNRangeUP = D_UNITSYS_NONE; 
		dNValLimt = D_UNITSYS_NONE; 
		nNprmLimt = D_UNITSYS_NONE;  
		nValidArCalc = D_UNITSYS_NONE;
		nValidCoef_e = D_UNITSYS_NONE;
	}
	int dBearCoef   ; 
	int dNRangeDN   ; 
	int dNRangeUP   ; 
	int dNValLimt   ; 
	int nNprmLimt   ; 
	int nValidArCalc;
	int nValidCoef_e;
};


#define T_PILE_K T_FGRP_K
struct T_PILE_D
{
	enum ARRANGE{
		ONE_A  = 0,   // 1
		TWO_A     ,       // 2 
		THRE_A    , //3 - Triangle
		THRE_B    , //3 - alphabet L Shape
		THRE_C    , //3 - Horizontal Line
		///
		FOUR_A    , //4 - Rectangle   
		FOUR_B    , //4 - ㅗ
		FOUR_C    , //4 - Horizontal Line
		FIVE_A    , //5 - X
		FIVE_B    , //5 - +
		///          
								//     . .
		FIVE_C    , //5 - . . .      
								
								//    . . . 
		SIX_A     , //6 - . . .

								 //     . .
		SEVE_A     , //7 - . . . 
								 //     . .

								 //     . . .
		SEVE_B     , //7 - . . . .

								 //    . . .
		EIGH_A     , //8 -  . . 
								 //    . . .
		///
								 //    . . . .
		EIGH_B     , //8 - . . . .    
		
								 //    . . . 
		NINE       , //9 - . . . 
								 //    . . . 

								 //      . . .
		TEN        , //10 - . . . .  
								 //      . . .

	};

	enum RBAR_CONNECT
	{
		MAIN_SETTL_1 = 0,   //주근 정착방식 1
		MAIN_SETTL_2,       //주근 정착방식 2
		INNER_REINFO,       //내부 메움 보강 방식
	};
	int nPileType;      // 말뚝종류
											 
	int nPileArrange;       //말뚝본수 템플릿 enum:ARRANGE
	double dPileAngle;  // 말뚝배치회전각
	CArray<T_PILE_ARRANGE, T_PILE_ARRANGE&> aPileArrange;  //파일 배치 좌표 정보

	//말둑 간격 조정
	BOOL   bAutoPIDS; //기초판 크기 자동변경
	double dPileDist; //말뚝 간격
	double dEdgeDist; //말뚝 연단 거리
	BOOL   bAutoPLSZ; //기초판 크기 자동변경
	BOOL   bAutoCOPL; //군말뚝 계수 자동계산
	double dCoefPile; //군말뚝 계수

	//말뚝 머리 설계 설정
	int    nRebaSettl; //RBAR_CONNECT::  접합 방식 
	double dRebaF;   //f
	double dRebaN;   //N
	BOOL   bRebaN;
	double dRebaM;   //M
	BOOL   bRebaM;
													
	CString strPHeadCodeName;
	CString strPHeadConcName;
	CString strPHRebarDia   ;
	CString strPHRebarMatl  ;
	int     nPHMainNum      ;
	double  dPHProtectThick ;
	
	T_PILE_PUBLIC pbMethod;

	int nApplyType;     // 설계 적용 말뚝 : 자동산정, 사용자정의
	int nApplyPile;     // 사용자정의 번호
	CArray<T_PILE_DETAIL, T_PILE_DETAIL&> aPileDetail;
	void Initialize(int nPileTp = PILE_PLACE)
	{
		nPileType=nPileTp;

		nPileArrange=T_PILE_D::FIVE_A;
		dPileAngle=0.;
		aPileArrange.RemoveAll();

		bAutoPIDS = TRUE;
		dPileDist = 0; //말뚝 간격
		dEdgeDist = 0; //말뚝 연단 거리
		bAutoPLSZ =  TRUE; //기초판 크기 자동변경
		bAutoCOPL = TRUE;
		dCoefPile = 1.0;
		
		//말뚝 머리 설계 설정
		if(PILE_STL_PIPE == nPileType)
		{
			nRebaSettl = RBAR_CONNECT::MAIN_SETTL_2;
		}
		else
		{
			nRebaSettl = RBAR_CONNECT::MAIN_SETTL_1;
		}

		dRebaF = 0.1;
		dRebaN = 0;   
		bRebaN = FALSE;
		dRebaM = 0;   
		bRebaM = FALSE;

		strPHeadCodeName = _T("JIS(RC)");
		strPHeadConcName = _T("Fc24");
		strPHRebarDia    = _T("D22");
		strPHRebarMatl   = _T("SD345");
		nPHMainNum       = 8;
		dPHProtectThick  = 0.01;

		pbMethod.Initialize();

		nApplyType=0;
		nApplyPile=0;
		aPileDetail.RemoveAll();
	}
	int GetPileNum()
	{
		return GetPileNum(nPileArrange);
	}
	static int GetPileNum(int nPileArrange)
	{
		int nPileNum=0;
		switch(nPileArrange)
		{
		case T_PILE_D::ONE_A :
			nPileNum=1;
			break;
		case T_PILE_D::TWO_A :
			nPileNum=2;
			break;
		case T_PILE_D::THRE_A :
		case T_PILE_D::THRE_B :
		case T_PILE_D::THRE_C :
			nPileNum=3;
			break;
		case T_PILE_D::FOUR_A :
		case T_PILE_D::FOUR_B :
		case T_PILE_D::FOUR_C :
			nPileNum=4;
			break;
		case T_PILE_D::FIVE_A :
		case T_PILE_D::FIVE_B :
		case T_PILE_D::FIVE_C :
			nPileNum=5;
			break;
		case T_PILE_D::SIX_A :
			nPileNum=6;
			break;
		case T_PILE_D::SEVE_A :
		case T_PILE_D::SEVE_B :
			nPileNum=7;
			break;
		case T_PILE_D::EIGH_A :
		case T_PILE_D::EIGH_B :
			nPileNum=8;
			break;
		case T_PILE_D::NINE :
			nPileNum=9;
			break;
		case T_PILE_D::TEN :
			nPileNum=10;
			break;
		default:
			ASSERT(0);
			break;
		}
		
		return nPileNum;
	}
	T_PILE_D() { Initialize(); };
	T_PILE_D(T_PILE_D& src) { *this = src; }
	T_PILE_D& operator=(const T_PILE_D& src)
	{ 
		nPileType = src.nPileType;
		nPileArrange=src.nPileArrange;
		dPileAngle=src.dPileAngle;
		aPileArrange.Copy(src.aPileArrange);

		bAutoPIDS = src.bAutoPIDS ;
		dPileDist = src.dPileDist ;
		dEdgeDist = src.dEdgeDist ;
		bAutoPLSZ = src.bAutoPLSZ ;
		bAutoCOPL = src.bAutoCOPL ;
		dCoefPile = src.dCoefPile ;
												
		nRebaSettl= src.nRebaSettl;
		dRebaF    = src.dRebaF    ;   
		dRebaN    = src.dRebaN    ;   
		bRebaN    = src.bRebaN    ;
		dRebaM    = src.dRebaM    ;   
		bRebaM    = src.bRebaM    ;
																		 
		strPHeadCodeName = src.strPHeadCodeName;
		strPHeadConcName = src.strPHeadConcName;
		strPHRebarDia    = src.strPHRebarDia   ;
		strPHRebarMatl   = src.strPHRebarMatl  ;
		nPHMainNum       = src.nPHMainNum      ;
		dPHProtectThick  = src.dPHProtectThick ;
																		 
		pbMethod  = src.pbMethod  ;

		nApplyType=src.nApplyType;
		nApplyPile=src.nApplyPile;
		aPileDetail.Copy(src.aPileDetail);
		return *this;
	}	
};

struct T_PILE_D_CH
{
	int nPileType;
	int nPileArrange;
	double dPileAngle;
	CArray<T_PILE_ARRANGE, T_PILE_ARRANGE&> aPileArrange;

	int    bAutoPIDS ;
	double dPileDist ; 
	double dEdgeDist ; 
	int    bAutoPLSZ ; 
	int    bAutoCOPL ;
	double dCoefPile ;
									 
	int    nRebaSettl;
	double dRebaF    ;
	double dRebaN    ;
	int    bRebaN    ;
	double dRebaM    ;
	int    bRebaM    ;

	T_PILE_PUBLIC pbMethod;

	char    strPHeadCodeName[NAME_LENGTH];
	char    strPHeadConcName[NAME_LENGTH];
	char    strPHRebarDia   [NAME_LENGTH];
	char    strPHRebarMatl  [NAME_LENGTH];
	int     nPHMainNum      ;
	double  dPHProtectThick ;

	int nApplyType;
	int nApplyPile;
	CArray<T_PILE_DETAIL_CH, T_PILE_DETAIL_CH&> aPileDetail;

	void ConvertToString(T_PILE_D& rData)
	{
		rData.nPileType = nPileType;
		rData.nPileArrange = nPileArrange;
		rData.dPileAngle = dPileAngle;
		rData.aPileArrange.Copy(aPileArrange);

		rData.bAutoPIDS  = bAutoPIDS ;
		rData.dPileDist  = dPileDist ;
		rData.dEdgeDist  = dEdgeDist ;
		rData.bAutoPLSZ  = bAutoPLSZ ;
		rData.bAutoCOPL  = bAutoCOPL ;
		rData.dCoefPile  = dCoefPile ;
										
		rData.nRebaSettl = nRebaSettl;
		rData.dRebaF     = dRebaF    ;
		rData.dRebaN     = dRebaN    ;
		rData.bRebaN     = bRebaN    ;
		rData.dRebaM     = dRebaM    ;
		rData.bRebaM     = bRebaM    ;
		
		ConvertCharStr(strPHeadCodeName, rData.strPHeadCodeName, sizeof(strPHeadCodeName));
		ConvertCharStr(strPHeadConcName, rData.strPHeadConcName, sizeof(strPHeadConcName));
		ConvertCharStr(strPHRebarDia   , rData.strPHRebarDia   , sizeof(strPHRebarDia   ));
		ConvertCharStr(strPHRebarMatl  , rData.strPHRebarMatl  , sizeof(strPHRebarMatl  ));
		rData.nPHMainNum = nPHMainNum;
		rData.dPHProtectThick = dPHProtectThick;

		rData.pbMethod   = pbMethod;

		rData.nApplyType = nApplyType;
		rData.nApplyPile = nApplyPile;
		
		int nDetailSize = (int)aPileDetail.GetSize();
		rData.aPileDetail.SetSize(nDetailSize);
		for(int k = 0 ; k < nDetailSize ; k ++)
		{
			aPileDetail[k].ConvertToString(rData.aPileDetail[k]);
		}
	}
	void ConvertToChar(T_PILE_D& rData)
	{
		nPileType=rData.nPileType;
		nPileArrange=rData.nPileArrange;
		dPileAngle=rData.dPileAngle;
		aPileArrange.Copy(rData.aPileArrange);
		bAutoPIDS =rData.bAutoPIDS ;
		dPileDist =rData.dPileDist ;
		dEdgeDist =rData.dEdgeDist ;
		bAutoPLSZ =rData.bAutoPLSZ ;
		bAutoCOPL = rData.bAutoCOPL;
		dCoefPile = rData.dCoefPile;
										
		nRebaSettl=rData.nRebaSettl;
		dRebaF    =rData.dRebaF    ;
		dRebaN    =rData.dRebaN    ;
		bRebaN    =rData.bRebaN    ;
		dRebaM    =rData.dRebaM    ;
		bRebaM    =rData.bRebaM    ;

		ConvertStrChar(rData.strPHeadCodeName, strPHeadCodeName, sizeof(strPHeadCodeName));
		ConvertStrChar(rData.strPHeadConcName, strPHeadConcName, sizeof(strPHeadConcName));
		ConvertStrChar(rData.strPHRebarDia   , strPHRebarDia   , sizeof(strPHRebarDia   ));
		ConvertStrChar(rData.strPHRebarMatl  , strPHRebarMatl  , sizeof(strPHRebarMatl  ));
		nPHMainNum = rData.nPHMainNum;
		dPHProtectThick = rData.dPHProtectThick;

		pbMethod = rData.pbMethod  ;

		nApplyType=rData.nApplyType;
		nApplyPile=rData.nApplyPile;
		
		int nDetailSize = (int)rData.aPileDetail.GetSize();
		aPileDetail.SetSize(nDetailSize);
		for(int k = 0; k < nDetailSize; k++)
		{
			aPileDetail[k].ConvertToChar(rData.aPileDetail[k]);
		}

	}
};

struct T_PILE_UDRD_D
{
	T_PILE_K key;
	T_PILE_D data;
};

struct T_PILE_UDRD_D_CH
{
	T_PILE_K key;
	T_PILE_D_CH data;
};

struct T_PILE_UDRD_D_CH_RW
{ 
	T_PILE_K  key;
	int nPileType;

	int nPileArrange;
	double dPileAngle;
	int nPileArrangeNum;

	int    bAutoPIDS ;
	double dPileDist ; 
	double dEdgeDist ; 
	int    bAutoPLSZ ;
									 
	int    nRebaSettl;
	double dRebaF    ;
	double dRebaN    ;
	int    bRebaN    ;
	double dRebaM    ;
	int    bRebaM    ;

	char    strPHeadCodeName[NAME_LENGTH];
	char    strPHeadConcName[NAME_LENGTH];
	char    strPHRebarDia   [NAME_LENGTH];
	char    strPHRebarMatl  [NAME_LENGTH];
	int     nPHMainNum      ;
	double  dPHProtectThick ;

	T_PILE_PUBLIC pbMethod;

	int nApplyType;
	int nApplyPile;
	int nNumPileDetail;

	int    bAutoCOPL ;
	double dCoefPile ;
	void GetPile(T_PILE_K& rKey, T_PILE_D_CH& rData)
	{
		rKey=key;
		rData.nPileType=nPileType;
		rData.nPileArrange=nPileArrange;
		rData.dPileAngle=dPileAngle;
		rData.aPileArrange.SetSize(nPileArrangeNum);

		rData.bAutoPIDS =bAutoPIDS ;
		rData.dPileDist =dPileDist ;
		rData.dEdgeDist =dEdgeDist ;
		rData.bAutoPLSZ =bAutoPLSZ ;
		rData.bAutoCOPL =bAutoCOPL ;
		rData.dCoefPile =dCoefPile ;
										
		rData.nRebaSettl=nRebaSettl;
		rData.dRebaF    =dRebaF    ;
		rData.dRebaN    =dRebaN    ;
		rData.bRebaN    =bRebaN    ;
		rData.dRebaM    =dRebaM    ;
		rData.bRebaM    =bRebaM    ;

		memcpy(rData.strPHeadCodeName, strPHeadCodeName, sizeof(strPHeadCodeName));
		memcpy(rData.strPHeadConcName, strPHeadConcName, sizeof(strPHeadConcName));
		memcpy(rData.strPHRebarDia   , strPHRebarDia   , sizeof(strPHRebarDia   ));
		memcpy(rData.strPHRebarMatl  , strPHRebarMatl  , sizeof(strPHRebarMatl  ));
		rData.nPHMainNum = nPHMainNum;
		rData.dPHProtectThick = dPHProtectThick;


		rData.pbMethod = pbMethod  ;

		rData.nApplyType=nApplyType;
		rData.nApplyPile=nApplyPile;
		rData.aPileDetail.SetSize(nNumPileDetail);
	}
	void SetPile(const T_PILE_K& rKey, T_PILE_D_CH& rData)
	{
		key=rKey;
		nPileType = rData.nPileType;
		nPileArrange = rData.nPileArrange;
		dPileAngle = rData.dPileAngle;
		nPileArrangeNum = (int)rData.aPileArrange.GetSize();

		bAutoPIDS  = rData.bAutoPIDS ;
		dPileDist  = rData.dPileDist ;
		dEdgeDist  = rData.dEdgeDist ;
		bAutoPLSZ  = rData.bAutoPLSZ ;
		bAutoCOPL  = rData.bAutoCOPL ;
		dCoefPile  = rData.dCoefPile ;
										
		nRebaSettl = rData.nRebaSettl;
		dRebaF     = rData.dRebaF    ;
		dRebaN     = rData.dRebaN    ;
		bRebaN     = rData.bRebaN    ;
		dRebaM     = rData.dRebaM    ;
		bRebaM     = rData.bRebaM    ;

		memcpy(strPHeadCodeName, rData.strPHeadCodeName, sizeof(strPHeadCodeName));
		memcpy(strPHeadConcName, rData.strPHeadConcName, sizeof(strPHeadConcName));
		memcpy(strPHRebarDia   , rData.strPHRebarDia   , sizeof(strPHRebarDia   ));
		memcpy(strPHRebarMatl  , rData.strPHRebarMatl  , sizeof(strPHRebarMatl  ));
		nPHMainNum = rData.nPHMainNum;
		dPHProtectThick = rData.dPHProtectThick;

		pbMethod   = rData.pbMethod  ;

		nApplyType = rData.nApplyType;
		nApplyPile = rData.nApplyPile;
		nNumPileDetail = (int)rData.aPileDetail.GetSize();
	}
};

struct T_PILE_UNIT
{
	T_PILE_UNIT()
	{
		nPileType   = D_UNITSYS_NONE;  
		nPileArrange= D_UNITSYS_NONE;
		dPileAngle  = D_UNITSYS_NONE;
		bAutoPIDS   = D_UNITSYS_NONE;
		dPileDist   = D_UNITSYS_BASE_LENGTH;
		dEdgeDist   = D_UNITSYS_BASE_LENGTH;
		bAutoPLSZ   = D_UNITSYS_NONE;                
		nRebaSettl  = D_UNITSYS_NONE;
		dRebaF      = D_UNITSYS_BASE_LENGTH;
		dRebaN      = D_UNITSYS_BASE_FORCE;
		bRebaN      = D_UNITSYS_NONE;
		dRebaM      = D_UNITSYS_BASE_MOMENT;
		bRebaM      = D_UNITSYS_NONE;   
		
		dPHPThick   = D_UNITSYS_BASE_LENGTH;

		nApplyType  = D_UNITSYS_NONE;
		nApplyPile  = D_UNITSYS_NONE;
	}
	int nPileType;  
	int nPileArrange;
	int dPileAngle;

	int bAutoPIDS ;
	int dPileDist ;
	int dEdgeDist ;
	int bAutoPLSZ ;
					
	int nRebaSettl;
	int dRebaF    ;
	int dRebaN    ;
	int bRebaN    ;
	int dRebaM    ;
	int bRebaM    ;  

	int dPHPThick ;

	int nApplyType;
	int nApplyPile;
};

#define HASHSIZEPILE 11

// 구조계산서 입력정보 데이터
struct T_JFDR_RPOP_FONT
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

struct T_JFDR_RPOP_FONT_UDRD_D
{
	T_JFDR_RPOP_FONT data;
};

struct T_JFDR_RPOP_FONT_CH
{
	char      csFontName[80];
	int       nFontSize;
	BOOL      bBold;
	BOOL      bItalic;
	BOOL      bUnderLine;
	BOOL      bStrikeOut;
	COLORREF  rgbFontColor;

	void ConvertToString(T_JFDR_RPOP_FONT& rData)
	{
		ConvertCharStr(csFontName, rData.csFontName, sizeof(csFontName));
		rData.nFontSize=nFontSize;
		rData.bBold=bBold;
		rData.bItalic=bItalic;
		rData.bUnderLine=bUnderLine;
		rData.bStrikeOut=bStrikeOut;
		rData.rgbFontColor=rgbFontColor;
	}
	void ConvertToChar(T_JFDR_RPOP_FONT& rData)
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

struct T_JFDR_RPOP_FONT_UNIT
{
	T_JFDR_RPOP_FONT_UNIT()
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

struct T_JFDR_RPOP_PAPER  // 용지여백
{
	// 단위계 - mm (fixed)

	double dMarginTop;      // 여백 : 상, 하, 좌, 우

	double dMarginBot;
	double dMarginLeft;
	double dMarginRight;

	double dPaperSizeX;     // Paper Size
	double dPaperSizeY;
	
	void Initialize()
	{              
		dMarginTop   = 20.0;  
		dMarginBot   = 15.0;
		dMarginLeft  = 30.0;
		dMarginRight = 30.0;

		dPaperSizeX  = 210.0;
		dPaperSizeY  = 297.0;
	}
};

struct T_JFDR_RPOP_PAPER_UDRD_D
{
	T_JFDR_RPOP_PAPER data;
};

struct T_JFDR_RPOP_PAPER_UNIT
{
	T_JFDR_RPOP_PAPER_UNIT()
	{
		dMarginTop = D_UNITSYS_NONE;
		dMarginBot = D_UNITSYS_NONE;
		dMarginLeft = D_UNITSYS_NONE;
		dMarginRight = D_UNITSYS_NONE;
		dPaperSizeX = D_UNITSYS_NONE;
		dPaperSizeY = D_UNITSYS_NONE;
	}
	int dMarginTop;
	int dMarginBot;
	int dMarginLeft;
	int dMarginRight;
	int dPaperSizeX;
	int dPaperSizeY;
};

struct T_JFDR_RPOP
{
	// 설정
	T_JFDR_RPOP_FONT  TextFont;      // 텍스트 폰트
	T_JFDR_RPOP_FONT  ImageFont;     // 이미지 폰트
	T_JFDR_RPOP_PAPER Paper;         // 용지여백

	double dLineSpace ;              // 행간격 (텍스트 폰트에 대한 %)

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

	void Initialize()
	{
		TextFont.Initialize();
		ImageFont.Initialize();
		Paper.Initialize();

		dLineSpace  = 150.0;

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
	}
};

struct T_JFDR_RPOP_UDRD_D
{
	T_JFDR_RPOP data;
};

struct T_JFDR_RPOP_UNIT
{
	T_JFDR_RPOP_UNIT()
	{
		dLineSpace        = D_UNITSYS_NONE;
		nDivideHor_Input  = D_UNITSYS_NONE;
		nDivideVer_Input  = D_UNITSYS_NONE;
		nDivideHor_Output = D_UNITSYS_NONE;
		nDivideVer_Output = D_UNITSYS_NONE;
	}
	int dLineSpace ;
	int nDivideHor_Input;
	int nDivideVer_Input;
	int nDivideHor_Output;
	int nDivideVer_Output;
};

struct T_JFDR_RPOP_CH
{
	T_JFDR_RPOP_FONT_CH  TextFont; 
	T_JFDR_RPOP_FONT_CH  ImageFont;
	T_JFDR_RPOP_PAPER Paper;
	
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
	void ConvertToString(T_JFDR_RPOP& rData)
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
	}
	void ConvertToChar(T_JFDR_RPOP& rData)
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
	}
};

struct T_JFDR_PAGE1
{
	CString strProject;
	
	void Initialize()
	{
		strProject.Empty();
	}
};

struct T_JFDR_PAGE1_UDRD_D
{
	T_JFDR_PAGE1 data;
};

struct T_JFDR_PAGE1_UNIT
{
	T_JFDR_PAGE1_UNIT()
	{
		strProject = D_UNITSYS_NONE;
	}
	int strProject;
};

struct T_JFDR_PAGE1_CH
{
	char strProject[80];
	
	void ConvertToString(T_JFDR_PAGE1& rData)
	{
		ConvertCharStr(strProject, rData.strProject, sizeof(strProject));
	}
	void ConvertToChar(T_JFDR_PAGE1& rData)
	{
		ConvertStrChar(rData.strProject, strProject, sizeof(strProject));
	}
};

#define HASESIZEJFDR 1

#define T_JFDR_K unsigned int
struct T_JFDR_D
{
	T_JFDR_RPOP   PageOption; // 설정
	T_JFDR_PAGE1  Page1;      // 1.기본사항
	
	void Initialize()
	{
		PageOption.Initialize();
		Page1.Initialize();
	}
};

struct T_JFDR_D_CH
{
	T_JFDR_RPOP_CH      PageOption;
	T_JFDR_PAGE1_CH     Page1;
	
	void ConvertToString(T_JFDR_D& rData)
	{
		PageOption.ConvertToString(rData.PageOption);
		Page1.ConvertToString(rData.Page1);
	}
	void ConvertToChar(T_JFDR_D& rData)
	{
		PageOption.ConvertToChar(rData.PageOption);
		Page1.ConvertToChar(rData.Page1);
	}
}; 

struct T_JFDR_UDRD_D
{
	T_JFDR_D data;
};

struct T_JFDR_UDRD_D_CH
{
	T_JFDR_K key;
	T_JFDR_D_CH data;
};

// MNET:XXXX-CJJEONG-20120816 (2088)
// about iGen-Foundation Module Temp Function

#define HASHSIZEJFOP 1

#define T_JFOP_K unsigned int

struct T_HFOP_Detail
{
	T_STLD_K StldK     ;
	CString  strGruName;
	BOOL     bTopHorizF;
	double   dTopHorizF; //최상층의 수평력
	BOOL     bFndWeight;
	double   dFndWeight; //기초 부분의 중량
	BOOL     bFndPlatWt;
	double   dFndPlatWt; //기초 판의 중량
	BOOL     bAddedLoad;
	double   dAddedLoad; //추가 하중

	void T_HFOP_Detail::Initialize()
	{
		StldK      = 0;
		strGruName = _T("Default");
		bTopHorizF = FALSE;
		dTopHorizF = 0.0; //최상층의 수평력
		bFndWeight = FALSE;
		dFndWeight = 0.0; //기초 부분의 중량
		bFndPlatWt = FALSE;
		dFndPlatWt = 0.0; //기초 판의 중량
		bAddedLoad = FALSE;
		dAddedLoad = 0.0; //추가 하중
	}

	void T_HFOP_Detail::SetValueAll(double dVal)
	{
		//StldK      = 0;
		//strGruName = _T("Default");
		//bTopHorizF = FALSE;
		dTopHorizF = dVal; //최상층의 수평력
		//bFndWeight = FALSE;
		dFndWeight = dVal; //기초 부분의 중량
		//bFndPlatWt = FALSE;
		dFndPlatWt = dVal; //기초 판의 중량
		//bAddedLoad = FALSE;
		dAddedLoad = dVal; //추가 하중
	}
	
	T_HFOP_Detail() { Initialize(); };
	//T_HFOP_Detail& operator=(const T_HFOP_Detail& src);
};

struct T_HFOP_Detail_CH
{
	unsigned int StldK     ;
	char    strGruName[NAME_LENGTH];
	int     bTopHorizF;
	double  dTopHorizF; //최상층의 수평력
	int     bFndWeight;
	double  dFndWeight; //기초 부분의 중량
	int     bFndPlatWt;
	double  dFndPlatWt; //기초 판의 중량
	int     bAddedLoad;
	double  dAddedLoad; //추가 하중

	void ConvertToString(T_HFOP_Detail& rData)
	{
		rData.StldK    = StldK    ;
		ConvertCharStr(strGruName, rData.strGruName, sizeof(strGruName));
		rData.bTopHorizF    = bTopHorizF    ;
		rData.dTopHorizF    = dTopHorizF    ;
		rData.bFndWeight    = bFndWeight    ;
		rData.dFndWeight    = dFndWeight    ;
		rData.bFndPlatWt    = bFndPlatWt    ;
		rData.dFndPlatWt    = dFndPlatWt    ;
		rData.bAddedLoad    = bAddedLoad    ;
		rData.dAddedLoad    = dAddedLoad    ;  
	}
	
	void ConvertToChar(T_HFOP_Detail& rData)
	{
		StldK     = rData.StldK;
		ConvertStrChar(rData.strGruName, strGruName, sizeof(strGruName));
		bTopHorizF = rData.bTopHorizF;
		dTopHorizF = rData.dTopHorizF;
		bFndWeight = rData.bFndWeight;
		dFndWeight = rData.dFndWeight;
		bFndPlatWt = rData.bFndPlatWt;
		dFndPlatWt = rData.dFndPlatWt;
		bAddedLoad = rData.bAddedLoad;
		dAddedLoad = rData.dAddedLoad;    
	}
};

struct T_HFOP_Detail_UNIT
{
	T_HFOP_Detail_UNIT()
	{
		dTopHorizF = D_UNITSYS_BASE_WEIGHT;
		dFndWeight = D_UNITSYS_BASE_WEIGHT;
		dFndPlatWt = D_UNITSYS_BASE_WEIGHT;
		dAddedLoad = D_UNITSYS_BASE_WEIGHT;
	}
	int   dTopHorizF;
	int   dFndWeight;
	int   dFndPlatWt;
	int   dAddedLoad;
};

struct T_HFOP_Grp
{
	CString  strGrpName; //그룹 이름

	void Initialize()
	{
		strGrpName  = _T("Default");
	}
	
	T_HFOP_Grp() { Initialize(); };
};

struct T_HFOP_Grp_CH
{
	char strGrpName[NAME_LENGTH];

	void ConvertToString(T_HFOP_Grp& rData)
	{
		ConvertCharStr(strGrpName, rData.strGrpName, sizeof(strGrpName));
	}
	void ConvertToChar(T_HFOP_Grp& rData)
	{
		ConvertStrChar(rData.strGrpName, strGrpName, sizeof(strGrpName));
	}
};

struct T_HFOP_LC
{
	enum DIRC
	{
		GLOBAL_X = 1,
		GLOBAL_Y,
	};
	
	T_STLD_K StldK      ; //하중 케이스
	int      nDirection ; //방향

	void Initialize()
	{
		StldK       = 0;
		nDirection  = GLOBAL_X;
	}
	
	T_HFOP_LC() { Initialize(); };
};

struct T_HFOP_D //이 구조체.. 따로 DB로 빼내는 것이 좋을 것 같습니다.
{
	CArray<T_HFOP_LC, T_HFOP_LC&> arHflopLcD;
	CArray<T_HFOP_Grp, T_HFOP_Grp&> arHfopGrpD;
	CArray<T_HFOP_Detail, T_HFOP_Detail&> arHfopDetail;

	void T_HFOP_D::Initialize()
	{
		arHflopLcD.RemoveAll();
		arHfopGrpD.RemoveAll();
		arHfopDetail.RemoveAll();
	}
	
	T_HFOP_D() { Initialize(); };
	
	T_HFOP_D(T_HFOP_D& src) { *this = src; }
	
	T_HFOP_D& operator=(const T_HFOP_D& src)
	{
		arHflopLcD.Copy(src.arHflopLcD);
		arHfopGrpD.Copy(src.arHfopGrpD);
		arHfopDetail.Copy(src.arHfopDetail);
		
		return *this;
	}
	
	BOOL IsHfopStld(const T_STLD_K stldK, int* nIndex = NULL)
	{
		if(nIndex != NULL)
		{
			*nIndex = -1;
		}
		
		BOOL bExist = FALSE;
		int nHfopLcdSize = (int)arHflopLcD.GetSize();
		for(int i = 0; (i < nHfopLcdSize) && (bExist == FALSE) ; i++)
		{
			if(arHflopLcD.GetAt(i).StldK == stldK)
			{
				bExist = TRUE;
				*nIndex = i;
			}
		}
		return bExist;
	}
	
	BOOL GetHfopDeatilData(const T_STLD_K StldK, const CString strGrpName, T_HFOP_Detail* const pHfopDetail)
	{
		BOOL bRet = FALSE;
		int nDetailSize = (int)arHfopDetail.GetSize();
		
		T_HFOP_Detail HfopDetail; HfopDetail.Initialize();
		for( int i = 0; (i < nDetailSize) && (bRet == FALSE) ; i ++ )
		{
			HfopDetail = arHfopDetail.GetAt(i);
			if( (HfopDetail.StldK == StldK) && (HfopDetail.strGruName == strGrpName) )
			{
				bRet = TRUE;
			}
		}
		
		if(bRet == TRUE)
		{
			*pHfopDetail = HfopDetail;
		}
		return bRet;
	}
};

struct T_HFOP_D_CH
{
	CArray<T_HFOP_LC, T_HFOP_LC&> arHflopLcD;
	CArray<T_HFOP_Grp_CH, T_HFOP_Grp_CH&> arHfopGrpD;
	CArray<T_HFOP_Detail_CH, T_HFOP_Detail_CH&> arHfopDetail;

	void ConvertToString(T_HFOP_D& rData)
	{
		rData.arHflopLcD.Copy(arHflopLcD);
		int nHfopGrpDSize = (int)arHfopGrpD.GetSize();
		rData.arHfopGrpD.SetSize(nHfopGrpDSize);      
		for(int i = 0; i < nHfopGrpDSize; i++)
		{
			arHfopGrpD[i].ConvertToString(rData.arHfopGrpD[i]);
		}

		int nHforpDetailSize = (int)arHfopDetail.GetSize();
		rData.arHfopDetail.SetSize(nHforpDetailSize);
		for(int i = 0; i < nHforpDetailSize; i++ )
		{
			arHfopDetail[i].ConvertToString(rData.arHfopDetail[i]);
		}
	}
	
	void ConvertToChar(T_HFOP_D& rData)
	{
		arHflopLcD.Copy(rData.arHflopLcD);
		int nHfopGrpDSize = (int)rData.arHfopGrpD.GetSize();
		arHfopGrpD.SetSize(nHfopGrpDSize);
		for(int i = 0; i < nHfopGrpDSize; i++ )
		{
			arHfopGrpD[i].ConvertToChar(rData.arHfopGrpD[i]);
		}

		int nHfopDetailSize = (int)rData.arHfopDetail.GetSize();
		arHfopDetail.SetSize(nHfopDetailSize);
		for(int i = 0; i < nHfopDetailSize; i++ )
		{
			arHfopDetail[i].ConvertToChar(rData.arHfopDetail[i]);
		}
	}
};

#define  RAEBR_DIV_GRAD 3

struct T_PILE_REBA
{
	enum POS
	{
		TOP =0,
		MID,
		BOT,
	};

	CString strMainDia   ;  //주근 직경
	double  dMainPGRto   ;  //주근 pg
	CString strHoopDia   ;  //후프근 직경
	double  dHoopPWRto   ;  //후프근 pw

	void Initialize(POS pos)
	{
		strMainDia  = _T("D25") ;  
		if(POS::TOP == pos)
		{
			dMainPGRto  = 0.8;  
		}
		else
		{
			dMainPGRto  = 0.4;  
		}
		strHoopDia  = _T("D16") ;  
		dHoopPWRto  = 0.2;  
	}
};

struct T_PILE_REBA_CH
{
	char    strMainDia[NAME_LENGTH]   ;  //주근 직경
	double  dMainPGRto   ;  //주근 pg
	char    strHoopDia[NAME_LENGTH]   ;  //후프근 직경
	double  dHoopPWRto   ;  //후프근 pw

	void ConvertToString(T_PILE_REBA& rData)
	{
		ConvertCharStr(strMainDia, rData.strMainDia, sizeof(strMainDia));
		rData.dMainPGRto    = dMainPGRto    ;
		ConvertCharStr(strHoopDia, rData.strHoopDia, sizeof(strHoopDia));
		rData.dHoopPWRto    = dHoopPWRto    ;
	}

	void ConvertToChar(T_PILE_REBA& rData)
	{
		ConvertStrChar(rData.strMainDia, strMainDia, sizeof(strMainDia));
		dMainPGRto = rData.dMainPGRto;
		ConvertStrChar(rData.strHoopDia, strHoopDia, sizeof(strHoopDia));
		dHoopPWRto = rData.dHoopPWRto;
	}
};

struct T_JFOP_D
{
	enum NRANGE
	{
		UP_4D_DN_1D = 0,
		UP_1D_DN_1D,
		PILE_N_VALU,
	};

	enum CRIT
	{
		MLIT = 0, //국토교통성 고시
		JIS ,     //건축기초구조설계지침
		TOKY_A,   //동경 A
		TOKY_B,   //동경 B
		YOKO,     //요코하마
		OSKA,     //오사카
		ACCEPT,   //인정공법
		TOKY_X,    //동경: 기성콘크리트와 강관에서는 동경 A, B로 나누지 않는다.(동경 A, 동경 B)와는 다름
	};

	enum SPLY    //현장타설 말뚝 지지층
	{
		TOKYO = 0,   //동경자갈층, 토단층
		FN_SD,       //세사층 
		OTHER,       //그외
	};

	enum GRNL    // 지반 비선형 고려 방법 
	{
		L_TYPE = 0, //y >= 1cm
		S_TYPE,     //y >= 0.1cm
	};

	enum DFST //지지력 검토 기준
	{
		B_S_EACH = 0, //B, θ 각각 Local xz 와 Local yz 중 불리한 값
		LOCAL_XZ, //Local xz 기준
		LO_XZ_YZ, //Local xz와 Local yz 기준 지지력 중 불리한 값
	};

	enum COMT //공법
	{
		EARTH_DRI, //어스드릴
		BH_MINI_E, //BH 미니어스
		P_DRIVING, //항타 
		P_RECLAIM, //매립
	};

	enum CNMT //시공 방법
	{
		CNMT_MUDWT, //흙탕물 콘크리트
		CNMT_OTHER, //기타
	};

// 설계설정
	int     nDFCriterion  ; //CRIT : MLIT, JIS Only
	int     nDFStandart   ; //DFST
	int     nDFWaterLevel ; //0: 기초 저면 B(기초폭) 까지, 1:기초 저면 dDFWaterLevel 까지
	double  dDFWaterLevel ; 

	//현장 타설
	int     nPFCICriterion  ; //CRIT 
	int     nPFCISupport    ; // SPLY 말뚝 지지층
	int     nPFCIPMethod    ; //공법
	int     nPFCIPAverN     ; //NRANGE
	int     nPFCIPlacMtd    ; //CNMT::시공 방법

	//기성
	int     nPFDBCriterion  ; //CRIT 
	int     nPFDBSupport    ; // SPLY 말뚝 지지층
	int     nPFDBPMethod    ; //공법
	int     nPFDBAverN      ; //NRANGE

	//Pipe
	int     nPFPICriterion  ; //CRIT
	int     nPFPISupport    ; // SPLY 말뚝 지지층 
	int     nPFPIPMethod    ; //공법
	int     nPFPipeAverN  ; //NRANGE

	double  dConstError   ; //시공 오차
	int     nAddStress    ; //0: 지중보에 고려, 1: 기초 판에 고려
	
	T_HFOP_D HfopD;

	int     nANDivideNum;
	int     nANLoadStrp   ; //하중스텝 갯수
	int     nANMaxNum     ; //최대시도 횟수
	double  dANNorm       ; //변위 놈
	BOOL    bGrndNonLin   ; //지반 비선형 고려
	int     nGrndNonLin   ; //GRNL

	int     nReductionLoad; //횡방향 감소 계수, 0:자동계산, 1:사용자 정의
	//횡방향 감소 계수, BOOL 타입으로 바꾸고 자동계산 값과 사용자 정의 값을 대화창에서 보여주 것이 좋을 듯
	double  dReductionLoad;
	
	BOOL    bLiquefaction ; //액상화
	double  dRichter      ; //리히터 규모
	double  dHorAcceler   ; //설계용 수평 가속도
	double  dGrvAcceler   ; //중력 가속도
	//
	double  dLiquCalcDepth; //액상화 판정 계산 깊이
	BOOL    bChkWLUpside  ; //지하수위 상부 검토
	BOOL    bCohesLimit   ; //점성토 함유율 20%이상 판정 안함
	BOOL    bFindGrLimit  ; //세립분 함유율 35%이상 판정 안함

	BOOL    bReduceKH     ; //말둑 해석시 KH  저감

	BOOL    bNearReduce   ; //근입 저감에 의한 수평력 저감
	double  dReduceRatio  ; //저감률
	double  dHorSeis      ; //수평진도

	CString strCodeName;  //Code Name

	CString strPLATConcName;  //콘크리트 재질
	CString strPLATMainDia ;  //주철근 직경
	CArray<CString, CString> arPLATXdir; //0 : ~D16, 1 : D19~D25, 2 : D25~
	CArray<CString, CString> arPLATYdir; //0 : ~D16, 1 : D19~D25, 2 : D25~
	double  dPLATPtThick; //피복 두께 

	CString strPIPLConcName;  //콘크리트 재질
	CArray<CString, CString> arPIPLMain; //0 : ~D16, 1 : D19~D25, 2 : D25~
	CArray<CString, CString> arPIPLHoop; //0 : ~D16, 1 : D19~D25, 2 : D25~
	double  dPIPLPtThick; //피복 두께 

	T_PILE_REBA rbPileTop; //상부 말뚝
	T_PILE_REBA rbPileMid; //중간 말뚝
	T_PILE_REBA rbPileBtm; //하부 말뚝

	void Initialize()
	{
		HfopD.Initialize();

		nDFCriterion = MLIT;
		nDFStandart  = LOCAL_XZ;
		nDFWaterLevel= 0;
		dDFWaterLevel= 1;
		
		nPFCICriterion = MLIT;
		nPFCISupport   = TOKYO;
		nPFCIPMethod   = EARTH_DRI;
		nPFCIPAverN    = UP_1D_DN_1D;
		nPFCIPlacMtd   = CNMT_MUDWT;
		
		nPFDBCriterion = MLIT;
		nPFDBSupport   = TOKYO;
		nPFDBPMethod   = P_DRIVING;
		nPFDBAverN     = UP_4D_DN_1D;
		
		nPFPICriterion = MLIT;
		nPFPISupport   = TOKYO;
		nPFPIPMethod   = P_DRIVING;
		nPFPipeAverN   = UP_4D_DN_1D;

		dConstError  = 0.1;
		nPFPISupport = TOKYO;
		nAddStress   = 0;
		bNearReduce  = TRUE;
		dReduceRatio = 0.9;
		dHorSeis     = 1.0;

		HfopD.Initialize();

		nANDivideNum = 30;
		nANLoadStrp  = 1;
		nANMaxNum    = 30;  
		dANNorm      = 0.001;
		bGrndNonLin  = TRUE;
		nGrndNonLin  = L_TYPE;
		nReductionLoad = 1;
		dReductionLoad = 1;
		bLiquefaction = TRUE;
		dRichter      = 7.5;
		dHorAcceler   = 2.0;
		dGrvAcceler   = 9.8;
		dLiquCalcDepth = 20;
		bChkWLUpside  = TRUE;
		bCohesLimit   = TRUE;
		bFindGrLimit  = TRUE;
		bReduceKH     = TRUE;
		
		strCodeName      = _T("JIS(RC)");
		strPLATConcName  = _T("Fc24"); 
		strPLATMainDia   = _T("D22");
		arPLATXdir.RemoveAll();
		arPLATXdir.Add(_T("SD295A"));
		arPLATXdir.Add(_T("SD345") );
		arPLATXdir.Add(_T("SD390") );
		arPLATXdir.FreeExtra();
		
		arPLATYdir.RemoveAll();
		arPLATYdir.Add(_T("SD295A"));
		arPLATYdir.Add(_T("SD345") );
		arPLATYdir.Add(_T("SD390") );
		arPLATYdir.FreeExtra();
		
		dPLATPtThick  = 0.1; 

		strPIPLConcName  = _T("Fc24"); 
		arPIPLMain.RemoveAll();
		arPIPLMain.Add(_T("SD295A"));
		arPIPLMain.Add(_T("SD345") );
		arPIPLMain.Add(_T("SD390") );
		arPIPLMain.FreeExtra();
		
		arPIPLHoop.RemoveAll();
		arPIPLHoop.Add(_T("SD295A"));
		arPIPLHoop.Add(_T("SD345") );
		arPIPLHoop.Add(_T("SD390") );
		arPIPLHoop.FreeExtra();
		
		dPIPLPtThick  = 0.1; 

		rbPileTop.Initialize(T_PILE_REBA::TOP); 
		rbPileMid.Initialize(T_PILE_REBA::MID); 
		rbPileBtm.Initialize(T_PILE_REBA::BOT); 
	
	}
	T_JFOP_D() { Initialize(); };
	T_JFOP_D(T_JFOP_D& src) { *this = src; }
	T_JFOP_D& operator=(const T_JFOP_D& src)
	{
		nDFCriterion      = src.nDFCriterion     ;
		nDFStandart       = src.nDFStandart      ;
		nDFWaterLevel     = src.nDFWaterLevel    ;
		dDFWaterLevel     = src.dDFWaterLevel    ;
		
		nPFCICriterion     = src.nPFCICriterion    ;
		nPFCISupport       = src.nPFCISupport      ;
		nPFCIPMethod       = src.nPFCIPMethod      ;
		nPFCIPAverN        = src.nPFCIPAverN       ;
		nPFCIPlacMtd       = src.nPFCIPlacMtd      ;

		nPFDBCriterion     = src.nPFDBCriterion    ;
		nPFDBSupport       = src.nPFDBSupport      ;
		nPFDBPMethod       = src.nPFDBPMethod      ;
		nPFDBAverN         = src.nPFDBAverN        ;
		
		nPFPICriterion     = src.nPFPICriterion    ;
		nPFPISupport       = src.nPFPISupport      ;
		nPFPIPMethod       = src.nPFPIPMethod      ;
		nPFPipeAverN       = src.nPFPipeAverN      ;

		dConstError       = src.dConstError      ;
		nPFPISupport      = src.nPFPISupport     ;
		nAddStress        = src.nAddStress       ;
		bNearReduce       = src.bNearReduce      ;
		dReduceRatio      = src.dReduceRatio     ;
		dHorSeis          = src.dHorSeis         ;
		HfopD             = src.HfopD;

		nANDivideNum      = src.nANDivideNum     ;
		nANLoadStrp       = src.nANLoadStrp      ;
		nANMaxNum         = src.nANMaxNum        ;
		dANNorm           = src.dANNorm          ;
		bGrndNonLin       = src.bGrndNonLin      ;
		nGrndNonLin       = src.nGrndNonLin      ;  
		nReductionLoad    = src.nReductionLoad   ;
		dReductionLoad    = src.dReductionLoad   ;
		bLiquefaction     = src.bLiquefaction    ;
		dRichter          = src.dRichter         ;
		dHorAcceler       = src.dHorAcceler      ;
		dGrvAcceler       = src.dGrvAcceler      ;
		dLiquCalcDepth     = src.dLiquCalcDepth    ;
		bChkWLUpside       = src.bChkWLUpside      ;
		bCohesLimit        = src.bCohesLimit       ;
		bFindGrLimit       = src.bFindGrLimit      ;
		bReduceKH         = src.bReduceKH        ;

		strCodeName       = src.strCodeName;  
		
		strPLATConcName   = src.strPLATConcName;
		strPLATMainDia    = src.strPLATMainDia;
		arPLATXdir.Copy(src.arPLATXdir);
		arPLATYdir.Copy(src.arPLATYdir);
		dPLATPtThick = src.dPLATPtThick;
		
		strPIPLConcName = src.strPIPLConcName;
		arPIPLMain.Copy(src.arPIPLMain);
		arPIPLHoop.Copy(src.arPIPLHoop);
		dPIPLPtThick = src.dPIPLPtThick; 
		
		rbPileTop = src.rbPileTop;
		rbPileMid = src.rbPileMid;
		rbPileBtm = src.rbPileBtm;

		return *this;
	}	
};

struct T_JFOP_UNIT
{
	T_JFOP_UNIT()
	{
		dDFWaterLevel  = D_UNITSYS_BASE_LENGTH;
		dConstError    = D_UNITSYS_BASE_LENGTH;
		nANDivideNum   = D_UNITSYS_NONE       ;
		nANLoadStrp    = D_UNITSYS_NONE       ;
		nANMaxNum      = D_UNITSYS_NONE       ;
		dANNorm        = D_UNITSYS_NONE       ;
		dReductionLoad = D_UNITSYS_NONE     ;
		dRichter       = D_UNITSYS_NONE;
		dHorAcceler    = D_UNITSYS_BASE_ACCELERATION;
		dGrvAcceler    = D_UNITSYS_BASE_GRAVITY;
		dLiquCalcDepth = D_UNITSYS_BASE_LENGTH;
		dReduceRatio   = D_UNITSYS_NONE;
		dHorSeis       = D_UNITSYS_NONE;
		dPLATPtThick   = D_UNITSYS_BASE_LENGTH;
		dPIPLPtThick   = D_UNITSYS_BASE_LENGTH;
	}
	int    dDFWaterLevel ;
	int    dConstError   ;
	int    nANDivideNum  ;
	int    nANLoadStrp   ;
	int    nANMaxNum     ;
	int    dANNorm       ;
	int    dReductionLoad;
	int    dRichter      ;
	int    dHorAcceler   ;
	int    dGrvAcceler   ;
	int    dLiquCalcDepth;
	int    dReduceRatio  ;
	int    dHorSeis      ;
	int    dPLATPtThick  ;
	int    dPIPLPtThick  ;
};

struct T_JFOP_D_CH
{
	// 설계설정
	int     nDFCriterion  ; //MLIT, JIS Only
	int     nDFStandart   ;
	int     nDFWaterLevel ;  //0: 기초 저면 B(기초폭) 까지, 1:기초 저면 dDFWaterLevel 까지
	double  dDFWaterLevel ;
	int     nPFCICriterion;
	int     nPFCISupport  ;
	int     nPFCIPMethod  ;
	int     nPFCIPAverN   ;
	int     nPFCIPlacMtd  ;
	int     nPFDBCriterion;
	int     nPFDBSupport  ;
	int     nPFDBPMethod  ;
	int     nPFDBAverN    ;
	int     nPFPICriterion;
	int     nPFPISupport  ;
	int     nPFPIPMethod  ;
	int     nPFPipeAverN  ;  
	double  dConstError   ;    //시공 오차
	int     nAddStress    ;     //0: 지중보에 고려, 1: 기초 판에 고려

	//T_HFOP_D_CH HfopD     ;

	int     nANDivideNum  ;
	int     nANLoadStrp   ;   //하중스텝 갯수
	int     nANMaxNum     ;     //최대시도 횟수
	double  dANNorm       ;       //변위 놈
	int     bGrndNonLin   ; //지반 비선형 고려
	int     nGrndNonLin   ; //GRNL
	int     nReductionLoad;//횡방향 감소 계수, 0:자동계산, 1:사용자 정의
	double  dReductionLoad;
	int     bLiquefaction ; //액상화
	double  dRichter      ; //리히터 규모
	double  dHorAcceler   ; //설계용 수평 가속도
	double  dGrvAcceler   ; //중력 가속도
	double  dLiquCalcDepth;
	int     bChkWLUpside  ;
	int     bCohesLimit   ;
	int     bFindGrLimit  ;
	int     bReduceKH     ; //말둑 해석시 KH  저감
	int     bNearReduce   ; //근입 저감에 의한 수평력 저감
	double  dReduceRatio  ; //저감률
	double  dHorSeis      ; //수평진도
	
	char strCodeName[NAME_LENGTH];  //Code Name
	char strPLATConcName[NAME_LENGTH];  //콘크리트 재질
	char strPLATMainDia[NAME_LENGTH] ;  //주철근 직경
	char arPLATXdir[3][NAME_LENGTH]; //0 : ~D16, 1 : D19~D25, 2 : D25~
	char arPLATYdir[3][NAME_LENGTH]; //0 : ~D16, 1 : D19~D25, 2 : D25~
	double  dPLATPtThick; //피복 두께 
	char strPIPLConcName[NAME_LENGTH];  //콘크리트 재질
	char arPIPLMain[3][NAME_LENGTH]; //0 : ~D16, 1 : D19~D25, 2 : D25~
	char arPIPLHoop[3][NAME_LENGTH]; //0 : ~D16, 1 : D19~D25, 2 : D25~
	double  dPIPLPtThick; //피복 두께 
	
	T_PILE_REBA_CH rbPileTop; //상부 말뚝
	T_PILE_REBA_CH rbPileMid; //중간 말뚝
	T_PILE_REBA_CH rbPileBtm; //하부 말뚝

	void ConvertToString(T_JFOP_D& rData)
	{
		rData.nDFCriterion      = nDFCriterion      ;
		rData.nDFStandart       = nDFStandart       ;
		rData.nDFWaterLevel     = nDFWaterLevel     ;
		rData.dDFWaterLevel     = dDFWaterLevel     ;
		rData.nPFCICriterion    = nPFCICriterion    ;
		rData.nPFCISupport      = nPFCISupport      ;
		rData.nPFCIPMethod      = nPFCIPMethod      ;
		rData.nPFCIPAverN       = nPFCIPAverN       ;
		rData.nPFCIPlacMtd      = nPFCIPlacMtd      ;
		rData.nPFDBCriterion    = nPFDBCriterion    ;
		rData.nPFDBSupport      = nPFDBSupport      ;
		rData.nPFDBPMethod      = nPFDBPMethod      ;
		rData.nPFDBAverN        = nPFDBAverN        ;
		rData.nPFPICriterion    = nPFPICriterion    ;
		rData.nPFPISupport      = nPFPISupport      ;
		rData.nPFPIPMethod      = nPFPIPMethod      ;
		rData.nPFPipeAverN      = nPFPipeAverN      ;
		rData.dConstError       = dConstError       ;
		rData.nAddStress        = nAddStress        ;
		
		rData.nANDivideNum      = nANDivideNum      ;
		rData.nANLoadStrp       = nANLoadStrp       ;
		rData.nANMaxNum         = nANMaxNum         ;
		rData.dANNorm           = dANNorm           ;
		rData.bGrndNonLin       = bGrndNonLin       ;
		rData.nGrndNonLin       = nGrndNonLin       ;
		rData.nReductionLoad    = nReductionLoad    ;
		rData.dReductionLoad    = dReductionLoad    ;
		rData.bLiquefaction     = bLiquefaction     ;
		rData.dRichter          = dRichter          ;
		rData.dHorAcceler       = dHorAcceler       ;
		rData.dGrvAcceler       = dGrvAcceler       ;
		rData.dLiquCalcDepth    = dLiquCalcDepth    ;
		rData.bChkWLUpside      = bChkWLUpside      ;
		rData.bCohesLimit       = bCohesLimit       ;
		rData.bFindGrLimit      = bFindGrLimit      ;
		rData.bReduceKH         = bReduceKH         ;
		rData.bNearReduce       = bNearReduce       ;
		rData.dReduceRatio      = dReduceRatio      ;
		rData.dHorSeis          = dHorSeis          ;

		ConvertCharStr(strCodeName, rData.strCodeName, sizeof(strCodeName));
		ConvertCharStr(strPLATConcName, rData.strPLATConcName, sizeof(strPLATConcName));
		ConvertCharStr(strPLATMainDia, rData.strPLATMainDia, sizeof(strPLATMainDia));

		ConvertCharStr(arPLATXdir[0], rData.arPLATXdir[0], sizeof(arPLATXdir[0]));
		ConvertCharStr(arPLATXdir[1], rData.arPLATXdir[1], sizeof(arPLATXdir[1]));
		ConvertCharStr(arPLATXdir[2], rData.arPLATXdir[2], sizeof(arPLATXdir[2]));

		ConvertCharStr(arPLATYdir[0], rData.arPLATYdir[0], sizeof(arPLATYdir[0]));
		ConvertCharStr(arPLATYdir[1], rData.arPLATYdir[1], sizeof(arPLATYdir[1]));
		ConvertCharStr(arPLATYdir[2], rData.arPLATYdir[2], sizeof(arPLATYdir[2]));
		rData.dPLATPtThick          = dPLATPtThick          ;

		ConvertCharStr(strPIPLConcName, rData.strPIPLConcName, sizeof(strPIPLConcName));
		ConvertCharStr(arPIPLMain[0], rData.arPIPLMain[0], sizeof(arPIPLMain[0]));
		ConvertCharStr(arPIPLMain[1], rData.arPIPLMain[1], sizeof(arPIPLMain[1]));
		ConvertCharStr(arPIPLMain[2], rData.arPIPLMain[2], sizeof(arPIPLMain[2]));
		
		ConvertCharStr(arPIPLHoop[0], rData.arPIPLHoop[0], sizeof(arPIPLHoop[0]));
		ConvertCharStr(arPIPLHoop[1], rData.arPIPLHoop[1], sizeof(arPIPLHoop[1]));
		ConvertCharStr(arPIPLHoop[2], rData.arPIPLHoop[2], sizeof(arPIPLHoop[2]));
		rData.dPIPLPtThick          = dPIPLPtThick          ;

		rbPileTop.ConvertToString(rData.rbPileTop);
		rbPileMid.ConvertToString(rData.rbPileMid);
		rbPileBtm.ConvertToString(rData.rbPileBtm);
	}

	void ConvertToChar(T_JFOP_D& rData)
	{
		nDFCriterion      = rData.nDFCriterion      ;
		nDFStandart       = rData.nDFStandart       ;
		nDFWaterLevel     = rData.nDFWaterLevel     ;
		dDFWaterLevel     = rData.dDFWaterLevel     ;
		nPFCICriterion    = rData.nPFCICriterion    ;
		nPFCISupport      = rData.nPFCISupport      ;
		nPFCIPMethod      = rData.nPFCIPMethod      ;
		nPFCIPAverN       = rData.nPFCIPAverN       ;
		nPFCIPlacMtd      = rData.nPFCIPlacMtd      ;
		nPFDBCriterion    = rData.nPFDBCriterion    ;
		nPFDBSupport      = rData.nPFDBSupport      ;
		nPFDBPMethod      = rData.nPFDBPMethod      ;
		nPFDBAverN        = rData.nPFDBAverN        ;
		nPFPICriterion    = rData.nPFPICriterion    ;
		nPFPISupport      = rData.nPFPISupport      ;
		nPFPIPMethod      = rData.nPFPIPMethod      ;
		nPFPipeAverN      = rData.nPFPipeAverN      ;
		dConstError       = rData.dConstError       ;
		nAddStress        = rData.nAddStress        ;

		nANDivideNum      = rData.nANDivideNum      ;
		nANLoadStrp       = rData.nANLoadStrp       ;
		nANMaxNum         = rData.nANMaxNum         ;
		dANNorm           = rData.dANNorm           ;
		bGrndNonLin       = rData.bGrndNonLin       ;
		nGrndNonLin       = rData.nGrndNonLin       ;
		nReductionLoad    = rData.nReductionLoad    ;
		dReductionLoad    = rData.dReductionLoad    ;
		bLiquefaction     = rData.bLiquefaction     ;
		dRichter          = rData.dRichter          ;
		dHorAcceler       = rData.dHorAcceler       ;
		dGrvAcceler       = rData.dGrvAcceler       ;
		dLiquCalcDepth    = rData.dLiquCalcDepth    ;
		bChkWLUpside      = rData.bChkWLUpside      ;
		bCohesLimit       = rData.bCohesLimit       ;
		bFindGrLimit      = rData.bFindGrLimit      ;
		bReduceKH         = rData.bReduceKH         ;
		bNearReduce       = rData.bNearReduce       ;
		dReduceRatio      = rData.dReduceRatio      ;
		dHorSeis          = rData.dHorSeis          ;

		ConvertStrChar(rData.strCodeName, strCodeName, sizeof(strCodeName));
		ConvertStrChar(rData.strPLATConcName, strPLATConcName, sizeof(strPLATConcName));
		ConvertStrChar(rData.strPLATMainDia, strPLATMainDia, sizeof(strPLATMainDia));

		ConvertStrChar(rData.arPLATXdir[0], arPLATXdir[0], sizeof(arPLATXdir[0]));
		ConvertStrChar(rData.arPLATXdir[1], arPLATXdir[1], sizeof(arPLATXdir[1]));
		ConvertStrChar(rData.arPLATXdir[2], arPLATXdir[2], sizeof(arPLATXdir[2]));
		
		ConvertStrChar(rData.arPLATYdir[0], arPLATYdir[0], sizeof(arPLATYdir[0]));
		ConvertStrChar(rData.arPLATYdir[1], arPLATYdir[1], sizeof(arPLATYdir[1]));
		ConvertStrChar(rData.arPLATYdir[2], arPLATYdir[2], sizeof(arPLATYdir[2]));

		dPLATPtThick = rData.dPLATPtThick;

		ConvertStrChar(rData.strPIPLConcName, strPIPLConcName, sizeof(strPIPLConcName));
		
		ConvertStrChar(rData.arPIPLMain[0], arPIPLMain[0], sizeof(arPIPLMain[0]));
		ConvertStrChar(rData.arPIPLMain[1], arPIPLMain[1], sizeof(arPIPLMain[1]));
		ConvertStrChar(rData.arPIPLMain[2], arPIPLMain[2], sizeof(arPIPLMain[2]));
		
		ConvertStrChar(rData.arPIPLHoop[0], arPIPLHoop[0], sizeof(arPIPLHoop[0]));
		ConvertStrChar(rData.arPIPLHoop[1], arPIPLHoop[1], sizeof(arPIPLHoop[1]));
		ConvertStrChar(rData.arPIPLHoop[2], arPIPLHoop[2], sizeof(arPIPLHoop[2]));

		dPIPLPtThick = rData.dPIPLPtThick;

		rbPileTop.ConvertToChar(rData.rbPileTop);
		rbPileMid.ConvertToChar(rData.rbPileMid);
		rbPileBtm.ConvertToChar(rData.rbPileBtm);
	}
};


struct T_JFOP_UDRD_D
{
	T_JFOP_D data;
};

//////////////////////////////////////////////////////////////////////////


#pragma pack()

#endif
