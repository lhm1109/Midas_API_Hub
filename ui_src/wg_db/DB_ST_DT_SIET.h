#ifndef __DB_ST_DT_SIET_H__
#define __DB_ST_DT_SIET_H__

#define MIDAS_SDK

#pragma pack(push, 1)

// for Civil JP

//////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// 조사설정 - 기타 ////////////////////////////////////////////

/**
* Seismic investigation settings - etc (조사설정 - 기타)
*/
#define T_SIET_K unsigned int
struct T_SIET_D
{
	T_SIPA_K kSipa;				//(조사 케이스)내진조사 파라미터 Key
	T_SIGR_LOWER_K kSigrLower;	//(조사 대상)내진조사 하부구조 그룹 Key
	int nOutTypeBearing;		//지승결과 산출방법 : enum T_SIET_D::eOutTypeBearing
	BOOL bPierBase_Sf;			//단면력 출력여부 산출위치 : 교각 기부(橋脚基部)
	BOOL bFootingLower_Sf;		//단면력 출력여부 산출위치 : 기초 하단(基礎下段)
	int nOutTypeMember;			//단면력 동시성 결과 출력방법 : enum T_SIET_D::eOutTypeMember

	T_SIET_D() { Initialize(); }
	T_SIET_D(const T_SIET_D& src) { *this = src; }
	T_SIET_D& operator=(const T_SIET_D& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigrLower = src.kSigrLower;
			nOutTypeBearing = src.nOutTypeBearing;
			bPierBase_Sf = src.bPierBase_Sf;
			bFootingLower_Sf = src.bFootingLower_Sf;
			nOutTypeMember = src.nOutTypeMember;
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigrLower = 0;
		nOutTypeBearing = 0;
		bPierBase_Sf = FALSE;
		bFootingLower_Sf = FALSE;
		nOutTypeMember = 0;
	}

	enum eOutTypeBearing
	{
		kUnKnownType_Br,
		kMinMax_Br,			//최대/최소
		kConcurvity_Each_Br,//동시성 각각
		kConcurvity_Line_Br,//동시성 지승선별
	};

	enum eOutTypeMember
	{
		kUnKnownType_Mb,
		kMinMax_Mb,				//최대/최소
		kConcurvity_Each_Mb,	//각각
		kConcurvity_FootingLow, //기초하단기준
	};

};

struct T_SIET_D_CH
{
	T_SIPA_K kSipa;
	T_SIGR_LOWER_K kSigrLower;
	int nOutTypeBearing;
	BOOL bPierBase_Sf;
	BOOL bFootingLower_Sf;
	int nOutTypeMember;

	void Get(T_SIET_D& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		rData.nOutTypeBearing = nOutTypeBearing;
		rData.bPierBase_Sf = bPierBase_Sf;
		rData.bFootingLower_Sf = bFootingLower_Sf;
		rData.nOutTypeMember = nOutTypeMember;
	}

	void Set(const T_SIET_D& data)
	{
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		nOutTypeBearing = data.nOutTypeBearing;
		bPierBase_Sf = data.bPierBase_Sf;
		bFootingLower_Sf = data.bFootingLower_Sf;
		nOutTypeMember = data.nOutTypeMember;
	}
};

struct T_SIET_UDRD_D
{
	T_SIET_K key;
	T_SIET_D data;
};

struct T_SIET_UDRD_D_CH
{
	T_SIET_K key;
	T_SIET_D_CH data;
};

struct T_SIET_UDRD_D_CH_RW
{
	T_SIET_K key;
	T_SIPA_K kSipa;
	T_SIGR_LOWER_K kSigrLower;
	int nOutTypeBearing;
	BOOL bPierBase_Sf;
	BOOL bFootingLower_Sf;
	int nOutTypeMember;

	void Get(T_SIET_K& rKey, T_SIET_D_CH& rData)
	{
		rKey = key;
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		rData.nOutTypeBearing = nOutTypeBearing;
		rData.bPierBase_Sf = bPierBase_Sf;
		rData.bFootingLower_Sf = bFootingLower_Sf;
		rData.nOutTypeMember = nOutTypeMember;
	}
	void Set(const T_SIET_K Key, const T_SIET_D_CH& data)
	{
		key = Key;
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		nOutTypeBearing = data.nOutTypeBearing;
		bPierBase_Sf = data.bPierBase_Sf;
		bFootingLower_Sf = data.bFootingLower_Sf;
		nOutTypeMember = data.nOutTypeMember;
	}
};

struct T_SIET_UNIT
{
	T_SIET_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};

#define HASHSIZESIET 1001
#define HASHSIZELISTSIET 11

#pragma pack(pop)

#endif  // __DB_ST_DT_SIET_H__