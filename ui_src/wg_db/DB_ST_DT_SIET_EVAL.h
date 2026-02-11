#ifndef __DB_ST_DT_SIET_EVAL__
#define __DB_ST_DT_SIET_EVAL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "..\..\dgnengine\idesign\DGN_lib\DGNCompare.h"

#pragma region 하부구조 관련부분

struct T_SIET_EVAL_BEAM_FORCE
{
public:
	double dFx;		//축력
	double dFy;		//전단y
	double dFz;		//전단z
	double dMx;		//뒤틀림
	double dMy;		//모멘트y
	double dMz;		//모멘트z

	T_SIET_EVAL_BEAM_FORCE() { Initialize(); }
	T_SIET_EVAL_BEAM_FORCE(const T_SIET_EVAL_BEAM_FORCE& src) { *this = src; }
	T_SIET_EVAL_BEAM_FORCE& operator = (const T_SIET_EVAL_BEAM_FORCE& src)
	{
		if (this != &src) {
			dFx = src.dFx;
			dFy = src.dFy;
			dFz = src.dFz;
			dMx = src.dMx;
			dMy = src.dMy;
			dMz = src.dMz;
		}
		return *this;
	}

	T_SIET_EVAL_BEAM_FORCE& operator += (const T_SIET_EVAL_BEAM_FORCE& src)
	{
		if (this != &src) {
			dFx += src.dFx;
			dFy += src.dFy;
			dFz += src.dFz;
			dMx += src.dMx;
			dMy += src.dMy;
			dMz += src.dMz;
		}
		return *this;
	}

	T_SIET_EVAL_BEAM_FORCE& operator /= (const double& den)
	{
		dFx = dgn::SafeDiv(dFx, den);
		dFy = dgn::SafeDiv(dFy, den);
		dFz = dgn::SafeDiv(dFz, den);
		dMx = dgn::SafeDiv(dMx, den);
		dMy = dgn::SafeDiv(dMy, den);
		dMz = dgn::SafeDiv(dMz, den);

		return *this;
	}

	void Initialize()
	{
		dFx = 0.0;
		dFy = 0.0;
		dFz = 0.0;
		dMx = 0.0;
		dMy = 0.0;
		dMz = 0.0;
	}
};

#pragma region 교각에 대한 기타 조사 - 단면력 성분별 최대 최소값
struct T_SIET_EVAL_MEMB_GROUP
{
public:
	enum eLocation
	{
		kUnkouwn,
		kPierBase,
		kFootingBot,
	};

	enum ePosNodeLow
	{
		kUnKnown,
		kI,
		kJ,
	};

	int nLocation;					//위치 : enum T_SIET_EVAL_PIER_MINMAX::eLocation
	CString strName;				//그룹이름
	UINT kElem;						//그룹안에서 요소 중 대상요소
	int ePosNodeLow;				//최하 요소의 아래 절점 : enum T_SIET_EVAL_MEMB_GROUP::ePosNodeLow
	T_SIET_EVAL_BEAM_FORCE max;
	T_SIET_EVAL_BEAM_FORCE min;

	T_SIET_EVAL_MEMB_GROUP() { Initialize(); }
	T_SIET_EVAL_MEMB_GROUP(const T_SIET_EVAL_MEMB_GROUP& src) { *this = src; }
	T_SIET_EVAL_MEMB_GROUP& operator= (const T_SIET_EVAL_MEMB_GROUP& src)
	{
		if (this != &src) {
			nLocation = src.nLocation;
			strName = src.strName;
			kElem = src.kElem;
			ePosNodeLow = src.ePosNodeLow;
			max = src.max;
			min = src.min;
		}
		return *this;
	}

	void Initialize()
	{
		nLocation = 0;
		strName.Empty();
		kElem = 0;
		ePosNodeLow = 0;
		max.Initialize();
		min.Initialize();
	}
};

struct T_SIET_EVAL_MEMB_GROUP_CH
{
	int nLocation;
	char strName[60];
	UINT kElem;
	int ePosNodeLow;
	T_SIET_EVAL_BEAM_FORCE max;
	T_SIET_EVAL_BEAM_FORCE min;

	void Get(T_SIET_EVAL_MEMB_GROUP& rData)
	{
		rData.nLocation = nLocation;
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.kElem = kElem;
		rData.ePosNodeLow = ePosNodeLow;
		rData.max = max;
		rData.min = min;
	}

	void Set(/*const*/T_SIET_EVAL_MEMB_GROUP& data)
	{
		nLocation = data.nLocation;
		ConvertStrChar(data.strName, strName, sizeof(strName));
		kElem = data.kElem;
		ePosNodeLow = data.ePosNodeLow;
		max = data.max;
		min = data.min;
	}
};

//기타 조사 - 교각에 대한 단면력 성분별 최대 최소값
struct T_SIET_EVAL_MEMB_MINMAX
{
	UINT kSipa;
	UINT kSigrLower;
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nSeismicForceDirection;	//지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	CArray<T_SIET_EVAL_MEMB_GROUP, T_SIET_EVAL_MEMB_GROUP> aGroup;

	T_SIET_EVAL_MEMB_MINMAX() { Initialize(); }
	T_SIET_EVAL_MEMB_MINMAX(const T_SIET_EVAL_MEMB_MINMAX& src) { *this = src; }
	T_SIET_EVAL_MEMB_MINMAX& operator= (const T_SIET_EVAL_MEMB_MINMAX& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigrLower = src.kSigrLower;
			nSeismicForceType = src.nSeismicForceType;
			nSeismicForceDirection = src.nSeismicForceDirection;
			aGroup.Copy(src.aGroup);
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigrLower = 0;
		nSeismicForceType = 0;
		nSeismicForceDirection = 0;
		aGroup.RemoveAll();
	}
};

struct T_SIET_EVAL_MEMB_MINMAX_CH
{
	UINT kSipa;
	UINT kSigrLower;
	int nSeismicForceType;
	int nSeismicForceDirection;
	CArray<T_SIET_EVAL_MEMB_GROUP_CH, T_SIET_EVAL_MEMB_GROUP_CH> aGroup;

	void Get(T_SIET_EVAL_MEMB_MINMAX& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		rData.aGroup.SetSize(aGroup.GetSize());
		for (int i = 0; i < aGroup.GetCount(); i++)
			aGroup[i].Get(rData.aGroup[i]);
	}

