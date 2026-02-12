#ifndef __DB_ST_DT_EVAL_KEYS__
#define __DB_ST_DT_EVAL_KEYS__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SIMD_EVAL_KEY unsigned int
union SIMD_EVAL_K
{
	SIMD_EVAL_KEY keyMap;
	struct
	{
		T_SIPA_K kSipa : 15;// T_SIPA_K (0~32767)
		T_SIGR_K kSigr : 15;// T_SIGR_K (0~32767)
		int nPosType   : 2;	//enum T_SIMD_EVAL::ePosType (0~3)
	}Key;

	SIMD_EVAL_K()
	{
		keyMap = 0;
	}
	SIMD_EVAL_K(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr, const int& nPosType)
	{
		Key.kSipa = kSipa;
		Key.kSigr = kSigr;
		Key.nPosType = nPosType;
	}
	void SetKey(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr, const int& nPosType)
	{
		Key.kSipa = kSipa;
		Key.kSigr = kSigr;
		Key.nPosType = nPosType;
	}
};

#define SIRD_EVAL_KEY unsigned int
union SIRD_EVAL_K
{
	SIMD_EVAL_KEY keyMap;
	struct
	{
		T_SIPA_K kSipa : 15;// T_SIPA_K (0~32767)
		T_SIGR_K kSigr : 15;// T_SIGR_K (0~32767)
		int nPosType : 2;	//enum T_SIRD_EVAL::ePosType (0~3)
	}Key;

	SIRD_EVAL_K()
	{
		keyMap = 0;
	}
	SIRD_EVAL_K(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr, const int& nPosType)
	{
		Key.kSipa = kSipa;
		Key.kSigr = kSigr;
		Key.nPosType = nPosType;
	}
	void SetKey(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr, const int& nPosType)
	{
		Key.kSipa = kSipa;
		Key.kSigr = kSigr;
		Key.nPosType = nPosType;
	}
};

#define SIBD_EVAL_KEY unsigned int
union SIBD_EVAL_K
{
	SIBD_EVAL_KEY keyMap;
	struct
	{
		T_SIPA_K kSipa : 4;	// T_SIPA_K (0~15)
		T_SIGR_K kSigr : 8;	// T_SIGR_K (0~255)
		T_ELEM_K kElem : 18;// T_ELEM_K (0~262,143)
		int nExamShearDir : 2; // (0~3)
	}Key;

	SIBD_EVAL_K()
	{
		keyMap = 0;
	}
	SIBD_EVAL_K(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr, const T_ELEM_K& kElem, const int& nDir)
	{
		Key.kSipa = kSipa;
		Key.kSigr = kSigr;
		Key.kElem = kElem;
		Key.nExamShearDir = nDir;
	}
	void SetKey(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr, const T_ELEM_K& kElem, const int& nDir)
	{
		Key.kSipa = kSipa;
		Key.kSigr = kSigr;
		Key.kElem = kElem;
		Key.nExamShearDir = nDir;
	}
};

#define SIDP_EVAL_KEY unsigned int
union SIDP_EVAL_K
{
	SIDP_EVAL_KEY keyMap;
	struct
	{
		T_SIPA_K kSipa : 16;// T_SIPA_K (0~65535)
		T_SIGR_K kSigr : 16;// T_SIGR_K (0~65535)
	}Key;

	SIDP_EVAL_K()
	{
		keyMap = 0;
	}
	SIDP_EVAL_K(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr)
	{
		Key.kSipa = kSipa;
		Key.kSigr = kSigr;
	}
	void SetKey(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr)
	{
		Key.kSipa = kSipa;
		Key.kSigr = kSigr;
	}
};

#define SISR_EVAL_KEY unsigned int
union SISR_EVAL_K
{
	SISR_EVAL_KEY keyMap;
	struct
	{
		T_SIPA_K kSipa : 4;	// T_SIPA_K (0~15)
		T_SIGR_K kSigr : 8;	// T_SIGR_K (0~255)
		T_ELEM_K kElem : 18;// T_ELEM_K (0~262,143)
		int nExamShearDir : 2; // (0~3)
	}Key;

	SISR_EVAL_K()
	{
		keyMap = 0;
	}
	SISR_EVAL_K(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr, const T_ELEM_K& kElem, const int& nDir)
	{
		Key.kSipa = kSipa;
		Key.kSigr = kSigr;
		Key.kElem = kElem;
		Key.nExamShearDir = nDir;
	}
	void SetKey(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr, const T_ELEM_K& kElem, const int& nDir)
	{
		Key.kSipa = kSipa;
		Key.kSigr = kSigr;
		Key.kElem = kElem;
		Key.nExamShearDir = nDir;
	}
};

#define SIET_EVAL_KEY unsigned int
union SIET_EVAL_K
{
	SIET_EVAL_KEY keyMap;
	struct
	{
		T_SIPA_K kSipa : 16;// T_SIPA_K (0~65535)
		T_SIGR_K kSigr : 16;// T_SIGR_K (0~65535)
	}Key;

	SIET_EVAL_K()
	{
		keyMap = 0;
	}
	SIET_EVAL_K(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr)
	{
		Key.kSipa = kSipa;
		Key.kSigr = kSigr;
	}
	void SetKey(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr)
	{
		Key.kSipa = kSipa;
		Key.kSigr = kSigr;
	}
};

#define SIET_EVAL_FOOT_KEY unsigned int
union SIET_EVAL_FOOT_K
{
	SIET_EVAL_KEY keyMap;
	struct
	{
		T_SIPA_K kSipa : 4;	// T_SIPA_K (0~15)
		T_SIGR_K kSigr : 8;	// T_SIGR_K (0~255)
		T_ELEM_K kElem : 20;// T_ELEM_K (0~1,048,575)
	}Key;

	SIET_EVAL_FOOT_K()
	{
		keyMap = 0;
	}
	SIET_EVAL_FOOT_K(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr, const T_ELEM_K& kElem)
	{
		Key.kSipa = kSipa;
		Key.kSigr = kSigr;
		Key.kElem = kElem;
	}
	void SetKey(const T_SIPA_K& kSipa, const T_SIGR_K& kSigr, const T_ELEM_K& kElem)
	{
		Key.kSipa = kSipa;
		Key.kSigr = kSigr;
		Key.kElem = kElem;
	}
};

#endif // !defined(__DB_ST_DT_EVAL_KEYS__)