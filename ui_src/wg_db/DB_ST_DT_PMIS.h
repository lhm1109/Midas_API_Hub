#ifndef __DB_ST_DT_PMIS_H__
#define __DB_ST_DT_PMIS_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#include <afxtempl.h>

// for Civil JP

//////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// 모델정보 이미지 출력의 상세설정 ////////////////////////////////////


#define T_PMIS_K unsigned int
#define T_PMOS_K unsigned int
/**
* Pre Model Image Setting (모델정보 이미지 출력의 상세설정)
*/
struct T_PMIS_D
{
	T_PMOS_K kPmos;
	CString strTitle;
	BOOL bUse;
	int nNo;
	int nKind;			// enum T_PMIS_D::eKindImage
	CString strGroup;
	int nViewPoint;		// enum T_PMIS_D::eViewPoint
	double dAngleHori;
	double dAngleVert;
	BOOL bNodeMark;
	BOOL bNodeNo;
	BOOL bElemNo;
	BOOL bLoadVal;
	BOOL bBoundary;
	BOOL bHidden;
	int nLayoutType;	// enum T_PMIS_D::eLayoutType

	T_PMIS_D() { Initialize(); }
	T_PMIS_D(const T_PMIS_D& src) { *this = src; }
	T_PMIS_D& operator=(const T_PMIS_D& src)
	{
		kPmos = src.kPmos;
		strTitle = src.strTitle;
		bUse = src.bUse;
		nNo = src.nNo;
		nKind = src.nKind;
		strGroup = src.strGroup;
		nViewPoint = src.nViewPoint;
		dAngleHori = src.dAngleHori;
		dAngleVert = src.dAngleVert;
		bNodeMark = src.bNodeMark;
		bNodeNo = src.bNodeNo;
		bElemNo = src.bElemNo;
		bLoadVal = src.bLoadVal;
		bBoundary = src.bBoundary;
		bHidden = src.bHidden;
		nLayoutType = src.nLayoutType;
		return *this;
	}

	void Initialize()
	{
		kPmos = 0;
		strTitle.Empty();
		bUse = FALSE;
		nNo = 0;
		nKind = T_PMIS_D::kUnKnownKind;
		strGroup.Empty();
		nViewPoint = T_PMIS_D::kUnKnown;
		dAngleHori = 0;
		dAngleVert = 0;
		bNodeMark = FALSE;
		bNodeNo = FALSE;
		bElemNo = FALSE;
		bLoadVal = FALSE;
		bBoundary = FALSE;
		bHidden = FALSE;
		nLayoutType = T_PMIS_D::kUnKnownLayout;
	}

	enum eKindImage
	{
		kUnKnownKind,
		kFrame,
		kLoad,
		kConstStage,
		kTotalKind,
	};

	enum eViewPoint
	{
		kUnKnown,
		kIsome,
		kTop,
		kFront,
		kRight,
		kLeft,
		kUser,
		kTotalView,
	};

	enum eLayoutType
	{
		kUnKnownLayout,
		kVertPageUpNone,
		kHoriPageUpNone,
		kVertPageUp2,
		kHoriPageUp2,
	};
};

struct T_PMIS_D_CH
{
	T_PMOS_K kPmos;
	char strTitle_[80];
	BOOL bUse;
	int nNo;
	int nKind;			
	char strGroup_[40];
	int nViewPoint;		
	double dAngleHori;
	double dAngleVert;
	BOOL bNodeMark;
	BOOL bNodeNo;
	BOOL bElemNo;
	BOOL bLoadVal;
	BOOL bBoundary;
	BOOL bHidden;
	int nLayoutType;	