	void Set(/*const*/T_SIET_EVAL_MEMB_MINMAX& data)
	{
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		nSeismicForceType = data.nSeismicForceType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		aGroup.SetSize(data.aGroup.GetSize());
		for (int i = 0; i < data.aGroup.GetCount(); i++)
			aGroup[i].Set(data.aGroup[i]);
	}
};

struct T_SIET_EVAL_MEMB_MINMAX_CH_RW
{
	UINT kSipa;
	UINT kSigrLower;
	int nSeismicForceType;
	int nSeismicForceDirection;
	int nGroup;

	T_SIET_EVAL_MEMB_MINMAX_CH_RW() { Initialize(); }
	T_SIET_EVAL_MEMB_MINMAX_CH_RW(const T_SIET_EVAL_MEMB_MINMAX_CH_RW& src) { *this = src; }
	T_SIET_EVAL_MEMB_MINMAX_CH_RW& operator= (const T_SIET_EVAL_MEMB_MINMAX_CH_RW& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigrLower = src.kSigrLower;
			nSeismicForceType = src.nSeismicForceType;
			nSeismicForceDirection = src.nSeismicForceDirection;
			nGroup = src.nGroup;
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigrLower = 0;
		nSeismicForceType = 0;
		nSeismicForceDirection = 0;
		nGroup = 0;
	}

	void Get(T_SIET_EVAL_MEMB_MINMAX_CH& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nSeismicForceDirection = nSeismicForceDirection;
	}

	void Set(const T_SIET_EVAL_MEMB_MINMAX_CH& data)
	{
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		nSeismicForceType = data.nSeismicForceType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		nGroup = (int)data.aGroup.GetSize();
	}
};
#pragma endregion

#pragma region 교각/기초에 대한 기타 조사 - 단면력 성분별 동시성(각각)
struct T_SIET_EVAL_MEMB_CONCU
{
public:
	enum eForceType
	{
		kAxial,
		kShearY,
		kShearZ,
		kTorsion,
		kMomentY,
		kMomentZ,
		kForceNum,
	};

	UINT kElem;
	int ePosNodeLow;			//최하 요소의 아래 절점 : enum T_SIET_EVAL_MEMB_GROUP::ePosNodeLow
	CString strName;			//기둥이름
	T_SIET_EVAL_BEAM_FORCE max[T_SIET_EVAL_MEMB_CONCU::kForceNum];
	T_SIET_EVAL_BEAM_FORCE min[T_SIET_EVAL_MEMB_CONCU::kForceNum];

	T_SIET_EVAL_MEMB_CONCU() { Initialize(); }
	T_SIET_EVAL_MEMB_CONCU(const T_SIET_EVAL_MEMB_CONCU& src) { *this = src; }
	T_SIET_EVAL_MEMB_CONCU& operator= (const T_SIET_EVAL_MEMB_CONCU& src)
	{
		if (this != &src) {
			kElem = src.kElem;
			ePosNodeLow = src.ePosNodeLow;
			strName = src.strName;
			for (int i = 0; i < T_SIET_EVAL_MEMB_CONCU::kForceNum; ++i) {
				max[i] = src.max[i];
				min[i] = src.min[i];
			}
		}
		return *this;
	}

	void Initialize()
	{
		kElem = 0;
		ePosNodeLow = 0;
		strName.Empty();
		for (int i = 0; i < T_SIET_EVAL_MEMB_CONCU::kForceNum; ++i) {
			max[i].Initialize();
			min[i].Initialize();
		}
	}
};

struct T_SIET_EVAL_MEMB_CONCU_CH
{
	UINT kElem;
	int ePosNodeLow;
	char strName[60];
	T_SIET_EVAL_BEAM_FORCE max[T_SIET_EVAL_MEMB_CONCU::kForceNum];
	T_SIET_EVAL_BEAM_FORCE min[T_SIET_EVAL_MEMB_CONCU::kForceNum];

	void Get(T_SIET_EVAL_MEMB_CONCU& rData)
	{
		rData.kElem = kElem;
		rData.ePosNodeLow = ePosNodeLow;
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		for (int i = 0; i < T_SIET_EVAL_MEMB_CONCU::kForceNum; ++i) {
			rData.max[i] = max[i];
			rData.min[i] = min[i];
		}
	}

	void Set(/*const*/T_SIET_EVAL_MEMB_CONCU& data)
	{
		kElem = data.kElem;
		ePosNodeLow = data.ePosNodeLow;
		ConvertStrChar(data.strName, strName, sizeof(strName));
		for (int i = 0; i < T_SIET_EVAL_MEMB_CONCU::kForceNum; ++i) {
			max[i] = data.max[i];
			min[i] = data.min[i];
		}
	}
};

//기타 조사 - 교각/기초에 대한 단면력 성분별 동시성(각각)
struct T_SIET_EVAL_MEMB_EACH
{
	UINT kSipa;
	UINT kSigrLower;
	int  nSeismicForceType;			//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int  nSeismicForceDirection;	//지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	CArray<T_SIET_EVAL_MEMB_CONCU, T_SIET_EVAL_MEMB_CONCU> aGroup;

	T_SIET_EVAL_MEMB_EACH() { Initialize(); }
	T_SIET_EVAL_MEMB_EACH(const T_SIET_EVAL_MEMB_EACH& src) { *this = src; }
	T_SIET_EVAL_MEMB_EACH& operator= (const T_SIET_EVAL_MEMB_EACH& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigrLower = src.kSigrLower;
			nSeismicForceType = src.nSeismicForceType;
			nSeismicForceDirection = src.nSeismicForceDirection;
			aGroup.Copy(src.aGroup);
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigrLower = 0;
		nSeismicForceType = 0;
		nSeismicForceDirection = 0;
		aGroup.RemoveAll();
	}
};

struct T_SIET_EVAL_MEMB_EACH_CH
{
	UINT kSipa;
	UINT kSigrLower;
	int  nSeismicForceType;
	int  nSeismicForceDirection;
	CArray<T_SIET_EVAL_MEMB_CONCU_CH, T_SIET_EVAL_MEMB_CONCU_CH> aGroup;

	void Get(T_SIET_EVAL_MEMB_EACH& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		rData.aGroup.SetSize(aGroup.GetSize());
		for (int i = 0; i < aGroup.GetCount(); ++i) {
			aGroup[i].Get(rData.aGroup[i]);
		}
	}

	void Set(/*const*/T_SIET_EVAL_MEMB_EACH& data)
	{
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		nSeismicForceType = data.nSeismicForceType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		aGroup.SetSize(data.aGroup.GetSize());
		for (int i = 0; i < data.aGroup.GetCount(); ++i) {
			aGroup[i].Set(data.aGroup[i]);
		}
	}
};

