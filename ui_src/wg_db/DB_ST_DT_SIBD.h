#ifndef __DB_ST_DT_SIBD_H__
#define __DB_ST_DT_SIBD_H__

#define MIDAS_SDK

#pragma pack(push, 1)

// for Civil JP

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// 조사설정 - 휨 ////////////////////////////////////////

/**
* Strength Survey (내력조사조건)
*/
struct T_SIBD_SRSV_D
{
	/**
	* 내력조사 조건(상부구조 그룹, 사용자 그룹인 경우에만 활성화) : enum T_SIBD_SRSV_D::eMomentType
	* 도시-H14/H24의 경우 : List:My(항복 모멘트), Mu(종극 모멘트)
	* 도시-H29	  의 경우 : "한계상태1,2" 인 경우 List:Myd(항복 모멘트), "한계상태3" 인 경우 List:항복 모멘트(Myd), 파괴 모멘트(Mud)
	*/
	int nMomentType;
	
	T_SIBD_SRSV_D() { }
	T_SIBD_SRSV_D(const T_SIBD_SRSV_D& src) { *this = src; }
	T_SIBD_SRSV_D& operator=(const T_SIBD_SRSV_D& src)
	{
		if (this != &src) {
			nMomentType = src.nMomentType;
		}
		return *this;
	}

	void Initialize()
	{
		nMomentType = 0;
	}

	enum eMomentType
	{
		kUnKnown,
		kMy,
		kMu,
		kMyd,
		kMud,
	};
};

struct T_SIBD_SRSV_D_CH
{
	int nMomentType;

	void Get(T_SIBD_SRSV_D& rData)
	{
		rData.nMomentType = nMomentType;
	}

	void Set(const T_SIBD_SRSV_D& data)
	{
		nMomentType = data.nMomentType;
	}
};

struct T_SIBD_SRSV_D_CH_RW
{
	int nMomentType;

	void Get(T_SIBD_SRSV_D_CH& rData)
	{
		rData.nMomentType = nMomentType;
	}

	void Set(const T_SIBD_SRSV_D_CH& data)
	{
		nMomentType = data.nMomentType;
	}
};

/**
* Curvature Survey (곡률조사조건)
*/
struct T_SIBD_CVSV_D
{
	int nCvsvType;//곡률조사 조건 : enum T_SIBD_CVSV_D::eCvsvType
	double dSurrenderAlpha;		//nCondition 가 항복조사 일 때 허용값 α
	/**
	* RC부재의 허용곡률 enum T_SIBD_CVSV_D::eAllowCurvFormula
	* 도시-H14의 경우 : "φa=φy0 + (φu-φy0)/α"
	* 도시-H24의 경우 : "φa=φy0+(φls-φy0)/α", "φa=φls/α" 둘 중 선택
	* 도시-H29의 경우 : "φls2d=ξ1φskφls2,　φls3d=ξ1ξ2φskφls3" 로 표기
	*/
	int nAllowCurvFormula;
	
	/**
	* nCvsvType 가 한계상태조사 일 때 허용값 α
	* 도시-H14/H24의 경우 : "조사 대상" 에서 선택한 교각 그룹의 지승 조건이 면진지승 인 경우 "α" 표기를 "αm"으로 변경
	* 도시-H29의 경우 : 표기를 "αm"으로 변경, 조사대상이 하부구조이며, 해당 그룹의 지승조건이 "면진지승"인 경우에만 활성화
	*/
	double dLimitStateAlpha;

	T_SIBD_CVSV_D() { }
	T_SIBD_CVSV_D(const T_SIBD_CVSV_D& src) { *this = src; }
	T_SIBD_CVSV_D& operator=(const T_SIBD_CVSV_D& src)
	{
		if (this != &src) {
			nCvsvType = src.nCvsvType;
			dSurrenderAlpha = src.dSurrenderAlpha;
			nAllowCurvFormula = src.nAllowCurvFormula;
			dLimitStateAlpha = src.dLimitStateAlpha;
		}
		return *this;
	}

	void Initialize()
	{
		nCvsvType = 0;
		dSurrenderAlpha = 0;
		nAllowCurvFormula = 0;
		dLimitStateAlpha = 0;
	}

	enum eCvsvType
	{
		kUnKnown,
		kSurrenderSurvey,	//항복조사
		kLimitStateSurvey,	//한계상태조사
	};

	/**
	* RC부재의 허용곡률 공식
	*/
	enum eAllowCurvFormula
	{
		kUnKnownFormula,
		kPhiA_Phiy0_Ellipsis,		// "φa=φy0 + (φu-φy0)/α"
		kPhiA_Phiyls_Ellipsis,		// "φa=φls/α"
		kPhils2d_Phils3d_Ellipsis,	// "φls2d=ξ1φskφls2,　φls3d=ξ1ξ2φskφls3"
	};
};

