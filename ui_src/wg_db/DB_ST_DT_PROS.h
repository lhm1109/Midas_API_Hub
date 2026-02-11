#ifndef __DB_ST_DT_PROS_H__
#define __DB_ST_DT_PROS_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#include <afxtempl.h>

// for Civil JP

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// 결과정보 출력항목 ///////////////////////////////////////////

//고유치 그래프 감쇠 정수 결정
struct T_GRAPH_EIGV_D
{
	enum eDir
	{
		kX = 0,
		kY,
		kDirMax,
	};

	BOOL bMass;				// 질량비례형
	BOOL bStiffness;		// 강성비례형
	int nMode1[kDirMax];	// 1차모드
	int nMode2[kDirMax];	// 2차모드

	T_GRAPH_EIGV_D() { Initialize(); }
	T_GRAPH_EIGV_D(const T_GRAPH_EIGV_D& src) { *this = src; }
	T_GRAPH_EIGV_D& operator=(const T_GRAPH_EIGV_D& src)
	{
		bMass = src.bMass;
		bStiffness = src.bStiffness;
		nMode1[kX] = src.nMode1[kX];
		nMode1[kY] = src.nMode1[kY];
		nMode2[kX] = src.nMode2[kX];
		nMode2[kY] = src.nMode2[kY];
		return *this;
	}


	void Initialize()
	{
		bMass = FALSE;
		bStiffness = FALSE;
		nMode1[kX] = 0;
		nMode1[kY] = 0;
		nMode2[kX] = 0;
		nMode2[kY] = 0;
	}
};

//시간이력해석 스마트 그래프
struct T_GRAPH_TIME_D
{
	CArray<T_THRG_K, T_THRG_K&> thrgKeys;
	int nLayoutType;	// enum T_GRAPH_STMART_D::eLayoutType

	T_GRAPH_TIME_D() { Initialize(); }
	T_GRAPH_TIME_D(const T_GRAPH_TIME_D& src) { *this = src; }
	T_GRAPH_TIME_D& operator=(const T_GRAPH_TIME_D& src)
	{
		thrgKeys.Copy(src.thrgKeys);
		nLayoutType = src.nLayoutType;
		return *this;
	}

	enum eLayoutType
	{
		kVertPageUp2 = 1,
		kHoriPageUp2,
	};

	void Initialize()
	{
		thrgKeys.RemoveAll();
		nLayoutType = 0;
	}

};

struct T_GRAPH_TIME_D_RW
{
	int nThrgKeys;  // count of aFunctionName
	int nLayoutType;	// enum T_GRAPH_STMART_D::eLayoutType

	void Get(T_GRAPH_TIME_D& rData)
	{
		rData.nLayoutType = nLayoutType;
	}
	void Set(const T_GRAPH_TIME_D& Data)
	{
		nThrgKeys = (int)Data.thrgKeys.GetSize();
		nLayoutType = Data.nLayoutType;
	}
};

struct T_PROS_ITEM_D
{
	int nSubTitleNo;  // Item Group 확인용 no
	BOOL bCheck;
	int nChapterNo;
	int nAnalysisType;	// enum T_PROS_ITEM_D::eAnalysisType
	int nOutputType;	// enum T_PROS_ITEM_D::eOutputType
	int nResultType;	// enum T_PROS_ITEM_D::eResultType
	int nCoordinateSys;	// 단명력/응력도의 방향 enum T_PROS_ITEM_D::eCoordinateSys
	BOOL bImage;
	BOOL bGraph;
	T_GRAPH_TIME_D GraphTime;	// 시간이력해석 스마트 그래프
	T_GRAPH_EIGV_D GraphEigv;	// 고유치 그래프 감쇠 정수 결정
	CArray<T_PRIS_K, T_PRIS_K> aPrisK;	// 출력 이미지 항목 키

	T_PROS_ITEM_D() { Initialize(); }
	T_PROS_ITEM_D(const T_PROS_ITEM_D& src) { *this = src; }
	T_PROS_ITEM_D& operator=(const T_PROS_ITEM_D& src)
	{
		nSubTitleNo = src.nSubTitleNo;
		bCheck = src.bCheck;
		nChapterNo = src.nChapterNo;
		nAnalysisType = src.nAnalysisType;
		nOutputType = src.nOutputType;
		nResultType = src.nResultType;
		nCoordinateSys = src.nCoordinateSys;
		bImage = src.bImage;
		bGraph = src.bGraph;
		GraphTime = src.GraphTime;
		GraphEigv = src.GraphEigv;
		aPrisK.Copy(src.aPrisK);
		return *this;
	}

