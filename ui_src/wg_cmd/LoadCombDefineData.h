#pragma once
#include <map>
#include "HeaderPre.h"

struct LOAD_CASE_K
{
	int		nLoadCaseType;	// 0: Static,	1: Response Spectrum,  2: comb
	UINT	Key;
	bool operator == (const LOAD_CASE_K &src) const
	{
		return Key == src.Key && nLoadCaseType == src.nLoadCaseType;
	}
	bool operator < (const LOAD_CASE_K &src) const
	{
		if(Key < src.Key)
			return true;
		if(Key > src.Key)
			return false;
		if(nLoadCaseType < src.nLoadCaseType)
			return true;
		if(nLoadCaseType > src.nLoadCaseType)
			return false;
		return false;
	}

	void Initialize()
	{
		nLoadCaseType = D_LCOM_STATIC;
		Key = 0;
	}
}; 
typedef CArrayEx<LOAD_CASE_K, LOAD_CASE_K&>	 LOAD_CASE_K_ARR;

struct __MY_EXT_CLASS__ LoadFactorTable
{
public:
	void initial();
	CArray<CString>	 m_aType;
	CArrayEx<CArrayEx<double, double>, CArrayEx<double, double>&>  m_aFactor;
};

 
class __MY_EXT_CLASS__ CLoadCombDefineData
{
public:
	CLoadCombDefineData();
	void initial();
public:
	BOOL	m_bImperfection;
	BOOL	m_bWindForward;
	CString	m_strCode;
	int     m_nStage;// 0:ST 1:CS 2:ST+CS
	//双向地震
	BOOL	m_bBiEarthquake;  //双向地震
	std::map<LOAD_CASE_K, LOAD_CASE_K>  m_mapBiEarthquake;
	//Comb Facto
	double  m_dCombFactor[3]; //L W T
	std::map<LOAD_CASE_K, double>  m_mapCombFactor;
	double  m_dLLAdjustCombFactor;
	//Special Factor
	std::map<LOAD_CASE_K, double>  m_mapSpecialFactor;
	LoadFactorTable	 m_item;
	//Gravity Factor
	double m_dLLGravityFactor;
	std::map<LOAD_CASE_K, double>  m_mapGravityFactor;
	//Live load  Adverse Or Mutex
	int m_nSpecitlArrange;//0 neither, 1 Adverse,  2 Mutex
	CArrayEx<LOAD_CASE_K_ARR, LOAD_CASE_K_ARR&>	m_aLLUnfavorable;
	std::map<LOAD_CASE_K, LOAD_CASE_K_ARR> m_mapExclusion;
	//风+地震
	std::map<LOAD_CASE_K, LOAD_CASE_K>	   m_mapBC2C;
};

//GB17-17&GB18-02&GB10-10&GB50429-2007 
//	D,	L,   LR,  W,	 E,  EVT,  S,   T,	PS,
const double CoefGB17_17[12][9] = {1.3, 1.5, 1.5, 0.0, 0.0, 0.0, 1.5, 0.0, 1.3,		//1
	1.0, 1.5, 1.5, 0.0, 0.0, 0.0, 1.5, 0.0, 1.0,	//2 
	1.3, 0.0, 0.0, 1.5, 0.0, 0.0, 0.0, 0.0, 1.3,	//3
	1.0, 0.0, 0.0, 1.5, 0.0, 0.0, 0.0, 0.0, 1.0,	//4
	1.3, 1.5, 1.5, 1.5, 0.0, 0.0, 1.5, 0.0, 1.3,	//5
	1.0, 1.5, 1.5, 1.5, 0.0, 0.0, 1.5, 0.0, 1.0,	//6
	1.3, 1.5, 1.5, 1.5, 0.0, 0.0, 1.5, 1.5, 1.3,	//7
	1.0, 1.5, 1.5, 1.5, 0.0, 0.0, 1.5, 1.5, 1.0,	//8
	1.3, 1.3, 1.3, 0.0, 1.4, 0.0, 1.3, 0.0, 0.0,	//9
	1.0, 1.0, 1.0, 0.0, 1.4, 0.0, 1.0, 0.0, 0.0,	//10
	1.3, 1.3, 1.3, 0.0, 0.0, 1.4, 1.3, 0.0, 0.0,	//11
	1.0, 1.0, 1.0, 0.0, 0.0, 1.4, 1.0, 0.0, 0.0};	//12

//GB17-03 && GB10-02				D,	  L,   LR,  W,	 E,  EVT,  S,   T,
const double CoefGB17_03[13][8] = {1.35, 1.4, 1.4, 0.0, 0.0, 0.0, 1.4, 1.4,	//1
	1.2, 1.4, 1.4, 0.0, 0.0, 0.0, 1.4, 0.0,	//2 
	1.0, 1.4, 1.4, 0.0, 0.0, 0.0, 1.4, 0.0,	//3
	1.2, 1.4, 1.4, 0.0, 0.0, 0.0, 1.4, 1.4,	//4
	1.0, 1.4, 1.4, 0.0, 0.0, 0.0, 1.4, 1.4,	//5
	1.2, 1.4, 1.4, 1.4, 0.0, 0.0, 1.4, 0.0,	//6
	1.0, 1.4, 1.4, 1.4, 0.0, 0.0, 1.4, 0.0,	//7
	1.2, 1.4, 1.4, 1.4, 0.0, 0.0, 1.4, 1.4,	//8
	1.0, 1.4, 1.4, 1.4, 0.0, 0.0, 1.4, 1.4,	//9
	1.2, 1.2, 1.2, 0.0, 1.3, 0.0, 1.2, 0.0,	//10
	1.0, 1.0, 1.0, 0.0, 1.3, 0.0, 1.0, 0.0,	//11
	1.2, 1.2, 1.2, 0.0, 0.0, 1.3, 1.2, 0.0,	//12
	1.0, 1.0, 1.0, 0.0, 0.0, 1.3, 1.0, 0.0};//13

