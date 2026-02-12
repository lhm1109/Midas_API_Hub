#ifndef __DB_ST_DT_SIGR_H__
#define __DB_ST_DT_SIGR_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#include "DB_ST_DT_SEIS_ENUM.h"

// for Civil JP

#pragma region // T_SIGR_D : Seismic investigation Group (내진조사 대상)

#define T_SIGR_K unsigned int
struct T_SIGR_D
{
	CString strName;//내진조사 케이스명(중복허용X)
	int nSubType;	//그룹 종류 enum T_SIGR_D::eSubType

	T_SIGR_D() { Initialize(); }
	T_SIGR_D(const T_SIGR_D& src) { *this = src; }
	T_SIGR_D& operator=(const T_SIGR_D& src)
	{
		if (this != &src) {
			strName = src.strName;
			nSubType = src.nSubType;
		}
		return *this;
	}

	void Initialize()
	{
		strName.Empty();
		nSubType = 0;
	}

	enum eSubType
	{
		kUnKnown,
		kUpper,
		kLower,
		kUser,
	};
};

struct T_SIGR_D_CH
{
	char strName[40];
	int nSubType;

	void Get(T_SIGR_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nSubType = nSubType;
	}

	void Set(/*const*/ T_SIGR_D& data)
	{
		ConvertStrChar(data.strName, strName, sizeof(strName));
		nSubType = data.nSubType;
	}
};

struct T_SIGR_UDRD_D
{
	T_SIGR_K key;
	T_SIGR_D data;
};

struct T_SIGR_UDRD_D_CH
{
	T_SIGR_K key;
	T_SIGR_D_CH data;
};

struct T_SIGR_UDRD_D_CH_RW
{
	T_SIGR_K key;
	char strName[40];
	int nSubType;

	void Get(T_SIGR_K& rKey, T_SIGR_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.strName, strName, sizeof(strName));
		rData.nSubType = nSubType;
	}
	void Set(const T_SIGR_K Key, const T_SIGR_D_CH& data)
	{
		key = Key;
		memcpy(strName, data.strName, sizeof(strName));
		nSubType = data.nSubType;
	}
};

struct T_SIGR_UNIT
{
	T_SIGR_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};

#define HASHSIZESIGR 1001
#define HASHSIZELISTSIGR 11
#pragma endregion

#pragma region // T_SIGR_USER_D : 평가그룹 정의 (사용자 설정)

#define T_SIGR_USER_K unsigned int
struct T_SIGR_USER_D
{
	T_SIGR_K kSigr;									//내진조사 그룹 Key와 동일
	int nDirShear;									//교축방향 전단력 : enum seis_jp::eDirShear
	CArray<T_ELEM_K, T_ELEM_K> aInvestigatedElem;	//내진조사 대상 요소 Keys

	T_SIGR_USER_D() { Initialize(); }
	T_SIGR_USER_D(const T_SIGR_USER_D& src) { *this = src; }
	T_SIGR_USER_D& operator=(const T_SIGR_USER_D& src)
	{
		if (this != &src) {
			kSigr = src.kSigr;
			nDirShear = src.nDirShear;
			aInvestigatedElem.Copy(src.aInvestigatedElem);
		}
		return *this;
	}

	void Initialize()
	{
		kSigr = 0;
		nDirShear = seis_jp::kDirShear_0;
		aInvestigatedElem.RemoveAll();
	}
};

struct T_SIGR_USER_D_CH
{
	T_SIGR_K kSigr;
	int nDirShear;
	CArray<T_ELEM_K, T_ELEM_K> aInvestigatedElem;

	void Get(T_SIGR_USER_D& rData)
	{
		rData.kSigr = kSigr;
		rData.nDirShear = nDirShear;
		rData.aInvestigatedElem.Copy(aInvestigatedElem);
	}

	void Set(const T_SIGR_USER_D& data)
	{
		kSigr = data.kSigr;
		nDirShear = data.nDirShear;
		aInvestigatedElem.Copy(data.aInvestigatedElem);
	}
};

