#ifndef __DB_ST_DT_SISR_H__
#define __DB_ST_DT_SISR_H__

#define MIDAS_SDK

#pragma pack(push, 1)

// for Civil JP

///////////////////////////////////////////// 조사설정 - 전단 ////////////////////////////////////////////

#define T_SISR_K unsigned int
#define T_SISR_USER_K unsigned int
#pragma region // T_SISR_USER_D

/**
* User Shear Force (유저가 입력한 전단내력 값)
*/
struct T_SISR_USER_SHFO_D
{
	double dSus;//상부구조 전단내력 Sus (사인장파괴), 하부구조의 경우 전단내력 Ps 로 사용
	double dSuc;//상부구조 전단내력 Suc (압축파괴)

	T_SISR_USER_SHFO_D() { }
	T_SISR_USER_SHFO_D(const T_SISR_USER_SHFO_D& src) { *this = src; }
	T_SISR_USER_SHFO_D& operator=(const T_SISR_USER_SHFO_D& src)
	{
		if (this != &src) {
			dSus = src.dSus;
			dSuc = src.dSuc;
		}
		return *this;
	}

	BOOL operator==(const T_SISR_USER_SHFO_D& src) const
	{
		constexpr double EPS = 1e-9;
		return (fabs(dSus - src.dSus) < EPS &&
			fabs(dSuc - src.dSuc) < EPS);
	}

	void Initialize()
	{
		dSus = 0;
		dSuc = 0;
	}
};

struct T_SISR_USER_SHFO_D_CH
{
	double dSus;
	double dSuc;

	void Get(T_SISR_USER_SHFO_D& rData)
	{
		rData.dSus = dSus;
		rData.dSuc = dSuc;
	}

	void Set(const T_SISR_USER_SHFO_D& data)
	{
		dSus = data.dSus;
		dSuc = data.dSuc;
	}
};

struct T_SISR_USER_SHFO_D_CH_RW
{
	double dSus;
	double dSuc;

	void Get(T_SISR_USER_SHFO_D_CH& rData)
	{
		rData.dSus = dSus;
		rData.dSuc = dSuc;
	}

	void Set(const T_SISR_USER_SHFO_D_CH& data)
	{
		dSus = data.dSus;
		dSuc = data.dSuc;
	}
};

/**
* user input (직접입력 조건)
*/
struct T_SISR_USER_D_955
{
	T_SISR_K kSisr;		//전단조사
	T_ELEM_K kElem;		//대상요소
	int nPosType;		//전단내력 위치 : enum T_SISR_USER_D::ePosType
	T_SISR_USER_SHFO_D PLUS_D;	//(+)방향
	T_SISR_USER_SHFO_D MINUS_D;	//(-)방향

	T_SISR_USER_D_955() { }
	T_SISR_USER_D_955(const T_SISR_USER_D_955& src) { *this = src; }
	T_SISR_USER_D_955& operator=(const T_SISR_USER_D_955& src)
	{
		if (this != &src) {
			kSisr = src.kSisr;
			kElem = src.kElem;
			nPosType = src.nPosType;
			PLUS_D = src.PLUS_D;
			MINUS_D = src.MINUS_D;
		}
		return *this;
	}

	void Initialize()
	{
		kSisr = 0;
		kElem = 0;
		nPosType = 0;
		PLUS_D.Initialize();
		MINUS_D.Initialize();
	}

	enum ePosType
	{
		kUnKnown,
		kI,
		kJ,
		kI_J,
	};
};

struct T_SISR_USER_D
{
	T_SISR_K kSisr;		//전단조사
	T_ELEM_K kElem;		//대상요소
	int nPosType;		// 전단내력 위치 : enum T_SISR_USER_D::ePosType -> v965 부터 사용하지 않음.
	T_SISR_USER_SHFO_D I_PLUS_D;	//(+)방향 
	T_SISR_USER_SHFO_D I_MINUS_D;	//(-)방향  

