#ifndef __DB_ST_DT_SIRD_EVAL__
#define __DB_ST_DT_SIRD_EVAL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

struct T_SIRD_EVAL_DISP
{
	UINT kThis;
	double dDelta_max;	//最大応答変位δmax
	double dDelta_y;	//降伏変位δy
	double d_Cr;		//残留変位補正係数
	double d_r;			//橋脚の降伏剛性に対する降伏後の2次剛性の比
	double dMu_r;		//最大応答塑性率、δmax/δy
	double dDelta_r;	//残留変位δr
	double dDelta_ra;	//許容残留変位δra
	double dRatio;		//δa/δra
	int nDirAnal;		//해석방향 enum seis_jp::eDirAnal

	T_SIRD_EVAL_DISP() { Initialize(); }
	T_SIRD_EVAL_DISP(const T_SIRD_EVAL_DISP& src) { *this = src; }
	T_SIRD_EVAL_DISP& operator= (const T_SIRD_EVAL_DISP& src)
	{
		if (this != &src) {
			kThis = src.kThis;
			dDelta_max = src.dDelta_max;
			dDelta_y = src.dDelta_y;
			d_Cr = src.d_Cr;
			d_r = src.d_r;
			dMu_r = src.dMu_r;
			dDelta_r = src.dDelta_r;
			dDelta_ra = src.dDelta_ra;
			dRatio = src.dRatio;
			nDirAnal = src.nDirAnal;
		}
		return *this;
	}

	void Copy(const T_SIRD_EVAL_DISP& src)
	{
		if (this != &src) {
			kThis = src.kThis;
			dDelta_max = src.dDelta_max;
			dDelta_y = src.dDelta_y;
			d_Cr = src.d_Cr;
			d_r = src.d_r;
			dMu_r = src.dMu_r;
			dDelta_r = src.dDelta_r;
			dDelta_ra = src.dDelta_ra;
			dRatio = src.dRatio;
			nDirAnal = src.nDirAnal;
		}
		return;
	}

	T_SIRD_EVAL_DISP& operator += (const T_SIRD_EVAL_DISP& src)
	{
		if (this != &src) {
			dDelta_max += abs(src.dDelta_max);
			dDelta_y += abs(src.dDelta_y);
			d_Cr += abs(src.d_Cr);
			d_r += abs(src.d_r);
			dMu_r += abs(src.dMu_r);
			dDelta_r += abs(src.dDelta_r);
			dDelta_ra += abs(src.dDelta_ra);
			dRatio += abs(src.dRatio);
		}
		return *this;
	}

	T_SIRD_EVAL_DISP& operator /= (const double& den)
	{
		dDelta_max = dgn::SafeDiv(dDelta_max, den);
		dDelta_y = dgn::SafeDiv(dDelta_y, den);
		d_Cr = dgn::SafeDiv(d_Cr, den);
		d_r = dgn::SafeDiv(d_r, den);
		dMu_r = dgn::SafeDiv(dMu_r, den);
		dDelta_r = dgn::SafeDiv(dDelta_r, den);
		dDelta_ra = dgn::SafeDiv(dDelta_ra, den);
		dRatio = dgn::SafeDiv(dRatio, den);

		return *this;
	}

	void Initialize()
	{
		kThis = 0;
		dDelta_max = 0.0;
		dDelta_y = 0.0;
		d_Cr = 0.0;
		d_r = 0.0;
		dMu_r = 0.0;
		dDelta_r = 0.0;
		dDelta_ra = 0.0;
		dRatio = 0.0;
		nDirAnal = seis_jp::kDirAnal_0;
	}
};

//잔류변위 조사결과 - (H14/24/29 공통)
struct T_SIRD_EVAL
{
	UINT kSird;
	UINT kSipa;
	UINT kSigr;
	CString strName;
	int nPosType;			//위치 : enum T_SIRD_EVAL::ePosType
	T_NODE_K kGirderNode;	//상부구조 좌,우단 절점
	T_NODE_K kPierTopNode;	//하부구조 관성력 작용위치
	T_SIGR_LOWER_K kSigrLower;
	int nSeismicForceType;	//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nDirection;			//해석방향 enum T_SIRD_EVAL::eDirection
	T_SIRD_EVAL_DISP Averge;
	BOOL bCheckResult;
	CArray<T_SIRD_EVAL_DISP, T_SIRD_EVAL_DISP> aDisplacement;