struct T_SIGR_USER_UDRD_D
{
	T_SIGR_USER_K key;
	T_SIGR_USER_D data;
};

struct T_SIGR_USER_UDRD_D_CH
{
	T_SIGR_USER_K key;
	T_SIGR_USER_D_CH data;
};

struct T_SIGR_USER_UDRD_D_CH_RW
{
	T_SIGR_USER_K key;
	T_SIGR_K kSigr;
	int nDirShear;
	int nCountElem;

	void Get(T_SIGR_USER_K& rKey, T_SIGR_USER_D_CH& rData)
	{
		rKey = key;
		rData.kSigr = kSigr;
		rData.nDirShear = nDirShear;
	}

	void Set(const T_SIGR_USER_K Key, const T_SIGR_USER_D_CH& data)
	{
		key = Key;
		kSigr = data.kSigr;
		nDirShear = data.nDirShear;
		nCountElem = (int)data.aInvestigatedElem.GetSize();
	}
};

struct T_SIGR_USER_UNIT
{
	T_SIGR_USER_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};

#define HASHSIZESIGR_USER 1001
#define HASHSIZELISTSIGR_USER 11
#pragma endregion

#pragma region // T_SIGR_UPPER_D : 평가그룹 정의 (상부구조)

#define T_SIGR_UPPER_K unsigned int
struct T_SIGR_UPPER_D
{
	T_SIGR_K kSigr;							//내진조사 그룹 Key와 동일
	T_NODE_K kNodeL;						//응답변위 산출위치(거더 좌단)
	T_NODE_K kNodeR;						//응답변위 산출위치(우단)
	int nDirShear;								//교축방향 전단력 : enum seis_jp::enDirShear
	CArray<T_ELEM_K, T_ELEM_K> aElements;	//내진조사 대상 요소 Keys
	
	T_SIGR_UPPER_D() { Initialize(); }
	T_SIGR_UPPER_D(const T_SIGR_UPPER_D& src) { *this = src; }
	T_SIGR_UPPER_D& operator=(const T_SIGR_UPPER_D& src)
	{
		if (this != &src) {
			kSigr = src.kSigr;
			kNodeL = src.kNodeL;
			kNodeR = src.kNodeR;
			nDirShear = src.nDirShear;
			aElements.Copy(src.aElements);
		}
		return *this;
	}

	void Initialize()
	{
		kSigr = 0;
		kNodeL = 0;
		kNodeR = 0;
		nDirShear = seis_jp::kDirShear_0;
		aElements.RemoveAll();
	}
};

struct T_SIGR_UPPER_D_CH
{
	T_SIGR_K kSigr;
	T_NODE_K kNodeL;
	T_NODE_K kNodeR;
	int nDirShear;
	CArray<T_ELEM_K, T_ELEM_K> aElements;

	void Get(T_SIGR_UPPER_D& rData)
	{
		rData.kSigr = kSigr;
		rData.kNodeL = kNodeL;
		rData.kNodeR = kNodeR;
		rData.nDirShear = nDirShear;
		rData.aElements.Copy(aElements);
	}

	void Set(const T_SIGR_UPPER_D& data)
	{
		kSigr = data.kSigr;
		kNodeL = data.kNodeL;
		kNodeR = data.kNodeR;
		nDirShear = data.nDirShear;
		aElements.Copy(data.aElements);
	}
};

struct T_SIGR_UPPER_UDRD_D
{
	T_SIGR_UPPER_K key;
	T_SIGR_UPPER_D data;
};

struct T_SIGR_UPPER_UDRD_D_CH
{
	T_SIGR_UPPER_K key;
	T_SIGR_UPPER_D_CH data;
};

struct T_SIGR_UPPER_UDRD_D_CH_RW
{
	T_SIGR_UPPER_K key;
	T_SIGR_K kSigr;
	T_NODE_K kNodeL;
	T_NODE_K kNodeR;
	int nDirShear;
	int nCountElem;