	void Initialize()
	{
		nSubTitleNo = 0;
		bCheck = FALSE;
		nChapterNo = 0;
		nAnalysisType = 0;
		nOutputType = 0;
		nResultType = 0;
		nCoordinateSys = 0;
		bImage = FALSE;
		bGraph = FALSE;
		GraphTime.Initialize();
		GraphEigv.Initialize();
		aPrisK.RemoveAll();
	}

	enum eAnalysisType
	{
		kStatic = 1,	// 일반정적
		kDynamic,		//동적
		kConsStep,		//시공단계
	};

	enum eOutputType
	{
		kTable = 1,		// 일람표
		kImage,			// 이미지
		kGraph,			// 그래프
	};

	enum eResultType
	{
		kReaction = 1,		// 반력
		kDisplacement,	// 변위
		kDisplaceSpeedAccel, // 변위/속도/가속도
		kSectionForce,	// 단면력
		k1D_RespForce,		// 1D 응답 단면력
		kInelasticHinge,// 비선형 특성
		kStress,		// 1D응력도
		kRink,			// 링크
		kRinkRespResult,	// 링크응답결과
		kEigenvalue,		// 고유치
		kBuckling,		// 좌굴
		kFiber,				// 파이버
		kPC_Steel,			// PC강재 변형
		kSpeed, // 속도
		kAccel, // 가속도
	};

	enum eCoordinateSys
	{
		kElement = 1,	// 요소좌표계	
		kGlobal,		// 전체좌표계
	};
};

struct T_PROS_ITEM_D_RW
{
	int nSubTitleNo;
	BOOL bCheck;
	int nChapterNo;
	int nAnalysisType;	// enum T_PROS_ITEM_D::eAnalysisType
	int nOutputType;	// enum T_PROS_ITEM_D::eOutputType
	int nResultType;	// enum T_PROS_ITEM_D::eResultType
	int nCoordinateSys;	// 단명력/응력도의 방향 enum T_PROS_ITEM_D::eCoordinateSys
	BOOL bImage;
	BOOL bGraph;
	T_GRAPH_TIME_D_RW GraphTime;	// 시간이력해석 스마트 그래프
	T_GRAPH_EIGV_D GraphEigv;	// 고유치 그래프 감쇠 정수 결정
	int nPrisK;	// 출력 이미지 항목 키 개수

	void Get(T_PROS_ITEM_D &rData)
	{
		rData.nSubTitleNo = nSubTitleNo;
		rData.bCheck = bCheck;
		rData.nChapterNo = nChapterNo;
		rData.nAnalysisType = nAnalysisType;
		rData.nOutputType = nOutputType;
		rData.nResultType = nResultType;
		rData.nCoordinateSys = nCoordinateSys;
		rData.bImage = bImage;
		rData.bGraph = bGraph;
		GraphTime.Get(rData.GraphTime);
		rData.GraphEigv = GraphEigv;
	}
	void Set(const T_PROS_ITEM_D &Data)
	{
		nSubTitleNo = Data.nSubTitleNo;
		bCheck = Data.bCheck;
		nChapterNo = Data.nChapterNo;
		nAnalysisType = Data.nAnalysisType;
		nOutputType = Data.nOutputType;
		nResultType = Data.nResultType;
		nCoordinateSys = Data.nCoordinateSys;
		bImage = Data.bImage;
		bGraph = Data.bGraph;
		GraphTime.Set(Data.GraphTime);
		GraphEigv = Data.GraphEigv;
		nPrisK = (int)Data.aPrisK.GetSize();
	}
};

#define T_PROS_K unsigned int
/**
* Post Result Output Setting (결과정보 출력항목)
*/
struct T_PROS_D
{
	CString strProjectName;
	int nPageNumStart;
	CArray<T_PROS_ITEM_D, T_PROS_ITEM_D&> aItem_Static_Table;
	CArray<T_PROS_ITEM_D, T_PROS_ITEM_D&> aItem_Static_Image;
	CArray<T_PROS_ITEM_D, T_PROS_ITEM_D&> aItem_Static_Graph;
	CArray<T_PROS_ITEM_D, T_PROS_ITEM_D&> aItem_Dynamic_Table;
	CArray<T_PROS_ITEM_D, T_PROS_ITEM_D&> aItem_Dynamic_Image;
	CArray<T_PROS_ITEM_D, T_PROS_ITEM_D&> aItem_Dynamic_Graph;
	CArray<T_PROS_ITEM_D, T_PROS_ITEM_D&> aItem_ConsStep_Table;
	CArray<T_PROS_ITEM_D, T_PROS_ITEM_D&> aItem_ConsStep_Image;
	CArray<T_PROS_ITEM_D, T_PROS_ITEM_D&> aItem_ConsStep_Graph;