	T_SIRD_EVAL() { Initialize(); }
	T_SIRD_EVAL(const T_SIRD_EVAL& src) { *this = src; }
	T_SIRD_EVAL& operator= (const T_SIRD_EVAL& src)
	{
		if (this != &src) {
			kSird = src.kSird;
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			strName = src.strName;
			nPosType = src.nPosType;
			kGirderNode = src.kGirderNode;
			kPierTopNode = src.kPierTopNode;
			kSigrLower = src.kSigrLower;
			nSeismicForceType = src.nSeismicForceType;
			nDirection = src.nDirection;
			Averge = src.Averge;
			bCheckResult = src.bCheckResult;
			aDisplacement.Copy(src.aDisplacement);
		}
		return *this;
	}

	void Initialize()
	{
		kSird = 0;
		kSipa = 0;
		kSigr = 0;
		strName = _T("");
		nPosType = 0;
		kGirderNode = 0;
		kPierTopNode = 0;
		kSigrLower = 0;
		nSeismicForceType = 0;
		nDirection = 0;
		Averge.Initialize();
		bCheckResult = FALSE;
		aDisplacement.RemoveAll();
	}

	enum ePosType
	{
		kUnKnown,
		kLeft,
		kRight,
		kLower,
	};

	enum eDirection
	{
		kPlus,
		kMinus,
		kDirMax,
	};
};

struct T_SIRD_EVAL_CH
{
	UINT kSird;
	UINT kSipa;
	UINT kSigr;
	char strName[60];
	int nPosType;
	T_NODE_K kGirderNode;
	T_NODE_K kPierTopNode;
	T_SIGR_LOWER_K kSigrLower;
	int nSeismicForceType;
	int nDirection;
	T_SIRD_EVAL_DISP Averge;
	BOOL bCheckResult;
	CArray<T_SIRD_EVAL_DISP, T_SIRD_EVAL_DISP> aDisplacement;

	void Get(T_SIRD_EVAL& rData)
	{
		rData.kSird = kSird;
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nPosType = nPosType;
		rData.kGirderNode = kGirderNode;
		rData.kPierTopNode = kPierTopNode;
		rData.kSigrLower = kSigrLower;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nDirection = nDirection;
		rData.Averge = Averge;
		rData.bCheckResult = bCheckResult;
		rData.aDisplacement.Copy(aDisplacement);
	}

	void Set(/*const*/ T_SIRD_EVAL& data)
	{
		kSird = data.kSird;
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		ConvertStrChar(data.strName, strName, sizeof(strName));
		nPosType = data.nPosType;
		kGirderNode = data.kGirderNode;
		kPierTopNode = data.kPierTopNode;
		kSigrLower = data.kSigrLower;
		nSeismicForceType = data.nSeismicForceType;
		nDirection = data.nDirection;
		Averge = data.Averge;
		bCheckResult = data.bCheckResult;
		aDisplacement.Copy(data.aDisplacement);
	}
};

struct T_SIRD_EVAL_CH_RW
{
	UINT kSird;
	UINT kSipa;
	UINT kSigr;
	char strName[60];
	int nPosType;
	T_NODE_K kGirderNode;
	T_NODE_K kPierTopNode;
	T_SIGR_LOWER_K kSigrLower;
	int nSeismicForceType;
	int nDirection;
	T_SIRD_EVAL_DISP Averge;
	BOOL bCheckResult;
	int nDisplacement;

	void Get(T_SIRD_EVAL_CH& rData)
	{
		rData.kSird = kSird;
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		strncpy(rData.strName, strName, sizeof(rData.strName) - 1);
		rData.strName[sizeof(rData.strName) - 1] = '\0';
		rData.nPosType = nPosType;
		rData.kGirderNode = kGirderNode;
		rData.kPierTopNode = kPierTopNode;
		rData.kSigrLower = kSigrLower;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nDirection = nDirection;
		rData.Averge = Averge;
		rData.bCheckResult = bCheckResult;
	}
	void Set(const T_SIRD_EVAL_CH& data)
	{
		kSird = data.kSird;
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		strncpy(strName, data.strName, sizeof(strName) - 1);
		strName[sizeof(strName) - 1] = '\0';
		nPosType = data.nPosType;
		kGirderNode = data.kGirderNode;
		kPierTopNode = data.kPierTopNode;
		kSigrLower = data.kSigrLower;
		nSeismicForceType = data.nSeismicForceType;
		nDirection = data.nDirection;
		Averge = data.Averge;
		bCheckResult = data.bCheckResult;
		nDisplacement = data.aDisplacement.GetCount();
	}
};

#endif // !defined(__DB_ST_DT_SIRD_EVAL__)