	void Get(T_SIGR_UPPER_K& rKey, T_SIGR_UPPER_D_CH& rData)
	{
		rKey = key;
		rData.kSigr = kSigr;
		rData.kNodeL = kNodeL;
		rData.kNodeR = kNodeR;
		rData.nDirShear = nDirShear;
	}
	void Set(const T_SIGR_UPPER_K Key, const T_SIGR_UPPER_D_CH& data)
	{
		key = Key;
		kSigr = data.kSigr;
		kNodeL = data.kNodeL;
		kNodeR = data.kNodeR;
		nDirShear = data.nDirShear;
		nCountElem = (int)data.aElements.GetSize();
	}
};

struct T_SIGR_UPPER_UNIT
{
	T_SIGR_UPPER_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};

#define HASHSIZESIGR_UPPER 1001
#define HASHSIZELISTSIGR_UPPER 11

#pragma endregion

#pragma region // T_SIGR_LOWER_BEAR_D : 평가그룹 정의 (하부구조) - 지승

struct T_SIGR_LOWER_BEAR_D
{
	CString strName;						//명칭
	int nDirShear;							//교축방향 단면력 : enum seis_jp::nDirShear
	int nLineNum;							//지승선 : enum T_SIGR_LOWER_BEAR_D::eBearingLineNum;
	int nType;								//지승 조건 : enum T_SIGR_LOWER_BEAR_D::eBearingType;
	CArray<T_NODE_K, T_NODE_K> aPosition;	//양단절점

	T_SIGR_LOWER_BEAR_D() { Initialize(); }
	T_SIGR_LOWER_BEAR_D(const T_SIGR_LOWER_BEAR_D& src) { *this = src; }
	T_SIGR_LOWER_BEAR_D& operator=(const T_SIGR_LOWER_BEAR_D& src)
	{
		if (this != &src) {
			strName = src.strName;
			nDirShear = src.nDirShear;
			nLineNum = src.nLineNum;
			nType = src.nType;
			aPosition.Copy(src.aPosition);
		}
		return *this;
	}

	void Initialize()
	{
		strName.Empty();
		nDirShear = seis_jp::kDirShear_0;
		nLineNum = 0;
		nType = 0;
		aPosition.RemoveAll();
	}

	enum eBearingLineNum
	{
		kLine0,
		kLine1,
		kLine2,
	};

	enum eBearingType
	{
		kUnKnown,
		kRigidLink,			//강체연결
		kFixed,				//고정지승
		kMovable,			//가동지승
		kElastic,			//탄성지승
		kSeismicIsolation,	//면진지승
	};
};

struct T_SIGR_LOWER_BEAR_D_CH
{
	char strName[60];
	int nDirShear;
	int nLineNum;
	int nType;
	CArray<T_NODE_K, T_NODE_K> aPosition;

	void Get(T_SIGR_LOWER_BEAR_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nDirShear = nDirShear;
		rData.nLineNum = nLineNum;
		rData.nType = nType;
		rData.aPosition.Copy(aPosition);
	}

	void Set(/*const*/ T_SIGR_LOWER_BEAR_D& data)
	{
		ConvertStrChar(data.strName, strName, sizeof(strName));
		nDirShear = data.nDirShear;
		nLineNum = data.nLineNum;
		nType = data.nType;
		aPosition.Copy(data.aPosition);
	}
};

struct T_SIGR_LOWER_BEAR_D_CH_RW
{
	char strName[60];
	int nDirShear;
	int nLineNum;
	int nType;
	int nCountNode;

	void Get(T_SIGR_LOWER_BEAR_D_CH& rData)
	{
		memcpy(rData.strName, strName, sizeof(strName));
		rData.nDirShear = nDirShear;
		rData.nLineNum = nLineNum;
		rData.nType = nType;
	}

	void Set(const T_SIGR_LOWER_BEAR_D_CH& data)
	{
		memcpy(strName, data.strName, sizeof(strName));
		nDirShear = data.nDirShear;
		nLineNum = data.nLineNum;
		nType = data.nType;
		nCountNode = (int)data.aPosition.GetSize();
	}
};