struct T_SIET_EVAL_MEMB_EACH_CH_RW
{
	UINT kSipa;
	UINT kSigrLower;
	int  nSeismicForceType;
	int  nSeismicForceDirection;
	int  nGroup;

	void Get(T_SIET_EVAL_MEMB_EACH_CH& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nSeismicForceDirection = nSeismicForceDirection;
	}

	void Set(/*const*/T_SIET_EVAL_MEMB_EACH_CH& data)
	{
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		nSeismicForceType = data.nSeismicForceType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		nGroup = data.aGroup.GetCount();
	}
};
#pragma endregion

#pragma region 교각/기초에 대한 기타 조사 - 단면력 성분별 동시성(기초하단기준)
struct T_SIET_EVAL_FOOT_GROUP
{
public:
	enum eLocation
	{
		kUnkouwn,
		kPierBase,
		kFootingBot,
	};

	enum ePosNodeLow
	{
		kUnKnown,
		kI,
		kJ,
	};

	int nLocation;					//위치 : enum T_SIET_EVAL_FOOT_GROUP::eLocation
	CString strName;				//그룹이름
	UINT kElem;						//그룹안에서 요소 중 대상요소
	int ePosNodeLow;				//최하 요소의 아래 절점 : enum T_SIET_EVAL_FOOT_GROUP::ePosNodeLow
	T_SIET_EVAL_BEAM_FORCE Force;

	T_SIET_EVAL_FOOT_GROUP() { Initialize(); }
	T_SIET_EVAL_FOOT_GROUP(const T_SIET_EVAL_FOOT_GROUP& src) { *this = src; }
	T_SIET_EVAL_FOOT_GROUP& operator= (const T_SIET_EVAL_FOOT_GROUP& src)
	{
		if (this != &src) {
			nLocation = src.nLocation;
			strName = src.strName;
			kElem = src.kElem;
			ePosNodeLow = src.ePosNodeLow;
			Force = src.Force;
		}
		return *this;
	}

	T_SIET_EVAL_FOOT_GROUP& operator += (const T_SIET_EVAL_FOOT_GROUP& src)
	{
		if (this != &src) {
			Force.dFx += src.Force.dFx;
			Force.dFy += src.Force.dFy;
			Force.dFz += src.Force.dFz;
			Force.dMx += src.Force.dMx;
			Force.dMy += src.Force.dMy;
			Force.dMz += src.Force.dMz;
		}
		return *this;
	}

	T_SIET_EVAL_FOOT_GROUP& operator /= (const double& den)
	{
		Force.dFx = dgn::SafeDiv(Force.dFx, den);
		Force.dFy = dgn::SafeDiv(Force.dFy, den);
		Force.dFz = dgn::SafeDiv(Force.dFz, den);
		Force.dMx = dgn::SafeDiv(Force.dMx, den);
		Force.dMy = dgn::SafeDiv(Force.dMy, den);
		Force.dMz = dgn::SafeDiv(Force.dMz, den);

		return *this;
	}

	void Initialize()
	{
		nLocation = 0;
		strName.Empty();
		kElem = 0;
		ePosNodeLow = 0;
		Force.Initialize();
	}
};

struct T_SIET_EVAL_FOOT_GROUP_CH
{
	int nLocation;
	char strName[60];
	UINT kElem;
	int ePosNodeLow;
	T_SIET_EVAL_BEAM_FORCE Force;

	void Get(T_SIET_EVAL_FOOT_GROUP& rData)
	{
		rData.nLocation = nLocation;
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.kElem = kElem;
		rData.ePosNodeLow = ePosNodeLow;
		rData.Force = Force;
	}

	void Set(/*const*/ T_SIET_EVAL_FOOT_GROUP& data)
	{
		nLocation = data.nLocation;
		ConvertStrChar(data.strName, strName, sizeof(strName));
		kElem = data.kElem;
		ePosNodeLow = data.ePosNodeLow;
		Force = data.Force;
	}
};

struct T_SIET_EVAL_FOOT_CONCU
{
	CArray<T_SIET_EVAL_FOOT_GROUP, T_SIET_EVAL_FOOT_GROUP> aMax;
	CArray<T_SIET_EVAL_FOOT_GROUP, T_SIET_EVAL_FOOT_GROUP> aMin;

	T_SIET_EVAL_FOOT_CONCU() { Initialize(); }
	T_SIET_EVAL_FOOT_CONCU(const T_SIET_EVAL_FOOT_CONCU& src) { *this = src; }
	T_SIET_EVAL_FOOT_CONCU& operator= (const T_SIET_EVAL_FOOT_CONCU& src)
	{
		if (this != &src) {
			aMax.Copy(src.aMax);
			aMin.Copy(src.aMin);
		}
		return *this;
	}

	T_SIET_EVAL_FOOT_CONCU& operator += (const T_SIET_EVAL_FOOT_CONCU& src)
	{
		for (int i = 0; i < src.aMax.GetSize(); ++i) {
			if (aMax.GetCount() < i + 1)
				aMax.Add(src.aMax[i]);
			else
				aMax[i].Force += src.aMax[i].Force;
		}
		for (int i = 0; i < src.aMin.GetSize(); ++i) {
			if (aMin.GetCount() < i + 1)
				aMin.Add(src.aMin[i]);
			else
				aMin[i].Force += src.aMin[i].Force;
		}
		return *this;
	}

	T_SIET_EVAL_FOOT_CONCU& operator /= (const double& den)
	{
		for (int i = 0; i < aMax.GetSize(); ++i) {
			aMax[i] /= den;
		}
		for (int i = 0; i < aMin.GetSize(); ++i) {
			aMin[i] /= den;
		}
		return *this;
	}

	void Initialize()
	{
		aMax.RemoveAll();
		aMin.RemoveAll();
	}
};

struct T_SIET_EVAL_FOOT_CONCU_CH
{
	CArray<T_SIET_EVAL_FOOT_GROUP_CH, T_SIET_EVAL_FOOT_GROUP_CH> aMax;
	CArray<T_SIET_EVAL_FOOT_GROUP_CH, T_SIET_EVAL_FOOT_GROUP_CH> aMin;

	void Get(T_SIET_EVAL_FOOT_CONCU& rData)
	{
		rData.aMax.SetSize(aMax.GetSize());
		for (int i = 0; i < aMax.GetSize(); ++i) {
			aMax[i].Get(rData.aMax[i]);
		}
		rData.aMin.SetSize(aMin.GetSize());
		for (int i = 0; i < aMin.GetSize(); ++i) {
			aMin[i].Get(rData.aMin[i]);
		}
	}

