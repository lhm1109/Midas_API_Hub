#pragma once
#include "DB_ST_DT_PF_BASE.h"
#include "DB_ST_DT_PFSL.h"

#define MIDAS_SDK

#pragma pack(push, 1)

#define T_PFSB_K unsigned int
#define HASHSIZE_PFSB 1001

//Pile foundation soil boring
#include "HeaderPre.h"
struct __MY_EXT_CLASS__ T_PFSB_BASE_D :public T_PF_BASE_D
{
public:
	T_PFSB_BASE_D(void);
	~T_PFSB_BASE_D(void);

public:
	T_PFSB_K keySoilLayer;
	double   dThick;

public:
	virtual void Init(void);
	virtual void S2D(const ftmXmlNode* pXmlNode);
	virtual void D2S(ftmXmlNode* pXmlNode) const;
	virtual void ConvertUintIn(CUnitCtrl* pUnitCtrl);
	virtual void ConvertUintOut(CUnitCtrl* pUnitCtrl);
	virtual void ConvertUnitPrev(CUnitCtrl* pUnitCtrl);
};

struct __MY_EXT_CLASS__ T_PFSB_D:public T_PF_BASE_D
{
public:
	T_PFSB_D(void);
	~T_PFSB_D(void);

public:
	CString       strName     ;
	std::vector<T_PFSB_BASE_D> vctSoilLayer;

public:
	virtual void Init(void);
	virtual void S2D(const ftmXmlNode* pXmlNode);
	virtual void D2S(ftmXmlNode* pXmlNode) const;
	virtual void ConvertUintIn(CUnitCtrl* pUnitCtrl);
	virtual void ConvertUintOut(CUnitCtrl* pUnitCtrl);
	virtual void ConvertUnitPrev(CUnitCtrl* pUnitCtrl);

public:
	double GetTotalDepth(void) const;
};

//for undo redon
struct __MY_EXT_CLASS__ T_PFSB_UDRD_D
{
	T_PFSB_K key;
	T_PFSB_D data;
};

//for read/write
struct __MY_EXT_CLASS__ T_PFSB_D_CH
{
public:
	T_PFSB_D_CH(void);
	~T_PFSB_D_CH(void);
public:
	char                       strName[40];
	CArray<T_PFSB_BASE_D, T_PFSB_BASE_D&> arSoilLayer;

	void Init(void);
	void ConvertToString(T_PFSB_D& rData);
	void ConvertToChar(T_PFSB_D& rData);
};

struct __MY_EXT_CLASS__ T_PFSB_UDRD_D_CH
{
	T_PFSB_K    key;
	T_PFSB_D_CH data;
};

struct __MY_EXT_CLASS__ T_PFSB_UDRD_D_CH_RW
{
	T_PFSB_K     key;
	char         strName[40];
	unsigned int uiSoilLayoutCount;
	void Init(void);
	void GetPfsb(T_PFSB_K& rKey, T_PFSB_D_CH& rData);
	void SetPfsb(const T_PFSB_K rKey, const T_PFSB_D_CH& rData);
};
#include "HeaderPost.h"

#pragma pack(pop)