#pragma endregion

#pragma region // T_SIGR_LOWER_COLM_D : 평가그룹 정의 (하부구조) - 교각 기둥

struct T_SIGR_LOWER_COLM_D
{
	CString strName;								//명칭
	int nDirShear;									//교축방향 전단력 seis_jp::nDirShear
	BOOL bRebarCut;									//철근 단락부 계산서 출력 여부
	int nAxialLoc;									//교축방향 철근 단락부 위치 T_SIGR_LOWER_COLM_D::ePosType
	int nAxisPerpLoc;								//교축직각방향 철근 단락부 위치 T_SIGR_LOWER_COLM_D::ePosType
	T_ELEM_K kAxialElem;							//교축방향 철근 단락부 요소
	T_ELEM_K kAxisPerpElem;							//교축직각방향 철근 단락부 요소
	CArray<T_ELEM_K, T_ELEM_K> aInvestigatedElem;	//대상 요소 Keys

	T_SIGR_LOWER_COLM_D() { Initialize(); }
	T_SIGR_LOWER_COLM_D(const T_SIGR_LOWER_COLM_D& src) { *this = src; }
	T_SIGR_LOWER_COLM_D& operator=(const T_SIGR_LOWER_COLM_D& src)
	{
		if (this != &src) {
			strName = src.strName;
			nDirShear = src.nDirShear;
			bRebarCut = src.bRebarCut;
			nAxialLoc = src.nAxialLoc;
			nAxisPerpLoc = src.nAxisPerpLoc;
			kAxialElem = src.kAxialElem;
			kAxisPerpElem = src.kAxisPerpElem;
			aInvestigatedElem.Copy(src.aInvestigatedElem);
		}
		return *this;
	}

	void Initialize()
	{
		strName.Empty();
		nDirShear = seis_jp::kDirShear_0;
		bRebarCut = FALSE;
		nAxialLoc = 0;
		nAxisPerpLoc = 0;
		kAxialElem = 0;
		kAxisPerpElem = 0;
		aInvestigatedElem.RemoveAll();
	}

	enum ePosType
	{
		kUnKnown,
		kI,
		kJ,
	};
};

struct T_SIGR_LOWER_COLM_D_CH
{
	char strName[60];
	int nDirShear;
	BOOL bRebarCut;
	int nAxialLoc;
	int nAxisPerpLoc;
	T_ELEM_K kAxialElem;
	T_ELEM_K kAxisPerpElem;
	CArray<T_ELEM_K, T_ELEM_K> aInvestigatedElem;

	void Get(T_SIGR_LOWER_COLM_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nDirShear = nDirShear;
		rData.bRebarCut = bRebarCut;
		rData.nAxialLoc = nAxialLoc;
		rData.nAxisPerpLoc = nAxisPerpLoc;
		rData.kAxialElem = kAxialElem;
		rData.kAxisPerpElem = kAxisPerpElem;
		rData.aInvestigatedElem.Copy(aInvestigatedElem);
	}

	void Set(/*const*/ T_SIGR_LOWER_COLM_D& data)
	{
		ConvertStrChar(data.strName, strName, sizeof(strName));
		nDirShear = data.nDirShear;
		bRebarCut = data.bRebarCut;
		nAxialLoc = data.nAxialLoc;
		nAxisPerpLoc = data.nAxisPerpLoc;
		kAxialElem = data.kAxialElem;
		kAxisPerpElem = data.kAxisPerpElem;
		aInvestigatedElem.Copy(data.aInvestigatedElem);
	}
};

struct T_SIGR_LOWER_COLM_D_CH_RW
{
	char strName[60];
	int nDirShear;
	BOOL bRebarCut;
	int nAxialLoc;
	int nAxisPerpLoc;
	T_ELEM_K kAxialElem;
	T_ELEM_K kAxisPerpElem;
	int nCountElem;

