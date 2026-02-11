#ifndef __DB_ST_DT_SIDP_EVAL__
#define __DB_ST_DT_SIDP_EVAL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

struct T_SIDP_EVAL_BY_CALC
{
	double dDelta_y;
	double dDelta_u_ls;
	double dMu_a;

	T_SIDP_EVAL_BY_CALC() { Initialize(); }
	T_SIDP_EVAL_BY_CALC(const T_SIDP_EVAL_BY_CALC& src) { *this = src; }
	T_SIDP_EVAL_BY_CALC& operator= (const T_SIDP_EVAL_BY_CALC& src)
	{
		if (this != &src) {
			dDelta_y = src.dDelta_y;
			dDelta_u_ls = src.dDelta_u_ls;
			dMu_a = src.dMu_a;
		}
		return *this;
	}

	T_SIDP_EVAL_BY_CALC& operator += (const T_SIDP_EVAL_BY_CALC& src)
	{
		if (this != &src) {
			dDelta_y += abs(src.dDelta_y);
			dDelta_u_ls += abs(src.dDelta_u_ls);
			dMu_a += abs(src.dMu_a);
		}
		return *this;
	}

	T_SIDP_EVAL_BY_CALC& operator /= (const double& den)
	{
		dDelta_y = dgn::SafeDiv(dDelta_y, den);
		dDelta_u_ls = dgn::SafeDiv(dDelta_u_ls, den);
		dMu_a = dgn::SafeDiv(dMu_a, den);

		return *this;
	}

	void Initialize()
	{
		dDelta_y = 0;
		dDelta_u_ls = 0;
		dMu_a = 0;
	}
};

struct T_SIDP_EVAL_BY_ANAL
{
	double dDeltaMax;
	double dMuR;
	double dRatio;

	T_SIDP_EVAL_BY_ANAL() { Initialize(); }
	T_SIDP_EVAL_BY_ANAL(const T_SIDP_EVAL_BY_ANAL& src) { *this = src; }
	T_SIDP_EVAL_BY_ANAL& operator= (const T_SIDP_EVAL_BY_ANAL& src)
	{
		if (this != &src) {
			dDeltaMax = src.dDeltaMax;
			dMuR = src.dMuR;
			dRatio = src.dRatio;
		}
		return *this;
	}

	T_SIDP_EVAL_BY_ANAL& operator += (const T_SIDP_EVAL_BY_ANAL& src)
	{
		if (this != &src) {
			dDeltaMax += abs(src.dDeltaMax);
			dMuR += abs(src.dMuR);
			dRatio += abs(src.dRatio);
		}
		return *this;
	}

	T_SIDP_EVAL_BY_ANAL& operator /= (const double& den)
	{
		dDeltaMax = dgn::SafeDiv(dDeltaMax, den);
		dMuR = dgn::SafeDiv(dMuR, den);
		dRatio = dgn::SafeDiv(dRatio, den);

		return *this;
	}

	void Initialize()
	{
		dDeltaMax = 0;
		dMuR = 0;
		dRatio = 0;
	}
};

//교각에 대한 변위소성율 조사 (H14/24 공통)
struct T_SIDP_EVAL
{
	T_SIDP_K kSidp;
	T_SIPA_K kSipa;
	T_SIGR_LOWER_K kSigrLower;
	T_NODE_K kNode;
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nDirection;				//해석방향 enum T_SIDP_EVAL::eDirection
	int nSeismicForceDirection;	//지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	CArray<T_SIDP_EVAL_BY_CALC, T_SIDP_EVAL_BY_CALC> aValByCalc;
	CArray<T_SIDP_EVAL_BY_ANAL, T_SIDP_EVAL_BY_ANAL> aValByAnal;
	T_SIDP_EVAL_BY_CALC AvergeCalc;
	T_SIDP_EVAL_BY_ANAL AvergeAnal;
	BOOL bCheckResult;

