#ifndef __DB_ST_DT_EVAL_SUMMARY__
#define __DB_ST_DT_EVAL_SUMMARY__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

struct T_SIMD_SUMM_ITEM
{
	CString strPierName;
	CString strColumnName;
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nSeismicForceDirection;	//지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	double dDeltaMax;
	double dDeltaA;

	T_SIMD_SUMM_ITEM() { Initialize(); }
	T_SIMD_SUMM_ITEM(const T_SIMD_SUMM_ITEM& src) { *this = src; }
	T_SIMD_SUMM_ITEM& operator= (const T_SIMD_SUMM_ITEM& src)
	{
		if (this != &src) {
			strPierName = src.strPierName;
			strColumnName = src.strColumnName;
			nSeismicForceType = src.nSeismicForceType;
			nSeismicForceDirection = src.nSeismicForceDirection;
			dDeltaMax = src.dDeltaMax;
			dDeltaA = src.dDeltaA;
		}
		return *this;
	}

	void Initialize()
	{
		strPierName = _T("");
		strColumnName = _T("");
		nSeismicForceType = 0;
		nSeismicForceDirection = 0;
		dDeltaMax = 0.0;
		dDeltaA = 0.0;
	}
};

struct T_SIMD_SUMM_ITEM_CH
{
	char strPierName[60];
	char strColumnName[60];
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nSeismicForceDirection;	//지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	double dDeltaMax;
	double dDeltaA;

	void Get(T_SIMD_SUMM_ITEM& rData)
	{
		ConvertCharStr(strPierName, rData.strPierName, sizeof(strPierName));
		ConvertCharStr(strColumnName, rData.strColumnName, sizeof(strColumnName));
		rData.nSeismicForceType = nSeismicForceType;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		rData.dDeltaMax = dDeltaMax;
		rData.dDeltaA = dDeltaA;
	}

	void Set(/*const*/T_SIMD_SUMM_ITEM& data)
	{
		ConvertStrChar(data.strPierName, strPierName, sizeof(strPierName));
		ConvertStrChar(data.strColumnName, strColumnName, sizeof(strColumnName));
		nSeismicForceType = data.nSeismicForceType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		dDeltaMax = data.dDeltaMax;
		dDeltaA = data.dDeltaA;
	}
};

struct T_SIMD_SUMM_DATA
{
	CArray<T_SIMD_SUMM_ITEM, T_SIMD_SUMM_ITEM> Left;
	CArray<T_SIMD_SUMM_ITEM, T_SIMD_SUMM_ITEM> Right;

	T_SIMD_SUMM_DATA() { Initialize(); }
	T_SIMD_SUMM_DATA(const T_SIMD_SUMM_DATA& src) { *this = src; }
	T_SIMD_SUMM_DATA& operator= (const T_SIMD_SUMM_DATA& src)
	{
		if (this != &src) {
			Left.Copy(src.Left);
			Right.Copy(src.Right);
		}
		return *this;
	}

	void Initialize()
	{
		Left.RemoveAll();
		Right.RemoveAll();
	}
};

struct T_SIMD_SUMM_DATA_CH
{
	CArray<T_SIMD_SUMM_ITEM_CH, T_SIMD_SUMM_ITEM_CH> Left;
	CArray<T_SIMD_SUMM_ITEM_CH, T_SIMD_SUMM_ITEM_CH> Right;
	
	void Get(T_SIMD_SUMM_DATA& rData)
	{
		rData.Left.SetSize(Left.GetSize());
		for (int i = 0; i < Left.GetSize(); ++i) {
			Left[i].Get(rData.Left[i]);
		}
		rData.Right.SetSize(Right.GetSize());
		for (int i = 0; i < Right.GetSize(); ++i) {
			Right[i].Get(rData.Right[i]);
		}
	}

	void Set(/*const*/T_SIMD_SUMM_DATA& data)
	{
		Left.SetSize(data.Left.GetSize());
		for (int i = 0; i < data.Left.GetSize(); ++i) {
			Left[i].Set(data.Left[i]);
		}
		Right.SetSize(data.Right.GetSize());
		for (int i = 0; i < data.Right.GetSize(); ++i) {
			Right[i].Set(data.Right[i]);
		}
	}
};

