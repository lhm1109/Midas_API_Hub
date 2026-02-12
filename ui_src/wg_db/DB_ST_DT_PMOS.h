#ifndef __DB_ST_DT_PMOS_H__
#define __DB_ST_DT_PMOS_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#include <afxtempl.h>

// for Civil JP

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// 모델정보 출력항목 ///////////////////////////////////////////

struct T_PMOS_ITEM_D
{
	BOOL bCheck;
	int nChapterNo;
	int nItemType;	// enum T_PMOS_ITEM_D::eItemType
	BOOL bImage;
	BOOL bGraph;

	T_PMOS_ITEM_D() { Initialize(); }
	T_PMOS_ITEM_D(const T_PMOS_ITEM_D& src) { *this = src; }
	T_PMOS_ITEM_D& operator=(const T_PMOS_ITEM_D& src)
	{
		bCheck = src.bCheck;
		nChapterNo = src.nChapterNo;
		nItemType = src.nItemType;
		bImage = src.bImage;
		bGraph = src.bGraph;
		return *this;
	}

	void Initialize()
	{
		bCheck = FALSE;
		nChapterNo = 0;
		nItemType = 0;
		bImage = FALSE;
		bGraph = FALSE;
	}

	enum eItemType
	{
		kUnKnown,
		kSkeletonDiagram,
		kNode,
		kElement,
		kMaterial,
		kSection,
		kBoundaryCondition,
		kLoadCase,
		kStaticLoad,
		kLiveLoad,
		kConstructionStage,
		kNonLinear,
		kControlVibration,
		kTimeCareer,
		kSpectrum,
		kAnalysis,
		kHeatHydration,
	};
};

#define T_PMOS_K unsigned int
/**
* Pre Model Output Setting (모델정보 출력항목)
*/
struct T_PMOS_D
{
	CString strProjectName;
	int nStartPageNum;
	CArray<T_PMOS_ITEM_D, T_PMOS_ITEM_D&> ITEMS;

	T_PMOS_D() { Initialize(); }
	T_PMOS_D(const T_PMOS_D& src) { *this = src; }
	T_PMOS_D& operator=(const T_PMOS_D& src)
	{
		strProjectName = src.strProjectName;
		nStartPageNum = src.nStartPageNum;
		ITEMS.Copy(src.ITEMS);
		return *this;
	}

	void Initialize()
	{
		strProjectName.Empty();
		nStartPageNum = 0;
		ITEMS.RemoveAll();
	}
};

struct T_PMOS_D_CH
{
	char strProjectName[80];
	int nStartPageNum;
	CArray<T_PMOS_ITEM_D, T_PMOS_ITEM_D&> ITEMS;

	void Get(T_PMOS_D& rData)
	{
		ConvertCharStr(strProjectName, rData.strProjectName, sizeof(strProjectName));
		rData.nStartPageNum = nStartPageNum;
		rData.ITEMS.Copy(ITEMS);
	}
	void Set(T_PMOS_D& data)
	{
		ConvertStrChar(data.strProjectName, strProjectName, sizeof(strProjectName));
		nStartPageNum = data.nStartPageNum;
		ITEMS.Copy(data.ITEMS);
	}
};

struct T_PMOS_UDRD_D
{
	T_PMOS_K key;
	T_PMOS_D data;
};

struct T_PMOS_UDRD_D_CH
{
	T_PMOS_K key;
	T_PMOS_D_CH data;
};

struct T_PMOS_D_CH_RW
{
	T_PMOS_K key;
	char strProjectName[80];
	int nStartPageNum;
	int nItems;

	void Get(T_PMOS_K &rKey, T_PMOS_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.strProjectName, strProjectName, sizeof(strProjectName));
		rData.nStartPageNum = nStartPageNum;
	}
	void Set(const T_PMOS_K Key ,const T_PMOS_D_CH& data)
	{
		key = Key;
		memcpy(strProjectName, data.strProjectName, sizeof(strProjectName));
		nStartPageNum = data.nStartPageNum;
		nItems = (int)data.ITEMS.GetSize();
	}
};

struct T_PMOS_UNIT
{
	T_PMOS_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};

#define HASHSIZEPMOS 1001
#define HASHSIZELISTPMOS 11

#pragma pack(pop)

#endif  // __DB_ST_DT_PMOS_H__