	void Get(T_SIGR_LOWER_COLM_D_CH& rData)
	{
		memcpy(rData.strName, strName, sizeof(strName));
		rData.nDirShear = nDirShear;
		rData.bRebarCut = bRebarCut;
		rData.nAxialLoc = nAxialLoc;
		rData.nAxisPerpLoc = nAxisPerpLoc;
		rData.kAxialElem = kAxialElem;
		rData.kAxisPerpElem = kAxisPerpElem;
	}

	void Set(const T_SIGR_LOWER_COLM_D_CH& data)
	{
		memcpy(strName, data.strName, sizeof(strName));
		nDirShear = data.nDirShear;
		bRebarCut = data.bRebarCut;
		nAxialLoc = data.nAxialLoc;
		nAxisPerpLoc = data.nAxisPerpLoc;
		kAxialElem = data.kAxialElem;
		kAxisPerpElem = data.kAxisPerpElem;
		nCountElem = (int)data.aInvestigatedElem.GetSize();
	}
};

#pragma endregion

#pragma region // T_SIGR_LOWER_BEAM_D : 평가그룹 정의 (하부구조) - 교각 보

struct T_SIGR_LOWER_BEAM_D
{
	CString strName;								//명칭
	int nDirShear;									//교축방향 전단력 : enum seis_jp::nDirShear
	CArray<T_ELEM_K, T_ELEM_K> aInvestigatedElem;	//대상 요소 Keys

	T_SIGR_LOWER_BEAM_D() { Initialize(); }
	T_SIGR_LOWER_BEAM_D(const T_SIGR_LOWER_BEAM_D& src) { *this = src; }
	T_SIGR_LOWER_BEAM_D& operator=(const T_SIGR_LOWER_BEAM_D& src)
	{
		if (this != &src) {
			strName = src.strName;
			nDirShear = src.nDirShear;
			aInvestigatedElem.Copy(src.aInvestigatedElem);
		}
		return *this;
	}

	void Initialize()
	{
		strName.Empty();
		nDirShear = seis_jp::kDirShear_0;
		aInvestigatedElem.RemoveAll();
	}
};

struct T_SIGR_LOWER_BEAM_D_CH
{
	char strName[60];
	int nDirShear;
	CArray<T_NODE_K, T_NODE_K> aInvestigatedElem;

	void Get(T_SIGR_LOWER_BEAM_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nDirShear = nDirShear;
		rData.aInvestigatedElem.Copy(aInvestigatedElem);
	}

	void Set(/*const*/ T_SIGR_LOWER_BEAM_D& data)
	{
		ConvertStrChar(data.strName, strName, sizeof(strName));
		nDirShear = data.nDirShear;
		aInvestigatedElem.Copy(data.aInvestigatedElem);
	}
};

struct T_SIGR_LOWER_BEAM_D_CH_RW
{
	char strName[60];
	int nDirShear;
	int nCountElem;

	void Get(T_SIGR_LOWER_BEAM_D_CH& rData)
	{
		memcpy(rData.strName, strName, sizeof(strName));
		rData.nDirShear = nDirShear;
	}

	void Set(const T_SIGR_LOWER_BEAM_D_CH& data)
	{
		memcpy(strName, data.strName, sizeof(strName));
		nDirShear = data.nDirShear;
		nCountElem = (int)data.aInvestigatedElem.GetSize();
	}
};

#pragma endregion

#pragma region // T_SIGR_LOWER_FOOT_D : 평가그룹 정의 (하부구조) - 교각 기초

struct T_SIGR_LOWER_FOOT_D
{
	CString strName;								//명칭
	int nDirShear;									//교축방향 전단력 : enum seis_jp::nDirShear
	CArray<T_ELEM_K, T_ELEM_K> aInvestigatedElem;	//대상 요소 Keys