	void Set(/*const*/ T_SIET_EVAL_FOOT_CONCU& data)
	{
		aMax.SetSize(data.aMax.GetSize());
		for (int i = 0; i < data.aMax.GetSize(); ++i) {
			aMax[i].Set(data.aMax[i]);
		}
		aMin.SetSize(data.aMin.GetSize());
		for (int i = 0; i < data.aMin.GetSize(); ++i) {
			aMin[i].Set(data.aMin[i]);
		}
	}
};

struct T_SIET_EVAL_FOOT_CONCU_CH_RW
{
	int nMax;
	int nMin;

	void Get(T_SIET_EVAL_FOOT_CONCU_CH& rData)
	{
	}

	void Set(const T_SIET_EVAL_FOOT_CONCU_CH& data)
	{
		nMax = (int)data.aMax.GetSize();
		nMin = (int)data.aMin.GetSize();
	}
};

//기타 조사 - 교각/기초에 대한 단면력 성분별 동시성(기초하단기준)
struct T_SIET_EVAL_FOOT_MASTER
{
public:
	enum eForceType
	{
		kAxial,
		kShearY,
		kShearZ,
		kTorsion,
		kMomentY,
		kMomentZ,
		kForceNum,
	};

	UINT kElem;
	UINT kSipa;
	UINT kSigrLower;
	int  nSeismicForceType;			//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int  nSeismicForceDirection;	//지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	T_SIET_EVAL_FOOT_CONCU FootConcu[T_SIET_EVAL_FOOT_MASTER::kForceNum];

	T_SIET_EVAL_FOOT_MASTER() { Initialize(); }
	T_SIET_EVAL_FOOT_MASTER(const T_SIET_EVAL_FOOT_MASTER& src) { *this = src; }
	T_SIET_EVAL_FOOT_MASTER& operator= (const T_SIET_EVAL_FOOT_MASTER& src)
	{
		if (this != &src) {
			kElem = src.kElem;
			kSipa = src.kSipa;
			kSigrLower = src.kSigrLower;
			nSeismicForceType = src.nSeismicForceType;
			nSeismicForceDirection = src.nSeismicForceDirection;
			for (int i = 0; i < kForceNum; ++i) {
				FootConcu[i] = src.FootConcu[i];
			}
		}
		return *this;
	}

	void Initialize()
	{
		kElem = 0;
		kSipa = 0;
		kSigrLower = 0;
		nSeismicForceType = 0;
		nSeismicForceDirection = 0;
		for (int i = 0; i < kForceNum; ++i) {
			FootConcu[i].Initialize();
		}
	}
};

struct T_SIET_EVAL_FOOT_MASTER_CH
{
	UINT kElem;
	UINT kSipa;
	UINT kSigrLower;
	int  nSeismicForceType;
	int  nSeismicForceDirection;
	T_SIET_EVAL_FOOT_CONCU_CH FootConcu[T_SIET_EVAL_FOOT_MASTER::kForceNum];

	void Get(T_SIET_EVAL_FOOT_MASTER& rData)
	{
		rData.kElem = kElem;
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		for (int i = 0; i < T_SIET_EVAL_FOOT_MASTER::kForceNum; ++i) {
			FootConcu[i].Get(rData.FootConcu[i]);
		}
	}

	void Set(/*const*/T_SIET_EVAL_FOOT_MASTER& data)
	{
		kElem = data.kElem;
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		nSeismicForceType = data.nSeismicForceType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		for (int i = 0; i < T_SIET_EVAL_FOOT_MASTER::kForceNum; ++i) {
			FootConcu[i].Set(data.FootConcu[i]);
		}
	}
};

struct T_SIET_EVAL_FOOT_MASTER_CH_RW
{
	UINT kElem;
	UINT kSipa;
	UINT kSigrLower;
	int  nSeismicForceType;
	int  nSeismicForceDirection;
	T_SIET_EVAL_FOOT_CONCU_CH_RW FootConcu[T_SIET_EVAL_FOOT_MASTER::kForceNum];

	void Get(T_SIET_EVAL_FOOT_MASTER_CH& rData)
	{
		rData.kElem = kElem;
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		for (int i = 0; i < T_SIET_EVAL_FOOT_MASTER::kForceNum; ++i) {
			FootConcu[i].Get(rData.FootConcu[i]);
		}
	}

	void Set(const T_SIET_EVAL_FOOT_MASTER_CH& data)
	{
		kElem = data.kElem;
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		nSeismicForceType = data.nSeismicForceType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		for (int i = 0; i < T_SIET_EVAL_FOOT_MASTER::kForceNum; ++i) {
			FootConcu[i].Set(data.FootConcu[i]);
		}
	}
};

#pragma endregion

#pragma region 지승과 관련된 부분

struct T_SIET_EVAL_NLNK_FORCE
{
	double dHmax;
	double dVmax;
	double dUmax;

	T_SIET_EVAL_NLNK_FORCE() { Initialize(); }
	T_SIET_EVAL_NLNK_FORCE(const T_SIET_EVAL_NLNK_FORCE& src) { *this = src; }
	T_SIET_EVAL_NLNK_FORCE& operator= (const T_SIET_EVAL_NLNK_FORCE& src)
	{
		if (this != &src) {
			dHmax = src.dHmax;
			dVmax = src.dVmax;
			dUmax = src.dUmax;
		}
		return *this;
	}

	T_SIET_EVAL_NLNK_FORCE& operator += (const T_SIET_EVAL_NLNK_FORCE& src)
	{
		if (this != &src) {
			dHmax += src.dHmax;
			dVmax += src.dVmax;
			dUmax += src.dUmax;
		}
		return *this;
	}

	T_SIET_EVAL_NLNK_FORCE& operator /= (const double& den)
	{
		dHmax = dgn::SafeDiv(dHmax, den);
		dVmax = dgn::SafeDiv(dVmax, den);
		dUmax = dgn::SafeDiv(dUmax, den);

		return *this;
	}

	void Initialize()
	{
		dHmax = 0;
		dVmax = 0;
		dUmax = 0;
	}
};

