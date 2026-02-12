#ifndef __DB_ST_DT_SIBD_EVAL__
#define __DB_ST_DT_SIBD_EVAL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

struct T_SIBD_EVAL_RESULT_ST
{
	double dRatioYc;
	double dRatioYt;
	BOOL bCheckResultYc;
	BOOL bCheckResultYt;

	T_SIBD_EVAL_RESULT_ST() { Initialize(); }
	T_SIBD_EVAL_RESULT_ST(const T_SIBD_EVAL_RESULT_ST& src) { *this = src; }
	T_SIBD_EVAL_RESULT_ST& operator= (const T_SIBD_EVAL_RESULT_ST& src)
	{
		if (this != &src) {
			dRatioYc = src.dRatioYc;
			dRatioYt = src.dRatioYt;
			bCheckResultYc = src.bCheckResultYc;
			bCheckResultYt = src.bCheckResultYt;
		}
		return *this;
	}

	void Initialize()
	{
		dRatioYc = 0;
		dRatioYt = 0;
		bCheckResultYc = FALSE;
		bCheckResultYt = FALSE;
	}
};

struct T_SIBD_EVAL_RESULT_RC
{
	UINT kNode;
	double dRatioMin;
	double dRatioMax;
	BOOL bCheckResultMin;
	BOOL bCheckResultMax;

	T_SIBD_EVAL_RESULT_RC() { Initialize(); }
	T_SIBD_EVAL_RESULT_RC(const T_SIBD_EVAL_RESULT_RC& src) { *this = src; }
	T_SIBD_EVAL_RESULT_RC& operator= (const T_SIBD_EVAL_RESULT_RC& src)
	{
		if (this != &src) {
			kNode = src.kNode;
			dRatioMin = src.dRatioMin;
			dRatioMax = src.dRatioMax;
			bCheckResultMin = src.bCheckResultMin;
			bCheckResultMax = src.bCheckResultMax;
		}
		return *this;
	}

	void Initialize()
	{
		kNode = 0;
		dRatioMin = 0;
		dRatioMax = 0;
		bCheckResultMin = FALSE;
		bCheckResultMax = FALSE;
	}
};

//해석케이스별 값
struct T_SIBD_EVAL_ANAL_VALUE
{
	double dAvergeMin;
	double dAvergeMax;
	CArray<double, double> aSmin;
	CArray<double, double> aSmax;

	T_SIBD_EVAL_ANAL_VALUE() { Initialize(); }
	T_SIBD_EVAL_ANAL_VALUE(const T_SIBD_EVAL_ANAL_VALUE& src) { *this = src; }
	T_SIBD_EVAL_ANAL_VALUE& operator= (const T_SIBD_EVAL_ANAL_VALUE& src)
	{
		if (this != &src) {
			dAvergeMin = src.dAvergeMin;
			dAvergeMax = src.dAvergeMax;
			aSmin.Copy(src.aSmin);
			aSmax.Copy(src.aSmax);
		}
		return *this;
	}

	void Initialize()
	{
		dAvergeMin = 0;
		dAvergeMax = 0;
		aSmin.RemoveAll();
		aSmax.RemoveAll();
	}
};

struct T_SIBD_EVAL_ANAL_VALUE_RW
{
	double dAvergeMin;
	double dAvergeMax;
	int nSmin;
	int nSmax;

	T_SIBD_EVAL_ANAL_VALUE_RW() { Initialize(); }
	T_SIBD_EVAL_ANAL_VALUE_RW(const T_SIBD_EVAL_ANAL_VALUE_RW& src) { *this = src; }
	T_SIBD_EVAL_ANAL_VALUE_RW& operator= (const T_SIBD_EVAL_ANAL_VALUE_RW& src)
	{
		if (this != &src) {
			dAvergeMin = src.dAvergeMin;
			dAvergeMax = src.dAvergeMax;
			nSmin = src.nSmin;
			nSmax = src.nSmax;
		}
		return *this;
	}

	void Get(T_SIBD_EVAL_ANAL_VALUE& rData)
	{
		rData.dAvergeMin = dAvergeMin;
		rData.dAvergeMax = dAvergeMax;
	}
	void Set(const T_SIBD_EVAL_ANAL_VALUE& data)
	{
		dAvergeMin = data.dAvergeMin;
		dAvergeMax = data.dAvergeMax;
		nSmin = data.aSmin.GetCount();
		nSmax = data.aSmax.GetCount();
	}

	void Initialize()
	{
		dAvergeMin = 0;
		dAvergeMax = 0;
		nSmin = 0;
		nSmax = 0;
	}
};

//M-Phi 검토결과
struct T_SIBD_EVAL_CALC_MPhi
{
	double dC;
	double dY;
	double dU;