	T_SIGR_LOWER_FOOT_D() { Initialize(); }
	T_SIGR_LOWER_FOOT_D(const T_SIGR_LOWER_FOOT_D& src) { *this = src; }
	T_SIGR_LOWER_FOOT_D& operator=(const T_SIGR_LOWER_FOOT_D& src)
	{
		if (this != &src) {
			strName = src.strName;
			nDirShear = src.nDirShear;
			aInvestigatedElem.Copy(src.aInvestigatedElem);
		}
		return *this;
	}

	void Initialize()
	{
		strName.Empty();
		nDirShear = seis_jp::kDirShear_0;
		aInvestigatedElem.RemoveAll();
	}
};

struct T_SIGR_LOWER_FOOT_D_CH
{
	char strName[60];
	int nDirShear;
	CArray<T_NODE_K, T_NODE_K> aInvestigatedElem;

	void Get(T_SIGR_LOWER_FOOT_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nDirShear = nDirShear;
		rData.aInvestigatedElem.Copy(aInvestigatedElem);
	}

	void Set(/*const*/ T_SIGR_LOWER_FOOT_D& data)
	{
		ConvertStrChar(data.strName, strName, sizeof(strName));
		nDirShear = data.nDirShear;
		aInvestigatedElem.Copy(data.aInvestigatedElem);
	}
};

struct T_SIGR_LOWER_FOOT_D_CH_RW
{
	char strName[60];
	int nDirShear;
	int nCountElem;

	void Get(T_SIGR_LOWER_FOOT_D_CH& rData)
	{
		memcpy(rData.strName, strName, sizeof(strName));
		rData.nDirShear = nDirShear;
	}

	void Set(const T_SIGR_LOWER_FOOT_D_CH& data)
	{
		memcpy(strName, data.strName, sizeof(strName));
		nDirShear = data.nDirShear;
		nCountElem = (int)data.aInvestigatedElem.GetSize();
	}
};

#pragma endregion

#pragma region // T_SIGR_LOWER_D : 평가그룹 정의 (하부구조)

#define T_SIGR_LOWER_K unsigned int
struct T_SIGR_LOWER_D
{
	T_SIGR_K kSigr;							//내진조사 그룹 Key와 동일
	T_NODE_K kNodeAxis;						//상부공 관성력 작용 위치(교축방향)
	T_NODE_K kNodePerp;						//상부공 관성력 작용 위치(교축직각방향)
	int nMaterialType;						//교각의 재료 타입 : enum T_SIGR_LOWER_D::eMaterialType

	CArray<T_SIGR_LOWER_BEAR_D, T_SIGR_LOWER_BEAR_D&> BEAR_D;	// (하부구조) - 지승
	CArray<T_SIGR_LOWER_COLM_D, T_SIGR_LOWER_COLM_D&> COLM_D;	// (하부구조) - 교각 기둥
	CArray<T_SIGR_LOWER_BEAM_D, T_SIGR_LOWER_BEAM_D&> BEAM_D;	// (하부구조) - 교각보
	CArray<T_SIGR_LOWER_FOOT_D, T_SIGR_LOWER_FOOT_D&> FOOT_D;	// (하부구조) - 기초

	T_SIGR_LOWER_D() { Initialize(); }
	T_SIGR_LOWER_D(const T_SIGR_LOWER_D& src) { *this = src; }
	T_SIGR_LOWER_D& operator=(const T_SIGR_LOWER_D& src)
	{
		if (this != &src) {
			kSigr = src.kSigr;
			kNodeAxis = src.kNodeAxis;
			kNodePerp = src.kNodePerp;
			nMaterialType = src.nMaterialType;

			BEAR_D.Copy(src.BEAR_D);
			COLM_D.Copy(src.COLM_D);
			BEAM_D.Copy(src.BEAM_D);
			FOOT_D.Copy(src.FOOT_D);
		}
		return *this;
	}

	void Initialize()
	{
		kSigr = 0;
		kNodeAxis = 0;
		kNodePerp = 0;
		nMaterialType = 0;

		BEAR_D.RemoveAll();
		COLM_D.RemoveAll();
		BEAM_D.RemoveAll();
		FOOT_D.RemoveAll();
	}

