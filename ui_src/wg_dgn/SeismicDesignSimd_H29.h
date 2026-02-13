#pragma once

#include "SeismicDesignSimd.h"

class CSeismicDesignSimd_H29 : public iSeismicDesignSimd, public CSeismicDesignSimd
{
public:
	CSeismicDesignSimd_H29();
	virtual ~CSeismicDesignSimd_H29() {};

	virtual void AddProgressStep(CDgnProgressDlg* pProgress);
	virtual BOOL Calculate(CDgnProgressDlg* pProgress);

private:
	void CalculateUpper(CDgnProgressDlg* pProgress, const T_SIMD_K& kSimd);
	void CalculateLower(CDgnProgressDlg* pProgress, const T_SIMD_K& kSimd);

protected:
	T_NODE_K m_kPierTopNode;
	T_SIGR_LOWER_D* m_pSigrLower;
	T_SIGR_LOWER_COLM_D* m_pColumn;

	BOOL GetDelta_yEd_fromAnalysis(IN T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT double& rDelta_yEd);
};

class CSeismicDesignSimd_H29_Upper : public CSeismicDesignSimd_H29
{
private:
	P_SIMD_Upper* m_PUpper;

public:
	CSeismicDesignSimd_H29_Upper(CDgnProgressDlg* pProgress);
	virtual ~CSeismicDesignSimd_H29_Upper() {};

	virtual BOOL Calculate(const T_SIMD_K& kSimd, T_SIMD_EVAL* pEval);

	virtual BOOL CalcDeltaA(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA);
private:
	BOOL CalcDeltaLsdByTotalDistance(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA);
	BOOL CalcDeltaLsdByPureDistance(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA);
};

class CSeismicDesignSimd_H29_Lower : public CSeismicDesignSimd_H29
{
private:
	P_SIMD_Lower* m_PLower;
	CSeismicDesignDisp_Lower m_DispL;

public:
	CSeismicDesignSimd_H29_Lower(CDgnProgressDlg* pProgress);
	virtual ~CSeismicDesignSimd_H29_Lower() {};

	virtual BOOL Calculate(const T_SIMD_K& kSimd, T_SIMD_EVAL* pEval);

	virtual BOOL CalcDeltaA(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA);

private:
	BOOL CalcDeltaA_fromAnalysis(OUT doubleArray& aDeltaA);
	BOOL CalcDeltaA_fromAnalysis_forRc(OUT doubleArray& aDeltaA);
	BOOL CalcDeltaA_fromAnalysis_forSteel(OUT doubleArray& aDeltaA);

	BOOL CalcDeltaA_fromkMphi_Dy(OUT doubleArray& aDeltaA);
	BOOL CalcDeltaA_fromkMphi_Dy_forRc(OUT doubleArray& aDeltaA);
	BOOL CalcDeltaA_fromkMphi_Dy_forSteel(OUT doubleArray& aDeltaA);
	BOOL GetDelta_yEd_fromMphi(OUT doubleArray& aDeltaA);

	BOOL CalcDeltaA_fromkMphi_Dpy(OUT doubleArray& aDeltaA);
};