	T_SIBD_EVAL_CALC_MPhi() { Initialize(); }
	T_SIBD_EVAL_CALC_MPhi(const T_SIBD_EVAL_CALC_MPhi& src) { *this = src; }
	T_SIBD_EVAL_CALC_MPhi& operator= (const T_SIBD_EVAL_CALC_MPhi& src)
	{
		if (this != &src) {
			dC = src.dC;
			dY = src.dY;
			dU = src.dU;
		}
		return *this;
	}

	void Initialize()
	{
		dC = 0;
		dY = 0;
		dU = 0;
	}
};

//휨 조사 (내력조사 용)
struct T_SIBD_EVAL_MOMENT
{
	UINT kSibd;
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	int nMemberType;						//부재 타입 : enum DB_MEMB_TYPE
	int nExamShearDir;						//검토된 기준 전단력 방향 : enum seis_jp::eDirShear
	T_SIBD_EVAL_CALC_MPhi calcMomeMin;
	T_SIBD_EVAL_CALC_MPhi calcMomeMax;
	T_SIBD_EVAL_ANAL_VALUE analMoment_i;
	T_SIBD_EVAL_ANAL_VALUE analMoment_j;
	T_SIBD_EVAL_RESULT_RC result_i;
	T_SIBD_EVAL_RESULT_RC result_j;

	T_SIBD_EVAL_MOMENT() { Initialize(); }
	T_SIBD_EVAL_MOMENT(const T_SIBD_EVAL_MOMENT& src) { *this = src; }
	T_SIBD_EVAL_MOMENT& operator= (const T_SIBD_EVAL_MOMENT& src)
	{
		if (this != &src) {
			kSibd = src.kSibd;
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			kElem = src.kElem;
			nMemberType = src.nMemberType;
			nExamShearDir = src.nExamShearDir;
			calcMomeMin = src.calcMomeMin;
			calcMomeMax = src.calcMomeMax;
			analMoment_i = src.analMoment_i;
			analMoment_j = src.analMoment_j;
			result_i = src.result_i;
			result_j = src.result_j;
		}
		return *this;
	}

	void Initialize()
	{
		kSibd = 0;
		kSipa = 0;
		kSigr = 0;
		kElem = 0;
		nMemberType = 0;
		nExamShearDir = 0;
		calcMomeMin.Initialize();
		calcMomeMax.Initialize();
		analMoment_i.Initialize();
		analMoment_j.Initialize();
		result_i.Initialize();
		result_j.Initialize();
	}
};

struct T_SIBD_EVAL_MOMENT_RW
{
	UINT kSibd;
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	int nMemberType;						//부재 타입 : enum DB_MEMB_TYPE
	int nExamShearDir;
	T_SIBD_EVAL_CALC_MPhi calcMomeMin;
	T_SIBD_EVAL_CALC_MPhi calcMomeMax;
	T_SIBD_EVAL_ANAL_VALUE_RW analMoment_i;
	T_SIBD_EVAL_ANAL_VALUE_RW analMoment_j;
	T_SIBD_EVAL_RESULT_RC result_i;
	T_SIBD_EVAL_RESULT_RC result_j;

	T_SIBD_EVAL_MOMENT_RW() { Initialize(); }
	T_SIBD_EVAL_MOMENT_RW(const T_SIBD_EVAL_MOMENT_RW& src) { *this = src; }
	T_SIBD_EVAL_MOMENT_RW& operator= (const T_SIBD_EVAL_MOMENT_RW& src)
	{
		if (this != &src) {
			kSibd = src.kSibd;
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			kElem = src.kElem;
			nMemberType = src.nMemberType;
			nExamShearDir = src.nExamShearDir;
			calcMomeMin = src.calcMomeMin;
			calcMomeMax = src.calcMomeMax;
			analMoment_i = src.analMoment_i;
			analMoment_j = src.analMoment_j;
			result_i = src.result_i;
			result_j = src.result_j;
		}
		return *this;
	}

	void Get(T_SIBD_EVAL_MOMENT& rData)
	{
		rData.kSibd = kSibd;
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.kElem = kElem;
		rData.nMemberType = nMemberType;
		rData.nExamShearDir = nExamShearDir;
		rData.calcMomeMin = calcMomeMin;
		rData.calcMomeMax = calcMomeMax;
		rData.analMoment_i.dAvergeMax = analMoment_i.dAvergeMax;
		rData.analMoment_i.dAvergeMin = analMoment_i.dAvergeMin;
		rData.analMoment_j.dAvergeMax = analMoment_j.dAvergeMax;
		rData.analMoment_j.dAvergeMin = analMoment_j.dAvergeMin;
		rData.result_i = result_i;
		rData.result_j = result_j;
	}
	void Set(const T_SIBD_EVAL_MOMENT& data)
	{
		kSibd = data.kSibd;
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		kElem = data.kElem;
		nMemberType = data.nMemberType;
		nExamShearDir = data.nExamShearDir;
		calcMomeMin = data.calcMomeMin;
		calcMomeMax = data.calcMomeMax;
		analMoment_i.dAvergeMax = data.analMoment_i.dAvergeMax;
		analMoment_i.dAvergeMin = data.analMoment_i.dAvergeMin;
		analMoment_i.nSmax = (int)data.analMoment_i.aSmax.GetSize();
		analMoment_i.nSmin = (int)data.analMoment_i.aSmin.GetSize();
		analMoment_j.dAvergeMax = data.analMoment_j.dAvergeMax;
		analMoment_j.dAvergeMin = data.analMoment_j.dAvergeMin;
		analMoment_j.nSmax = (int)data.analMoment_j.aSmax.GetSize();
		analMoment_j.nSmin = (int)data.analMoment_j.aSmin.GetSize();
		result_i = data.result_i;
		result_j = data.result_j;
	}