#pragma region 지승에 대한 기타 조사 - 단면력 성분별 최대 최소값
struct T_SIET_EVAL_BEAR_GROUP
{
	CString strName;				//지승이름
	UINT kNode_i;					//절점번호
	UINT kNode_j;
	int nType;						//지승 조건 : enum T_SIGR_LOWER_BEAR_D::eBearingType;
	int nDirShear;					//전단의 방향 : enum T_SIGR_LOWER_BEAR_D::eDirection;
	CString strComp;				//동시성의 착목성분
	T_SIET_EVAL_NLNK_FORCE Force;

	T_SIET_EVAL_BEAR_GROUP() { Initialize(); }
	T_SIET_EVAL_BEAR_GROUP(const T_SIET_EVAL_BEAR_GROUP& src) { *this = src; }
	T_SIET_EVAL_BEAR_GROUP& operator= (const T_SIET_EVAL_BEAR_GROUP& src)
	{
		if (this != &src) {
			strName = src.strName;
			kNode_i = src.kNode_i;
			kNode_j = src.kNode_j;
			nType = src.nType;
			nDirShear = src.nDirShear;
			strComp = src.strComp;
			Force = src.Force;
		}
		return *this;
	}

	void Initialize()
	{
		strName.Empty();
		kNode_i = 0;
		kNode_j = 0;
		nType = 0;
		nDirShear = 0;
		strComp.Empty();
		Force.Initialize();
	}
};

struct T_SIET_EVAL_BEAR_GROUP_CH
{
	char strName[60];
	UINT kNode_i;
	UINT kNode_j;
	int nType;
	int nDirShear;
	char strComp[60];
	T_SIET_EVAL_NLNK_FORCE Force;

	void Get(T_SIET_EVAL_BEAR_GROUP& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.kNode_i = kNode_i;
		rData.kNode_j = kNode_j;
		rData.nType = nType;
		rData.nDirShear = nDirShear;
		ConvertCharStr(strComp, rData.strComp, sizeof(strComp));
		rData.Force = Force;
	}

	void Set(/*const*/ T_SIET_EVAL_BEAR_GROUP& data)
	{
		ConvertStrChar(data.strName, strName, sizeof(strName));
		kNode_i = data.kNode_i;
		kNode_j = data.kNode_j;
		nType = data.nType;
		nDirShear = data.nDirShear;
		ConvertStrChar(data.strComp, strComp, sizeof(strComp));
		Force = data.Force;
	}
};

struct T_SIET_EVAL_BEAR_LINE
{
	int nLineNum;						//지승선 : enum T_SIGR_LOWER_BEAR_D::eBearingLineNum;
	T_SIET_EVAL_NLNK_FORCE SumForce;
	CArray<T_SIET_EVAL_BEAR_GROUP, T_SIET_EVAL_BEAR_GROUP> aGroup;

	T_SIET_EVAL_BEAR_LINE() { Initialize(); }
	T_SIET_EVAL_BEAR_LINE(const T_SIET_EVAL_BEAR_LINE& src) { *this = src; }
	T_SIET_EVAL_BEAR_LINE& operator= (const T_SIET_EVAL_BEAR_LINE& src)
	{
		if (this != &src) {
			nLineNum = src.nLineNum;
			SumForce = src.SumForce;
			aGroup.Copy(src.aGroup);
		}
		return *this;
	}

	void Initialize()
	{
		nLineNum = 0;
		SumForce.Initialize();
		aGroup.RemoveAll();
	}
};

struct T_SIET_EVAL_BEAR_LINE_CH
{
	int nLineNum;
	T_SIET_EVAL_NLNK_FORCE SumForce;
	CArray<T_SIET_EVAL_BEAR_GROUP_CH, T_SIET_EVAL_BEAR_GROUP_CH> aGroup;

	T_SIET_EVAL_BEAR_LINE_CH() { Initialize(); }
	T_SIET_EVAL_BEAR_LINE_CH(const T_SIET_EVAL_BEAR_LINE_CH& src) { *this = src; }
	T_SIET_EVAL_BEAR_LINE_CH& operator= (const T_SIET_EVAL_BEAR_LINE_CH& src)
	{
		if (this != &src) {
			nLineNum = src.nLineNum;
			SumForce = src.SumForce;
			aGroup.Copy(src.aGroup);
		}
		return *this;
	}

	void Initialize()
	{
		nLineNum = 0;
		SumForce.Initialize();
		aGroup.RemoveAll();
	}

	void Get(T_SIET_EVAL_BEAR_LINE& rData)
	{
		rData.nLineNum = nLineNum;
		rData.SumForce = SumForce;
	}

	void Set(/*const*/T_SIET_EVAL_BEAR_LINE& data)
	{
		nLineNum = data.nLineNum;
		SumForce = data.SumForce;
		aGroup.SetSize(data.aGroup.GetSize());
		for (int i = 0; i < data.aGroup.GetCount(); i++)
			aGroup[i].Set(data.aGroup[i]);
	}
};

struct T_SIET_EVAL_BEAR_LINE_CH_RW
{
	int nLineNum;
	T_SIET_EVAL_NLNK_FORCE SumForce;
	int nGroup;

	T_SIET_EVAL_BEAR_LINE_CH_RW() { Initialize(); }
	T_SIET_EVAL_BEAR_LINE_CH_RW(const T_SIET_EVAL_BEAR_LINE_CH_RW& src) { *this = src; }
	T_SIET_EVAL_BEAR_LINE_CH_RW& operator= (const T_SIET_EVAL_BEAR_LINE_CH_RW& src)
	{
		if (this != &src) {
			nLineNum = src.nLineNum;
			SumForce = src.SumForce;
			nGroup = src.nGroup;
		}
		return *this;
	}

	void Initialize()
	{
		nLineNum = 0;
		SumForce.Initialize();
		nGroup = 0;
	}

	void Get(T_SIET_EVAL_BEAR_LINE_CH& rData)
	{
		rData.nLineNum = nLineNum;
		rData.SumForce = SumForce;
	}

	void Set(const T_SIET_EVAL_BEAR_LINE_CH& data)
	{
		nLineNum = data.nLineNum;
		SumForce = data.SumForce;
		nGroup = (int)data.aGroup.GetSize();
	}
};

//기타 조사 - 지승에 대한 단면력 성분별 최대 최소값
struct T_SIET_EVAL_BEAR_MINMAX
{
	UINT kSipa;
	UINT kSigrLower;
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nSeismicForceDirection;	//지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	T_SIET_EVAL_BEAR_LINE Line1;
	T_SIET_EVAL_BEAR_LINE Line2;

