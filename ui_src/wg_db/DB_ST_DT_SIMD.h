#ifndef __DB_ST_DT_SIMD_H__
#define __DB_ST_DT_SIMD_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#include "DB_ST_DT_SEIS_ENUM.h"

// for Civil JP

/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// 조사설정 - 최대응답변위 ///////////////////////////////////

/**
* Seismic performance limit displacement (내진성능 한계 변위)
*/
struct T_SIMD_SPLD_D
{
	T_THIS_K kThis;		//시간이력해석 케이스(정적해석은 제외)
	T_SIGR_LOWER_K kSigrLower;	//(교각)하부구조 그룹 Key
	CString strNameCOLM;//기둥의 이름
	int nDirAnal;		//해석방향 enum seis_jp::eDirAnal
	int nStepDeltaY;	//(도시V-H14의 경우만 사용)δy: 선택한 하중 케이스에 대해 항복변위 결과를 추출할 스텝
	/**
	* 도시V-H14의 경우(내진성능2,3 & 상부구조) : δu  - 선택한 하중 케이스에 대해 종극변위 결과를 추출할 스텝
	* 도시V-H24의 경우(내진성능2,3 & 상부구조) : δls - 선택한 하중 케이스에 대해 한계상태 변위 결과를 추출할 스텝
	* 도시V-H29의 경우(한계상태  1 & 하부구조) : δyE - 선택한 하중 케이스에 대해 한계상태 변위 결과를 추출할 스텝
	* 도시V-H29의 경우(한계상태2,3 & 상부구조) : δls							〃
	* 도시V-H29의 경우(한계상태2,3 & 하부구조) : δls							〃
	*/
	int nStepDeltaVar;
	/**
	* T_SIMD_D::nCalcType 이 '제한값 직접 입력'의 경우에만 활성화
	* 도시V-H14의 경우(내진성능2,3 & 상부구조) : δa
	* 도시V-H24의 경우(내진성능2,3 & 상부구조) : δa
	* 도시V-H29의 경우(한계상태  1 & 하부구조) : δyEd
	* 도시V-H29의 경우(한계상태2,3 & 상부구조) : δlsd
	* 도시V-H29의 경우(한계상태2,3 & 하부구조) : δlsd
	*/
	double dLengthDeltaVar;

	T_SIMD_SPLD_D() { }
	T_SIMD_SPLD_D(const T_SIMD_SPLD_D& src) { *this = src; }
	T_SIMD_SPLD_D& operator=(const T_SIMD_SPLD_D& src)
	{
		if (this != &src) {
			kThis = src.kThis;
			kSigrLower = src.kSigrLower;
			strNameCOLM = src.strNameCOLM;
			nDirAnal = src.nDirAnal;
			nStepDeltaY = src.nStepDeltaY;
			nStepDeltaVar = src.nStepDeltaVar;
			dLengthDeltaVar = src.dLengthDeltaVar;
		}
		return *this;
	}

	void Initialize()
	{
		kThis = 0;
		kSigrLower = 0;
		strNameCOLM.Empty();
		nDirAnal = seis_jp::kDirAnal_0;
		nStepDeltaY = 0;
		nStepDeltaVar = 0;
		dLengthDeltaVar = 0;
	}
};

struct T_SIMD_SPLD_D_CH
{
	T_THIS_K kThis;
	T_SIGR_LOWER_K kSigrLower;
	char strNameCOLM[60];
	int nDirAnal;
	int nStepDeltaY;
	int nStepDeltaVar;
	double dLengthDeltaVar;

	void Get(T_SIMD_SPLD_D& rData)
	{
		rData.kThis = kThis;
		rData.kSigrLower = kSigrLower;
		ConvertCharStr(strNameCOLM, rData.strNameCOLM, sizeof(strNameCOLM));
		rData.nDirAnal = nDirAnal;
		rData.nStepDeltaY = nStepDeltaY;
		rData.nStepDeltaVar = nStepDeltaVar;
		rData.dLengthDeltaVar = dLengthDeltaVar;
	}

	void Set(/*const*/ T_SIMD_SPLD_D& data)
	{
		kThis = data.kThis;
		kSigrLower = data.kSigrLower;
		ConvertStrChar(data.strNameCOLM, strNameCOLM, sizeof(strNameCOLM));
		nDirAnal = data.nDirAnal;
		nStepDeltaY = data.nStepDeltaY;
		nStepDeltaVar = data.nStepDeltaVar;
		dLengthDeltaVar = data.dLengthDeltaVar;
	}
};

struct T_SIMD_SPLD_D_CH_RW
{
	T_THIS_K kThis;
	T_SIGR_LOWER_K kSigrLower;
	char strNameCOLM[60];
	int nDirAnal;
	int nStepDeltaY;
	int nStepDeltaVar;
	double dLengthDeltaVar;