struct T_SIMD_SUMM_DATA_CH_RW
{
	int nLeft;
	int nRight;

	void Get(T_SIMD_SUMM_DATA_CH& rData)
	{
	}

	void Set(const T_SIMD_SUMM_DATA_CH& data)
	{
		nLeft = (int)data.Left.GetSize();
		nRight = (int)data.Right.GetSize();
	}
};

struct T_SIMD_SUMM_DATA_RW
{
	int nLeft;
	int nRight;

	void Get(T_SIMD_SUMM_DATA& rData)
	{
	}

	void Set(const T_SIMD_SUMM_DATA& data)
	{
		nLeft = (int)data.Left.GetSize();
		nRight = (int)data.Right.GetSize();
	}
};

struct T_SIRD_SUMM_ITEM
{
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nSeismicForceDirection;	//지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	CString strPierName;	//허용변위산출 교각
	CString strColumnName;	//허용변위산출 기둥
	double dMuR;
	double dDeltaR;
	double dDrataRa;

	T_SIRD_SUMM_ITEM() { Initialize(); }
	T_SIRD_SUMM_ITEM(const T_SIRD_SUMM_ITEM& src) { *this = src; }
	T_SIRD_SUMM_ITEM& operator= (const T_SIRD_SUMM_ITEM& src)
	{
		if (this != &src) {
			nSeismicForceType = src.nSeismicForceType;
			nSeismicForceDirection = src.nSeismicForceDirection;
			strPierName = src.strPierName;
			strColumnName = src.strColumnName;
			dMuR = src.dMuR;
			dDeltaR = src.dDeltaR;
			dDrataRa = src.dDrataRa;
		}
		return *this;
	}

	void Initialize()
	{
		nSeismicForceType = 0;
		nSeismicForceDirection = 0;
		strPierName.Empty();
		strColumnName.Empty();
		dMuR = 0.0;
		dDeltaR = 0.0;
		dDrataRa = 0.0;
	}
};

struct T_SIRD_SUMM_ITEM_CH
{
	int nSeismicForceType;
	int nSeismicForceDirection;
	char strPierName[60];
	char strColumnName[60];
	double dMuR;
	double dDeltaR;
	double dDrataRa;

	void Get(T_SIRD_SUMM_ITEM& rData)
	{
		rData.nSeismicForceType = nSeismicForceType;
		rData.nSeismicForceDirection = nSeismicForceDirection;
		ConvertCharStr(strPierName, rData.strPierName, sizeof(strPierName));
		ConvertCharStr(strColumnName, rData.strColumnName, sizeof(strColumnName));
		rData.dMuR = dMuR;
		rData.dDeltaR = dDeltaR;
		rData.dDrataRa = dDrataRa;
	}

	void Set(/*const*/T_SIRD_SUMM_ITEM& data)
	{
		nSeismicForceType = data.nSeismicForceType;
		nSeismicForceDirection = data.nSeismicForceDirection;
		ConvertStrChar(data.strPierName, strPierName, sizeof(strPierName));
		ConvertStrChar(data.strColumnName, strColumnName, sizeof(strColumnName));
		dMuR = data.dMuR;
		dDeltaR = data.dDeltaR;
		dDrataRa = data.dDrataRa;
	}
};

struct T_SIRD_SUMM_DATA
{
	CArray<T_SIRD_SUMM_ITEM, T_SIRD_SUMM_ITEM> Left;
	CArray<T_SIRD_SUMM_ITEM, T_SIRD_SUMM_ITEM> Right;

	T_SIRD_SUMM_DATA() { Initialize(); }
	T_SIRD_SUMM_DATA(const T_SIRD_SUMM_DATA& src) { *this = src; }
	T_SIRD_SUMM_DATA& operator= (const T_SIRD_SUMM_DATA& src)
	{
		if (this != &src) {
			Left.Copy(src.Left);
			Right.Copy(src.Right);
		}
		return *this;
	}

	void Initialize()
	{
		Left.RemoveAll();
		Right.RemoveAll();
	}
};

