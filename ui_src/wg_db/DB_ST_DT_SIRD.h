#ifndef __DB_ST_DT_SIRD_H__
#define __DB_ST_DT_SIRD_H__

#define MIDAS_SDK

#include "DB_ST_DT_SEIS_ENUM.h"

#pragma pack(push, 1)

// for Civil JP

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// 조사설정 - 잔류변위 /////////////////////////////////////

/**
* Residual Displacement (잔류 변위 계산조건)
*/
struct T_SIRD_RSDP_D
{
	T_THIS_K kThis;		//푸시오버 해석 케이스(정적해석 수법 시간력)
	T_SIGR_LOWER_K kSigrLower;	//(교각)하부구조 그룹 Key
	CString strNameCOLM;//기둥의 이름
	int nDirAnal;		//해석방향 enum seis_jp::eDirAnal
	/**
	* "계산 방식=해석결과로 산정"의 경우 활성화: 선택한 하중 케이스에 대해 항복변위 결과를 추출할 스텝
	* "부재 M-φ관계로 산정"의 경우 "교각별로 잔류변위을 조사할 경우의 항복변위δy 산출 방법"에 따라 내부 계산
	*/
	int nStepDeltaY;
	/**
	* T_SIRD_D::nCalcType 이 "직접입력"의 경우에만 활성화
	*/
	double dLengthDeltaVar;

	T_SIRD_RSDP_D() { }
	T_SIRD_RSDP_D(const T_SIRD_RSDP_D& src) { *this = src; }
	T_SIRD_RSDP_D& operator=(const T_SIRD_RSDP_D& src)
	{
		if (this != &src) {
			kThis = src.kThis;
			kSigrLower = src.kSigrLower;
			strNameCOLM = src.strNameCOLM;
			nDirAnal = src.nDirAnal;
			nStepDeltaY = src.nStepDeltaY;
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
		dLengthDeltaVar = 0;
	}
};

struct T_SIRD_RSDP_D_CH
{
	T_THIS_K kThis;
	T_SIGR_LOWER_K kSigrLower;
	char strNameCOLM[60];
	int nDirAnal;
	int nStepDeltaY;
	double dLengthDeltaVar;

	void Get(T_SIRD_RSDP_D& rData)
	{
		rData.kThis = kThis;
		rData.kSigrLower = kSigrLower;
		ConvertCharStr(strNameCOLM, rData.strNameCOLM, sizeof(strNameCOLM));
		rData.nDirAnal = nDirAnal;
		rData.nStepDeltaY = nStepDeltaY;
		rData.dLengthDeltaVar = dLengthDeltaVar;
	}

	void Set(/*const */T_SIRD_RSDP_D& data)
	{
		kThis = data.kThis;
		kSigrLower = data.kSigrLower;
		ConvertStrChar(data.strNameCOLM, strNameCOLM, sizeof(strNameCOLM));
		nDirAnal = data.nDirAnal;
		nStepDeltaY = data.nStepDeltaY;
		dLengthDeltaVar = data.dLengthDeltaVar;
	}
};

struct T_SIRD_RSDP_D_CH_RW
{
	T_THIS_K kThis;
	T_SIGR_LOWER_K kSigrLower;
	char strNameCOLM[60];
	int nDirAnal;
	int nStepDeltaY;
	double dLengthDeltaVar;

	void Get(T_SIRD_RSDP_D_CH& rData)
	{
		rData.kThis = kThis;
		rData.kSigrLower = kSigrLower;
		memcpy(rData.strNameCOLM, strNameCOLM, sizeof(strNameCOLM));
		rData.nDirAnal = nDirAnal;
		rData.nStepDeltaY = nStepDeltaY;
		rData.dLengthDeltaVar = dLengthDeltaVar;
	}

	void Set(const T_SIRD_RSDP_D_CH& data)
	{
		kThis = data.kThis;
		kSigrLower = data.kSigrLower;
		memcpy(strNameCOLM, data.strNameCOLM, sizeof(strNameCOLM));
		nDirAnal = data.nDirAnal;
		nStepDeltaY = data.nStepDeltaY;
		dLengthDeltaVar = data.dLengthDeltaVar;
	}
};

/**
* Seismic investigation settings - residual displacement (조사설정 - 잔류변위)
*/
#define T_SIRD_K unsigned int
struct T_SIRD_D_955
{
	T_SIPA_K kSipa;				//(조사 케이스)내진조사 파라미터 Key
	T_SIGR_K kSigr;				//(조사 대상)내진조사 그룹 Key
	int nDirDisp;				//변위 방향 : enum seis_jp::eDirDisp
	double dAngle;				//X축과의 각도

