#ifndef __DB_ST_DT_SISR_EVAL_COMPRESS__
#define __DB_ST_DT_SISR_EVAL_COMPRESS__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

struct T_SISR_EVAL_COMPRESS_CALC_CAPACITY
{
	double dBw;
	double dD;
	double dTau_rMax;
	double dAp;
	double dSigma_pe;
	double dSucw;
	double dSp;
	double dSuc;

	T_SISR_EVAL_COMPRESS_CALC_CAPACITY() { Initialize(); }
	T_SISR_EVAL_COMPRESS_CALC_CAPACITY(const T_SISR_EVAL_COMPRESS_CALC_CAPACITY& src) { *this = src; }
	T_SISR_EVAL_COMPRESS_CALC_CAPACITY& operator= (const T_SISR_EVAL_COMPRESS_CALC_CAPACITY& src)
	{
		if (this != &src) {
			dBw = src.dBw;
			dD = src.dD;
			dTau_rMax = src.dTau_rMax;
			dAp = src.dAp;
			dSigma_pe = src.dSigma_pe;
			dSucw = src.dSucw;
			dSp = src.dSp;
			dSuc = src.dSuc;
		}
		return *this;
	}

	void Initialize()
	{
		dBw = 0;
		dD = 0;
		dTau_rMax = 0;
		dAp = 0;
		dSigma_pe = 0;
		dSucw = 0;
		dSp = 0;
		dSuc = 0;
	}
};

struct T_SISR_EVAL_COMPRESS_ANAL_CAPACITY
{
	CArray<double, double> aSmin;
	CArray<double, double> aSmax;
	double dAvergeMin;
	double dAvergeMax;
	double dShMin;
	double dShMax;

	T_SISR_EVAL_COMPRESS_ANAL_CAPACITY() { Initialize(); }
	T_SISR_EVAL_COMPRESS_ANAL_CAPACITY(const T_SISR_EVAL_COMPRESS_ANAL_CAPACITY& src) { *this = src; }
	T_SISR_EVAL_COMPRESS_ANAL_CAPACITY& operator= (const T_SISR_EVAL_COMPRESS_ANAL_CAPACITY& src)
	{
		if (this != &src) {
			aSmin.Copy(src.aSmin);
			aSmax.Copy(src.aSmax);
			dAvergeMin = src.dAvergeMin;
			dAvergeMax = src.dAvergeMax;
			dShMin = src.dShMin;
			dShMax = src.dShMax;
		}
		return *this;
	}

	void Initialize()
	{
		dAvergeMin = 0;
		dAvergeMax = 0;
		aSmin.RemoveAll();
		aSmax.RemoveAll();
		dShMin = 0;
		dShMax = 0;
	}
};

//PC상부공에 대한 전단조사 - 상부구조 사인장파괴(H14/24 공통) diagonal tension failure
struct T_SISR_EVAL_COMPRESS
{
	UINT kElem;
	T_SISR_EVAL_COMPRESS_CALC_CAPACITY calcCapa;
	T_SISR_EVAL_COMPRESS_ANAL_CAPACITY analCapa;
	double dRatioMin;
	double dRatioMax;
	BOOL bCheckResultMin;
	BOOL bCheckResultMax;

	T_SISR_EVAL_COMPRESS() { Initialize(); }
	T_SISR_EVAL_COMPRESS(const T_SISR_EVAL_COMPRESS& src) { *this = src; }
	T_SISR_EVAL_COMPRESS& operator= (const T_SISR_EVAL_COMPRESS& src)
	{
		if (this != &src) {
			kElem = src.kElem;
			calcCapa = src.calcCapa;
			analCapa = src.analCapa;
			dRatioMin = src.dRatioMin;
			dRatioMax = src.dRatioMax;
			bCheckResultMin = src.bCheckResultMin;
			bCheckResultMax = src.bCheckResultMax;
		}
		return *this;
	}

	void Initialize()
	{
		kElem = 0;
		calcCapa.Initialize();
		analCapa.Initialize();
		bCheckResultMin = FALSE;
		bCheckResultMax = FALSE;
		dRatioMin = 0;
		dRatioMax = 0;
	}
};

#endif // !defined(__DB_ST_DT_SISR_EVAL_COMPRESS__)