	CString GetBearGroupText() const
	{
		CString str;
		for (int i = 0; i < BEAR_D.GetCount(); i++)
		{
			if (i > 0)
				str = str + _T(", ");
			str = str + BEAR_D[i].strName;
		}
		return str;
	}
	CString GetColmGroupText() const
	{
		CString str;
		for (int i = 0; i < COLM_D.GetCount(); i++)
		{
			if (i > 0)
				str = str + _T(", ");
			str = str + COLM_D[i].strName;
		}
		return str;
	}
	CString GetBeamGroupText() const
	{
		CString str;
		for (int i = 0; i < BEAM_D.GetCount(); i++)
		{
			if (i > 0)
				str = str + _T(", ");
			str = str + BEAM_D[i].strName;
		}
		return str;
	}
	CString GetFootGroupText() const
	{
		CString str;
		for (int i = 0; i < FOOT_D.GetCount(); i++)
		{
			if (i > 0)
				str = str + _T(", ");
			str = str + FOOT_D[i].strName;
		}
		return str;
	}
	
	enum eMaterialType
	{
		kUnKnown,
		kRC,
		kSteel,
		kSRC,
	};
};

struct T_SIGR_LOWER_D_CH
{
	T_SIGR_K kSigr;
	T_NODE_K kNodeAxis;
	T_NODE_K kNodePerp;
	int nMaterialType;

	CArray<T_SIGR_LOWER_BEAR_D_CH, T_SIGR_LOWER_BEAR_D_CH&> BEAR_D;
	CArray<T_SIGR_LOWER_COLM_D_CH, T_SIGR_LOWER_COLM_D_CH&> COLM_D;
	CArray<T_SIGR_LOWER_BEAM_D_CH, T_SIGR_LOWER_BEAM_D_CH&> BEAM_D;
	CArray<T_SIGR_LOWER_FOOT_D_CH, T_SIGR_LOWER_FOOT_D_CH&> FOOT_D;

	void Get(T_SIGR_LOWER_D& rData)
	{
		rData.kSigr = kSigr;
		rData.kNodeAxis = kNodeAxis;
		rData.kNodePerp = kNodePerp;
		rData.nMaterialType = nMaterialType;

		int nCount = 0;

		nCount = (int)BEAR_D.GetSize();
		rData.BEAR_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			BEAR_D[i].Get(rData.BEAR_D[i]);
		}

		nCount = (int)COLM_D.GetSize();
		rData.COLM_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			COLM_D[i].Get(rData.COLM_D[i]);
		}

		nCount = (int)BEAM_D.GetSize();
		rData.BEAM_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			BEAM_D[i].Get(rData.BEAM_D[i]);
		}
		
		nCount = (int)FOOT_D.GetSize();
		rData.FOOT_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			FOOT_D[i].Get(rData.FOOT_D[i]);
		}
	}

	void Set(/*const*/ T_SIGR_LOWER_D& data)
	{
		kSigr = data.kSigr;
		kNodeAxis = data.kNodeAxis;
		kNodePerp = data.kNodePerp;
		nMaterialType = data.nMaterialType;
		
		int nCount = 0;

		nCount = (int)data.BEAR_D.GetSize();
		BEAR_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			BEAR_D[i].Set(data.BEAR_D[i]);
		}

		nCount = (int)data.COLM_D.GetSize();
		COLM_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			COLM_D[i].Set(data.COLM_D[i]);
		}

		nCount = (int)data.BEAM_D.GetSize();
		BEAM_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			BEAM_D[i].Set(data.BEAM_D[i]);
		}

		nCount = (int)data.FOOT_D.GetSize();
		FOOT_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			FOOT_D[i].Set(data.FOOT_D[i]);
		}
	}
};