	//내진성능 한계 변위 관련변수
	/**
	* 계산 방식 : enum seis_jp::eAlwCalcType
	* 조사 대상=상부구조 경우, 리스트="자동 산정", "해석결과로 산정", "직접 입력"
	* 조사 대상=하부구조 경우, 리스트="자동 산정", "해석결과로 산정", "부재 M-φ관계로 산정", "직접 입력"
	*/
	int nAlwCalcType;
	T_SIRD_RSDP_D PLUS_D;	//(+) 방향 잔류변위 조건
	T_SIRD_RSDP_D MINUS_D;	//(-) 방향 잔류변위 조건

	T_SIRD_D_955() { Initialize(); }
	T_SIRD_D_955(const T_SIRD_D_955& src) { *this = src; }
	T_SIRD_D_955& operator=(const T_SIRD_D_955& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			nDirDisp = src.nDirDisp;
			dAngle = src.dAngle;
			nAlwCalcType = src.nAlwCalcType;
			PLUS_D = src.PLUS_D;
			MINUS_D = src.MINUS_D;
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigr = 0;
		nDirDisp = seis_jp::kDirDisp_0;
		dAngle = 0;
		nAlwCalcType = 0;
		PLUS_D.Initialize();
		MINUS_D.Initialize();
	}
};

struct T_SIRD_D_CH_955
{
	T_SIPA_K kSipa;
	T_SIGR_K kSigr;
	int nDirDisp;
	double dAngle;
	int nAlwCalcType;
	T_SIRD_RSDP_D_CH PLUS_D;
	T_SIRD_RSDP_D_CH MINUS_D;

	void Get(T_SIRD_D_955& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.nDirDisp = nDirDisp;
		rData.dAngle = dAngle;
		rData.nAlwCalcType = nAlwCalcType;
		PLUS_D.Get(rData.PLUS_D);
		MINUS_D.Get(rData.MINUS_D);
	}

	void Set(/*const*/ T_SIRD_D_955& data)
	{
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		nDirDisp = data.nDirDisp;
		dAngle = data.dAngle;
		nAlwCalcType = data.nAlwCalcType;
		PLUS_D.Set(data.PLUS_D);
		MINUS_D.Set(data.MINUS_D);
	}
};

struct T_SIRD_D_CH_RW_955
{
	T_SIRD_K key;
	T_SIPA_K kSipa;
	T_SIGR_K kSigr;
	int nDirDisp;
	double dAngle;
	int nAlwCalcType;
	T_SIRD_RSDP_D_CH_RW PLUS_D;
	T_SIRD_RSDP_D_CH_RW MINUS_D;

	void Get(T_SIRD_K& rKey, T_SIRD_D_CH_955& rData)
	{
		rKey = key;
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.nDirDisp = nDirDisp;
		rData.dAngle = dAngle;
		rData.nAlwCalcType = nAlwCalcType;
		PLUS_D.Get(rData.PLUS_D);
		MINUS_D.Get(rData.MINUS_D);
	}
	void Set(const T_SIRD_K Key, const T_SIRD_D_CH_955& data)
	{
		key = Key;
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		nDirDisp = data.nDirDisp;
		dAngle = data.dAngle;
		nAlwCalcType = data.nAlwCalcType;
		PLUS_D.Set(data.PLUS_D);
		MINUS_D.Set(data.MINUS_D);
	}
};

struct T_SIRD_UDRD_D_955
{
	T_SIRD_K key;
	T_SIRD_D_955 data;
};

struct T_SIRD_UDRD_D_CH_955
{
	T_SIRD_K key;
	T_SIRD_D_CH_955 data;
};

struct T_SIRD_TIME_RANGE_D
{
	T_THIS_K kThis;		//시간이력해석 케이스
	double dTimeStt;
	double dTimeEnd;

	T_SIRD_TIME_RANGE_D() {}
	T_SIRD_TIME_RANGE_D(const T_SIRD_TIME_RANGE_D& src) { *this = src; }
	T_SIRD_TIME_RANGE_D& operator=(const T_SIRD_TIME_RANGE_D& src)
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
typedef CArray<T_SIRD_TIME_RANGE_D, T_SIRD_TIME_RANGE_D> CSirdTimeArray;

struct T_SIRD_D
{
	T_SIPA_K kSipa;				//(조사 케이스)내진조사 파라미터 Key
	T_SIGR_K kSigr;				//(조사 대상)내진조사 그룹 Key
	int nDirDisp;				//변위 방향 : enum seis_jp::eDirDisp
	double dAngle;				//X축과의 각도

	//내진성능 한계 변위 관련변수
	/**
	* 계산 방식 : enum seis_jp::eAlwCalcType
	* 조사 대상=상부구조 경우, 리스트="자동 산정", "해석결과로 산정", "직접 입력"
	* 조사 대상=하부구조 경우, 리스트="자동 산정", "해석결과로 산정", "부재 M-φ관계로 산정", "직접 입력"
	*/
	int nAlwCalcType;
	T_SIRD_RSDP_D PLUS_D;	//(+) 방향 잔류변위 조건
	T_SIRD_RSDP_D MINUS_D;	//(-) 방향 잔류변위 조건
	