	void Get(T_SIMD_SPLD_D_CH& rData)
	{
		rData.kThis = kThis;
		rData.kSigrLower = kSigrLower;
		memcpy(rData.strNameCOLM, strNameCOLM, sizeof(strNameCOLM));
		rData.nDirAnal = nDirAnal;
		rData.nStepDeltaY = nStepDeltaY;
		rData.nStepDeltaVar = nStepDeltaVar;
		rData.dLengthDeltaVar = dLengthDeltaVar;
	}

	void Set(const T_SIMD_SPLD_D_CH& data)
	{
		kThis = data.kThis;
		kSigrLower = data.kSigrLower;
		memcpy(strNameCOLM, data.strNameCOLM, sizeof(strNameCOLM));
		nDirAnal = data.nDirAnal;
		nStepDeltaY = data.nStepDeltaY;
		nStepDeltaVar = data.nStepDeltaVar;
		dLengthDeltaVar = data.dLengthDeltaVar;
	}
};

/**
* Seismic investigation settings - maximum response displacement (조사설정 - 최대응답변위)
*/
#define T_SIMD_K unsigned int
struct T_SIMD_D_955
{
	T_SIPA_K kSipa;			//(조사 케이스)내진조사 파라미터 Key
	T_SIGR_K kSigr;			//(조사 대상)내진조사 그룹 Key

	int nDirDisp;			//변위 방향 : enum seis_jp::eDirDisp
	double dAngle;			//X축과의 각도

	//내진성능 한계 변위 관련변수
	int nAlwCalcType;		//허용변위 계산 방식 : enum seis_jp::eAlwCalcType
	double dAlpha;			//안전 계수α
	/**
	* 도시V-H14의 경우 : 사용하지 않음
	* 도시V-H24의 경우 : 기초변위 영향분에 대해 안전계수 고려
	* 도시V-H29의 경우 : 기초변위 영향분에 대해 부분계수 고려
	*/
	BOOL bConsiderAlpha;
	T_SIMD_SPLD_D PLUS_D;	//(+) 방향 내진성능 한계 변위 조건
	T_SIMD_SPLD_D MINUS_D;	//(-) 방향 내진성능 한계 변위 조건

	T_SIMD_D_955() { Initialize(); }
	T_SIMD_D_955(const T_SIMD_D_955& src) { *this = src; }
	T_SIMD_D_955& operator=(const T_SIMD_D_955& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			nDirDisp = src.nDirDisp;
			dAngle = src.dAngle;
			nAlwCalcType = src.nAlwCalcType;
			dAlpha = src.dAlpha;
			bConsiderAlpha = src.bConsiderAlpha;
			PLUS_D = src.PLUS_D;
			MINUS_D = src.MINUS_D;
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigr = 0;
		nDirDisp = 0;
		dAngle = 0;
		nAlwCalcType = 0;
		dAlpha = 0;
		bConsiderAlpha = 0;
		PLUS_D.Initialize();
		MINUS_D.Initialize();
	}
};

struct T_SIMD_D_CH_955
{
	T_SIPA_K kSipa;
	T_SIGR_K kSigr;
	int nDirDisp;
	double dAngle;
	int nAlwCalcType;
	double dAlpha;
	BOOL bConsiderAlpha;
	T_SIMD_SPLD_D_CH PLUS_D;
	T_SIMD_SPLD_D_CH MINUS_D;

	void Get(T_SIMD_D_955& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.nDirDisp = nDirDisp;
		rData.dAngle = dAngle;
		rData.nAlwCalcType = nAlwCalcType;
		rData.dAlpha = dAlpha;
		rData.bConsiderAlpha = bConsiderAlpha;
		PLUS_D.Get(rData.PLUS_D);
		MINUS_D.Get(rData.MINUS_D);
	}

	void Set(/*const*/ T_SIMD_D_955& data)
	{
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		nDirDisp = data.nDirDisp;
		dAngle = data.dAngle;
		nAlwCalcType = data.nAlwCalcType;
		dAlpha = data.dAlpha;
		bConsiderAlpha = data.bConsiderAlpha;
		PLUS_D.Set(data.PLUS_D);
		MINUS_D.Set(data.MINUS_D);
	}
};

struct T_SIMD_D_CH_RW_955
{
	T_SIMD_K key;
	T_SIPA_K kSipa;
	T_SIGR_K kSigr;
	int nDirDisp;
	double dAngle;
	int nAlwCalcType;
	double dAlpha;
	BOOL bConsiderAlpha;
	T_SIMD_SPLD_D_CH_RW PLUS_D;
	T_SIMD_SPLD_D_CH_RW MINUS_D;