//JGJ99-2015 & JGJ 3-2010		D,	  L,   LR,  W,	 E,  EVT,  S,   T,	PS,
const double CoefJGJ[20][9] = {1.3, 1.5, 1.5, 0.0, 0.0, 0.0, 1.5, 0.0, 1.3,	//1
	1.0, 1.5, 1.5, 0.0, 0.0, 0.0, 1.5, 0.0, 1.0,	//2 
	1.3, 1.5, 1.5, 0.0, 0.0, 0.0, 1.5, 1.5, 1.3,	//3
	1.0, 1.5, 1.5, 0.0, 0.0, 0.0, 1.5, 1.5, 1.0,	//4
	1.3, 1.5, 1.5, 1.5, 0.0, 0.0, 1.5, 0.0, 1.3,	//5
	1.0, 1.5, 1.5, 1.5, 0.0, 0.0, 1.5, 0.0, 1.0,	//6
	1.3, 1.5, 1.5, 1.5, 0.0, 0.0, 1.5, 1.5, 1.3,	//7
	1.0, 1.5, 1.5, 1.5, 0.0, 0.0, 1.5, 1.5, 1.0,	//8
	1.3, 1.3, 1.3, 0.0, 1.4, 0.0, 1.3, 0.0, 0.0,	//new 9
	1.0, 1.0, 1.0, 0.0, 1.4, 0.0, 1.0, 0.0, 0.0,	//new 10
	1.3, 1.3, 1.3, 0.0, 1.4, 0.5, 1.3, 0.0, 0.0,	//11
	1.0, 1.0, 1.0, 0.0, 1.4, 0.5, 1.0, 0.0, 0.0,	//12
	1.3, 1.3, 1.3, 0.0, 0.0, 1.4, 1.3, 0.0, 0.0,	//13
	1.0, 1.0, 1.0, 0.0, 0.0, 1.4, 1.0, 0.0, 0.0,	//14
	1.3, 1.3, 1.3, 1.5, 1.4, 0.0, 1.3, 0.0, 0.0,	//15
	1.0, 1.0, 1.0, 1.5, 1.4, 0.0, 1.0, 0.0, 0.0,	//16
	1.3, 1.3, 1.3, 1.5, 0.5, 1.4, 1.3, 0.0, 0.0,	//17
	1.0, 1.0, 1.0, 1.5, 0.5, 1.4, 1.0, 0.0, 0.0,	//18
	1.3, 1.3, 1.3, 1.5, 1.4, 0.5, 1.3, 0.0, 0.0,	//19
	1.0, 1.0, 1.0, 1.5, 1.4, 0.5, 1.0, 0.0, 0.0};	//20

//GB50069-02		D,	  L,   LR,  W,	 E,  EVT,  S,   T,	PS,  EH,   EV,   WP,   FP, SF,   B
const double CoefGB69_02[4][15] = {1.2, 1.4, 1.4, 1.4, 0.0, 0.0, 1.4, 1.4, 1.27, 1.27, 1.27, 1.4, 1.4, 1.4, 1.4,	//1
	1.0, 1.4, 1.4, 1.4, 0.0, 0.0, 1.4, 1.4, 1.0,  1.0,  1.0, 1.4, 1.4, 1.4, 1.4,	//2 
	1.2, 1.4, 1.4, 0.0, 1.3, 0.0, 1.4, 0.0, 0.0,  1.2,  1.2, 0.0, 0.0, 0.0, 0.0,	//3
	1.0, 1.4, 1.4, 0.0, 1.3, 0.0, 1.4, 0.0, 0.0,  1.0,  1.0, 0.0, 0.0, 0.0, 0.0};	//4
class __MY_EXT_CLASS__ ILoadCombDefineMgr : public CDBUpdateConnector
{
public:
	static ILoadCombDefineMgr* Instance();
	CLoadCombDefineData *Para();
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
private:
	ILoadCombDefineMgr();
	~ILoadCombDefineMgr();
public:
	void getCodeData(int nIndex, LoadFactorTable &TableItem, int &nRow, int &nCol);
	BOOL GetUsedKeyList(BOOL bSeismicEvaluation, CArray<T_STLD_K, T_STLD_K> &arKeyList);
	void GetLCaseCoef(double& dW, double& dL, double& dGL);
	void SetLCaseCoef(double dW, double dL, double dGL);
private:
	CLoadCombDefineData *m_pData;
	double m_dW;
	double m_dL;
	double m_dGL;
};

#include "HeaderPost.h"