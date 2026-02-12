#ifndef __DB_ST_DT_SIPA_H__
#define __DB_ST_DT_SIPA_H__

#define MIDAS_SDK

#pragma pack(push, 1)

// Seismic investigation Code for Civil JP
#define T_SIDC_K unsigned int
struct T_SIDC_D
{
	UINT nDesignCode; // enum BrdgSE_CodeJP

	void Initialize()
	{
		nDesignCode = BrdgSE_CodeJP::JROAD5_H14;
	}

	T_SIDC_D() { Initialize(); }
	T_SIDC_D(const T_SIDC_D& src) { *this = src; }
	T_SIDC_D& operator=(const T_SIDC_D& src)
	{
		if (this != &src) {
			nDesignCode = src.nDesignCode;
		}
		return *this;
	}

};
struct T_SIDC_UDRD_D
{
	T_SIDC_K key;
	T_SIDC_D data;
};
struct T_SIDC_UNIT
{
	T_SIDC_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;
	}
	int nNoUnit;
};
#define HASHSIZESIDC 1
#define HASHSIZELISTSIDC 11

/**
* (T_SIPA_D) Seismic investigation Paramater (내진조사 파라미터)
*/
#define T_SIPA_K unsigned int
struct T_SIPA_D_960
{
	CString strCaseName;
	int nDesignCode;
	int nSesmicPerformanceType;
	int nEarthquakeType;
	int nLimitType;
	int nSeismicForceDirection;
	int nSeismicForceType;
	T_THIS_K_LIST aSeisThisK;

	void Initialize()
	{
		strCaseName.Empty();
		nDesignCode = 0;
		nSesmicPerformanceType = 0;
		nEarthquakeType = 0;
		nLimitType = 0;
		nSeismicForceDirection = 0;
		nSeismicForceType = 0;
		aSeisThisK.RemoveAll();
	}
};

struct T_SIPA_D
{
	CString strCaseName;		//내진조사 케이스명(중복허용X)
	int nDesignCode;			//설계코드 T_SIDC_D::nDesignCode 와 같은 값
	int nSesmicPerformanceType; //(도시-H14/H24" 의 경우만) 교량의 내진 성능 : enum T_SIPA_D::eSesmicPerformanceType
	int nEarthquakeType;		//(도시-H29" 의 경우만) 작용의 종류 : enum T_SIPA_D::eEarthquakeType
	int nLimitType;				//(도시-H29" 의 경우만) 한계상태타입 : enum T_SIPA_D::eLimitStateType
	int nSeismicForceDirection; //지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	T_THIS_K_LIST aSeisThisK;	//동적하중 케이스
	int nExamTarget;			//검토대상 : T_MPHG_D::eExamTarget v965추가

	void Initialize()
	{
		strCaseName.Empty();
		nDesignCode = 0;
		nSesmicPerformanceType = 0;
		nEarthquakeType = 0;
		nLimitType = 0;
		nSeismicForceDirection = 0;
		nSeismicForceType = 0;
		aSeisThisK.RemoveAll();
		nExamTarget = 0;
	}

	T_SIPA_D() { Initialize(); }
	T_SIPA_D(const T_SIPA_D& src) { *this = src; }
	T_SIPA_D& operator=(const T_SIPA_D& src)
	{
		if (this != &src) {
			strCaseName = src.strCaseName;
			nDesignCode = src.nDesignCode;
			nSesmicPerformanceType = src.nSesmicPerformanceType;
			nEarthquakeType = src.nEarthquakeType;
			nLimitType = src.nLimitType;
			nSeismicForceDirection = src.nSeismicForceDirection;
			nSeismicForceType = src.nSeismicForceType;
			aSeisThisK.Copy(src.aSeisThisK);
			nExamTarget = src.nExamTarget;
		}
		return *this;
	}

	void Convert960(T_SIPA_D_960 rData)
	{
		strCaseName = rData.strCaseName;
		nDesignCode = rData.nDesignCode;
		nSesmicPerformanceType = rData.nSesmicPerformanceType;
		nEarthquakeType = rData.nEarthquakeType;
		nLimitType = rData.nLimitType;
		nSeismicForceDirection = rData.nSeismicForceDirection;
		nSeismicForceType = rData.nSeismicForceType;
		aSeisThisK.Copy(rData.aSeisThisK);
		nExamTarget = eExamTarget::kExistingSection;
	
	}

	enum eSesmicPerformanceType
	{
		kLevel0,
		kLevel1,	//내진성능1
		kLevel2,	//내진성능2
		kLevel3,	//내진성능3
	};

	enum eEarthquakeType
	{
		kUnKnown,
		kFluctuating_No10,	//변동작용-No.10
		kAccidental_No11,	//우발작용-No.11
	};

	enum eLimitStateType
	{
		kLimitState0,
		kLimitState1,	//한계상태1
		kLimitState2,	//한계상태2
		kLimitState3,	//한계상태3
	};

