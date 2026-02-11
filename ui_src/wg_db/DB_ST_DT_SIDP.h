#ifndef __DB_ST_DT_SIDP_H__
#define __DB_ST_DT_SIDP_H__

#define MIDAS_SDK

#pragma pack(push, 1)

// for Civil JP

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// 조사설정 - 변위 소성율 ////////////////////////////////////////

/**
* Allowable displacement plasticity rate (조사조건 - 허용 변위 소성율)
*/
struct T_SIDP_ADPR_D
{
	/**
	* 허용 변위 소성율을 산정하기 위한 식 : enum T_SIDP_ADPR_D::eAllowDispPlasFormula
	* 도시-H14의 경우 : "μa=1+(δu-δy)/(αδy)"  이 식만 활성화
	* 도시-H24의 경우 : "μa=1+(δls-δy)/(αδy)", "μa=δls/(αδy)" 둘 중 선택
	*/
	int nAllowDispPlasFormula;
	/**
	* 안전 계수α, "조사 대상" 에서 선택한 교각 그룹의 지승 조건이 면진지승 인 경우,  
	* "α" 표기를 "αm"으로 변경
	*/
	double dAlpha;
	/**
	* δls : "조사 대상" 에서 δy 가 직접 입력이 경우 활성
	*/
	double dDeltaLs;

	T_SIDP_ADPR_D() { }
	T_SIDP_ADPR_D(const T_SIDP_ADPR_D& src) { *this = src; }
	T_SIDP_ADPR_D& operator=(const T_SIDP_ADPR_D& src)
	{
		if (this != &src) {
			nAllowDispPlasFormula = src.nAllowDispPlasFormula;
			dAlpha = src.dAlpha;
			dDeltaLs = src.dDeltaLs;
		}
		return *this;
	}

	void Initialize()
	{
		nAllowDispPlasFormula = 0;
		dAlpha = 0;
		dDeltaLs = std::nan("-");
	}

	/**
	* 허용 변위 소성율을 산정하기 위한 식
	*/
	enum eAllowDispPlasFormula
	{
		kUnKnownFormula,
		kMuA_1_Plus_Ellipsis,// "μa=1+(δu-δy)/(αδy)"
		kMuA_DeltaLs_Ellipsis,// "μa=1+(δls-δy)/(αδy)", "μa=δls/(αδy)"
	};
};

struct T_SIDP_ADPR_D_CH
{
	int nAllowDispPlasFormula;
	double dAlpha;
	double dDeltaLs;

	void Get(T_SIDP_ADPR_D& rData)
	{
		rData.nAllowDispPlasFormula = nAllowDispPlasFormula;
		rData.dAlpha = dAlpha;
		rData.dDeltaLs = dDeltaLs;
	}

	void Set(const T_SIDP_ADPR_D& data)
	{
		nAllowDispPlasFormula = data.nAllowDispPlasFormula;
		dAlpha = data.dAlpha;
		dDeltaLs = data.dDeltaLs;
	}
};

/**
* Limit State displacement - plastic hinge length (조사조건 - 한계상태 변위_소성힌지 길이)
*/
struct T_SIDP_LSLP_D
{
	/**
	* 종극 변위_소성힌지 길이 enum T_SIDP_LSLP_D::eLpFormula
	* 도시-H14의 경우 : "Lp= 0.2h-0.1D", "직접 입력" 만 활성화
	* 도시-H24의 경우 : "Lp= 0.2h-0.1D", "Lp = 9.5σsy^(1/6)βn^(-1/3)φ'", "직접 입력" 중 선택
	*/
	int nLpFormula;
	double dLpLength; // nLpFormula 가 "직접 입력" 인 경우 입력 값

	T_SIDP_LSLP_D() { }
	T_SIDP_LSLP_D(const T_SIDP_LSLP_D& src) { *this = src; }
	T_SIDP_LSLP_D& operator=(const T_SIDP_LSLP_D& src)
	{
		if (this != &src) {
			nLpFormula = src.nLpFormula;
			dLpLength = src.dLpLength;
		}
		return *this;
	}

	void Initialize()
	{
		nLpFormula = 0;
		dLpLength = 0;
	}

	enum eLpFormula
	{
		kUnKnownFormula,
		kLp_0point2_Ellipsis,	// "Lp = 0.2h-0.1D"
		kLp_9point5_Ellipsis,	// "Lp = 9.5σsy^(1/6)βn^(-1/3)φ'"
		kLp_UserValue,
	};
};

struct T_SIDP_LSLP_D_CH
{
	int nLpFormula;
	double dLpLength;

	void Get(T_SIDP_LSLP_D& rData)
	{
		rData.nLpFormula = nLpFormula;
		rData.dLpLength = dLpLength;
	}

	void Set(const T_SIDP_LSLP_D& data)
	{
		nLpFormula = data.nLpFormula;
		dLpLength = data.dLpLength;
	}
};

struct T_SIDP_LSLP_D_CH_RW
{
	int nLpFormula;
	double dLpLength;

	void Get(T_SIDP_LSLP_D_CH& rData)
	{
		rData.nLpFormula = nLpFormula;
		rData.dLpLength = dLpLength;
	}

	void Set(const T_SIDP_LSLP_D_CH& data)
	{
		nLpFormula = data.nLpFormula;
		dLpLength = data.dLpLength;
	}
};

/**
* Seismic investigation settings - displacement plasticity rate (조사설정 - 변위 소성율)
*/
#define T_SIDP_K unsigned int
struct T_SIDP_D
{
	T_SIPA_K	   kSipa;		//(조사 케이스)내진조사 파라미터 Key
	T_SIGR_LOWER_K kSigrLower;	//(조사 대상)내진조사 그룹 Key
	CString strNameCOLM;		//기둥의 이름
	T_SIDP_ADPR_D  ADPR_D;		//조사조건 - 허용 변위 소성율
	T_SIDP_LSLP_D  LSLP_D_DEL;		// v965 사용 안함 -> 삭제