	T_PROS_D() { Initialize(); }
	T_PROS_D(const T_PROS_D& src) { *this = src; }
	T_PROS_D& operator=(const T_PROS_D& src)
	{
		strProjectName = src.strProjectName;
		nPageNumStart = src.nPageNumStart;
		aItem_Static_Table.Copy(src.aItem_Static_Table);
		aItem_Static_Image.Copy(src.aItem_Static_Image);
		aItem_Static_Graph.Copy(src.aItem_Static_Graph);
		aItem_Dynamic_Table.Copy(src.aItem_Dynamic_Table);
		aItem_Dynamic_Image.Copy(src.aItem_Dynamic_Image);
		aItem_Dynamic_Graph.Copy(src.aItem_Dynamic_Graph);
		aItem_ConsStep_Table.Copy(src.aItem_ConsStep_Table);
		aItem_ConsStep_Image.Copy(src.aItem_ConsStep_Image);
		aItem_ConsStep_Graph.Copy(src.aItem_ConsStep_Graph);
		
		return *this;
	}

	void Initialize()
	{
		strProjectName.Empty();
		nPageNumStart = 0;
		aItem_Static_Table.RemoveAll();
		aItem_Static_Image.RemoveAll();
		aItem_Static_Graph.RemoveAll();
		aItem_Dynamic_Table.RemoveAll();
		aItem_Dynamic_Image.RemoveAll();
		aItem_Dynamic_Graph.RemoveAll();
		aItem_ConsStep_Table.RemoveAll();
		aItem_ConsStep_Image.RemoveAll();
		aItem_ConsStep_Graph.RemoveAll();
	}
};

struct T_PROS_D_CH
{
	char strProjectName[80];
	int nPageNumStart;
	CArray<T_PROS_ITEM_D_RW, T_PROS_ITEM_D_RW&> aItem_Static_Table;
	CArray<T_PROS_ITEM_D_RW, T_PROS_ITEM_D_RW&> aItem_Static_Image;
	CArray<T_PROS_ITEM_D_RW, T_PROS_ITEM_D_RW&> aItem_Static_Graph;
	CArray<T_PROS_ITEM_D_RW, T_PROS_ITEM_D_RW&> aItem_Dynamic_Table;
	CArray<T_PROS_ITEM_D_RW, T_PROS_ITEM_D_RW&> aItem_Dynamic_Image;
	CArray<T_PROS_ITEM_D_RW, T_PROS_ITEM_D_RW&> aItem_Dynamic_Graph;
	CArray<T_PROS_ITEM_D_RW, T_PROS_ITEM_D_RW&> aItem_ConsStep_Table;
	CArray<T_PROS_ITEM_D_RW, T_PROS_ITEM_D_RW&> aItem_ConsStep_Image;
	CArray<T_PROS_ITEM_D_RW, T_PROS_ITEM_D_RW&> aItem_ConsStep_Graph;

	void Get(T_PROS_D& rData)
	{
		ConvertCharStr(strProjectName, rData.strProjectName, sizeof(strProjectName));
		rData.nPageNumStart = nPageNumStart;
		
		int nCount = 0;
		nCount = (int)aItem_Static_Table.GetSize();
		rData.aItem_Static_Table.SetSize(nCount);
		for (int i = 0; i < nCount; i++) 
			aItem_Static_Table[i].Get(rData.aItem_Static_Table[i]);

		nCount = (int)aItem_Static_Image.GetSize();
		rData.aItem_Static_Image.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_Static_Image[i].Get(rData.aItem_Static_Image[i]);

		nCount = (int)aItem_Static_Graph.GetSize();
		rData.aItem_Static_Graph.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_Static_Graph[i].Get(rData.aItem_Static_Graph[i]);

		nCount = (int)aItem_Dynamic_Table.GetSize();
		rData.aItem_Dynamic_Table.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_Dynamic_Table[i].Get(rData.aItem_Dynamic_Table[i]);

		nCount = (int)aItem_Dynamic_Image.GetSize();
		rData.aItem_Dynamic_Image.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_Dynamic_Image[i].Get(rData.aItem_Dynamic_Image[i]);

		nCount = (int)aItem_Dynamic_Graph.GetSize();
		rData.aItem_Dynamic_Graph.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_Dynamic_Graph[i].Get(rData.aItem_Dynamic_Graph[i]);

		nCount = (int)aItem_ConsStep_Table.GetSize();
		rData.aItem_ConsStep_Table.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_ConsStep_Table[i].Get(rData.aItem_ConsStep_Table[i]);

		nCount = (int)aItem_ConsStep_Image.GetSize();
		rData.aItem_ConsStep_Image.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_ConsStep_Image[i].Get(rData.aItem_ConsStep_Image[i]);

		nCount = (int)aItem_ConsStep_Graph.GetSize();
		rData.aItem_ConsStep_Graph.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_ConsStep_Graph[i].Get(rData.aItem_ConsStep_Graph[i]);
	}