struct T_SIRD_SUMM_DATA_CH
{
	CArray<T_SIRD_SUMM_ITEM_CH, T_SIRD_SUMM_ITEM_CH> Left;
	CArray<T_SIRD_SUMM_ITEM_CH, T_SIRD_SUMM_ITEM_CH> Right;

	void Get(T_SIRD_SUMM_DATA& rData)
	{
		rData.Left.SetSize(Left.GetSize());
		for (int i = 0; i < Left.GetSize(); ++i) {
			Left[i].Get(rData.Left[i]);
		}
		rData.Right.SetSize(Right.GetSize());
		for (int i = 0; i < Right.GetSize(); ++i) {
			Right[i].Get(rData.Right[i]);
		}
	}

	void Set(/*const*/T_SIRD_SUMM_DATA& data)
	{
		Left.SetSize(data.Left.GetSize());
		for (int i = 0; i < data.Left.GetSize(); ++i) {
			Left[i].Set(data.Left[i]);
		}
		Right.SetSize(data.Right.GetSize());
		for (int i = 0; i < data.Right.GetSize(); ++i) {
			Right[i].Set(data.Right[i]);
		}
	}
};

struct T_SIRD_SUMM_DATA_CH_RW
{
	int nLeft;
	int nRight;

	void Get(T_SIRD_SUMM_DATA_CH& rData)
	{
	}

	void Set(const T_SIRD_SUMM_DATA_CH& data)
	{
		nLeft = (int)data.Left.GetSize();
		nRight = (int)data.Right.GetSize();
	}
};

//전체계 변위조사
struct T_EVAL_SUMM_OVERALL
{
	T_SIGR_K kSigr;
	T_SIMD_SUMM_DATA SimdSum;
	T_SIRD_SUMM_DATA SirdSum;

	T_EVAL_SUMM_OVERALL() { Initialize(); }
	T_EVAL_SUMM_OVERALL(const T_EVAL_SUMM_OVERALL& src) { *this = src; }
	T_EVAL_SUMM_OVERALL& operator= (const T_EVAL_SUMM_OVERALL& src)
	{
		if (this != &src) {
			kSigr = src.kSigr;
			SimdSum = src.SimdSum;
			SirdSum = src.SirdSum;
		}
		return *this;
	}

	void Initialize()
	{
		kSigr = 0; // 기본 생성자를 호출하여 초기화
		SimdSum.Initialize();
		SirdSum.Initialize();
	}

	BOOL IsExistData() {
		if (SimdSum.Left.GetCount() < 1 && SimdSum.Right.GetCount() < 1 && SirdSum.Left.GetCount() < 1 && SirdSum.Right.GetCount() < 1)
			return FALSE;

		return TRUE;
	}

	BOOL IsExistData(const int& nSeisDir) {
		if (SimdSum.Left.GetCount() < 1 && SimdSum.Right.GetCount() < 1 && SirdSum.Left.GetCount() < 1 && SirdSum.Right.GetCount() < 1)
			return FALSE;

		for (int i = 0; i < SimdSum.Left.GetCount(); i++) {
			if (SimdSum.Left[i].nSeismicForceDirection == nSeisDir)
				return TRUE;
		}
		for (int i = 0; i < SimdSum.Right.GetCount(); i++) {
			if (SimdSum.Right[i].nSeismicForceDirection == nSeisDir)
				return TRUE;
		}
		for (int i = 0; i < SirdSum.Left.GetCount(); i++) {
			if (SirdSum.Left[i].nSeismicForceDirection == nSeisDir)
				return TRUE;
		}
		for (int i = 0; i < SirdSum.Right.GetCount(); i++) {
			if (SirdSum.Right[i].nSeismicForceDirection == nSeisDir)
				return TRUE;
		}

		return FALSE;
	}
};

struct T_EVAL_SUMM_OVERALL_CH
{
	T_SIGR_K kSigr;
	T_SIMD_SUMM_DATA_CH SimdSum;
	T_SIRD_SUMM_DATA_CH SirdSum;

	void Get(T_EVAL_SUMM_OVERALL& rData)
	{
		rData.kSigr = kSigr;
		SimdSum.Get(rData.SimdSum);
		SirdSum.Get(rData.SirdSum);
	}