	T_SISR_USER_SHFO_D J_PLUS_D;	//(+)방향 
	T_SISR_USER_SHFO_D J_MINUS_D;	//(-)방향 

	T_SISR_USER_D() { }
	T_SISR_USER_D(const T_SISR_USER_D& src) { *this = src; }
	T_SISR_USER_D& operator=(const T_SISR_USER_D& src)
	{
		if (this != &src) {
			kSisr = src.kSisr;
			kElem = src.kElem;
			nPosType = src.nPosType;
			I_PLUS_D = src.I_PLUS_D;
			I_MINUS_D = src.I_MINUS_D;
			J_PLUS_D = src.J_PLUS_D;
			J_MINUS_D = src.J_MINUS_D;
		}
		return *this;
	}

	void Initialize()
	{
		kSisr = 0;
		kElem = 0;
		nPosType = 0;
		I_PLUS_D.Initialize();
		I_MINUS_D.Initialize();
		J_PLUS_D.Initialize();
		J_MINUS_D.Initialize();
	}
	void Convert955(const T_SISR_USER_D_955& data)
	{
		kSisr = data.kSisr;
		kElem = data.kElem;
		nPosType = data.nPosType;
		if (data.nPosType == T_SISR_USER_D_955::kI_J) {
			I_PLUS_D = data.PLUS_D;
			I_MINUS_D = data.MINUS_D;
			J_PLUS_D = data.PLUS_D;
			J_MINUS_D = data.MINUS_D;
		}
		else {
			if (data.nPosType == T_SISR_USER_D_955::kI || data.nPosType == T_SISR_USER_D_955::kUnKnown) {
				I_PLUS_D = data.PLUS_D;
				I_MINUS_D = data.MINUS_D;
				J_PLUS_D.Initialize();
				J_MINUS_D.Initialize();
			}
			else if (data.nPosType == T_SISR_USER_D_955::kJ) {
				J_PLUS_D = data.PLUS_D;
				J_MINUS_D = data.MINUS_D;
				I_PLUS_D.Initialize();
				I_MINUS_D.Initialize();
			}
		}
	}

	BOOL IsSame() const { return I_PLUS_D == J_PLUS_D && I_MINUS_D == J_MINUS_D; }
	enum ePosType
	{
		kUnKnown,
		kI,
		kJ,
		kI_J,
	};
};

// CH
struct T_SISR_USER_D_CH_955
{
	T_SISR_K kSisr;
	T_ELEM_K kElem;
	int nPosType;
	T_SISR_USER_SHFO_D_CH PLUS_D;
	T_SISR_USER_SHFO_D_CH MINUS_D;

	void Get(T_SISR_USER_D_955& rData)
	{
		rData.kSisr = kSisr;
		rData.kElem = kElem;
		rData.nPosType = nPosType;
		PLUS_D.Get(rData.PLUS_D);
		MINUS_D.Get(rData.MINUS_D);
	}

	void Set(const T_SISR_USER_D_955& data)
	{
		kSisr = data.kSisr;
		kElem = data.kElem;
		nPosType = data.nPosType;
		PLUS_D.Set(data.PLUS_D);
		MINUS_D.Set(data.MINUS_D);
	}
};

struct T_SISR_USER_D_CH
{
	T_SISR_K kSisr;		//전단조사
	T_ELEM_K kElem;		//대상요소
	int nPosType;		// 전단내력 위치 : enum T_SISR_USER_D::ePosType -> 955
	T_SISR_USER_SHFO_D_CH I_PLUS_D;	//(+)방향 
	T_SISR_USER_SHFO_D_CH I_MINUS_D;	//(-)방향  

	T_SISR_USER_SHFO_D_CH J_PLUS_D;	//(+)방향 
	T_SISR_USER_SHFO_D_CH J_MINUS_D;	//(-)방향 