	T_SIET_EVAL_BEAR_MINMAX() { Initialize(); }
	T_SIET_EVAL_BEAR_MINMAX(const T_SIET_EVAL_BEAR_MINMAX& src) { *this = src; }
	T_SIET_EVAL_BEAR_MINMAX& operator= (const T_SIET_EVAL_BEAR_MINMAX& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigrLower = src.kSigrLower;
			nSeismicForceType = src.nSeismicForceType;
			nSeismicForceDirection = src.nSeismicForceDirection;
			Line1 = src.Line1;
			Line2 = src.Line2;
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigrLower = 0;
		nSeismicForceType = 0;
		nSeismicForceDirection = 0;
		Line1.Initialize();
		Line2.Initialize();
	}
};

struct T_SIET_EVAL_BEAR_MINMAX_CH
{
	UINT kSipa;
	UINT kSigrLower;
	int nSeismicForceType;
	int nSeismicForceDirection;
	T_SIET_EVAL_BEAR_LINE_CH Line1;
	T_SIET_EVAL_BEAR_LINE_CH Line2;

	void Get(T_SIET_EVAL_BEAR_MINMAX& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		Line1.Get(rData.Line1);
		Line2.Get(rData.Line2);
	}

	void Set(/*const*/T_SIET_EVAL_BEAR_MINMAX& data)
	{
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		nSeismicForceType = data.nSeismicForceType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		Line1.Set(data.Line1);
		Line2.Set(data.Line2);
	}
};

struct T_SIET_EVAL_BEAR_MINMAX_CH_RW
{
	UINT kSipa;
	UINT kSigrLower;
	int nSeismicForceType;
	int nSeismicForceDirection;
	T_SIET_EVAL_BEAR_LINE_CH_RW Line1;
	T_SIET_EVAL_BEAR_LINE_CH_RW Line2;

	T_SIET_EVAL_BEAR_MINMAX_CH_RW() { Initialize(); }
	T_SIET_EVAL_BEAR_MINMAX_CH_RW(const T_SIET_EVAL_BEAR_MINMAX_CH_RW& src) { *this = src; }
	T_SIET_EVAL_BEAR_MINMAX_CH_RW& operator= (const T_SIET_EVAL_BEAR_MINMAX_CH_RW& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigrLower = src.kSigrLower;
			nSeismicForceType = src.nSeismicForceType;
			nSeismicForceDirection = src.nSeismicForceDirection;
			Line1 = src.Line1;
			Line2 = src.Line2;
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigrLower = 0;
		nSeismicForceType = 0;
		nSeismicForceDirection = 0;
		Line1.Initialize();
		Line2.Initialize();
	}

	void Get(T_SIET_EVAL_BEAR_MINMAX_CH& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		Line1.Get(rData.Line1);
		Line2.Get(rData.Line2);
	}

	void Set(const T_SIET_EVAL_BEAR_MINMAX_CH& data)
	{
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		nSeismicForceType = data.nSeismicForceType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		Line1.Set(data.Line1);
		Line2.Set(data.Line2);
	}
};
#pragma endregion

#pragma region 지승에 대한 기타 조사 - 단면력 성분별 동시성(각각)
struct T_SIET_EVAL_BEAR_GROUP_CONCU
{
	CString strName;				//지승이름
	UINT kNode_i;					//절점번호
	UINT kNode_j;
	int nType;						//지승 조건 : enum T_SIGR_LOWER_BEAR_D::eBearingType;
	int nDirShear;					//전단의 방향 : enum T_SIGR_LOWER_BEAR_D::eDirection;
	T_SIET_EVAL_NLNK_FORCE max;
	T_SIET_EVAL_NLNK_FORCE min;

	T_SIET_EVAL_BEAR_GROUP_CONCU() { Initialize(); }
	T_SIET_EVAL_BEAR_GROUP_CONCU(const T_SIET_EVAL_BEAR_GROUP_CONCU& src) { *this = src; }
	T_SIET_EVAL_BEAR_GROUP_CONCU& operator= (const T_SIET_EVAL_BEAR_GROUP_CONCU& src)
	{
		if (this != &src) {
			strName = src.strName;
			kNode_i = src.kNode_i;
			kNode_j = src.kNode_j;
			nType = src.nType;
			nDirShear = src.nDirShear;
			max = src.max;
			min = src.min;
		}
		return *this;
	}

	void Initialize()
	{
		strName = _T("");
		kNode_i = 0;
		kNode_j = 0;
		nType = 0;
		nDirShear = 0;
		max.Initialize();
		min.Initialize();
	}
};

struct T_SIET_EVAL_BEAR_GROUP_CONCU_CH
{
	char strName[60];
	UINT kNode_i;
	UINT kNode_j;
	int nType;
	int nDirShear;
	T_SIET_EVAL_NLNK_FORCE max;
	T_SIET_EVAL_NLNK_FORCE min;

	void Get(T_SIET_EVAL_BEAR_GROUP_CONCU& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.kNode_i = kNode_i;
		rData.kNode_j = kNode_j;
		rData.nType = nType;
		rData.nDirShear = nDirShear;
		rData.max = max;
		rData.min = min;
	}

	void Set(/*const*/ T_SIET_EVAL_BEAR_GROUP_CONCU& data)
	{
		ConvertStrChar(data.strName, strName, sizeof(strName));
		kNode_i = data.kNode_i;
		kNode_j = data.kNode_j;
		nType = data.nType;
		nDirShear = data.nDirShear;
		max = data.max;
		min = data.min;
	}
};

struct T_SIET_EVAL_BEAR_LINE_CONCU
{
	int nLineNum;
	CArray<T_SIET_EVAL_BEAR_GROUP_CONCU, T_SIET_EVAL_BEAR_GROUP_CONCU> aGroup;

	T_SIET_EVAL_BEAR_LINE_CONCU() { Initialize(); }
	T_SIET_EVAL_BEAR_LINE_CONCU(const T_SIET_EVAL_BEAR_LINE_CONCU& src) { *this = src; }
	T_SIET_EVAL_BEAR_LINE_CONCU& operator= (const T_SIET_EVAL_BEAR_LINE_CONCU& src)
	{
		if (this != &src) {
			nLineNum = src.nLineNum;
			aGroup.Copy(src.aGroup);
		}
		return *this;
	}

	void Initialize()
	{
		nLineNum = 0;
		aGroup.RemoveAll();
	}
};

struct T_SIET_EVAL_BEAR_LINE_CONCU_CH
{
	int nLineNum;
	CArray<T_SIET_EVAL_BEAR_GROUP_CONCU_CH, T_SIET_EVAL_BEAR_GROUP_CONCU_CH> aGroup;