	void Set(/*const*/ T_EVAL_SUMM_OVERALL& data)
	{
		kSigr = data.kSigr;
		SimdSum.Set(data.SimdSum);
		SirdSum.Set(data.SirdSum);
	}
};

struct T_EVAL_SUMM_OVERALL_CH_RW
{
	T_SIGR_K kSigr;
	T_SIMD_SUMM_DATA_CH_RW SimdSum;
	T_SIRD_SUMM_DATA_CH_RW SirdSum;

	void Get(T_EVAL_SUMM_OVERALL_CH& rData)
	{
		rData.kSigr = kSigr;
		SimdSum.Get(rData.SimdSum);
		SirdSum.Get(rData.SirdSum);
	}

	void Set(/*const*/ T_EVAL_SUMM_OVERALL_CH& data)
	{
		kSigr = data.kSigr;
		SimdSum.Set(data.SimdSum);
		SirdSum.Set(data.SirdSum);
	}
};

//교각별 변위조사
struct T_EVAL_SUMM_PIER
{
	T_SIGR_K kSigr;
	CArray<T_SIMD_SUMM_ITEM, T_SIMD_SUMM_ITEM> SimdSum;
	CArray<T_SIRD_SUMM_ITEM, T_SIRD_SUMM_ITEM> SirdSum;

	T_EVAL_SUMM_PIER() { Initialize(); }
	T_EVAL_SUMM_PIER(const T_EVAL_SUMM_PIER& src) { *this = src; }
	T_EVAL_SUMM_PIER& operator= (const T_EVAL_SUMM_PIER& src)
	{
		if (this != &src) {
			kSigr = src.kSigr;
			SimdSum.Copy(src.SimdSum);
			SirdSum.Copy(src.SirdSum);
		}
		return *this;
	}

	void Initialize()
	{
		kSigr = 0;
		SimdSum.RemoveAll();
		SirdSum.RemoveAll();
	}

	BOOL IsExistData() {
		if (SimdSum.GetCount() < 1 && SirdSum.GetCount() < 1) return FALSE;

		return TRUE;
	}
};

struct T_EVAL_SUMM_PIER_CH
{
	T_SIGR_K kSigr;
	CArray<T_SIMD_SUMM_ITEM_CH, T_SIMD_SUMM_ITEM_CH> SimdSum;
	CArray<T_SIRD_SUMM_ITEM_CH, T_SIRD_SUMM_ITEM_CH> SirdSum;

	void Get(T_EVAL_SUMM_PIER& rData)
	{
		rData.kSigr = kSigr;
		rData.SimdSum.SetSize(SimdSum.GetSize());
		for (int i = 0; i < SimdSum.GetCount(); i++)
			SimdSum[i].Get(rData.SimdSum[i]);
		rData.SirdSum.SetSize(SirdSum.GetSize());
		for (int i = 0; i < SirdSum.GetCount(); i++)
			SirdSum[i].Get(rData.SirdSum[i]);
	}

	void Set(/*const*/T_EVAL_SUMM_PIER& data)
	{
		kSigr = data.kSigr;
		SimdSum.SetSize(data.SimdSum.GetSize());
		for (int i = 0; i < data.SimdSum.GetCount(); i++)
			SimdSum[i].Set(data.SimdSum[i]);
		SirdSum.SetSize(data.SirdSum.GetSize());
		for (int i = 0; i < data.SirdSum.GetCount(); i++)
			SirdSum[i].Set(data.SirdSum[i]);
	}
};

struct T_EVAL_SUMM_PIER_CH_RW
{
	T_SIGR_K kSigr;
	int nSimdSum;
	int nSirdSum;

	void Get(T_EVAL_SUMM_PIER_CH& rData)
	{
		rData.kSigr = kSigr;
	}

	void Set(const T_EVAL_SUMM_PIER_CH& data)
	{
		kSigr = data.kSigr;
		nSimdSum = (int)data.SimdSum.GetSize();
		nSirdSum = (int)data.SirdSum.GetSize();
	}
};