	void Get(T_SISR_USER_D& rData)
	{
		rData.kSisr = kSisr;
		rData.kElem = kElem;
		rData.nPosType = nPosType;
		I_PLUS_D.Get(rData.I_PLUS_D);
		I_MINUS_D.Get(rData.I_MINUS_D);
		J_PLUS_D.Get(rData.J_PLUS_D);
		J_MINUS_D.Get(rData.J_MINUS_D);
	}

	void Set(const T_SISR_USER_D& data)
	{
		kSisr = data.kSisr;
		kElem = data.kElem;
		nPosType = data.nPosType;
		I_PLUS_D.Set(data.I_PLUS_D);
		I_MINUS_D.Set(data.I_MINUS_D);
		J_PLUS_D.Set(data.J_PLUS_D);
		J_MINUS_D.Set(data.J_MINUS_D);
	}
};;

// UDRD
struct T_SISR_USER_UDRD_D_955
{
	T_SISR_USER_K key;
	T_SISR_USER_D_955 data;
};

struct T_SISR_USER_UDRD_D_CH_955
{
	T_SISR_USER_K key;
	T_SISR_USER_D_CH_955 data;
};

struct T_SISR_USER_UDRD_D_CH_RW_955
{
	T_SISR_USER_K key;
	T_SISR_K kSisr;
	T_ELEM_K kElem;
	int nPosType;
	T_SISR_USER_SHFO_D_CH_RW PLUS_D;
	T_SISR_USER_SHFO_D_CH_RW MINUS_D;

	void Get(T_SISR_USER_K& rKey, T_SISR_USER_D_CH_955& rData)
	{
		rKey = key;
		rData.kSisr = kSisr;
		rData.kElem = kElem;
		rData.nPosType = nPosType;
		PLUS_D.Get(rData.PLUS_D);
		MINUS_D.Get(rData.MINUS_D);
	}

	void Set(const T_SISR_USER_K Key, const T_SISR_USER_D_CH_955& data)
	{
		key = Key;
		kSisr = data.kSisr;
		kElem = data.kElem;
		nPosType = data.nPosType;
		PLUS_D.Set(data.PLUS_D);
		MINUS_D.Set(data.MINUS_D);
	}
};

struct T_SISR_USER_UDRD_D
{
	T_SISR_USER_K key;
	T_SISR_USER_D data;
};

struct T_SISR_USER_UDRD_D_CH
{
	T_SISR_USER_K key;
	T_SISR_USER_D_CH data;
};

struct T_SISR_USER_UDRD_D_CH_RW
{
	T_SISR_USER_K key;
	T_SISR_K kSisr;
	T_ELEM_K kElem;
	int nPosType;		// 전단내력 위치 : enum T_SISR_USER_D::ePosType -> 955
	T_SISR_USER_SHFO_D_CH_RW I_PLUS_D;	//(+)방향 
	T_SISR_USER_SHFO_D_CH_RW I_MINUS_D;	//(-)방향  

	T_SISR_USER_SHFO_D_CH_RW J_PLUS_D;	//(+)방향 
	T_SISR_USER_SHFO_D_CH_RW J_MINUS_D;	//(-)방향 

	void Get(T_SISR_USER_K& rKey, T_SISR_USER_D_CH& rData)
	{
		rKey = key;
		rData.kSisr = kSisr;
		rData.kElem = kElem;
		rData.nPosType = nPosType;
		I_PLUS_D.Get(rData.I_PLUS_D);
		I_MINUS_D.Get(rData.I_MINUS_D);
		J_PLUS_D.Get(rData.J_PLUS_D);
		J_MINUS_D.Get(rData.J_MINUS_D);
	}

	void Set(const T_SISR_USER_K Key, const T_SISR_USER_D_CH& data)
	{
		key = Key;
		kSisr = data.kSisr;
		kElem = data.kElem;
		nPosType = data.nPosType;
		I_PLUS_D.Set(data.I_PLUS_D);
		I_MINUS_D.Set(data.I_MINUS_D);
		J_PLUS_D.Set(data.J_PLUS_D);
		J_MINUS_D.Set(data.J_MINUS_D);
	}
};

