#pragma once

#include "SeismicDesignSimd.h"

class CSeismicDesignSimd_H14 : public iSeismicDesignSimd, public CSeismicDesignSimd
{
public:
	CSeismicDesignSimd_H14() {};
	virtual ~CSeismicDesignSimd_H14() {};

	virtual void AddProgressStep(CDgnProgressDlg* pProgress);
	virtual BOOL Calculate(CDgnProgressDlg* pProgress);

private:
	void CalculateUpper(CDgnProgressDlg* pProgress, const T_SIMD_K& kSimd);
	void CalculateLower(CDgnProgressDlg* pProgress, const T_SIMD_K& kSimd);
};

class CSeismicDesignSimd_H14_Upper : public CSeismicDesignSimd
{
private:
	P_SIMD_Upper* m_PUpper;

public:
	CSeismicDesignSimd_H14_Upper(CDgnProgressDlg* pProgress);
	virtual ~CSeismicDesignSimd_H14_Upper() {};

	virtual BOOL Calculate(const T_SIMD_K& kSimd, T_SIMD_EVAL* pEval);

	virtual BOOL CalcDeltaA(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA);

private:
	BOOL CalcDeltaA_forRc(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA);
	BOOL CalcDeltaA_forSteel(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA);
};

class CSeismicDesignSimd_H14_Lower : public CSeismicDesignSimd
{
private:
	P_SIMD_Lower* m_PLower;
	CSeismicDesignDisp_Lower m_DispL;

public:
	CSeismicDesignSimd_H14_Lower(CDgnProgressDlg* pProgress);
	virtual ~CSeismicDesignSimd_H14_Lower() {};

	virtual BOOL Calculate(const T_SIMD_K& kSimd, T_SIMD_EVAL* pEval);

	virtual BOOL CalcDeltaA(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA);

private:
	BOOL CalcDeltaA_fromAnalysis(OUT doubleArray& aDeltaA);
	BOOL CalcDeltaA_fromAnalysis_forRc(OUT doubleArray& aDeltaA);
	BOOL CalcDeltaA_fromAnalysis_forSteel(OUT doubleArray& aDeltaA);

	BOOL CalcDeltaA_fromkMphi_Dy(OUT doubleArray& aDeltaA);
	BOOL CalcDeltaA_fromkMphi_Dy_forRc(OUT doubleArray& aDeltaA);
	BOOL CalcDeltaA_fromkMphi_Dy_forSteel(OUT doubleArray& aDeltaA);

	BOOL CalcDeltaA_fromkMphi_Dpy(OUT doubleArray& aDeltaA);
};