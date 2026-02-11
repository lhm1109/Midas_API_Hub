#pragma once
#include "DB_ST_DT_PF_BASE.h"

#define MIDAS_SDK

#pragma pack(push, 1)

#define T_PFSL_K unsigned int
#define HASHSIZE_PFSL 1001

//Pile foundation soil layer

class CUnitCtrl;

#include "HeaderPre.h"
struct __MY_EXT_CLASS__ T_PFSL_D:public T_PF_BASE_D
{
public:
	T_PFSL_D(void);
	~T_PFSL_D(void);

public:
	CString strName;
	double dM     ;      
	double dM0    ;     
	BOOL   bWater ;  
	double dfa0   ;    
	double dGama  ;   
	double dGamab ;  
	double dPsi   ;    
	double dEs    ;     
	double dc     ;      
	double dqik   ;    
	double dBetasi; 
	double dBetap ;  
	double dqrs   ;    
	double dAlphai; 
	double dAlphar; 
	double dqrk   ;    
	double dc2i   ;    
	double dc1    ;     
	double dfrk   ;    

public:
	virtual void Init(void);
	virtual void S2D(const ftmXmlNode* pXmlNode);
	virtual void D2S(ftmXmlNode* pXmlNode) const;
	virtual void ConvertUintIn(CUnitCtrl* pUnitCtrl);
	virtual void ConvertUintOut(CUnitCtrl* pUnitCtrl);
	virtual void ConvertUnitPrev(CUnitCtrl* pUnitCtrl);
};

//for undo/redo
struct __MY_EXT_CLASS__ T_PFSL_UDRD_D
{
	T_PFSL_K key ;
	T_PFSL_D data;
};

//for read/write
struct __MY_EXT_CLASS__ T_PFSL_D_CH
{
public:
	char    strName[40];
	double dM;
	double dM0;
	BOOL   bWater;
	double dfa0;
	double dGama;
	double dGamab;
	double dPsi;
	double dEs;
	double dc;
	double dqik;
	double dBetasi;
	double dBetap;
	double dqrs;
	double dAlphai;
	double dAlphar;
	double dqrk;
	double dc2i;
	double dc1;
	double dfrk;

	void Init(void);
	void ConvertToString(T_PFSL_D& rData);
	void ConvertToChar(T_PFSL_D& rData);
};

struct __MY_EXT_CLASS__ T_PFSL_UDRD_D_CH
{
	T_PFSL_K    key;
	T_PFSL_D_CH data;
};
#include "HeaderPost.h"

#pragma pack(pop)