struct T_SIBD_CVSV_D_CH
{
	int nCvsvType;
	double dSurrenderAlpha;
	int nAllowCurvFormula;
	double dLimitStateAlpha;

	void Get(T_SIBD_CVSV_D& rData)
	{
		rData.nCvsvType = nCvsvType;
		rData.dSurrenderAlpha = dSurrenderAlpha;
		rData.nAllowCurvFormula = nAllowCurvFormula;
		rData.dLimitStateAlpha = dLimitStateAlpha;
	}

	void Set(const T_SIBD_CVSV_D& data)
	{
		nCvsvType = data.nCvsvType;
		dSurrenderAlpha = data.dSurrenderAlpha;
		nAllowCurvFormula = data.nAllowCurvFormula;
		dLimitStateAlpha = data.dLimitStateAlpha;
	}
};

struct T_SIBD_CVSV_D_CH_RW
{
	int nCvsvType;
	double dSurrenderAlpha;
	int nAllowCurvFormula;
	double dLimitStateAlpha;

	void Get(T_SIBD_CVSV_D_CH& rData)
	{
		rData.nCvsvType = nCvsvType;
		rData.dSurrenderAlpha = dSurrenderAlpha;
		rData.nAllowCurvFormula = nAllowCurvFormula;
		rData.dLimitStateAlpha = dLimitStateAlpha;
	}

	void Set(const T_SIBD_CVSV_D_CH& data)
	{
		nCvsvType = data.nCvsvType;
		dSurrenderAlpha = data.dSurrenderAlpha;
		nAllowCurvFormula = data.nAllowCurvFormula;
		dLimitStateAlpha = data.dLimitStateAlpha;
	}
};

/**
* Seismic investigation settings - bending (조사설정 - 휨)
*/
#define T_SIBD_K unsigned int
struct T_SIBD_D
{
	T_SIPA_K kSipa;				//(조사 케이스)내진조사 파라미터 Key
	T_SIGR_K kSigr;				//(조사 대상)내진조사 그룹 Key
	int nSibdType;				//조사 조건 : enum T_SIBD_D::eSibdType
	T_SIBD_SRSV_D SRSV_D;		//내력조사 조건
	T_SIBD_CVSV_D CVSV_D;		//곡률조사 조건

	T_SIBD_D() { Initialize(); }
	T_SIBD_D(const T_SIBD_D& src) { *this = src; }
	T_SIBD_D& operator=(const T_SIBD_D& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			nSibdType = src.nSibdType;
			SRSV_D = src.SRSV_D;
			CVSV_D = src.CVSV_D;
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigr = 0;
		nSibdType = 0;
		SRSV_D.Initialize();
		CVSV_D.Initialize();
	}

	enum eSibdType
	{
		kUnKnown,
		kStrengthSurvey,	//내력조사
		kCurvatureSurvey,	//곡률조사
	};
};

struct T_SIBD_D_CH
{
	T_SIPA_K kSipa;
	T_SIGR_K kSigr;
	int nSibdType;
	T_SIBD_SRSV_D_CH SRSV_D;
	T_SIBD_CVSV_D_CH CVSV_D;

	void Get(T_SIBD_D& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.nSibdType = nSibdType;
		SRSV_D.Get(rData.SRSV_D);
		CVSV_D.Get(rData.CVSV_D);
	}

	void Set(const T_SIBD_D& data)
	{
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		nSibdType = data.nSibdType;
		SRSV_D.Set(data.SRSV_D);
		CVSV_D.Set(data.CVSV_D);
	}
};

struct T_SIBD_UDRD_D
{
	T_SIBD_K key;
	T_SIBD_D data;
};

struct T_SIBD_UDRD_D_CH
{
	T_SIBD_K key;
	T_SIBD_D_CH data;
};

struct T_SIBD_UDRD_D_CH_RW
{
	T_SIBD_K key;
	T_SIPA_K kSipa;
	T_SIGR_K kSigr;
	int nSibdType;
	T_SIBD_SRSV_D_CH_RW SRSV_D;
	T_SIBD_CVSV_D_CH_RW CVSV_D;

	void Get(T_SIBD_K& rKey, T_SIBD_D_CH& rData)
	{
		rKey = key;
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.nSibdType = nSibdType;
		SRSV_D.Get(rData.SRSV_D);
		CVSV_D.Get(rData.CVSV_D);
	}
	void Set(const T_SIBD_K Key, const T_SIBD_D_CH& data)
	{
		key = Key;
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		nSibdType = data.nSibdType;
		SRSV_D.Set(data.SRSV_D);
		CVSV_D.Set(data.CVSV_D);
	}
};

struct T_SIBD_UNIT
{
	T_SIBD_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};

#define HASHSIZESIBD 1001
#define HASHSIZELISTSIBD 11

#pragma pack(pop)

#endif  // __DB_ST_DT_SIBD_H__