	T_SIDP_EVAL() { Initialize(); }
	T_SIDP_EVAL(const T_SIDP_EVAL& src) { *this = src; }
	T_SIDP_EVAL& operator= (const T_SIDP_EVAL& src)
	{
		if (this != &src) {
			kSidp = src.kSidp;
			kSipa = src.kSipa;
			kSigrLower = src.kSigrLower;
			kNode = src.kNode;
			nSeismicForceType = src.nSeismicForceType;
			nDirection = src.nDirection;
			nSeismicForceDirection = src.nSeismicForceDirection;
			aValByCalc.Copy(src.aValByCalc);
			aValByAnal.Copy(src.aValByAnal);
			AvergeCalc = src.AvergeCalc;
			AvergeAnal = src.AvergeAnal;
			bCheckResult = src.bCheckResult;
		}
		return *this;
	}

	void Initialize()
	{
		kSidp = 0;
		kSipa = 0;
		kSigrLower = 0;
		kNode = 0;
		nSeismicForceType = 0;
		nDirection = 0;
		nSeismicForceDirection = 0;
		aValByCalc.RemoveAll();
		aValByAnal.RemoveAll();
		AvergeCalc.Initialize();
		AvergeAnal.Initialize();
		bCheckResult = FALSE;
	}

	enum eDirection
	{
		kPlus,
		kMinus,
		kDirMax,
	};
};

struct T_SIDP_EVAL_RW
{
	T_SIDP_K kSidp;
	T_SIPA_K kSipa;
	T_SIGR_LOWER_K kSigrLower;
	T_NODE_K kNode;
	int nSeismicForceType;
	int nDirection;
	int nSeismicForceDirection;
	int nValByCalc;
	int nValByAnal;
	T_SIDP_EVAL_BY_CALC AvergeCalc;
	T_SIDP_EVAL_BY_ANAL AvergeAnal;
	BOOL bCheckResult;

	T_SIDP_EVAL_RW() { Initialize(); }
	T_SIDP_EVAL_RW(const T_SIDP_EVAL_RW& src) { *this = src; }
	T_SIDP_EVAL_RW& operator= (const T_SIDP_EVAL_RW& src)
	{
		if (this != &src) {
			kSidp = src.kSidp;
			kSipa = src.kSipa;
			kSigrLower = src.kSigrLower;
			kNode = src.kNode;
			nSeismicForceType = src.nSeismicForceType;
			nDirection = src.nDirection;
			nSeismicForceDirection = src.nSeismicForceDirection;
			nValByCalc = src.nValByCalc;
			nValByAnal = src.nValByAnal;
			AvergeCalc = src.AvergeCalc;
			AvergeAnal = src.AvergeAnal;
			bCheckResult = src.bCheckResult;
		}
		return *this;
	}

	void Initialize()
	{
		kSidp = 0;
		kSipa = 0;
		kSigrLower = 0;
		kNode = 0;
		nSeismicForceType = 0;
		nDirection = 0;
		nSeismicForceDirection = 0;
		nValByCalc = 0;
		nValByAnal = 0;
		AvergeCalc.Initialize();
		AvergeAnal.Initialize();
		bCheckResult = FALSE;
	}

	void Get(T_SIDP_EVAL& rData)
	{
		rData.kSidp = kSidp;
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		rData.kNode = kNode;
		rData.nDirection = nDirection;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		rData.nSeismicForceType = nSeismicForceType;
		rData.AvergeCalc = AvergeCalc;
		rData.AvergeAnal = AvergeAnal;
		rData.bCheckResult = bCheckResult;
	}

	void Set(const T_SIDP_EVAL& data)
	{
		kSidp = data.kSidp;
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		kNode = data.kNode;
		nDirection = data.nDirection;
		nSeismicForceDirection = data.nSeismicForceDirection;
		nSeismicForceType = data.nSeismicForceType;
		nValByCalc = (int)data.aValByCalc.GetSize();
		nValByAnal = (int)data.aValByAnal.GetSize();
		AvergeCalc = data.AvergeCalc;
		AvergeAnal = data.AvergeAnal;
		bCheckResult = data.bCheckResult;
	}
};

#endif // !defined(__DB_ST_DT_SIDP_EVAL__)