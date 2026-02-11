#ifndef __DB_ST_DT_STAG_H__
#define __DB_ST_DT_STAG_H__

#define MIDAS_SDK

#include "DB_ST_DT_TDMF.h"
#include "DB_ST_DT_TDNT.h"
#include "DB_ST_DT_ANAL.h"

#pragma pack(push, 1)

/// stag ///
#define T_STAG_K unsigned int
struct T_STAG_AELEM_BASE
{
	T_GRUP_K GrupKey;
	double   dAge;      // Stage 시작 당시의 나이(단위:day)
	void Initialize()
	{
		GrupKey = 0;
		dAge = 0.0;
	}
};
struct T_STAG_DELEM_BASE
{
	T_GRUP_K GrupKey;
	double   dRedistribution;
	void Initialize()
	{
		GrupKey = 0;
		dRedistribution = 0.0;
	}
};
struct T_STAG_BNDR_BASE
{
	T_BNGR_K BngrKey;
	int      nPosition; // 1:Original Position, 2:Deformed Position
	void Initialize()
	{
		BngrKey = 0;
		nPosition = 2;
	}
};
struct T_STAG_LOAD_BASE
{
	T_LDGR_K LdgrKey;
	double   dDay;    // 하중이 재하되는 시점 (0.0 First, -1.0:Last, etc:User-Step)
	void Initialize()
	{
		LdgrKey = 0;
		dDay = 0.0;
	}
};

struct T_STAG_D
{
	CString StageName;
	double  dDuration;
	BOOL    bSaveResultStage;
	BOOL    bSaveResultStep;
	CArray<double, double> aUserDefStep;
	CArray<T_STAG_AELEM_BASE, T_STAG_AELEM_BASE&> aActiveElem;
	CArray<T_STAG_DELEM_BASE, T_STAG_DELEM_BASE&> aDeactiveElem;
	CArray<T_STAG_BNDR_BASE, T_STAG_BNDR_BASE&> aActiveBndr;
	CArray<T_BNGR_K, T_BNGR_K> aDeactiveBndr;
	CArray<T_STAG_LOAD_BASE, T_STAG_LOAD_BASE&> aActiveLoad;
	CArray<T_STAG_LOAD_BASE, T_STAG_LOAD_BASE&> aDeactiveLoad;
	BOOL bIncreStep;
	int  nIncreStep;
	unsigned int nStageId;
	void Initialize()
	{
		StageName.Empty();
		dDuration = 0.0;
		bSaveResultStage = TRUE;
		bSaveResultStep = FALSE;
		aUserDefStep.RemoveAll();
		aActiveElem.RemoveAll();
		aDeactiveElem.RemoveAll();
		aActiveBndr.RemoveAll();
		aDeactiveBndr.RemoveAll();
		aActiveLoad.RemoveAll();
		aDeactiveLoad.RemoveAll();
		bIncreStep = FALSE;
		nIncreStep = 5;
		nStageId=0;
	}
	T_STAG_D()
	{
	}
	T_STAG_D(T_STAG_D& src)
	{
		*this = src;
	}
	T_STAG_D& operator=(T_STAG_D& src)
	{
		StageName = src.StageName;
		dDuration = src.dDuration;
		bSaveResultStage = src.bSaveResultStage;
		bSaveResultStep = src.bSaveResultStep;
		aUserDefStep.Copy(src.aUserDefStep);
		aActiveElem.Copy(src.aActiveElem);
		aDeactiveElem.Copy(src.aDeactiveElem);
		aActiveBndr.Copy(src.aActiveBndr);
		aDeactiveBndr.Copy(src.aDeactiveBndr);
		aActiveLoad.Copy(src.aActiveLoad);
		aDeactiveLoad.Copy(src.aDeactiveLoad);
		bIncreStep = src.bIncreStep;
		nIncreStep = src.nIncreStep;
		nStageId = src.nStageId;
		return *this;
	}
};
struct T_STAG_D_CH
{
	char StageName[80];
	double dDuration;
	BOOL bSaveResultStage;
	BOOL bSaveResultStep;
	CArray<double, double> aUserDefStep;
	CArray<T_STAG_AELEM_BASE, T_STAG_AELEM_BASE&> aActiveElem;
	CArray<T_STAG_DELEM_BASE, T_STAG_DELEM_BASE&> aDeactiveElem;
	CArray<T_STAG_BNDR_BASE, T_STAG_BNDR_BASE&> aActiveBndr;
	CArray<T_BNGR_K, T_BNGR_K> aDeactiveBndr;
	CArray<T_STAG_LOAD_BASE, T_STAG_LOAD_BASE&> aActiveLoad;
	CArray<T_STAG_LOAD_BASE, T_STAG_LOAD_BASE&> aDeactiveLoad;
	BOOL bIncreStep;
	int  nIncreStep;
	unsigned int nStageId;

	void ConvertToString(T_STAG_D& rData)
	{
		ConvertCharStr(StageName, rData.StageName, sizeof(StageName));
		rData.dDuration = dDuration;
		rData.bSaveResultStage = bSaveResultStage;
		rData.bSaveResultStep = bSaveResultStep;
		rData.aUserDefStep.Copy(aUserDefStep);
		rData.aActiveElem.Copy(aActiveElem);
		rData.aDeactiveElem.Copy(aDeactiveElem);
		rData.aActiveBndr.Copy(aActiveBndr);
		rData.aDeactiveBndr.Copy(aDeactiveBndr);
		rData.aActiveLoad.Copy(aActiveLoad);
		rData.aDeactiveLoad.Copy(aDeactiveLoad);
		rData.bIncreStep = bIncreStep;
		rData.nIncreStep = nIncreStep;
		rData.nStageId = nStageId;
	}
	void ConvertToChar(T_STAG_D& rData)
	{
		ConvertStrChar(rData.StageName, StageName, sizeof(StageName));
		dDuration = rData.dDuration;
		bSaveResultStage = rData.bSaveResultStage;
		bSaveResultStep = rData.bSaveResultStep;
		aUserDefStep.Copy(rData.aUserDefStep);
		aActiveElem.Copy(rData.aActiveElem);
		aDeactiveElem.Copy(rData.aDeactiveElem);
		aActiveBndr.Copy(rData.aActiveBndr);
		aDeactiveBndr.Copy(rData.aDeactiveBndr);
		aActiveLoad.Copy(rData.aActiveLoad);
		aDeactiveLoad.Copy(rData.aDeactiveLoad);
		bIncreStep = rData.bIncreStep;
		nIncreStep = rData.nIncreStep;
		nStageId = rData.nStageId;
	}
};
struct T_STAG_UDRD_D
{
	T_STAG_K key;
	T_STAG_D data;
};
struct T_STAG_ITEM_UDRD_D
{
	T_STAG_K key;
	T_STAG_AELEM_BASE ActiveElem;
	T_STAG_DELEM_BASE DeactiveElem;
	T_STAG_BNDR_BASE ActiveBndr;
	T_BNGR_K DeactiveBndr;
	T_STAG_LOAD_BASE ActiveLoad;
	T_STAG_LOAD_BASE DeactiveLoad;
};
struct T_STAG_UDRD_D_CH
{
	T_STAG_K key;
	T_STAG_D_CH data;
};
struct T_STAG_UDRD_D_CH_RW_865
{
	T_STAG_K key;
	char StageName[80];
	double dDuration;
	BOOL bSaveResultStage;
	BOOL bSaveResultStep;
	int  nCountUserDefStep;
	int  nCountActiveElem;
	int  nCountDeactiveElem;
	int  nCountActiveBndr;
	int  nCountDeactiveBndr;
	int  nCountActiveLoad;
	int  nCountDeactiveLoad;
	unsigned int nStageId;
};
struct T_STAG_UDRD_D_CH_RW
{
	T_STAG_K key;
	char StageName[80];
	double dDuration;
	BOOL bSaveResultStage;
	BOOL bSaveResultStep;
	int  nCountUserDefStep;
	int  nCountActiveElem;
	int  nCountDeactiveElem;
	int  nCountActiveBndr;
	int  nCountDeactiveBndr;
	int  nCountActiveLoad;
	int  nCountDeactiveLoad;
	BOOL bIncreStep;
	int  nIncreStep;
	unsigned int nStageId;	

	void GetStag(T_STAG_K& rKey, T_STAG_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.StageName, StageName, sizeof(StageName));
		rData.dDuration = dDuration;
		rData.bSaveResultStage = bSaveResultStage;
		rData.bSaveResultStep = bSaveResultStep;
		rData.bIncreStep = bIncreStep;
		rData.nIncreStep = nIncreStep;
		rData.nStageId = nStageId;
	}
	void SetStag(const T_STAG_K Key, const T_STAG_D_CH& rData)
	{
		key = Key;
		memcpy(StageName, rData.StageName, sizeof(StageName));
		dDuration = rData.dDuration;
		bSaveResultStage = rData.bSaveResultStage;
		bSaveResultStep = rData.bSaveResultStep;
		nCountUserDefStep = (int)rData.aUserDefStep.GetSize();
		nCountActiveElem = (int)rData.aActiveElem.GetSize();
		nCountDeactiveElem = (int)rData.aDeactiveElem.GetSize();
		nCountActiveBndr = (int)rData.aActiveBndr.GetSize();
		nCountDeactiveBndr = (int)rData.aDeactiveBndr.GetSize();
		nCountActiveLoad = (int)rData.aActiveLoad.GetSize();
		nCountDeactiveLoad = (int)rData.aDeactiveLoad.GetSize();
		bIncreStep = rData.bIncreStep;
		nIncreStep = rData.nIncreStep;
		nStageId = rData.nStageId;
	}
	void Convert865(T_STAG_UDRD_D_CH_RW_865& rData)
	{
		key=rData.key;
		memcpy(StageName, rData.StageName, sizeof(rData.StageName));
		dDuration = rData.dDuration;
		bSaveResultStage = rData.bSaveResultStage;
		bSaveResultStep = rData.bSaveResultStep;
		nCountUserDefStep = rData.nCountUserDefStep;
		nCountActiveElem = rData.nCountActiveElem;
		nCountDeactiveElem = rData.nCountDeactiveElem;
		nCountActiveBndr = rData.nCountActiveBndr;
		nCountDeactiveBndr = rData.nCountDeactiveBndr;
		nCountActiveLoad = rData.nCountActiveLoad; 
		nCountDeactiveLoad=rData.nCountDeactiveLoad;
		bIncreStep = FALSE;
		nIncreStep = 5;
		nStageId = rData.nStageId;
	}
};
struct T_STAG_UNIT
{
	T_STAG_UNIT()
	{
		dDuration = D_UNITSYS_BASE_DAY;
		dActiveElemAge = D_UNITSYS_BASE_DAY;
		aUserDefStep = D_UNITSYS_BASE_DAY;
	}
	int dDuration;
	int dActiveElemAge;
	int aUserDefStep;
};

struct T_STAG_STEP_INFO_BASE
{
	double dDay;
	int nStepType;  // 0:Time Step, 1:하중에 의한 Step
	CArray<T_LDGR_K, T_LDGR_K> aActiveLdgrKey;
	CArray<T_LDGR_K, T_LDGR_K> aDeactiveLdgrKey;
	T_STAG_STEP_INFO_BASE() {}
	T_STAG_STEP_INFO_BASE(const T_STAG_STEP_INFO_BASE& src) { *this = src; }
	T_STAG_STEP_INFO_BASE& operator=(const T_STAG_STEP_INFO_BASE& src)
	{
		dDay = src.dDay;
		nStepType = src.nStepType;
		aActiveLdgrKey.Copy(src.aActiveLdgrKey);
		aDeactiveLdgrKey.Copy(src.aDeactiveLdgrKey);
		return *this;
	}
	void Initialize()
	{
		dDay = 0.0;
		nStepType = 0;
		aActiveLdgrKey.RemoveAll();
		aDeactiveLdgrKey.RemoveAll();
	}
};

struct T_STAG_STEP_INFO
{
	T_STAG_K  StagKey;
	double    dDuration;
	double    dDurationInfo; //dDuration정보이용해서 First, Last 구분에 사용위함. (dDurationInfo=0.0은 기존 dDuration=0.0과 같음)
	BOOL      bSaveResultStage;
	BOOL      bSaveResultStep;
	BOOL      bIncreStep;
	int       nIncreStep;
	CArray<T_STAG_STEP_INFO_BASE, T_STAG_STEP_INFO_BASE&> aInfo;
	T_STAG_STEP_INFO(){}
	T_STAG_STEP_INFO(const T_STAG_STEP_INFO& src) { *this = src; }
	T_STAG_STEP_INFO& operator=(const T_STAG_STEP_INFO& src)
	{
		StagKey = src.StagKey;
		dDuration = src.dDuration;
		dDurationInfo = src.dDurationInfo;
		bSaveResultStage = src.bSaveResultStage;
		bSaveResultStep = src.bSaveResultStep;
		aInfo.Copy(src.aInfo);
		bIncreStep = src.bIncreStep;
		nIncreStep = src.nIncreStep;
		return *this;
	}
	void Initialize()
	{
		StagKey = 0;
		dDuration = 0.0;
		dDurationInfo = 0.0;
		bSaveResultStage = FALSE;
		bSaveResultStep = FALSE;
		bIncreStep = FALSE;
		nIncreStep = 5;
		aInfo.RemoveAll();
	}
};

struct T_STAG_INFO_FOR_POST
{
	CString csStageName;
	CString csStepName;
	T_STAG_K StagKey;
	int nStep;      // start from 1
	int nTotalStep;
};

#define HASHSIZESTAG 101
#define HASHSIZESTAGITEM HASHSIZEELEM

// post analysis of each construction stage
#define T_CSPA_K unsigned int

struct T_CSPA_STLD_BASE
{
	CArray<T_STLD_K, T_STLD_K> aStldKeys;

	void Initialize()
	{
		aStldKeys.RemoveAll();
	}
	T_CSPA_STLD_BASE& operator=(const T_CSPA_STLD_BASE& src)
	{
		(*this).aStldKeys.Copy(src.aStldKeys);
		return *this;
	}
};

struct T_CSPA_D
{
	CArray<T_CSPA_STLD_BASE, T_CSPA_STLD_BASE&> aSTLoad;
	//BOOL bBuckling;
	CArray<T_CSBK_D, T_CSBK_D&> aBuckData;

	void Initialize()
	{
		aSTLoad.RemoveAll();
		//bBuckling = FALSE;
		aBuckData.RemoveAll();
	}

	T_CSPA_D() { Initialize(); }
	T_CSPA_D(const T_CSPA_D& src) { *this = src; }
	T_CSPA_D& operator=(const T_CSPA_D& src)
	{
		(*this).aSTLoad.Copy(src.aSTLoad);
		//(*this).bBuckling = src.bBuckling;
		(*this).aBuckData.Copy(src.aBuckData);
		return *this;
	}
};

struct T_CSPA_D_RW
{
	int nNumStaticCase;
	//BOOL bBuckling;
	int nNumBuckCase;

	void initialize()
	{
		nNumStaticCase = 0;
		nNumBuckCase = 0;
	}
	void Get(T_CSPA_D& rData)const
	{
		rData.aSTLoad.SetSize(nNumStaticCase);
		//rData.bBuckling = bBuckling;
		rData.aBuckData.SetSize(nNumBuckCase);
	}
	void Set(const T_CSPA_D& rData)
	{
		nNumStaticCase = (int)rData.aSTLoad.GetSize();
		//bBuckling      = rData.bBuckling;
		nNumBuckCase   = (int)rData.aBuckData.GetSize();
	}
};

struct T_CSPA_UDRD_D
{
	T_CSPA_K key;	// T_STAG_K
	T_CSPA_D data;
};

struct T_CSPA_UDRD_D_RW
{
	T_CSPA_K key;
	T_CSPA_D_RW data;

	void GetCspa(T_CSPA_K& rKey, T_CSPA_D& rData)
	{
		rKey = key;
		data.Get(rData);
	}
	void SetCspa(const T_CSPA_K Key, const T_CSPA_D& rData)
	{
		key = Key;
		data.Set(rData);
	}
};

struct T_CSPA_STLD_BASE_RW
{
	CArray<int, int> aStldCount;
	CArray<T_STLD_K, T_STLD_K> aStldKeys;

	void Set(const T_CSPA_D& crData)
	{
		int nCount = crData.aSTLoad.GetSize();
		aStldCount.SetSize(nCount);
		aStldKeys.RemoveAll();

		int ix = 0;
		for (int i = 0; i < nCount; i++)
		{
			aStldCount[ix++] = crData.aSTLoad[i].aStldKeys.GetSize();
			aStldKeys.Append(crData.aSTLoad[i].aStldKeys);
		}
	}
	void Get(T_CSPA_D& rData)const
	{
		int nCount = (int)aStldCount.GetSize();
		rData.aSTLoad.SetSize(nCount);
		int ix = 0;
		for (int i = 0; i < nCount; i++)
		{
			int nStldCount = aStldCount[i];
			rData.aSTLoad[i].aStldKeys.SetSize(nStldCount);
			for (int j = 0; j < nStldCount; j++)
			{
				rData.aSTLoad[i].aStldKeys[j] = aStldKeys[ix++];
			}
		}
	}
};

struct T_CSBK_UDRD_D_RW2
{
	CArray<T_CSBK_D_RW, T_CSBK_D_RW&> aCsbkData;
	CArray<T_CSBK_BASE, T_CSBK_BASE&> CSBucKCase;

	void Set(const T_CSPA_D& crData)
	{
		int nCount = crData.aBuckData.GetSize();
		aCsbkData.SetSize(nCount);
		CSBucKCase.RemoveAll();

		for (int i = 0; i < nCount; i++)
		{
			aCsbkData[i].Set(crData.aBuckData[i]);
			CSBucKCase.Append(crData.aBuckData[i].CSBucKCase);
		}
	}
	void Get(T_CSPA_D& rData)const
	{
		int nCount = (int)aCsbkData.GetSize();
		rData.aBuckData.SetSize(nCount);
		int ix = 0;
		for (int i = 0; i < nCount; i++)
		{
			aCsbkData[i].Get(rData.aBuckData[i]);

			int nNumCSBuckCase = rData.aBuckData[i].CSBucKCase.GetSize();
			for (int j = 0; j < nNumCSBuckCase; j++)
			{
				rData.aBuckData[i].CSBucKCase[j] = CSBucKCase[ix++];
			}
		}
	}
};

//struct T_CSPA_D
//{
//	CArray<T_STLD_K, T_STLD_K> aSTLoad;
//	BOOL     bBuckling;
//	T_CSBK_K CSBuckKey;
//
//	void Initialize()
//	{
//		aSTLoad.RemoveAll();
//		bBuckling = FALSE;
//		CSBuckKey = 0;
//	}
//
//	T_CSPA_D() { Initialize(); }
//	T_CSPA_D(const T_CSPA_D& src) { *this = src; }
//	T_CSPA_D& operator=(const T_CSPA_D& src)
//	{
//		aSTLoad.Copy(src.aSTLoad);
//		bBuckling = src.bBuckling;
//		CSBuckKey = src.CSBuckKey;
//		return *this;
//	}
//};
//
//struct T_CSPA_D_RW
//{
//	int nNnumSTLoad;
//	BOOL bBuckling;
//	T_CSBK_K CSBuckKey;
//
//	void Get(T_CSPA_D& rData)const
//	{
//		rData.aSTLoad.SetSize(nNnumSTLoad);
//		rData.bBuckling = bBuckling;
//		rData.CSBuckKey = CSBuckKey;
//	}
//	void Set(const T_CSPA_D& rData)
//	{
//		nNnumSTLoad = (int)rData.aSTLoad.GetSize();
//		bBuckling = rData.bBuckling;
//		CSBuckKey = rData.CSBuckKey;
//	}
//};
//
//struct T_CSPA_UDRD_D
//{
//	T_CSPA_K key;	// T_STAG_K
//	T_CSPA_D data;
//};
//
//struct T_CSPA_UDRD_D_RW
//{
//	T_CSPA_K key;
//	T_CSPA_D_RW data;
//
//	void GetCspa(T_CSPA_K& rKey, T_CSPA_D& rData)
//	{
//		rKey = key;
//		data.Get(rData);
//	}
//	void SetCspa(const T_CSPA_K Key, const T_CSPA_D& rData)
//	{
//		key = Key;
//		data.Set(rData);
//	}
//};

#define HASHSIZECSPA 101

// struct T_TDMF_BASE
// {
//   double dDay;
//   double dValue;
//   void Initialize()
//   {
//     dDay=0.0;
//     dValue=0.0;
//   }
// };
// #define T_TDMF_DT CArray<T_TDMF_BASE, T_TDMF_BASE&>
// #define T_TDMF_K unsigned int
// struct T_TDMF_D
// {
//   CString MatlFuncName;
//   int nMatlFuncType;    // 1:Creep, 2:Shrinkage Strain
//   int nCreepType;       // 1:Specific Creep, 2:Creep Compliance, 3:Creep Coefficient
//   double dElast;        // nMatlFuncType == 1
//   double ScaleFactor;
//   unsigned int nMatlFuncId;    // 순번
//   T_TDMF_DT arMatlFuncData;
//   CString Description;
//   void Initialize()
//   {
//     MatlFuncName.Empty();
//     nMatlFuncType=1;
//     nCreepType=1;
//     dElast = 0.0;
//     ScaleFactor=1.0;
//     nMatlFuncId=0;
//     arMatlFuncData.RemoveAll();
//     Description.Empty();
//   }
//   T_TDMF_D()
//   {
//   }
//   T_TDMF_D(T_TDMF_D& src)
//   {
//     *this = src;
//   }
//   T_TDMF_D& operator=(T_TDMF_D& src)
//   {
//     MatlFuncName=src.MatlFuncName;
//     nMatlFuncType=src.nMatlFuncType;
//     nCreepType = src.nCreepType;
//     dElast = src.dElast;
//     ScaleFactor=src.ScaleFactor;
//     nMatlFuncId=src.nMatlFuncId;
//     arMatlFuncData.Copy(src.arMatlFuncData);
//     Description=src.Description;
//     return *this;
//   }
// };
// struct T_TDMF_D_CH
// {
//   char MatlFuncName[20];
//   int nMatlFuncType;
//   int nCreepType;
//   double dElast;
//   double ScaleFactor;
//   unsigned int nMatlFuncId;    // 순번
//   T_TDMF_DT arMatlFuncData;
//   char Description[80];
//   void ConvertToString(T_TDMF_D& rData)
//   {
//     ConvertCharStr(MatlFuncName, rData.MatlFuncName, sizeof(MatlFuncName));
//     rData.nMatlFuncType = nMatlFuncType;
//     rData.nCreepType = nCreepType;
//     rData.dElast = dElast;
//     rData.ScaleFactor = ScaleFactor;
//     rData.nMatlFuncId = nMatlFuncId;
//     rData.arMatlFuncData.Copy(arMatlFuncData);
//     ConvertCharStr(Description, rData.Description, sizeof(Description));
//   }
//   void ConvertToChar(T_TDMF_D& rData)
//   {
//     ConvertStrChar(rData.MatlFuncName, MatlFuncName, sizeof(MatlFuncName));
//     nMatlFuncType = rData.nMatlFuncType;
//     nCreepType = rData.nCreepType;
//     dElast = rData.dElast;
//     ScaleFactor = rData.ScaleFactor;
//     nMatlFuncId = rData.nMatlFuncId;
//     arMatlFuncData.Copy(rData.arMatlFuncData);
//     ConvertStrChar(rData.Description, Description, sizeof(Description));
//   }
// };
// struct T_TDMF_UDRD_D
// {
//   T_TDMF_K key;
//   T_TDMF_D data;
// };
// struct T_TDMF_UDRD_D_CH
// {
//   T_TDMF_K key;
//   T_TDMF_D_CH data;
// };
// struct T_TDMF_UDRD_D_CH_RW
// {
//   T_TDMF_K key;
//   char MatlFuncName[20];
//   int nMatlFuncType;
//   int nCreepType;
//   double dElast;
//   double ScaleFactor;
//   unsigned int nMatlFuncId;    // 순번
//   char Description[80];
//   void GetTdmf(T_TDMF_K& rKey, T_TDMF_D_CH& rData)
//   {
//     rKey=key;
//     memcpy(rData.MatlFuncName, MatlFuncName, sizeof(MatlFuncName));
//     rData.nMatlFuncType = nMatlFuncType;
//     rData.nCreepType = nCreepType;
//     rData.dElast = dElast;
//     rData.ScaleFactor = ScaleFactor;
//     rData.nMatlFuncId = nMatlFuncId;
//     memcpy(rData.Description, Description, sizeof(Description));
//   }
//   void SetTdmf(const T_TDMF_K Key, const T_TDMF_D_CH& rData)
//   {
//     key = Key;
//     memcpy(MatlFuncName, rData.MatlFuncName, sizeof(MatlFuncName));
//     nMatlFuncType = rData.nMatlFuncType;
//     nCreepType = rData.nCreepType;
//     dElast = rData.dElast;
//     ScaleFactor = rData.ScaleFactor;
//     nMatlFuncId = rData.nMatlFuncId;
//     memcpy(Description, rData.Description, sizeof(Description));
//   }
// };
// struct T_TDMF_UNIT
// {
//   T_TDMF_UNIT()
//   {
//     dDay = D_UNITSYS_BASE_DAY;
//     dValue[0] = D_UNITSYS_NONE;         // dCreep 사용
//     dValue[1] = D_UNITSYS_NONE;         // shrinkage strain
//     dCreep[0] = D_UNITSYS_BASE_RSTRESS; // specific creep
//     dCreep[1] = D_UNITSYS_BASE_RSTRESS; // creep compliance
//     dCreep[2] = D_UNITSYS_NONE;         // creep coefficient
//     dElast = D_UNITSYS_BASE_ELAST;
//   }
//   int dDay;
//   int dValue[2];
//   int dCreep[3];
//   int dElast;
// };
// #define HASHSIZETDMF 11

struct T_TDMT_BASE
{
	T_TDMF_K CreepFuncKey;
	double dLoadingAge;        // 단위:day
	void Initialize()
	{
		CreepFuncKey = 0;
		dLoadingAge = 0.0;
	}
};
#define T_TDMT_DT CArray<T_TDMT_BASE, T_TDMT_BASE&>
#define T_TDMT_K unsigned int

enum EN_TDMT_TYPE
{
    TDMT_TYPE_NOTATIONAL     = 0,  // Notational Size
};

enum EN_TDMT_CODE
{
	TDMT_CODE_NONE            = 0,  // None
	TDMT_CODE_ACI             ,	    // ACI
	TDMT_CODE_CEB             ,	    // CEB
	TDMT_CODE_ELA_MODULUS     ,	    // Modify Elasticity Modulus
	TDMT_CODE_USER_DEFINE     ,	    // User defined
	TDMT_CODE_KS              ,	    // KS
	TDMT_CODE_PCA             ,	    // PCA
	TDMT_CODE_COMB_ACI_PCA    ,	    // Combined(ACI&PCA)
	TDMT_CODE_JAPAN           ,	    // JAPAN
	TDMT_CODE_CHINA           ,	    // CHINA
	TDMT_CODE_JPJSCE          ,	    // JPJSCE
	TDMT_CODE_CHINA_JTG       ,	    // China(JTG D62-2004)
	TDMT_CODE_AASHTO          ,	    // AASHTO
	TDMT_CODE_CEB_1978        ,	    // CEB-FIP(1978)
	TDMT_CODE_INDIA_IRC_18    ,	    // INDIA(IRC:18-2000)
	TDMT_CODE_EUROPEAN        = 15,	// European
	TDMT_CODE_JPJSCE07        ,     // JPJSCE07
	TDMT_CODE_KSCE_2010       = 20,	// KSCE 2010
	TDMT_CODE_KCI_USD12       ,	    // KCI-USD12
	TDMT_CODE_AUSTRALIA_2009  ,	    // AUSTRALIA 2009
	TDMT_CODE_BS              ,	    // BS
	TDMT_CODE_AUSTRALIA_2011  ,	    // AUSTRALIA 2011
	TDMT_CODE_INDIA_IRC_112   ,	    // INDIA(IRC:112-2011)
	TDMT_CODE_RUSSIAN         ,	    // Russian
	TDMT_CODE_CEB_2010        ,	    // CEB-FIP(2010)
	TDMT_CODE_CHINA_JTG_16    ,	    // 28, China(JTG3362-2018)
	TDMT_CODE_AUSTRALIA_2016  ,	    // 29, AUSTRALIA 2016
	TDMT_CODE_NEWZEALAND      ,     // 30, NEWZEALAND(NZ Bridage (SP/M/022))
	TDMT_CODE_AUSTRALIA_2017  ,	    // 31, AUSTRALIA 2017
	TDMT_CODE_KDS_2016        = 33, // 33, KDS-2016
	TDMT_CODE_JPJSCE12        ,     // 34, JPJSCE12
    TDMT_CODE_INDIA_IRC_112_2020,	// 35, INDIA(IRC:112-2020)
	TDMT_CODE_NEWZEALAND_2022,      // 36, NEWZEALAND(NZ Bridage (SP/M/022 Amd 4:2022))
	TDMT_CODE_AUSTRALIA_2018_AMD_2021,	//37,  AS 3600-2018 Amd 2:2021
	TDMT_CODE_AUSTRALIA_2017_AMD_2024,	//38,  AS 5100.5-2017 Amd 2:2024
	TDMT_CODE_CHINA_CHJTG_T_D65_2015,	//39,  China (JTG/T D65-06-2015)
	TDMT_CODE_ABNT_NBR_6118_2023,	//40,  Brazil (ABNT NBR 6118:2023)
	// 이 밑으로 추가하시오!!

	// 기준 추가 후 CPostCtrl::MakeElementDependentMaterial_Sub 확인!!
	EN_TDMT_CODE_SIZE,
};

struct T_TDMT_COMMON
{
	int    nCode;             // TDMT_CODE_XXXX
	double dCompStrength;     // (ACI, CEB, KS, PCA, COMBINED, JAPAN, CHINA, INDIA, European, KSCE 2010, AUSTRALIA 2009, AUSTRALIA 2011, Russian, JPJSCE07, Brazil)
	double dRelativeHumidity; // (ACI, CEB, KS, PCA, COMBINED, JAPAN, CHINA, INDIA, European, KSCE 2010, Russian, JPJSCE07, Brazil)
	double dConcreteAge;      // (ACI, CEB, KS, JAPAN, CHINA, JPJSCE, INDIA, European, KSCE 2010, AUSTRALIA 2009, AUSTRALIA 2011, Russian, JPJSCE07, JPJSCE12)

	void Initialize()
	{
// 		#if defined(_CIVIL_ORG)
//     nCode=21;		// KCI-USD12
// 		#else
//     nCode=21;		// KS
//     #endif

		nCode             = TDMT_CODE_KCI_USD12;
		dCompStrength     = 0.0;
		dRelativeHumidity = 70.0;
		dConcreteAge      = 3.0;
	}
};
struct T_TDMT_ACI_520
{
	int    nCuringMethod;       // 1:moist cure, 2:steam cure (ACI)
	double dVolumeSurfaceRatio; // (ACI)
	double dSlump;              // (ACI)
	double dFineAggPercent;     // (ACI)
	double dAirContent;         // (ACI)
	double dCementContent;      // (ACI)
	double dMoistCuringPeriod;  // (ACI)
	void Initialize()
	{
		nCuringMethod=1;
		dVolumeSurfaceRatio=0.0;
		dSlump=0.0;
		dFineAggPercent=0.0;
		dAirContent=0.0;
		dCementContent=0.0;
		dMoistCuringPeriod=0.0;
	}
};
struct T_TDMT_ACI_785
{
	int    nType;               // (ACI)  0:ACI code, 1:User (530추가)
	double dCreepCoeff;         // (ACI)  nType==1 (530추가)
	double dShrinkStrain;       // (ACI)  nType==1 (530추가)
	int    nCuringMethod;       // (ACI)  1:moist cure, 2:steam cure
	double dVolumeSurfaceRatio; // (ACI)
	double dSlump;              // (ACI)
	double dFineAggPercent;     // (ACI)
	double dAirContent;         // (ACI)
	double dCementContent;      // (ACI)
	double dMoistCuringPeriod;  // (ACI)
	void Initialize()
	{
		nType = 0;
		dCreepCoeff = 0.0;
		dShrinkStrain = 0.0;
		nCuringMethod=1;
		dVolumeSurfaceRatio=0.0;
		dSlump=0.0;
		dFineAggPercent=0.0;
		dAirContent=0.0;
		dCementContent=0.0;
		dMoistCuringPeriod=0.0;
	}
};
struct T_TDMT_ACI
{
	int    nType;               // (ACI)  0:ACI code, 1:User (530추가)
	double dCreepCoeff;         // (ACI)  nType==1 (530추가)
	double dShrinkStrain;       // (ACI)  nType==1 (530추가)
	int    nCuringMethod;       // (ACI)  1:moist cure, 2:steam cure
	double dVolumeSurfaceRatio; // (ACI)
	double dSlump;              // (ACI)
	double dFineAggPercent;     // (ACI)
	double dAirContent;         // (ACI)
	double dCementContent;      // (ACI)
	double dMoistCuringPeriod;  // (ACI)
	double dConcFactorA;        // (ACI)  Concrete Compressive Strength Factor(a) (790 추가)
	double dConcFactorB;        // (ACI)  Concrete Compressive Strength Factor(b) (790 추가)
	void Initialize()
	{
		nType = 0;
		dCreepCoeff = 0.0;
		dShrinkStrain = 0.0;
		nCuringMethod=1;
		dVolumeSurfaceRatio=0.0;
		dSlump=0.0;
		dFineAggPercent=0.0;
		dAirContent=0.0;
		dCementContent=0.0;
		dMoistCuringPeriod=0.0;
		dConcFactorA = 4.0;
		dConcFactorB = 0.85;
	}
	void Convert520(T_TDMT_ACI_520& rData)
	{
		nType = 0;
		dCreepCoeff = 0.0;
		dShrinkStrain = 0.0;
		nCuringMethod=rData.nCuringMethod;
		dVolumeSurfaceRatio=rData.dVolumeSurfaceRatio;
		dSlump=rData.dSlump;
		dFineAggPercent=rData.dFineAggPercent;
		dAirContent=rData.dAirContent;
		dCementContent=rData.dCementContent;
		dMoistCuringPeriod=rData.dMoistCuringPeriod;
		dConcFactorA = 4.0;
		dConcFactorB = 0.85;
	}
	void Convert785(T_TDMT_ACI_785& rData)
	{
		nType = rData.nType;
		dCreepCoeff = rData.dCreepCoeff;
		dShrinkStrain = rData.dShrinkStrain;
		nCuringMethod=rData.nCuringMethod;
		dVolumeSurfaceRatio=rData.dVolumeSurfaceRatio;
		dSlump=rData.dSlump;
		dFineAggPercent=rData.dFineAggPercent;
		dAirContent=rData.dAirContent;
		dCementContent=rData.dCementContent;
		dMoistCuringPeriod=rData.dMoistCuringPeriod;
		dConcFactorA = 4.0;
		dConcFactorB = 0.85;
	}
};
struct T_TDMT_CEB_750
{
	double dMemberSize; // (CEB, KS, INDIA, European)
	int    nCementType; // (CEB, KS) 1:RS, 2:N,R, 3:SL 
											// (European) 1:Class S, 2:Class N, 3:Class R
	void Initialize()
	{
		dMemberSize=0.0;
		nCementType=2;
	}
};
struct T_TDMT_CEB_841
{
	double dMemberSize; // (CEB, KS, INDIA, European, KSCE 2010)
	int    nCementType; // (CEB, KS, KSCE 2010) 1:RS, 2:N,R, 3:SL 
	// (European) 1:Class S, 2:Class N, 3:Class R
	int    nTypeofCode; // (European) 0:EN 1992-1 (General Structure)
	//            1:EN 1992-2 (Concrete Bridge)
	BOOL   bSilicaFume; 

	void Initialize()
	{
		dMemberSize=0.0;
		nCementType=2;
		nTypeofCode=1;
		bSilicaFume=FALSE;
	}
};
struct T_TDMT_CEB
{
	double dMemberSize; // (CEB, KS, INDIA, European, KSCE 2010)
	int    nCementType; // (CEB, KS, KSCE 2010) 1:RS, 2:N,R, 3:SL 
											// (European) 1:Class S, 2:Class N, 3:Class R
	int    nTypeofCode; // (European) 0:EN 1992-1 (General Structure)
											//            1:EN 1992-2 (Concrete Bridge)
	BOOL   bSilicaFume; 

	int    nTypeofAffr; // (CEB-FIP(2010) 0:Basalt, dense limestone, 1:Quartzite, 2:Limestone, 3:Sandstone
	void Initialize()
	{
		dMemberSize=0.0;
		nCementType=2;
		nTypeofCode=1;
		bSilicaFume=FALSE;
		nTypeofAffr=0;
	}
	void Convert842(T_TDMT_CEB_841& rData)
	{
		dMemberSize=rData.dMemberSize;
		nCementType=rData.nCementType;
		nTypeofCode=rData.nTypeofCode;
		bSilicaFume=rData.bSilicaFume;
		nTypeofAffr=0;
	}
	void Convert750(T_TDMT_CEB_750& rData)
	{
		dMemberSize=rData.dMemberSize;
		nCementType=rData.nCementType;
		nTypeofCode=1;
		bSilicaFume=FALSE;
		nTypeofAffr=0;
	}
};

struct T_TDMT_ELAST
{
	double dN1;         // (ELAST)
	double dPhi1;       // (ELAST)
	double dN2;         // (ELAST)
	double dPhi2;       // (ELAST)
	void Initialize()
	{
		dN1=3.0;
		dPhi1=0.73;
		dN2=5.0;
		dPhi2=1.0;
	}
};
struct T_TDMT_USER
{
	T_TDMF_K ShrinkageStrainFuncKey;
	void Initialize()
	{
		ShrinkageStrainFuncKey = 0;
	}
};
struct T_TDMT_PCA
{
	double dUltimateShrinkageStrain;  // (PCA)
	double dUltimateCreepStrain;      // (PCA)
	double dVolumeSurfaceRatio;       // (PCA)
	double dReinforcementRatio;       // (PCA)
	double dModulusOfElasticity;      // (PCA)
	void Initialize()
	{
		dUltimateShrinkageStrain = 780.0;
		dUltimateCreepStrain = 4.0;
		dVolumeSurfaceRatio = 0.0;
		dReinforcementRatio = 0.0;
		dModulusOfElasticity = 0.0;
	}
};
struct T_TDMT_COMBINED
{
	double dVolumeSurfaceRatio;       // (COMBINED)
	double dCreep_CreepStrain;        // (COMBINED)
	int    nCreep_VolumeSurfaceRatio; // (COMBINED) 0:ACI, 1:PCA
	int    nCreep_LoadingAgedFactor;  // (COMBINED) 0:moist cured ACI, 1:steam cured ACI, 2:PCA
	double dShrink_Shrinkage;         // (COMBINED)
	int    nShrink_VolumeSurfaceRatio;// (COMBINED) 0:ACI, 1:PCA
	int    nShrink_ProgressOfShrinkage; // (COMBINED) 0:moist cured ACI, 1:steam cured ACI, 2:PCA
	BOOL   bReinforcedConcreteEffect; // (COMBINED)
	double dReinforcementRatio;       // (COMBINED)
	double dModulusOfElasticity;      // (COMBINED)
	void Initialize()
	{
		dVolumeSurfaceRatio        = 0.0;
		dCreep_CreepStrain         = 0.0;
		nCreep_VolumeSurfaceRatio  = 0;
		nCreep_LoadingAgedFactor   = 0;
		dShrink_Shrinkage          = 0.0;
		nShrink_VolumeSurfaceRatio = 0;
		nShrink_ProgressOfShrinkage= 0;
		bReinforcedConcreteEffect  = FALSE;
		dReinforcementRatio        = 0.0;
		dModulusOfElasticity       = 0.0;
	}
};
struct T_TDMT_JAPAN_632
{
	int    nRelativeHumidity;
	double dMemberSize;
	int    nCementType;
};
struct T_TDMT_JAPAN_900
{
	int    nRelativeHumidity; // (JAPAN) 1:Curing Underwater, 2:Relative Humidity
	double dMemberSize;       // (JAPAN)
	int    nCementType;       // (JAPAN) 1:Rapid Hardening cement, 2:Normal cement
	int    nCalcMethodForE;   // (JAPAN) 0:JSCE, 1:AIJ (6.4.0에서 추가)
};
struct T_TDMT_JAPAN
{
	int    nRelativeHumidity; // (JAPAN) 1:Curing Underwater, 2:Relative Humidity
	double dMemberSize;       // (JAPAN)
	int    nCementType;       // (JAPAN) 1:Rapid Hardening cement, 2:Normal cement
	int    nCalcMethodForE;   // (JAPAN) 0:JSCE, 1:AIJ (6.4.0에서 추가)
	double dLambda;

	void Initialize()
	{
		nRelativeHumidity=2;
		dMemberSize=0.0;
		nCementType=2;
		#if defined(_CIVIL)
			nCalcMethodForE = 0;  // 도로교 시방서
		#else
			nCalcMethodForE = 1;  // 일본 건축 학회
		#endif
		dLambda = 1.0;
	}
	void Convert632(const T_TDMT_JAPAN_632& rData)
	{
		nRelativeHumidity     = rData.nRelativeHumidity;
		dMemberSize           = rData.dMemberSize;
		nCementType           = rData.nCementType;
		nCalcMethodForE       = 1;  // 640이전은 AIJ로
		dLambda               = 1.0;
	}
	void Convert900(const T_TDMT_JAPAN_900& rData)
	{
		nRelativeHumidity     = rData.nRelativeHumidity;
		dMemberSize           = rData.dMemberSize;
		nCementType           = rData.nCementType;
		nCalcMethodForE       = rData.nCalcMethodForE;
		dLambda               = 1.0;
	}
};

struct T_TDMT_CHINA    // added by KYM
{
	int    nRelativeHumidity; // (CHINA) 1:Curing Underwater, 2:Relative Humidity
	double dMemberSize;       // (CHINA)  
	void Initialize()
	{
		nRelativeHumidity=2;
		dMemberSize=0.0;    
	}
};

struct T_TDMT_JPJSCE_900            // 6.4.0 추가
{
	double dModulusOfElasticity;
	double dRelativeHumidity;         // 
	double dVolumeSurfaceRatio;       // 
	double dCementContent;            // 
	double dWaterContent;            //  
	double dConcreteAge; 
	void Initialize()
	{
		dModulusOfElasticity = 0.;
		dRelativeHumidity    = 70.;
		dVolumeSurfaceRatio  = 0.;
		dCementContent       = 0.;
		dWaterContent        = 0.;    
		dConcreteAge         = 3.;    
	}
};

struct T_TDMT_JPJSCE            // 6.4.0 추가
{
	double dModulusOfElasticity;
	double dRelativeHumidity;         // 
	double dVolumeSurfaceRatio;       // 
	double dCementContent;            // 
	double dWaterContent;            //  
	double dConcreteAge; 
	void Initialize()
	{
		dModulusOfElasticity = 0.;
		dRelativeHumidity    = 70.;
		dVolumeSurfaceRatio  = 0.;
		dCementContent       = 0.;
		dWaterContent        = 0.;    
		dConcreteAge         = 3.;    
	}
};
struct T_TDMT_JPJSCE07
{
// Common
// 	double dCompStrength;       // Comp
// 	double dRelativeHumidity;   // RH
// 	double dConcreteAge;        // AC
	double dVolumeSurfaceRatio;   //  25~300mm
	double dCementContent;
	double dWaterContent; 
	double dAlpaFactor;
	BOOL bAutoShrankage;
	double dGammaFactor;
	double dAFactor; 
	double dBFactor; 
	BOOL bGeneralShrankage;
	void Initialize()
	{
		dVolumeSurfaceRatio  = 0.;
		dCementContent       = 0.;
		dWaterContent        = 0.;    
		dAlpaFactor =  11.0;  //11 or 15
		bAutoShrankage = TRUE;
		dGammaFactor = 1.0;
		dAFactor     = 0.1;
		dBFactor     = 0.7;
		bGeneralShrankage = TRUE;
	}
};
struct T_TDMT_JPJSCE12
{
	// Common
	// 	double dCompStrength;       // Comp
	// 	double dRelativeHumidity;   // RH
	// 	double dConcreteAge;        // AC
	double dVolumeSurfaceRatio;
	double dCementContent;     
	double dWaterContent;      
	double dImpactFactor;
	double dConcreteAgeSol; 
	void Initialize()
	{
		dVolumeSurfaceRatio  = 0.;
		dCementContent       = 0.;
		dWaterContent        = 0.;    
		dImpactFactor   = 1.0;
		dConcreteAgeSol = 1.0;
	}
};


struct T_TDMT_CHJTG04_851    // 6.6.0추가 : 6.6.0부터는 공통데이터도 각 Local에서 정의하도록 변경
{
	double dCompStrength;     
	double dRelativeHumidity; 
	double dMemberSize; 
	double dCementTypeCoef;
	double dConcreteAge;   
	void Initialize()
	{
		dCompStrength = 0.;    
		dRelativeHumidity = 70.;;
		dMemberSize = 0.; 
		dCementTypeCoef = 5.;
		dConcreteAge = 3.;   
	}
};
struct T_TDMT_CHJTG04_955    // 6.6.0추가 : 6.6.0부터는 공통데이터도 각 Local에서 정의하도록 변경
{
	double dCompStrength;
	double dRelativeHumidity;
	double dMemberSize;
	double dCementTypeCoef;
	double dConcreteAge;
	double dAddFlyAsh;
};
struct T_TDMT_CHJTG04_960_R1 
{
	double dCompStrength;
	double dRelativeHumidity;
	double dMemberSize;
	double dCementTypeCoef;
	double dConcreteAge;
	double dAddFlyAsh;
	double dR_Elast;
	double dR_Area;
	double dEqv_Elast;
};
struct T_TDMT_CHJTG04    // 6.6.0추가 : 6.6.0부터는 공통데이터도 각 Local에서 정의하도록 변경
{
	double dCompStrength;     
	double dRelativeHumidity; 
	double dMemberSize; 
	double dCementTypeCoef;
	double dConcreteAge;   
	double dAddFlyAsh;
	double dR_Elast;
	double dR_Area;
	double dEqv_Elast;
	BOOL   bEqv;
	void Initialize()
	{
		dCompStrength = 0.;    
		dRelativeHumidity = 70.;;
		dMemberSize = 0.; 
		dCementTypeCoef = 5.;
		dConcreteAge = 3.;   
		dAddFlyAsh = 0.0;
		dR_Elast = 0.0;
		dR_Area  = 0.0;
		dEqv_Elast = 0.0;
		bEqv = FALSE;
	}
	void Convert955(const T_TDMT_CHJTG04_960_R1& rData)
	{
		dCompStrength     = rData.dCompStrength;
		dRelativeHumidity = rData.dRelativeHumidity;
		dMemberSize       = rData.dMemberSize;
		dCementTypeCoef   = rData.dCementTypeCoef;
		dConcreteAge      = rData.dConcreteAge;
		dAddFlyAsh        = rData.dAddFlyAsh;
		dR_Elast          = rData.dR_Elast;
		dR_Area           = rData.dR_Area;
		dEqv_Elast        = rData.dEqv_Elast;
		bEqv              = FALSE;
	}
	void Convert955(const T_TDMT_CHJTG04_955& rData)
	{
		dCompStrength     = rData.dCompStrength;
		dRelativeHumidity = rData.dRelativeHumidity;
		dMemberSize       = rData.dMemberSize;
		dCementTypeCoef   = rData.dCementTypeCoef;
		dConcreteAge      = rData.dConcreteAge;
		dAddFlyAsh        = rData.dAddFlyAsh;
		dR_Elast          = 0.0;
		dR_Area           = 0.0;
		dEqv_Elast        = 0.0;
		bEqv              = FALSE;
	}
	void Convert851(const T_TDMT_CHJTG04_851& rData)
	{
		dCompStrength     = rData.dCompStrength;
		dRelativeHumidity = rData.dRelativeHumidity;
		dMemberSize       = rData.dMemberSize;
		dCementTypeCoef   = rData.dCementTypeCoef;
		dConcreteAge      = rData.dConcreteAge;
		dAddFlyAsh        = 0.0;
		dR_Elast          = 0.0;
		dR_Area           = 0.0;
		dEqv_Elast        = 0.0;
		bEqv              = FALSE;
	}
};
struct T_TDMT_AASHTO  // 7.0.1추가
{
	double dVolumeSurfaceRatio;
	BOOL   bExpose;
	void Initialize()
	{
		dVolumeSurfaceRatio = 0.0;
		bExpose = FALSE;
	}
};

struct T_TDMT_AUSTRALIA_883
{
	int    nExposureEnv; // 0: Arid  1: Semi arid 2: Temperate Inland 3: Tropical
	double dThickness;   // 구조물의 기하형상 치수
	int    nEps_dry;     // 0 :800   1:900   2:1000, 3:User Define
	void Initialize()
	{
		nExposureEnv = 0;
		dThickness = 0.0;
		nEps_dry = 0;
	}
};

struct T_TDMT_AUSTRALIA_950  // 8.1.5 추가
{
	int    nExposureEnv; // 0: Arid  1: Semi arid 2: Temperate Inland 3: Tropical
	double dThickness;   // 구조물의 기하형상 치수
	int    nEps_dry;     // 0 :800   1:900   2:1000, 3:User Define
	double dEps_dry;     // nEps_dry 이 User Define 일 경우에 씀
};

struct T_TDMT_AUSTRALIA  // 8.1.5 추가
{
	int    nExposureEnv; // 0: Arid  1: Semi arid 2: Temperate Inland 3: Tropical
	double dThickness;   // 구조물의 기하형상 치수
	int    nEps_dry;     // 0 :800   1:900   2:1000, 3:User Define
	double dEps_dry;     // nEps_dry 이 User Define 일 경우에 씀
	double dStressfactor; // 사용안함.
	void Initialize()
	{
		nExposureEnv = 0;
		dThickness = 0.0;
		nEps_dry = 0;
		dEps_dry = 0.0;
		dStressfactor = 1.0;
	}

	void Convert883(const T_TDMT_AUSTRALIA_883& rData)
	{
		nExposureEnv = rData.nExposureEnv;
		dThickness	 = rData.dThickness;
		nEps_dry     = rData.nEps_dry;
		dEps_dry     = 0.0;
		dStressfactor = 1.0;
	}
	void Convert950(const T_TDMT_AUSTRALIA_950& rData)
	{
		nExposureEnv = rData.nExposureEnv;
		dThickness   = rData.dThickness;
		nEps_dry     = rData.nEps_dry;
		dEps_dry     = rData.dEps_dry;
		dStressfactor = 1.0;
	}
};

struct T_TDMT_RUSSIA  // 8.2.5 추가
{
	double dExposeSurfM;  // Module of an exposed surface, M
	int    nCuringMeth;   // Curing Method
	int    nCementType;   // Cement Type 0:Normal 1:Fast-hardened 2:Slag 3:Pozzolan    
	BOOL   bCreep;        // Fast-accumulating creep
	int    nConcType;     // Concrete Type 0:Heavy concrete (N) 1:Fine-grained concrete (M)
	double dWater;        // Water content, W
	double dMaxSize;      // Maximum aggregate size   
	double dAir;          // Air content, V
	double dPz;           // Specific content of the cement paste , pz

	void Initialize()
	{
		dExposeSurfM = 0.0;
		nCuringMeth = 0;
		nCementType = 0;
		bCreep = FALSE;
		nConcType = 0;
		dWater = 0.0;
		dMaxSize = 0.0;
		dAir = 0.0;
		dPz = 0.25;
	}
};

struct T_TDMT_NEWZEALAND_950  // 8.2.5 추가
{
	int nExposureEnv;       // 0: Arid  1: Semi arid 2: Temperate Inland 3: Tropical
	double dRFactorShirink; // Relative Humidity Factor for Shrinkage
	double dThickness;      // Hypothetical Thickness
	double dEps_dry;        // Drying Basic Shrinkage Strain
	int nEps_dry;
};

struct T_TDMT_NEWZEALAND  // 8.2.5 추가
{
	int nExposureEnv;       // 0: Arid  1: Semi arid 2: Temperate Inland 3: Tropical
	double dRFactorShirink; // Relative Humidity Factor for Shrinkage
	double dThickness;      // Hypothetical Thickness
	double dEps_dry;        // Drying Basic Shrinkage Strain
	int nEps_dry;           // 0~10: DB(CMTdmtNEWZEALAND::SetCtrl() 참고) 11:User Defined
	double dAggFactor;      // modification factor for aggregate type
	void Initialize()
	{
		nExposureEnv = 0;       // 0: Arid  1: Semi arid 2: Temperate Inland 3: Tropical
		dRFactorShirink = 0.72; // Relative Humidity Factor for Shrinkage
		dThickness = 0.0;      // Hypothetical Thickness
		dEps_dry = 1000;        // Drying Basic Shrinkage Strain
		nEps_dry = 4;           // 0~9: DB(CMTdmtNEWZEALAND::SetCtrl() 참고) 11:User Defined  
		dAggFactor = 1.0;
	}
	void Convert950(const T_TDMT_NEWZEALAND_950& rData)
	{
		nExposureEnv    = rData.nExposureEnv;
		dRFactorShirink = rData.dRFactorShirink;
		dThickness      = rData.dThickness;
		dEps_dry        = rData.dEps_dry;
		nEps_dry        = rData.nEps_dry;
		dAggFactor      = 1.0;
	}

};

struct T_TDMT_KDS2016
{
	double dDensity;       // Weight Density (고정단위계라 Unit 처리 안함)

	void Initialize()
	{
		dDensity = 0.0;
	}
};

struct T_TDMT_BRAZIL  // 9.7.0 추가
{
	BOOL bInWater;
  double dThickness;          // Fictitious Thickness
	int nCementType;            // 0 : 0.38 for CPIII and IV,  1 : 0.25 for CPI and II,  2 : 0.2 for CPV-ARI
	int nAggtype;               // 0 : 1.2 for Basalt and Diabase,  1 : 1.0 for Granite and Gneiss,  2 : 0.9 for Limestone,  3 : 0.7 for Sandstone
	int nCementPerformanceType; // 0 : C20 to C45  1: C50 to C90
	int nDeflectionRange;       // 0 : 0cm to 4cm  1: 5 to 9cm  2 : 10cm to 15cm

	void Initialize()
	{
		bInWater = FALSE;
		dThickness = 0.;
		nCementType = 0;
		nAggtype = 0;
		nCementPerformanceType = 0;
		nDeflectionRange = 0;
	}
};

/*
struct T_TDMT_D_520
{
	CString         strMatlName;
	T_TDMT_COMMON   COMMON;
	T_TDMT_ACI      ACI;
	T_TDMT_CEB      CEB;  // KS도 CEB에 저장, KS인 경우 COMMON::nCode=5
	T_TDMT_ELAST    ELAST;
	T_TDMT_USER     USER;
	T_TDMT_DT       aCreepFunc;
	unsigned int    nSeq;
	void Initialize()
	{
		strMatlName.Empty();
		
		COMMON.Initialize();
		ACI.Initialize();
		CEB.Initialize();
		ELAST.Initialize();
		USER.Initialize();
		aCreepFunc.RemoveAll();
		nSeq = 0;
	}
};
struct T_TDMT_D_530
{
	CString         strMatlName;
	T_TDMT_COMMON   COMMON;
	T_TDMT_ACI      ACI;
	T_TDMT_CEB      CEB;  // KS도 CEB에 저장, KS인 경우 COMMON::nCode=5
	T_TDMT_ELAST    ELAST;
	T_TDMT_USER     USER;
	T_TDMT_PCA      PCA;
	T_TDMT_COMBINED COMBINED;
	T_TDMT_DT       aCreepFunc;
	unsigned int    nSeq;

	void Initialize()
	{
		strMatlName.Empty();
		
		COMMON.Initialize();
		ACI.Initialize();
		CEB.Initialize();
		ELAST.Initialize();
		USER.Initialize();
		PCA.Initialize();
		COMBINED.Initialize();
		aCreepFunc.RemoveAll();
		nSeq = 0;
	}
};
struct T_TDMT_D_550     // added by KYM
{
	CString           strMatlName;
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI        ACI;
	T_TDMT_CEB        CEB;  // KS도 CEB에 저장, KS인 경우 COMMON::nCode=5
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN_632  JAPAN;  
	T_TDMT_DT         aCreepFunc;
	unsigned int      nSeq;

	void Initialize()
	{
		strMatlName.Empty();
		
		COMMON.Initialize();
		ACI.Initialize();
		CEB.Initialize();
		ELAST.Initialize();
		USER.Initialize();
		PCA.Initialize();
		COMBINED.Initialize();
		JAPAN.Initialize();
		aCreepFunc.RemoveAll();
		nSeq = 0;
	}
};
struct T_TDMT_D_630
{
	CString           strMatlName;
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI        ACI;
	T_TDMT_CEB        CEB;  // KS도 CEB에 저장, KS인 경우 COMMON::nCode=5
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN_632  JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	T_TDMT_DT         aCreepFunc;
	unsigned int      nSeq;
};
struct T_TDMT_D_632
{
	CString           strMatlName;
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI        ACI;
	T_TDMT_CEB        CEB;  // KS도 CEB에 저장, KS인 경우 COMMON::nCode=5
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN_632  JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	T_TDMT_DT         aCreepFunc;
	T_TDMT_JPJSCE     JPJSCE;       // 6.4.0 추가
	unsigned int      nSeq;
};
*/
struct T_TDMT_D
{
	CString           strMatlName;
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI        ACI;
	T_TDMT_CEB        CEB;  // KS, INDIA도 CEB에 저장, KS인 경우 COMMON::nCode=5, INDIA:14
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN      JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	T_TDMT_DT         aCreepFunc;
	T_TDMT_JPJSCE     JPJSCE;     // 6.4.0 추가
	T_TDMT_CHJTG04    CHJTG04;    // 6.6.0 추가   
	T_TDMT_AASHTO     AASHTO;     // 7.0.1 추가
	T_TDMT_AUSTRALIA  AUSTRALIA;  // 8.1.5 추가
	T_TDMT_RUSSIA     RUSSIA;     // 8.2.5 추가  
	T_TDMT_NEWZEALAND NEWZEALAND; // 8.6.0 추가
	T_TDMT_KDS2016    KDS2016;    // 8.9.0 추가
	T_TDMT_JPJSCE07   JPJSCE07; 
	T_TDMT_JPJSCE12   JPJSCE12; 
  T_TDMT_BRAZIL     BRAZIL;     // 9.7.0 추가
	unsigned int      nSeq;

	void Initialize()
	{
		strMatlName.Empty();
		
		COMMON.Initialize();
		ACI.Initialize();
		CEB.Initialize();
		ELAST.Initialize();
		USER.Initialize();
		PCA.Initialize();
		COMBINED.Initialize();
		JAPAN.Initialize();
		CHINA.Initialize();
		aCreepFunc.RemoveAll();
		JPJSCE.Initialize();
		CHJTG04.Initialize();
		AASHTO.Initialize();
		AUSTRALIA.Initialize();
		RUSSIA.Initialize();
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq = 0;
	}
	T_TDMT_D()
	{
	}
	T_TDMT_D(T_TDMT_D& src)
	{
		*this = src;
	}
	T_TDMT_D& operator=(T_TDMT_D& src)
	{
		strMatlName=src.strMatlName;
		COMMON = src.COMMON;
		ACI = src.ACI;
		CEB = src.CEB;
		ELAST = src.ELAST;
		USER = src.USER;
		PCA = src.PCA;
		COMBINED = src.COMBINED;
		JAPAN = src.JAPAN;
		CHINA = src.CHINA;
		aCreepFunc.Copy(src.aCreepFunc);
		nSeq=src.nSeq;
		JPJSCE = src.JPJSCE;
		CHJTG04 = src.CHJTG04;
		AASHTO = src.AASHTO;
		AUSTRALIA = src.AUSTRALIA;
		RUSSIA = src.RUSSIA;
		NEWZEALAND = src.NEWZEALAND;
		KDS2016 = src.KDS2016;
		JPJSCE07 = src.JPJSCE07;
		JPJSCE12 = src.JPJSCE12;
		BRAZIL = src.BRAZIL;

		return *this;
	}
	void ConvertHcrp(T_HCRP_D& rData)
	{
		strMatlName = "old-version creep";
		COMMON.nCode = rData.nCode;
		COMMON.dCompStrength = rData.dCompStrength;
		COMMON.dRelativeHumidity = rData.dRelativeHumidity;
		COMMON.dConcreteAge = rData.nConcreteAge;
		ACI.nCuringMethod = rData.nCuringMethod;
		ACI.dVolumeSurfaceRatio = rData.dVolumeSurfaceRatio;
		ACI.dSlump = rData.dSlump;
		ACI.dFineAggPercent = rData.dFineAggPercent;
		ACI.dAirContent = rData.dAirContent;
		ACI.dCementContent = rData.dCementContent;
		ACI.dMoistCuringPeriod = rData.nMoistCuringPeriod;
		CEB.dMemberSize = rData.dMemberSize;
		CEB.nCementType = rData.nCementType;
		ELAST.dN1 = rData.nN1;
		ELAST.dPhi1 = rData.dPhi1;
		ELAST.dN2 = rData.nN2;
		ELAST.dPhi2 = rData.dPhi2;

		USER.Initialize();
		PCA.Initialize();
		COMBINED.Initialize();
		JAPAN.Initialize();
		CHINA.Initialize();   // added by KYM
		aCreepFunc.RemoveAll();
		JPJSCE.Initialize();
		CHJTG04.Initialize();
		AASHTO.Initialize();
		AUSTRALIA.Initialize();
		RUSSIA.Initialize();
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq = 0;
	}
};
struct T_TDMT_D_CH
{
	char              strMatlName[60];
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI        ACI;
	T_TDMT_CEB        CEB;
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN      JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	T_TDMT_DT         aCreepFunc;
	T_TDMT_JPJSCE     JPJSCE;
	T_TDMT_CHJTG04    CHJTG04;    // 6.6.0 추가
	T_TDMT_AASHTO     AASHTO;     // 7.0.1 추가
	T_TDMT_AUSTRALIA  AUSTRALIA;  // 8.1.5 추가
	T_TDMT_RUSSIA     RUSSIA;     // 8.2.5 추가
	T_TDMT_NEWZEALAND NEWZEALAND; // 8.6.0 추가
	T_TDMT_KDS2016    KDS2016;    // 8.9.0 추가
	T_TDMT_JPJSCE07     JPJSCE07;
	T_TDMT_JPJSCE12     JPJSCE12;
	T_TDMT_BRAZIL       BRAZIL;     // 9.7.0 추가
	unsigned int    nSeq;  

	void ConvertToString(T_TDMT_D& rData)
	{
		ConvertCharStr(strMatlName, rData.strMatlName, sizeof(strMatlName));
		rData.COMMON = COMMON;
		rData.ACI = ACI;
		rData.CEB = CEB;
		rData.ELAST = ELAST;
		rData.USER = USER;
		rData.PCA = PCA;
		rData.COMBINED = COMBINED;
		rData.JAPAN = JAPAN;
		rData.CHINA = CHINA;  // added by KYM
		rData.aCreepFunc.Copy(aCreepFunc);
		rData.JPJSCE = JPJSCE;
		rData.CHJTG04 = CHJTG04;
		rData.AASHTO = AASHTO;
		rData.AUSTRALIA = AUSTRALIA;
		rData.RUSSIA = RUSSIA;
		rData.NEWZEALAND = NEWZEALAND;    
		rData.KDS2016 = KDS2016;
		rData.JPJSCE07 = JPJSCE07;
		rData.JPJSCE12 = JPJSCE12;
    rData.BRAZIL = BRAZIL;
		rData.nSeq=nSeq;
	}
	void ConvertToChar(T_TDMT_D& rData)
	{
		ConvertStrChar(rData.strMatlName, strMatlName, sizeof(strMatlName));
		COMMON = rData.COMMON;
		ACI = rData.ACI;
		CEB = rData.CEB;
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN = rData.JAPAN;
		CHINA = rData.CHINA;  // added by KYM
		aCreepFunc.Copy(rData.aCreepFunc);
		JPJSCE = rData.JPJSCE;
		CHJTG04 = rData.CHJTG04;
		AASHTO = rData.AASHTO;
		AUSTRALIA = rData.AUSTRALIA;
		RUSSIA = rData.RUSSIA;
		NEWZEALAND = rData.NEWZEALAND;
		KDS2016 = rData.KDS2016;
		JPJSCE07 = rData.JPJSCE07;
		JPJSCE12 = rData.JPJSCE12;
		BRAZIL = rData.BRAZIL;
		nSeq=rData.nSeq;
	}
};
struct T_TDMT_UDRD_D
{
	T_TDMT_K key;
	T_TDMT_D data;
};
struct T_TDMT_UDRD_D_CH
{
	T_TDMT_K key;
	T_TDMT_D_CH data;
};
struct T_TDMT_UDRD_D_CH_RW_520
{
	T_TDMT_K        key;
	char            strMatlName[20];
	T_TDMT_COMMON   COMMON;
	T_TDMT_ACI_520  ACI;
	T_TDMT_CEB_750  CEB;
	T_TDMT_ELAST    ELAST;
	T_TDMT_USER     USER;
	int             nCreepFuncCount;
	unsigned int    nSeq;
};
struct T_TDMT_UDRD_D_CH_RW_530
{
	T_TDMT_K        key;
	char            strMatlName[20];
	T_TDMT_COMMON   COMMON;
	T_TDMT_ACI_785  ACI;
	T_TDMT_CEB_750  CEB;
	T_TDMT_ELAST    ELAST;
	T_TDMT_USER     USER;
	T_TDMT_PCA      PCA;
	T_TDMT_COMBINED COMBINED;
	int             nCreepFuncCount;
	unsigned int    nSeq;
};
struct T_TDMT_UDRD_D_CH_RW_550   // added by KYM
{
	T_TDMT_K        key;
	char            strMatlName[20];
	T_TDMT_COMMON   COMMON;
	T_TDMT_ACI_785  ACI;
	T_TDMT_CEB_750  CEB;
	T_TDMT_ELAST    ELAST;
	T_TDMT_USER     USER;
	T_TDMT_PCA      PCA;
	T_TDMT_COMBINED COMBINED;
	T_TDMT_JAPAN_632    JAPAN;
	int             nCreepFuncCount;
	unsigned int    nSeq;
};
struct T_TDMT_UDRD_D_CH_RW_630
{
	T_TDMT_K          key;
	char              strMatlName[20];
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI_785    ACI;
	T_TDMT_CEB_750    CEB;
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN_632  JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	int               nCreepFuncCount;
	unsigned int      nSeq;
};
struct T_TDMT_UDRD_D_CH_RW_632
{
	T_TDMT_K          key;
	char              strMatlName[20];
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI_785    ACI;
	T_TDMT_CEB_750    CEB;
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN_632  JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	int               nCreepFuncCount;
	T_TDMT_JPJSCE     JPJSCE;
	unsigned int      nSeq;
};
struct T_TDMT_UDRD_D_CH_RW_650
{
	T_TDMT_K          key;
	char              strMatlName[20];
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI_785    ACI;
	T_TDMT_CEB_750    CEB;
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN_900  JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	int               nCreepFuncCount;
	T_TDMT_JPJSCE     JPJSCE;
	unsigned int      nSeq;
};
struct T_TDMT_UDRD_D_CH_RW_700
{
	T_TDMT_K           key;
	char               strMatlName[20];
	T_TDMT_COMMON      COMMON;
	T_TDMT_ACI_785     ACI;
	T_TDMT_CEB_750     CEB;
	T_TDMT_ELAST       ELAST;
	T_TDMT_USER        USER;
	T_TDMT_PCA         PCA;
	T_TDMT_COMBINED    COMBINED;
	T_TDMT_JAPAN_900   JAPAN;
	T_TDMT_CHINA       CHINA;   // added by KYM
	int                nCreepFuncCount;
	T_TDMT_JPJSCE      JPJSCE;
	T_TDMT_CHJTG04_851 CHJTG04;   // 6.6.0 추가
	unsigned int       nSeq;
};
struct T_TDMT_UDRD_D_CH_RW_750
{
	T_TDMT_K           key;
	char               strMatlName[20];
	T_TDMT_COMMON      COMMON;
	T_TDMT_ACI_785     ACI;
	T_TDMT_CEB_750     CEB;
	T_TDMT_ELAST       ELAST;
	T_TDMT_USER        USER;
	T_TDMT_PCA         PCA;
	T_TDMT_COMBINED    COMBINED;
	T_TDMT_JAPAN_900   JAPAN;
	T_TDMT_CHINA       CHINA;   // added by KYM
	int                nCreepFuncCount;
	T_TDMT_JPJSCE      JPJSCE;
	T_TDMT_CHJTG04_851 CHJTG04;   // 6.6.0 추가
	T_TDMT_AASHTO      AASHTO;     // 7.0.1 추가
	unsigned int       nSeq;
};
struct T_TDMT_UDRD_D_CH_RW_785
{
	T_TDMT_K           key;
	char               strMatlName[20];
	T_TDMT_COMMON      COMMON;
	T_TDMT_ACI_785     ACI;  // 790 추가
	T_TDMT_CEB_841     CEB;
	T_TDMT_ELAST       ELAST;
	T_TDMT_USER        USER;
	T_TDMT_PCA         PCA;
	T_TDMT_COMBINED    COMBINED;
	T_TDMT_JAPAN_900   JAPAN;
	T_TDMT_CHINA       CHINA;
	int                nCreepFuncCount;
	T_TDMT_JPJSCE      JPJSCE;
	T_TDMT_CHJTG04_851 CHJTG04;
	T_TDMT_AASHTO      AASHTO;
	unsigned int       nSeq;
};

struct T_TDMT_UDRD_D_CH_RW_811
{
	T_TDMT_K           key;
	char               strMatlName[20];
	T_TDMT_COMMON      COMMON;
	T_TDMT_ACI         ACI;
	T_TDMT_CEB_841     CEB;
	T_TDMT_ELAST       ELAST;
	T_TDMT_USER        USER;
	T_TDMT_PCA         PCA;
	T_TDMT_COMBINED    COMBINED;
	T_TDMT_JAPAN_900   JAPAN;
	T_TDMT_CHINA       CHINA;   // added by KYM
	int                nCreepFuncCount;
	T_TDMT_JPJSCE      JPJSCE;
	T_TDMT_CHJTG04_851 CHJTG04;   // 6.6.0 추가
	T_TDMT_AASHTO      AASHTO;     // 7.0.1 추가
	unsigned int       nSeq;
};
struct T_TDMT_UDRD_D_CH_RW_821
{
	T_TDMT_K           key;
	char               strMatlName[20];
	T_TDMT_COMMON      COMMON;
	T_TDMT_ACI         ACI;
	T_TDMT_CEB_841     CEB;
	T_TDMT_ELAST       ELAST;
	T_TDMT_USER        USER;
	T_TDMT_PCA         PCA;
	T_TDMT_COMBINED    COMBINED;
	T_TDMT_JAPAN_900   JAPAN;
	T_TDMT_CHINA       CHINA;   // added by KYM
	int                nCreepFuncCount;
	T_TDMT_JPJSCE      JPJSCE;
	T_TDMT_CHJTG04_851 CHJTG04;   // 6.6.0 추가
	T_TDMT_AASHTO      AASHTO;    // 7.0.1 추가
	T_TDMT_AUSTRALIA_883   AUSTRALIA;  // 8.1.5 추가
	unsigned int       nSeq;
};
struct T_TDMT_UDRD_D_CH_RW_841
{
	T_TDMT_K           key;
	char               strMatlName[20];
	T_TDMT_COMMON      COMMON;
	T_TDMT_ACI         ACI;
	T_TDMT_CEB_841     CEB;
	T_TDMT_ELAST       ELAST;
	T_TDMT_USER        USER;
	T_TDMT_PCA         PCA;
	T_TDMT_COMBINED    COMBINED;
	T_TDMT_JAPAN_900   JAPAN;
	T_TDMT_CHINA       CHINA;   // added by KYM
	int                nCreepFuncCount;
	T_TDMT_JPJSCE      JPJSCE;
	T_TDMT_CHJTG04_851 CHJTG04;   // 6.6.0 추가
	T_TDMT_AASHTO      AASHTO;    // 7.0.1 추가
	T_TDMT_AUSTRALIA_883   AUSTRALIA; // 8.1.5 추가
	T_TDMT_RUSSIA      RUSSIA;    // 8.2.5 추가
	unsigned int       nSeq;
};
struct T_TDMT_UDRD_D_CH_RW_851
{
	T_TDMT_K           key;
	char               strMatlName[20];
	T_TDMT_COMMON      COMMON;
	T_TDMT_ACI         ACI;
	T_TDMT_CEB         CEB;
	T_TDMT_ELAST       ELAST;
	T_TDMT_USER        USER;
	T_TDMT_PCA         PCA;
	T_TDMT_COMBINED    COMBINED;
	T_TDMT_JAPAN_900   JAPAN;
	T_TDMT_CHINA       CHINA;   // added by KYM
	int                nCreepFuncCount;
	T_TDMT_JPJSCE      JPJSCE;
	T_TDMT_CHJTG04_851 CHJTG04;   // 6.6.0 추가
	T_TDMT_AASHTO      AASHTO;    // 7.0.1 추가
	T_TDMT_AUSTRALIA_883   AUSTRALIA; // 8.1.5 추가
	T_TDMT_RUSSIA      RUSSIA;    // 8.2.5 추가
	unsigned int       nSeq;
};
struct T_TDMT_UDRD_D_CH_RW_855
{
	T_TDMT_K           key;
	char               strMatlName[20];
	T_TDMT_COMMON      COMMON;
	T_TDMT_ACI         ACI;
	T_TDMT_CEB         CEB;
	T_TDMT_ELAST       ELAST;
	T_TDMT_USER        USER;
	T_TDMT_PCA         PCA;
	T_TDMT_COMBINED    COMBINED;
	T_TDMT_JAPAN_900   JAPAN;
	T_TDMT_CHINA       CHINA;   // added by KYM
	int                nCreepFuncCount;
	T_TDMT_JPJSCE      JPJSCE;
	T_TDMT_CHJTG04_955 CHJTG04;   // 6.6.0 추가
	T_TDMT_AASHTO      AASHTO;    // 7.0.1 추가
	T_TDMT_AUSTRALIA_883   AUSTRALIA; // 8.1.5 추가
	T_TDMT_RUSSIA      RUSSIA;    // 8.2.5 추가
	unsigned int       nSeq;
};

struct T_TDMT_UDRD_D_CH_RW_883
{
	T_TDMT_K          key;
	char              strMatlName[20];
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI        ACI;
	T_TDMT_CEB        CEB;
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN_900  JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	int               nCreepFuncCount;
	T_TDMT_JPJSCE     JPJSCE;
	T_TDMT_CHJTG04_955 CHJTG04;   // 6.6.0 추가
	T_TDMT_AASHTO     AASHTO;    // 7.0.1 추가
	T_TDMT_AUSTRALIA_883  AUSTRALIA; // 8.1.5 추가
	T_TDMT_RUSSIA     RUSSIA;    // 8.2.5 추가
	T_TDMT_NEWZEALAND_950 NEWZEALAND;// 8.6.0 추가  
	unsigned int      nSeq;
};

struct T_TDMT_UDRD_D_CH_RW_885
{
	T_TDMT_K          key;
	char              strMatlName[20];
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI        ACI;
	T_TDMT_CEB        CEB;
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN_900  JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	int               nCreepFuncCount;
	T_TDMT_JPJSCE     JPJSCE;
	T_TDMT_CHJTG04_955    CHJTG04;   // 6.6.0 추가
	T_TDMT_AASHTO     AASHTO;    // 7.0.1 추가
	T_TDMT_AUSTRALIA_950  AUSTRALIA; // 8.1.5 추가
	T_TDMT_RUSSIA     RUSSIA;    // 8.2.5 추가
	T_TDMT_NEWZEALAND_950 NEWZEALAND;// 8.6.0 추가  
	unsigned int      nSeq;
};

struct T_TDMT_UDRD_D_CH_RW_900
{
	T_TDMT_K          key;
	char              strMatlName[20];
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI        ACI;
	T_TDMT_CEB        CEB;
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN_900  JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	int               nCreepFuncCount;
	T_TDMT_JPJSCE     JPJSCE;
	T_TDMT_CHJTG04_955    CHJTG04;   // 6.6.0 추가
	T_TDMT_AASHTO     AASHTO;    // 7.0.1 추가
	T_TDMT_AUSTRALIA_950  AUSTRALIA; // 8.1.5 추가
	T_TDMT_RUSSIA     RUSSIA;    // 8.2.5 추가
	T_TDMT_NEWZEALAND_950 NEWZEALAND;// 8.6.0 추가  
	T_TDMT_KDS2016    KDS2016;   // 8.9.0 추가  
	unsigned int      nSeq;
};

struct T_TDMT_UDRD_D_CH_RW_950
{
	T_TDMT_K          key;
	char              strMatlName[20];
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI        ACI;
	T_TDMT_CEB        CEB;
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN      JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	int               nCreepFuncCount;
	T_TDMT_JPJSCE     JPJSCE;
	T_TDMT_CHJTG04_955    CHJTG04;   // 6.6.0 추가
	T_TDMT_AASHTO     AASHTO;    // 7.0.1 추가
	T_TDMT_AUSTRALIA_950  AUSTRALIA; // 8.1.5 추가
	T_TDMT_RUSSIA     RUSSIA;    // 8.2.5 추가
	T_TDMT_NEWZEALAND_950 NEWZEALAND;// 8.6.0 추가  
	T_TDMT_KDS2016    KDS2016;   // 8.9.0 추가  
	T_TDMT_JPJSCE07   JPJSCE07;
	T_TDMT_JPJSCE12   JPJSCE12;
	unsigned int      nSeq;
};

struct T_TDMT_UDRD_D_CH_RW_950_2
{
	T_TDMT_K          key;
	char              strMatlName[60]; // (v955) Size 변경 
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI        ACI;
	T_TDMT_CEB        CEB;
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN      JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	int               nCreepFuncCount;
	T_TDMT_JPJSCE     JPJSCE;
	T_TDMT_CHJTG04_955    CHJTG04;   // 6.6.0 추가
	T_TDMT_AASHTO     AASHTO;    // 7.0.1 추가
	T_TDMT_AUSTRALIA  AUSTRALIA; // 8.1.5 추가
	T_TDMT_RUSSIA     RUSSIA;    // 8.2.5 추가
	T_TDMT_NEWZEALAND_950 NEWZEALAND;// 8.6.0 추가  
	T_TDMT_KDS2016    KDS2016;   // 8.9.0 추가  
	T_TDMT_JPJSCE07   JPJSCE07;
	T_TDMT_JPJSCE12   JPJSCE12;
	unsigned int      nSeq;
};

struct T_TDMT_UDRD_D_CH_RW_955
{
	T_TDMT_K          key;
	char              strMatlName[60]; // (v955) Size 변경 
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI        ACI;
	T_TDMT_CEB        CEB;
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN      JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	int               nCreepFuncCount;
	T_TDMT_JPJSCE     JPJSCE;
	T_TDMT_CHJTG04_955 CHJTG04;   // 6.6.0 추가
	T_TDMT_AASHTO     AASHTO;    // 7.0.1 추가
	T_TDMT_AUSTRALIA  AUSTRALIA; // 8.1.5 추가
	T_TDMT_RUSSIA     RUSSIA;    // 8.2.5 추가
	T_TDMT_NEWZEALAND NEWZEALAND;// 8.6.0 추가  
	T_TDMT_KDS2016    KDS2016;   // 8.9.0 추가  
	T_TDMT_JPJSCE07   JPJSCE07;
	T_TDMT_JPJSCE12   JPJSCE12;
	unsigned int      nSeq;
};

struct T_TDMT_UDRD_D_CH_RW_960_R1
{
	T_TDMT_K          key;
	char              strMatlName[60]; // (v955) Size 변경 
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI        ACI;
	T_TDMT_CEB        CEB;
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN      JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	int               nCreepFuncCount;
	T_TDMT_JPJSCE     JPJSCE;
	T_TDMT_CHJTG04_960_R1 CHJTG04;
	T_TDMT_AASHTO     AASHTO;    // 7.0.1 추가
	T_TDMT_AUSTRALIA  AUSTRALIA; // 8.1.5 추가
	T_TDMT_RUSSIA     RUSSIA;    // 8.2.5 추가
	T_TDMT_NEWZEALAND NEWZEALAND;// 8.6.0 추가  
	T_TDMT_KDS2016    KDS2016;   // 8.9.0 추가  
	T_TDMT_JPJSCE07   JPJSCE07;
	T_TDMT_JPJSCE12   JPJSCE12;
	unsigned int      nSeq;
};

struct T_TDMT_UDRD_D_CH_RW_965
{
	T_TDMT_K          key;
	char              strMatlName[60]; // (v955) Size 변경 
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI        ACI;
	T_TDMT_CEB        CEB;
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN      JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	int               nCreepFuncCount;
	T_TDMT_JPJSCE     JPJSCE;
	T_TDMT_CHJTG04    CHJTG04;   // 6.6.0 추가
	T_TDMT_AASHTO     AASHTO;    // 7.0.1 추가
	T_TDMT_AUSTRALIA  AUSTRALIA; // 8.1.5 추가
	T_TDMT_RUSSIA     RUSSIA;    // 8.2.5 추가
	T_TDMT_NEWZEALAND NEWZEALAND;// 8.6.0 추가  
	T_TDMT_KDS2016    KDS2016;   // 8.9.0 추가  
	T_TDMT_JPJSCE07   JPJSCE07;
	T_TDMT_JPJSCE12   JPJSCE12;
	unsigned int      nSeq;
};

struct T_TDMT_UDRD_D_CH_RW
{
	T_TDMT_K          key;
	char              strMatlName[60]; // (v955) Size 변경 
	T_TDMT_COMMON     COMMON;
	T_TDMT_ACI        ACI;
	T_TDMT_CEB        CEB;
	T_TDMT_ELAST      ELAST;
	T_TDMT_USER       USER;
	T_TDMT_PCA        PCA;
	T_TDMT_COMBINED   COMBINED;
	T_TDMT_JAPAN      JAPAN;
	T_TDMT_CHINA      CHINA;   // added by KYM
	int               nCreepFuncCount;
	T_TDMT_JPJSCE     JPJSCE;
	T_TDMT_CHJTG04    CHJTG04;   // 6.6.0 추가
	T_TDMT_AASHTO     AASHTO;    // 7.0.1 추가
	T_TDMT_AUSTRALIA  AUSTRALIA; // 8.1.5 추가
	T_TDMT_RUSSIA     RUSSIA;    // 8.2.5 추가
	T_TDMT_NEWZEALAND NEWZEALAND;// 8.6.0 추가  
	T_TDMT_KDS2016    KDS2016;   // 8.9.0 추가  
	T_TDMT_JPJSCE07   JPJSCE07;
	T_TDMT_JPJSCE12   JPJSCE12;
	T_TDMT_BRAZIL     BRAZIL;     // 9.7.0 추가
	unsigned int      nSeq;

	void GetTdmt(T_TDMT_K& rKey, T_TDMT_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strMatlName, strMatlName, sizeof(rData.strMatlName));
		rData.COMMON = COMMON;
		rData.ACI = ACI;
		rData.CEB = CEB;
		rData.ELAST = ELAST;
		rData.USER = USER;
		rData.PCA = PCA;
		rData.COMBINED = COMBINED;
		rData.JAPAN = JAPAN;
		rData.CHINA = CHINA;  // added by KYM
		rData.JPJSCE = JPJSCE;    
		rData.CHJTG04 = CHJTG04;
		rData.AASHTO = AASHTO;
		rData.AUSTRALIA = AUSTRALIA;
		rData.RUSSIA = RUSSIA;
		rData.NEWZEALAND = NEWZEALAND;
		rData.KDS2016 = KDS2016;
		rData.JPJSCE07 = JPJSCE07;    
		rData.JPJSCE12 = JPJSCE12;    
		rData.BRAZIL = BRAZIL;
		rData.nSeq = nSeq;    
	}
	void SetTdmt(const T_TDMT_K Key, const T_TDMT_D_CH& rData)
	{
		key=Key;
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI = rData.ACI;
		CEB = rData.CEB;
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN = rData.JAPAN;
		CHINA = rData.CHINA;  // KYM
		nCreepFuncCount=(int)rData.aCreepFunc.GetSize();
		JPJSCE = rData.JPJSCE;
		CHJTG04 = rData.CHJTG04;
		AASHTO = rData.AASHTO;
		AUSTRALIA = rData.AUSTRALIA;
		RUSSIA = rData.RUSSIA;
		NEWZEALAND = rData.NEWZEALAND;
		KDS2016 = rData.KDS2016;
		JPJSCE07 = rData.JPJSCE07;    
		JPJSCE12 = rData.JPJSCE12;    
		BRAZIL = rData.BRAZIL;
		nSeq=rData.nSeq;
	}
	void Convert520(T_TDMT_UDRD_D_CH_RW_520& rData)
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI.Convert520(rData.ACI);
		CEB.Convert750(rData.CEB);
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA.Initialize();
		COMBINED.Initialize();
		JAPAN.Initialize();
		CHINA.Initialize();     // added by KYM
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE.Initialize();
		CHJTG04.Initialize();
		AASHTO.Initialize();
		AUSTRALIA.Initialize();
		RUSSIA.Initialize();
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert530(T_TDMT_UDRD_D_CH_RW_530& rData)
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI.Convert785(rData.ACI);
		CEB.Convert750(rData.CEB);
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Initialize();
		CHINA.Initialize();     // added by KYM
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE.Initialize();
		CHJTG04.Initialize();
		AASHTO.Initialize();
		AUSTRALIA.Initialize();
		RUSSIA.Initialize();
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert550(T_TDMT_UDRD_D_CH_RW_550& rData)  // added by KYM
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI.Convert785(rData.ACI);
		CEB.Convert750(rData.CEB);
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Convert632(rData.JAPAN);
		CHINA.Initialize();
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE.Initialize();
		CHJTG04.Initialize();
		AASHTO.Initialize();
		AUSTRALIA.Initialize();
		RUSSIA.Initialize();
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert630(T_TDMT_UDRD_D_CH_RW_630& rData)  // added by KYM
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI.Convert785(rData.ACI);
		CEB.Convert750(rData.CEB);
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Convert632(rData.JAPAN);  
		CHINA = rData.CHINA;
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE.Initialize();
		CHJTG04.Initialize();
		AASHTO.Initialize();
		AUSTRALIA.Initialize();
		RUSSIA.Initialize();
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert632(T_TDMT_UDRD_D_CH_RW_632& rData)  // added by KYM
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI.Convert785(rData.ACI);
		CEB.Convert750(rData.CEB);
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Convert632(rData.JAPAN);  
		CHINA = rData.CHINA;
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Initialize();
		AASHTO.Initialize();
		AUSTRALIA.Initialize();
		RUSSIA.Initialize();
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert650(T_TDMT_UDRD_D_CH_RW_650& rData)  // added by KYM
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI.Convert785(rData.ACI);
		CEB.Convert750(rData.CEB);
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Convert900(rData.JAPAN);  
		CHINA = rData.CHINA;
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Initialize();
		AASHTO.Initialize();
		AUSTRALIA.Initialize();
		RUSSIA.Initialize();
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert700(T_TDMT_UDRD_D_CH_RW_700& rData)
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI.Convert785(rData.ACI);
		CEB.Convert750(rData.CEB);
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Convert900(rData.JAPAN);  
		CHINA = rData.CHINA;
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Convert851(rData.CHJTG04);
		AASHTO.Initialize();
		AUSTRALIA.Initialize();
		RUSSIA.Initialize();
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert750(T_TDMT_UDRD_D_CH_RW_750& rData)
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI.Convert785(rData.ACI);
		CEB.Convert750(rData.CEB);
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Convert900(rData.JAPAN);  
		CHINA = rData.CHINA;
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Convert851(rData.CHJTG04);
		AASHTO = rData.AASHTO;
		AUSTRALIA.Initialize();
		RUSSIA.Initialize();
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert785(T_TDMT_UDRD_D_CH_RW_785& rData)
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI.Convert785(rData.ACI);
		CEB.Convert842(rData.CEB);
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Convert900(rData.JAPAN);  
		CHINA = rData.CHINA;
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Convert851(rData.CHJTG04);
		AASHTO = rData.AASHTO;
		AUSTRALIA.Initialize();
		RUSSIA.Initialize();
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert811(T_TDMT_UDRD_D_CH_RW_811& rData)
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI = rData.ACI;
		CEB.Convert842(rData.CEB);
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Convert900(rData.JAPAN);  
		CHINA = rData.CHINA; 
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Convert851(rData.CHJTG04);
		AASHTO = rData.AASHTO;
		AUSTRALIA.Initialize();
		RUSSIA.Initialize();
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert821(T_TDMT_UDRD_D_CH_RW_821& rData)
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI = rData.ACI;
		CEB.Convert842(rData.CEB);
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Convert900(rData.JAPAN);  
		CHINA = rData.CHINA; 
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Convert851(rData.CHJTG04);
		AASHTO = rData.AASHTO;
		AUSTRALIA.Convert883(rData.AUSTRALIA);
		RUSSIA.Initialize();
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert841(T_TDMT_UDRD_D_CH_RW_841& rData)
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI = rData.ACI;
		CEB.Convert842(rData.CEB);
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Convert900(rData.JAPAN);  
		CHINA = rData.CHINA; 
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Convert851(rData.CHJTG04);
		AASHTO = rData.AASHTO;
		AUSTRALIA.Convert883(rData.AUSTRALIA);
		RUSSIA = rData.RUSSIA;
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert851(T_TDMT_UDRD_D_CH_RW_851& rData)
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI = rData.ACI;
		CEB = rData.CEB;
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Convert900(rData.JAPAN);  
		CHINA = rData.CHINA; 
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Convert851(rData.CHJTG04);
		AASHTO = rData.AASHTO;
		AUSTRALIA.Convert883(rData.AUSTRALIA);
		RUSSIA = rData.RUSSIA;
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert855(T_TDMT_UDRD_D_CH_RW_855& rData)
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI = rData.ACI;
		CEB = rData.CEB;
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Convert900(rData.JAPAN);  
		CHINA = rData.CHINA; 
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Convert955(rData.CHJTG04);
		AASHTO = rData.AASHTO;
		AUSTRALIA.Convert883(rData.AUSTRALIA);
		RUSSIA = rData.RUSSIA;
		NEWZEALAND.Initialize();
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert883(T_TDMT_UDRD_D_CH_RW_883& rData)
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI = rData.ACI;
		CEB = rData.CEB;
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Convert900(rData.JAPAN);  
		CHINA = rData.CHINA; 
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Convert955(rData.CHJTG04);
		AASHTO = rData.AASHTO;
		AUSTRALIA.Convert883(rData.AUSTRALIA);
		RUSSIA = rData.RUSSIA;
		NEWZEALAND.Convert950(rData.NEWZEALAND);
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert885(T_TDMT_UDRD_D_CH_RW_885& rData)
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI = rData.ACI;
		CEB = rData.CEB;
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Convert900(rData.JAPAN);  
		CHINA = rData.CHINA; 
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Convert955(rData.CHJTG04);
		AASHTO = rData.AASHTO;
		AUSTRALIA.Convert950(rData.AUSTRALIA);
		RUSSIA = rData.RUSSIA;
		NEWZEALAND.Convert950(rData.NEWZEALAND);
		KDS2016.Initialize();
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert900(T_TDMT_UDRD_D_CH_RW_900& rData)
	{
		key=rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI = rData.ACI;
		CEB = rData.CEB;
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN.Convert900(rData.JAPAN);  
		CHINA = rData.CHINA; 
		nCreepFuncCount=rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Convert955(rData.CHJTG04);
		AASHTO = rData.AASHTO;
		AUSTRALIA.Convert950(rData.AUSTRALIA);
		RUSSIA = rData.RUSSIA;
		NEWZEALAND.Convert950(rData.NEWZEALAND);
		KDS2016 = rData.KDS2016;
		JPJSCE07.Initialize();
		JPJSCE12.Initialize();
		BRAZIL.Initialize();
		nSeq=rData.nSeq;
	}
	void Convert950(T_TDMT_UDRD_D_CH_RW_950& rData)
	{
		key = rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON     = rData.COMMON;
		ACI        = rData.ACI;
		CEB        = rData.CEB;
		ELAST      = rData.ELAST;
		USER       = rData.USER;
		PCA        = rData.PCA;
		COMBINED   = rData.COMBINED;
		JAPAN      = rData.JAPAN;
		CHINA      = rData.CHINA;
		nCreepFuncCount = rData.nCreepFuncCount;
		JPJSCE     = rData.JPJSCE;
		CHJTG04.Convert955(rData.CHJTG04);
		AASHTO     = rData.AASHTO;
		AUSTRALIA.Convert950(rData.AUSTRALIA);
		RUSSIA     = rData.RUSSIA;
		NEWZEALAND.Convert950(rData.NEWZEALAND);
		KDS2016    = rData.KDS2016;
		JPJSCE07   = rData.JPJSCE07;
		JPJSCE12   = rData.JPJSCE12;
		BRAZIL.Initialize();
		nSeq = rData.nSeq;
	}
	void Convert950_2(T_TDMT_UDRD_D_CH_RW_950_2& rData)
	{
		key = rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI = rData.ACI;
		CEB = rData.CEB;
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN = rData.JAPAN;
		CHINA = rData.CHINA;
		nCreepFuncCount = rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Convert955(rData.CHJTG04);
		AASHTO = rData.AASHTO;
		AUSTRALIA = rData.AUSTRALIA;
		RUSSIA = rData.RUSSIA;
		NEWZEALAND.Convert950(rData.NEWZEALAND);
		KDS2016 = rData.KDS2016;
		JPJSCE07 = rData.JPJSCE07;
		JPJSCE12 = rData.JPJSCE12;
		BRAZIL.Initialize();
		nSeq = rData.nSeq;
	}
	void Convert955(T_TDMT_UDRD_D_CH_RW_955& rData)
	{
		key = rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI = rData.ACI;
		CEB = rData.CEB;
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN = rData.JAPAN;
		CHINA = rData.CHINA;
		nCreepFuncCount = rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Convert955(rData.CHJTG04);
		AASHTO = rData.AASHTO;
		AUSTRALIA = rData.AUSTRALIA;
		RUSSIA = rData.RUSSIA;
		NEWZEALAND = rData.NEWZEALAND;
		KDS2016 = rData.KDS2016;
		JPJSCE07 = rData.JPJSCE07;
		JPJSCE12 = rData.JPJSCE12;
		BRAZIL.Initialize();
		nSeq = rData.nSeq;
	}
	void Convert960R1(T_TDMT_UDRD_D_CH_RW_960_R1& rData)
	{
		key = rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI = rData.ACI;
		CEB = rData.CEB;
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN = rData.JAPAN;
		CHINA = rData.CHINA;
		nCreepFuncCount = rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04.Convert955(rData.CHJTG04);
		AASHTO = rData.AASHTO;
		AUSTRALIA = rData.AUSTRALIA;
		RUSSIA = rData.RUSSIA;
		NEWZEALAND = rData.NEWZEALAND;
		KDS2016 = rData.KDS2016;
		JPJSCE07 = rData.JPJSCE07;
		JPJSCE12 = rData.JPJSCE12;
		BRAZIL.Initialize();
		nSeq = rData.nSeq;
	}
	void Convert965(T_TDMT_UDRD_D_CH_RW_965& rData)
	{
		key = rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		COMMON = rData.COMMON;
		ACI = rData.ACI;
		CEB = rData.CEB;
		ELAST = rData.ELAST;
		USER = rData.USER;
		PCA = rData.PCA;
		COMBINED = rData.COMBINED;
		JAPAN = rData.JAPAN;
		CHINA = rData.CHINA;
		nCreepFuncCount = rData.nCreepFuncCount;
		JPJSCE = rData.JPJSCE;
		CHJTG04=rData.CHJTG04;
		AASHTO = rData.AASHTO;
		AUSTRALIA = rData.AUSTRALIA;
		RUSSIA = rData.RUSSIA;
		NEWZEALAND = rData.NEWZEALAND;
		KDS2016 = rData.KDS2016;
		JPJSCE07 = rData.JPJSCE07;
		JPJSCE12 = rData.JPJSCE12;
		BRAZIL.Initialize();
		nSeq = rData.nSeq;
	}
};
struct T_TDMT_UNIT
{
	T_TDMT_UNIT()
	{
		dCompStrength       = D_UNITSYS_BASE_STRESS;
		dVolumeSurfaceRatio = D_UNITSYS_BASE_LENGTH;
		dSlump              = D_UNITSYS_BASE_LENGTH;
		dConcreteAge        = D_UNITSYS_BASE_DAY;
		dCementContent      = D_UNITSYS_BASE_DENSITY;
		dWaterContent       = D_UNITSYS_BASE_DENSITY;
		dMoistCuringPeriod  = D_UNITSYS_BASE_DAY;
		dMemberSize         = D_UNITSYS_BASE_LENGTH;
		dN1                 = D_UNITSYS_BASE_DAY;
		dN2                 = D_UNITSYS_BASE_DAY;
		dModulusOfElasticity= D_UNITSYS_BASE_STRESS;
		dCreep_CreepStrain  = D_UNITSYS_BASE_RSTRESS2;  
		dThickness          = D_UNITSYS_BASE_LENGTH;
		dExposeSurfM        = D_UNITSYS_BASE_PERLENGTH;
		dWater              = D_UNITSYS_BASE_PERVOLUME;
		dMaxSize            = D_UNITSYS_BASE_LENGTH;
		dAir                = D_UNITSYS_BASE_PERVOLUME;
		dEqv_Elast          = D_UNITSYS_BASE_STRESS;
	}

	int dCompStrength;
	int dVolumeSurfaceRatio;
	int dSlump;
	int dConcreteAge;
	int dCementContent;
	int dWaterContent;
	int dMoistCuringPeriod;
	int dMemberSize;
	int dN1;
	int dN2;
	int dModulusOfElasticity;
	int dCreep_CreepStrain;
	int dThickness;
	int dExposeSurfM;
	int dWater;
	int dMaxSize;
	int dAir;
	int dEqv_Elast;
};
#define HASHSIZETDMT 11
#define D_TDMT_MINNUM       1   // 
#define D_TDMT_MAXNUM    9999   // 

struct T_TDME_BASE_650
{
	double dTime;
	double dValue;
	void Initialize()
	{
		dTime=0.0;
		dValue=0.0;
	}
};
struct T_TDME_BASE
{
	double dTime;  
	double dComp;
	double dElast;
	double dTensile;  
	void Initialize()
	{
		dTime = 0.0;
		dComp = 0.0;
		dElast = 0.0;
		dTensile = 0.0;
	}
};
#define T_TDME_DT_650 CArray<T_TDME_BASE_650, T_TDME_BASE_650&>
#define T_TDME_DT CArray<T_TDME_BASE, T_TDME_BASE&>

#define T_TDME_K unsigned int
struct T_TDME_D_630
{
	CString   strMatlName;
	int       nMatlType;      // Code:1, User:2
	double    dScaleFactor;
	T_TDME_DT_650 aFuncData;      // nMatlType = User(1)
	CString   strCode;        // nMatlType = Code(0)  // see CDBLib::GetHmtpCodeID
	double    dCompStrength;  // nMatlType = Code(0)
	double    dFactorA;       // nMatlType = Code(0) Korea Standard, ACI
	double    dFactorB;       // nMatlType = Code(0) Korea Standard, ACI
	int       nCementType;    // nMatlType = Code(0) CEB-FIP, Ohzagi (RS=1, N,R=2, SL=3, Fly-ash=4)
	unsigned int nSeq;        // 순번
};
struct T_TDME_D_650
{
	CString   strMatlName;
	int       nMatlType;      // Code:1, User:2
	double    dScaleFactor;
	T_TDME_DT_650 aFuncData;      // nMatlType = User(1)
	CString   strCode;        // nMatlType = Code(0)  // see CDBLib::GetHmtpCodeID
	double    dCompStrength;  // nMatlType = Code(0)
	double    dFactorA;       // nMatlType = Code(0) Korea Standard, ACI
	double    dFactorB;       // nMatlType = Code(0) Korea Standard, ACI
	int       nCementType;    // nMatlType = Code(0) CEB-FIP, Ohzagi (RS=1, N,R=2, SL=3, Fly-ash=4)
	unsigned int nSeq;        // 순번
	double    dFactorD;       // 6.4.0 추가
	BOOL      bUseConcData;   // 6.4.0 추가
	int       nJpCementType;  // 6.4.0 추가   0=보통 포틀랜드 1=중용열, 2=조강 포틀랜드 
	int       nJpElaCementType; // 6.4.0 추가   0=Normal Type, 1=Rapid Type
	double    dTensileStrengthFactor;  // 6.4.0 추가
};
struct T_TDME_D_821
{
	CString   strMatlName;
	int       nMatlType;      // Code:1, User:2
	double    dScaleFactor;
	T_TDME_DT aFuncData;      // nMatlType = User(1)
	CString   strCode;        // nMatlType = Code(0)  // see CDBLib::GetHmtpCodeID
	double    dCompStrength;  // nMatlType = Code(0)
	double    dFactorA;       // nMatlType = Code(0) Korea Standard, ACI
	double    dFactorB;       // nMatlType = Code(0) Korea Standard, ACI
	int       nCementType;    // nMatlType = Code(0) CEB-FIP, Ohzagi (RS=1, N,R=2, SL=3, Fly-ash=4)
	unsigned int nSeq;        // 순번
	double    dFactorD;       // 6.4.0 추가
	BOOL      bUseConcData;   // 6.4.0 추가
	int       nJpCementType;  // 6.4.0 추가   0=보통 포틀랜드 1=중용열, 2=조강 포틀랜드 
	int       nJpElaCementType; // 6.4.0 추가   0=Normal Type, 1=Rapid Type
	double    dTensileStrengthFactor;  // 6.4.0 추가
};
struct T_TDME_D_840
{
	CString   strMatlName;
	int       nMatlType;      // Code:1, User:2
	double    dScaleFactor;
	T_TDME_DT aFuncData;      // nMatlType = User(1)
	CString   strCode;        // nMatlType = Code(0)  // see CDBLib::GetHmtpCodeID
	double    dCompStrength;  // nMatlType = Code(0)
	double    dFactorA;       // nMatlType = Code(0) Korea Standard, ACI
	double    dFactorB;       // nMatlType = Code(0) Korea Standard, ACI
	int       nCementType;    // nMatlType = Code(0) CEB-FIP, Ohzagi (RS=1, N,R=2, SL=3, Fly-ash=4)
	unsigned int nSeq;        // 순번
	double    dFactorD;       // 6.4.0 추가
	BOOL      bUseConcData;   // 6.4.0 추가
	int       nJpCementType;  // 6.4.0 추가   0=보통 포틀랜드 1=중용열, 2=조강 포틀랜드 
	int       nJpElaCementType; // 6.4.0 추가   0=Normal Type, 1=Rapid Type
	double    dTensileStrengthFactor;  // 6.4.0 추가
	int       nCuringMeth; // 8.2.5 추가 (Russian)
	int       nConcType;   // 8.2.5 추가 (Russian)
	double    dMaxSize;    // 8.2.5 추가 (Russian)
	double    dPz;         // 8.2.5 추가 (Russian)
};
struct T_TDME_D_885
{
	CString   strMatlName;
	int       nMatlType;      // Code:1, User:2
	double    dScaleFactor;
	T_TDME_DT aFuncData;      // nMatlType = User(1)
	CString   strCode;        // nMatlType = Code(0)  // see CDBLib::GetHmtpCodeID
	double    dCompStrength;  // nMatlType = Code(0)
	double    dFactorA;       // nMatlType = Code(0) Korea Standard, ACI
	double    dFactorB;       // nMatlType = Code(0) Korea Standard, ACI
	int       nCementType;    // nMatlType = Code(0) CEB-FIP, Ohzagi (RS=1, N,R=2, SL=3, Fly-ash=4)
	unsigned int nSeq;        // 순번
	double    dFactorD;       // 6.4.0 추가
	BOOL      bUseConcData;   // 6.4.0 추가
	int       nJpCementType;  // 6.4.0 추가   0=보통 포틀랜드 1=중용열, 2=조강 포틀랜드 
	int       nJpElaCementType; // 6.4.0 추가   0=Normal Type, 1=Rapid Type
	double    dTensileStrengthFactor;  // 6.4.0 추가
	int       nCuringMeth; // 8.2.5 추가 (Russian)
	int       nConcType;   // 8.2.5 추가 (Russian)
	double    dMaxSize;    // 8.2.5 추가 (Russian)
	double    dPz;         // 8.2.5 추가 (Russian)
	int       nAggreType;    // nAggreType = CEB-FIP(2010)
};
struct T_TDME_D
{
	CString   strMatlName;
	int       nMatlType;      // Code:1, User:2
	double    dScaleFactor;
	T_TDME_DT aFuncData;      // nMatlType = User(1)
	CString   strCode;        // nMatlType = Code(0)  // see CDBLib::GetHmtpCodeID
	double    dCompStrength;  // nMatlType = Code(0)
	double    dFactorA;       // nMatlType = Code(0) Korea Standard, ACI
	double    dFactorB;       // nMatlType = Code(0) Korea Standard, ACI
	int       nCementType;    // nMatlType = Code(0) CEB-FIP, Ohzagi (RS=1, N,R=2, SL=3, Fly-ash=4)
	unsigned int nSeq;        // 순번
	double    dFactorD;       // 6.4.0 추가
	BOOL      bUseConcData;   // 6.4.0 추가
	int       nJpCementType;  // 6.4.0 추가   0=보통 포틀랜드 1=중용열, 2=조강 포틀랜드 
	int       nJpElaCementType; // 6.4.0 추가   0=Normal Type, 1=Rapid Type
	double    dTensileStrengthFactor;  // 6.4.0 추가
	int       nCuringMeth;    // 8.2.5 추가 (Russian)
	int       nConcType;      // 8.2.5 추가 (Russian)
	double    dMaxSize;       // 8.2.5 추가 (Russian)
	double    dPz;            // 8.2.5 추가 (Russian)
	int       nAggreType;     // nAggreType = CEB-FIP(2010)
	double    dDensity;       // Weight Density (고정단위계라 Unit 처리 안함)

	void Initialize()
	{
		strMatlName.Empty();
		nMatlType = 1;
		dScaleFactor = 1.0;
		aFuncData.RemoveAll();
		strCode.Empty();
		dCompStrength = 0.0;
		dFactorA = 0.0;
		dFactorB = 0.0;
		nCementType = 2;
		nSeq = 0;
		dFactorD = 0.;
		bUseConcData = FALSE;
		nJpCementType = 0;
		nJpElaCementType = 0;
		dTensileStrengthFactor = 0.44;
		nCuringMeth = 0;
		nConcType = 0;
		dMaxSize = 0.0;
		dPz = 0.25;
		nAggreType=0;
		dDensity = 0.0;
	}
	T_TDME_D()
	{
	}
	T_TDME_D(T_TDME_D& src)
	{
		*this = src;
	}
	T_TDME_D& operator=(T_TDME_D& src)
	{
		strMatlName=src.strMatlName;
		nMatlType=src.nMatlType;
		dScaleFactor=src.dScaleFactor;
		aFuncData.Copy(src.aFuncData);
		strCode=src.strCode;
		dCompStrength=src.dCompStrength;
		dFactorA=src.dFactorA;
		dFactorB=src.dFactorB;
		nCementType=src.nCementType;
		nSeq=src.nSeq;
		dFactorD = src.dFactorD;
		bUseConcData = src.bUseConcData;
		nJpCementType = src.nJpCementType;
		nJpElaCementType = src.nJpElaCementType;
		dTensileStrengthFactor = src.dTensileStrengthFactor;
		nCuringMeth = src.nCuringMeth;
		nConcType = src.nConcType;
		dMaxSize = src.dMaxSize;
		dPz = src.dPz;
		nAggreType = src.nAggreType;
		dDensity = src.dDensity;
		return *this;
	}
	void ConvertHmtp(T_HMTP_D& rData)
	{
		ASSERT(rData.nMatlType != 1);
		strMatlName=rData.strMatlName;
		nMatlType=rData.nMatlType-1;
		dScaleFactor=rData.dScaleFactor;
		aFuncData.SetSize(rData.aFuncData.GetSize());
		for (int i = 0; i < rData.aFuncData.GetSize(); i++)
		{
			aFuncData[i].dTime = (rData.aFuncData[i].dTime / 24.0); // hr->day
			aFuncData[i].dElast = rData.aFuncData[i].dValue;
			aFuncData[i].dComp = 0.;
			aFuncData[i].dTensile = 0.;
		}
		strCode=rData.strCode;
		dCompStrength=rData.dCompStrength;
		dFactorA=rData.dFactorA;
		dFactorB=rData.dFactorB;
		nCementType=rData.nCementType;
		nSeq=rData.nSeq;
		dFactorD = 0.;
		bUseConcData = FALSE;
		nJpCementType = 0;   
		nJpElaCementType = 0;
		dTensileStrengthFactor = 0.44;
		nCuringMeth = 0;
		nConcType = 0;
		dMaxSize = 0.0;
		dPz = 0.25;
		nAggreType=0;
		dDensity = 0.0;
	}
};

struct T_TDME_D_CH
{
	char        strMatlName[60];
	int         nMatlType;
	double      dScaleFactor;
	T_TDME_DT   aFuncData;
	char        strCode[60];
	double      dCompStrength;
	double      dFactorA;
	double      dFactorB;
	int         nCementType;
	unsigned int nSeq;
	double    dFactorD;       // 6.4.0 추가
	BOOL      bUseConcData;   // 6.4.0 추가
	int       nJpCementType;  // 6.4.0 추가   0=보통 포틀랜드 1=중용열, 2=조강 포틀랜드 
	int       nJpElaCementType; // 6.4.0 추가   0=Normal Type, 1=Rapid Type
	double    dTensileStrengthFactor;  // 6.4.0 추가
	int       nCuringMeth; // 8.2.5 추가 (Russian)
	int       nConcType;   // 8.2.5 추가 (Russian)
	double    dMaxSize;    // 8.2.5 추가 (Russian)
	double    dPz;         // 8.2.5 추가 (Russian)
	int       nAggreType;
	double    dDensity;

	void ConvertToString(T_TDME_D& rData)
	{
		ConvertCharStr(strMatlName, rData.strMatlName, sizeof(strMatlName));
		rData.nMatlType=nMatlType;
		rData.dScaleFactor=dScaleFactor;
		rData.aFuncData.Copy(aFuncData);
		ConvertCharStr(strCode, rData.strCode, sizeof(strCode));
		rData.dCompStrength=dCompStrength;
		rData.dFactorA=dFactorA;
		rData.dFactorB=dFactorB;
		rData.nCementType=nCementType;
		rData.nSeq=nSeq;
		rData.dFactorD      = dFactorD;
		rData.bUseConcData  = bUseConcData;
		rData.nJpCementType = nJpCementType;
		rData.nJpElaCementType = nJpElaCementType;
		rData.dTensileStrengthFactor = dTensileStrengthFactor;
		rData.nCuringMeth = nCuringMeth;
		rData.nConcType = nConcType;
		rData.dMaxSize = dMaxSize;
		rData.dPz = dPz;
		rData.nAggreType =  nAggreType;
		rData.dDensity = dDensity;
	}
	void ConvertToChar(T_TDME_D& rData)
	{
		ConvertStrChar(rData.strMatlName, strMatlName, sizeof(strMatlName));
		nMatlType=rData.nMatlType;
		dScaleFactor=rData.dScaleFactor;
		aFuncData.Copy(rData.aFuncData);
		ConvertStrChar(rData.strCode, strCode, sizeof(strCode));
		dCompStrength=rData.dCompStrength;
		dFactorA=rData.dFactorA;
		dFactorB=rData.dFactorB;
		nCementType=rData.nCementType;
		nSeq=rData.nSeq;
		dFactorD      = rData.dFactorD;
		bUseConcData  = rData.bUseConcData;
		nJpCementType = rData.nJpCementType;
		nJpElaCementType = rData.nJpElaCementType;
		dTensileStrengthFactor = rData.dTensileStrengthFactor;
		nCuringMeth = rData.nCuringMeth;
		nConcType = rData.nConcType;
		dMaxSize = rData.dMaxSize;
		dPz = rData.dPz;
		nAggreType = rData.nAggreType;
		dDensity = rData.dDensity;
	}
};
struct T_TDME_UDRD_D
{
	T_TDME_K key;
	T_TDME_D data;
};
struct T_TDME_UDRD_D_CH
{
	T_TDME_K key;
	T_TDME_D_CH data;
};
struct T_TDME_UDRD_D_CH_RW_630
{
	T_TDME_K    key;
	char        strMatlName[20];
	int         nMatlType;
	double      dScaleFactor;
	int         nFuncDataCount;
	char        strCode[20];
	double      dCompStrength;
	double      dFactorA;
	double      dFactorB;
	int         nCementType;
	unsigned int nSeq;
};
struct T_TDME_UDRD_D_CH_RW_650
{
	T_TDME_K    key;
	char        strMatlName[20];
	int         nMatlType;
	double      dScaleFactor;
	int         nFuncDataCount;
	char        strCode[20];
	double      dCompStrength;
	double      dFactorA;
	double      dFactorB;
	int         nCementType;
	unsigned int nSeq;
	double      dFactorD;       // 6.4.0 추가
	BOOL        bUseConcData;   // 6.4.0 추가
	int         nJpCementType;  // 6.4.0 추가   0=보통 포틀랜드 1=중용열, 2=조강 포틀랜드 
	int         nJpElaCementType; // 6.4.0 추가   0=Normal Type, 1=Rapid Type
	double      dTensileStrengthFactor;  // 6.4.0 추가
};
struct T_TDME_UDRD_D_CH_RW_821
{
	T_TDME_K    key;
	char        strMatlName[20];
	int         nMatlType;
	double      dScaleFactor;
	int         nFuncDataCount;
	char        strCode[20];
	double      dCompStrength;
	double      dFactorA;
	double      dFactorB;
	int         nCementType;
	unsigned int nSeq;
	double      dFactorD;       // 6.4.0 추가
	BOOL        bUseConcData;   // 6.4.0 추가
	int         nJpCementType;  // 6.4.0 추가   0=보통 포틀랜드 1=중용열, 2=조강 포틀랜드 
	int         nJpElaCementType; // 6.4.0 추가   0=Normal Type, 1=Rapid Type
	double      dTensileStrengthFactor;  // 6.4.0 추가
	double      dTemp1;          // 6.6.0추가 Data Size 맞추기 위한 임시 변수
	double      dTemp2;          // 6.6.0추가 Data Size 맞추기 위한 임시 변수
};
struct T_TDME_UDRD_D_CH_RW_840
{
	T_TDME_K    key;
	char        strMatlName[20];
	int         nMatlType;
	double      dScaleFactor;
	int         nFuncDataCount;
	char        strCode[20];
	double      dCompStrength;
	double      dFactorA;
	double      dFactorB;
	int         nCementType;
	unsigned int nSeq;
	double      dFactorD;       // 6.4.0 추가
	BOOL        bUseConcData;   // 6.4.0 추가
	int         nJpCementType;  // 6.4.0 추가   0=보통 포틀랜드 1=중용열, 2=조강 포틀랜드 
	int         nJpElaCementType; // 6.4.0 추가   0=Normal Type, 1=Rapid Type
	double      dTensileStrengthFactor;  // 6.4.0 추가
	double      dTemp1;          // 6.6.0추가 Data Size 맞추기 위한 임시 변수
	double      dTemp2;          // 6.6.0추가 Data Size 맞추기 위한 임시 변수
	int         nCuringMeth; // 8.2.5 추가 (Russian)
	int         nConcType;   // 8.2.5 추가 (Russian)
	double      dMaxSize;    // 8.2.5 추가 (Russian)
	double      dPz;         // 8.2.5 추가 (Russian)
};
struct T_TDME_UDRD_D_CH_RW_885
{
	T_TDME_K    key;
	char        strMatlName[20];
	int         nMatlType;
	double      dScaleFactor;
	int         nFuncDataCount;
	char        strCode[20];
	double      dCompStrength;
	double      dFactorA;
	double      dFactorB;
	int         nCementType;
	unsigned int nSeq;
	double      dFactorD;       // 6.4.0 추가
	BOOL        bUseConcData;   // 6.4.0 추가
	int         nJpCementType;  // 6.4.0 추가   0=보통 포틀랜드 1=중용열, 2=조강 포틀랜드 
	int         nJpElaCementType; // 6.4.0 추가   0=Normal Type, 1=Rapid Type
	double      dTensileStrengthFactor;  // 6.4.0 추가
	double      dTemp1;          // 6.6.0추가 Data Size 맞추기 위한 임시 변수
	double      dTemp2;          // 6.6.0추가 Data Size 맞추기 위한 임시 변수
	int         nCuringMeth; // 8.2.5 추가 (Russian)
	int         nConcType;   // 8.2.5 추가 (Russian)
	double      dMaxSize;    // 8.2.5 추가 (Russian)
	double      dPz;         // 8.2.5 추가 (Russian)
	int         nAggreType;
};
struct T_TDME_UDRD_D_CH_RW_950
{
	T_TDME_K    key;
	char        strMatlName[20];
	int         nMatlType;
	double      dScaleFactor;
	int         nFuncDataCount;
	char        strCode[20];
	double      dCompStrength;
	double      dFactorA;
	double      dFactorB;
	int         nCementType;
	unsigned int nSeq;
	double      dFactorD;       // 6.4.0 추가
	BOOL        bUseConcData;   // 6.4.0 추가
	int         nJpCementType;  // 6.4.0 추가   0=보통 포틀랜드 1=중용열, 2=조강 포틀랜드 
	int         nJpElaCementType; // 6.4.0 추가   0=Normal Type, 1=Rapid Type
	double      dTensileStrengthFactor;  // 6.4.0 추가
	double      dTemp1;          // 6.6.0추가 Data Size 맞추기 위한 임시 변수
	double      dTemp2;          // 6.6.0추가 Data Size 맞추기 위한 임시 변수
	int         nCuringMeth; // 8.2.5 추가 (Russian)
	int         nConcType;   // 8.2.5 추가 (Russian)
	double      dMaxSize;    // 8.2.5 추가 (Russian)
	double      dPz;         // 8.2.5 추가 (Russian)
	int         nAggreType;
	double      dDensity;
};

struct T_TDME_UDRD_D_CH_RW
{
	T_TDME_K    key;
	char        strMatlName[60]; // (v955) Size 변경
	int         nMatlType;
	double      dScaleFactor;
	int         nFuncDataCount;
	char        strCode[60];     // (v955) Size 변경
	double      dCompStrength;
	double      dFactorA;
	double      dFactorB;
	int         nCementType;
	unsigned int nSeq;
	double      dFactorD;       // 6.4.0 추가
	BOOL        bUseConcData;   // 6.4.0 추가
	int         nJpCementType;  // 6.4.0 추가   0=보통 포틀랜드 1=중용열, 2=조강 포틀랜드 
	int         nJpElaCementType; // 6.4.0 추가   0=Normal Type, 1=Rapid Type
	double      dTensileStrengthFactor;  // 6.4.0 추가
	double      dTemp1;          // 6.6.0추가 Data Size 맞추기 위한 임시 변수
	double      dTemp2;          // 6.6.0추가 Data Size 맞추기 위한 임시 변수
	int         nCuringMeth; // 8.2.5 추가 (Russian)
	int         nConcType;   // 8.2.5 추가 (Russian)
	double      dMaxSize;    // 8.2.5 추가 (Russian)
	double      dPz;         // 8.2.5 추가 (Russian)
	int         nAggreType;
	double      dDensity;

	void GetTdme(T_TDME_K& rKey, T_TDME_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strMatlName, strMatlName, sizeof(rData.strMatlName));
		rData.nMatlType=nMatlType;
		rData.dScaleFactor=dScaleFactor;
		memcpy(rData.strCode, strCode, sizeof(rData.strCode));
		rData.dCompStrength=dCompStrength;
		rData.dFactorA=dFactorA;
		rData.dFactorB=dFactorB;
		rData.nCementType=nCementType;
		rData.nSeq=nSeq;
		rData.dFactorD     = dFactorD;
		rData.bUseConcData = bUseConcData;
		rData.nJpCementType  = nJpCementType; 
		rData.nJpElaCementType = nJpElaCementType;
		rData.dTensileStrengthFactor = dTensileStrengthFactor;    
		rData.nCuringMeth = nCuringMeth;    
		rData.nConcType = nConcType;    
		rData.dMaxSize = dMaxSize;    
		rData.dPz = dPz;    
		rData.nAggreType = nAggreType;
		rData.dDensity = dDensity;
	}
	void SetTdme(const T_TDME_K Key, const T_TDME_D_CH& rData)
	{
		key=Key;
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		nMatlType=rData.nMatlType;
		dScaleFactor=rData.dScaleFactor;
		nFuncDataCount=(int)rData.aFuncData.GetSize();
		memcpy(strCode, rData.strCode, sizeof(rData.strCode));
		dCompStrength=rData.dCompStrength;
		dFactorA=rData.dFactorA;
		dFactorB=rData.dFactorB;
		nCementType=rData.nCementType;
		nSeq=rData.nSeq;
		dFactorD      = rData.dFactorD;
		bUseConcData  = rData.bUseConcData;
		nJpCementType = rData.nJpCementType;  
		nJpElaCementType = rData.nJpElaCementType;
		dTensileStrengthFactor = rData.dTensileStrengthFactor;
		dTemp1 = 0;
		dTemp2 = 0;
		nCuringMeth = rData.nCuringMeth;
		nConcType = rData.nConcType;
		dMaxSize = rData.dMaxSize;
		dPz = rData.dPz;
		nAggreType = rData.nAggreType;
		dDensity = rData.dDensity;
	}
	void Convert630(T_TDME_UDRD_D_CH_RW_630& rData)
	{
		key = rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));   
		nMatlType=rData.nMatlType;
		dScaleFactor=rData.dScaleFactor;
		nFuncDataCount = rData.nFuncDataCount;
		memset(strCode, 0, sizeof(strCode)); // 초기화
		memcpy(strCode, rData.strCode, sizeof(rData.strCode));            
		dCompStrength=rData.dCompStrength;
		dFactorA=rData.dFactorA;
		dFactorB=rData.dFactorB;
		nCementType=rData.nCementType;
		nSeq=rData.nSeq;
		dFactorD = 0.;
		bUseConcData = FALSE;
		nJpCementType = 0;   
		nJpElaCementType = 0;
		dTensileStrengthFactor = 0.44;
		dTemp1 = 0;
		dTemp2 = 0;
		nCuringMeth = 0;
		nConcType = 0;
		dMaxSize = 0.0;
		dPz = 0.25;
		nAggreType=0;
		dDensity = 0.0;
	}
	void Convert650(T_TDME_UDRD_D_CH_RW_650& rData)
	{
		key = rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));   
		nMatlType=rData.nMatlType;
		dScaleFactor=rData.dScaleFactor;
		nFuncDataCount = rData.nFuncDataCount;
		memset(strCode, 0, sizeof(strCode)); // 초기화
		memcpy(strCode, rData.strCode, sizeof(rData.strCode));            
		dCompStrength=rData.dCompStrength;
		dFactorA=rData.dFactorA;
		dFactorB=rData.dFactorB;
		nCementType=rData.nCementType;
		nSeq=rData.nSeq;
		dFactorD = rData.dFactorD;
		bUseConcData = rData.bUseConcData;
		nJpCementType = rData.nJpCementType;   
		nJpElaCementType = rData.nJpElaCementType;
		dTensileStrengthFactor = rData.dTensileStrengthFactor;
		dTemp1 = 0;
		dTemp2 = 0;
		nCuringMeth = 0;
		nConcType = 0;
		dMaxSize = 0.0;
		dPz = 0.25;
		nAggreType=0;
		dDensity = 0.0;
	}
	void Convert821(T_TDME_UDRD_D_CH_RW_821& rData)
	{
		key = rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));   
		nMatlType=rData.nMatlType;
		dScaleFactor=rData.dScaleFactor;
		nFuncDataCount = rData.nFuncDataCount;
		memset(strCode, 0, sizeof(strCode)); // 초기화
		memcpy(strCode, rData.strCode, sizeof(rData.strCode));            
		dCompStrength=rData.dCompStrength;
		dFactorA=rData.dFactorA;
		dFactorB=rData.dFactorB;
		nCementType=rData.nCementType;
		nSeq=rData.nSeq;
		dFactorD = rData.dFactorD;
		bUseConcData = rData.bUseConcData;
		nJpCementType = rData.nJpCementType;   
		nJpElaCementType = rData.nJpElaCementType;
		dTensileStrengthFactor = rData.dTensileStrengthFactor;
		dTemp1 = rData.dTemp1;
		dTemp2 = rData.dTemp2;
		nCuringMeth = 0;
		nConcType = 0;
		dMaxSize = 0.0;
		dPz = 0.25;
		nAggreType=0;
		dDensity = 0.0;
	}
	void Convert840(T_TDME_UDRD_D_CH_RW_840& rData)
	{
		key = rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));   
		nMatlType=rData.nMatlType;
		dScaleFactor=rData.dScaleFactor;
		nFuncDataCount = rData.nFuncDataCount;
		memset(strCode, 0, sizeof(strCode)); // 초기화
		memcpy(strCode, rData.strCode, sizeof(rData.strCode));            
		dCompStrength=rData.dCompStrength;
		dFactorA=rData.dFactorA;
		dFactorB=rData.dFactorB;
		nCementType=rData.nCementType;
		nSeq=rData.nSeq;
		dFactorD = rData.dFactorD;
		bUseConcData = rData.bUseConcData;
		nJpCementType = rData.nJpCementType;   
		nJpElaCementType = rData.nJpElaCementType;
		dTensileStrengthFactor = rData.dTensileStrengthFactor;
		dTemp1 = rData.dTemp1;
		dTemp2 = rData.dTemp2;
		nCuringMeth = rData.nCuringMeth;
		nConcType = rData.nConcType;
		dMaxSize = rData.dMaxSize;
		dPz = rData.dPz;
		nAggreType = 0;
		dDensity = 0.0;
	}
	void Convert885(T_TDME_UDRD_D_CH_RW_885& rData)
	{
		key = rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));   
		nMatlType=rData.nMatlType;
		dScaleFactor=rData.dScaleFactor;
		nFuncDataCount = rData.nFuncDataCount;
		memset(strCode, 0, sizeof(strCode)); // 초기화
		memcpy(strCode, rData.strCode, sizeof(rData.strCode));            
		dCompStrength=rData.dCompStrength;
		dFactorA=rData.dFactorA;
		dFactorB=rData.dFactorB;
		nCementType=rData.nCementType;
		nSeq=rData.nSeq;
		dFactorD = rData.dFactorD;
		bUseConcData = rData.bUseConcData;
		nJpCementType = rData.nJpCementType;   
		nJpElaCementType = rData.nJpElaCementType;
		dTensileStrengthFactor = rData.dTensileStrengthFactor;
		dTemp1 = rData.dTemp1;
		dTemp2 = rData.dTemp2;
		nCuringMeth = rData.nCuringMeth;
		nConcType = rData.nConcType;
		dMaxSize = rData.dMaxSize;
		dPz = rData.dPz;
		nAggreType = rData.nAggreType;
		dDensity = 0.0;
	}
	void Convert950(T_TDME_UDRD_D_CH_RW_950& rData)
	{
		key = rData.key;
		memset(strMatlName, 0, sizeof(strMatlName)); // 초기화
		memcpy(strMatlName, rData.strMatlName, sizeof(rData.strMatlName));
		nMatlType      = rData.nMatlType;
		dScaleFactor   = rData.dScaleFactor;
		nFuncDataCount = rData.nFuncDataCount;
		memset(strCode, 0, sizeof(strCode)); // 초기화
		memcpy(strCode, rData.strCode, sizeof(rData.strCode));
		dCompStrength  = rData.dCompStrength;
		dFactorA       = rData.dFactorA;
		dFactorB       = rData.dFactorB;
		nCementType    = rData.nCementType;
		nSeq           = rData.nSeq;
		dFactorD       = rData.dFactorD;
		bUseConcData   = rData.bUseConcData;
		nJpCementType  = rData.nJpCementType;
		nJpElaCementType = rData.nJpElaCementType;
		dTensileStrengthFactor = rData.dTensileStrengthFactor;
		dTemp1      = rData.dTemp1;
		dTemp2      = rData.dTemp2;
		nCuringMeth = rData.nCuringMeth;
		nConcType   = rData.nConcType;
		dMaxSize    = rData.dMaxSize;
		dPz         = rData.dPz;
		nAggreType  = rData.nAggreType;
		dDensity    = rData.dDensity;
	}
};
struct T_TDME_UNIT
{
	T_TDME_UNIT()
	{
		dTime=D_UNITSYS_BASE_DAY;
		dValue=D_UNITSYS_BASE_ELAST;
		dCompStrength=D_UNITSYS_BASE_STRESS;
		dMaxSize=D_UNITSYS_BASE_LENGTH;
	}
	int dTime;
	int dValue;
	int dCompStrength;
	int dMaxSize;
};
#define HASHSIZETDME 11
#define D_TDME_MINNUM       1   // 
#define D_TDME_MAXNUM    9999   // 

#define T_TMAT_K T_MATL_K
struct T_TMAT_D
{
	T_TDMT_K TdMatlTypeKey;
	T_TDME_K TdMatlElastKey;
	void Initialize()
	{
		TdMatlTypeKey=0;
		TdMatlElastKey=0;
	}
};
struct T_TMAT_UDRD_D
{
	T_TMAT_K key;
	T_TMAT_D data;
};
struct T_TMAT_UNIT
{
	T_TMAT_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZETMAT HASHSIZEMATL

// Tendon Type
// #define T_TDNT_K unsigned int
// 
// enum EN_TDNT_RELAX
// {
//   TDNT_RELAX_MAGURA                 = 0,	// Magura
//   TDNT_RELAX_CEB_FIP                ,	    // CEB-FIP
//   TDNT_RELAX_JTG04                  ,	    // JTG04
//   TDNT_RELAX_TB05                   ,	    // TB05
//   TDNT_RELAX_INDIA_IRC18_2000       ,	    // INDIA(IRC18-2000)
//   TDNT_RELAX_EUROPEAN               ,	    // European
//   TDNT_RELAX_KSCE_LSD15             ,	    // KSCE-LSD15
//   TDNT_RELAX_INDIA_IRC112_2011      ,	    // INDIA(IRC:112-2011)
//   TDNT_RELAX_USER_DEFINED           ,	    // User Defined
// };
// 
// struct T_TDNT_D
// {
//   CString   TendonTypeName;
//   int       nTendonType;          // 1:Internal, 2:External
//   T_MATL_K  MatlKey;
//   double    dTendonArea;
//   double    dDuctArea;            // Internal-Pre인 경우는 StrandArea로 사용됨
//   double    dRelaxCoef;
//   double    dFrictionFactor;
//   double    dWobbleFactor;
//   double    dUltimateStrength;
//   double    dYieldStrength;
//   int       nLoadType;      // 0=Pre-tension, 1=Post-tension  
//   double    dAnchoSlipBegin;
//   double    dAnchoSlipEnd;
//   unsigned int nTypeId;     // 순번
//   int       nBondType;      // 0: Bonded, 1: Unbonded   // 5.7.0 추가
//   double    dAlphaExtCable; // External 텐던 파괴저항 모멘트 증가계수 (6.4.0 추가)
//   int       nRelaxMethod;   // TDNT_RELAX_XXXX
//                             // 0=Magura, 1=CEB-FIP, 2=JTG04 : 6.4.0 추가 , JTG04는 6.6.0추가
//                             // 3=TB05, 4=INDIA(IRC:18-2000) 7.5.0 추가
//                             // 5=European : 7.6.0 추가
//   double    dRelaxRatio;    // (6.4.0 추가)
//   BOOL      bOverStressRedFactor;  // 6.6.0추가 중국 버전에서만 사용
//   double    dJTG04Factor;   // 6.7.0추가  : 1.0 or 0.3
//   double    dFpk;           // 6.7.0추가 
//   double    dInadvertentChange;//7.1.0추가
//   void Initialize()
//   {
//     TendonTypeName.Empty();
//     nTendonType = 1;
//     MatlKey = 0;
//     dTendonArea = 0.0;
//     dDuctArea = 0.0;
//     dRelaxCoef = 45.0;
//     dFrictionFactor = 0.3;
//     dWobbleFactor = 0.0066;
//     dUltimateStrength = 0.0;
//     dYieldStrength = 0.0;
//     nLoadType = 1;          // post    
//     dAnchoSlipBegin = 0.0;
//     dAnchoSlipEnd = 0.0;
//     nTypeId=0;
//     nBondType = 0;
//     dAlphaExtCable = 0.0;
//     #if defined(_CH)
//     nRelaxMethod = TDNT_RELAX_JTG04; // M1496-SJB-050624
//     #else
//     nRelaxMethod = TDNT_RELAX_MAGURA;
//     #endif
//     dRelaxRatio = 0.0;
//     bOverStressRedFactor = FALSE;
//     dJTG04Factor = 1.0;
//     dFpk = 0.;
//     dInadvertentChange=0.0;
//   }
// };
// struct T_TDNT_D_702
// {
//   CString   TendonTypeName;
//   int       nTendonType;          // 1:Internal, 2:External
//   T_MATL_K  MatlKey;
//   double    dTendonArea;
//   double    dDuctArea;            // Internal-Pre인 경우는 StrandArea로 사용됨
//   double    dRelaxCoef;
//   double    dFrictionFactor;
//   double    dWobbleFactor;
//   double    dUltimateStrength;
//   double    dYieldStrength;
//   int       nLoadType;  // 0=Pre-tension, 1=Post-tension  
//   double    dAnchoSlipBegin;
//   double    dAnchoSlipEnd;
//   unsigned int nTypeId;    // 순번
//   int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
//   double    dAlphaExtCable; // External 텐던 파괴저항 모멘트 증가계수 (6.4.0 추가)
//   int       nRelaxMethod;   // 0=Magura, 1=CEB-FIP, 2=JTG04 : 6.4.0 추가 , JTG04는 6.6.0추가
//   double    dRelaxRatio;    // (6.4.0 추가)
//   BOOL      bOverStressRedFactor;  // 6.6.0추가 중국 버전에서만 사용
//   double    dJTG04Factor;   // 6.7.0추가  : 1.0 or 0.3
//   double    dFpk;           // 6.7.0추가 
// };
// struct T_TDNT_D_660
// {
//   CString   TendonTypeName;
//   int       nTendonType;          // 1:Internal, 2:External
//   T_MATL_K  MatlKey;
//   double    dTendonArea;
//   double    dDuctArea;            // Internal-Pre인 경우는 StrandArea로 사용됨
//   double    dRelaxCoef;
//   double    dFrictionFactor;
//   double    dWobbleFactor;
//   double    dUltimateStrength;
//   double    dYieldStrength;
//   int       nLoadType;  // 0=Pre-tension, 1=Post-tension  
//   double    dAnchoSlipBegin;
//   double    dAnchoSlipEnd;
//   unsigned int nTypeId;    // 순번
//   int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
//   double    dAlphaExtCable; // External 텐던 파괴저항 모멘트 증가계수 (6.4.0 추가)
//   int       nRelaxMethod;   // 0:Method1(기존방법), 1:Method2, 2:Method3 (이름 미결정) (6.4.0 추가)
//   double    dRelaxRatio;    // (6.4.0 추가)
//   BOOL      bOverStressRedFactor;  // 6.6.0추가 중국 버전에서만 사용
// };
// struct T_TDNT_D_CH_560
// {
//   char      TendonTypeName[20];
//   int       nTendonType;          // 1:Internal, 2:External
//   T_MATL_K  MatlKey;
//   double    dTendonArea;
//   double    dDuctArea;
//   double    dRelaxCoef;
//   double    dFrictionFactor;
//   double    dWobbleFactor;
//   double    dUltimateStrength;
//   double    dYieldStrength;
//   int       nLoadType;  // 0=Prestress, 1=Post-stress  
//   double    dAnchoSlipBegin;
//   double    dAnchoSlipEnd;
//   unsigned int nTypeId;    // 순번
// };
// struct T_TDNT_D_CH_630
// {
//   char      TendonTypeName[20];
//   int       nTendonType;          // 1:Internal, 2:External
//   T_MATL_K  MatlKey;
//   double    dTendonArea;
//   double    dDuctArea;
//   double    dRelaxCoef;
//   double    dFrictionFactor;
//   double    dWobbleFactor;
//   double    dUltimateStrength;
//   double    dYieldStrength;
//   int       nLoadType;  // 0=Prestress, 1=Post-stress  
//   double    dAnchoSlipBegin;
//   double    dAnchoSlipEnd;
//   unsigned int nTypeId;    // 순번
//   int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
// };
// struct T_TDNT_D_CH_650
// {
//   char      TendonTypeName[20];
//   int       nTendonType;          // 1:Internal, 2:External
//   T_MATL_K  MatlKey;
//   double    dTendonArea;
//   double    dDuctArea;
//   double    dRelaxCoef;
//   double    dFrictionFactor;
//   double    dWobbleFactor;
//   double    dUltimateStrength;
//   double    dYieldStrength;
//   int       nLoadType;  // 0=Prestress, 1=Post-stress  
//   double    dAnchoSlipBegin;
//   double    dAnchoSlipEnd;
//   unsigned int nTypeId;    // 순번
//   int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
//   double    dAlphaExtCable; // (6.4.0 추가)
//   int       nRelaxMethod;   // (6.4.0 추가)
//   double    dRelaxRatio;    // (6.4.0 추가)
// };
// struct T_TDNT_D_CH_660
// {
//   char      TendonTypeName[20];
//   int       nTendonType;          // 1:Internal, 2:External
//   T_MATL_K  MatlKey;
//   double    dTendonArea;
//   double    dDuctArea;
//   double    dRelaxCoef;
//   double    dFrictionFactor;
//   double    dWobbleFactor;
//   double    dUltimateStrength;
//   double    dYieldStrength;
//   int       nLoadType;  // 0=Prestress, 1=Post-stress  
//   double    dAnchoSlipBegin;
//   double    dAnchoSlipEnd;
//   unsigned int nTypeId;    // 순번
//   int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
//   double    dAlphaExtCable; // (6.4.0 추가)
//   int       nRelaxMethod;   // (6.4.0 추가)
//   double    dRelaxRatio;    // (6.4.0 추가)
//   BOOL      bOverStressRedFactor;  // 6.6.0추가 중국 버전에서만 사용
// };
// struct T_TDNT_D_CH_702
// {
//   char      TendonTypeName[20];
//   int       nTendonType;          // 1:Internal, 2:External
//   T_MATL_K  MatlKey;
//   double    dTendonArea;
//   double    dDuctArea;
//   double    dRelaxCoef;
//   double    dFrictionFactor;
//   double    dWobbleFactor;
//   double    dUltimateStrength;
//   double    dYieldStrength;
//   int       nLoadType;  // 0=Prestress, 1=Post-stress  
//   double    dAnchoSlipBegin;
//   double    dAnchoSlipEnd;
//   unsigned int nTypeId;    // 순번
//   int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
//   double    dAlphaExtCable; // (6.4.0 추가)
//   int       nRelaxMethod;   // (6.4.0 추가)
//   double    dRelaxRatio;    // (6.4.0 추가)
//   BOOL      bOverStressRedFactor;  // 6.6.0추가 중국 버전에서만 사용
//   double    dJTG04Factor;   // 6.7.0추가  : 1.0 or 0.3
//   double    dFpk;           // 6.7.0추가 
// };
// struct T_TDNT_D_CH
// {
//   char      TendonTypeName[20];
//   int       nTendonType;          // 1:Internal, 2:External
//   T_MATL_K  MatlKey;
//   double    dTendonArea;
//   double    dDuctArea;
//   double    dRelaxCoef;
//   double    dFrictionFactor;
//   double    dWobbleFactor;
//   double    dUltimateStrength;
//   double    dYieldStrength;
//   int       nLoadType;  // 0=Prestress, 1=Post-stress  
//   double    dAnchoSlipBegin;
//   double    dAnchoSlipEnd;
//   unsigned int nTypeId;    // 순번
//   int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
//   double    dAlphaExtCable; // (6.4.0 추가)
//   int       nRelaxMethod;   // (6.4.0 추가)
//   double    dRelaxRatio;    // (6.4.0 추가)
//   BOOL      bOverStressRedFactor;  // 6.6.0추가 중국 버전에서만 사용
//   double    dJTG04Factor;   // 6.7.0추가  : 1.0 or 0.3
//   double    dFpk;           // 6.7.0추가 
//   double    dInadvertentChange;  //7.1.0추가
//   void ConvertToString(T_TDNT_D& rData)
//   {
//     ConvertCharStr(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));
//     rData.nTendonType = nTendonType;
//     rData.MatlKey = MatlKey;
//     rData.dTendonArea = dTendonArea;
//     rData.dDuctArea = dDuctArea;
//     rData.dRelaxCoef = dRelaxCoef;
//     rData.dFrictionFactor = dFrictionFactor;
//     rData.dWobbleFactor = dWobbleFactor;
//     rData.dUltimateStrength = dUltimateStrength;
//     rData.dYieldStrength = dYieldStrength;
//     rData.nLoadType = nLoadType;    
//     rData.dAnchoSlipBegin = dAnchoSlipBegin;
//     rData.dAnchoSlipEnd = dAnchoSlipEnd;
//     rData.nTypeId=nTypeId;
//     rData.nBondType = nBondType;
//     rData.dAlphaExtCable = dAlphaExtCable;
//     rData.nRelaxMethod   = nRelaxMethod;
//     rData.dRelaxRatio    = dRelaxRatio;
//     rData.bOverStressRedFactor = bOverStressRedFactor;
//     rData.dJTG04Factor = dJTG04Factor;
//     rData.dFpk = dFpk;
//     rData.dInadvertentChange=dInadvertentChange;
//   }
//   void ConvertToChar(T_TDNT_D& rData)
//   {
//     ConvertStrChar(rData.TendonTypeName, TendonTypeName, sizeof(TendonTypeName));
//     nTendonType = rData.nTendonType;
//     MatlKey = rData.MatlKey;
//     dTendonArea = rData.dTendonArea;
//     dDuctArea = rData.dDuctArea;
//     dRelaxCoef = rData.dRelaxCoef;
//     dFrictionFactor = rData.dFrictionFactor;
//     dWobbleFactor = rData.dWobbleFactor;
//     dUltimateStrength = rData.dUltimateStrength;
//     dYieldStrength = rData.dYieldStrength;
//     nLoadType = rData.nLoadType;    
//     dAnchoSlipBegin = rData.dAnchoSlipBegin;
//     dAnchoSlipEnd = rData.dAnchoSlipEnd;
//     nTypeId = rData.nTypeId;
//     nBondType = rData.nBondType;
//     dAlphaExtCable = rData.dAlphaExtCable;
//     nRelaxMethod   = rData.nRelaxMethod;
//     dRelaxRatio    = rData.dRelaxRatio;
//     bOverStressRedFactor = rData.bOverStressRedFactor;
//     dJTG04Factor = rData.dJTG04Factor;
//     dFpk = rData.dFpk;
//     dInadvertentChange=rData.dInadvertentChange;
//   }
//   void Convert560(T_TDNT_D_CH_560& rData)
//   {
//     memcpy(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));    
//     nTendonType = rData.nTendonType;
//     MatlKey = rData.MatlKey;
//     dTendonArea = rData.dTendonArea;
//     dDuctArea = rData.dDuctArea;
//     dRelaxCoef = rData.dRelaxCoef;
//     dFrictionFactor = rData.dFrictionFactor;
//     dWobbleFactor = rData.dWobbleFactor;
//     dUltimateStrength = rData.dUltimateStrength;
//     dYieldStrength = rData.dYieldStrength;
//     nLoadType = rData.nLoadType;    
//     dAnchoSlipBegin = rData.dAnchoSlipBegin;
//     dAnchoSlipEnd = rData.dAnchoSlipEnd;
//     nTypeId = rData.nTypeId;
//     nBondType = 0;    
//     dAlphaExtCable = 0.0;
//     nRelaxMethod   = 0;
//     dRelaxRatio    = 0.0;
//     bOverStressRedFactor = FALSE;
//     dJTG04Factor = 1.0;
//     dFpk = 0.;
//     dInadvertentChange=0.0;
// 
//   }
//   void Convert630(T_TDNT_D_CH_630& rData)
//   {
//     memcpy(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));
//     nTendonType = rData.nTendonType;
//     MatlKey = rData.MatlKey;
//     dTendonArea = rData.dTendonArea;
//     dDuctArea = rData.dDuctArea;
//     dRelaxCoef = rData.dRelaxCoef;
//     dFrictionFactor = rData.dFrictionFactor;
//     dWobbleFactor = rData.dWobbleFactor;
//     dUltimateStrength = rData.dUltimateStrength;
//     dYieldStrength = rData.dYieldStrength;
//     nLoadType = rData.nLoadType;    
//     dAnchoSlipBegin = rData.dAnchoSlipBegin;
//     dAnchoSlipEnd = rData.dAnchoSlipEnd;
//     nTypeId = rData.nTypeId;
//     nBondType = rData.nBondType;    
//     dAlphaExtCable = 0.0;
//     nRelaxMethod   = 0;
//     dRelaxRatio    = 0.0;
//     bOverStressRedFactor = FALSE;
//     dJTG04Factor = 1.0;
//     dFpk = 0.;
//     dInadvertentChange=0.0;
//   }
//   void Convert650(T_TDNT_D_CH_650& rData)
//   {
//     memcpy(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));
//     nTendonType = rData.nTendonType;
//     MatlKey = rData.MatlKey;
//     dTendonArea = rData.dTendonArea;
//     dDuctArea = rData.dDuctArea;
//     dRelaxCoef = rData.dRelaxCoef;
//     dFrictionFactor = rData.dFrictionFactor;
//     dWobbleFactor = rData.dWobbleFactor;
//     dUltimateStrength = rData.dUltimateStrength;
//     dYieldStrength = rData.dYieldStrength;
//     nLoadType = rData.nLoadType;    
//     dAnchoSlipBegin = rData.dAnchoSlipBegin;
//     dAnchoSlipEnd = rData.dAnchoSlipEnd;
//     nTypeId = rData.nTypeId;
//     nBondType = rData.nBondType;    
//     dAlphaExtCable = rData.dAlphaExtCable;
//     nRelaxMethod   = rData.nRelaxMethod;
//     dRelaxRatio    = rData.dRelaxRatio;
//     bOverStressRedFactor = FALSE;
//     dJTG04Factor = 1.0;
//     dFpk = 0.;
//     dInadvertentChange=0.0;
//   }
//   void Convert660(T_TDNT_D_CH_660& rData)
//   {
//     memcpy(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));
//     nTendonType = rData.nTendonType;
//     MatlKey = rData.MatlKey;
//     dTendonArea = rData.dTendonArea;
//     dDuctArea = rData.dDuctArea;
//     dRelaxCoef = rData.dRelaxCoef;
//     dFrictionFactor = rData.dFrictionFactor;
//     dWobbleFactor = rData.dWobbleFactor;
//     dUltimateStrength = rData.dUltimateStrength;
//     dYieldStrength = rData.dYieldStrength;
//     nLoadType = rData.nLoadType;    
//     dAnchoSlipBegin = rData.dAnchoSlipBegin;
//     dAnchoSlipEnd = rData.dAnchoSlipEnd;
//     nTypeId = rData.nTypeId;
//     nBondType = rData.nBondType;    
//     dAlphaExtCable = rData.dAlphaExtCable;
//     nRelaxMethod   = rData.nRelaxMethod;
//     dRelaxRatio    = rData.dRelaxRatio;
//     bOverStressRedFactor = rData.bOverStressRedFactor;
//     dJTG04Factor = 1.0;
//     dFpk = 0.;
//     dInadvertentChange=0.0;
//   }
//   void Convert702(T_TDNT_D_CH_702& rData)
//   {
//     memcpy(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));
//     nTendonType = rData.nTendonType;
//     MatlKey = rData.MatlKey;
//     dTendonArea = rData.dTendonArea;
//     dDuctArea = rData.dDuctArea;
//     dRelaxCoef = rData.dRelaxCoef;
//     dFrictionFactor = rData.dFrictionFactor;
//     dWobbleFactor = rData.dWobbleFactor;
//     dUltimateStrength = rData.dUltimateStrength;
//     dYieldStrength = rData.dYieldStrength;
//     nLoadType = rData.nLoadType;    
//     dAnchoSlipBegin = rData.dAnchoSlipBegin;
//     dAnchoSlipEnd = rData.dAnchoSlipEnd;
//     nTypeId = rData.nTypeId;
//     nBondType = rData.nBondType;    
//     dAlphaExtCable = rData.dAlphaExtCable;
//     nRelaxMethod   = rData.nRelaxMethod;
//     dRelaxRatio    = rData.dRelaxRatio;
//     bOverStressRedFactor = rData.bOverStressRedFactor;
//     dJTG04Factor = rData.dJTG04Factor;
//     dFpk = rData.dFpk;
//     dInadvertentChange=0.0;
//   }
// };
// struct T_TDNT_UDRD_D
// {
//   T_TDNT_K key;
//   T_TDNT_D data;
// };
// struct T_TDNT_UDRD_D_CH_560
// {
//   T_TDNT_K key;
//   T_TDNT_D_CH_560 data;
// };
// struct T_TDNT_UDRD_D_CH_630
// {
//   T_TDNT_K key;
//   T_TDNT_D_CH_630 data;
// };
// struct T_TDNT_UDRD_D_CH_650
// {
//   T_TDNT_K key;
//   T_TDNT_D_CH_650 data;
// };
// struct T_TDNT_UDRD_D_CH_660
// {
//   T_TDNT_K key;
//   T_TDNT_D_CH_660 data;
// };
// struct T_TDNT_UDRD_D_CH_702
// {
//   T_TDNT_K key;
//   T_TDNT_D_CH_702 data;
// };
// struct T_TDNT_UDRD_D_CH
// {
//   T_TDNT_K key;
//   T_TDNT_D_CH data;
// };
// struct T_TDNT_UNIT
// {
//   T_TDNT_UNIT()
//   {
//     dTendonArea = D_UNITSYS_BASE_AREA;
//     dDuctArea = D_UNITSYS_BASE_LENGTH;
//     dWobbleFactor = D_UNITSYS_BASE_PERLENGTH;
//     dUltimateStrength = D_UNITSYS_BASE_STRESS;
//     dYieldStrength = D_UNITSYS_BASE_STRESS;
//     dAnchoSlipBegin = D_UNITSYS_BASE_LENGTH;
//     dAnchoSlipEnd = D_UNITSYS_BASE_LENGTH;
//     dAlphaExtCable = D_UNITSYS_BASE_STRESS;
//     dFpk = D_UNITSYS_BASE_STRESS;
//     dInadvertentChange = D_UNITSYS_BASE_RADIAN;
//   }
//   int dTendonArea;
//   int dDuctArea;
//   int dWobbleFactor;
//   int dUltimateStrength;
//   int dYieldStrength;
//   int dAnchoSlipBegin;
//   int dAnchoSlipEnd;
//   int dAlphaExtCable;
//   int dFpk;
//   int dInadvertentChange;
// };
// #define HASHSIZETDNT 101

// Tendon profile
struct T_TDNA_BASE_501
{
	int nInputOption;   // 0:Relative, 1:Absolute
	// Relative
	T_ELEM_K RefElemKey;  // Frame Type 요소만 가능
	int    nDistOpt;      // 0:Relative, 1:Absolute
	double dDistI;       // 단위:없음(0.0 ~ 1.0)
	double dEccY, dEccZ;
	BOOL   bAlpha;
	double dAlphaY, dAlphaZ;
	// Absolute
	double dCoord[3];   // x, y, z
	BOOL   bXYZ[3];     // x, y, z
	double dDirVec[3];  // dx, dy, dz

	void Initiaize()
	{
		nInputOption = 0;
		RefElemKey = 0;
		nDistOpt = 0;
		dDistI = 0.0;
		dEccY = dEccZ = 0.0;
		bAlpha = FALSE;
		dAlphaY = dAlphaZ = 0.0;
		bXYZ[0] = bXYZ[1] = bXYZ[2] = FALSE;
		for (int i = 0; i < 3; i++) dCoord[i] = dDirVec[i] = 0.0;
	}
};
struct T_TDNA_BASE_540
{
	double x, y, z; // Local Coordinates
	BOOL   bFix;
	double dRy;
	double dRz;
};
struct T_TDNA_BASE
{
	double x, y, z; // Local Coordinates
	BOOL   bFix;
	double dRy;
	double dRz;
	double dRadius;
	void Initiaize()
	{
		x = y = z = 0.0;
		bFix = FALSE;
		dRy = 0.0;
		dRz = 0.0;
		dRadius = 0.0;
	}
	void Convert501(T_TDNA_BASE_501& rData);  // AttrCtrl에 있음 : 반영할 것
	void Convert540(T_TDNA_BASE_540& rData, double dTPRadius)
	{
		x = rData.x;
		y = rData.y;
		z = rData.z;
		bFix = rData.bFix;
		dRy = rData.dRy;
		dRz = rData.dRz;
		dRadius = dTPRadius;
	}
};
struct T_TDNA_BASE_UNIT
{
	T_TDNA_BASE_UNIT()
	{
		x = y = z = D_UNITSYS_BASE_LENGTH;
		dRy = dRz = D_UNITSYS_BASE_DEGREE;
		dRadius = D_UNITSYS_BASE_LENGTH;
	}
	int x, y, z;
	int dRy, dRz;
	int dRadius;
};
struct T_TDNA_BASE_2D_630
{
	double x, yz; // Local Coordinates
	BOOL   bFix;
	double dR;  
	double dRadius;
};
struct T_TDNA_BASE_2D_692
{
	double x, yz; // Local Coordinates
	BOOL   bFix;
	double dR;  
	double dRadius;
	int    nAddOpt;    // 0:None, 1:Left, 2:Right (6.4.0에서 추가)
	double dAngle;     // -90 < dAngle < 90       (6.4.0에서 추가)
	double dHeight;    // Height                  (6.4.0에서 추가)
	double dRadius2;   // Second Radius           (6.4.0에서 추가)
};
struct T_TDNA_BASE_2D
{
	double x, yz; // Local Coordinates
	BOOL   bFix;
	double dR;  
	double dRadius;
	int    nAddOpt;    // 0:None, 1:Left, 2:Right (6.4.0에서 추가)
	double dAngle;     // -90 < dAngle < 90       (6.4.0에서 추가)
	double dHeight;    // Height                  (6.4.0에서 추가)
	double dRadius2;   // Second Radius           (6.4.0에서 추가)
	BOOL   bBottomZ;   // 7.0.0 추가
	void Initiaize()
	{
		x = yz = 0.0;
		bFix = FALSE;
		dR = 0.0;    
		dRadius = 0.0;
		nAddOpt = 0;
		dAngle = 0.0;
		dHeight = 0.0;
		dRadius2 = 0.0;
		bBottomZ = FALSE;
	}
	void Convert630(T_TDNA_BASE_2D_630& rData)
	{
		x = rData.x;
		yz = rData.yz;
		bFix = rData.bFix;
		dR = rData.dR;
		dRadius = rData.dRadius;
		nAddOpt = 0;
		dAngle = 0.0;
		dHeight = 0.0;
		dRadius2 = 0.0;
		bBottomZ = FALSE;
	}
	void Convert692(T_TDNA_BASE_2D_692& rData)
	{
		x = rData.x;
		yz = rData.yz;
		bFix = rData.bFix;
		dR = rData.dR;
		dRadius = rData.dRadius;
		nAddOpt = rData.nAddOpt;
		dAngle = rData.dAngle;
		dHeight = rData.dHeight;
		dRadius2 = rData.dRadius2;
		bBottomZ = FALSE;
	}
};
struct T_TDNA_BASE_2D_UNIT
{
	T_TDNA_BASE_2D_UNIT()
	{
		x = yz = D_UNITSYS_BASE_LENGTH;
		dR = D_UNITSYS_BASE_DEGREE;
		dRadius = D_UNITSYS_BASE_LENGTH;
		dAngle = D_UNITSYS_BASE_DEGREE;
		dHeight = D_UNITSYS_BASE_LENGTH;
		dRadius2 = D_UNITSYS_BASE_LENGTH;
	}
	int x, yz;
	int dR;
	int dRadius;
	int dAngle;
	int dHeight;
	int dRadius2;
};
#define T_TDNA_K unsigned int
struct T_TDNA_D_501
{
	CString TendonName;
	T_TDNT_K TendonTypeKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	double dBeginLength;
	double dEndLength;
	CArray<T_TDNA_BASE_501, T_TDNA_BASE_501&> aProfile;
	void Initialize()
	{
		TendonName.Empty();
		TendonTypeKey = 0;
		aElemList.RemoveAll();
		dBeginLength = 0.0;
		dEndLength = 0.0;
		aProfile.RemoveAll();
	}
};

// struct T_TDNA_D_896
// {
// 	CString TendonName;
// 	T_TDNT_K TendonTypeKey;
// 	CArray<T_ELEM_K, T_ELEM_K> aElemList;
// 	int    nCurveType;          // 0:Spline, 1:Round
// 	double dBeginLength;
// 	double dEndLength;
// 	CArray<T_TDNA_BASE, T_TDNA_BASE&> aProfile;
// 	CArray<T_TDNA_BASE_2D, T_TDNA_BASE_2D&> aProfileY;   // (6.1.0 추가)
// 	CArray<T_TDNA_BASE_2D, T_TDNA_BASE_2D&> aProfileZ;   // (6.1.0 추가)
// 	int    nTendonShape;        // 0:Straight, 1:Curve, 2:Element(6.1.0에서 추가)
// 	double dInsertPointX;       // Profile 원점이 위치할 전체 좌표계 기준 좌표값
// 	double dInsertPointY;
// 	double dInsertPointZ;
// 	int    nXAxisDirection;     // 0:X, 1:Y, 2:Vector
// 	double dVectorX, dVectorY;  // nXAxisDirection=Vector인 경우
// 	double dCenterX, dCenterY;  // 반경 중심
// 	double dOffset;             // 반경방향 오프셋
// 	int    nCurveDir;           // 0:CW, 1:CCW
// 	double dXAxisRotAngle;      // Profile x축의 회전각
// 	BOOL   bProjection;
// 	int    nGradRotAxis;        // 종구배 회전 축 0:X, 1:Y
// 	double dGradRotAngle;       // 종구배 회전각
// 	int    nInputType;          // 0 : 2D, 1 : 3D          (6.1.0 추가)
// 	int    nInsertPoint;        // 0 : End-I, 1 : End-J    (6.1.0 추가)
// 	T_ELEM_K InsertElemKey;     //                         (6.1.0 추가)
// 	int    nXAxisDirectionIJ;   // 0 : I->J, 1 : J->I      (6.1.0 추가)  
// 	double dOffsetY;            // Y Offset                (6.1.0 추가)
// 	double dOffsetZ;            // Z Offset                (6.1.0 추가)
// 	T_TDGR_K GroupKey;          // Tendon Group            (6.4.0 추가)
// 	int    nNoTensionCalc;      // 무효장 길이 자동 계산 (6.4.0 추가)
// 	// 0=User Defined, 1=Auto Calc:0.5*(H+Bf/n), 
// 	// 2=Auto Calc:65*Strand Diameter
// 	double dNoTensionBeginLen;  // 무효장 길이 사용자 정의 시작 길이 (6.4.0 추가)
// 	double dNoTensionEndLen;    // 무효장 길이 사용자 정의 끝 길이   (6.4.0 추가)
// 	BOOL   bTypicalTendon;      // 7.0.0 추가
// 	double dTendonNum;          // 텐던 갯수(7.0.0 추가)
// };

struct T_TDNA_D
{
	CString TendonName;
	T_TDNT_K TendonTypeKey;
	CArray<T_ELEM_K, T_ELEM_K> aElemList; // 기존: ElemK    PT-Slab:NodeK
	int    nCurveType;          // 0:Spline, 1:Round
	double dBeginLength;
	double dEndLength;
	CArray<T_TDNA_BASE, T_TDNA_BASE&> aProfile;
	CArray<T_TDNA_BASE_2D, T_TDNA_BASE_2D&> aProfileY;   // (6.1.0 추가)
	CArray<T_TDNA_BASE_2D, T_TDNA_BASE_2D&> aProfileZ;   // (6.1.0 추가)
	int    nTendonShape;        // 0:Straight, 1:Curve, 2:Element(6.1.0에서 추가)
	double dInsertPointX;       // Profile 원점이 위치할 전체 좌표계 기준 좌표값
	double dInsertPointY;
	double dInsertPointZ;
	int    nXAxisDirection;     // 0:X, 1:Y, 2:Vector
	double dVectorX, dVectorY;  // nXAxisDirection=Vector인 경우
	double dCenterX, dCenterY;  // 반경 중심
	double dOffset;             // 반경방향 오프셋
	int    nCurveDir;           // 0:CW, 1:CCW
	double dXAxisRotAngle;      // Profile x축의 회전각
	BOOL   bProjection;
	int    nGradRotAxis;        // 종구배 회전 축 0:X, 1:Y
	double dGradRotAngle;       // 종구배 회전각
	int    nInputType;          // 0 : 2D, 1 : 3D          (6.1.0 추가)
	int    nInsertPoint;        // 0 : End-I, 1 : End-J    (6.1.0 추가)
	T_ELEM_K InsertElemKey;     //                         (6.1.0 추가)
	int    nXAxisDirectionIJ;   // 0 : I->J, 1 : J->I      (6.1.0 추가)  
	double dOffsetY;            // Y Offset                (6.1.0 추가)
	double dOffsetZ;            // Z Offset                (6.1.0 추가)
	T_TDGR_K GroupKey;          // Tendon Group            (6.4.0 추가)
	int    nNoTensionCalc;      // 무효장 길이 자동 계산 (6.4.0 추가)
															// 0=User Defined, 1=Auto Calc:0.5*(H+Bf/n), 
															// 2=Auto Calc:65*Strand Diameter
	double dNoTensionBeginLen;  // 무효장 길이 사용자 정의 시작 길이 (6.4.0 추가)
	double dNoTensionEndLen;    // 무효장 길이 사용자 정의 끝 길이   (6.4.0 추가)
	BOOL   bTypicalTendon;      // 7.0.0 추가
	double dTendonNum;          // 텐던 갯수(7.0.0 추가)
	double dDebondBeginLen;     // Debonded 시작 길이 (9.0.0 추가)
	double dDebondEndLen;       // Debonded 끝 길이   (9.0.0 추가)

	void Initialize()
	{
		TendonName.Empty();
		TendonTypeKey = 0;
		aElemList.RemoveAll();
		nCurveType = 0;
#if defined(_CH)
		nCurveType = 1;
#endif
		dBeginLength = 0.0;
		dEndLength = 0.0;
		aProfile.RemoveAll();
		aProfileY.RemoveAll();
		aProfileZ.RemoveAll();
		nTendonShape = 2;
		dInsertPointX = dInsertPointY = dInsertPointZ = 0.0;
		nXAxisDirection = 0;
		dVectorX = dVectorY = 0.0;
		dCenterX = dCenterY = 0.0;
		dOffset = 0.0;
		nCurveDir = 0;
		dXAxisRotAngle = 0.0;
		bProjection = TRUE;
		nGradRotAxis = 1;
		dGradRotAngle = 0.0;
		nInputType   = 0;
		nInsertPoint = 0;
		InsertElemKey = 0;
		nXAxisDirectionIJ = 0;
		dOffsetY = 0.;
		dOffsetZ = 0.;
		GroupKey = 0;
		nNoTensionCalc = 0;
#if defined(_JP)
		nNoTensionCalc = 1;
#endif
		dNoTensionBeginLen = 0.0;
		dNoTensionEndLen = 0.0;
		bTypicalTendon = FALSE;
		dTendonNum = 1.0;
		dDebondBeginLen = 0.0;
		dDebondEndLen = 0.0;
	}
	T_TDNA_D()
	{
	}
	T_TDNA_D(T_TDNA_D& src)
	{
		*this = src;
	}
	T_TDNA_D& operator=(const T_TDNA_D& src)
	{
		TendonName        = src.TendonName;
		TendonTypeKey     = src.TendonTypeKey;
		aElemList.Copy    ( src.aElemList);
		nCurveType        = src.nCurveType;
		dBeginLength      = src.dBeginLength;
		dEndLength        = src.dEndLength;
		aProfile.Copy     ( src.aProfile);
		aProfileY.Copy    ( src.aProfileY);
		aProfileZ.Copy    ( src.aProfileZ);
		nTendonShape      = src.nTendonShape;
		dInsertPointX     = src.dInsertPointX;
		dInsertPointY     = src.dInsertPointY;
		dInsertPointZ     = src.dInsertPointZ;
		nXAxisDirection   = src.nXAxisDirection;
		dVectorX          = src.dVectorX;
		dVectorY          = src.dVectorY;
		dCenterX          = src.dCenterX;
		dCenterY          = src.dCenterY;
		dOffset           = src.dOffset;
		nCurveDir         = src.nCurveDir;
		dXAxisRotAngle    = src.dXAxisRotAngle;
		bProjection       = src.bProjection;
		nGradRotAxis      = src.nGradRotAxis;
		dGradRotAngle     = src.dGradRotAngle;
		nInputType        = src.nInputType       ;
		nInsertPoint      = src.nInsertPoint     ;
		InsertElemKey     = src.InsertElemKey    ;
		nXAxisDirectionIJ = src.nXAxisDirectionIJ;
		dOffsetY          = src.dOffsetY         ;
		dOffsetZ          = src.dOffsetZ         ;
		GroupKey          = src.GroupKey;
		nNoTensionCalc    = src.nNoTensionCalc;
		dNoTensionBeginLen= src.dNoTensionBeginLen;
		dNoTensionEndLen  = src.dNoTensionEndLen;
		bTypicalTendon    = src.bTypicalTendon;
		dTendonNum        = src.dTendonNum;
		dDebondBeginLen   = src.dDebondBeginLen;
		dDebondEndLen     = src.dDebondEndLen;

		return *this;
	}
	void Convert501(T_TDNA_D_501& rData); // AttrCtrl.cpp에 있음
};

struct T_TDNA_D_CH_692
{
	char TendonName[20];
	T_TDNT_K TendonTypeKey;
	CArray<T_ELEM_K, T_ELEM_K>aElemList;
	int    nCurveType;
	double dBeginLength;
	double dEndLength;
	CArray<T_TDNA_BASE, T_TDNA_BASE&> aProfile;
	CArray<T_TDNA_BASE_2D_692, T_TDNA_BASE_2D_692&> aProfileY;
	CArray<T_TDNA_BASE_2D_692, T_TDNA_BASE_2D_692&> aProfileZ;
	int    nTendonShape;      
	double dInsertPointX;     
	double dInsertPointY;
	double dInsertPointZ;
	int    nXAxisDirection;   
	double dVectorX, dVectorY;
	double dCenterX, dCenterY;
	double dOffset;           
	int    nCurveDir;
	double dXAxisRotAngle;    
	BOOL   bProjection;
	int    nGradRotAxis;      
	double dGradRotAngle;     
	int    nInputType;        
	int    nInsertPoint;      
	T_ELEM_K InsertElemKey;   
	int    nXAxisDirectionIJ; 
	double dOffsetY;          
	double dOffsetZ;          
	T_TDGR_K GroupKey;        
	BOOL   nNoTensionCalc;    
	double dNoTensionBeginLen;
	double dNoTensionEndLen;  
};
struct T_TDNA_D_CH_896
{
	char TendonName[20];
	T_TDNT_K TendonTypeKey;
	CArray<T_ELEM_K, T_ELEM_K>aElemList;
	int    nCurveType;
	double dBeginLength;
	double dEndLength;
	CArray<T_TDNA_BASE, T_TDNA_BASE&> aProfile;
	CArray<T_TDNA_BASE_2D, T_TDNA_BASE_2D&> aProfileY;   // (6.1.0 추가)
	CArray<T_TDNA_BASE_2D, T_TDNA_BASE_2D&> aProfileZ;   // (6.1.0 추가)
	int    nTendonShape;        // 0:Straight, 1:Curve
	double dInsertPointX;       // Profile 원점이 위치할 전체 좌표계 기준 좌표값
	double dInsertPointY;
	double dInsertPointZ;
	int    nXAxisDirection;     // 0:X, 1:Y, 2:Vector
	double dVectorX, dVectorY;  // nXAxisDirection=Vector인 경우
	double dCenterX, dCenterY;  // 반경 중심
	double dOffset;             // 반경방향 오프셋
	int    nCurveDir;
	double dXAxisRotAngle;      // Profile x축의 회전각
	BOOL   bProjection;
	int    nGradRotAxis;        // 종구배 회전 축 0:X, 1:Y
	double dGradRotAngle;       // 종구배 회전각
	int    nInputType;          // 0 : 2D, 1 : 3D          (6.1.0 추가)
	int    nInsertPoint;        // 0 : End-I, 1 : End-J    (6.1.0 추가)
	T_ELEM_K InsertElemKey;     //                         (6.1.0 추가)
	int    nXAxisDirectionIJ;   // 0 : I->J, 1 : J->I      (6.1.0 추가)  
	double dOffsetY;            // Y Offset                (6.1.0 추가)
	double dOffsetZ;            // Z Offset                (6.1.0 추가)
	T_TDGR_K GroupKey;          // Tendon Group            (6.4.0 추가)
	BOOL   nNoTensionCalc;      // 무효장 길이 자동 계산   (6.4.0 추가)
	double dNoTensionBeginLen;  // 무효장 길이 사용자 정의 시작 길이 (6.4.0 추가)
	double dNoTensionEndLen;    // 무효장 길이 사용자 정의 끝 길이   (6.4.0 추가)
	BOOL   bTypicalTendon;      // 7.0.0 추가
	double dTendonNum;          // 텐던 갯수(7.0.0 추가)
};
struct T_TDNA_D_CH
{
	char TendonName[20];
	T_TDNT_K TendonTypeKey;
	CArray<T_ELEM_K, T_ELEM_K>aElemList;
	int    nCurveType;
	double dBeginLength;
	double dEndLength;
	CArray<T_TDNA_BASE, T_TDNA_BASE&> aProfile;
	CArray<T_TDNA_BASE_2D, T_TDNA_BASE_2D&> aProfileY;   // (6.1.0 추가)
	CArray<T_TDNA_BASE_2D, T_TDNA_BASE_2D&> aProfileZ;   // (6.1.0 추가)
	int    nTendonShape;        // 0:Straight, 1:Curve
	double dInsertPointX;       // Profile 원점이 위치할 전체 좌표계 기준 좌표값
	double dInsertPointY;
	double dInsertPointZ;
	int    nXAxisDirection;     // 0:X, 1:Y, 2:Vector
	double dVectorX, dVectorY;  // nXAxisDirection=Vector인 경우
	double dCenterX, dCenterY;  // 반경 중심
	double dOffset;             // 반경방향 오프셋
	int    nCurveDir;
	double dXAxisRotAngle;      // Profile x축의 회전각
	BOOL   bProjection;
	int    nGradRotAxis;        // 종구배 회전 축 0:X, 1:Y
	double dGradRotAngle;       // 종구배 회전각
	int    nInputType;          // 0 : 2D, 1 : 3D          (6.1.0 추가)
	int    nInsertPoint;        // 0 : End-I, 1 : End-J    (6.1.0 추가)
	T_ELEM_K InsertElemKey;     //                         (6.1.0 추가)
	int    nXAxisDirectionIJ;   // 0 : I->J, 1 : J->I      (6.1.0 추가)  
	double dOffsetY;            // Y Offset                (6.1.0 추가)
	double dOffsetZ;            // Z Offset                (6.1.0 추가)
	T_TDGR_K GroupKey;          // Tendon Group            (6.4.0 추가)
	BOOL   nNoTensionCalc;      // 무효장 길이 자동 계산   (6.4.0 추가)
	double dNoTensionBeginLen;  // 무효장 길이 사용자 정의 시작 길이 (6.4.0 추가)
	double dNoTensionEndLen;    // 무효장 길이 사용자 정의 끝 길이   (6.4.0 추가)
	BOOL   bTypicalTendon;      // 7.0.0 추가
	double dTendonNum;          // 텐던 갯수(7.0.0 추가)
	double dDebondBeginLen;     // Debonded 시작 길이 (9.0.0 추가)
	double dDebondEndLen;       // Debonded 끝 길이   (9.0.0 추가)

	void ConvertToString(T_TDNA_D& rData)
	{
		ConvertCharStr(TendonName, rData.TendonName, sizeof(TendonName));
		rData.TendonTypeKey = TendonTypeKey;
		rData.aElemList.Copy(aElemList);
		rData.nCurveType = nCurveType;
		rData.dBeginLength = dBeginLength;
		rData.dEndLength = dEndLength;
		rData.aProfile.Copy(aProfile);
		rData.aProfileY.Copy(aProfileY);
		rData.aProfileZ.Copy(aProfileZ);
		rData.nTendonShape = nTendonShape;
		rData.dInsertPointX = dInsertPointX;
		rData.dInsertPointY = dInsertPointY;
		rData.dInsertPointZ = dInsertPointZ;
		rData.nXAxisDirection = nXAxisDirection;
		rData.dVectorX = dVectorX;
		rData.dVectorY = dVectorY;
		rData.dCenterX = dCenterX;
		rData.dCenterY = dCenterY;
		rData.dOffset = dOffset;
		rData.nCurveDir = nCurveDir;
		rData.dXAxisRotAngle = dXAxisRotAngle;
		rData.bProjection = bProjection;
		rData.nGradRotAxis = nGradRotAxis;
		rData.dGradRotAngle = dGradRotAngle;
		rData.nInputType        = nInputType       ;
		rData.nInsertPoint      = nInsertPoint     ;
		rData.InsertElemKey     = InsertElemKey    ;
		rData.nXAxisDirectionIJ = nXAxisDirectionIJ;
		rData.dOffsetY          = dOffsetY         ;
		rData.dOffsetZ          = dOffsetZ         ;
		rData.GroupKey          = GroupKey;
		rData.nNoTensionCalc    = nNoTensionCalc;
		rData.dNoTensionBeginLen= dNoTensionBeginLen;
		rData.dNoTensionEndLen  = dNoTensionEndLen;
		rData.bTypicalTendon    = bTypicalTendon;
		rData.dTendonNum        = dTendonNum;
		rData.dDebondBeginLen   = dDebondBeginLen;
		rData.dDebondEndLen     = dDebondEndLen;
	}
	void ConvertToChar(T_TDNA_D& rData)
	{
		ConvertStrChar(rData.TendonName, TendonName, sizeof(TendonName));
		TendonTypeKey = rData.TendonTypeKey;
		aElemList.Copy(rData.aElemList);
		nCurveType = rData.nCurveType;
		dBeginLength = rData.dBeginLength;
		dEndLength = rData.dEndLength;
		aProfile.Copy(rData.aProfile);
		aProfileY.Copy(rData.aProfileY);
		aProfileZ.Copy(rData.aProfileZ);
		nTendonShape = rData.nTendonShape;
		dInsertPointX = rData.dInsertPointX;
		dInsertPointY = rData.dInsertPointY;
		dInsertPointZ = rData.dInsertPointZ;
		nXAxisDirection = rData.nXAxisDirection;
		dVectorX = rData.dVectorX;
		dVectorY = rData.dVectorY;
		dCenterX = rData.dCenterX;
		dCenterY = rData.dCenterY;
		dOffset = rData.dOffset;
		nCurveDir = rData.nCurveDir;
		dXAxisRotAngle = rData.dXAxisRotAngle;
		bProjection = rData.bProjection;
		nGradRotAxis = rData.nGradRotAxis;
		dGradRotAngle = rData.dGradRotAngle;
		nInputType        = rData.nInputType       ;
		nInsertPoint      = rData.nInsertPoint     ;
		InsertElemKey     = rData.InsertElemKey    ;
		nXAxisDirectionIJ = rData.nXAxisDirectionIJ;
		dOffsetY          = rData.dOffsetY         ;
		dOffsetZ          = rData.dOffsetZ         ;
		GroupKey          = rData.GroupKey;
		nNoTensionCalc    = rData.nNoTensionCalc;
		dNoTensionBeginLen= rData.dNoTensionBeginLen;
		dNoTensionEndLen  = rData.dNoTensionEndLen;
		bTypicalTendon    = rData.bTypicalTendon;
		dTendonNum        = rData.dTendonNum;
		dDebondBeginLen   = rData.dDebondBeginLen;
		dDebondEndLen     = rData.dDebondEndLen;
	}
};

struct T_TDNA_UDRD_D
{
	T_TDNA_K key;
	T_TDNA_D data;
};
struct T_TDNA_UDRD_D_CH
{
	T_TDNA_K key;
	T_TDNA_D_CH data;
};
struct T_TDNA_UDRD_D_CH_RW_501
{
	T_TDNA_K key;
	char TendonName[20];
	T_TDNT_K TendonTypeKey;
	unsigned int nCountElemList;
	double dBeginLength;
	double dEndLength;
	unsigned int nCountProfile;
};
struct T_TDNA_UDRD_D_CH_RW_510_ALPHA
{
	T_TDNA_K key;
	char TendonName[20];
	T_TDNT_K TendonTypeKey;
	unsigned int nCountElemList;
	double dBeginLength;
	double dEndLength;
	unsigned int nCountProfile;
	int    nTendonShape;        // 0:Straight, 1:Curve
	double dInsertPointX;       // Profile 원점이 위치할 전체 좌표계 기준 좌표값
	double dInsertPointY;
	double dInsertPointZ;
	int    nXAxisDirection;     // 0:X, 1:Y, 2:Vector
	double dVectorX, dVectorY;  // nXAxisDirection=Vector인 경우
	double dCenterX, dCenterY;  // 반경 중심
	double dOffset;             // 반경방향 오프셋
	double dXAxisRotAngle;      // Profile x축의 회전각
	BOOL   bProjection;
	int    nGradRotAxis;        // 종구배 회전 축 0:X, 1:Y
	double dGradRotAngle;       // 종구배 회전각
};
struct T_TDNA_UDRD_D_CH_RW_530
{
	T_TDNA_K key;
	char TendonName[20];
	T_TDNT_K TendonTypeKey;
	unsigned int nCountElemList;
	double dBeginLength;
	double dEndLength;
	unsigned int nCountProfile;
	int    nTendonShape;        // 0:Straight, 1:Curve
	double dInsertPointX;       // Profile 원점이 위치할 전체 좌표계 기준 좌표값
	double dInsertPointY;
	double dInsertPointZ;
	int    nXAxisDirection;     // 0:X, 1:Y, 2:Vector
	double dVectorX, dVectorY;  // nXAxisDirection=Vector인 경우
	double dCenterX, dCenterY;  // 반경 중심
	double dOffset;             // 반경방향 오프셋
	int    nCurveDir;
	double dXAxisRotAngle;      // Profile x축의 회전각
	BOOL   bProjection;
	int    nGradRotAxis;        // 종구배 회전 축 0:X, 1:Y
	double dGradRotAngle;       // 종구배 회전각
};
struct T_TDNA_UDRD_D_CH_RW_540
{
	T_TDNA_K key;
	char TendonName[20];
	T_TDNT_K TendonTypeKey;
	unsigned int nCountElemList;
	int    nCurveType;
	double dTPRadius;
	double dBeginLength;
	double dEndLength;
	unsigned int nCountProfile;
	int    nTendonShape;        // 0:Straight, 1:Curve
	double dInsertPointX;       // Profile 원점이 위치할 전체 좌표계 기준 좌표값
	double dInsertPointY;
	double dInsertPointZ;
	int    nXAxisDirection;     // 0:X, 1:Y, 2:Vector
	double dVectorX, dVectorY;  // nXAxisDirection=Vector인 경우
	double dCenterX, dCenterY;  // 반경 중심
	double dOffset;             // 반경방향 오프셋
	int    nCurveDir;
	double dXAxisRotAngle;      // Profile x축의 회전각
	BOOL   bProjection;
	int    nGradRotAxis;        // 종구배 회전 축 0:X, 1:Y
	double dGradRotAngle;       // 종구배 회전각
};
struct T_TDNA_UDRD_D_CH_RW_600
{
	T_TDNA_K key;
	char TendonName[20];
	T_TDNT_K TendonTypeKey;
	unsigned int nCountElemList;
	int    nCurveType;
	double dBeginLength;
	double dEndLength;
	unsigned int nCountProfile;
	int    nTendonShape;        // 0:Straight, 1:Curve
	double dInsertPointX;       // Profile 원점이 위치할 전체 좌표계 기준 좌표값
	double dInsertPointY;
	double dInsertPointZ;
	int    nXAxisDirection;     // 0:X, 1:Y, 2:Vector
	double dVectorX, dVectorY;  // nXAxisDirection=Vector인 경우
	double dCenterX, dCenterY;  // 반경 중심
	double dOffset;             // 반경방향 오프셋
	int    nCurveDir;
	double dXAxisRotAngle;      // Profile x축의 회전각
	BOOL   bProjection;
	int    nGradRotAxis;        // 종구배 회전 축 0:X, 1:Y
	double dGradRotAngle;       // 종구배 회전각
};
struct T_TDNA_UDRD_D_CH_RW_630
{
	T_TDNA_K key;
	char TendonName[20];
	T_TDNT_K TendonTypeKey;
	unsigned int nCountElemList;
	int    nCurveType;
	double dBeginLength;
	double dEndLength;
	unsigned int nCountProfile;
	unsigned int nCountProfileY;
	unsigned int nCountProfileZ;
	int    nTendonShape;        // 0:Straight, 1:Curve
	double dInsertPointX;       // Profile 원점이 위치할 전체 좌표계 기준 좌표값
	double dInsertPointY;
	double dInsertPointZ;
	int    nXAxisDirection;     // 0:X, 1:Y, 2:Vector
	double dVectorX, dVectorY;  // nXAxisDirection=Vector인 경우
	double dCenterX, dCenterY;  // 반경 중심
	double dOffset;             // 반경방향 오프셋
	int    nCurveDir;
	double dXAxisRotAngle;      // Profile x축의 회전각
	BOOL   bProjection;
	int    nGradRotAxis;        // 종구배 회전 축 0:X, 1:Y
	double dGradRotAngle;       // 종구배 회전각
	int    nInputType;
	int    nInsertPoint;        // 0 : End-I, 1 : End-J    (6.1.0 추가)
	T_ELEM_K InsertElemKey;     //                         (6.1.0 추가)
	int    nXAxisDirectionIJ;   // 0 : I->J, 1 : J->I      (6.1.0 추가)  
	double dOffsetY;            // Y Offset                (6.1.0 추가)
	double dOffsetZ;            // Z Offset                (6.1.0 추가)
};
struct T_TDNA_UDRD_D_CH_RW_692
{
	T_TDNA_K key;
	char TendonName[20];
	T_TDNT_K TendonTypeKey;
	unsigned int nCountElemList;
	int    nCurveType;
	double dBeginLength;
	double dEndLength;
	unsigned int nCountProfile;
	unsigned int nCountProfileY;
	unsigned int nCountProfileZ;
	int    nTendonShape;        // 0:Straight, 1:Curve
	double dInsertPointX;       // Profile 원점이 위치할 전체 좌표계 기준 좌표값
	double dInsertPointY;
	double dInsertPointZ;
	int    nXAxisDirection;     // 0:X, 1:Y, 2:Vector
	double dVectorX, dVectorY;  // nXAxisDirection=Vector인 경우
	double dCenterX, dCenterY;  // 반경 중심
	double dOffset;             // 반경방향 오프셋
	int    nCurveDir;
	double dXAxisRotAngle;      // Profile x축의 회전각
	BOOL   bProjection;
	int    nGradRotAxis;        // 종구배 회전 축 0:X, 1:Y
	double dGradRotAngle;       // 종구배 회전각
	int    nInputType;
	int    nInsertPoint;        // 0 : End-I, 1 : End-J    (6.1.0 추가)
	T_ELEM_K InsertElemKey;     //                         (6.1.0 추가)
	int    nXAxisDirectionIJ;   // 0 : I->J, 1 : J->I      (6.1.0 추가)  
	double dOffsetY;            // Y Offset                (6.1.0 추가)
	double dOffsetZ;            // Z Offset                (6.1.0 추가)
	T_TDGR_K GroupKey;          // Tendon Group            (6.4.0 추가)
	BOOL   nNoTensionCalc;      // 무효장 길이 자동 계산   (6.4.0 추가)
	double dNoTensionBeginLen;  // 무효장 길이 사용자 정의 시작 길이 (6.4.0 추가)
	double dNoTensionEndLen;    // 무효장 길이 사용자 정의 끝 길이   (6.4.0 추가)
};
struct T_TDNA_UDRD_D_CH_RW_896
{
	T_TDNA_K key;
	char TendonName[20];
	T_TDNT_K TendonTypeKey;
	unsigned int nCountElemList;
	int    nCurveType;
	double dBeginLength;
	double dEndLength;
	unsigned int nCountProfile;
	unsigned int nCountProfileY;
	unsigned int nCountProfileZ;
	int    nTendonShape;        // 0:Straight, 1:Curve
	double dInsertPointX;       // Profile 원점이 위치할 전체 좌표계 기준 좌표값
	double dInsertPointY;
	double dInsertPointZ;
	int    nXAxisDirection;     // 0:X, 1:Y, 2:Vector
	double dVectorX, dVectorY;  // nXAxisDirection=Vector인 경우
	double dCenterX, dCenterY;  // 반경 중심
	double dOffset;             // 반경방향 오프셋
	int    nCurveDir;
	double dXAxisRotAngle;      // Profile x축의 회전각
	BOOL   bProjection;
	int    nGradRotAxis;        // 종구배 회전 축 0:X, 1:Y
	double dGradRotAngle;       // 종구배 회전각
	int    nInputType;
	int    nInsertPoint;        // 0 : End-I, 1 : End-J    (6.1.0 추가)
	T_ELEM_K InsertElemKey;     //                         (6.1.0 추가)
	int    nXAxisDirectionIJ;   // 0 : I->J, 1 : J->I      (6.1.0 추가)  
	double dOffsetY;            // Y Offset                (6.1.0 추가)
	double dOffsetZ;            // Z Offset                (6.1.0 추가)
	T_TDGR_K GroupKey;          // Tendon Group            (6.4.0 추가)
	BOOL   nNoTensionCalc;      // 무효장 길이 자동 계산   (6.4.0 추가)
	double dNoTensionBeginLen;  // 무효장 길이 사용자 정의 시작 길이 (6.4.0 추가)
	double dNoTensionEndLen;    // 무효장 길이 사용자 정의 끝 길이   (6.4.0 추가)
	BOOL   bTypicalTendon;      // 7.0.0 추가
	double dTendonNum;          // 텐던 갯수(7.0.0 추가)
};
struct T_TDNA_UDRD_D_CH_RW
{
	T_TDNA_K key;
	char TendonName[20];
	T_TDNT_K TendonTypeKey;
	unsigned int nCountElemList;
	int    nCurveType;
	double dBeginLength;
	double dEndLength;
	unsigned int nCountProfile;
	unsigned int nCountProfileY;
	unsigned int nCountProfileZ;
	int    nTendonShape;        // 0:Straight, 1:Curve
	double dInsertPointX;       // Profile 원점이 위치할 전체 좌표계 기준 좌표값
	double dInsertPointY;
	double dInsertPointZ;
	int    nXAxisDirection;     // 0:X, 1:Y, 2:Vector
	double dVectorX, dVectorY;  // nXAxisDirection=Vector인 경우
	double dCenterX, dCenterY;  // 반경 중심
	double dOffset;             // 반경방향 오프셋
	int    nCurveDir;
	double dXAxisRotAngle;      // Profile x축의 회전각
	BOOL   bProjection;
	int    nGradRotAxis;        // 종구배 회전 축 0:X, 1:Y
	double dGradRotAngle;       // 종구배 회전각
	int    nInputType;
	int    nInsertPoint;        // 0 : End-I, 1 : End-J    (6.1.0 추가)
	T_ELEM_K InsertElemKey;     //                         (6.1.0 추가)
	int    nXAxisDirectionIJ;   // 0 : I->J, 1 : J->I      (6.1.0 추가)  
	double dOffsetY;            // Y Offset                (6.1.0 추가)
	double dOffsetZ;            // Z Offset                (6.1.0 추가)
	T_TDGR_K GroupKey;          // Tendon Group            (6.4.0 추가)
	BOOL   nNoTensionCalc;      // 무효장 길이 자동 계산   (6.4.0 추가)
	double dNoTensionBeginLen;  // 무효장 길이 사용자 정의 시작 길이 (6.4.0 추가)
	double dNoTensionEndLen;    // 무효장 길이 사용자 정의 끝 길이   (6.4.0 추가)
	BOOL   bTypicalTendon;      // 7.0.0 추가
	double dTendonNum;          // 텐던 갯수(7.0.0 추가)
	double dDebondBeginLen;     // Debonded 시작 길이 (9.0.0 추가)
	double dDebondEndLen;       // Debonded 끝 길이   (9.0.0 추가)

	void GetTdna(T_TDNA_K& rKey, T_TDNA_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.TendonName, TendonName, sizeof(TendonName));
		rData.TendonTypeKey = TendonTypeKey;
		rData.nCurveType = nCurveType;
		rData.dBeginLength = dBeginLength;
		rData.dEndLength = dEndLength;
		rData.nTendonShape = nTendonShape;
		rData.dInsertPointX = dInsertPointX;
		rData.dInsertPointY = dInsertPointY;
		rData.dInsertPointZ = dInsertPointZ;
		rData.nXAxisDirection = nXAxisDirection;
		rData.dVectorX = dVectorX;
		rData.dVectorY = dVectorY;
		rData.dCenterX = dCenterX;
		rData.dCenterY = dCenterY;
		rData.dOffset = dOffset;
		rData.nCurveDir = nCurveDir;
		rData.dXAxisRotAngle = dXAxisRotAngle;
		rData.bProjection = bProjection;
		rData.nGradRotAxis = nGradRotAxis;
		rData.dGradRotAngle = dGradRotAngle;
		rData.nInputType        = nInputType       ;
		rData.nInsertPoint      = nInsertPoint     ;
		rData.InsertElemKey     = InsertElemKey    ;
		rData.nXAxisDirectionIJ = nXAxisDirectionIJ;
		rData.dOffsetY          = dOffsetY         ;
		rData.dOffsetZ          = dOffsetZ         ;
		rData.GroupKey          = GroupKey;
		rData.nNoTensionCalc    = nNoTensionCalc;
		rData.dNoTensionBeginLen= dNoTensionBeginLen;
		rData.dNoTensionEndLen  = dNoTensionEndLen  ;
		rData.bTypicalTendon    = bTypicalTendon;
		rData.dTendonNum        = dTendonNum;
		rData.dDebondBeginLen   = dDebondBeginLen;
		rData.dDebondEndLen     = dDebondEndLen;
	}
	void SetTdna(const T_TDNA_K Key, const T_TDNA_D_CH& rData)
	{
		key=Key;
		memcpy(TendonName, rData.TendonName, sizeof(TendonName));
		TendonTypeKey = rData.TendonTypeKey;
		nCountElemList = (int)rData.aElemList.GetSize();
		nCurveType = rData.nCurveType;
		dBeginLength = rData.dBeginLength;
		dEndLength = rData.dEndLength;
		nCountProfile  = (int)rData.aProfile.GetSize();
		nCountProfileY = (int)rData.aProfileY.GetSize(); 
		nCountProfileZ = (int)rData.aProfileZ.GetSize(); 
		nTendonShape = rData.nTendonShape;
		dInsertPointX = rData.dInsertPointX;
		dInsertPointY = rData.dInsertPointY;
		dInsertPointZ = rData.dInsertPointZ;
		nXAxisDirection = rData.nXAxisDirection;
		dVectorX = rData.dVectorX;
		dVectorY = rData.dVectorY;
		dCenterX = rData.dCenterX;
		dCenterY = rData.dCenterY;
		dOffset = rData.dOffset;
		nCurveDir = rData.nCurveDir;
		dXAxisRotAngle = rData.dXAxisRotAngle;
		bProjection = rData.bProjection;
		nGradRotAxis = rData.nGradRotAxis;
		dGradRotAngle = rData.dGradRotAngle;
		nInputType        = rData.nInputType       ;
		nInsertPoint      = rData.nInsertPoint     ;
		InsertElemKey     = rData.InsertElemKey    ;
		nXAxisDirectionIJ = rData.nXAxisDirectionIJ;
		dOffsetY          = rData.dOffsetY         ;
		dOffsetZ          = rData.dOffsetZ         ;
		GroupKey          = rData.GroupKey          ;
		nNoTensionCalc    = rData.nNoTensionCalc    ;
		dNoTensionBeginLen= rData.dNoTensionBeginLen;
		dNoTensionEndLen  = rData.dNoTensionEndLen  ;
		bTypicalTendon    = rData.bTypicalTendon;
		dTendonNum        = rData.dTendonNum;
		dDebondBeginLen   = rData.dDebondBeginLen;
		dDebondEndLen     = rData.dDebondEndLen;
	}
	void Convert501(T_TDNA_UDRD_D_CH_RW_501& rData)
	{
		key = rData.key;
		memcpy(TendonName, rData.TendonName, sizeof(TendonName));
		TendonTypeKey = rData.TendonTypeKey;
		nCountElemList = rData.nCountElemList;
		nCurveType = 0;
		dBeginLength = rData.dBeginLength;
		dEndLength = rData.dEndLength;
		nCountProfile = rData.nCountProfile;
		nCountProfileY = 0;
		nCountProfileZ = 0;
		nTendonShape = 0;
		dInsertPointX = 0.0;
		dInsertPointY = 0.0;
		dInsertPointZ = 0.0;
		nXAxisDirection = 0;
		dVectorX = 0.0;
		dVectorY = 0.0;
		dCenterX = 0.0;
		dCenterY = 0.0;
		dOffset = 0.0;
		nCurveDir = 0;
		dXAxisRotAngle = 0.0;
		bProjection = TRUE;
		nGradRotAxis = 1;
		dGradRotAngle = 0.0;
		nInputType    = 1;
		nInsertPoint = 0;
		InsertElemKey = 0;
		nXAxisDirectionIJ = 0;
		dOffsetY = 0.;
		dOffsetZ = 0.;
		GroupKey          = 0;
		nNoTensionCalc    = 0;
		dNoTensionBeginLen= 0.0;
		dNoTensionEndLen  = 0.0;
		bTypicalTendon = FALSE;
		dTendonNum = 1.0;
		dDebondBeginLen   = 0.0;
		dDebondEndLen     = 0.0;
	}
	void Convert510Alpha(T_TDNA_UDRD_D_CH_RW_510_ALPHA& rData)
	{
		key = rData.key;
		memcpy(TendonName, rData.TendonName, sizeof(TendonName));
		TendonTypeKey = rData.TendonTypeKey;
		nCountElemList = rData.nCountElemList;
		nCurveType = 0;
		dBeginLength = rData.dBeginLength;
		dEndLength = rData.dEndLength;
		nCountProfile = rData.nCountProfile;
		nCountProfileY = 0;
		nCountProfileZ = 0;
		nTendonShape = rData.nTendonShape;
		dInsertPointX = rData.dInsertPointX;
		dInsertPointY = rData.dInsertPointY;
		dInsertPointZ = rData.dInsertPointZ;
		nXAxisDirection = rData.nXAxisDirection;
		dVectorX = rData.dVectorX;
		dVectorY = rData.dVectorY;
		dCenterX = rData.dCenterX;
		dCenterY = rData.dCenterY;
		dOffset = rData.dOffset;
		if (rData.nTendonShape == 1)
			nCurveDir = (rData.dCenterY < 0.0) ? 0 : 1;
		else nCurveDir = 0;
		dXAxisRotAngle = rData.dXAxisRotAngle;
		bProjection = rData.bProjection;
		nGradRotAxis = rData.nGradRotAxis;
		dGradRotAngle = rData.dGradRotAngle;
		nInputType    = 1;
		nInsertPoint = 0;
		InsertElemKey = 0;
		nXAxisDirectionIJ = 0;
		dOffsetY = 0.;
		dOffsetZ = 0.;
		GroupKey          = 0;
		nNoTensionCalc    = 0;
		dNoTensionBeginLen= 0.0;
		dNoTensionEndLen  = 0.0;
		bTypicalTendon = FALSE;
		dTendonNum = 1.0;
		dDebondBeginLen   = 0.0;
		dDebondEndLen     = 0.0;
	}
	void Convert530(T_TDNA_UDRD_D_CH_RW_530& rData)
	{
		key = rData.key;
		memcpy(TendonName, rData.TendonName, sizeof(TendonName));
		TendonTypeKey = rData.TendonTypeKey;
		nCountElemList = rData.nCountElemList;
		nCurveType = 0;
		dBeginLength = rData.dBeginLength;
		dEndLength = rData.dEndLength;
		nCountProfile = rData.nCountProfile;
		nCountProfileY = 0;
		nCountProfileZ = 0;
		nTendonShape = rData.nTendonShape;
		dInsertPointX = rData.dInsertPointX;
		dInsertPointY = rData.dInsertPointY;
		dInsertPointZ = rData.dInsertPointZ;
		nXAxisDirection = rData.nXAxisDirection;
		dVectorX = rData.dVectorX;
		dVectorY = rData.dVectorY;
		dCenterX = rData.dCenterX;
		dCenterY = rData.dCenterY;
		dOffset = rData.dOffset;
		nCurveDir = rData.nCurveDir;
		dXAxisRotAngle = rData.dXAxisRotAngle;
		bProjection = rData.bProjection;
		nGradRotAxis = rData.nGradRotAxis;
		dGradRotAngle = rData.dGradRotAngle;
		nInputType    = 1;
		nInsertPoint = 0;
		InsertElemKey = 0;
		nXAxisDirectionIJ = 0;
		dOffsetY = 0.;
		dOffsetZ = 0.;
		GroupKey          = 0;
		nNoTensionCalc    = 0;
		dNoTensionBeginLen= 0.0;
		dNoTensionEndLen  = 0.0;
		bTypicalTendon = FALSE;
		dTendonNum = 1.0;
		dDebondBeginLen   = 0.0;
		dDebondEndLen     = 0.0;
	}
	void Convert540(T_TDNA_UDRD_D_CH_RW_540& rData)
	{
		key = rData.key;
		memcpy(TendonName, rData.TendonName, sizeof(TendonName));
		TendonTypeKey = rData.TendonTypeKey;
		nCountElemList = rData.nCountElemList;
		nCurveType = rData.nCurveType;
		dBeginLength = rData.dBeginLength;
		dEndLength = rData.dEndLength;
		nCountProfile = rData.nCountProfile;
		nCountProfileY = 0;
		nCountProfileZ = 0;
		nTendonShape = rData.nTendonShape;
		dInsertPointX = rData.dInsertPointX;
		dInsertPointY = rData.dInsertPointY;
		dInsertPointZ = rData.dInsertPointZ;
		nXAxisDirection = rData.nXAxisDirection;
		dVectorX = rData.dVectorX;
		dVectorY = rData.dVectorY;
		dCenterX = rData.dCenterX;
		dCenterY = rData.dCenterY;
		dOffset = rData.dOffset;
		nCurveDir = rData.nCurveDir;
		dXAxisRotAngle = rData.dXAxisRotAngle;
		bProjection = rData.bProjection;
		nGradRotAxis = rData.nGradRotAxis;
		dGradRotAngle = rData.dGradRotAngle;
		nInputType    = 1;
		nInsertPoint = 0;
		InsertElemKey = 0;
		nXAxisDirectionIJ = 0;
		dOffsetY = 0.;
		dOffsetZ = 0.;
		GroupKey          = 0;
		nNoTensionCalc    = 0;
		dNoTensionBeginLen= 0.0;
		dNoTensionEndLen  = 0.0;
		bTypicalTendon = FALSE;
		dTendonNum = 1.0;
		dDebondBeginLen   = 0.0;
		dDebondEndLen     = 0.0;
	}
	void Convert600(T_TDNA_UDRD_D_CH_RW_600& rData)
	{
		key = rData.key;
		memcpy(TendonName, rData.TendonName, sizeof(TendonName));
		TendonTypeKey = rData.TendonTypeKey;
		nCountElemList = rData.nCountElemList;
		nCurveType = rData.nCurveType;
		dBeginLength = rData.dBeginLength;
		dEndLength = rData.dEndLength;
		nCountProfile = rData.nCountProfile;
		nCountProfileY = 0;
		nCountProfileZ = 0;
		nTendonShape = rData.nTendonShape;
		dInsertPointX = rData.dInsertPointX;
		dInsertPointY = rData.dInsertPointY;
		dInsertPointZ = rData.dInsertPointZ;
		nXAxisDirection = rData.nXAxisDirection;
		dVectorX = rData.dVectorX;
		dVectorY = rData.dVectorY;
		dCenterX = rData.dCenterX;
		dCenterY = rData.dCenterY;
		dOffset = rData.dOffset;
		nCurveDir = rData.nCurveDir;
		dXAxisRotAngle = rData.dXAxisRotAngle;
		bProjection = rData.bProjection;
		nGradRotAxis = rData.nGradRotAxis;
		dGradRotAngle = rData.dGradRotAngle;
		nInputType    = 1;
		nInsertPoint = 0;
		InsertElemKey = 0;
		nXAxisDirectionIJ = 0;
		dOffsetY = 0.;
		dOffsetZ = 0.;
		GroupKey          = 0;
		nNoTensionCalc    = 0;
		dNoTensionBeginLen= 0.0;
		dNoTensionEndLen  = 0.0;
		bTypicalTendon = FALSE;
		dTendonNum = 1.0;
		dDebondBeginLen   = 0.0;
		dDebondEndLen     = 0.0;
	}
	void Convert630(T_TDNA_UDRD_D_CH_RW_630& rData)
	{
		key = rData.key;
		memcpy(TendonName, rData.TendonName, sizeof(TendonName));
		TendonTypeKey = rData.TendonTypeKey;
		nCountElemList = rData.nCountElemList;
		nCurveType = rData.nCurveType;
		dBeginLength = rData.dBeginLength;
		dEndLength = rData.dEndLength;
		nCountProfile = rData.nCountProfile;
		nCountProfileY = rData.nCountProfileY;
		nCountProfileZ = rData.nCountProfileZ;
		nTendonShape = rData.nTendonShape;
		dInsertPointX = rData.dInsertPointX;
		dInsertPointY = rData.dInsertPointY;
		dInsertPointZ = rData.dInsertPointZ;
		nXAxisDirection = rData.nXAxisDirection;
		dVectorX = rData.dVectorX;
		dVectorY = rData.dVectorY;
		dCenterX = rData.dCenterX;
		dCenterY = rData.dCenterY;
		dOffset = rData.dOffset;
		nCurveDir = rData.nCurveDir;
		dXAxisRotAngle = rData.dXAxisRotAngle;
		bProjection = rData.bProjection;
		nGradRotAxis = rData.nGradRotAxis;
		dGradRotAngle = rData.dGradRotAngle;
		nInputType    = rData.nInputType;
		nInsertPoint = rData.nInsertPoint;
		InsertElemKey = rData.InsertElemKey;
		nXAxisDirectionIJ = rData.nXAxisDirectionIJ;
		dOffsetY = rData.dOffsetY;
		dOffsetZ = rData.dOffsetZ;
		GroupKey          = 0;
		nNoTensionCalc    = 0;
		dNoTensionBeginLen= 0.0;
		dNoTensionEndLen  = 0.0;
		bTypicalTendon = FALSE;
		dTendonNum = 1.0;
		dDebondBeginLen   = 0.0;
		dDebondEndLen     = 0.0;
	}
	void Convert692(T_TDNA_UDRD_D_CH_RW_692& rData)
	{
		key = rData.key;
		memcpy(TendonName, rData.TendonName, sizeof(TendonName));
		TendonTypeKey = rData.TendonTypeKey;
		nCountElemList = rData.nCountElemList;
		nCurveType = rData.nCurveType;
		dBeginLength = rData.dBeginLength;
		dEndLength = rData.dEndLength;
		nCountProfile = rData.nCountProfile;
		nCountProfileY = rData.nCountProfileY;
		nCountProfileZ = rData.nCountProfileZ;
		nTendonShape = rData.nTendonShape;
		dInsertPointX = rData.dInsertPointX;
		dInsertPointY = rData.dInsertPointY;
		dInsertPointZ = rData.dInsertPointZ;
		nXAxisDirection = rData.nXAxisDirection;
		dVectorX = rData.dVectorX;
		dVectorY = rData.dVectorY;
		dCenterX = rData.dCenterX;
		dCenterY = rData.dCenterY;
		dOffset = rData.dOffset;
		nCurveDir = rData.nCurveDir;
		dXAxisRotAngle = rData.dXAxisRotAngle;
		bProjection = rData.bProjection;
		nGradRotAxis = rData.nGradRotAxis;
		dGradRotAngle = rData.dGradRotAngle;
		nInputType    = rData.nInputType;
		nInsertPoint = rData.nInsertPoint;
		InsertElemKey = rData.InsertElemKey;
		nXAxisDirectionIJ = rData.nXAxisDirectionIJ;
		dOffsetY = rData.dOffsetY;
		dOffsetZ = rData.dOffsetZ;
		GroupKey          = rData.GroupKey;
		nNoTensionCalc    = rData.nNoTensionCalc;
		dNoTensionBeginLen= rData.dNoTensionBeginLen;
		dNoTensionEndLen  = rData.dNoTensionEndLen;
		bTypicalTendon = FALSE;
		dTendonNum = 1.0;
		dDebondBeginLen   = 0.0;
		dDebondEndLen     = 0.0;
	}
	void Convert896(T_TDNA_UDRD_D_CH_RW_896& rData)
	{
		key = rData.key;
		memcpy(TendonName, rData.TendonName, sizeof(TendonName));
		TendonTypeKey = rData.TendonTypeKey;
		nCountElemList = rData.nCountElemList;
		nCurveType = rData.nCurveType;
		dBeginLength = rData.dBeginLength;
		dEndLength = rData.dEndLength;
		nCountProfile = rData.nCountProfile;
		nCountProfileY = rData.nCountProfileY;
		nCountProfileZ = rData.nCountProfileZ;
		nTendonShape = rData.nTendonShape;
		dInsertPointX = rData.dInsertPointX;
		dInsertPointY = rData.dInsertPointY;
		dInsertPointZ = rData.dInsertPointZ;
		nXAxisDirection = rData.nXAxisDirection;
		dVectorX = rData.dVectorX;
		dVectorY = rData.dVectorY;
		dCenterX = rData.dCenterX;
		dCenterY = rData.dCenterY;
		dOffset = rData.dOffset;
		nCurveDir = rData.nCurveDir;
		dXAxisRotAngle = rData.dXAxisRotAngle;
		bProjection = rData.bProjection;
		nGradRotAxis = rData.nGradRotAxis;
		dGradRotAngle = rData.dGradRotAngle;
		nInputType    = rData.nInputType;
		nInsertPoint = rData.nInsertPoint;
		InsertElemKey = rData.InsertElemKey;
		nXAxisDirectionIJ = rData.nXAxisDirectionIJ;
		dOffsetY = rData.dOffsetY;
		dOffsetZ = rData.dOffsetZ;
		GroupKey          = rData.GroupKey;
		nNoTensionCalc    = rData.nNoTensionCalc;
		dNoTensionBeginLen= rData.dNoTensionBeginLen;
		dNoTensionEndLen  = rData.dNoTensionEndLen;
		bTypicalTendon = rData.bTypicalTendon;
		dTendonNum = rData.dTendonNum;
		dDebondBeginLen   = 0.0;
		dDebondEndLen     = 0.0;
	}
};
struct T_TDNA_UNIT
{
	T_TDNA_UNIT()
	{
		dBeginLength = D_UNITSYS_BASE_LENGTH;
		dEndLength = D_UNITSYS_BASE_LENGTH;
		dInsertPointX = dInsertPointY = dInsertPointZ = D_UNITSYS_BASE_LENGTH;
		dVectorX = dVectorY = D_UNITSYS_BASE_LENGTH;
		dCenterX = dCenterY = D_UNITSYS_BASE_LENGTH;
		dOffset = D_UNITSYS_BASE_LENGTH;
		dXAxisRotAngle = D_UNITSYS_BASE_DEGREE;
		dGradRotAngle = D_UNITSYS_BASE_DEGREE;
		dOffsetY = D_UNITSYS_BASE_LENGTH;
		dOffsetZ = D_UNITSYS_BASE_LENGTH;
		dNoTensionBeginLen = D_UNITSYS_BASE_LENGTH;
		dNoTensionEndLen = D_UNITSYS_BASE_LENGTH;
		dDebondBeginLen = D_UNITSYS_BASE_LENGTH;
		dDebondEndLen = D_UNITSYS_BASE_LENGTH;
	}
	int dBeginLength;
	int dEndLength;
	T_TDNA_BASE_UNIT Profile;
	T_TDNA_BASE_2D_UNIT Profile2D;
	int dInsertPointX;
	int dInsertPointY;
	int dInsertPointZ;
	int dVectorX, dVectorY;  // nXAxisDirection=Vector인 경우
	int dCenterX, dCenterY;  // 반경 중심
	int dOffset;             // 반경방향 오프셋
	int dXAxisRotAngle;      // Profile x축의 회전각
	int dGradRotAngle;       // 종구배 회전각
	int dOffsetY;
	int dOffsetZ;
	int dNoTensionBeginLen;
	int dNoTensionEndLen;
	int dDebondBeginLen;
	int dDebondEndLen;
};
#define HASHSIZETDNA 101

// Tendon Prestress Load
#define T_TDPL_KEY unsigned int
union T_TDPL_K
{
	T_TDPL_KEY keymap;
	struct 
	{
		unsigned int entity : 20; // T_TDNA_K
		unsigned int serial : 12;
	} key;
};

struct T_TDPL_D_ALPHA
{
	T_STLD_K LoadCaseKey;
	T_LDGR_K GroupKey;
	int nValueType; // 0=Stress, 1=Force;
	double dValueBegin[2];  // 0 for Stress, 1 for Force
	double dValueEnd[2];    // 0 for Stress, 1 for Force
};
struct T_TDPL_D_501
{
	T_STLD_K LoadCaseKey;
	T_LDGR_K GroupKey;
	int nOrder;     // 0 : begin 먼저, 1 : end 먼저
	int nValueType; // 0=Stress, 1=Force;
	double dValueBegin[2];  // 0 for Stress, 1 for Force
	double dValueEnd[2];    // 0 for Stress, 1 for Force
};
struct T_TDPL_D
{
	T_STLD_K LoadCaseKey;
	T_LDGR_K GroupKey;
	int nOrder;     // 0 : begin 먼저, 1 : end 먼저, 2 : both(Both는 6.1.0에 추가됨)
	int nValueType; // 0=Stress, 1=Force;
	double dValueBegin[2];  // 0 for Stress, 1 for Force
	double dValueEnd[2];    // 0 for Stress, 1 for Force
	int    nGrouting;
	
	void Initialize()
	{
		LoadCaseKey = 0;
		GroupKey = 0;
		nOrder = 0;
		nValueType = 0;
		dValueBegin[0] = dValueBegin[1] = 0;
		dValueEnd[0] = dValueEnd[1] = 0;
		nGrouting = 0;
	}
	void ConvertAlpha(T_TDPL_D_ALPHA& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		GroupKey = rData.GroupKey;
		nOrder = 0;
		nValueType = rData.nValueType;
		memcpy(dValueBegin, rData.dValueBegin, sizeof(dValueBegin));
		memcpy(dValueEnd, rData.dValueEnd, sizeof(dValueEnd));
		nGrouting = 0;
	}
	void Convert501(T_TDPL_D_501& rData)
	{
		LoadCaseKey = rData.LoadCaseKey;
		GroupKey = rData.GroupKey;
		nOrder = rData.nOrder;
		nValueType = rData.nValueType;
		memcpy(dValueBegin, rData.dValueBegin, sizeof(dValueBegin));
		memcpy(dValueEnd, rData.dValueEnd, sizeof(dValueEnd));
		nGrouting = 0;
	}
};
struct T_TDPL_UDRD_D_ALPHA
{
	T_TDPL_K key;
	T_TDPL_D_ALPHA data;
};
struct T_TDPL_UDRD_D_501
{
	T_TDPL_K key;
	T_TDPL_D_501 data;
};
struct T_TDPL_UDRD_D
{
	T_TDPL_K key;
	T_TDPL_D data;
};
struct T_TDPL_UNIT
{
	T_TDPL_UNIT()
	{
		dStressValue = D_UNITSYS_BASE_STRESS;
		dForceValue = D_UNITSYS_BASE_FORCE;
	}
	int dStressValue;
	int dForceValue;
}; 
#define HASHSIZETDPL 101

// Tendon Location for Composite Section
#define T_TDCS_K unsigned int
struct T_TDCS_D
{
	T_TDNA_K TdnaK;
	T_CSCS_K CscsK;
	int nPart;
	
	void Initialize()
	{
		TdnaK = 0;
		CscsK = 0;
		nPart = 0;
	}
};
struct T_TDCS_UDRD_D
{
	T_TDCS_K key;
	T_TDCS_D data;
};
struct T_TDCS_UNIT
{
	T_TDCS_UNIT()
	{
		// Nothing
	}
}; 
#define HASHSIZETDCS 101

// stage analysis control
#define T_STCT_K unsigned int
#define D_STCT_NUMSTCT  15
#define D_STCT_EREC_MAX_GEN 20
#define D_STCT_EREC_MAX_CVL 20   // D_SGLD_ER20_CVL 까지
struct T_STCT_BASE
{
	T_STLD_K LoadCaseKey; 
	void Initialize()
	{
		LoadCaseKey=0;    
	}
};
struct T_GILC_BASE
{
	UINT GilcKey; 
	void Initialize()
	{
		GilcKey=0;    
	}
};
struct T_STCT_EREC
{
	CString  strErection;
	CString  LoadCaseType;
	int      nNumStctCase;
	T_STLD_K LoadCaseKey[D_STCT_NUMSTCT];

	void Initialize()
	{
		strErection = "";

#if defined(_US) || defined(_RUS)
		LoadCaseType = _T("DW");
#else
		LoadCaseType = _T("D");
#endif
		nNumStctCase = 0;
		for(int i=0; i<D_STCT_NUMSTCT; i++) LoadCaseKey[i] = 0;
	}
};
struct T_STCT_EREC_CH
{
	char     strErection[40];
	char     LoadCaseType[20];
	int      nNumStctCase;
	T_STLD_K LoadCaseKey[D_STCT_NUMSTCT];
	
	void ConvertToString(T_STCT_EREC& rData)
	{    
		ConvertCharStr(strErection, rData.strErection, sizeof(strErection));
		ConvertCharStr(LoadCaseType, rData.LoadCaseType, sizeof(LoadCaseType));
		rData.nNumStctCase = nNumStctCase;
		for(int i=0; i<D_STCT_NUMSTCT; i++) rData.LoadCaseKey[i] = 0;
		for(int i=0; i< nNumStctCase; i++) rData.LoadCaseKey[i] = LoadCaseKey[i];
	}
	void ConvertToChar(T_STCT_EREC& rData)
	{
		ConvertStrChar(rData.strErection, strErection, sizeof(strErection));
		ConvertStrChar(rData.LoadCaseType, LoadCaseType, sizeof(LoadCaseType));
		nNumStctCase = rData.nNumStctCase;
		for(int i=0; i<D_STCT_NUMSTCT; i++) LoadCaseKey[i] = 0;
		for(int i=0; i<nNumStctCase; i++)  LoadCaseKey[i] = rData.LoadCaseKey[i];
	}
};

struct T_STCT_LOADSTEP
{
	int nStep;
	double dIncreFactor;
	BOOL bSaveStep;

	void Initialize()
	{
		nStep = 1;
		dIncreFactor = 1.;
		bSaveStep = FALSE;
	}
};

struct T_STCT_NLCTRL
{
	BOOL		bConsiderGeoNLEffect;					  // MEC 추가 <- 사용 안 함
	int			nNumOfIncre;								  // MEC 추가
	int			nLoadStepOption;							  // MEC 추가	0 : Num of Incre, 1 : Manual
	int			nOutputReq;								  // MEC 추가	0 : Every Increment, 1 : Last Increment
	CArray<T_STCT_LOADSTEP, T_STCT_LOADSTEP&> aLoadStep;	  // MEC 추가
	BOOL		bConv[3];								  // MEC 추가	0 : Displacement, 1 : Load, 2 : Work
	double		dConv[3];								  // MEC 추가	0 : Displacement, 1 : Load, 2 : Work
	T_NLCT_NLCTRL_PARM Param;

	void Initialize()
	{
		nNumOfIncre = 1;
		bConsiderGeoNLEffect = FALSE;
		nLoadStepOption = 0;
		nOutputReq = 0;
		aLoadStep.RemoveAll();
		bConv[0] = FALSE;
		bConv[1] = TRUE;
		bConv[2] = TRUE;
		dConv[0] = 0.001;
		dConv[1] = 0.001;
		dConv[2] = 1.e-6;
		Param.Initialize();
		Param.nMaxBisectLevel = 0;
	}
	T_STCT_NLCTRL& operator=(T_STCT_NLCTRL& src)
	{
		nNumOfIncre = src.nNumOfIncre;
		bConsiderGeoNLEffect = src.bConsiderGeoNLEffect;
		nLoadStepOption = src.nLoadStepOption;
		nOutputReq = src.nOutputReq;
		aLoadStep.Copy(src.aLoadStep);
		for (int i = 0; i < 3; i++)bConv[i] = src.bConv[i];
		for (int i = 0; i < 3; i++)dConv[i] = src.dConv[i];
		Param = src.Param;
		return *this;
	}
};

struct T_STCT_NLCTRL_UDRD_D_CH_RW
{
	BOOL		bConsiderGeoNLEffect;		// MEC 추가
	int			nNumOfIncre;					// MEC 추가
	int			nLoadStepOption;				// MEC 추가	0 : Num of Incre, 1 : Manual
	int			nOutputReq;					// MEC 추가	0 : Every Increment, 1 : Last Increment
	int			nLoadStepCount;
	BOOL		bConv[3];					// MEC 추가	0 : Displacement, 1 : Load, 2 : Work
	double		dConv[3];					// MEC 추가	0 : Displacement, 1 : Load, 2 : Work
	T_NLCT_NLCTRL_PARM Param;

	void Initialize()
	{
		nNumOfIncre = 1;
		bConsiderGeoNLEffect = FALSE;
		nLoadStepOption = 0;
		nOutputReq = 0;
		nLoadStepCount = 0;
		bConv[0] = FALSE;
		bConv[1] = TRUE;
		bConv[2] = TRUE;
		dConv[0] = 0.001;
		dConv[1] = 0.001;
		dConv[2] = 1.e-6;
		Param.Initialize();
		Param.nMaxBisectLevel = 0;
	}
};

struct T_STCT_D_ALPHA
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bVariation;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
};
struct T_STCT_D_500
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;

	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bIncludeTendonTensionLossEffct;

	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bVariation;

	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force

	BOOL      bTTLEElasticShortening;
	BOOL      bTTLECreepShrinkage;
};
struct T_STCT_D_501
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;

	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;

	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;

	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
};
struct T_STCT_D_530
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;

	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;

	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;

	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
};
struct T_STCT_D_530US
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;

	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;

	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;

	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nNumStctCase;
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
};
struct T_STCT_D_550
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;

	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;

	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;

	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force

	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
};
struct T_STCT_D_560
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;

	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;

	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;

	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force

	int       nCablePretensionForceCtrl; // 0:Internal, 1:External

	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
};
struct T_STCT_D_580
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;

	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;

	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;

	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force

	int       nCablePretensionForceCtrl; // 0:Internal, 1:External

	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
};
struct T_STCT_D_590
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;

	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;

	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;

	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force

	int       nCablePretensionForceCtrl; // 0:Internal, 1:External

	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우
																					// 0:Add, 1:Replace  
};
struct T_STCT_D_600
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;

	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;

	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;

	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force

	int       nCablePretensionForceCtrl; // 0:Internal, 1:External

	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우
																					// 0:Add, 1:Replace
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
};
struct T_STCT_D_610
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;

	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;

	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;

	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force

	int       nCablePretensionForceCtrl; // 0:Internal, 1:External

	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우
																					// 0:Add, 1:Replace
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용)   
};
struct T_STCT_D_630
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;

	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;

	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;

	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force

	int       nCablePretensionForceCtrl; // 0:Internal, 1:External

	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우
																					// 0:Add, 1:Replace
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	CString   LoadCaseType;                 // 6.2.0 추가 (Civil만 사용) 
};
struct T_STCT_D_650
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;

	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;

	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;

	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force

	int       nCablePretensionForceCtrl; // 0:Internal, 1:External

	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우
																					// 0:Add, 1:Replace
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	CString   LoadCaseType;                 // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)  
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;                 // 6.4.0 추가 (Civil만 사용)    
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
};
struct T_STCT_D_670
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우 0:Add, 1:Replace
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	CString   LoadCaseType;                 // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)  
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;                 // 6.4.0 추가 (Civil만 사용)    
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
};
struct T_STCT_D_692
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우 0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	CString   LoadCaseType;                 // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)  
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;                 // 6.4.0 추가 (Civil만 사용)    
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)
};
struct T_STCT_D_712
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우 0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	CString   LoadCaseType;                 // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)  
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;                 // 6.4.0 추가 (Civil만 사용)    
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)
	BOOL      bIncludePDeltaEffect;         // 7.0.0 추가 (Civil만 사용)
};
struct T_STCT_D_830
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우 0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	CString   LoadCaseType;                 // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)  
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;                 // 6.4.0 추가 (Civil만 사용)    
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)
	BOOL      bIncludePDeltaEffect;         // 7.0.0 추가 (Civil만 사용)
	BOOL      bRestartStage;                // 7.2.0 추가 (Civil만 사용)
	CArray<UINT, UINT> aRestartStagK;       // 7.2.0 추가 (Civil만 사용)
	BOOL      bApplyInitMemberForce;        // 7.3.0 추가 (Civil만 사용)
	BOOL      bChangeCableElementForPostCS; // 7.3.0 추가 (Civil만 사용)
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가
};
struct T_STCT_D_842
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우 0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	CString   LoadCaseType;                 // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)  
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;                 // 6.4.0 추가 (Civil만 사용)    
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)
	BOOL      bIncludePDeltaEffect;         // 7.0.0 추가 (Civil만 사용)
	BOOL      bRestartStage;                // 7.2.0 추가 (Civil만 사용)
	CArray<UINT, UINT> aRestartStagK;       // 7.2.0 추가 (Civil만 사용)
	BOOL      bApplyInitMemberForce;        // 7.3.0 추가 (Civil만 사용)
	BOOL      bChangeCableElementForPostCS; // 7.3.0 추가 (Civil만 사용)
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가
	CString   LoadCaseType2;                // 8.3.1 추가
	CString   LoadCaseType3;                // 8.3.1 추가
	int       nNumStctCase2;                // 8.3.1 추가
	int       nNumStctCase3;                // 8.3.1 추가
	T_STCT_BASE StctCase2[D_STCT_NUMSTCT];  // 8.3.1 추가
	T_STCT_BASE StctCase3[D_STCT_NUMSTCT];  // 8.3.1 추가
	CString   strErection1;                 // 8.3.1 추가 
	CString   strErection2;                 // 8.3.1 추가 
	CString   strErection3;                 // 8.3.1 추가 
	BOOL      bApplyCamber;                 // 8.3.1 추가 
};
struct T_STCT_D_865
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우 0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	CString   LoadCaseType;                 // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)  
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;                 // 6.4.0 추가 (Civil만 사용)    
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)
	BOOL      bIncludePDeltaEffect;         // 7.0.0 추가 (Civil만 사용)
	BOOL      bRestartStage;                // 7.2.0 추가 (Civil만 사용)
	CArray<UINT, UINT> aRestartStagK;       // 7.2.0 추가 (Civil만 사용)
	BOOL      bApplyInitMemberForce;        // 7.3.0 추가 (Civil만 사용)
	BOOL      bChangeCableElementForPostCS; // 7.3.0 추가 (Civil만 사용)
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가
	CString   LoadCaseType2;                // 8.3.1 추가
	CString   LoadCaseType3;                // 8.3.1 추가
	int       nNumStctCase2;                // 8.3.1 추가
	int       nNumStctCase3;                // 8.3.1 추가
	T_STCT_BASE StctCase2[D_STCT_NUMSTCT];  // 8.3.1 추가
	T_STCT_BASE StctCase3[D_STCT_NUMSTCT];  // 8.3.1 추가
	CString   strErection1;                 // 8.3.1 추가 
	CString   strErection2;                 // 8.3.1 추가 
	CString   strErection3;                 // 8.3.1 추가 
	BOOL      bApplyCamber;                 // 8.3.1 추가 
	BOOL      bSelfCons;                    // 8.4.5 추가
};
struct T_STCT_D_883
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	int       nIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우 0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	CString   LoadCaseType;                 // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)  
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;                 // 6.4.0 추가 (Civil만 사용)    
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)
	BOOL      bIncludePDeltaEffect;         // 7.0.0 추가 (Civil만 사용)
	BOOL      bRestartStage;                // 7.2.0 추가 (Civil만 사용)
	CArray<UINT, UINT> aRestartStagK;       // 7.2.0 추가 (Civil만 사용)
	BOOL      bApplyInitMemberForce;        // 7.3.0 추가 (Civil만 사용)
	BOOL      bChangeCableElementForPostCS; // 7.3.0 추가 (Civil만 사용)
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가
	CString   LoadCaseType2;                // 8.3.1 추가
	CString   LoadCaseType3;                // 8.3.1 추가
	int       nNumStctCase2;                // 8.3.1 추가
	int       nNumStctCase3;                // 8.3.1 추가
	T_STCT_BASE StctCase2[D_STCT_NUMSTCT];  // 8.3.1 추가
	T_STCT_BASE StctCase3[D_STCT_NUMSTCT];  // 8.3.1 추가
	CString   strErection1;                 // 8.3.1 추가 
	CString   strErection2;                 // 8.3.1 추가 
	CString   strErection3;                 // 8.3.1 추가 
	BOOL      bApplyCamber;                 // 8.3.1 추가 
	BOOL      bSelfCons;                    // 8.4.5 추가
	BOOL      bConvergenceFail;             // 8.7.0 추가
	int       nMaxBiSectLoadStep;           // 8.7.0 추가
	int       nMaxDivergedStep;             // 8.7.0 추가
};

struct T_STCT_D_960
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	int       nIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl;    // 0:Internal, 1:External
	int       nNumStctCase_NotUse;                  // 8.8.5 부터 사용하지 않음
	T_STCT_BASE StctCase_NotUse[D_STCT_NUMSTCT];    // 8.8.5 부터 사용하지 않음
	BOOL      bCalcCompSectionPart;         // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우 0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	CString   LoadCaseType_NotUse;          // 6.2.0 추가 (Civil만 사용) - 8.8.5 부터 사용하지 않음
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)  
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;                 // 6.4.0 추가 (Civil만 사용)    
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)
	BOOL      bIncludePDeltaEffect;         // 7.0.0 추가 (Civil만 사용)
	BOOL      bRestartStage;                // 7.2.0 추가 (Civil만 사용)
	CArray<UINT, UINT> aRestartStagK;       // 7.2.0 추가 (Civil만 사용)
	BOOL      bApplyInitMemberForce;        // 7.3.0 추가 (Civil만 사용)
	BOOL      bChangeCableElementForPostCS; // 7.3.0 추가 (Civil만 사용)
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가
	CString   LoadCaseType2_NotUse;         // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	CString   LoadCaseType3_NotUse;         // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	int       nNumStctCase2_NotUse;         // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	int       nNumStctCase3_NotUse;         // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	T_STCT_BASE StctCase2_NotUse[D_STCT_NUMSTCT];  // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	T_STCT_BASE StctCase3_NotUse[D_STCT_NUMSTCT];  // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	CString   strErection1_NotUse;          // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	CString   strErection2_NotUse;          // 8.3.1 추가 - 8.8.5 부터 사용하지 않음 
	CString   strErection3_NotUse;          // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	BOOL      bApplyCamber;                 // 8.3.1 추가 
	BOOL      bSelfCons;                    // 8.4.5 추가
	BOOL      bConvergenceFail;             // 8.7.0 추가
	int       nMaxBiSectLoadStep;           // 8.7.0 추가
	int       nMaxDivergedStep;             // 8.7.0 추가
	CArray<T_STCT_EREC, T_STCT_EREC&> aErection; // 8.8.5 추가
};

enum D_STCT_NL_ANAL {
	D_STCT_NL_ANAL_NONLINEAR,	// Nonlinear Analysis
	D_STCT_NL_ANAL_GEOMETRY,	// Geometric Nonlinear Analysis 
	D_STCT_NL_ANAL_MATERIAL,	// Material Nonlinear Analysis
	D_STCT_NL_ANAL_GEOM_MAT,		// Geom + Mat Nonlinear Analysis
};

struct T_STCT_D
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	int       nIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl;    // 0:Internal, 1:External
	int       nNumStctCase_NotUse;                  // 8.8.5 부터 사용하지 않음
	T_STCT_BASE StctCase_NotUse[D_STCT_NUMSTCT];    // 8.8.5 부터 사용하지 않음
	BOOL      bCalcCompSectionPart;         // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우 0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	CString   LoadCaseType_NotUse;          // 6.2.0 추가 (Civil만 사용) - 8.8.5 부터 사용하지 않음
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)  
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;                 // 6.4.0 추가 (Civil만 사용)    
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)
	BOOL      bIncludePDeltaEffect;         // 7.0.0 추가 (Civil만 사용)
	BOOL      bRestartStage;                // 7.2.0 추가 (Civil만 사용)
	CArray<UINT, UINT> aRestartStagK;       // 7.2.0 추가 (Civil만 사용)
	BOOL      bApplyInitMemberForce;        // 7.3.0 추가 (Civil만 사용)
	BOOL      bChangeCableElementForPostCS; // 7.3.0 추가 (Civil만 사용)
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가
	CString   LoadCaseType2_NotUse;         // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	CString   LoadCaseType3_NotUse;         // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	int       nNumStctCase2_NotUse;         // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	int       nNumStctCase3_NotUse;         // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	T_STCT_BASE StctCase2_NotUse[D_STCT_NUMSTCT];  // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	T_STCT_BASE StctCase3_NotUse[D_STCT_NUMSTCT];  // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	CString   strErection1_NotUse;          // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	CString   strErection2_NotUse;          // 8.3.1 추가 - 8.8.5 부터 사용하지 않음 
	CString   strErection3_NotUse;          // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	BOOL      bApplyCamber;                 // 8.3.1 추가 
	BOOL      bSelfCons;                    // 8.4.5 추가
	BOOL      bConvergenceFail;             // 8.7.0 추가
	int       nMaxBiSectLoadStep;           // 8.7.0 추가
	int       nMaxDivergedStep;             // 8.7.0 추가
	CArray<T_STCT_EREC, T_STCT_EREC&> aErection; // 8.8.5 추가
	T_STCT_NLCTRL NlCtrl;					// MEC 추가

	T_STCT_D() {}
	T_STCT_D(T_STCT_D& src) { *this = src; }
	T_STCT_D& operator=(T_STCT_D& src)
	{
		nFinalStage = src.nFinalStage;
		FinalStagKey = src.FinalStagKey;
		bIncludeTimeDependentEffect = src.bIncludeTimeDependentEffect;
		nIncludeNonLinearAnalysis = src.nIncludeNonLinearAnalysis;
		bCreepShrinkage = src.bCreepShrinkage;
		nType = src.nType;
		nIter = src.nIter;
		dTol = src.dTol;
		bOnlyUserCreepCoeff = src.bOnlyUserCreepCoeff;
		bInternalTimeStep = src.bInternalTimeStep;
		nInternalTimeStep = src.nInternalTimeStep;
		bAutoTimeStep = src.bAutoTimeStep;
		memcpy(nAutoTimeStep, src.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLEElasticShortening = src.bTTLEElasticShortening;
		bVariation = src.bVariation;
		bTTLECreepShrinkage = src.bTTLECreepShrinkage;
		nMaxIterPerLoadStep = src.nMaxIterPerLoadStep;
		memcpy(bConvFlag, src.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, src.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = src.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = src.nNumStctCase_NotUse;
		memcpy(StctCase_NotUse, src.StctCase_NotUse, sizeof(StctCase_NotUse));
		bCalcCompSectionPart = src.bCalcCompSectionPart;
		nCablePretensionExternalOpt = src.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = src.bConvFinalStageMemberForce;
		bConvTruss = src.bConvTruss;
		bConvBeam = src.bConvBeam;
		nNumLoadStep = src.nNumLoadStep;
		bInitTangentDisp = src.bInitTangentDisp;
		nInitTangentDisp = src.nInitTangentDisp;
		KeyGrup = src.KeyGrup;
		bCalcConcurrentForce = src.bCalcConcurrentForce;
		LoadCaseType_NotUse = src.LoadCaseType_NotUse;
		bSecondaryDLEffect = src.bSecondaryDLEffect;
		nNumGilcCase = src.nNumGilcCase;
		memcpy(GilcCase, src.GilcCase, sizeof(GilcCase));
		bConsiderConfinement = src.bConsiderConfinement;
		bConsiderEndPartStress = src.bConsiderEndPartStress;
		nEndPartStress = src.nEndPartStress;
		dStressRatio = src.dStressRatio;
		nBeamSectionChange = src.nBeamSectionChange;
		nNonlAnalType = src.nNonlAnalType;
		bIncludeEquiMembForce = src.bIncludeEquiMembForce;
		bSaveCurStageOutput = src.bSaveCurStageOutput;
		bLackofFitForce = src.bLackofFitForce;
		KeyGrup4LackofFit = src.KeyGrup4LackofFit;
		bIncludePDeltaEffect = src.bIncludePDeltaEffect;
		bRestartStage = src.bRestartStage;
		aRestartStagK.Copy(src.aRestartStagK);
		bApplyInitMemberForce = src.bApplyInitMemberForce;
		bChangeCableElementForPostCS = src.bChangeCableElementForPostCS;
		bApplyTDElastic = src.bApplyTDElastic;
		nTndnTensType = src.nTndnTensType;
		LoadCaseType2_NotUse = src.LoadCaseType2_NotUse;
		LoadCaseType3_NotUse = src.LoadCaseType3_NotUse;
		nNumStctCase2_NotUse = src.nNumStctCase2_NotUse;
		nNumStctCase3_NotUse = src.nNumStctCase3_NotUse;
		memcpy(StctCase2_NotUse, src.StctCase2_NotUse, sizeof(StctCase2_NotUse));
		memcpy(StctCase3_NotUse, src.StctCase3_NotUse, sizeof(StctCase3_NotUse));
		strErection1_NotUse  = src.strErection1_NotUse;
		strErection2_NotUse  = src.strErection2_NotUse;
		strErection3_NotUse  = src.strErection3_NotUse;
		bApplyCamber  = src.bApplyCamber;
		bSelfCons = src.bSelfCons;
		bConvergenceFail = src.bConvergenceFail;
		nMaxBiSectLoadStep = src.nMaxBiSectLoadStep;
		nMaxDivergedStep = src.nMaxDivergedStep;
		aErection.Copy(src.aErection);
		NlCtrl = src.NlCtrl;
		return *this;
	}

	void Initialize()
	{
		nFinalStage = 0;
		FinalStagKey = 0;
		bIncludeTimeDependentEffect = TRUE;
		nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = TRUE;
		nType = 3;
		nIter = 5;
		dTol = 0.01;
		bOnlyUserCreepCoeff=FALSE;
		bInternalTimeStep=FALSE;
		nInternalTimeStep=2;
		bAutoTimeStep=TRUE;
		nAutoTimeStep[0] = 2;
		nAutoTimeStep[1] = 5;
		nAutoTimeStep[2] = 7;
		nAutoTimeStep[3] = 10;
		nAutoTimeStep[4] = 20;
		bTTLECreepShrinkage = TRUE;
		bVariation = TRUE;
		bTTLEElasticShortening = TRUE;
		nMaxIterPerLoadStep = 30;
		bConvFlag[0] = FALSE;
		bConvFlag[1] = TRUE;
		bConvFlag[2] = FALSE;
		dConvValue[0] = 0.01;
		dConvValue[1] = 0.01;
		dConvValue[2] = 0.01;
		nCablePretensionForceCtrl = 0;
		nNumStctCase_NotUse = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();    
		bCalcCompSectionPart = TRUE;
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		LoadCaseType_NotUse = _T("DW");
#else
		LoadCaseType_NotUse = _T("D");
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();    
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;
#if defined(_CIVIL)
		bIncludeEquiMembForce = FALSE; // GEN 때문에 일부러 분리해두었음.
#else
		bIncludeEquiMembForce = FALSE; // GEN인 경우는 반드시 FALSE이어야 함.
#endif
		bSaveCurStageOutput = FALSE;   
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;
	
#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif

		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();    
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void ConvertAlpha(T_STCT_D_ALPHA& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		if (bIncludeTimeDependentEffect && nIncludeNonLinearAnalysis == 1)
			nIncludeNonLinearAnalysis = 0;  // 둘중 하나는 끈다.

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=FALSE;
		bInternalTimeStep=FALSE;
		nInternalTimeStep=2;
		bAutoTimeStep=FALSE;
		nAutoTimeStep[0] = 2;
		nAutoTimeStep[1] = 5;
		nAutoTimeStep[2] = 7;
		nAutoTimeStep[3] = 10;
		nAutoTimeStep[4] = 20;

		bTTLECreepShrinkage = FALSE;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = FALSE;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = 0;
		nNumStctCase_NotUse = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();
		bCalcCompSectionPart = TRUE;
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		LoadCaseType_NotUse = _T("DW");
#else
		LoadCaseType_NotUse = _T("D");
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		bConsiderConfinement = FALSE;
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;    
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;   
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert500(T_STCT_D_500& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		if (bIncludeTimeDependentEffect && nIncludeNonLinearAnalysis == 1)
			nIncludeNonLinearAnalysis = 0;  // 둘중 하나는 끈다.

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=FALSE;
		bInternalTimeStep=FALSE;
		nInternalTimeStep=2;
		bAutoTimeStep=FALSE;
		nAutoTimeStep[0] = 2;
		nAutoTimeStep[1] = 5;
		nAutoTimeStep[2] = 7;
		nAutoTimeStep[3] = 10;
		nAutoTimeStep[4] = 20;

		bTTLECreepShrinkage = rData.bIncludeTendonTensionLossEffct ? rData.bTTLECreepShrinkage : FALSE;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bIncludeTendonTensionLossEffct ? rData.bTTLEElasticShortening : FALSE;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = 0;

		nNumStctCase_NotUse = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();  
		bCalcCompSectionPart = TRUE;
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		LoadCaseType_NotUse = _T("DW");
#else
		LoadCaseType_NotUse = _T("D");
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;     
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert501(T_STCT_D_501& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=FALSE;
		bInternalTimeStep=FALSE;
		nInternalTimeStep=2;
		bAutoTimeStep=FALSE;
		nAutoTimeStep[0] = 2;
		nAutoTimeStep[1] = 5;
		nAutoTimeStep[2] = 7;
		nAutoTimeStep[3] = 10;
		nAutoTimeStep[4] = 20;

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		bConvFlag[0] = rData.bConvFlag[0];
		bConvFlag[1] = rData.bConvFlag[1];
		bConvFlag[2] = rData.bConvFlag[2];
		dConvValue[0] = rData.dConvValue[0];
		dConvValue[1] = rData.dConvValue[1];
		dConvValue[2] = rData.dConvValue[2];

		nCablePretensionForceCtrl = 0;

		nNumStctCase_NotUse = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();
		bCalcCompSectionPart = TRUE;    
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		LoadCaseType_NotUse = _T("DW");
#else
		LoadCaseType_NotUse = _T("D");
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;     
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert530(T_STCT_D_530& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = 0;

		nNumStctCase_NotUse = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();
		bCalcCompSectionPart = TRUE;
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		LoadCaseType_NotUse = _T("DW");
#else
		LoadCaseType_NotUse = _T("D");
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;      
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert530US(T_STCT_D_530US& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = 0;

		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;    
		bCalcCompSectionPart = TRUE;
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		LoadCaseType_NotUse = _T("DW");
#else
		LoadCaseType_NotUse = _T("D");
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;    
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert550(T_STCT_D_550& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;

		nNumStctCase_NotUse = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();           
		bCalcCompSectionPart = TRUE;
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		LoadCaseType_NotUse = _T("DW");
#else
		LoadCaseType_NotUse = _T("D");
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;     
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert560(T_STCT_D_560& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;

		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;    
		bCalcCompSectionPart = TRUE;
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		LoadCaseType_NotUse = _T("DW");
#else
		LoadCaseType_NotUse = _T("D");
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;      
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert580(T_STCT_D_580& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;

		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;    
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		LoadCaseType_NotUse = _T("DW");
#else
		LoadCaseType_NotUse = _T("D");
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;    
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert590(T_STCT_D_590& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;

		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;    
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		LoadCaseType_NotUse = _T("DW");
#else
		LoadCaseType_NotUse = _T("D");
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;     
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert600(T_STCT_D_600& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;

		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		LoadCaseType_NotUse = _T("DW");
#else
		LoadCaseType_NotUse = _T("D");
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;     
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert610(T_STCT_D_610& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;

		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
#if defined(_US) || defined(_RUS)
		LoadCaseType_NotUse = _T("DW");
#else
		LoadCaseType_NotUse = _T("D");
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;  
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert630(T_STCT_D_630& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		LoadCaseType_NotUse = rData.LoadCaseType;
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;  
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert650(T_STCT_D_650& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		LoadCaseType_NotUse = rData.LoadCaseType;
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;    
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert670(T_STCT_D_670& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		LoadCaseType_NotUse = rData.LoadCaseType;
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;                
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;     
		bSaveCurStageOutput = rData.bSaveCurStageOutput;    
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert692(T_STCT_D_692& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		LoadCaseType_NotUse = rData.LoadCaseType;
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;                
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;     
		bSaveCurStageOutput = rData.bSaveCurStageOutput;    
		bLackofFitForce = rData.bInternalTimeStep;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert712(T_STCT_D_712& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		LoadCaseType_NotUse = rData.LoadCaseType;
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;                
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;     
		bSaveCurStageOutput = rData.bSaveCurStageOutput;    
		bLackofFitForce = rData.bInternalTimeStep;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert830(T_STCT_D_830& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		LoadCaseType_NotUse = rData.LoadCaseType;
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;                
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;     
		bSaveCurStageOutput = rData.bSaveCurStageOutput;    
		bLackofFitForce = rData.bInternalTimeStep;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		aRestartStagK.Copy(rData.aRestartStagK);
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		
#if defined(_US) || defined(_RUS)
		LoadCaseType2_NotUse = _T("DW");
		LoadCaseType3_NotUse = _T("DW");
#else
		LoadCaseType2_NotUse = _T("D");
		LoadCaseType3_NotUse = _T("D");
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();  
		strErection1_NotUse = "";
		strErection2_NotUse = "";
		strErection3_NotUse = "";
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert842(T_STCT_D_842& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		LoadCaseType_NotUse = rData.LoadCaseType;
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;                
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;     
		bSaveCurStageOutput = rData.bSaveCurStageOutput;    
		bLackofFitForce = rData.bInternalTimeStep;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		aRestartStagK.Copy(rData.aRestartStagK);
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		LoadCaseType2_NotUse = rData.LoadCaseType2;
		LoadCaseType3_NotUse = rData.LoadCaseType3;
		nNumStctCase2_NotUse = rData.nNumStctCase2;
		nNumStctCase3_NotUse = rData.nNumStctCase3;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i] = rData.StctCase2[i];
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i] = rData.StctCase3[i];
		strErection1_NotUse = rData.strErection1;
		strErection2_NotUse = rData.strErection2;
		strErection3_NotUse = rData.strErection3;
		bApplyCamber = rData.bApplyCamber;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert865(T_STCT_D_865& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		LoadCaseType_NotUse = rData.LoadCaseType;
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;                
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;     
		bSaveCurStageOutput = rData.bSaveCurStageOutput;    
		bLackofFitForce = rData.bInternalTimeStep;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		aRestartStagK.Copy(rData.aRestartStagK);
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		LoadCaseType2_NotUse = rData.LoadCaseType2;
		LoadCaseType3_NotUse = rData.LoadCaseType3;
		nNumStctCase2_NotUse = rData.nNumStctCase2;
		nNumStctCase3_NotUse = rData.nNumStctCase3;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i] = rData.StctCase2[i];
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i] = rData.StctCase3[i];
		strErection1_NotUse = rData.strErection1;
		strErection2_NotUse = rData.strErection2;
		strErection3_NotUse = rData.strErection3;
		bApplyCamber = rData.bApplyCamber;
		bSelfCons = rData.bSelfCons;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert883(T_STCT_D_883& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		nIncludeNonLinearAnalysis = rData.nIncludeNonLinearAnalysis;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		LoadCaseType_NotUse = rData.LoadCaseType;
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;                
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;     
		bSaveCurStageOutput = rData.bSaveCurStageOutput;    
		bLackofFitForce = rData.bInternalTimeStep;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		aRestartStagK.Copy(rData.aRestartStagK);
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		LoadCaseType2_NotUse = rData.LoadCaseType2;
		LoadCaseType3_NotUse = rData.LoadCaseType3;
		nNumStctCase2_NotUse = rData.nNumStctCase2;
		nNumStctCase3_NotUse = rData.nNumStctCase3;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i] = rData.StctCase2[i];
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i] = rData.StctCase3[i];
		strErection1_NotUse = rData.strErection1;
		strErection2_NotUse = rData.strErection2;
		strErection3_NotUse = rData.strErection3;
		bApplyCamber = rData.bApplyCamber;
		bSelfCons = rData.bSelfCons;
		bConvergenceFail = rData.bConvergenceFail;
		nMaxBiSectLoadStep = rData.nMaxBiSectLoadStep;
		nMaxDivergedStep = rData.nMaxDivergedStep;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert960(T_STCT_D_960& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		nIncludeNonLinearAnalysis = rData.nIncludeNonLinearAnalysis;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff = rData.bOnlyUserCreepCoeff;
		bInternalTimeStep = rData.bInternalTimeStep;
		nInternalTimeStep = rData.nInternalTimeStep;
		bAutoTimeStep = rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase_NotUse;
		for (int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();
		for (int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase_NotUse[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		LoadCaseType_NotUse = rData.LoadCaseType_NotUse;
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;
		for (int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for (int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;
		bSaveCurStageOutput = rData.bSaveCurStageOutput;
		bLackofFitForce = rData.bInternalTimeStep;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		aRestartStagK.Copy(rData.aRestartStagK);
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		LoadCaseType2_NotUse = rData.LoadCaseType2_NotUse;
		LoadCaseType3_NotUse = rData.LoadCaseType3_NotUse;
		nNumStctCase2_NotUse = rData.nNumStctCase2_NotUse;
		nNumStctCase3_NotUse = rData.nNumStctCase3_NotUse;
		for (int i = 0; i < D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i] = rData.StctCase2_NotUse[i];
		for (int i = 0; i < D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i] = rData.StctCase3_NotUse[i];
		strErection1_NotUse = rData.strErection1_NotUse;
		strErection2_NotUse = rData.strErection2_NotUse;
		strErection3_NotUse = rData.strErection3_NotUse;
		bApplyCamber = rData.bApplyCamber;
		bSelfCons = rData.bSelfCons;
		bConvergenceFail = rData.bConvergenceFail;
		nMaxBiSectLoadStep = rData.nMaxBiSectLoadStep;
		nMaxDivergedStep = rData.nMaxDivergedStep;
		aErection.Copy(rData.aErection);
		NlCtrl.Initialize();
	}
	BOOL IsNlAnalGeom()
	{
		return nIncludeNonLinearAnalysis == D_STCT_NL_ANAL_GEOMETRY || nIncludeNonLinearAnalysis == D_STCT_NL_ANAL_GEOM_MAT;
	}
};
struct T_STCT_D_CH_630
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우
																					// 0:Add, 1:Replace
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	char      LoadCaseType[20];             // 6.2.0 추가 (Civil만 사용) 
};
struct T_STCT_D_CH_650
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우
																					// 0:Add, 1:Replace
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	char      LoadCaseType[20];             // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;   
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
};
struct T_STCT_D_CH_670
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우  0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	char      LoadCaseType[20];             // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;   
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
};
struct T_STCT_D_CH_692
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우  0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	char      LoadCaseType[20];             // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;   
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)   
};
struct T_STCT_D_CH_712
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우  0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	char      LoadCaseType[20];             // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;   
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)   
	BOOL      bIncludePDeltaEffect;         // 7.0.0 추가 (Civil만 사용)
};
struct T_STCT_D_CH_830
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우  0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	char      LoadCaseType[20];             // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;   
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)   
	BOOL      bIncludePDeltaEffect;         // 7.0.0 추가 (Civil만 사용)
	BOOL      bRestartStage;                // 7.2.0 추가 (Civil만 사용)
	CArray<UINT, UINT> aRestartStagK;       // 7.2.0 추가 (Civil만 사용)
	BOOL      bApplyInitMemberForce;        // 7.3.0 추가 (Civil만 사용)
	BOOL      bChangeCableElementForPostCS; // 7.3.0 추가 (Civil만 사용)
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가 
};
struct T_STCT_D_CH_842
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우  0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	char      LoadCaseType[20];             // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;   
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)   
	BOOL      bIncludePDeltaEffect;         // 7.0.0 추가 (Civil만 사용)
	BOOL      bRestartStage;                // 7.2.0 추가 (Civil만 사용)
	CArray<UINT, UINT> aRestartStagK;       // 7.2.0 추가 (Civil만 사용)
	BOOL      bApplyInitMemberForce;        // 7.3.0 추가 (Civil만 사용)
	BOOL      bChangeCableElementForPostCS; // 7.3.0 추가 (Civil만 사용)
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가 
	char      LoadCaseType2[20];            // 8.3.1 추가
	char      LoadCaseType3[20];            // 8.3.1 추가
	int       nNumStctCase2;                // 8.3.1 추가
	int       nNumStctCase3;                // 8.3.1 추가
	T_STCT_BASE StctCase2[D_STCT_NUMSTCT];  // 8.3.1 추가
	T_STCT_BASE StctCase3[D_STCT_NUMSTCT];  // 8.3.1 추가
	char      strErection1[40];             // 8.3.1 추가
	char      strErection2[40];             // 8.3.1 추가
	char      strErection3[40];             // 8.3.1 추가
	BOOL      bApplyCamber;                 // 8.3.1 추가
};
struct T_STCT_D_CH_865
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우  0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	char      LoadCaseType[20];             // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;   
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)   
	BOOL      bIncludePDeltaEffect;         // 7.0.0 추가 (Civil만 사용)
	BOOL      bRestartStage;                // 7.2.0 추가 (Civil만 사용)
	CArray<UINT, UINT> aRestartStagK;       // 7.2.0 추가 (Civil만 사용)
	BOOL      bApplyInitMemberForce;        // 7.3.0 추가 (Civil만 사용)
	BOOL      bChangeCableElementForPostCS; // 7.3.0 추가 (Civil만 사용)
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가 
	char      LoadCaseType2[20];            // 8.3.1 추가
	char      LoadCaseType3[20];            // 8.3.1 추가
	int       nNumStctCase2;                // 8.3.1 추가
	int       nNumStctCase3;                // 8.3.1 추가
	T_STCT_BASE StctCase2[D_STCT_NUMSTCT];  // 8.3.1 추가
	T_STCT_BASE StctCase3[D_STCT_NUMSTCT];  // 8.3.1 추가
	char      strErection1[40];             // 8.3.1 추가
	char      strErection2[40];             // 8.3.1 추가
	char      strErection3[40];             // 8.3.1 추가
	BOOL      bApplyCamber;                 // 8.3.1 추가
	BOOL      bSelfCons;                    // 8.4.5 추가
};
struct T_STCT_D_CH_883
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	int       nIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우  0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	char      LoadCaseType[20];             // 6.2.0 추가 (Civil만 사용) 
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;   
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)   
	BOOL      bIncludePDeltaEffect;         // 7.0.0 추가 (Civil만 사용)
	BOOL      bRestartStage;                // 7.2.0 추가 (Civil만 사용)
	CArray<UINT, UINT> aRestartStagK;       // 7.2.0 추가 (Civil만 사용)
	BOOL      bApplyInitMemberForce;        // 7.3.0 추가 (Civil만 사용)
	BOOL      bChangeCableElementForPostCS; // 7.3.0 추가 (Civil만 사용)
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가 
	char      LoadCaseType2[20];            // 8.3.1 추가
	char      LoadCaseType3[20];            // 8.3.1 추가
	int       nNumStctCase2;                // 8.3.1 추가
	int       nNumStctCase3;                // 8.3.1 추가
	T_STCT_BASE StctCase2[D_STCT_NUMSTCT];  // 8.3.1 추가
	T_STCT_BASE StctCase3[D_STCT_NUMSTCT];  // 8.3.1 추가
	char      strErection1[40];             // 8.3.1 추가
	char      strErection2[40];             // 8.3.1 추가
	char      strErection3[40];             // 8.3.1 추가
	BOOL      bApplyCamber;                 // 8.3.1 추가
	BOOL      bSelfCons;                    // 8.4.5 추가
	BOOL      bConvergenceFail;             // 8.7.0 추가
	int       nMaxBiSectLoadStep;           // 8.7.0 추가
	int       nMaxDivergedStep;             // 8.7.0 추가
};
struct T_STCT_D_CH_960
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	int       nIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase_NotUse;                 // 8.8.5 부터 사용하지 않음
	T_STCT_BASE StctCase_NotUse[D_STCT_NUMSTCT];   // 8.8.5 부터 사용하지 않음 
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우  0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	char      LoadCaseType_NotUse[20];      // 6.2.0 추가 (Civil만 사용) - 8.8.5 부터 사용하지 않음
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)   
	BOOL      bIncludePDeltaEffect;         // 7.0.0 추가 (Civil만 사용)
	BOOL      bRestartStage;                // 7.2.0 추가 (Civil만 사용)
	CArray<UINT, UINT> aRestartStagK;       // 7.2.0 추가 (Civil만 사용)
	BOOL      bApplyInitMemberForce;        // 7.3.0 추가 (Civil만 사용)
	BOOL      bChangeCableElementForPostCS; // 7.3.0 추가 (Civil만 사용)
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가 
	char      LoadCaseType2_NotUse[20];     // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	char      LoadCaseType3_NotUse[20];     // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	int       nNumStctCase2_NotUse;         // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	int       nNumStctCase3_NotUse;         // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	T_STCT_BASE StctCase2_NotUse[D_STCT_NUMSTCT];  // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	T_STCT_BASE StctCase3_NotUse[D_STCT_NUMSTCT];  // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	char      strErection1_NotUse[40];      // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	char      strErection2_NotUse[40];      // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	char      strErection3_NotUse[40];      // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	BOOL      bApplyCamber;                 // 8.3.1 추가
	BOOL      bSelfCons;                    // 8.4.5 추가
	BOOL      bConvergenceFail;             // 8.7.0 추가
	int       nMaxBiSectLoadStep;           // 8.7.0 추가
	int       nMaxDivergedStep;             // 8.7.0 추가
	CArray<T_STCT_EREC_CH, T_STCT_EREC_CH&> aErection; // 8.8.5 추가
};
struct T_STCT_D_CH
{
	int       nFinalStage;  // 0:Last Stage, 1:Other Stage
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	int       nIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;   // 1:Creep, 2:Shrinkage, 3:Creep and Shrinkage
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];       // index=0:Engergy, index=1:Displacement, index=2:Force
	double    dConvValue[3];     // index=0:Engergy, index=1:Displacement, index=2:Force
	int       nCablePretensionForceCtrl; // 0:Internal, 1:External
	int       nNumStctCase_NotUse;                 // 8.8.5 부터 사용하지 않음
	T_STCT_BASE StctCase_NotUse[D_STCT_NUMSTCT];   // 8.8.5 부터 사용하지 않음 
	BOOL      bCalcCompSectionPart;        // 5.7.0 추가
	int       nCablePretensionExternalOpt;  // 5.9.0 추가 nCablePretensionForceCtrl==1인경우  0:Add, 1:Replace                                          
	BOOL      bConvFinalStageMemberForce;   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvTruss;                   // 6.0.0 추가 (Civil만 사용)
	BOOL      bConvBeam;                    // 6.0.0 추가 (Civil만 사용)
	int       nNumLoadStep;                 // 6.0.0 추가 (Civil만 사용)
	BOOL      bInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 
	int       nInitTangentDisp;             // 6.1.0 추가 (Civil만 사용) 0=All, 1=Group
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;         // 6.1.0 추가 (Civil만 사용) 
	char      LoadCaseType_NotUse[20];      // 6.2.0 추가 (Civil만 사용) - 8.8.5 부터 사용하지 않음
	BOOL      bSecondaryDLEffect;           // 6.4.0 추가 (Civil만 사용)
	int       nNumGilcCase;                 // 6.4.0 추가 (Civil만 사용)
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];   // 6.4.0 추가 (Civil만 사용)  
	BOOL      bConsiderConfinement;         // 6.4.0 추가 (Civil만 사용) 
	BOOL      bConsiderEndPartStress;       // 6.4.0 추가 (Civil만 사용)
	int       nEndPartStress;               // 6.4.0 추가 (Civil만 사용)  0=Linear Interpolation, 1=0%
	double    dStressRatio;   
	int       nBeamSectionChange;           // 6.4.0 추가 (Civil만 사용)   0=Constant, 1=Change
	int       nNonlAnalType;                // 6.6.0 추가 (Civil만 사용)   0=Independent Stage, 1=Accumulative Stage
	BOOL      bIncludeEquiMembForce;        // 6.6.0 추가 (Civil만 사용)
	BOOL      bSaveCurStageOutput;          // 6.6.0 추가 (Civil만 사용)
	BOOL      bLackofFitForce;              // 6.7.1 추가 (Civil만 사용)   
	UINT      KeyGrup4LackofFit;            // 6.7.1 추가 (Civil만 사용)   
	BOOL      bIncludePDeltaEffect;         // 7.0.0 추가 (Civil만 사용)
	BOOL      bRestartStage;                // 7.2.0 추가 (Civil만 사용)
	CArray<UINT, UINT> aRestartStagK;       // 7.2.0 추가 (Civil만 사용)
	BOOL      bApplyInitMemberForce;        // 7.3.0 추가 (Civil만 사용)
	BOOL      bChangeCableElementForPostCS; // 7.3.0 추가 (Civil만 사용)
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가 
	char      LoadCaseType2_NotUse[20];     // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	char      LoadCaseType3_NotUse[20];     // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	int       nNumStctCase2_NotUse;         // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	int       nNumStctCase3_NotUse;         // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	T_STCT_BASE StctCase2_NotUse[D_STCT_NUMSTCT];  // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	T_STCT_BASE StctCase3_NotUse[D_STCT_NUMSTCT];  // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	char      strErection1_NotUse[40];      // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	char      strErection2_NotUse[40];      // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	char      strErection3_NotUse[40];      // 8.3.1 추가 - 8.8.5 부터 사용하지 않음
	BOOL      bApplyCamber;                 // 8.3.1 추가
	BOOL      bSelfCons;                    // 8.4.5 추가
	BOOL      bConvergenceFail;             // 8.7.0 추가
	int       nMaxBiSectLoadStep;           // 8.7.0 추가
	int       nMaxDivergedStep;             // 8.7.0 추가
	CArray<T_STCT_EREC_CH, T_STCT_EREC_CH&> aErection; // 8.8.5 추가
	T_STCT_NLCTRL NlCtrl;
	void ConvertToString(T_STCT_D& rData)
	{    
		rData.nFinalStage = nFinalStage;
		rData.FinalStagKey = FinalStagKey;
		rData.bIncludeTimeDependentEffect = bIncludeTimeDependentEffect;
		rData.nIncludeNonLinearAnalysis = nIncludeNonLinearAnalysis;
		rData.bCreepShrinkage = bCreepShrinkage;
		rData.nType = nType;
		rData.nIter = nIter;
		rData.dTol = dTol;
		rData.bOnlyUserCreepCoeff=bOnlyUserCreepCoeff;
		rData.bInternalTimeStep=bInternalTimeStep;
		rData.nInternalTimeStep=nInternalTimeStep;
		rData.bAutoTimeStep=bAutoTimeStep;
		memcpy(rData.nAutoTimeStep, nAutoTimeStep, sizeof(nAutoTimeStep));
		rData.bTTLECreepShrinkage = bTTLECreepShrinkage;
		rData.bVariation = bVariation;
		rData.bTTLEElasticShortening = bTTLEElasticShortening;
		rData.nMaxIterPerLoadStep = nMaxIterPerLoadStep;
		memcpy(rData.bConvFlag, bConvFlag, sizeof(bConvFlag));
		memcpy(rData.dConvValue, dConvValue, sizeof(dConvValue));
		rData.nCablePretensionForceCtrl = nCablePretensionForceCtrl;
		rData.nNumStctCase_NotUse = nNumStctCase_NotUse;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  rData.StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  rData.StctCase_NotUse[i].LoadCaseKey = StctCase_NotUse[i].LoadCaseKey;    
		rData.bCalcCompSectionPart = bCalcCompSectionPart;
		rData.nCablePretensionExternalOpt = nCablePretensionExternalOpt;
		rData.bConvFinalStageMemberForce = bConvFinalStageMemberForce;
		rData.bConvTruss = bConvTruss;
		rData.bConvBeam = bConvBeam;
		rData.nNumLoadStep = nNumLoadStep;
		rData.bInitTangentDisp = bInitTangentDisp;
		rData.nInitTangentDisp = nInitTangentDisp;
		rData.KeyGrup = KeyGrup;
		rData.bCalcConcurrentForce = bCalcConcurrentForce;
		ConvertCharStr(LoadCaseType_NotUse, rData.LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));
		rData.bSecondaryDLEffect = bSecondaryDLEffect;
		rData.nNumGilcCase = nNumGilcCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  rData.GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  rData.GilcCase[i].GilcKey = GilcCase[i].GilcKey;
		rData.bConsiderConfinement = bConsiderConfinement;
		rData.bConsiderEndPartStress = bConsiderEndPartStress;
		rData.nEndPartStress = nEndPartStress;
		rData.dStressRatio = dStressRatio;    
		rData.nBeamSectionChange = nBeamSectionChange;
		rData.nNonlAnalType = nNonlAnalType;
		rData.bIncludeEquiMembForce = bIncludeEquiMembForce;
		rData.bSaveCurStageOutput = bSaveCurStageOutput;
		rData.bLackofFitForce = bLackofFitForce;
		rData.KeyGrup4LackofFit = KeyGrup4LackofFit;
		rData.bIncludePDeltaEffect = bIncludePDeltaEffect;
		rData.bRestartStage = bRestartStage;
		rData.aRestartStagK.Copy(aRestartStagK);
		rData.bApplyInitMemberForce = bApplyInitMemberForce;
		rData.bChangeCableElementForPostCS = bChangeCableElementForPostCS;
		rData.bApplyTDElastic = bApplyTDElastic;
		rData.nTndnTensType = nTndnTensType;
		ConvertCharStr(LoadCaseType2_NotUse, rData.LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertCharStr(LoadCaseType3_NotUse, rData.LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
		rData.nNumStctCase2_NotUse = nNumStctCase2_NotUse;
		rData.nNumStctCase3_NotUse = nNumStctCase3_NotUse;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) rData.StctCase2_NotUse[i].Initialize();      
		for(int i = 0; i<nNumStctCase2_NotUse; ++i) rData.StctCase2_NotUse[i].LoadCaseKey = StctCase2_NotUse[i].LoadCaseKey;    
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) rData.StctCase3_NotUse[i].Initialize();      
		for(int i = 0; i<nNumStctCase3_NotUse; ++i) rData.StctCase3_NotUse[i].LoadCaseKey = StctCase3_NotUse[i].LoadCaseKey;    
		ConvertCharStr(strErection1_NotUse, rData.strErection1_NotUse, sizeof(strErection1_NotUse));
		ConvertCharStr(strErection2_NotUse, rData.strErection2_NotUse, sizeof(strErection2_NotUse));
		ConvertCharStr(strErection3_NotUse, rData.strErection3_NotUse, sizeof(strErection3_NotUse));
		rData.bApplyCamber = bApplyCamber;
		rData.bSelfCons = bSelfCons;
		rData.bConvergenceFail = bConvergenceFail;
		rData.nMaxBiSectLoadStep = nMaxBiSectLoadStep;
		rData.nMaxDivergedStep = nMaxDivergedStep;
		rData.aErection.SetSize(aErection.GetSize());
		for(int i=0; i<aErection.GetSize(); i++) aErection[i].ConvertToString(rData.aErection[i]);
		rData.NlCtrl = NlCtrl;
	}
	void ConvertToChar(T_STCT_D& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		nIncludeNonLinearAnalysis = rData.nIncludeNonLinearAnalysis;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase_NotUse;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase_NotUse[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		ConvertStrChar(rData.LoadCaseType_NotUse, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();      
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i].GilcKey = rData.GilcCase[i].GilcKey;        
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;    
		nBeamSectionChange = rData.nBeamSectionChange;
		nNonlAnalType = rData.nNonlAnalType;
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;
		bSaveCurStageOutput = rData.bSaveCurStageOutput;
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		aRestartStagK.Copy(rData.aRestartStagK);
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		ConvertStrChar(rData.LoadCaseType2_NotUse, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(rData.LoadCaseType3_NotUse, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
		nNumStctCase2_NotUse = rData.nNumStctCase2_NotUse;
		nNumStctCase3_NotUse = rData.nNumStctCase3_NotUse;
		for(int i = 0; i< D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();      
		for(int i = 0; i< nNumStctCase2_NotUse; ++i) StctCase2_NotUse[i].LoadCaseKey = rData.StctCase2_NotUse[i].LoadCaseKey;    
		for(int i = 0; i< D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();      
		for(int i = 0; i< nNumStctCase3_NotUse; ++i) StctCase3_NotUse[i].LoadCaseKey = rData.StctCase3_NotUse[i].LoadCaseKey;    
		ConvertStrChar(rData.strErection1_NotUse, strErection1_NotUse, sizeof(strErection1_NotUse));
		ConvertStrChar(rData.strErection2_NotUse, strErection2_NotUse, sizeof(strErection2_NotUse));
		ConvertStrChar(rData.strErection3_NotUse, strErection3_NotUse, sizeof(strErection3_NotUse));
		bApplyCamber = rData.bApplyCamber;
		bSelfCons = rData.bSelfCons;
		bConvergenceFail = rData.bConvergenceFail;
		nMaxBiSectLoadStep = rData.nMaxBiSectLoadStep;
		nMaxDivergedStep = rData.nMaxDivergedStep;
		aErection.SetSize(rData.aErection.GetSize());
		for(int i=0; i<rData.aErection.GetSize(); i++) aErection[i].ConvertToChar(rData.aErection[i]);
		NlCtrl = rData.NlCtrl;
	}
	void ConvertAlpha(T_STCT_D_ALPHA& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		if (bIncludeTimeDependentEffect && nIncludeNonLinearAnalysis == 1)
			nIncludeNonLinearAnalysis = 0;  // 둘중 하나는 끈다.
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=FALSE;
		bInternalTimeStep=FALSE;
		nInternalTimeStep=2;
		bAutoTimeStep=FALSE;
		nAutoTimeStep[0] = 2;
		nAutoTimeStep[1] = 5;
		nAutoTimeStep[2] = 7;
		nAutoTimeStep[3] = 10;
		nAutoTimeStep[4] = 20;
		bTTLECreepShrinkage = FALSE;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = FALSE;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = 0;
		nNumStctCase_NotUse = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();
		bCalcCompSectionPart = FALSE;
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));    
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		bConsiderConfinement = FALSE;
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;   
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif

		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert500(T_STCT_D_500& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		if (bIncludeTimeDependentEffect && nIncludeNonLinearAnalysis == 1)
			nIncludeNonLinearAnalysis = 0;  // 둘중 하나는 끈다.

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=FALSE;
		bInternalTimeStep=FALSE;
		nInternalTimeStep=2;
		bAutoTimeStep=FALSE;
		nAutoTimeStep[0] = 2;
		nAutoTimeStep[1] = 5;
		nAutoTimeStep[2] = 7;
		nAutoTimeStep[3] = 10;
		nAutoTimeStep[4] = 20;

		bTTLECreepShrinkage = rData.bIncludeTendonTensionLossEffct ? rData.bTTLECreepShrinkage : FALSE;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bIncludeTendonTensionLossEffct ? rData.bTTLEElasticShortening : FALSE;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = 0;

		nNumStctCase_NotUse = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();  
		bCalcCompSectionPart = FALSE;
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));    
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;  
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert501(T_STCT_D_501& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=FALSE;
		bInternalTimeStep=FALSE;
		nInternalTimeStep=2;
		bAutoTimeStep=FALSE;
		nAutoTimeStep[0] = 2;
		nAutoTimeStep[1] = 5;
		nAutoTimeStep[2] = 7;
		nAutoTimeStep[3] = 10;
		nAutoTimeStep[4] = 20;

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		bConvFlag[0] = rData.bConvFlag[0];
		bConvFlag[1] = rData.bConvFlag[1];
		bConvFlag[2] = rData.bConvFlag[2];
		dConvValue[0] = rData.dConvValue[0];
		dConvValue[1] = rData.dConvValue[1];
		dConvValue[2] = rData.dConvValue[2];

		nCablePretensionForceCtrl = 0;

		nNumStctCase_NotUse = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();
		bCalcCompSectionPart = FALSE;    
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));    
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;  
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert530(T_STCT_D_530& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = 0;

		nNumStctCase_NotUse = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();           
		bCalcCompSectionPart = FALSE;
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));    
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;     
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert530US(T_STCT_D_530US& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = 0;

		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = FALSE;
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));    
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;    
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert550(T_STCT_D_550& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;

		nNumStctCase_NotUse = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();           
		bCalcCompSectionPart = FALSE;
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));    
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;   
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert560(T_STCT_D_560& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;

		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = FALSE;
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));    
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;      
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert580(T_STCT_D_580& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;

		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = 0;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));    
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;      
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert590(T_STCT_D_590& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;

		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = FALSE;
		bConvTruss = TRUE;
		bConvBeam = TRUE;
		nNumLoadStep = 1;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));    
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;      
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert600(T_STCT_D_600& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;

		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = FALSE;
		nInitTangentDisp = 0;
		KeyGrup = 0;
		bCalcConcurrentForce = FALSE;
#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));    
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;   
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert610(T_STCT_D_610& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;

		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;

		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));

		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;

		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));

		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;

		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType_NotUse, sizeof(LoadCaseType_NotUse));    
#endif
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;     
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert630(T_STCT_D_CH_630& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));    
		bSecondaryDLEffect = FALSE;
		nNumGilcCase = 0;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
#if defined(_CIVIL)
		bConsiderConfinement = FALSE;
#else
		bConsiderConfinement = TRUE;
#endif
		bConsiderEndPartStress = FALSE;
		nBeamSectionChange = 1;
#if defined(_JP)
		bConsiderEndPartStress = TRUE;
		nBeamSectionChange = 0;
#endif
		nEndPartStress = 0;
		dStressRatio = 0.;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;      
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert650(T_STCT_D_CH_650& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));    
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = 1;                
		bIncludeEquiMembForce = FALSE;     
		bSaveCurStageOutput = FALSE;      
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert670(T_STCT_D_CH_670& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));    
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;                
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;     
		bSaveCurStageOutput = rData.bSaveCurStageOutput;    
		bLackofFitForce = FALSE;
		KeyGrup4LackofFit = 0;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert692(T_STCT_D_CH_692& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));    
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;                
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;     
		bSaveCurStageOutput = rData.bSaveCurStageOutput;    
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = FALSE;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert712(T_STCT_D_CH_712& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));    
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;                
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;     
		bSaveCurStageOutput = rData.bSaveCurStageOutput;    
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = FALSE;
		aRestartStagK.RemoveAll();
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert830(T_STCT_D_CH_830& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));    
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;                
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;     
		bSaveCurStageOutput = rData.bSaveCurStageOutput;    
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		aRestartStagK.Copy(rData.aRestartStagK);
		bApplyInitMemberForce = rData.bApplyInitMemberForce;;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;;
		bApplyTDElastic = rData.bApplyTDElastic;;
		nTndnTensType = rData.nTndnTensType;;
		
#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert842(T_STCT_D_CH_842& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));    
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;                
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;     
		bSaveCurStageOutput = rData.bSaveCurStageOutput;    
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		aRestartStagK.Copy(rData.aRestartStagK);
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		memcpy(LoadCaseType2_NotUse, rData.LoadCaseType2, sizeof(rData.LoadCaseType2));    
		memcpy(LoadCaseType3_NotUse, rData.LoadCaseType3, sizeof(rData.LoadCaseType3));    
		nNumStctCase2_NotUse = rData.nNumStctCase2;
		nNumStctCase3_NotUse = rData.nNumStctCase3;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i] = rData.StctCase2[i];
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i] = rData.StctCase3[i];
		memcpy(strErection1_NotUse, rData.strErection1, sizeof(rData.strErection1));    
		memcpy(strErection2_NotUse, rData.strErection2, sizeof(rData.strErection2));    
		memcpy(strErection3_NotUse, rData.strErection3, sizeof(rData.strErection3));    
		bApplyCamber = rData.bApplyCamber;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert865(T_STCT_D_CH_865& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));    
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;                
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;     
		bSaveCurStageOutput = rData.bSaveCurStageOutput;    
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		aRestartStagK.Copy(rData.aRestartStagK);
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		memcpy(LoadCaseType2_NotUse, rData.LoadCaseType2, sizeof(rData.LoadCaseType2));    
		memcpy(LoadCaseType3_NotUse, rData.LoadCaseType3, sizeof(rData.LoadCaseType3));    
		nNumStctCase2_NotUse = rData.nNumStctCase2;
		nNumStctCase3_NotUse = rData.nNumStctCase3;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i] = rData.StctCase2[i];
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i] = rData.StctCase3[i];
		memcpy(strErection1_NotUse, rData.strErection1, sizeof(rData.strErection1));    
		memcpy(strErection2_NotUse, rData.strErection2, sizeof(rData.strErection2));    
		memcpy(strErection3_NotUse, rData.strErection3, sizeof(rData.strErection3));    
		bApplyCamber = rData.bApplyCamber;
		bSelfCons = rData.bSelfCons;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert883(T_STCT_D_CH_883& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		nIncludeNonLinearAnalysis = rData.nIncludeNonLinearAnalysis;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff=rData.bOnlyUserCreepCoeff;
		bInternalTimeStep=rData.bInternalTimeStep;
		nInternalTimeStep=rData.nInternalTimeStep;
		bAutoTimeStep=rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();      
		for(int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));    
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;    
		for(int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for(int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;                
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;     
		bSaveCurStageOutput = rData.bSaveCurStageOutput;    
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		aRestartStagK.Copy(rData.aRestartStagK);
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		memcpy(LoadCaseType2_NotUse, rData.LoadCaseType2, sizeof(rData.LoadCaseType2));    
		memcpy(LoadCaseType3_NotUse, rData.LoadCaseType3, sizeof(rData.LoadCaseType3));    
		nNumStctCase2_NotUse = rData.nNumStctCase2;
		nNumStctCase3_NotUse = rData.nNumStctCase3;
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i] = rData.StctCase2[i];
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i] = rData.StctCase3[i];
		memcpy(strErection1_NotUse, rData.strErection1, sizeof(rData.strErection1));    
		memcpy(strErection2_NotUse, rData.strErection2, sizeof(rData.strErection2));    
		memcpy(strErection3_NotUse, rData.strErection3, sizeof(rData.strErection3));    
		bApplyCamber = rData.bApplyCamber;
		bSelfCons = rData.bSelfCons;
		bConvergenceFail = rData.bConvergenceFail;
		nMaxBiSectLoadStep = rData.nMaxBiSectLoadStep;
		nMaxDivergedStep = rData.nMaxDivergedStep;
		aErection.RemoveAll();
		NlCtrl.Initialize();
	}
	void Convert960(T_STCT_D_CH_960& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		nIncludeNonLinearAnalysis = rData.nIncludeNonLinearAnalysis;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff = rData.bOnlyUserCreepCoeff;
		bInternalTimeStep = rData.bInternalTimeStep;
		nInternalTimeStep = rData.nInternalTimeStep;
		bAutoTimeStep = rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(nAutoTimeStep));
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		bVariation = rData.bVariation;
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase_NotUse;
		for (int i = 0; i < D_STCT_NUMSTCT; i++)  StctCase_NotUse[i].Initialize();
		for (int i = 0; i < nNumStctCase_NotUse; i++)  StctCase_NotUse[i].LoadCaseKey = rData.StctCase_NotUse[i].LoadCaseKey;
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType_NotUse, sizeof(rData.LoadCaseType_NotUse));
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;
		for (int i = 0; i < D_STCT_NUMSTCT; i++)  GilcCase[i].Initialize();
		for (int i = 0; i < nNumGilcCase; i++)  GilcCase[i] = rData.GilcCase[i];
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nBeamSectionChange = rData.nBeamSectionChange;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nNonlAnalType = rData.nNonlAnalType;
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;
		bSaveCurStageOutput = rData.bSaveCurStageOutput;
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		aRestartStagK.Copy(rData.aRestartStagK);
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		memcpy(LoadCaseType2_NotUse, rData.LoadCaseType2_NotUse, sizeof(rData.LoadCaseType2_NotUse));
		memcpy(LoadCaseType3_NotUse, rData.LoadCaseType3_NotUse, sizeof(rData.LoadCaseType3_NotUse));
		nNumStctCase2_NotUse = rData.nNumStctCase2_NotUse;
		nNumStctCase3_NotUse = rData.nNumStctCase3_NotUse;
		for (int i = 0; i < D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i] = rData.StctCase2_NotUse[i];
		for (int i = 0; i < D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i] = rData.StctCase3_NotUse[i];
		memcpy(strErection1_NotUse, rData.strErection1_NotUse, sizeof(rData.strErection1_NotUse));
		memcpy(strErection2_NotUse, rData.strErection2_NotUse, sizeof(rData.strErection2_NotUse));
		memcpy(strErection3_NotUse, rData.strErection3_NotUse, sizeof(rData.strErection3_NotUse));
		bApplyCamber = rData.bApplyCamber;
		bSelfCons = rData.bSelfCons;
		bConvergenceFail = rData.bConvergenceFail;
		nMaxBiSectLoadStep = rData.nMaxBiSectLoadStep;
		nMaxDivergedStep = rData.nMaxDivergedStep;
		aErection.Copy(rData.aErection);
		NlCtrl.Initialize();
	}
};
struct T_STCT_UDRD_D
{
	T_STCT_D data;
};
struct T_STCT_UDRD_D_CH
{
	T_STCT_D_CH data;
};
struct T_STCT_UDRD_D_CH_RW_721
{
	int       nFinalStage;
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];
	double    dConvValue[3];
	int       nCablePretensionForceCtrl;
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;
	int       nCablePretensionExternalOpt;
	BOOL      bConvFinalStageMemberForce;
	BOOL      bConvTruss;
	BOOL      bConvBeam;
	int       nNumLoadStep;
	BOOL      bInitTangentDisp;
	int       nInitTangentDisp;
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;
	char      LoadCaseType[20];
	BOOL      bSecondaryDLEffect;
	int       nNumGilcCase;
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];
	BOOL      bConsiderConfinement;
	BOOL      bConsiderEndPartStress;
	int       nEndPartStress;
	double    dStressRatio;
	int       nBeamSectionChange;
	int       nNonlAnalType;
	BOOL      bIncludeEquiMembForce;
	BOOL      bSaveCurStageOutput;
	BOOL      bLackofFitForce;
	UINT      KeyGrup4LackofFit;
	BOOL      bIncludePDeltaEffect;
	BOOL      bRestartStage;
	int       nRestartStageCount;
};
struct T_STCT_UDRD_D_CH_RW_730A
{
	int       nFinalStage;
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];
	double    dConvValue[3];
	int       nCablePretensionForceCtrl;
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;
	int       nCablePretensionExternalOpt;
	BOOL      bConvFinalStageMemberForce;
	BOOL      bConvTruss;
	BOOL      bConvBeam;
	int       nNumLoadStep;
	BOOL      bInitTangentDisp;
	int       nInitTangentDisp;
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;
	char      LoadCaseType[20];
	BOOL      bSecondaryDLEffect;
	int       nNumGilcCase;
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];
	BOOL      bConsiderConfinement;
	BOOL      bConsiderEndPartStress;
	int       nEndPartStress;
	double    dStressRatio;
	int       nBeamSectionChange;
	int       nNonlAnalType;
	BOOL      bIncludeEquiMembForce;
	BOOL      bSaveCurStageOutput;
	BOOL      bLackofFitForce;
	UINT      KeyGrup4LackofFit;
	BOOL      bIncludePDeltaEffect;
	BOOL      bRestartStage;
	int       nRestartStageCount;
	BOOL      bApplyInitMemberForce;
};
struct T_STCT_UDRD_D_CH_RW_785
{
	int       nFinalStage;
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];
	double    dConvValue[3];
	int       nCablePretensionForceCtrl;
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;
	int       nCablePretensionExternalOpt;
	BOOL      bConvFinalStageMemberForce;
	BOOL      bConvTruss;
	BOOL      bConvBeam;
	int       nNumLoadStep;
	BOOL      bInitTangentDisp;
	int       nInitTangentDisp;
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;
	char      LoadCaseType[20];
	BOOL      bSecondaryDLEffect;
	int       nNumGilcCase;
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];
	BOOL      bConsiderConfinement;
	BOOL      bConsiderEndPartStress;
	int       nEndPartStress;
	double    dStressRatio;
	int       nBeamSectionChange;
	int       nNonlAnalType;
	BOOL      bIncludeEquiMembForce;
	BOOL      bSaveCurStageOutput;
	BOOL      bLackofFitForce;
	UINT      KeyGrup4LackofFit;
	BOOL      bIncludePDeltaEffect;
	BOOL      bRestartStage;
	int       nRestartStageCount;
	BOOL      bApplyInitMemberForce;
	BOOL      bChangeCableElementForPostCS;  // 7.3.0 
};
struct T_STCT_UDRD_D_CH_RW_791
{
	int       nFinalStage;
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];
	double    dConvValue[3];
	int       nCablePretensionForceCtrl;
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;
	int       nCablePretensionExternalOpt;
	BOOL      bConvFinalStageMemberForce;
	BOOL      bConvTruss;
	BOOL      bConvBeam;
	int       nNumLoadStep;
	BOOL      bInitTangentDisp;
	int       nInitTangentDisp;
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;
	char      LoadCaseType[20];
	BOOL      bSecondaryDLEffect;
	int       nNumGilcCase;
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];
	BOOL      bConsiderConfinement;
	BOOL      bConsiderEndPartStress;
	int       nEndPartStress;
	double    dStressRatio;
	int       nBeamSectionChange;
	int       nNonlAnalType;
	BOOL      bIncludeEquiMembForce;
	BOOL      bSaveCurStageOutput;
	BOOL      bLackofFitForce;
	UINT      KeyGrup4LackofFit;
	BOOL      bIncludePDeltaEffect;
	BOOL      bRestartStage;
	int       nRestartStageCount;
	BOOL      bApplyInitMemberForce;        // 7.3.0 A
	BOOL      bChangeCableElementForPostCS; // 7.3.0 
	BOOL      bApplyTDElastic;              // 7.9.0 추가
};
struct T_STCT_UDRD_D_CH_RW_830
{
	int       nFinalStage;
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];
	double    dConvValue[3];
	int       nCablePretensionForceCtrl;
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;
	int       nCablePretensionExternalOpt;
	BOOL      bConvFinalStageMemberForce;
	BOOL      bConvTruss;
	BOOL      bConvBeam;
	int       nNumLoadStep;
	BOOL      bInitTangentDisp;
	int       nInitTangentDisp;
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;
	char      LoadCaseType[20];
	BOOL      bSecondaryDLEffect;
	int       nNumGilcCase;
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];
	BOOL      bConsiderConfinement;
	BOOL      bConsiderEndPartStress;
	int       nEndPartStress;
	double    dStressRatio;
	int       nBeamSectionChange;
	int       nNonlAnalType;
	BOOL      bIncludeEquiMembForce;
	BOOL      bSaveCurStageOutput;
	BOOL      bLackofFitForce;
	UINT      KeyGrup4LackofFit;
	BOOL      bIncludePDeltaEffect;
	BOOL      bRestartStage;
	int       nRestartStageCount;
	BOOL      bApplyInitMemberForce;        // 7.3.0 A
	BOOL      bChangeCableElementForPostCS; // 7.3.0 
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가
};
struct T_STCT_UDRD_D_CH_RW_842
{
	int       nFinalStage;
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];
	double    dConvValue[3];
	int       nCablePretensionForceCtrl;
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;
	int       nCablePretensionExternalOpt;
	BOOL      bConvFinalStageMemberForce;
	BOOL      bConvTruss;
	BOOL      bConvBeam;
	int       nNumLoadStep;
	BOOL      bInitTangentDisp;
	int       nInitTangentDisp;
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;
	char      LoadCaseType[20];
	BOOL      bSecondaryDLEffect;
	int       nNumGilcCase;
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];
	BOOL      bConsiderConfinement;
	BOOL      bConsiderEndPartStress;
	int       nEndPartStress;
	double    dStressRatio;
	int       nBeamSectionChange;
	int       nNonlAnalType;
	BOOL      bIncludeEquiMembForce;
	BOOL      bSaveCurStageOutput;
	BOOL      bLackofFitForce;
	UINT      KeyGrup4LackofFit;
	BOOL      bIncludePDeltaEffect;
	BOOL      bRestartStage;
	int       nRestartStageCount;
	BOOL      bApplyInitMemberForce;        // 7.3.0 A
	BOOL      bChangeCableElementForPostCS; // 7.3.0 
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가
	char      LoadCaseType2[20];            // 8.3.1 추가
	char      LoadCaseType3[20];            // 8.3.1 추가
	int       nNumStctCase2;                // 8.3.1 추가
	int       nNumStctCase3;                // 8.3.1 추가
	T_STCT_BASE StctCase2[D_STCT_NUMSTCT];  // 8.3.1 추가
	T_STCT_BASE StctCase3[D_STCT_NUMSTCT];  // 8.3.1 추가
	char      strErection1[40];             // 8.3.1 추가
	char      strErection2[40];             // 8.3.1 추가
	char      strErection3[40];             // 8.3.1 추가
	BOOL      bApplyCamber;                 // 8.3.1 추가
};
struct T_STCT_UDRD_D_CH_RW_865
{
	int       nFinalStage;
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	BOOL      bIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];
	double    dConvValue[3];
	int       nCablePretensionForceCtrl;
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;
	int       nCablePretensionExternalOpt;
	BOOL      bConvFinalStageMemberForce;
	BOOL      bConvTruss;
	BOOL      bConvBeam;
	int       nNumLoadStep;
	BOOL      bInitTangentDisp;
	int       nInitTangentDisp;
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;
	char      LoadCaseType[20];
	BOOL      bSecondaryDLEffect;
	int       nNumGilcCase;
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];
	BOOL      bConsiderConfinement;
	BOOL      bConsiderEndPartStress;
	int       nEndPartStress;
	double    dStressRatio;
	int       nBeamSectionChange;
	int       nNonlAnalType;
	BOOL      bIncludeEquiMembForce;
	BOOL      bSaveCurStageOutput;
	BOOL      bLackofFitForce;
	UINT      KeyGrup4LackofFit;
	BOOL      bIncludePDeltaEffect;
	BOOL      bRestartStage;
	int       nRestartStageCount;
	BOOL      bApplyInitMemberForce;        // 7.3.0 A
	BOOL      bChangeCableElementForPostCS; // 7.3.0 
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가
	char      LoadCaseType2[20];            // 8.3.1 추가
	char      LoadCaseType3[20];            // 8.3.1 추가
	int       nNumStctCase2;                // 8.3.1 추가
	int       nNumStctCase3;                // 8.3.1 추가
	T_STCT_BASE StctCase2[D_STCT_NUMSTCT];  // 8.3.1 추가
	T_STCT_BASE StctCase3[D_STCT_NUMSTCT];  // 8.3.1 추가
	char      strErection1[40];             // 8.3.1 추가
	char      strErection2[40];             // 8.3.1 추가
	char      strErection3[40];             // 8.3.1 추가
	BOOL      bApplyCamber;                 // 8.3.1 추가
	BOOL      bSelfCons;                    // 8.4.5 추가
};
struct T_STCT_UDRD_D_CH_RW_883
{
	int       nFinalStage;
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	int       nIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];
	double    dConvValue[3];
	int       nCablePretensionForceCtrl;
	int       nNumStctCase;  
	T_STCT_BASE StctCase[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;
	int       nCablePretensionExternalOpt;
	BOOL      bConvFinalStageMemberForce;
	BOOL      bConvTruss;
	BOOL      bConvBeam;
	int       nNumLoadStep;
	BOOL      bInitTangentDisp;
	int       nInitTangentDisp;
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;
	char      LoadCaseType[20];
	BOOL      bSecondaryDLEffect;
	int       nNumGilcCase;
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];
	BOOL      bConsiderConfinement;
	BOOL      bConsiderEndPartStress;
	int       nEndPartStress;
	double    dStressRatio;
	int       nBeamSectionChange;
	int       nNonlAnalType;
	BOOL      bIncludeEquiMembForce;
	BOOL      bSaveCurStageOutput;
	BOOL      bLackofFitForce;
	UINT      KeyGrup4LackofFit;
	BOOL      bIncludePDeltaEffect;
	BOOL      bRestartStage;
	int       nRestartStageCount;
	BOOL      bApplyInitMemberForce;        // 7.3.0 A
	BOOL      bChangeCableElementForPostCS; // 7.3.0 
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가
	char      LoadCaseType2[20];            // 8.3.1 추가
	char      LoadCaseType3[20];            // 8.3.1 추가
	int       nNumStctCase2;                // 8.3.1 추가
	int       nNumStctCase3;                // 8.3.1 추가
	T_STCT_BASE StctCase2[D_STCT_NUMSTCT];  // 8.3.1 추가
	T_STCT_BASE StctCase3[D_STCT_NUMSTCT];  // 8.3.1 추가
	char      strErection1[40];             // 8.3.1 추가
	char      strErection2[40];             // 8.3.1 추가
	char      strErection3[40];             // 8.3.1 추가
	BOOL      bApplyCamber;                 // 8.3.1 추가
	BOOL      bSelfCons;                    // 8.4.5 추가
	BOOL      bConvergenceFail;             // 8.7.0 추가
	int       nMaxBiSectLoadStep;           // 8.7.0 추가 
	int       nMaxDivergedStep;             // 8.7.0 추가 
};
struct T_STCT_UDRD_D_CH_RW_960
{
	int       nFinalStage;
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	int       nIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];
	double    dConvValue[3];
	int       nCablePretensionForceCtrl;
	int       nNumStctCase_NotUse;
	T_STCT_BASE StctCase_NotUse[D_STCT_NUMSTCT];
	BOOL      bCalcCompSectionPart;
	int       nCablePretensionExternalOpt;
	BOOL      bConvFinalStageMemberForce;
	BOOL      bConvTruss;
	BOOL      bConvBeam;
	int       nNumLoadStep;
	BOOL      bInitTangentDisp;
	int       nInitTangentDisp;
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;
	char      LoadCaseType_NotUse[20];
	BOOL      bSecondaryDLEffect;
	int       nNumGilcCase;
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];
	BOOL      bConsiderConfinement;
	BOOL      bConsiderEndPartStress;
	int       nEndPartStress;
	double    dStressRatio;
	int       nBeamSectionChange;
	int       nNonlAnalType;
	BOOL      bIncludeEquiMembForce;
	BOOL      bSaveCurStageOutput;
	BOOL      bLackofFitForce;
	UINT      KeyGrup4LackofFit;
	BOOL      bIncludePDeltaEffect;
	BOOL      bRestartStage;
	int       nRestartStageCount;
	BOOL      bApplyInitMemberForce;        // 7.3.0 A
	BOOL      bChangeCableElementForPostCS; // 7.3.0 
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가
	char      LoadCaseType2_NotUse[20];     // 8.3.1 추가
	char      LoadCaseType3_NotUse[20];     // 8.3.1 추가
	int       nNumStctCase2_NotUse;         // 8.3.1 추가
	int       nNumStctCase3_NotUse;         // 8.3.1 추가
	T_STCT_BASE StctCase2_NotUse[D_STCT_NUMSTCT];  // 8.3.1 추가
	T_STCT_BASE StctCase3_NotUse[D_STCT_NUMSTCT];  // 8.3.1 추가
	char      strErection1_NotUse[40];      // 8.3.1 추가
	char      strErection2_NotUse[40];      // 8.3.1 추가
	char      strErection3_NotUse[40];      // 8.3.1 추가
	BOOL      bApplyCamber;                 // 8.3.1 추가
	BOOL      bSelfCons;                    // 8.4.5 추가
	BOOL      bConvergenceFail;             // 8.7.0 추가
	int       nMaxBiSectLoadStep;           // 8.7.0 추가 
	int       nMaxDivergedStep;             // 8.7.0 추가 
	int       nErectionCount;               // 8.8.5 추가
};
struct T_STCT_UDRD_D_CH_RW
{
	int       nFinalStage;
	T_STAG_K  FinalStagKey;
	BOOL      bIncludeTimeDependentEffect;
	int       nIncludeNonLinearAnalysis;
	BOOL      bCreepShrinkage;
	int       nType;
	int       nIter;
	double    dTol;
	BOOL      bOnlyUserCreepCoeff;
	BOOL      bInternalTimeStep;
	int       nInternalTimeStep;
	BOOL      bAutoTimeStep;
	int       nAutoTimeStep[5];
	BOOL      bTTLEElasticShortening;
	BOOL      bVariation;
	BOOL      bTTLECreepShrinkage;
	int       nMaxIterPerLoadStep;
	BOOL      bConvFlag[3];
	double    dConvValue[3];
	int       nCablePretensionForceCtrl;
	int       nNumStctCase_NotUse;  
	T_STCT_BASE StctCase_NotUse[D_STCT_NUMSTCT];   
	BOOL      bCalcCompSectionPart;
	int       nCablePretensionExternalOpt;
	BOOL      bConvFinalStageMemberForce;
	BOOL      bConvTruss;
	BOOL      bConvBeam;
	int       nNumLoadStep;
	BOOL      bInitTangentDisp;
	int       nInitTangentDisp;
	unsigned int KeyGrup;
	BOOL      bCalcConcurrentForce;
	char      LoadCaseType_NotUse[20];
	BOOL      bSecondaryDLEffect;
	int       nNumGilcCase;
	T_GILC_BASE GilcCase[D_STCT_NUMSTCT];
	BOOL      bConsiderConfinement;
	BOOL      bConsiderEndPartStress;
	int       nEndPartStress;
	double    dStressRatio;
	int       nBeamSectionChange;
	int       nNonlAnalType;
	BOOL      bIncludeEquiMembForce;
	BOOL      bSaveCurStageOutput;
	BOOL      bLackofFitForce;
	UINT      KeyGrup4LackofFit;
	BOOL      bIncludePDeltaEffect;
	BOOL      bRestartStage;
	int       nRestartStageCount;
	BOOL      bApplyInitMemberForce;        // 7.3.0 A
	BOOL      bChangeCableElementForPostCS; // 7.3.0 
	BOOL      bApplyTDElastic;              // 7.9.0 추가
	int       nTndnTensType;                // 7.9.5 추가
	char      LoadCaseType2_NotUse[20];     // 8.3.1 추가
	char      LoadCaseType3_NotUse[20];     // 8.3.1 추가
	int       nNumStctCase2_NotUse;         // 8.3.1 추가
	int       nNumStctCase3_NotUse;         // 8.3.1 추가
	T_STCT_BASE StctCase2_NotUse[D_STCT_NUMSTCT];  // 8.3.1 추가
	T_STCT_BASE StctCase3_NotUse[D_STCT_NUMSTCT];  // 8.3.1 추가
	char      strErection1_NotUse[40];      // 8.3.1 추가
	char      strErection2_NotUse[40];      // 8.3.1 추가
	char      strErection3_NotUse[40];      // 8.3.1 추가
	BOOL      bApplyCamber;                 // 8.3.1 추가
	BOOL      bSelfCons;                    // 8.4.5 추가
	BOOL      bConvergenceFail;             // 8.7.0 추가
	int       nMaxBiSectLoadStep;           // 8.7.0 추가 
	int       nMaxDivergedStep;             // 8.7.0 추가 
	int       nErectionCount;               // 8.8.5 추가
	T_STCT_NLCTRL_UDRD_D_CH_RW NlCtrl;		// NS 추가

	void GetStct(T_STCT_D_CH& rData)
	{
		rData.nFinalStage = nFinalStage;
		rData.FinalStagKey = FinalStagKey;
		rData.bIncludeTimeDependentEffect = bIncludeTimeDependentEffect;
		rData.nIncludeNonLinearAnalysis = nIncludeNonLinearAnalysis;
		rData.bCreepShrinkage = bCreepShrinkage;
		rData.nType = nType;
		rData.nIter = nIter;
		rData.dTol = dTol;
		rData.bOnlyUserCreepCoeff = bOnlyUserCreepCoeff;
		rData.bInternalTimeStep = bInternalTimeStep;
		rData.nInternalTimeStep = nInternalTimeStep;
		rData.bAutoTimeStep = bAutoTimeStep;
		memcpy(rData.nAutoTimeStep, nAutoTimeStep, sizeof(rData.nAutoTimeStep));
		rData.bTTLEElasticShortening = bTTLEElasticShortening;
		rData.bVariation = bVariation;
		rData.bTTLECreepShrinkage = bTTLECreepShrinkage;
		rData.nMaxIterPerLoadStep = nMaxIterPerLoadStep;
		memcpy(rData.bConvFlag, bConvFlag, sizeof(rData.bConvFlag));
		memcpy(rData.dConvValue, dConvValue, sizeof(rData.dConvValue));
		rData.nCablePretensionForceCtrl = nCablePretensionForceCtrl;
		rData.nNumStctCase_NotUse = nNumStctCase_NotUse;
		memcpy(rData.StctCase_NotUse, StctCase_NotUse, sizeof(rData.StctCase_NotUse));
		rData.bCalcCompSectionPart = bCalcCompSectionPart;
		rData.nCablePretensionExternalOpt = nCablePretensionExternalOpt;
		rData.bConvFinalStageMemberForce = bConvFinalStageMemberForce;
		rData.bConvTruss = bConvTruss;
		rData.bConvBeam = bConvBeam;
		rData.nNumLoadStep = nNumLoadStep;
		rData.bInitTangentDisp = bInitTangentDisp;
		rData.nInitTangentDisp = nInitTangentDisp;
		rData.KeyGrup = KeyGrup;
		rData.bCalcConcurrentForce = bCalcConcurrentForce;
		memcpy(rData.LoadCaseType_NotUse, LoadCaseType_NotUse, sizeof(rData.LoadCaseType_NotUse));
		rData.bSecondaryDLEffect = bSecondaryDLEffect;
		rData.nNumGilcCase = nNumGilcCase;
		memcpy(rData.GilcCase, GilcCase, sizeof(rData.GilcCase));
		rData.bConsiderConfinement = bConsiderConfinement;
		rData.bConsiderEndPartStress = bConsiderEndPartStress;
		rData.nEndPartStress = nEndPartStress;
		rData.dStressRatio = dStressRatio;
		rData.nBeamSectionChange = nBeamSectionChange;
		rData.nNonlAnalType = nNonlAnalType;
		rData.bIncludeEquiMembForce = bIncludeEquiMembForce;
		rData.bSaveCurStageOutput = bSaveCurStageOutput;
		rData.bLackofFitForce = bLackofFitForce;
		rData.KeyGrup4LackofFit = KeyGrup4LackofFit;
		rData.bIncludePDeltaEffect = bIncludePDeltaEffect;
		rData.bRestartStage = bRestartStage;
		rData.bApplyInitMemberForce = bApplyInitMemberForce;
		rData.bChangeCableElementForPostCS = bChangeCableElementForPostCS;
		rData.bApplyTDElastic = bApplyTDElastic;
		rData.nTndnTensType = nTndnTensType;
		memcpy(rData.LoadCaseType2_NotUse, LoadCaseType2_NotUse, sizeof(rData.LoadCaseType2_NotUse));
		memcpy(rData.LoadCaseType3_NotUse, LoadCaseType3_NotUse, sizeof(rData.LoadCaseType3_NotUse));
		rData.nNumStctCase2_NotUse = nNumStctCase2_NotUse;
		rData.nNumStctCase3_NotUse = nNumStctCase3_NotUse;
		memcpy(rData.StctCase2_NotUse, StctCase2_NotUse, sizeof(rData.StctCase2_NotUse));
		memcpy(rData.StctCase3_NotUse, StctCase3_NotUse, sizeof(rData.StctCase3_NotUse));
		memcpy(rData.strErection1_NotUse, strErection1_NotUse, sizeof(rData.strErection1_NotUse));
		memcpy(rData.strErection2_NotUse, strErection2_NotUse, sizeof(rData.strErection2_NotUse));
		memcpy(rData.strErection3_NotUse, strErection3_NotUse, sizeof(rData.strErection3_NotUse));
		rData.bApplyCamber = bApplyCamber;
		rData.bSelfCons = bSelfCons;
		rData.bConvergenceFail = bConvergenceFail;
		rData.nMaxBiSectLoadStep = nMaxBiSectLoadStep;
		rData.nMaxDivergedStep = nMaxDivergedStep;
		rData.NlCtrl.bConsiderGeoNLEffect = NlCtrl.bConsiderGeoNLEffect;
		rData.NlCtrl.nNumOfIncre = NlCtrl.nNumOfIncre;
		for (int i = 0; i < 3; i++)rData.NlCtrl.bConv[i] = NlCtrl.bConv[i];
		for (int i = 0; i < 3; i++)rData.NlCtrl.dConv[i] = NlCtrl.dConv[i];
		rData.NlCtrl.nLoadStepOption = NlCtrl.nLoadStepOption;
		rData.NlCtrl.nOutputReq = NlCtrl.nOutputReq;
		rData.NlCtrl.Param = NlCtrl.Param;
	}
	void SetStct(const T_STCT_D_CH& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		nIncludeNonLinearAnalysis = rData.nIncludeNonLinearAnalysis;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff = rData.bOnlyUserCreepCoeff;
		bInternalTimeStep = rData.bInternalTimeStep;
		nInternalTimeStep = rData.nInternalTimeStep;
		bAutoTimeStep = rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(rData.nAutoTimeStep));
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		bVariation = rData.bVariation;
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag,  rData.bConvFlag, sizeof(rData.bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(rData.dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase_NotUse;
		memcpy(StctCase_NotUse, rData.StctCase_NotUse, sizeof(rData.StctCase_NotUse));
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType_NotUse, sizeof(rData.LoadCaseType_NotUse));
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;
		memcpy(GilcCase, rData.GilcCase, sizeof(rData.GilcCase));
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nBeamSectionChange = rData.nBeamSectionChange;
		nNonlAnalType = rData.nNonlAnalType;
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;
		bSaveCurStageOutput = rData.bSaveCurStageOutput;
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		nRestartStageCount = (int)rData.aRestartStagK.GetSize();
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		memcpy(LoadCaseType2_NotUse, rData.LoadCaseType2_NotUse, sizeof(rData.LoadCaseType2_NotUse));
		memcpy(LoadCaseType3_NotUse, rData.LoadCaseType3_NotUse, sizeof(rData.LoadCaseType3_NotUse));
		nNumStctCase2_NotUse = rData.nNumStctCase2_NotUse;
		nNumStctCase3_NotUse = rData.nNumStctCase3_NotUse;
		memcpy(StctCase2_NotUse, rData.StctCase2_NotUse, sizeof(rData.StctCase2_NotUse));
		memcpy(StctCase3_NotUse, rData.StctCase3_NotUse, sizeof(rData.StctCase3_NotUse));
		memcpy(strErection1_NotUse, rData.strErection1_NotUse, sizeof(rData.strErection1_NotUse));
		memcpy(strErection2_NotUse, rData.strErection2_NotUse, sizeof(rData.strErection2_NotUse));
		memcpy(strErection3_NotUse, rData.strErection3_NotUse, sizeof(rData.strErection3_NotUse));
		bApplyCamber = rData.bApplyCamber;
		bSelfCons = rData.bSelfCons;
		bConvergenceFail = rData.bConvergenceFail;
		nMaxBiSectLoadStep = rData.nMaxBiSectLoadStep;
		nMaxDivergedStep = rData.nMaxDivergedStep;
		nErectionCount = (int)rData.aErection.GetSize();
		NlCtrl.nLoadStepCount = (int)rData.NlCtrl.aLoadStep.GetSize();
		NlCtrl.bConsiderGeoNLEffect = rData.NlCtrl.bConsiderGeoNLEffect;
		NlCtrl.nNumOfIncre = rData.NlCtrl.nNumOfIncre;
		for (int i = 0; i < 3; i++)NlCtrl.bConv[i] = rData.NlCtrl.bConv[i];
		for (int i = 0; i < 3; i++)NlCtrl.dConv[i] = rData.NlCtrl.dConv[i];
		NlCtrl.nLoadStepOption = rData.NlCtrl.nLoadStepOption;
		NlCtrl.nOutputReq = rData.NlCtrl.nOutputReq;
		NlCtrl.Param = rData.NlCtrl.Param;
	}
	void Convert960(T_STCT_UDRD_D_CH_RW_960& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		nIncludeNonLinearAnalysis = rData.nIncludeNonLinearAnalysis;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff = rData.bOnlyUserCreepCoeff;
		bInternalTimeStep = rData.bInternalTimeStep;
		nInternalTimeStep = rData.nInternalTimeStep;
		bAutoTimeStep = rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(rData.nAutoTimeStep));
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		bVariation = rData.bVariation;
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag, rData.bConvFlag, sizeof(rData.bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(rData.dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase_NotUse;
		memcpy(StctCase_NotUse, rData.StctCase_NotUse, sizeof(rData.StctCase_NotUse));
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType_NotUse, sizeof(rData.LoadCaseType_NotUse));
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;
		memcpy(GilcCase, rData.GilcCase, sizeof(rData.GilcCase));
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nBeamSectionChange = rData.nBeamSectionChange;
		nNonlAnalType = rData.nNonlAnalType;
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;
		bSaveCurStageOutput = rData.bSaveCurStageOutput;
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		nRestartStageCount = rData.nRestartStageCount;
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		memcpy(LoadCaseType2_NotUse, rData.LoadCaseType2_NotUse, sizeof(rData.LoadCaseType2_NotUse));
		memcpy(LoadCaseType3_NotUse, rData.LoadCaseType3_NotUse, sizeof(rData.LoadCaseType3_NotUse));
		nNumStctCase2_NotUse = rData.nNumStctCase2_NotUse;
		nNumStctCase3_NotUse = rData.nNumStctCase3_NotUse;
		for (int i = 0; i < D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i] = rData.StctCase2_NotUse[i];
		for (int i = 0; i < D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i] = rData.StctCase3_NotUse[i];
		memcpy(strErection1_NotUse, rData.strErection1_NotUse, sizeof(rData.strErection1_NotUse));
		memcpy(strErection2_NotUse, rData.strErection2_NotUse, sizeof(rData.strErection2_NotUse));
		memcpy(strErection3_NotUse, rData.strErection3_NotUse, sizeof(rData.strErection3_NotUse));
		bApplyCamber = rData.bApplyCamber;
		bSelfCons = rData.bSelfCons;
		bConvergenceFail = rData.bConvergenceFail;
		nMaxBiSectLoadStep = rData.nMaxBiSectLoadStep;
		nMaxDivergedStep = rData.nMaxDivergedStep;
		nErectionCount = rData.nErectionCount;
		NlCtrl.Initialize();
	}
	void Convert883(T_STCT_UDRD_D_CH_RW_883& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		nIncludeNonLinearAnalysis = rData.nIncludeNonLinearAnalysis;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff = rData.bOnlyUserCreepCoeff;
		bInternalTimeStep = rData.bInternalTimeStep;
		nInternalTimeStep = rData.nInternalTimeStep;
		bAutoTimeStep = rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(rData.nAutoTimeStep));
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		bVariation = rData.bVariation;
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag,  rData.bConvFlag, sizeof(rData.bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(rData.dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		memcpy(StctCase_NotUse, rData.StctCase, sizeof(rData.StctCase));
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;
		memcpy(GilcCase, rData.GilcCase, sizeof(rData.GilcCase));
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nBeamSectionChange = rData.nBeamSectionChange;
		nNonlAnalType = rData.nNonlAnalType;
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;
		bSaveCurStageOutput = rData.bSaveCurStageOutput;
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		nRestartStageCount = rData.nRestartStageCount;
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		memcpy(LoadCaseType2_NotUse, rData.LoadCaseType2, sizeof(rData.LoadCaseType2));
		memcpy(LoadCaseType3_NotUse, rData.LoadCaseType3, sizeof(rData.LoadCaseType3));
		nNumStctCase2_NotUse = rData.nNumStctCase2;
		nNumStctCase3_NotUse = rData.nNumStctCase3;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i] = rData.StctCase2[i];
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i] = rData.StctCase3[i];
		memcpy(strErection1_NotUse, rData.strErection1, sizeof(rData.strErection1));
		memcpy(strErection2_NotUse, rData.strErection2, sizeof(rData.strErection2));
		memcpy(strErection3_NotUse, rData.strErection3, sizeof(rData.strErection3));
		bApplyCamber = rData.bApplyCamber;
		bSelfCons = rData.bSelfCons;
		bConvergenceFail = rData.bConvergenceFail;
		nMaxBiSectLoadStep = rData.nMaxBiSectLoadStep;
		nMaxDivergedStep = rData.nMaxDivergedStep;
		nErectionCount = 0;
		NlCtrl.Initialize();
	}
	void Convert865(T_STCT_UDRD_D_CH_RW_865& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff = rData.bOnlyUserCreepCoeff;
		bInternalTimeStep = rData.bInternalTimeStep;
		nInternalTimeStep = rData.nInternalTimeStep;
		bAutoTimeStep = rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(rData.nAutoTimeStep));
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		bVariation = rData.bVariation;
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag,  rData.bConvFlag, sizeof(rData.bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(rData.dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		memcpy(StctCase_NotUse, rData.StctCase, sizeof(rData.StctCase));
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;
		memcpy(GilcCase, rData.GilcCase, sizeof(rData.GilcCase));
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nBeamSectionChange = rData.nBeamSectionChange;
		nNonlAnalType = rData.nNonlAnalType;
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;
		bSaveCurStageOutput = rData.bSaveCurStageOutput;
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		nRestartStageCount = rData.nRestartStageCount;
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		memcpy(LoadCaseType2_NotUse, rData.LoadCaseType2, sizeof(rData.LoadCaseType2));
		memcpy(LoadCaseType3_NotUse, rData.LoadCaseType3, sizeof(rData.LoadCaseType3));
		nNumStctCase2_NotUse = rData.nNumStctCase2;
		nNumStctCase3_NotUse = rData.nNumStctCase3;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i] = rData.StctCase2[i];
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i] = rData.StctCase3[i];
		memcpy(strErection1_NotUse, rData.strErection1, sizeof(rData.strErection1));
		memcpy(strErection2_NotUse, rData.strErection2, sizeof(rData.strErection2));
		memcpy(strErection3_NotUse, rData.strErection3, sizeof(rData.strErection3));
		bApplyCamber = rData.bApplyCamber;
		bSelfCons = rData.bSelfCons;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		nErectionCount = 0;
		NlCtrl.Initialize();
	}
	void Convert842(T_STCT_UDRD_D_CH_RW_842& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff = rData.bOnlyUserCreepCoeff;
		bInternalTimeStep = rData.bInternalTimeStep;
		nInternalTimeStep = rData.nInternalTimeStep;
		bAutoTimeStep = rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(rData.nAutoTimeStep));
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		bVariation = rData.bVariation;
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag,  rData.bConvFlag, sizeof(rData.bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(rData.dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		memcpy(StctCase_NotUse, rData.StctCase, sizeof(rData.StctCase));
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;
		memcpy(GilcCase, rData.GilcCase, sizeof(rData.GilcCase));
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nBeamSectionChange = rData.nBeamSectionChange;
		nNonlAnalType = rData.nNonlAnalType;
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;
		bSaveCurStageOutput = rData.bSaveCurStageOutput;
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		nRestartStageCount = rData.nRestartStageCount;
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		memcpy(LoadCaseType2_NotUse, rData.LoadCaseType2, sizeof(rData.LoadCaseType2));
		memcpy(LoadCaseType3_NotUse, rData.LoadCaseType3, sizeof(rData.LoadCaseType3));
		nNumStctCase2_NotUse = rData.nNumStctCase2;
		nNumStctCase3_NotUse = rData.nNumStctCase3;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i] = rData.StctCase2[i];
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i] = rData.StctCase3[i];
		memcpy(strErection1_NotUse, rData.strErection1, sizeof(rData.strErection1));
		memcpy(strErection2_NotUse, rData.strErection2, sizeof(rData.strErection2));
		memcpy(strErection3_NotUse, rData.strErection3, sizeof(rData.strErection3));
		bApplyCamber = rData.bApplyCamber;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		nErectionCount = 0;
		NlCtrl.Initialize();
	}
	void Convert830(T_STCT_UDRD_D_CH_RW_830& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff = rData.bOnlyUserCreepCoeff;
		bInternalTimeStep = rData.bInternalTimeStep;
		nInternalTimeStep = rData.nInternalTimeStep;
		bAutoTimeStep = rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(rData.nAutoTimeStep));
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		bVariation = rData.bVariation;
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag,  rData.bConvFlag, sizeof(rData.bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(rData.dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		memcpy(StctCase_NotUse, rData.StctCase, sizeof(rData.StctCase));
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;
		memcpy(GilcCase, rData.GilcCase, sizeof(rData.GilcCase));
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nBeamSectionChange = rData.nBeamSectionChange;
		nNonlAnalType = rData.nNonlAnalType;
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;
		bSaveCurStageOutput = rData.bSaveCurStageOutput;
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		nRestartStageCount = rData.nRestartStageCount;
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = rData.nTndnTensType;
		
#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		nErectionCount = 0;
		NlCtrl.Initialize();
	}
	void Convert791(T_STCT_UDRD_D_CH_RW_791& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff = rData.bOnlyUserCreepCoeff;
		bInternalTimeStep = rData.bInternalTimeStep;
		nInternalTimeStep = rData.nInternalTimeStep;
		bAutoTimeStep = rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(rData.nAutoTimeStep));
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		bVariation = rData.bVariation;
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag,  rData.bConvFlag, sizeof(rData.bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(rData.dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		memcpy(StctCase_NotUse, rData.StctCase, sizeof(rData.StctCase));
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;
		memcpy(GilcCase, rData.GilcCase, sizeof(rData.GilcCase));
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nBeamSectionChange = rData.nBeamSectionChange;
		nNonlAnalType = rData.nNonlAnalType;
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;
		bSaveCurStageOutput = rData.bSaveCurStageOutput;
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		nRestartStageCount = rData.nRestartStageCount;
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = rData.bApplyTDElastic;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		nErectionCount = 0;
		NlCtrl.Initialize();
	}
	void Convert785(T_STCT_UDRD_D_CH_RW_785& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff = rData.bOnlyUserCreepCoeff;
		bInternalTimeStep = rData.bInternalTimeStep;
		nInternalTimeStep = rData.nInternalTimeStep;
		bAutoTimeStep = rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(rData.nAutoTimeStep));
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		bVariation = rData.bVariation;
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag,  rData.bConvFlag, sizeof(rData.bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(rData.dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		memcpy(StctCase_NotUse, rData.StctCase, sizeof(rData.StctCase));
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;
		memcpy(GilcCase, rData.GilcCase, sizeof(rData.GilcCase));
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nBeamSectionChange = rData.nBeamSectionChange;
		nNonlAnalType = rData.nNonlAnalType;
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;
		bSaveCurStageOutput = rData.bSaveCurStageOutput;
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		nRestartStageCount = rData.nRestartStageCount;
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = rData.bChangeCableElementForPostCS;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		nErectionCount = 0;
		NlCtrl.Initialize();
	}
	void Convert730A(T_STCT_UDRD_D_CH_RW_730A& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff = rData.bOnlyUserCreepCoeff;
		bInternalTimeStep = rData.bInternalTimeStep;
		nInternalTimeStep = rData.nInternalTimeStep;
		bAutoTimeStep = rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(rData.nAutoTimeStep));
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		bVariation = rData.bVariation;
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag,  rData.bConvFlag, sizeof(rData.bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(rData.dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		memcpy(StctCase_NotUse, rData.StctCase, sizeof(rData.StctCase));
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;
		memcpy(GilcCase, rData.GilcCase, sizeof(rData.GilcCase));
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nBeamSectionChange = rData.nBeamSectionChange;
		nNonlAnalType = rData.nNonlAnalType;
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;
		bSaveCurStageOutput = rData.bSaveCurStageOutput;
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		nRestartStageCount = rData.nRestartStageCount;
		bApplyInitMemberForce = rData.bApplyInitMemberForce;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		nErectionCount = 0;
		NlCtrl.Initialize();
	}
	void Convert721(T_STCT_UDRD_D_CH_RW_721& rData)
	{
		nFinalStage = rData.nFinalStage;
		FinalStagKey = rData.FinalStagKey;
		bIncludeTimeDependentEffect = rData.bIncludeTimeDependentEffect;
		if(rData.bIncludeNonLinearAnalysis) nIncludeNonLinearAnalysis = 1;
		else                                nIncludeNonLinearAnalysis = 0;
		bCreepShrinkage = rData.bCreepShrinkage;
		nType = rData.nType;
		nIter = rData.nIter;
		dTol = rData.dTol;
		bOnlyUserCreepCoeff = rData.bOnlyUserCreepCoeff;
		bInternalTimeStep = rData.bInternalTimeStep;
		nInternalTimeStep = rData.nInternalTimeStep;
		bAutoTimeStep = rData.bAutoTimeStep;
		memcpy(nAutoTimeStep, rData.nAutoTimeStep, sizeof(rData.nAutoTimeStep));
		bTTLEElasticShortening = rData.bTTLEElasticShortening;
		bVariation = rData.bVariation;
		bTTLECreepShrinkage = rData.bTTLECreepShrinkage;
		nMaxIterPerLoadStep = rData.nMaxIterPerLoadStep;
		memcpy(bConvFlag,  rData.bConvFlag, sizeof(rData.bConvFlag));
		memcpy(dConvValue, rData.dConvValue, sizeof(rData.dConvValue));
		nCablePretensionForceCtrl = rData.nCablePretensionForceCtrl;
		nNumStctCase_NotUse = rData.nNumStctCase;
		memcpy(StctCase_NotUse, rData.StctCase, sizeof(rData.StctCase));
		bCalcCompSectionPart = rData.bCalcCompSectionPart;
		nCablePretensionExternalOpt = rData.nCablePretensionExternalOpt;
		bConvFinalStageMemberForce = rData.bConvFinalStageMemberForce;
		bConvTruss = rData.bConvTruss;
		bConvBeam = rData.bConvBeam;
		nNumLoadStep = rData.nNumLoadStep;
		bInitTangentDisp = rData.bInitTangentDisp;
		nInitTangentDisp = rData.nInitTangentDisp;
		KeyGrup = rData.KeyGrup;
		bCalcConcurrentForce = rData.bCalcConcurrentForce;
		memcpy(LoadCaseType_NotUse, rData.LoadCaseType, sizeof(rData.LoadCaseType));
		bSecondaryDLEffect = rData.bSecondaryDLEffect;
		nNumGilcCase = rData.nNumGilcCase;
		memcpy(GilcCase, rData.GilcCase, sizeof(rData.GilcCase));
		bConsiderConfinement = rData.bConsiderConfinement;
		bConsiderEndPartStress = rData.bConsiderEndPartStress;
		nEndPartStress = rData.nEndPartStress;
		dStressRatio = rData.dStressRatio;
		nBeamSectionChange = rData.nBeamSectionChange;
		nNonlAnalType = rData.nNonlAnalType;
		bIncludeEquiMembForce = rData.bIncludeEquiMembForce;
		bSaveCurStageOutput = rData.bSaveCurStageOutput;
		bLackofFitForce = rData.bLackofFitForce;
		KeyGrup4LackofFit = rData.KeyGrup4LackofFit;
		bIncludePDeltaEffect = rData.bIncludePDeltaEffect;
		bRestartStage = rData.bRestartStage;
		nRestartStageCount = rData.nRestartStageCount;
		bApplyInitMemberForce = FALSE;
		bChangeCableElementForPostCS = FALSE;
		bApplyTDElastic = FALSE;
		nTndnTensType = 0;

#if defined(_US) || defined(_RUS)
		CString str = _T("DW");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#else
		CString str = _T("D");
		ConvertStrChar(str, LoadCaseType2_NotUse, sizeof(LoadCaseType2_NotUse));
		ConvertStrChar(str, LoadCaseType3_NotUse, sizeof(LoadCaseType3_NotUse));
#endif
		
		nNumStctCase2_NotUse = 0;
		nNumStctCase3_NotUse = 0;
		for(int i=0; i<D_STCT_NUMSTCT; ++i) StctCase2_NotUse[i].Initialize();
		for(int i = 0; i<D_STCT_NUMSTCT; ++i) StctCase3_NotUse[i].Initialize();
		memset(strErection1_NotUse, 0, sizeof(strErection1_NotUse));
		memset(strErection2_NotUse, 0, sizeof(strErection2_NotUse));
		memset(strErection3_NotUse, 0, sizeof(strErection3_NotUse));
		bApplyCamber = FALSE;
		bSelfCons = FALSE;
		bConvergenceFail = FALSE;
		nMaxBiSectLoadStep = 5;
		nMaxDivergedStep = 3;
		nErectionCount = 0;
		NlCtrl.Initialize();
	}
};
struct T_STCT_UNIT
{
	T_STCT_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZESTCT 1

// restart construction stage Analysis
#define T_RCSA_K unsigned int
struct T_RCSA_D
{
	T_STAG_K RestartStagK;
	BOOL     bPostCS;
	CArray<T_STAG_K, T_STAG_K> aEnableRestartStagK;
	void Initialize()
	{
		RestartStagK = 0;
		aEnableRestartStagK.RemoveAll();
		bPostCS = TRUE;
	}
	T_RCSA_D() {Initialize();}
	T_RCSA_D(T_RCSA_D& src) { *this = src; }
	T_RCSA_D& operator=(T_RCSA_D& src)
	{
		RestartStagK = src.RestartStagK;
		bPostCS = src.bPostCS;
		aEnableRestartStagK.Copy(src.aEnableRestartStagK);
		return *this;
	}
};
struct T_RCSA_UDRD_D
{
	T_RCSA_D data;
};
struct T_RCSA_UDRD_D_RW
{
	T_STAG_K RestartStagK;
	BOOL     bPostCS;
	int      nCountEnableRestartStagK;
	void GetRcsa(T_RCSA_D& rData)
	{ 
		rData.RestartStagK = RestartStagK;
		rData.bPostCS      = bPostCS;
	}
	void SetRcsa(const T_RCSA_D& rData)
	{    
		RestartStagK = rData.RestartStagK;
		bPostCS      = rData.bPostCS;
		nCountEnableRestartStagK = (int)rData.aEnableRestartStagK.GetSize();
	}
};
struct T_RCSA_UNIT
{
	T_RCSA_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZERCSA 1

// construction stage load case : old version (use sgld)
#define T_CSLD_K unsigned int
struct T_CSLD_D
{
	CString LoadCaseName;
	CString Description;
	unsigned int LoadCaseId;
};
#define HASHSIZECSLD 1

#define T_SGLD_K T_STLD_K
#define T_SGLD_D T_STLD_D

#define D_SGLD_DL_GEN   1
#define D_SGLD_ER1_GEN  2
#define D_SGLD_ER2_GEN  3
#define D_SGLD_ER3_GEN  4
#define D_SGLD_ER4_GEN  5
#define D_SGLD_ER5_GEN  6
#define D_SGLD_ER6_GEN  7
#define D_SGLD_ER7_GEN  8
#define D_SGLD_ER8_GEN  9
#define D_SGLD_ER9_GEN  10
#define D_SGLD_ER10_GEN 11
#define D_SGLD_ER11_GEN 12
#define D_SGLD_ER12_GEN 13
#define D_SGLD_ER13_GEN 14
#define D_SGLD_ER14_GEN 15
#define D_SGLD_ER15_GEN 16
#define D_SGLD_ER16_GEN 17
#define D_SGLD_ER17_GEN 18
#define D_SGLD_ER18_GEN 19
#define D_SGLD_ER19_GEN 20
#define D_SGLD_ER20_GEN 21
#define D_SGLD_TP_GEN   22
#define D_SGLD_TS_GEN   23
#define D_SGLD_CR_GEN   24
#define D_SGLD_SH_GEN   25
#define D_SGLD_SUM_GEN  26

#define D_SGLD_DL_CVL   1
#define D_SGLD_ER1_CVL  2
#define D_SGLD_ER2_CVL  3
#define D_SGLD_ER3_CVL  4
#define D_SGLD_ER4_CVL  5
#define D_SGLD_ER5_CVL  6
#define D_SGLD_ER6_CVL  7
#define D_SGLD_ER7_CVL  8
#define D_SGLD_ER8_CVL  9
#define D_SGLD_ER9_CVL  10
#define D_SGLD_ER10_CVL 11
#define D_SGLD_ER11_CVL 12
#define D_SGLD_ER12_CVL 13
#define D_SGLD_ER13_CVL 14
#define D_SGLD_ER14_CVL 15
#define D_SGLD_ER15_CVL 16
#define D_SGLD_ER16_CVL 17
#define D_SGLD_ER17_CVL 18
#define D_SGLD_ER18_CVL 19
#define D_SGLD_ER19_CVL 20
#define D_SGLD_ER20_CVL 21
#define D_SGLD_TP_CVL   22
#define D_SGLD_TS_CVL   23
#define D_SGLD_CP_CVL   24
#define D_SGLD_CS_CVL   25
#define D_SGLD_SP_CVL   26
#define D_SGLD_SS_CVL   27
#define D_SGLD_SUM_CVL  28

#define HASHSIZESGLD 31
// Civil NX MEC 에서만 사용. Tendon이 Primary/Secondary 말고 Tendon 정보만 따로 가지고 있기도 해서..
#define D_SGLD_TD_CVL_NX_MEC 29
#define D_SGLD_SCAS_CVL_NX_MEC 30

/// tmld  : time load
#define T_TMLD_KEY unsigned int
union T_TMLD_K
{
	T_TMLD_KEY keymap;
	struct
	{
		unsigned int entity : 20; // element
		unsigned int serial : 12;
	} key;
};
struct T_TMLD_D
{
	double dDay;
	T_LDGR_K GroupKey;
	void Initialize()
	{
		dDay = 0.0;
		GroupKey = 0;
	}
};
struct T_TMLD_UDRD_D
{
	T_TMLD_K key;
	T_TMLD_D data;
};
struct T_TMLD_UNIT
{
	T_TMLD_UNIT()
	{
		dDay=D_UNITSYS_BASE_DAY;
	}
	int dDay;
};
#define HASHSIZETMLD 97

/// crpc  : creep coefficient
#define T_CRPC_KEY unsigned int
union T_CRPC_K
{
	T_CRPC_KEY keymap;
	struct
	{
		unsigned int entity : 20; // element
		unsigned int serial : 12;
	} key;
};
struct T_CRPC_D
{
	double dCreep;
	T_LDGR_K GroupKey;
	void Initialize()
	{
		dCreep = 0.0;
		GroupKey = 0;
	}
};
struct T_CRPC_UDRD_D
{
	T_CRPC_K key;
	T_CRPC_D data;
};
struct T_CRPC_UNIT
{
	T_CRPC_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZECRPC 97

struct T_RPOS_BASE
{
	T_STAG_K Stag;
	T_NODE_K Node;
	void Initialize()
	{
		Stag=0;
		Node=0;
	}
};
#define T_RPOS_DT CArray<T_RPOS_BASE, T_RPOS_BASE&>
#define T_RPOS_K unsigned int
struct T_RPOS_D
{
	CString Name;
	CString Description;
	unsigned int nSeq;    // 순번
	T_RPOS_DT aReacPos;
	void Initialize()
	{
		Name.Empty();
		Description.Empty();
		nSeq=0;
		aReacPos.RemoveAll();
	}
	T_RPOS_D()
	{
	}
	T_RPOS_D(T_RPOS_D& src)
	{
		*this = src;
	}
	T_RPOS_D& operator=(T_RPOS_D& src)
	{
		Name=src.Name;
		Description = src.Description;
		nSeq=src.nSeq;
		aReacPos.Copy(src.aReacPos);
		return *this;
	}
};
struct T_RPOS_D_CH
{
	char Name[20];
	char Description[80];
	unsigned int nSeq;    // 순번
	T_RPOS_DT aReacPos;
	void ConvertToString(T_RPOS_D& rData)
	{
		ConvertCharStr(Name, rData.Name, sizeof(Name));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.nSeq=nSeq;
		rData.aReacPos.Copy(aReacPos);
	}
	void ConvertToChar(T_RPOS_D& rData)
	{
		ConvertStrChar(rData.Name, Name, sizeof(Name));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		nSeq=rData.nSeq;
		aReacPos.Copy(rData.aReacPos);
	}
};
struct T_RPOS_UDRD_D
{
	T_RPOS_K key;
	T_RPOS_D data;
};
struct T_RPOS_UDRD_D_CH
{
	T_RPOS_K key;
	T_RPOS_D_CH data;
};
struct T_RPOS_UDRD_D_CH_RW
{
	T_RPOS_K key;
	char Name[20];
	char Description[80];
	unsigned int nSeq;    // 순번
	int nCountReacPos;

	void GetRpos(T_RPOS_K& rKey, T_RPOS_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.Name, Name, sizeof(rData.Name));
		memcpy(rData.Description, Description, sizeof(Description));
		rData.nSeq=nSeq;
	}
	void SetRpos(const T_RPOS_K Key, const T_RPOS_D_CH& rData)
	{
		key=Key;
		memcpy(Name, rData.Name, sizeof(Name));
		memcpy(Description, rData.Description, sizeof(Description));
		nSeq=rData.nSeq;
		nCountReacPos=(int)rData.aReacPos.GetSize();
	}
};
struct T_RPOS_UNIT
{
	T_RPOS_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZERPOS  101

// stage analysis control
#define T_AETL_K unsigned int
struct T_AETL_AASHTO06_D
{
	int iEstMethod; //공통 0->Rational Approximate Method 1-> Lump Sum Estimate Method
	int iTendonType; //공통 iEstMethod 가 Rational 일때 0-> Low-relaxtion Strand, 1-> Stress-relieved strand
									 //                가 Lump 일때     0-> Wire or Strand 1-> Low-relaxtion Strand 2-> Bar   
	//////  Rational ///////////////
	double dHumidity; //Ambient Relative Humidity 범위 0.0<=  => 100.0 초기값 0.0
	double dInitStrength;//Compressive Strength of Concrete at time of Initial Loading 범위 >0.0 초기값 0.0
	int iRelaxation;//Relaxtion Loss 0-> PerCode 1-> User Defined
	double dRelaxationLoss;// User Defined 일 경우 직접 입력 범위 >= 0.0 초기값 0.0
	/////// Lump ///////////////////
	int iSectionType;//Type of Beam Section 0-> Rectangular, 1-> beam, 2-> single t,double t
	int iLevel; //Level 0-> Average , 1-> Upper Bound
	double dPRatio; //Partial Prestress Ratio ,Range 0.2<=  >=1.0 Initail Value 1.0
	double dSpecStrength;//Specified Compressive Strength of Concrete  ,Range >0.0  Initial Value 0.0
	BOOL bLowDensity;//Structual Low-density Concree ,Initial Value 0.0
	void Initialize()
	{
		iEstMethod=0;
		iTendonType=0;
		dHumidity=0.0;
		dInitStrength=0.0;
		iRelaxation=0;
		dRelaxationLoss=0.0;
		iSectionType=0;
		iLevel=0;
		dPRatio=1.0;
		dSpecStrength=0.0;
		bLowDensity=FALSE;
	}
};
struct T_AETL_PCI04_D
{
	int iEstMethod; //공통 0->Rational Approximate Method 1-> Lump Sum Estimate Method
	//////  Rational ///////////////
	double dHumidity; //Ambient Relative Humidity 범위 0.0<=  => 100.0 초기값 0.0
	BOOL bRelaxCoeff;//Relaxation Coefficient 
	double dRelaxCoeff;//Range >0.0 , Initial Value 10
	BOOL bRelaxAdjust;//Relaxtion Adjustement 
	double dRelaxAdjust;//Range 0.0<=  >= 100.0, Initail Value 30
	double dRelaxAge;//Age of Relaxation Calculation ,Range >0.0 , Initail Value 0.0
	/////// Lump ///////////////////
	int iTendonType; //0-> Wire or Strand 1-> Low-relaxtion Strand 2-> Bar   
	int iSectionType;//Type of Beam Section 0-> Rectangular, 1-> Box Girder, 2-> I Girder, 4-> Single T, double T
	int iLevel; //Level 0-> Average , 1-> Upper Bound
	double dPRatio; //Partial Prestress Ratio ,Range 0.2<=  >=1.0 Initail Value 1.0
	double dSpecStrength;//Specified Compressive Strength of Concrete  ,Range >0.0  Initial Value 0.0
	BOOL bLowDensity;//Structual Low-density Concree 
	void Initialize()
	{
	 iEstMethod=0;
	 dHumidity=0.0;
	 bRelaxCoeff=TRUE;
	 dRelaxCoeff=10.0;
	 bRelaxAdjust=TRUE;
	 dRelaxAdjust=30.0;
	 dRelaxAge=0.0;
	 iTendonType=0;   
	 iSectionType=0;
	 iLevel=0;
	 dPRatio=1.0;
	 dSpecStrength=0.0;
	 bLowDensity=FALSE;
	}

};
struct T_AETL_JSCE02_D
{
	double dHumidity;//Ambient Relative Humidity(45-80) ,Range 45<=  >=80 , Initial Value 70.0
	double dSurRatio;//Volume Surface Ratio(100-300mm), Range >0 , Initial Value 0
	double dCeContent;//Cement Content(260-500kg/m^3), Range >0, Initail Value 0
	double dWaContent;//Water Content(130-230kg/m^3),Range >0 ,Initial Value 0
	double dRelaxRatio;//Relaxation Ratio , Range >=0.0 , Initial Value 5
	double dShrinkageDay;//Begging of Shrinkage (t0)(1-98), Range 1<=  >=98 , Initial Value 3
	double dLoadingDay;//Age of Loading(t'), Range >=dShrinkage, Initial Value 3
	double dCalcDay;//Age of Calculation(t),Range >=dLoadingDay, Initail Value 3
	void Initialize()
	{
		dHumidity=70.0;
		dSurRatio=0.0;
		dCeContent=0.0;
		dWaContent=0.0;
		dRelaxRatio=5.0;
		dShrinkageDay=3.0;
		dLoadingDay=3.0;
		dCalcDay=3.0;
	}
};

struct T_AETL_SNIP_D
{
	double dShrinkageDay; // Begging of Shrinkage (t0)(1-98), Range 1<=  >=98,
	double dLoadingDay;   // Age of Loading(t'), Range >=dShrinkage,
	double dCalcDay;      // Age of Calculation(t),Range >=dLoadingDay,
	double dConcRbp;      // Transfer Strength of Concrete (Rbp)
	int    nConcClass; 
	int    nAlphaType;
	double dHardend;
	double dHeat;
	int    nPreRebar;
	int    nRelaType;
	
	void Initialize()
	{
		dShrinkageDay=3.0;
		dLoadingDay=3.0;
		dCalcDay=3.0;
		dConcRbp=0.0;
		nConcClass=0;
		nAlphaType=0;
		dHardend=1.0;
		dHeat=0.85;
		nPreRebar=0;
		nRelaType=0;
	}
};

enum
{
	AETL_CODE_AASHTI06 = 0, // AASHTO-LRFD 06
	AETL_CODE_PCI04,	    // PCI Bridge Design Manual 04
	AETL_CODE_JSCE02,	    // JSCE02
	AETL_CODE_SNIP,		    // SNiP 2.05.03-84*
	AETL_CODE_SP		    // SP 35.13330.2011
};

struct T_AETL_D
{
	int iDesignCode;
	CArray <T_STLD_K,T_STLD_K> arTendonLoad;
	CArray <T_STLD_K,T_STLD_K> arDeadLoad;
	CArray <T_STLD_K,T_STLD_K> arAddiLoad;
	T_AETL_AASHTO06_D AASHTO06Data;
	T_AETL_PCI04_D PCI04Data;
	T_AETL_JSCE02_D JSCE02Data;
	T_AETL_SNIP_D SNiPData;

	T_AETL_D()
	{
	}
	void Initialize()
	{
		iDesignCode=0;
		arTendonLoad.RemoveAll();
		arDeadLoad.RemoveAll();
		arAddiLoad.RemoveAll();
		AASHTO06Data.Initialize();
		PCI04Data.Initialize();
		JSCE02Data.Initialize();
		SNiPData.Initialize();
	}
	T_AETL_D(T_AETL_D& src)
	{
		*this = src;
	}
	T_AETL_D& operator=(const T_AETL_D& src)
	{
		iDesignCode=src.iDesignCode;
		arTendonLoad.Copy(src.arTendonLoad);
		arDeadLoad.Copy(src.arDeadLoad);
		arAddiLoad.Copy(src.arAddiLoad);
		AASHTO06Data=src.AASHTO06Data;
		PCI04Data=src.PCI04Data;
		JSCE02Data=src.JSCE02Data;
		SNiPData=src.SNiPData;
		
		return *this;
	}
};
struct T_AETL_UDRD_D
{  
	T_AETL_D data;
};
struct T_AETL_UDRD_D_RW_821
{
	int iDesignCode;
	int nTendonLoad;
	int nDeadLoad;
	int nAddiLoad;
	T_AETL_AASHTO06_D AASHTO06Data;
	T_AETL_PCI04_D PCI04Data;
	T_AETL_JSCE02_D JSCE02Data;
};
struct T_AETL_UDRD_D_RW
{
	int iDesignCode;
	int nTendonLoad;
	int nDeadLoad;
	int nAddiLoad;
	T_AETL_AASHTO06_D AASHTO06Data;
	T_AETL_PCI04_D PCI04Data;
	T_AETL_JSCE02_D JSCE02Data;
	T_AETL_SNIP_D SNiPData;
	
	void GetAetl(T_AETL_D& rData)
	{ 
		rData.iDesignCode = iDesignCode;
		rData.AASHTO06Data = AASHTO06Data;
		rData.JSCE02Data = JSCE02Data;
		rData.PCI04Data = PCI04Data;
		rData.SNiPData = SNiPData;
	}
	void SetAetl(const T_AETL_D& rData)
	{    
		iDesignCode = rData.iDesignCode;
		AASHTO06Data = rData.AASHTO06Data;
		JSCE02Data = rData.JSCE02Data;
		PCI04Data = rData.PCI04Data;
		SNiPData = rData.SNiPData;
		nTendonLoad = (int)rData.arTendonLoad.GetSize();
		nDeadLoad = (int)rData.arDeadLoad.GetSize();
		nAddiLoad = (int)rData.arAddiLoad.GetSize();
	}
	void Convert821(T_AETL_UDRD_D_RW_821& rData)
	{
		iDesignCode = rData.iDesignCode;
		AASHTO06Data = rData.AASHTO06Data;
		JSCE02Data = rData.JSCE02Data;
		PCI04Data = rData.PCI04Data;
		SNiPData.Initialize();
		nTendonLoad = rData.nTendonLoad;
		nDeadLoad = rData.nDeadLoad;
		nAddiLoad = rData.nAddiLoad;
	}
};
	
struct T_AETL_UNIT
{
	T_AETL_UNIT()
	{
		dInitStrength=D_UNITSYS_BASE_STRESS;
		dRelaxaionLoss=D_UNITSYS_BASE_STRESS;
		dSpecStrength=D_UNITSYS_BASE_STRESS;
		dSurRatio=D_UNITSYS_BASE_LENGTH;
		dCeContent=D_UNITSYS_BASE_DENSITY;
		dWaContent=D_UNITSYS_BASE_DENSITY;
		dConcRbp=D_UNITSYS_BASE_STRESS;
	}
	int dInitStrength;
	int dRelaxaionLoss;
	int dSpecStrength;
	int dSurRatio;
	int dCeContent;
	int dWaContent;
	int dConcRbp;
};
#define HASHSIZEAETL 1

#define T_DGMC_K T_STAG_K
struct T_DGMC_D
{
	COLORREF Color;
	void Initialize()
	{
		Color=COLORREF(RGB(0, 0, 255));
	}
};

struct T_DGMC_UDRD_D
{
	T_DGMC_K key;
	T_DGMC_D data;
};
struct T_DGMC_UNIT
{
	T_DGMC_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEDGMC HASHSIZESTAG

#define T_DGML_K unsigned int
struct T_DGML_D
{
	unsigned int AnalType;   //D_LCOM_???
	unsigned int LoadCaseKey;
	COLORREF Color[3];
	void Initialize()
	{
		AnalType = 0;
		LoadCaseKey = 0;
		Color[0]=Color[1]=Color[2]=COLORREF(RGB(0, 0, 255));
	}
};

struct T_DGML_UDRD_D
{
	T_DGML_K key;
	T_DGML_D data;
};
struct T_DGML_UNIT
{
	T_DGML_UNIT()
	{
		NoUnit=D_UNITSYS_NONE;
	}
	int NoUnit;
};
#define HASHSIZEDGML 101

// Camber for Construction Stage
#define  T_CMCS_K T_NODE_K
struct T_CMCS_D
{
	double dDeform; // Deformation Camber
	double dUser;   // User Defined Camber

	void Initialize()
	{
		dDeform = 0.0;
		dUser = 0.0;
	}

	T_CMCS_D() {}
	T_CMCS_D(T_CMCS_D& src) { *this = src; }
	T_CMCS_D& operator=(T_CMCS_D& src)
	{
		dDeform = src.dDeform;
		dUser   = src.dUser;
		return *this;
	}
};
struct T_CMCS_UDRD_D
{
	T_CMCS_K key;
	T_CMCS_D data;
};
struct T_CMCS_UNIT
{
	T_CMCS_UNIT()
	{
		dDeform=D_UNITSYS_BASE_LENGTH;
		dUser=D_UNITSYS_BASE_LENGTH;
	}
	int dDeform;
	int dUser;
};
#define HASHSIZECMCS 97



#pragma pack(pop)

#endif  // __DB_ST_DT_STAG_H__