	void Initialize()
	{
		kSibd = 0;
		kSipa = 0;
		kSigr = 0;
		kElem = 0;
		nMemberType = 0;
		calcMomeMin.Initialize();
		calcMomeMax.Initialize();
		analMoment_i.Initialize();
		analMoment_j.Initialize();
		result_i.Initialize();
		result_j.Initialize();
	}
};

//M-Phi 검토결과
struct T_SIBD_EVAL_CALC_PHI_Y
{
	double dPhi_c;
	double dPhi_y;
	double dPhi_u; //H-14 : φu, H-24 : φls

	T_SIBD_EVAL_CALC_PHI_Y() { Initialize(); }
	T_SIBD_EVAL_CALC_PHI_Y(const T_SIBD_EVAL_CALC_PHI_Y& src) { *this = src; }
	T_SIBD_EVAL_CALC_PHI_Y& operator= (const T_SIBD_EVAL_CALC_PHI_Y& src)
	{
		if (this != &src) {
			dPhi_c = src.dPhi_c;
			dPhi_y = src.dPhi_y;
			dPhi_u = src.dPhi_u;
		}
		return *this;
	}

	void Initialize()
	{
		dPhi_c = 0;
		dPhi_y = 0;
		dPhi_u = 0;
	}
};

//휨 조사 (항복조사 용)
struct T_SIBD_EVAL_PHI_Y
{
	UINT kSibd;
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	int nMemberType;						//부재 타입 : enum DB_MEMB_TYPE
	int nMaterialType;						//교각의 재료 타입 : enum T_SIGR_LOWER_D::eMaterialType
	int nExamShearDir;						//검토된 기준 전단력 방향 : enum seis_jp::eDirShear
	T_SIBD_EVAL_CALC_PHI_Y calcDeformMin;
	T_SIBD_EVAL_CALC_PHI_Y calcDeformMax;
	T_SIBD_EVAL_ANAL_VALUE analDeform;
	T_SIBD_EVAL_RESULT_RC resultRc;

	T_SIBD_EVAL_PHI_Y() { Initialize(); }
	T_SIBD_EVAL_PHI_Y(const T_SIBD_EVAL_PHI_Y& src) { *this = src; }
	T_SIBD_EVAL_PHI_Y& operator= (const T_SIBD_EVAL_PHI_Y& src)
	{
		if (this != &src) {
			kSibd = src.kSibd;
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			kElem = src.kElem;
			nMemberType = src.nMemberType;
			nMaterialType = src.nMaterialType;
			nExamShearDir = src.nExamShearDir;
			calcDeformMin = src.calcDeformMin;
			calcDeformMax = src.calcDeformMax;
			analDeform = src.analDeform;
			resultRc = src.resultRc;
		}
		return *this;
	}

	void Initialize()
	{
		kSibd = 0;
		kSipa = 0;
		kSigr = 0;
		kElem = 0;
		nMemberType = 0;
		nMaterialType = 0;
		nExamShearDir = 0;
		calcDeformMin.Initialize();
		calcDeformMax.Initialize();
		analDeform.Initialize();
		resultRc.Initialize();
	}
};

struct T_SIBD_EVAL_PHI_Y_RW
{
	UINT kSibd;
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	int nMemberType;
	int nMaterialType;
	int nExamShearDir;
	T_SIBD_EVAL_CALC_PHI_Y calcDeformMin;
	T_SIBD_EVAL_CALC_PHI_Y calcDeformMax;
	T_SIBD_EVAL_ANAL_VALUE_RW analDeform;
	T_SIBD_EVAL_RESULT_RC resultRc;