struct T_SISR_USER_UNIT
{
	T_SISR_USER_UNIT()
	{
		Force = D_UNITSYS_BASE_FORCE;
	}
	int Force;
};

#define HASHSIZESISR_USER 1001
#define HASHSIZELISTSISR_USER 11
#pragma endregion

#pragma region // T_SISR_AUTO_D
/**
* automatic calculation (자동계산 조건)
*/
struct T_SISR_AUTO_D
{
	int		nDesignCriteria;	//적용기준 : enum T_SISR_AUTO_D::eDesignCriteria
	BOOL	bSigmaCe;			//σce (프로스트레스에 의한 응력)를 별도로 고려
	BOOL	bExtraByShearSpan;	//전단 스팬비에 의한 할증 적용여부
	int		nShearSpanType;		//전단 스팬 산정방식 : enum T_SISR_AUTO_D::eShearSpanType
	double	dUserLength;		//전단 스팬 유저입력 길이
	BOOL	bApplyCc;			//Cc=1.0 적용여부
	int		nTargetElements;	//Cc=1.0 적용요소 : enum T_SISR_AUTO_D::eTargetElements
	CArray<T_ELEM_K, T_ELEM_K> aCcElements;//Cc=1.0 적용 대상요소(nTargetElements == eTargetElements::kEachElement 일때)

	T_SISR_AUTO_D() { }
	T_SISR_AUTO_D(const T_SISR_AUTO_D& src) { *this = src; }
	T_SISR_AUTO_D& operator=(const T_SISR_AUTO_D& src)
	{
		if (this != &src) {
			nDesignCriteria = src.nDesignCriteria;
			bSigmaCe = src.bSigmaCe;
			bExtraByShearSpan = src.bExtraByShearSpan;
			nShearSpanType = src.nShearSpanType;
			dUserLength = src.dUserLength;
			bApplyCc = src.bApplyCc;
			nTargetElements = src.nTargetElements;
			aCcElements.Copy(src.aCcElements);
		}
		return *this;
	}

	void Initialize()
	{
		nDesignCriteria = 0;
		bSigmaCe = FALSE;
		bExtraByShearSpan = FALSE;
		nShearSpanType = 0;
		dUserLength = 0;
		bApplyCc = FALSE;
		nTargetElements = 0;
		aCcElements.RemoveAll();
	}

	enum eDesignCriteria
	{
		kRoad0,
		kRoad3,	//도로교 시방서-Ⅲ
		kRoad4,	//도로교 시방서-Ⅳ
		kRoad5,	//도로교 시방서-V
	};

	enum eShearSpanType
	{
		kUnKnown,
		kColumnLength,		//기둥부 길이
		kLocInertialForce,	//상부공관성력 작용위치
		kUserInput,			//직접 입력
	};

	enum eTargetElements
	{
		kUnKnownTarget,
		kWholeGroup,		//그룹전체
		kEachElement,		//개별요소
	};
};

struct T_SISR_AUTO_D_CH
{
	int		nDesignCriteria;
	BOOL	bSigmaCe;
	BOOL	bExtraByShearSpan;
	int		nShearSpanType;
	double	dUserLength;
	BOOL	bApplyCc;
	int		nTargetElements;
	CArray<T_ELEM_K, T_ELEM_K> aCcElements;

	void Get(T_SISR_AUTO_D& rData)
	{
		rData.nDesignCriteria = nDesignCriteria;
		rData.bSigmaCe = bSigmaCe;
		rData.bExtraByShearSpan = bExtraByShearSpan;
		rData.nShearSpanType = nShearSpanType;
		rData.dUserLength = dUserLength;
		rData.bApplyCc = bApplyCc;
		rData.nTargetElements = nTargetElements;
		rData.aCcElements.Copy(aCcElements);
	}