	enum eSeismicForceDirection
	{
		kUnKnown_Dir,
		kAxis_Dir,	//교축방향
		kPerp_Dir,	//교축직각방향
	};

	enum eSeismicForceType
	{
		kType0,
		kType1,			//타입1
		kType2,			//타입2
	};

	enum eExamTarget
	{
		kExistingSection,//기설단면
		kReinforceSection,//기설+보강 단면
	};
};

struct T_SIPA_D_CH
{
	char strCaseName[40];
	int nDesignCode;
	int nSesmicPerformanceType;
	int nEarthquakeType;
	int nLimitType;
	int nSeismicForceDirection;
	int nSeismicForceType;
	CArray<T_THIS_K, T_THIS_K> aSeisThisK;
	int nExamTarget;

	void Get(T_SIPA_D& rData)
	{
		ConvertCharStr(strCaseName, rData.strCaseName, sizeof(strCaseName));
		rData.nDesignCode = nDesignCode;
		rData.nSesmicPerformanceType = nSesmicPerformanceType;
		rData.nEarthquakeType = nEarthquakeType;
		rData.nLimitType = nLimitType;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		rData.nSeismicForceType = nSeismicForceType;
		rData.aSeisThisK.Copy(aSeisThisK);
		rData.nExamTarget = nExamTarget;
	}

	void Set(/*const*/ T_SIPA_D& data)
	{
		ConvertStrChar(data.strCaseName, strCaseName, sizeof(strCaseName));
		nDesignCode = data.nDesignCode;
		nSesmicPerformanceType = data.nSesmicPerformanceType;
		nEarthquakeType = data.nEarthquakeType;
		nLimitType = data.nLimitType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		nSeismicForceType = data.nSeismicForceType;
		aSeisThisK.Copy(data.aSeisThisK);
		nExamTarget = data.nExamTarget;
	}
};

struct T_SIPA_D_CH_RW_960
{
	T_SIPA_K key;
	int nDesignCode;
	char strCaseName[40];
	int nSesmicPerformanceType;
	int nEarthquakeType;
	int nLimitType;
	int nSeismicForceDirection;
	int nSeismicForceType;
	int nCountSeisThisK;
};

struct T_SIPA_D_CH_RW
{
	T_SIPA_K key;
	char strCaseName[40];
	int nDesignCode;
	int nSesmicPerformanceType;
	int nEarthquakeType;
	int nLimitType;
	int nSeismicForceDirection;
	int nSeismicForceType;
	int nCountSeisThisK;
	int nExamTarget;

	void Get(T_SIPA_K& rKey, T_SIPA_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.strCaseName, strCaseName, sizeof(strCaseName));
		rData.nDesignCode = nDesignCode;
		rData.nSesmicPerformanceType = nSesmicPerformanceType;
		rData.nEarthquakeType = nEarthquakeType;
		rData.nLimitType = nLimitType;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nExamTarget = nExamTarget;
	}
	void Set(const T_SIPA_K Key, const T_SIPA_D_CH& data)
	{
		key = Key;
		memcpy(strCaseName, data.strCaseName, sizeof(strCaseName));
		nDesignCode = data.nDesignCode;
		nSesmicPerformanceType = data.nSesmicPerformanceType;
		nEarthquakeType = data.nEarthquakeType;
		nLimitType = data.nLimitType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		nSeismicForceType = data.nSeismicForceType;
		nCountSeisThisK = (int)data.aSeisThisK.GetSize();
		nExamTarget = data.nExamTarget;
	}

	void Convert960(T_SIPA_D_CH_RW_960& rData)
	{
		key = rData.key;
		memcpy(strCaseName, rData.strCaseName, sizeof(strCaseName));
		nDesignCode = rData.nDesignCode;
		nSesmicPerformanceType = rData.nSesmicPerformanceType;
		nEarthquakeType = rData.nEarthquakeType;
		nLimitType = rData.nLimitType;
		nSeismicForceDirection = rData.nSeismicForceDirection;
		nSeismicForceType = rData.nSeismicForceType;
		nCountSeisThisK = rData.nCountSeisThisK;
		nExamTarget = T_SIPA_D::eExamTarget::kExistingSection;
	}
};

struct T_SIPA_UDRD_D_960
{
	T_SIPA_K key;
	T_SIPA_D_960 data;
};

struct T_SIPA_UDRD_D
{
	T_SIPA_K key;
	T_SIPA_D data;
};

struct T_SIPA_UNIT
{
	T_SIPA_UNIT()
	{
		NoUnit = D_UNITSYS_NONE;
	}
	int NoUnit;
};

#define HASHSIZESIPA 1001
#define HASHSIZELISTSIPA 11

#pragma pack(pop)

#endif  // __DB_ST_DT_SIPA_H__