struct T_SIBD_CVSV_ITEM
{
	T_SIBD_K kSibd;
	T_ELEM_K kElem;
	int nMemberType;			//부재 타입 : enum DB_MEMB_TYPE
	int nMaterialType;			//교각의 재료 타입 : enum T_SIGR_LOWER_D::eMaterialType
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nSeismicForceDirection;	//지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	int nExamShearDir;			//조사 방향 : enum T_SIBD_CVSV_D::eCondition
	double dDeltaMax;
	double dDeltaY;
	double dDeltaA;
	double dRatio;
	
	T_SIBD_CVSV_ITEM() { Initialize(); }
	T_SIBD_CVSV_ITEM(const T_SIBD_CVSV_ITEM& src) { *this = src; }
	T_SIBD_CVSV_ITEM& operator= (const T_SIBD_CVSV_ITEM& src)
	{
		if (this != &src) {
			kSibd = src.kSibd;
			kElem = src.kElem;
			nMemberType = src.nMemberType;
			nMaterialType = src.nMaterialType;
			nSeismicForceType = src.nSeismicForceType;
			nSeismicForceDirection = src.nSeismicForceDirection;
			nExamShearDir = src.nExamShearDir;
			dDeltaMax = src.dDeltaMax;
			dDeltaY = src.dDeltaY;
			dDeltaA = src.dDeltaA;
			dRatio = src.dRatio;
		}
		return *this;
	}

	void Initialize()
	{
		kSibd = 0;
		kElem = 0;
		nMemberType = 0;
		nMaterialType = 0;
		nSeismicForceType = 0;
		nSeismicForceDirection = 0;
		nExamShearDir = 0;
		dDeltaMax = 0.0;
		dDeltaY = 0.0;
		dDeltaA = 0.0;
		dRatio = 0.0;
	}
};

struct T_SIBD_CVSV_DATA
{
	CArray<T_SIBD_CVSV_ITEM, T_SIBD_CVSV_ITEM> CvsvMin;
	CArray<T_SIBD_CVSV_ITEM, T_SIBD_CVSV_ITEM> CvsvMax;

	T_SIBD_CVSV_DATA() { Initialize(); }
	T_SIBD_CVSV_DATA(const T_SIBD_CVSV_DATA& src) { *this = src; }
	T_SIBD_CVSV_DATA& operator= (const T_SIBD_CVSV_DATA& src)
	{
		if (this != &src) {
			CvsvMin.Copy(src.CvsvMin);
			CvsvMax.Copy(src.CvsvMax);
		}
		return *this;
	}

	void Initialize()
	{
		CvsvMin.RemoveAll();
		CvsvMax.RemoveAll();
	}
};

struct T_SIBD_CVSV_DATA_RW
{
	int nCvsvMin;
	int nCvsvMax;

	void Get(T_SIBD_CVSV_DATA& rData)
	{
	}

	void Set(const T_SIBD_CVSV_DATA& data)
	{
		nCvsvMin = (int)data.CvsvMin.GetSize();
		nCvsvMax = (int)data.CvsvMax.GetSize();
	}
};

struct T_SIBD_STEL_ITEM
{
	T_ELEM_K kElem;
	int nSurveyType;			// T_SIBD_CVSV_D::eCondition
	int nMemberType;			//부재 타입 : enum DB_MEMB_TYPE
	int nMaterialType;			//교각의 재료 타입 : enum T_SIGR_LOWER_D::eMaterialType
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nSeismicForceDirection;	//지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	int nExamShearDir;			//조사 방향 : enum T_SIBD_CVSV_D::eCondition
	double dDeltaMax;
	double dDeltaYc;
	double dDeltaYt;
	double dDeltaA;
	double dRatioYc;
	double dRatioYt;

	T_SIBD_STEL_ITEM() { Initialize(); }
	T_SIBD_STEL_ITEM(const T_SIBD_STEL_ITEM& src) { *this = src; }
	T_SIBD_STEL_ITEM& operator= (const T_SIBD_STEL_ITEM& src)
	{
		if (this != &src) {
			kElem = src.kElem;
			nSurveyType = src.nSurveyType;
			nMemberType = src.nMemberType;
			nMaterialType = src.nMaterialType;
			nSeismicForceType = src.nSeismicForceType;
			nSeismicForceDirection = src.nSeismicForceDirection;
			nExamShearDir = src.nExamShearDir;
			dDeltaMax = src.dDeltaMax;
			dDeltaYc = src.dDeltaYc;
			dDeltaYt = src.dDeltaYt;
			dDeltaA = src.dDeltaA;
			dRatioYc = src.dRatioYc;
			dRatioYt = src.dRatioYt;
		}
		return *this;
	}