	T_SIBD_EVAL_PHI_Y_RW() { Initialize(); }
	T_SIBD_EVAL_PHI_Y_RW(const T_SIBD_EVAL_PHI_Y_RW& src) { *this = src; }
	T_SIBD_EVAL_PHI_Y_RW& operator= (const T_SIBD_EVAL_PHI_Y_RW& src)
	{
		if (this != &src) {
			kSibd = src.kSibd;
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			kElem = src.kElem;
			nMemberType = src.nMemberType;
			nMaterialType = src.nMaterialType;
			nExamShearDir = src.nExamShearDir;
			calcDeformMin = src.calcDeformMin;
			calcDeformMax = src.calcDeformMax;
			analDeform = src.analDeform;
			resultRc = src.resultRc;
		}
		return *this;
	}

	void Get(T_SIBD_EVAL_PHI_Y& rData)
	{
		rData.kSibd = kSibd;
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.kElem = kElem;
		rData.nMemberType = nMemberType;
		rData.nMaterialType = nMaterialType;
		rData.nExamShearDir = nExamShearDir;
		rData.calcDeformMin = calcDeformMin;
		rData.calcDeformMax = calcDeformMax;
		rData.analDeform.dAvergeMax = analDeform.dAvergeMax;
		rData.analDeform.dAvergeMin = analDeform.dAvergeMin;
		rData.resultRc = resultRc;
	}
	void Set(const T_SIBD_EVAL_PHI_Y& data)
	{
		kSibd = data.kSibd;
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		kElem = data.kElem;
		nMemberType = data.nMemberType;
		nMaterialType = data.nMaterialType;
		nExamShearDir = data.nExamShearDir;
		calcDeformMin = data.calcDeformMin;
		calcDeformMax = data.calcDeformMax;
		analDeform.dAvergeMax = data.analDeform.dAvergeMax;
		analDeform.dAvergeMin = data.analDeform.dAvergeMin;
		analDeform.nSmax = (int)data.analDeform.aSmax.GetSize();
		analDeform.nSmin = (int)data.analDeform.aSmin.GetSize();
		resultRc = data.resultRc;
	}

	void Initialize()
	{
		kSibd = 0;
		kSipa = 0;
		kSigr = 0;
		kElem = 0;
		nMemberType = 0;
		nMaterialType = 0;
		nExamShearDir = 0;
		calcDeformMin.Initialize();
		calcDeformMax.Initialize();
		analDeform.Initialize();
		resultRc.Initialize();
	}
};

//휨 조사 (항복조사 강재교각 용)
struct T_SIBD_EVAL_PHI_Y_ST
{
	UINT kSibd;
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	int nMemberType;						//부재 타입 : enum DB_MEMB_TYPE
	int nMaterialType;						//교각의 재료 타입 : enum T_SIGR_LOWER_D::eMaterialType
	int nExamShearDir;						//검토된 기준 전단력 방향 : enum seis_jp::eDirShear
	T_SIBD_EVAL_CALC_PHI_Y calcDeform;
	T_SIBD_EVAL_RESULT_ST resultSt;
	double dAverge;
	CArray<double, double> aAnalDeform;

	T_SIBD_EVAL_PHI_Y_ST() { Initialize(); }
	T_SIBD_EVAL_PHI_Y_ST(const T_SIBD_EVAL_PHI_Y_ST& src) { *this = src; }
	T_SIBD_EVAL_PHI_Y_ST& operator= (const T_SIBD_EVAL_PHI_Y_ST& src)
	{
		if (this != &src) {
			kSibd = src.kSibd;
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			kElem = src.kElem;
			nMemberType = src.nMemberType;
			nMaterialType = src.nMaterialType;
			nExamShearDir = src.nExamShearDir;
			calcDeform = src.calcDeform;
			resultSt = src.resultSt;
			dAverge = src.dAverge;
			aAnalDeform.Copy(src.aAnalDeform);
		}
		return *this;
	}

	void Initialize()
	{
		kSibd = 0;
		kSipa = 0;
		kSigr = 0;
		kElem = 0;
		nMemberType = 0;
		nMaterialType = 0;
		nExamShearDir = 0;
		calcDeform.Initialize();
		resultSt.Initialize();
		dAverge = 0;
		aAnalDeform.RemoveAll();
	}
};

struct T_SIBD_EVAL_PHI_Y_ST_RW
{
	UINT kSibd;
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	int nMemberType;
	int nMaterialType;
	int nExamShearDir;
	T_SIBD_EVAL_CALC_PHI_Y calcDeform;
	T_SIBD_EVAL_RESULT_ST resultSt;
	double dAverge;
	int nAnalDeform;

	void Get(T_SIBD_EVAL_PHI_Y_ST& rData)
	{
		rData.kSibd = kSibd;
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.kElem = kElem;
		rData.nMemberType = nMemberType;
		rData.nMaterialType = nMaterialType;
		rData.nExamShearDir = nExamShearDir;
		rData.calcDeform = calcDeform;
		rData.resultSt = resultSt;
		rData.dAverge = dAverge;
	}
	void Set(const T_SIBD_EVAL_PHI_Y_ST& data)
	{
		kSibd = data.kSibd;
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		kElem = data.kElem;
		nMemberType = data.nMemberType;
		nMaterialType = data.nMaterialType;
		nExamShearDir = data.nExamShearDir;
		calcDeform = data.calcDeform;
		resultSt = data.resultSt;
		dAverge = data.dAverge;
		nAnalDeform = (int)data.aAnalDeform.GetSize();
	}