	void Get(T_PMIS_D& rData)
	{
		rData.kPmos = kPmos;
		ConvertCharStr(strTitle_, rData.strTitle, sizeof(strTitle_));
		rData.bUse = bUse;
		rData.nNo = nNo;
		rData.nKind = nKind;
		ConvertCharStr(strGroup_, rData.strGroup, sizeof(strGroup_));
		rData.nViewPoint = nViewPoint;
		rData.dAngleHori = dAngleHori;
		rData.dAngleVert = dAngleVert;
		rData.bNodeMark = bNodeMark;
		rData.bNodeNo = bNodeNo;
		rData.bElemNo = bElemNo;
		rData.bLoadVal = bLoadVal;
		rData.bBoundary = bBoundary;
		rData.bHidden = bHidden;
		rData.nLayoutType = nLayoutType;
	}
	void Set(T_PMIS_D& data)
	{
		kPmos = data.kPmos;
		ConvertStrChar(data.strTitle, strTitle_, sizeof(strTitle_));
		bUse = data.bUse;
		nNo = data.nNo;
		nKind = data.nKind;
		ConvertStrChar(data.strGroup, strGroup_, sizeof(strGroup_));
		nViewPoint = data.nViewPoint;
		dAngleHori = data.dAngleHori;
		dAngleVert = data.dAngleVert;
		bNodeMark = data.bNodeMark;
		bNodeNo = data.bNodeNo;
		bElemNo = data.bElemNo;
		bLoadVal = data.bLoadVal;
		bBoundary = data.bBoundary;
		bHidden = data.bHidden;
		nLayoutType = data.nLayoutType;
	}
};

struct T_PMIS_UDRD_D
{
	T_PMIS_K key;
	T_PMIS_D data;
};

struct T_PMIS_UDRD_D_CH
{
	T_PMIS_K key;
	T_PMIS_D_CH data;
};

struct T_PMIS_D_CH_RW
{
	T_PMIS_K key;
	T_PMOS_K kPmos;
	char strTitle_[80];
	BOOL bUse;
	int nNo;
	int nKind;			
	char strGroup_[40];
	int nViewPoint;		
	double dAngleHori;
	double dAngleVert;
	BOOL bNodeMark;
	BOOL bNodeNo;
	BOOL bElemNo;
	BOOL bLoadVal;
	BOOL bBoundary;
	BOOL bHidden;
	int nLayoutType;	

	void Get(T_PMIS_K &rKey, T_PMIS_D_CH& rData)
	{
		rKey = key;
		rData.kPmos = kPmos;
		memcpy(rData.strTitle_, strTitle_, sizeof(strTitle_));
		rData.bUse = bUse;
		rData.nNo = nNo;
		rData.nKind = nKind;
		memcpy(rData.strGroup_, strGroup_, sizeof(strGroup_));
		rData.nViewPoint = nViewPoint;
		rData.dAngleHori = dAngleHori;
		rData.dAngleVert = dAngleVert;
		rData.bNodeMark = bNodeMark;
		rData.bNodeNo = bNodeNo;
		rData.bElemNo = bElemNo;
		rData.bLoadVal = bLoadVal;
		rData.bBoundary = bBoundary;
		rData.bHidden = bHidden;
		rData.nLayoutType = nLayoutType;
	}
	void Set(const T_PMIS_K Key , const T_PMIS_D_CH& data)
	{
		key = Key;
		kPmos = data.kPmos;
		memcpy(strTitle_, data.strTitle_, sizeof(strTitle_));
		bUse = data.bUse;
		nNo = data.nNo;
		nKind = data.nKind;
		memcpy(strGroup_, data.strGroup_, sizeof(strGroup_));
		nViewPoint = data.nViewPoint;
		dAngleHori = data.dAngleHori;
		dAngleVert = data.dAngleVert;
		bNodeMark = data.bNodeMark;
		bNodeNo = data.bNodeNo;
		bElemNo = data.bElemNo;
		bLoadVal = data.bLoadVal;
		bBoundary = data.bBoundary;
		bHidden = data.bHidden;
		nLayoutType = data.nLayoutType;
	}
};
struct T_PMIS_UNIT
{
	T_PMIS_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};

#define HASHSIZEPMIS 1001
#define HASHSIZELISTPMIS 11

#pragma pack(pop)

#endif  // __DB_ST_DT_PMIS_H__