	void Initialize()
	{
		kElem = 0;
		nSurveyType = 0;
		nMemberType = 0;
		nMaterialType = 0;
		nSeismicForceType = 0;
		nSeismicForceDirection = 0;
		nExamShearDir = 0;
		dDeltaMax = 0.0;
		dDeltaYc = 0.0;
		dDeltaYt = 0.0;
		dDeltaA = 0.0;
		dRatioYc = 0.0;
		dRatioYt = 0.0;
	}
};

struct T_SIBD_SRSV_ITEM
{
	T_SIBD_K kSibd;
	T_ELEM_K kElem;
	int nMemberType;
	int nMaterialType;
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nSeismicForceDirection;	//지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	int nExamShearDir;			//조사 방향 : enum T_SIBD_CVSV_D::eCondition
	double dMmax;
	double dMy;
	double dMu;
	double dRatio;

	T_SIBD_SRSV_ITEM() { Initialize(); }
	T_SIBD_SRSV_ITEM(const T_SIBD_SRSV_ITEM& src) { *this = src; }
	T_SIBD_SRSV_ITEM& operator= (const T_SIBD_SRSV_ITEM& src)
	{
		if (this != &src) {
			kSibd = src.kSibd;
			kElem = src.kElem;
			nMemberType = src.nMemberType;
			nMaterialType = src.nMaterialType;
			nSeismicForceType = src.nSeismicForceType;
			nSeismicForceDirection = src.nSeismicForceDirection;
			nExamShearDir = src.nExamShearDir;
			dMmax = src.dMmax;
			dMy = src.dMy;
			dMu = src.dMu;
			dRatio = src.dRatio;
		}
		return *this;
	}

	void Initialize()
	{
		kSibd = 0;
		kElem = 0;
		nMemberType = 0;
		nMaterialType = 0;
		nSeismicForceType = 0;
		nSeismicForceDirection = 0;
		nExamShearDir = 0;
		dMmax = 0.0;
		dMy = 0.0;
		dMu = 0.0;
		dRatio = 0.0;
	}
};

struct T_SIBD_SRSV_DATA
{
	CArray<T_SIBD_SRSV_ITEM, T_SIBD_SRSV_ITEM> SrsvMin;
	CArray<T_SIBD_SRSV_ITEM, T_SIBD_SRSV_ITEM> SrsvMax;

	T_SIBD_SRSV_DATA() { Initialize(); }
	T_SIBD_SRSV_DATA(const T_SIBD_SRSV_DATA& src) { *this = src; }
	T_SIBD_SRSV_DATA& operator= (const T_SIBD_SRSV_DATA& src)
	{
		if (this != &src) {
			SrsvMin.Copy(src.SrsvMin);
			SrsvMax.Copy(src.SrsvMax);
		}
		return *this;
	}

	void Initialize()
	{
		SrsvMin.RemoveAll();
		SrsvMax.RemoveAll();
	}
};

struct T_SIBD_SRSV_DATA_RW
{
	int nSrsvMin;
	int nSrsvMax;

	void Get(T_SIBD_SRSV_DATA& rData)
	{
	}

	void Set(const T_SIBD_SRSV_DATA& data)
	{
		nSrsvMin = (int)data.SrsvMin.GetSize();
		nSrsvMax = (int)data.SrsvMax.GetSize();
	}
};

struct T_SISR_SUMM_ITEM
{
	UINT kElem;
	int nMemberType;			//부재 타입 : enum DB_MEMB_TYPE
	int nMaterialType;			//교각의 재료 타입 : enum T_SIGR_LOWER_D::eMaterialType
	int nSeismicForceType;		//지진력 타입 : enum T_SIPA_D::eSeismicForceType
	int nSeismicForceDirection;	//지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
	int nExamShearDir;			//조사 방향 : enum 
	int	nDesignCriteria;		//적용기준 : enum T_SISR_AUTO_D::eDesignCriteria
	double dSmax;
	double dSus;
	double dSuc;
	double dPs;
	double dRatio;