	void Set(const T_SISR_AUTO_D& data)
	{
		nDesignCriteria = data.nDesignCriteria;
		bSigmaCe = data.bSigmaCe;
		bExtraByShearSpan = data.bExtraByShearSpan;
		nShearSpanType = data.nShearSpanType;
		dUserLength = data.dUserLength;
		bApplyCc = data.bApplyCc;
		nTargetElements = data.nTargetElements;
		aCcElements.Copy(data.aCcElements);
	}
};

struct T_SISR_AUTO_D_CH_RW
{
	int		nDesignCriteria;
	BOOL	bSigmaCe;
	BOOL	bExtraByShearSpan;
	int		nShearSpanType;
	double	dUserLength;
	BOOL	bApplyCc;
	int		nTargetElements;
	int		nCountElem;

	void Get(T_SISR_AUTO_D_CH& rData)
	{
		rData.nDesignCriteria = nDesignCriteria;
		rData.bSigmaCe = bSigmaCe;
		rData.bExtraByShearSpan = bExtraByShearSpan;
		rData.nShearSpanType = nShearSpanType;
		rData.dUserLength = dUserLength;
		rData.bApplyCc = bApplyCc;
		rData.nTargetElements = nTargetElements;
	}

	void Set(const T_SISR_AUTO_D_CH& data)
	{
		nDesignCriteria = data.nDesignCriteria;
		bSigmaCe = data.bSigmaCe;
		bExtraByShearSpan = data.bExtraByShearSpan;
		nShearSpanType = data.nShearSpanType;
		dUserLength = data.dUserLength;
		bApplyCc = data.bApplyCc;
		nTargetElements = data.nTargetElements;
		nCountElem = (int)data.aCcElements.GetSize();
	}
};
#pragma endregion

#pragma region // T_SISR_D
/**
* Seismic investigation settings - shearing (조사설정 - 전단)
*/
struct T_SISR_D
{
	T_SIPA_K kSipa;				//(조사 케이스)내진조사 파라미터 Key
	T_SIGR_K kSigr;				//(조사 대상)내진조사 그룹 Key
	T_SISR_AUTO_D AUTO_D;		//자동계산 조건

	T_SISR_D() { Initialize(); }
	T_SISR_D(const T_SISR_D& src) { *this = src; }
	T_SISR_D& operator=(const T_SISR_D& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			AUTO_D = src.AUTO_D;
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigr = 0;
		AUTO_D.Initialize();
	}
};

struct T_SISR_D_CH
{
	T_SIPA_K kSipa;
	T_SIGR_K kSigr;
	T_SISR_AUTO_D_CH AUTO_D;

	void Get(T_SISR_D& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		AUTO_D.Get(rData.AUTO_D);
	}

	void Set(const T_SISR_D& data)
	{
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		AUTO_D.Set(data.AUTO_D);
	}
};

struct T_SISR_D_CH_RW
{
	T_SISR_K key;
	T_SIPA_K kSipa;
	T_SIGR_K kSigr;
	T_SISR_AUTO_D_CH_RW AUTO_D;

	void Get(T_SISR_K& rKey, T_SISR_D_CH& rData)
	{
		rKey = key;
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		AUTO_D.Get(rData.AUTO_D);
	}
	void Set(const T_SISR_K Key, const T_SISR_D_CH& data)
	{
		key = Key;
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		AUTO_D.Set(data.AUTO_D);
	}
};

struct T_SISR_UDRD_D
{
	T_SISR_K key;
	T_SISR_D data;
};

struct T_SISR_UDRD_D_CH
{
	T_SISR_K key;
	T_SISR_D_CH data;
};

struct T_SISR_UNIT
{
	T_SISR_UNIT()
	{
		Length = D_UNITSYS_BASE_LENGTH;
	}
	int Length;
};

#define HASHSIZESISR 1001
#define HASHSIZELISTSISR 11
#pragma endregion

#pragma pack(pop)

#endif  // __DB_ST_DT_SISR_H__