	void Initialize()
	{
		kSibd = 0;
		kSipa = 0;
		kSigr = 0;
		kElem = 0;
		nMemberType = 0;
		nMaterialType = 0;
		nExamShearDir = 0;
		calcDeform.Initialize();
		resultSt.Initialize();
		dAverge = 0;
		nAnalDeform = 0;
	}
};

//M-Phi 검토결과
struct T_SIBD_EVAL_CALC_PHI_A
{
	double dPhi_c;
	double dPhi_y;
	double dPhi_u; //H-14 : φu, H-24 : φls
	double dPhi_a;
	double dAlpha; //安全係数

	T_SIBD_EVAL_CALC_PHI_A() { Initialize(); }
	T_SIBD_EVAL_CALC_PHI_A(const T_SIBD_EVAL_CALC_PHI_A& src) { *this = src; }
	T_SIBD_EVAL_CALC_PHI_A& operator= (const T_SIBD_EVAL_CALC_PHI_A& src)
	{
		if (this != &src) {
			dPhi_c = src.dPhi_c;
			dPhi_y = src.dPhi_y;
			dPhi_u = src.dPhi_u;
			dPhi_a = src.dPhi_a;
			dAlpha = src.dAlpha;
		}
		return *this;
	}

	void Initialize()
	{
		dPhi_c = 0;
		dPhi_y = 0;
		dPhi_u = 0;
		dPhi_a = 0;
		dAlpha = 0;
	}
};

//휨 조사 (한계상태조사 용)
struct T_SIBD_EVAL_PHI_A
{
	UINT kSibd;
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	int nMemberType;						//부재 타입 : enum DB_MEMB_TYPE
	int nMaterialType;						//교각의 재료 타입 : enum T_SIGR_LOWER_D::eMaterialType
	int nExamShearDir;						//검토된 기준 전단력 방향 : enum seis_jp::eDirShear
	T_SIBD_EVAL_CALC_PHI_A calcDeformMin;
	T_SIBD_EVAL_CALC_PHI_A calcDeformMax;
	T_SIBD_EVAL_ANAL_VALUE analDeform;
	T_SIBD_EVAL_RESULT_RC resultRc;

	T_SIBD_EVAL_PHI_A() { Initialize(); }
	T_SIBD_EVAL_PHI_A(const T_SIBD_EVAL_PHI_A& src) { *this = src; }
	T_SIBD_EVAL_PHI_A& operator= (const T_SIBD_EVAL_PHI_A& src)
	{
		if (this != &src) {
			kSibd = src.kSibd;
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			kElem = src.kElem;
			nMemberType = src.nMemberType;
			nMaterialType = src.nMaterialType;
			nExamShearDir = src.nExamShearDir;
			calcDeformMin = src.calcDeformMin;
			calcDeformMax = src.calcDeformMax;
			analDeform = src.analDeform;
			resultRc = src.resultRc;
		}
		return *this;
	}

	void Initialize()
	{
		kSibd = 0;
		kSipa = 0;
		kSigr = 0;
		kElem = 0;
		nMemberType = 0;
		nMaterialType = 0;
		nExamShearDir = 0;
		calcDeformMin.Initialize();
		calcDeformMax.Initialize();
		analDeform.Initialize();
		resultRc.Initialize();
	}

	enum eMemberType
	{
		kUnkown,
		kColumn,
		kBeam,
	};
};

struct T_SIBD_EVAL_PHI_A_RW
{
	UINT kSibd;
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	int nMemberType;
	int nMaterialType;
	int nExamShearDir;
	T_SIBD_EVAL_CALC_PHI_A calcDeformMin;
	T_SIBD_EVAL_CALC_PHI_A calcDeformMax;
	T_SIBD_EVAL_ANAL_VALUE_RW analDeform;
	T_SIBD_EVAL_RESULT_RC resultRc;

	T_SIBD_EVAL_PHI_A_RW() { Initialize(); }
	T_SIBD_EVAL_PHI_A_RW(const T_SIBD_EVAL_PHI_A_RW& src) { *this = src; }
	T_SIBD_EVAL_PHI_A_RW& operator= (const T_SIBD_EVAL_PHI_A_RW& src)
	{
		if (this != &src) {
			kSibd = src.kSibd;
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			kElem = src.kElem;
			nMemberType = src.nMemberType;
			nMaterialType = src.nMaterialType;
			nExamShearDir = src.nExamShearDir;
			calcDeformMin = src.calcDeformMin;
			calcDeformMax = src.calcDeformMax;
			analDeform = src.analDeform;
			resultRc = src.resultRc;
		}
		return *this;
	}
	
