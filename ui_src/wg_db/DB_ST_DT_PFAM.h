#pragma once
#include "DB_ST_DT_PF_BASE.h"
#include "DB_ST_DT_PFSB.h"

#define MIDAS_SDK

#pragma pack(push, 1)

#define T_PFAM_K unsigned int
#define HASHSIZE_PFAM 1001

class CUnitCtrl;
#include "HeaderPre.h"
struct __MY_EXT_CLASS__ T_PFAM_D:public T_PF_BASE_D
{
public:
	T_PFAM_D(void);
	T_PFAM_D(const T_PFAM_D& src);
	~T_PFAM_D(void);

	enum EnGenerateType
	{
		EnGenerateType_ElemBoundary = 0,
		EnGenerateType_OnlyBoundary = 1,
	};
	enum EnLayoutType
	{
		EnLayoutType_Auto    = 0,
		EnLayoutType_Mannual = 1,
	};

public:
	BOOL           bSelect                 ;
	CString        strName                 ;
	EnGenerateType enGenerateType          ;
	EnLayoutType   enLayoutType            ;
	T_NODE_K       keyNodeCushionCapBottom[10] ;
	T_PFSB_K       keySoilBoring           ;
	UINT           iLongitudinalPileCount  ;
	UINT           iHorizonalPileCount     ;
	double         dLongitudinalPileSpacing;
	double         dHorizonalPileSpacing   ;
	T_MATL_K       keyPileMalt             ;
	double         dPileDiameter           ;
	double         dPileLength             ;
	double         dDivLength              ;
	double         dScouringLine2Top       ;//一般冲刷线位置 正数表示冲刷线在桩基顶面向上，负数表示向下
	double         dTopLevel               ;//土层顶位置     正数表示土层在桩基顶面向上，负数表示向下
	double         dScaleFactor            ;//adjust factor

public:
	virtual void Init(void);
	virtual void S2D(const ftmXmlNode* pXmlNode);
	virtual void D2S(ftmXmlNode* pXmlNode) const;
	virtual void ConvertUintIn(CUnitCtrl* pUnitCtrl);
	virtual void ConvertUintOut(CUnitCtrl* pUnitCtrl);
	virtual void ConvertUnitPrev(CUnitCtrl* pUnitCtrl);

	T_PFAM_D& operator=(const T_PFAM_D& src);
};

//for undo/redo
struct __MY_EXT_CLASS__ T_PFAM_UDRD_D
{
	T_PFAM_K key;
	T_PFAM_D data;
};

//for read/write
struct __MY_EXT_CLASS__ T_PFAM_D_CH_9300
{
public:
	BOOL           bSelect;
	char           strName[40];
	UINT           enGenerateType;
	UINT           enLayoutType;
	T_NODE_K       keyNodeCushionCapBottom;
	T_PFSB_K       keySoilBoring;
	UINT           iLongitudinalPileCount;
	UINT           iHorizonalPileCount;
	double         dLongitudinalPileSpacing;
	double         dHorizonalPileSpacing;
	T_MATL_K       keyPileMalt;
	double         dPileDiameter;
	double         dPileLength;
	double         dDivLength;
	double         dScouringLine2Top;
	double         dTopLevel;

	void Init(void);
	void ConvertToString(T_PFAM_D& rData);
};

struct __MY_EXT_CLASS__ T_PFAM_D_CH_9301
{
public:
	BOOL           bSelect;
	char           strName[40];
	UINT           enGenerateType;
	UINT           enLayoutType;
	T_NODE_K       keyNodeCushionCapBottom[10];
	T_PFSB_K       keySoilBoring;
	UINT           iLongitudinalPileCount;
	UINT           iHorizonalPileCount;
	double         dLongitudinalPileSpacing;
	double         dHorizonalPileSpacing;
	T_MATL_K       keyPileMalt;
	double         dPileDiameter;
	double         dPileLength;
	double         dDivLength;
	double         dScouringLine2Top;
	double         dTopLevel;

	void Init(void);
	void ConvertToString(T_PFAM_D& rData);
};

struct __MY_EXT_CLASS__ T_PFAM_D_CH
{
public:
	BOOL           bSelect;
	char           strName[40];
	UINT           enGenerateType;
	UINT           enLayoutType;
	T_NODE_K       keyNodeCushionCapBottom[10];
	T_PFSB_K       keySoilBoring;
	UINT           iLongitudinalPileCount;
	UINT           iHorizonalPileCount;
	double         dLongitudinalPileSpacing;
	double         dHorizonalPileSpacing;
	T_MATL_K       keyPileMalt;
	double         dPileDiameter;
	double         dPileLength;
	double         dDivLength;
	double         dScouringLine2Top;
	double         dTopLevel;
	double         dScaleFactor;

	void Init(void);
	void ConvertToString(T_PFAM_D& rData);
	void ConvertToChar(T_PFAM_D& rData);
	void Convert9300(const T_PFAM_D_CH_9300& rData);
	void Convert9301(const T_PFAM_D_CH_9301& rData);
};

struct __MY_EXT_CLASS__ T_PFAM_UDRD_D_CH_9300
{
	T_PFAM_K    key ;
	T_PFAM_D_CH_9300 data;
};

struct __MY_EXT_CLASS__ T_PFAM_UDRD_D_CH_9301
{
	T_PFAM_K    key;
	T_PFAM_D_CH_9301 data;
};


struct __MY_EXT_CLASS__ T_PFAM_UDRD_D_CH
{
	T_PFAM_K     key ;
	T_PFAM_D_CH  data;
};
#include "HeaderPost.h"

#pragma pack(pop)