struct T_SIGR_LOWER_D_CH_RW
{
	CArray<T_SIGR_LOWER_BEAR_D_CH_RW, T_SIGR_LOWER_BEAR_D_CH_RW&> BEAR_D;
	CArray<T_SIGR_LOWER_COLM_D_CH_RW, T_SIGR_LOWER_COLM_D_CH_RW&> COLM_D;
	CArray<T_SIGR_LOWER_BEAM_D_CH_RW, T_SIGR_LOWER_BEAM_D_CH_RW&> BEAM_D;
	CArray<T_SIGR_LOWER_FOOT_D_CH_RW, T_SIGR_LOWER_FOOT_D_CH_RW&> FOOT_D;

	void Get(T_SIGR_LOWER_D_CH& rData)
	{
		int nCount = 0;

		nCount = (int)BEAR_D.GetSize();
		rData.BEAR_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			BEAR_D[i].Get(rData.BEAR_D[i]);
		}

		nCount = (int)COLM_D.GetSize();
		rData.COLM_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			COLM_D[i].Get(rData.COLM_D[i]);
		}

		nCount = (int)BEAM_D.GetSize();
		rData.BEAM_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			BEAM_D[i].Get(rData.BEAM_D[i]);
		}

		nCount = (int)FOOT_D.GetSize();
		rData.FOOT_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			FOOT_D[i].Get(rData.FOOT_D[i]);
		}
	}

	void Set(/*const*/ T_SIGR_LOWER_D_CH& data)
	{
		int nCount = 0;

		nCount = (int)data.BEAR_D.GetSize();
		BEAR_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			BEAR_D[i].Set(data.BEAR_D[i]);
		}

		nCount = (int)data.COLM_D.GetSize();
		COLM_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			COLM_D[i].Set(data.COLM_D[i]);
		}

		nCount = (int)data.BEAM_D.GetSize();
		BEAM_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			BEAM_D[i].Set(data.BEAM_D[i]);
		}

		nCount = (int)data.FOOT_D.GetSize();
		FOOT_D.SetSize(nCount);
		for (int i = 0; i < nCount; i++)
		{
			FOOT_D[i].Set(data.FOOT_D[i]);
		}
	}
};

struct T_SIGR_LOWER_UDRD_D
{
	T_SIGR_LOWER_K key;
	T_SIGR_LOWER_D data;
};

struct T_SIGR_LOWER_UDRD_D_CH
{
	T_SIGR_LOWER_K key;
	T_SIGR_LOWER_D_CH data;
};

struct T_SIGR_LOWER_UDRD_D_CH_RW
{
	T_SIGR_LOWER_K key;
	T_SIGR_K kSigr;
	T_NODE_K kNodeAxis;
	T_NODE_K kNodePerp;
	int nMaterialType;
	
	int nCountBEAR;
	int nCountCOLM;
	int nCountBEAM;
	int nCountFOOT;

	void Get(T_SIGR_LOWER_K& rKey, T_SIGR_LOWER_D_CH& rData)
	{
		rKey = key;
		rData.kSigr = kSigr;
		rData.kNodeAxis = kNodeAxis;
		rData.kNodePerp = kNodePerp;
		rData.nMaterialType = nMaterialType;
	}

	void Set(const T_SIGR_LOWER_K Key, const T_SIGR_LOWER_D_CH& data)
	{
		key = Key;
		kSigr = data.kSigr;
		kNodeAxis = data.kNodeAxis;
		kNodePerp = data.kNodePerp;
		nMaterialType = data.nMaterialType;

		nCountBEAR = (int)data.BEAR_D.GetSize();
		nCountCOLM = (int)data.COLM_D.GetSize();
		nCountBEAM = (int)data.BEAM_D.GetSize();
		nCountFOOT = (int)data.FOOT_D.GetSize();
	}
};

struct T_SIGR_LOWER_UNIT
{
	T_SIGR_LOWER_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};

#define HASHSIZESIGR_LOWER 1001
#define HASHSIZELISTSIGR_LOWER 11

#pragma endregion

#pragma pack(pop)

#endif  // __DB_ST_DT_SIGR_H__