	void Get(T_SIET_EVAL_BEAR_LINE_CONCU& rData)
	{
		rData.nLineNum = nLineNum;
	}

	void Set(/*const*/T_SIET_EVAL_BEAR_LINE_CONCU& data)
	{
		nLineNum = data.nLineNum;
		aGroup.SetSize(data.aGroup.GetSize());
		for (int i = 0; i < data.aGroup.GetCount(); i++)
			aGroup[i].Set(data.aGroup[i]);
	}
};

struct T_SIET_EVAL_BEAR_LINE_CONCU_CH_RW
{
	int nLineNum;
	int nGroup;

	T_SIET_EVAL_BEAR_LINE_CONCU_CH_RW() { Initialize(); }
	T_SIET_EVAL_BEAR_LINE_CONCU_CH_RW(const T_SIET_EVAL_BEAR_LINE_CONCU_CH_RW& src) { *this = src; }
	T_SIET_EVAL_BEAR_LINE_CONCU_CH_RW& operator= (const T_SIET_EVAL_BEAR_LINE_CONCU_CH_RW& src)
	{
		if (this != &src) {
			nLineNum = src.nLineNum;
			nGroup = src.nGroup;
		}
		return *this;
	}

	void Initialize()
	{
		nLineNum = 0;
		nGroup = 0;
	}

	void Get(T_SIET_EVAL_BEAR_LINE_CONCU_CH& rData)
	{
		rData.nLineNum = nLineNum;
	}

	void Set(const T_SIET_EVAL_BEAR_LINE_CONCU_CH& data)
	{
		nLineNum = data.nLineNum;
		nGroup = (int)data.aGroup.GetSize();
	}
};

//기타 조사 - 지승에 대한 단면력 성분별 동시성(각각)
struct T_SIET_EVAL_BEAR_EACH
{
	UINT kSipa;
	UINT kSigrLower;
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nSeismicForceDirection;	//지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	T_SIET_EVAL_BEAR_LINE_CONCU Line1;
	T_SIET_EVAL_BEAR_LINE_CONCU Line2;

	T_SIET_EVAL_BEAR_EACH() { Initialize(); }
	T_SIET_EVAL_BEAR_EACH(const T_SIET_EVAL_BEAR_EACH& src) { *this = src; }
	T_SIET_EVAL_BEAR_EACH& operator= (const T_SIET_EVAL_BEAR_EACH& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigrLower = src.kSigrLower;
			nSeismicForceType = src.nSeismicForceType;
			nSeismicForceDirection = src.nSeismicForceDirection;
			Line1 = src.Line1;
			Line2 = src.Line2;
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigrLower = 0;
		nSeismicForceType = 0;
		nSeismicForceDirection = 0;
		Line1.Initialize();
		Line2.Initialize();
	}
};

struct T_SIET_EVAL_BEAR_EACH_CH
{
	UINT kSipa;
	UINT kSigrLower;
	int nSeismicForceType;
	int nSeismicForceDirection;
	T_SIET_EVAL_BEAR_LINE_CONCU_CH Line1;
	T_SIET_EVAL_BEAR_LINE_CONCU_CH Line2;

	void Get(T_SIET_EVAL_BEAR_EACH& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		rData.nSeismicForceType = nSeismicForceType;
		Line1.Get(rData.Line1);
		Line2.Get(rData.Line2);
	}

	void Set(/*const*/T_SIET_EVAL_BEAR_EACH& data)
	{
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		nSeismicForceType = data.nSeismicForceType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		Line1.Set(data.Line1);
		Line2.Set(data.Line2);
	}
};

struct T_SIET_EVAL_BEAR_EACH_CH_RW
{
	UINT kSipa;
	UINT kSigrLower;
	int nSeismicForceType;
	int nSeismicForceDirection;
	T_SIET_EVAL_BEAR_LINE_CONCU_CH_RW Line1;
	T_SIET_EVAL_BEAR_LINE_CONCU_CH_RW Line2;

	void Get(T_SIET_EVAL_BEAR_EACH_CH& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		rData.nSeismicForceType = nSeismicForceType;
		Line1.Get(rData.Line1);
		Line2.Get(rData.Line2);
	}

	void Set(/*const*/T_SIET_EVAL_BEAR_EACH_CH& data)
	{
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		nSeismicForceType = data.nSeismicForceType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		Line1.Set(data.Line1);
		Line2.Set(data.Line2);
	}
};
#pragma endregion

#pragma region 지승에 대한 기타 조사 - 단면력 성분별 동시성(지승선별)
struct T_SIET_EVAL_LINE_GROUP
{
	CString strName;				//지승이름
	UINT kNode_i;					//절점번호
	UINT kNode_j;
	int nType;						//지승 조건 : enum T_SIGR_LOWER_BEAR_D::eBearingType;
	int nDirShear;					//전단의 방향 : enum T_SIGR_LOWER_BEAR_D::eDirection;
	T_SIET_EVAL_NLNK_FORCE Force;

	T_SIET_EVAL_LINE_GROUP() { Initialize(); }
	T_SIET_EVAL_LINE_GROUP(const T_SIET_EVAL_LINE_GROUP& src) { *this = src; }
	T_SIET_EVAL_LINE_GROUP& operator= (const T_SIET_EVAL_LINE_GROUP& src)
	{
		if (this != &src) {
			strName = src.strName;
			kNode_i = src.kNode_i;
			kNode_j = src.kNode_j;
			nType = src.nType;
			nDirShear = src.nDirShear;
			Force = src.Force;
		}
		return *this;
	}

	void Initialize()
	{
		strName.Empty();
		kNode_i = 0;
		kNode_j = 0;
		nType = 0;
		nDirShear = 0;
		Force.Initialize();
	}
};

struct T_SIET_EVAL_LINE_GROUP_CH
{
	char strName[60];
	UINT kNode_i;
	UINT kNode_j;
	int nType;
	int nDirShear;
	T_SIET_EVAL_NLNK_FORCE Force;

	void Get(T_SIET_EVAL_LINE_GROUP& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.kNode_i = kNode_i;
		rData.kNode_j = kNode_j;
		rData.nType = nType;
		rData.nDirShear = nDirShear;
		rData.Force = Force;
	}

	void Set(/*const*/ T_SIET_EVAL_LINE_GROUP& data)
	{
		ConvertStrChar(data.strName, strName, sizeof(strName));
		kNode_i = data.kNode_i;
		kNode_j = data.kNode_j;
		nType = data.nType;
		nDirShear = data.nDirShear;
		Force = data.Force;
	}
};