	void Get(T_SIMD_K& rKey, T_SIMD_D_CH_955& rData)
	{
		rKey = key;
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.nDirDisp = nDirDisp;
		rData.dAngle = dAngle;
		rData.nAlwCalcType = nAlwCalcType;
		rData.dAlpha = dAlpha;
		rData.bConsiderAlpha = bConsiderAlpha;
		PLUS_D.Get(rData.PLUS_D);
		MINUS_D.Get(rData.MINUS_D);
	}
	void Set(const T_SIMD_K Key, const T_SIMD_D_CH_955& data)
	{
		key = Key;
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		nDirDisp = data.nDirDisp;
		dAngle = data.dAngle;
		nAlwCalcType = data.nAlwCalcType;
		dAlpha = data.dAlpha;
		bConsiderAlpha = data.bConsiderAlpha;
		PLUS_D.Set(data.PLUS_D);
		MINUS_D.Set(data.MINUS_D);
	}
};

struct T_SIMD_UDRD_D_955
{
	T_SIMD_K key;
	T_SIMD_D_955 data;
};

struct T_SIMD_UDRD_D_CH_955
{
	T_SIMD_K key;
	T_SIMD_D_CH_955 data;
};


struct T_SIMD_TIME_RANGE_D
{
	T_THIS_K kThis;		//시간이력해석 케이스
	double dTimeStt;
	double dTimeEnd;

	T_SIMD_TIME_RANGE_D() {}
	T_SIMD_TIME_RANGE_D(const T_SIMD_TIME_RANGE_D& src) { *this = src; }
	T_SIMD_TIME_RANGE_D& operator=(const T_SIMD_TIME_RANGE_D& src)
	{
		if (this != &src) {
			kThis = src.kThis;
			dTimeStt = src.dTimeStt;
			dTimeEnd = src.dTimeEnd;
		}
		return *this;
	}

	void Initialize()
	{
		kThis = 0;
		dTimeStt = 0;
		dTimeEnd = 0;
	}
};
typedef CArray<T_SIMD_TIME_RANGE_D, T_SIMD_TIME_RANGE_D> CSimdTimeArray;

struct T_SIMD_D
{
	T_SIPA_K kSipa;			//(조사 케이스)내진조사 파라미터 Key
	T_SIGR_K kSigr;			//(조사 대상)내진조사 그룹 Key

	int nDirDisp;			//변위 방향 : enum seis_jp::eDirDisp
	double dAngle;			//X축과의 각도

	//내진성능 한계 변위 관련변수
	int nAlwCalcType;		//허용변위 계산 방식 : enum seis_jp::eAlwCalcType
	double dAlpha;			//안전 계수α

	/**
	* 도시V-H14의 경우 : 사용하지 않음
	* 도시V-H24의 경우 : 기초변위 영향분에 대해 안전계수 고려
	* 도시V-H29의 경우 : 기초변위 영향분에 대해 부분계수 고려
	*/
	BOOL bConsiderAlpha;
	T_SIMD_SPLD_D PLUS_D;	//(+) 방향 내진성능 한계 변위 조건
	T_SIMD_SPLD_D MINUS_D;	//(-) 방향 내진성능 한계 변위 조건

	int nRspCalcType;			//응답변위 계산 방식 : enum seis_jp::eRspCalcType - v965 추가
	BOOL bTimeStepRange;		//변위를 계산할 시간대의 설정 유무 - v965 추가
	CSimdTimeArray aTimeRange;	//변위를 계산할 시간대 - v965 추가

	T_SIMD_D() { Initialize(); }
	T_SIMD_D(const T_SIMD_D& src) { *this = src; }
	T_SIMD_D& operator=(const T_SIMD_D& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			nDirDisp = src.nDirDisp;
			dAngle = src.dAngle;
			nAlwCalcType = src.nAlwCalcType;
			dAlpha = src.dAlpha;
			bConsiderAlpha = src.bConsiderAlpha;
			PLUS_D = src.PLUS_D;
			MINUS_D = src.MINUS_D;
			nRspCalcType = src.nRspCalcType;
			bTimeStepRange = src.bTimeStepRange;
			aTimeRange.Copy(src.aTimeRange);
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigr = 0;
		nDirDisp = seis_jp::kDirDisp_0;
		dAngle = 0;
		nAlwCalcType = seis_jp::kAlwCalcType_0;
		dAlpha = 0;
		bConsiderAlpha = 0;
		PLUS_D.Initialize();
		MINUS_D.Initialize();
		nRspCalcType = seis_jp::kRspType_0;
		bTimeStepRange = FALSE;
		aTimeRange.RemoveAll();
	}
	void Convert955(T_SIMD_D_955& rData)
	{
		kSipa = rData.kSipa;
		kSigr = rData.kSigr;
		nDirDisp = rData.nDirDisp;
		dAngle = rData.dAngle;
		nAlwCalcType = rData.nAlwCalcType;
		dAlpha = rData.dAlpha;
		bConsiderAlpha = rData.bConsiderAlpha;
		PLUS_D = rData.PLUS_D;
		MINUS_D = rData.MINUS_D;
		nRspCalcType = seis_jp::kNEXCO;
		bTimeStepRange = FALSE;
		aTimeRange.RemoveAll();
	}
};