	int nRspCalcType;			//응답변위 계산 방식 : enum seis_jp::eRspCalcType - v965 추가
	BOOL bTimeStepRange;		//변위를 계산할 시간대의 설정 유무 - v965 추가
	CSirdTimeArray aTimeRange;	//변위를 계산할 시간대 - v965 추가

	T_SIRD_D() { Initialize(); }
	T_SIRD_D(const T_SIRD_D& src) { *this = src; }
	T_SIRD_D& operator=(const T_SIRD_D& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			nDirDisp = src.nDirDisp;
			dAngle = src.dAngle;
			nAlwCalcType = src.nAlwCalcType;
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
		nRspCalcType = seis_jp::kRspType_0;
		PLUS_D.Initialize();
		MINUS_D.Initialize();
		bTimeStepRange = FALSE;
		aTimeRange.RemoveAll();
	}
	void Convert955(T_SIRD_D_955& rData)
	{
		kSipa = rData.kSipa;
		kSigr = rData.kSigr;
		nDirDisp = rData.nDirDisp;
		dAngle = rData.dAngle;
		nAlwCalcType = rData.nAlwCalcType;
		PLUS_D = rData.PLUS_D;
		MINUS_D = rData.MINUS_D;
		nRspCalcType = seis_jp::kNEXCO;
		bTimeStepRange = FALSE;
		aTimeRange.RemoveAll();
	}
};

struct T_SIRD_D_CH
{
	T_SIPA_K kSipa;
	T_SIGR_K kSigr;
	int nDirDisp;
	double dAngle;
	int nAlwCalcType;
	T_SIRD_RSDP_D_CH PLUS_D;
	T_SIRD_RSDP_D_CH MINUS_D;
	int nRspCalcType;
	BOOL bTimeStepRange;
	CSirdTimeArray aTimeRange;

	void Get(T_SIRD_D& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.nDirDisp = nDirDisp;
		rData.dAngle = dAngle;
		rData.nAlwCalcType = nAlwCalcType;
		PLUS_D.Get(rData.PLUS_D);
		MINUS_D.Get(rData.MINUS_D);
		rData.nRspCalcType = nRspCalcType;
		rData.bTimeStepRange = bTimeStepRange;
		rData.aTimeRange.Copy(aTimeRange);
	}

	void Set(/*const*/ T_SIRD_D& data)
	{
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		nDirDisp = data.nDirDisp;
		dAngle = data.dAngle;
		nAlwCalcType = data.nAlwCalcType;
		PLUS_D.Set(data.PLUS_D);
		MINUS_D.Set(data.MINUS_D);
		nRspCalcType = data.nRspCalcType;
		bTimeStepRange = data.bTimeStepRange;
		aTimeRange.Copy(data.aTimeRange);
	}
};

struct T_SIRD_D_CH_RW
{
	T_SIRD_K key;
	T_SIPA_K kSipa;
	T_SIGR_K kSigr;
	int nDirDisp;
	double dAngle;
	int nAlwCalcType;
	T_SIRD_RSDP_D_CH_RW PLUS_D;
	T_SIRD_RSDP_D_CH_RW MINUS_D;
	int nRspCalcType;
	BOOL bTimeStepRange;
	int nTimeRange;

	void Get(T_SIRD_K& rKey, T_SIRD_D_CH& rData)
	{
		rKey = key;
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.nDirDisp = nDirDisp;
		rData.dAngle = dAngle;
		rData.nAlwCalcType = nAlwCalcType;
		PLUS_D.Get(rData.PLUS_D);
		MINUS_D.Get(rData.MINUS_D);
		rData.nRspCalcType = nRspCalcType;
		rData.bTimeStepRange = bTimeStepRange;
	}
	void Set(const T_SIRD_K Key, const T_SIRD_D_CH& data)
	{
		key = Key;
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		nDirDisp = data.nDirDisp;
		dAngle = data.dAngle;
		nAlwCalcType = data.nAlwCalcType;
		PLUS_D.Set(data.PLUS_D);
		MINUS_D.Set(data.MINUS_D);
		nRspCalcType = data.nRspCalcType;
		bTimeStepRange = data.bTimeStepRange;
		nTimeRange = (int)data.aTimeRange.GetSize();
	}
};

struct T_SIRD_UDRD_D
{
	T_SIRD_K key;
	T_SIRD_D data;
};

struct T_SIRD_UDRD_D_CH
{
	T_SIRD_K key;
	T_SIRD_D_CH data;
};

struct T_SIRD_UNIT
{
	T_SIRD_UNIT()
	{
		Angle = D_UNITSYS_BASE_DEGREE;
		Length = D_UNITSYS_BASE_LENGTH;
	}
	int Angle;
	int Length;
};

#define HASHSIZESIRD 1001
#define HASHSIZELISTSIRD 11

#pragma pack(pop)

#endif  // __DB_ST_DT_SIRD_H__