	void Get(T_SIBD_EVAL_PHI_A& rData)
	{
		rData.kSibd = kSibd;
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.kElem = kElem;
		rData.nMemberType = nMemberType;
		rData.nMaterialType = nMaterialType;
		rData.nExamShearDir = nExamShearDir;
		rData.calcDeformMin = calcDeformMin;
		rData.calcDeformMax = calcDeformMax;
		rData.analDeform.dAvergeMax = analDeform.dAvergeMax;
		rData.analDeform.dAvergeMin = analDeform.dAvergeMin;
		rData.resultRc = resultRc;
	}
	void Set(const T_SIBD_EVAL_PHI_A& data)
	{
		kSibd = data.kSibd;
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		kElem = data.kElem;
		nMemberType = data.nMemberType;
		nMaterialType = data.nMaterialType;
		nExamShearDir = data.nExamShearDir;
		calcDeformMin = data.calcDeformMin;
		calcDeformMax = data.calcDeformMax;
		analDeform.dAvergeMax = data.analDeform.dAvergeMax;
		analDeform.dAvergeMin = data.analDeform.dAvergeMin;
		analDeform.nSmax = (int)data.analDeform.aSmax.GetSize();
		analDeform.nSmin = (int)data.analDeform.aSmin.GetSize();
		resultRc = data.resultRc;
	}

	void Initialize()
	{
		kSibd = 0;
		kSipa = 0;
		kSigr = 0;
		kElem = 0;
		nMemberType = 0;
		nMaterialType = 0;
		nExamShearDir = 0;
		calcDeformMin.Initialize();
		calcDeformMax.Initialize();
		analDeform.Initialize();
		resultRc.Initialize();
	}
};

//휨 조사 (한계상태조사 강재교갹 용)
struct T_SIBD_EVAL_PHI_A_ST
{
	UINT kSibd;
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	int nMemberType;						//부재 타입 : enum DB_MEMB_TYPE
	int nMaterialType;						//교각의 재료 타입 : enum T_SIGR_LOWER_D::eMaterialType
	int nExamShearDir;						//검토된 기준 전단력 방향 : enum seis_jp::eDirShear
	T_SIBD_EVAL_CALC_PHI_A calcDeform;
	T_SIBD_EVAL_RESULT_ST resultSt;
	double dAverge;
	CArray<double, double> aAnalDeform;

	T_SIBD_EVAL_PHI_A_ST() { Initialize(); }
	T_SIBD_EVAL_PHI_A_ST(const T_SIBD_EVAL_PHI_A_ST& src) { *this = src; }
	T_SIBD_EVAL_PHI_A_ST& operator= (const T_SIBD_EVAL_PHI_A_ST& src)
	{
		if (this != &src) {
			kSibd = src.kSibd;
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			kElem = src.kElem;
			nMemberType = src.nMemberType;
			nMaterialType = src.nMaterialType;
			nExamShearDir = src.nExamShearDir;
			calcDeform = src.calcDeform;
			resultSt = src.resultSt;
			dAverge = src.dAverge;
			aAnalDeform.Copy(src.aAnalDeform);
		}
		return *this;
	}

	void Initialize()
	{
		kSibd = 0;
		kSipa = 0;
		kSigr = 0;
		kElem = 0;
		nMemberType = 0;
		nMaterialType = 0;
		nExamShearDir = 0;
		calcDeform.Initialize();
		resultSt.Initialize();
		dAverge = 0;
		aAnalDeform.RemoveAll();
	}
};

struct T_SIBD_EVAL_PHI_A_ST_RW
{
	UINT kSibd;
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	int nMemberType;
	int nMaterialType;
	int nExamShearDir;
	T_SIBD_EVAL_CALC_PHI_A calcDeform;
	T_SIBD_EVAL_RESULT_ST resultSt;
	double dAverge;
	int nAnalDeform;

	void Get(T_SIBD_EVAL_PHI_A_ST& rData)
	{
		rData.kSibd = kSibd;
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.kElem = kElem;
		rData.nMemberType = nMemberType;
		rData.nMaterialType = nMaterialType;
		rData.nExamShearDir = nExamShearDir;
		rData.calcDeform = calcDeform;
		rData.resultSt = resultSt;
		rData.dAverge = dAverge;
	}
	void Set(const T_SIBD_EVAL_PHI_A_ST& data)
	{
		kSibd = data.kSibd;
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		kElem = data.kElem;
		nMemberType = data.nMemberType;
		nMaterialType = data.nMaterialType;
		nExamShearDir = data.nExamShearDir;
		calcDeform = data.calcDeform;
		resultSt = data.resultSt;
		dAverge = data.dAverge;
		nAnalDeform = (int)data.aAnalDeform.GetSize();
	}