struct T_SIMD_D_CH
{
	T_SIPA_K kSipa;
	T_SIGR_K kSigr;
	int nDirDisp;
	double dAngle;
	int nAlwCalcType;
	double dAlpha;
	BOOL bConsiderAlpha;
	T_SIMD_SPLD_D_CH PLUS_D;
	T_SIMD_SPLD_D_CH MINUS_D;
	int nRspCalcType;
	BOOL bTimeStepRange;
	CSimdTimeArray aTimeRange;

	void Get(T_SIMD_D& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.nDirDisp = nDirDisp;
		rData.dAngle = dAngle;
		rData.nAlwCalcType = nAlwCalcType;
		rData.dAlpha = dAlpha;
		rData.bConsiderAlpha = bConsiderAlpha;
		PLUS_D.Get(rData.PLUS_D);
		MINUS_D.Get(rData.MINUS_D);
		rData.nRspCalcType = nRspCalcType;
		rData.bTimeStepRange = bTimeStepRange;
		rData.aTimeRange.Copy(aTimeRange);
	}

	void Set(/*const*/ T_SIMD_D& data)
	{
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		nDirDisp = data.nDirDisp;
		dAngle = data.dAngle;
		nAlwCalcType = data.nAlwCalcType;
		dAlpha = data.dAlpha;
		bConsiderAlpha = data.bConsiderAlpha;
		PLUS_D.Set(data.PLUS_D);
		MINUS_D.Set(data.MINUS_D);
		nRspCalcType = data.nRspCalcType;
		bTimeStepRange = data.bTimeStepRange;
		aTimeRange.Copy(data.aTimeRange);
	}
};

struct T_SIMD_D_CH_RW
{
	T_SIMD_K key;
	T_SIPA_K kSipa;
	T_SIGR_K kSigr;
	int nDirDisp;
	double dAngle;
	int nAlwCalcType;
	double dAlpha;
	BOOL bConsiderAlpha;
	T_SIMD_SPLD_D_CH_RW PLUS_D;
	T_SIMD_SPLD_D_CH_RW MINUS_D;
	int nRspCalcType;
	BOOL bTimeStepRange;
	int nTimeRange;

	void Get(T_SIMD_K& rKey, T_SIMD_D_CH& rData)
	{
		rKey = key;
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.nDirDisp = nDirDisp;
		rData.dAngle = dAngle;
		rData.nAlwCalcType = nAlwCalcType;
		rData.dAlpha = dAlpha;
		rData.bConsiderAlpha = bConsiderAlpha;
		PLUS_D.Get(rData.PLUS_D);
		MINUS_D.Get(rData.MINUS_D);
		rData.nRspCalcType = nRspCalcType;
		rData.bTimeStepRange = bTimeStepRange;
	}
	void Set(const T_SIMD_K Key, const T_SIMD_D_CH& data)
	{
		key = Key;
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		nDirDisp = data.nDirDisp;
		dAngle = data.dAngle;
		nAlwCalcType = data.nAlwCalcType;
		dAlpha = data.dAlpha;
		bConsiderAlpha = data.bConsiderAlpha;
		PLUS_D.Set(data.PLUS_D);
		MINUS_D.Set(data.MINUS_D);
		nRspCalcType = data.nRspCalcType;
		bTimeStepRange = data.bTimeStepRange;
		nTimeRange = (int)data.aTimeRange.GetSize();
	}
};

struct T_SIMD_UDRD_D
{
	T_SIMD_K key;
	T_SIMD_D data;
};

struct T_SIMD_UDRD_D_CH
{
	T_SIMD_K key;
	T_SIMD_D_CH data;
};

struct T_SIMD_UNIT
{
	T_SIMD_UNIT()
	{
		Angle = D_UNITSYS_BASE_DEGREE;
		Length = D_UNITSYS_BASE_LENGTH;
	}
	int Angle;
	int Length;
};

#define HASHSIZESIMD 1001
#define HASHSIZELISTSIMD 11

#pragma pack(pop)

#endif  // __DB_ST_DT_SIMD_H__