	T_SISR_SUMM_ITEM() { Initialize(); }
	T_SISR_SUMM_ITEM(const T_SISR_SUMM_ITEM& src) { *this = src; }
	T_SISR_SUMM_ITEM& operator= (const T_SISR_SUMM_ITEM& src)
	{
		if (this != &src) {
			kElem = src.kElem;
			nMemberType = src.nMemberType;
			nMaterialType = src.nMaterialType;
			nSeismicForceType = src.nSeismicForceType;
			nSeismicForceDirection = src.nSeismicForceDirection;
			nExamShearDir = src.nExamShearDir;
			nDesignCriteria = src.nDesignCriteria;
			dSmax = src.dSmax;
			dSus = src.dSus;
			dSuc = src.dSuc;
			dPs = src.dPs;
			dRatio = src.dRatio;
		}
		return *this;
	}

	void Initialize()
	{
		kElem = 0;
		nMemberType = 0;
		nMaterialType = 0;
		nSeismicForceType = 0;
		nSeismicForceDirection = 0;
		nExamShearDir = 0;
		nDesignCriteria = 0;
		dSmax = 0.0;
		dSus = 0.0;
		dSuc = 0.0;
		dPs = 0.0;
		dRatio = 0.0;
	}
};

struct T_SISR_SUMM_DATA
{
	CArray<T_SISR_SUMM_ITEM, T_SISR_SUMM_ITEM> SisrMin;
	CArray<T_SISR_SUMM_ITEM, T_SISR_SUMM_ITEM> SisrMax;

	T_SISR_SUMM_DATA() { Initialize(); }
	T_SISR_SUMM_DATA(const T_SISR_SUMM_DATA& src) { *this = src; }
	T_SISR_SUMM_DATA& operator= (const T_SISR_SUMM_DATA& src)
	{
		if (this != &src) {
			SisrMin.Copy(src.SisrMin);
			SisrMax.Copy(src.SisrMax);
		}
		return *this;
	}

	void Initialize()
	{
		SisrMin.RemoveAll();
		SisrMax.RemoveAll();
	}
};

struct T_SISR_SUMM_DATA_RW
{
	int nSisrMin;
	int nSisrMax;

	void Get(T_SISR_SUMM_DATA& rData)
	{
	}

	void Set(const T_SISR_SUMM_DATA& data)
	{
		nSisrMin = (int)data.SisrMin.GetSize();
		nSisrMax = (int)data.SisrMax.GetSize();
	}
};

//상부구조 조사
struct T_EVAL_SUMM_UPPER
{
	T_SIGR_K kSigr;
	//곡률
	T_SIBD_CVSV_DATA CvsvSum;
	//모멘트
	T_SIBD_SRSV_DATA SrsvSum;
	//전단
	T_SISR_SUMM_DATA SisrSum;

	T_EVAL_SUMM_UPPER() { Initialize(); }
	T_EVAL_SUMM_UPPER(const T_EVAL_SUMM_UPPER& src) { *this = src; }
	T_EVAL_SUMM_UPPER& operator= (const T_EVAL_SUMM_UPPER& src)
	{
		if (this != &src) {
			kSigr = src.kSigr;
			CvsvSum = src.CvsvSum;
			SrsvSum = src.SrsvSum;
			SisrSum = src.SisrSum;
		}
		return *this;
	}

	void Initialize()
	{
		kSigr = 0;
		CvsvSum.Initialize();
		SrsvSum.Initialize();
		SisrSum.Initialize();
	}

	BOOL IsExistData() {
		if (CvsvSum.CvsvMin.GetCount() < 1 && CvsvSum.CvsvMax.GetCount() < 1 &&
			SrsvSum.SrsvMax.GetCount() < 1 && SrsvSum.SrsvMin.GetCount() < 1 &&
			SisrSum.SisrMin.GetCount() < 1 && SisrSum.SisrMax.GetCount() < 1
			) {
			return FALSE;
		}

		return TRUE;
	}
};

struct T_EVAL_SUMM_UPPER_RW
{
	T_SIGR_K kSigr;
	T_SIBD_CVSV_DATA_RW CvsvSum;
	T_SIBD_SRSV_DATA_RW SrsvSum;
	T_SISR_SUMM_DATA_RW SisrSum;