	void Initialize()
	{
		kSibd = 0;
		kSipa = 0;
		kSigr = 0;
		kElem = 0;
		nMemberType = 0;
		nMaterialType = 0;
		nExamShearDir = 0;
		calcDeform.Initialize();
		resultSt.Initialize();
		dAverge = 0;
		nAnalDeform = 0;
	}
};

//휨 조사 H29 (철근 단락부 용) 
struct T_SIBD_EVAL_CALC_DAMAGE_H29
{
	double dHi;		//橋脚基部から段落し位置までの高さ(m) 
	double dH;		//橋脚基部から上部構造の慣性力作用位置までの高さ(m) 
	double dMyi;	//段落し位置の断面の降伏曲げモーメント(kN・m) 
	double dMyB;	//橋脚基部断面の降伏曲げモーメント(kN・m) 
	double dD;		//段落し位置における定着長での橋脚の断面寸法(m)
	double dVal;	//h(1-Myi/(2*MyB))+D

	T_SIBD_EVAL_CALC_DAMAGE_H29() { Initialize(); }
	T_SIBD_EVAL_CALC_DAMAGE_H29(const T_SIBD_EVAL_CALC_DAMAGE_H29& src) { *this = src; }
	T_SIBD_EVAL_CALC_DAMAGE_H29& operator= (const T_SIBD_EVAL_CALC_DAMAGE_H29& src)
	{
		if (this != &src) {
			dHi = src.dHi;
			dH = src.dH;
			dMyi = src.dMyi;
			dMyB = src.dMyB;
			dD = src.dD;
			dVal = src.dVal;
		}
		return *this;
	}

	void Initialize()
	{
		dHi = 0;
		dH = 0;
		dMyi = 0;
		dMyB = 0;
		dD = 0;
		dVal = 0;
	}
};

struct T_SIBD_EVAL_REBAR_CUT
{
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	CString strSigrName;
	CString strColmName;
	int nRebarCutPos;			//철근 단락부 위치 T_SIGR_LOWER_COLM_D::ePosType, CDgnToolMPhi_SeismicDesign_JP::ePos
	int nSeismicForceDirection; //지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nExamShearDir;			//검토된 기준 전단력 방향 : enum seis_jp::eDirShear
	T_SIBD_EVAL_CALC_MPhi calcStrengthMax;
	T_SIBD_EVAL_CALC_MPhi calcStrengthMin;
	T_SIBD_EVAL_CALC_DAMAGE_H29 DamageH29Max;
	T_SIBD_EVAL_CALC_DAMAGE_H29 DamageH29Min;
	T_SIBD_EVAL_ANAL_VALUE analStrength;
	T_SIBD_EVAL_RESULT_RC resultRc;

	T_SIBD_EVAL_REBAR_CUT() { Initialize(); }
	T_SIBD_EVAL_REBAR_CUT(const T_SIBD_EVAL_REBAR_CUT& src) { *this = src; }
	T_SIBD_EVAL_REBAR_CUT& operator= (const T_SIBD_EVAL_REBAR_CUT& src)
	{
		if (this != &src) {
			kSipa = src.kSipa;
			kSigr = src.kSigr;
			kElem = src.kElem;
			strSigrName = src.strSigrName;
			strColmName = src.strColmName;
			nRebarCutPos = src.nRebarCutPos;
			nSeismicForceDirection = src.nSeismicForceDirection;
			nSeismicForceType = src.nSeismicForceType;
			nExamShearDir = src.nExamShearDir;
			calcStrengthMax = src.calcStrengthMax;
			calcStrengthMin = src.calcStrengthMin;
			analStrength = src.analStrength;
			DamageH29Max = src.DamageH29Max;
			DamageH29Min = src.DamageH29Min;
			resultRc = src.resultRc;
		}
		return *this;
	}

	void Initialize()
	{
		kSipa = 0;
		kSigr = 0;
		kElem = 0;
		strSigrName = _T("");
		strColmName = _T("");
		nRebarCutPos = 0;
		nSeismicForceDirection = 0;
		nSeismicForceType = 0;
		nExamShearDir = 0;
		calcStrengthMax.Initialize();
		calcStrengthMin.Initialize();
		DamageH29Max.Initialize();
		DamageH29Min.Initialize();
		resultRc.Initialize();
	}
};

struct T_SIBD_EVAL_REBAR_CUT_CH
{
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	char strSigrName[40];
	char strColmName[60];
	int nRebarCutPos;
	int nSeismicForceDirection;
	int nSeismicForceType;
	int nExamShearDir;
	T_SIBD_EVAL_CALC_MPhi calcStrengthMax;
	T_SIBD_EVAL_CALC_MPhi calcStrengthMin;
	T_SIBD_EVAL_CALC_DAMAGE_H29 DamageH29Max;
	T_SIBD_EVAL_CALC_DAMAGE_H29 DamageH29Min;
	T_SIBD_EVAL_ANAL_VALUE analStrength;
	T_SIBD_EVAL_RESULT_RC resultRc;