typedef CArray<T_SIET_EVAL_LINE_GROUP, T_SIET_EVAL_LINE_GROUP> GroupArray;

struct T_SIET_EVAL_LINE_MINMAX
{
	int nLineNum;
	T_SIET_EVAL_NLNK_FORCE SumMax;
	T_SIET_EVAL_NLNK_FORCE SumMin;
	GroupArray aHmax;
	GroupArray aHmin;

	T_SIET_EVAL_LINE_MINMAX() { Initialize(); }
	T_SIET_EVAL_LINE_MINMAX(const T_SIET_EVAL_LINE_MINMAX& src) { *this = src; }
	T_SIET_EVAL_LINE_MINMAX& operator= (const T_SIET_EVAL_LINE_MINMAX& src)
	{
		if (this != &src) {
			nLineNum = src.nLineNum;
			SumMax = src.SumMax;
			SumMin = src.SumMin;
			aHmax.Copy(src.aHmax);
			aHmin.Copy(src.aHmin);
		}
		return *this;
	}

	void Initialize()
	{
		nLineNum = 0;
		SumMax.Initialize();
		SumMin.Initialize();
		aHmax.RemoveAll();
		aHmin.RemoveAll();
	}
};

struct T_SIET_EVAL_LINE_MINMAX_CH
{
	int nLineNum;
	T_SIET_EVAL_NLNK_FORCE SumMax;
	T_SIET_EVAL_NLNK_FORCE SumMin;
	CArray<T_SIET_EVAL_LINE_GROUP_CH, T_SIET_EVAL_LINE_GROUP_CH> aHmax;
	CArray<T_SIET_EVAL_LINE_GROUP_CH, T_SIET_EVAL_LINE_GROUP_CH> aHmin;

	void Get(T_SIET_EVAL_LINE_MINMAX& rData)
	{
		rData.nLineNum = nLineNum;
		rData.SumMax = SumMax;
		rData.SumMin = SumMin;
		for (int i = 0; i < aHmax.GetCount(); ++i) {
			aHmax[i].Get(rData.aHmax[i]);
		}
		for (int i = 0; i < aHmin.GetCount(); ++i) {
			aHmin[i].Get(rData.aHmin[i]);
		}
	}

	void Set(/*const*/ T_SIET_EVAL_LINE_MINMAX& data)
	{
		nLineNum = data.nLineNum;
		SumMax = data.SumMax;
		SumMin = data.SumMin;
		aHmax.SetSize(data.aHmax.GetCount());
		for (int i = 0; i < data.aHmax.GetCount(); ++i) {
			aHmax[i].Set(data.aHmax[i]);
		}
		aHmin.SetSize(data.aHmin.GetCount());
		for (int i = 0; i < data.aHmin.GetCount(); ++i) {
			aHmin[i].Set(data.aHmin[i]);
		}
	}
};

struct T_SIET_EVAL_LINE_MINMAX_CH_RW
{
	int nLineNum;
	T_SIET_EVAL_NLNK_FORCE SumMax;
	T_SIET_EVAL_NLNK_FORCE SumMin;
	int nHmax;
	int nHmin;

	void Get(T_SIET_EVAL_LINE_MINMAX_CH& rData)
	{
		rData.nLineNum = nLineNum;
		rData.SumMax = SumMax;
		rData.SumMin = SumMin;
	}

	void Set(/*const*/ T_SIET_EVAL_LINE_MINMAX_CH& data)
	{
		nLineNum = data.nLineNum;
		SumMax = data.SumMax;
		SumMin = data.SumMin;
		nHmax = data.aHmax.GetCount();
		nHmin = data.aHmin.GetCount();
	}
};

//기타 조사 - 지승에 대한 단면력 성분별 동시성(지승선별)
struct T_SIET_EVAL_LINE_CONC
{
	UINT kSipa;
	UINT kSigrLower;
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nSeismicForceDirection;	//지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	T_SIET_EVAL_LINE_MINMAX Line1;
	T_SIET_EVAL_LINE_MINMAX Line2;

	T_SIET_EVAL_LINE_CONC() { Initialize(); }
	T_SIET_EVAL_LINE_CONC(const T_SIET_EVAL_LINE_CONC& src) { *this = src; }
	T_SIET_EVAL_LINE_CONC& operator= (const T_SIET_EVAL_LINE_CONC& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigrLower = src.kSigrLower;
			nSeismicForceType = src.nSeismicForceType;
			nSeismicForceDirection = src.nSeismicForceDirection;
			Line1 = src.Line1;
			Line2 = src.Line2;
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigrLower = 0;
		nSeismicForceType = 0;
		nSeismicForceDirection = 0;
		Line1.Initialize();
		Line2.Initialize();
	}
};

struct T_SIET_EVAL_LINE_CONC_CH
{
	UINT kSipa;
	UINT kSigrLower;
	int nSeismicForceType;
	int nSeismicForceDirection;
	T_SIET_EVAL_LINE_MINMAX_CH Line1;
	T_SIET_EVAL_LINE_MINMAX_CH Line2;

	void Get(T_SIET_EVAL_LINE_CONC& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		Line1.Get(rData.Line1);
		Line2.Get(rData.Line2);
	}

	void Set(/*const*/ T_SIET_EVAL_LINE_CONC& data)
	{
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		nSeismicForceType = data.nSeismicForceType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		Line1.Set(data.Line1);
		Line2.Set(data.Line2);
	}
};

struct T_SIET_EVAL_LINE_CONC_CH_RW
{
	UINT kSipa;
	UINT kSigrLower;
	int nSeismicForceType;
	int nSeismicForceDirection;
	T_SIET_EVAL_LINE_MINMAX_CH_RW Line1;
	T_SIET_EVAL_LINE_MINMAX_CH_RW Line2;

	void Get(T_SIET_EVAL_LINE_CONC_CH& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigrLower = kSigrLower;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		Line1.Get(rData.Line1);
		Line2.Get(rData.Line2);
	}

	void Set(/*const*/ T_SIET_EVAL_LINE_CONC_CH& data)
	{
		kSipa = data.kSipa;
		kSigrLower = data.kSigrLower;
		nSeismicForceType = data.nSeismicForceType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		Line1.Set(data.Line1);
		Line2.Set(data.Line2);
	}
};
#pragma endregion

#pragma endregion



#endif // !defined(__DB_ST_DT_SIET_EVAL__)