	void Set(/*const*/ T_PROS_D& data)
	{
		ConvertStrChar(data.strProjectName, strProjectName, sizeof(strProjectName));
		nPageNumStart = data.nPageNumStart;

		int nCount = 0;
		nCount = (int)data.aItem_Static_Table.GetSize();
		aItem_Static_Table.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_Static_Table[i].Set(data.aItem_Static_Table[i]);

		nCount = (int)data.aItem_Static_Image.GetSize();
		aItem_Static_Image.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_Static_Image[i].Set(data.aItem_Static_Image[i]);

		nCount = (int)data.aItem_Static_Graph.GetSize();
		aItem_Static_Graph.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_Static_Graph[i].Set(data.aItem_Static_Graph[i]);

		nCount = (int)data.aItem_Dynamic_Table.GetSize();
		aItem_Dynamic_Table.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_Dynamic_Table[i].Set(data.aItem_Dynamic_Table[i]);

		nCount = (int)data.aItem_Dynamic_Image.GetSize();
		aItem_Dynamic_Image.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_Dynamic_Image[i].Set(data.aItem_Dynamic_Image[i]);

		nCount = (int)data.aItem_Dynamic_Graph.GetSize();
		aItem_Dynamic_Graph.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_Dynamic_Graph[i].Set(data.aItem_Dynamic_Graph[i]);

		nCount = (int)data.aItem_ConsStep_Table.GetSize();
		aItem_ConsStep_Table.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_ConsStep_Table[i].Set(data.aItem_ConsStep_Table[i]);

		nCount = (int)data.aItem_ConsStep_Image.GetSize();
		aItem_ConsStep_Image.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_ConsStep_Image[i].Set(data.aItem_ConsStep_Image[i]);

		nCount = (int)data.aItem_ConsStep_Graph.GetSize();
		aItem_ConsStep_Graph.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
			aItem_ConsStep_Graph[i].Set(data.aItem_ConsStep_Graph[i]);
	}
};

struct T_PROS_UDRD_D
{
	T_PROS_K key;
	T_PROS_D data;
};

struct T_PROS_UDRD_D_CH
{
	T_PROS_K key;
	T_PROS_D_CH data;
};

struct T_PROS_D_CH_RW
{
	T_PROS_K key;
	char strProjectName[80];
	int nPageNumStart;
	int nStatic_Table;
	int nStatic_Image;
	int nStatic_Graph;
	int nDynamic_Table;
	int nDynamic_Image;
	int nDynamic_Graph;
	int nConsStep_Table;
	int nConsStep_Image;
	int nConsStep_Graph;
	
	void Get(T_PROS_K& rKey, T_PROS_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.strProjectName, strProjectName, sizeof(strProjectName));
		rData.nPageNumStart = nPageNumStart;
	}

	void Set(const T_PROS_K Key, const T_PROS_D_CH& data)
	{
		key = Key;
		memcpy(strProjectName, data.strProjectName, sizeof(strProjectName));
		nPageNumStart = data.nPageNumStart;
		nStatic_Table = (int)data.aItem_Static_Table.GetSize();
		nStatic_Image = (int)data.aItem_Static_Image.GetSize();
		nStatic_Graph = (int)data.aItem_Static_Graph.GetSize();
		nDynamic_Table = (int)data.aItem_Dynamic_Table.GetSize();
		nDynamic_Image = (int)data.aItem_Dynamic_Image.GetSize();
		nDynamic_Graph = (int)data.aItem_Dynamic_Graph.GetSize();
		nConsStep_Table = (int)data.aItem_ConsStep_Table.GetSize();
		nConsStep_Image = (int)data.aItem_ConsStep_Image.GetSize();
		nConsStep_Graph = (int)data.aItem_ConsStep_Graph.GetSize();
	}
};

struct T_PROS_UNIT
{
	T_PROS_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};

#define HASHSIZEPROS 1001
#define HASHSIZELISTPROS 11

#pragma pack(pop)

#endif  // __DB_ST_DT_PROS_H__