	void Get(T_SIBD_EVAL_REBAR_CUT& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.kElem = kElem;
		ConvertCharStr(strSigrName, rData.strSigrName, sizeof(strSigrName));
		ConvertCharStr(strColmName, rData.strColmName, sizeof(strColmName));
		rData.nRebarCutPos = nRebarCutPos;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nExamShearDir = nExamShearDir;
		rData.calcStrengthMax = calcStrengthMax;
		rData.calcStrengthMin = calcStrengthMin;
		rData.DamageH29Max = DamageH29Max;
		rData.DamageH29Min = DamageH29Min;
		rData.analStrength = analStrength;
		rData.resultRc = resultRc;
	}

	void Set(/*const*/ T_SIBD_EVAL_REBAR_CUT& data)
	{
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		kElem = data.kElem;
		ConvertStrChar(data.strSigrName, strSigrName, sizeof(strSigrName));
		ConvertStrChar(data.strColmName, strColmName, sizeof(strColmName));
		nRebarCutPos = data.nRebarCutPos;
		nSeismicForceDirection = data.nSeismicForceDirection;
		nSeismicForceType = data.nSeismicForceType;
		nExamShearDir = data.nExamShearDir;
		calcStrengthMax = data.calcStrengthMax;
		calcStrengthMin = data.calcStrengthMin;
		DamageH29Max = data.DamageH29Max;
		DamageH29Min = data.DamageH29Min;
		analStrength = data.analStrength;
		resultRc = data.resultRc;
	}
};

struct T_SIBD_EVAL_REBAR_CUT_CH_RW
{
	UINT kSipa;
	UINT kSigr;
	UINT kElem;
	char strSigrName[40];
	char strColmName[60];
	int nRebarCutPos;
	int nSeismicForceDirection;
	int nSeismicForceType;
	int nExamShearDir;
	T_SIBD_EVAL_CALC_MPhi calcStrengthMax;
	T_SIBD_EVAL_CALC_MPhi calcStrengthMin;
	T_SIBD_EVAL_CALC_DAMAGE_H29 DamageH29Max;
	T_SIBD_EVAL_CALC_DAMAGE_H29 DamageH29Min;
	T_SIBD_EVAL_ANAL_VALUE_RW analStrength;
	T_SIBD_EVAL_RESULT_RC resultRc;

	void Get(T_SIBD_EVAL_REBAR_CUT_CH& rData)
	{
		rData.kSipa = kSipa;
		rData.kSigr = kSigr;
		rData.kElem = kElem;
		memcpy(rData.strSigrName, strSigrName, sizeof(strSigrName));
		memcpy(rData.strColmName, strColmName, sizeof(strColmName));
		rData.nRebarCutPos = nRebarCutPos;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		rData.nSeismicForceType = nSeismicForceType;
		rData.nExamShearDir = nExamShearDir;
		rData.calcStrengthMax = calcStrengthMax;
		rData.calcStrengthMin = calcStrengthMin;
		rData.DamageH29Max = DamageH29Max;
		rData.DamageH29Min = DamageH29Min;
		rData.analStrength.dAvergeMax = analStrength.dAvergeMax;
		rData.analStrength.dAvergeMin = analStrength.dAvergeMin;
		rData.resultRc = resultRc;
	}

	void Set(const T_SIBD_EVAL_REBAR_CUT_CH& data)
	{
		kSipa = data.kSipa;
		kSigr = data.kSigr;
		kElem = data.kElem;
		memcpy(strSigrName, data.strSigrName, sizeof(strSigrName));
		memcpy(strColmName, data.strColmName, sizeof(strColmName));
		nRebarCutPos = data.nRebarCutPos;
		nSeismicForceDirection = data.nSeismicForceDirection;
		nSeismicForceType = data.nSeismicForceType;
		nExamShearDir = data.nExamShearDir;
		calcStrengthMax = data.calcStrengthMax;
		calcStrengthMin = data.calcStrengthMin;
		DamageH29Max = data.DamageH29Max;
		DamageH29Min = data.DamageH29Min;
		analStrength.dAvergeMax = data.analStrength.dAvergeMax;
		analStrength.dAvergeMin = data.analStrength.dAvergeMin;
		analStrength.nSmax = (int)data.analStrength.aSmax.GetSize();
		analStrength.nSmin = (int)data.analStrength.aSmin.GetSize();
		resultRc = data.resultRc;
	}
};

#endif // !defined(__DB_ST_DT_SIBD_EVAL__)