	void Get(T_EVAL_SUMM_UPPER& rData)
	{
		rData.kSigr = kSigr;
		CvsvSum.Get(rData.CvsvSum);
		SrsvSum.Get(rData.SrsvSum);
		SisrSum.Get(rData.SisrSum);
	}

	void Set(const T_EVAL_SUMM_UPPER& data)
	{
		kSigr = data.kSigr;
		CvsvSum.Set(data.CvsvSum);
		SrsvSum.Set(data.SrsvSum);
		SisrSum.Set(data.SisrSum);
	}
};

//하부구조 조사
struct T_EVAL_SUMM_LOWER
{
	T_SIGR_K kSigr;
	//곡률
	T_SIBD_CVSV_DATA CvsvSum;
	//전단
	T_SISR_SUMM_DATA SisrSum;
	//휨(철근 단락부)
	T_SIBD_SRSV_DATA RebarCutSrsvSum;
	//전단(철근 단락부)
	T_SISR_SUMM_DATA RebarCutSisrSum;
	//곡률(Steel)
	CArray<T_SIBD_STEL_ITEM, T_SIBD_STEL_ITEM> StelSum;

	T_EVAL_SUMM_LOWER() { Initialize(); }
	T_EVAL_SUMM_LOWER(const T_EVAL_SUMM_LOWER& src) { *this = src; }
	T_EVAL_SUMM_LOWER& operator= (const T_EVAL_SUMM_LOWER& src)
	{
		if (this != &src) {
			kSigr = src.kSigr;
			CvsvSum = src.CvsvSum;
			SisrSum = src.SisrSum;
			RebarCutSrsvSum = src.RebarCutSrsvSum;
			RebarCutSisrSum = src.RebarCutSisrSum;
			StelSum.Copy(src.StelSum);
		}
		return *this;
	}

	void Initialize()
	{
		kSigr = 0;
		CvsvSum.Initialize();
		SisrSum.Initialize();
		RebarCutSrsvSum.Initialize();
		RebarCutSisrSum.Initialize();
		StelSum.RemoveAll();
	}

	BOOL IsExistData() {
		if (CvsvSum.CvsvMin.GetCount() < 1 && CvsvSum.CvsvMax.GetCount() < 1 &&
			SisrSum.SisrMin.GetCount() < 1 && SisrSum.SisrMax.GetCount() < 1 &&
			RebarCutSrsvSum.SrsvMin.GetCount() < 1 && RebarCutSrsvSum.SrsvMax.GetCount() < 1 &&
			RebarCutSisrSum.SisrMin.GetCount() < 1 && RebarCutSisrSum.SisrMax.GetCount() < 1 &&
			StelSum.GetCount() < 1
			) {
			return FALSE;
		}

		return TRUE;
	}
};

struct T_EVAL_SUMM_LOWER_RW
{
	T_SIGR_K kSigr;
	T_SIBD_CVSV_DATA_RW CvsvSum;
	T_SISR_SUMM_DATA_RW SisrSum;
	T_SIBD_SRSV_DATA_RW RebarCutSrsvSum;
	T_SISR_SUMM_DATA_RW RebarCutSisrSum;
	int nStelSum;

	void Get(T_EVAL_SUMM_LOWER& rData)
	{
		rData.kSigr = kSigr;
		CvsvSum.Get(rData.CvsvSum);
		SisrSum.Get(rData.SisrSum);
		RebarCutSrsvSum.Get(rData.RebarCutSrsvSum);
		RebarCutSisrSum.Get(rData.RebarCutSisrSum);
	}

	void Set(const T_EVAL_SUMM_LOWER& data)
	{
		kSigr = data.kSigr;
		CvsvSum.Set(data.CvsvSum);
		SisrSum.Set(data.SisrSum);
		RebarCutSrsvSum.Set(data.RebarCutSrsvSum);
		RebarCutSisrSum.Set(data.RebarCutSisrSum);
		nStelSum = (int)data.StelSum.GetSize();
	}
};

#endif // !defined(__DB_ST_DT_EVAL_SUMMARY__)