	// 아래 변수는 삭제된 것으로 사용하지 말것
	int nDirDisp_del;			//변위 방향 : enum seis_jp::eDirDisp	<-- 2024.11.07 잔류변위조사설정 값을 따라가기로 해서 삭제
	double dAngle_del;			//X축과의 각도							<-- 2024.11.07 잔류변위조사설정 값을 따라가기로 해서 삭제

	T_SIDP_D() { Initialize(); }
	T_SIDP_D(const T_SIDP_D& src) { *this = src; }
	T_SIDP_D& operator=(const T_SIDP_D& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigrLower = src.kSigrLower;
			strNameCOLM = src.strNameCOLM;
			nDirDisp_del = src.nDirDisp_del;
			dAngle_del = src.dAngle_del;
			ADPR_D = src.ADPR_D;
			LSLP_D_DEL = src.LSLP_D_DEL;
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigrLower = 0;
		strNameCOLM.Empty();
		nDirDisp_del = 0;
		dAngle_del = 0;
		ADPR_D.Initialize();
		LSLP_D_DEL.Initialize();
	}
};

struct T_SIDP_D_CH
{
	T_SIPA_K kSipa;
	T_SIGR_LOWER_K kSigrLower;
	char strNameCOLM[60];
	int nDirDisp_del;
	double dAngle_del;
	T_SIDP_ADPR_D_CH ADPR_D;
	T_SIDP_LSLP_D_CH LSLP_D_DEL;

	void Get(T_SIDP_D& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		ConvertCharStr(strNameCOLM, rData.strNameCOLM, sizeof(strNameCOLM));
		rData.nDirDisp_del = nDirDisp_del;
		rData.dAngle_del = dAngle_del;
		ADPR_D.Get(rData.ADPR_D);
		LSLP_D_DEL.Get(rData.LSLP_D_DEL);
	}

	void Set(/*const*/ T_SIDP_D& data)
	{
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		ConvertStrChar(data.strNameCOLM, strNameCOLM, sizeof(strNameCOLM));
		nDirDisp_del = data.nDirDisp_del;
		dAngle_del = data.dAngle_del;
		ADPR_D.Set(data.ADPR_D);
		LSLP_D_DEL.Set(data.LSLP_D_DEL);
	}
};

#include "DB_ST_DT_SIDP_OLD.h"

struct T_SIDP_ADPR_D_CH_RW
{
	int nAllowDispPlasFormula;
	double dAlpha;
	double dDeltaLs;

	void Get(T_SIDP_ADPR_D_CH& rData)
	{
		rData.nAllowDispPlasFormula = nAllowDispPlasFormula;
		rData.dAlpha = dAlpha;
		rData.dDeltaLs = dDeltaLs;
	}

	void Set(const T_SIDP_ADPR_D_CH& data)
	{
		nAllowDispPlasFormula = data.nAllowDispPlasFormula;
		dAlpha = data.dAlpha;
		dDeltaLs = data.dDeltaLs;
	}

	void Convert955(T_SIDP_ADPR_D_CH_RW_955& data)
	{
		nAllowDispPlasFormula = data.nAllowDispPlasFormula;
		dAlpha = data.dAlpha;
		dDeltaLs = std::nan("-");
	}
};

struct T_SIDP_D_CH_RW
{
	T_SIDP_K key;
	T_SIPA_K kSipa;
	T_SIGR_LOWER_K kSigrLower;
	char strNameCOLM[60];
	int nDirDisp_del;
	double dAngle_del;
	T_SIDP_ADPR_D_CH_RW ADPR_D;
	T_SIDP_LSLP_D_CH_RW LSLP_D_DEL;

	void Get(T_SIDP_K& rKey, T_SIDP_D_CH& rData)
	{
		rKey = key;
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		memcpy(rData.strNameCOLM, strNameCOLM, sizeof(strNameCOLM));
		rData.nDirDisp_del = nDirDisp_del;
		rData.dAngle_del = dAngle_del;
		ADPR_D.Get(rData.ADPR_D);
		LSLP_D_DEL.Get(rData.LSLP_D_DEL);
	}
	void Set(const T_SIDP_K Key, const T_SIDP_D_CH& data)
	{
		key = Key;
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		memcpy(strNameCOLM, data.strNameCOLM, sizeof(strNameCOLM));
		nDirDisp_del = data.nDirDisp_del;
		dAngle_del = data.dAngle_del;
		ADPR_D.Set(data.ADPR_D);
		LSLP_D_DEL.Set(data.LSLP_D_DEL);
	}
	void Convert955(T_SIDP_D_CH_RW_955& data)
	{
		key = data.key;
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		memcpy(strNameCOLM, data.strNameCOLM, sizeof(strNameCOLM));
		nDirDisp_del = data.nDirDisp_del;
		dAngle_del = data.dAngle_del;
		ADPR_D.Convert955(data.ADPR_D);
		LSLP_D_DEL = data.LSLP_D;
	}
};

struct T_SIDP_UDRD_D
{
	T_SIDP_K key;
	T_SIDP_D data;
};

struct T_SIDP_UDRD_D_CH
{
	T_SIDP_K key;
	T_SIDP_D_CH data;
};

struct T_SIDP_UNIT
{
	T_SIDP_UNIT()
	{
		Length = D_UNITSYS_BASE_LENGTH;
	}
	int Length;
};

#define HASHSIZESIDP 1001
#define HASHSIZELISTSIDP 11

#pragma pack(pop)

#endif  // __DB_ST_DT_SIDP_H__