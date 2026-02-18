#pragma once

#include "SeismicDesignSird.h"

class CSeismicDesignSird_H14 : public iSeismicDesignSird, public CSeismicDesignSird
{
public:
	CSeismicDesignSird_H14() {};
	virtual ~CSeismicDesignSird_H14() {};

	virtual void AddProgressStep(CDgnProgressDlg* pProgress);
	virtual BOOL Calculate(CDgnProgressDlg* pProgress);

	static BOOL Get_Cr(const T_SIGR_LOWER_K& kSigrLower, OUT double& d_Cr);
	static BOOL Get_r(const T_SIGR_LOWER_K& kSigrLower, OUT double& d_r);

private:
	void CalculateUpper(CDgnProgressDlg* pProgress, const T_SIRD_K& kSird);
	void CalculateLower(CDgnProgressDlg* pProgress, const T_SIRD_K& kSird);
};

class CSeismicDesignSird_H14_Upper : public CSeismicDesignSird_H14
{
public:
	CSeismicDesignSird_H14_Upper(CDgnProgressDlg* pProgress);
	virtual ~CSeismicDesignSird_H14_Upper() {};

	virtual BOOL Calculate(const T_SIRD_K& kSimd, T_SIRD_EVAL* pEval);

	virtual BOOL CalcDeltaMax(IN T_SIRD_EVAL* pEval, const T_THIS_K& kThis, const int nTimeIdx, OUT double& rDeltaMax);

	virtual BOOL CalcDelta_y(const T_THIS_K& kThis, T_SIRD_RSDP_D* pRsdp, OUT double& rDeltaY);

private:
	BOOL GetDispFromAnalsys(const T_THIS_K& kThis, const T_SIRD_RSDP_D* pRsdp, const T_NODE_K& kNode, OUT double& dDisp);
};

class CSeismicDesignSird_H14_Lower : public CSeismicDesignSird_H14
{
private:
	P_SIRD_Lower* m_PLower;
	CSeismicDesignDisp_Lower m_DispL;

public:
	CSeismicDesignSird_H14_Lower(CDgnProgressDlg* pProgress);
	virtual ~CSeismicDesignSird_H14_Lower() {};

	virtual BOOL Calculate(const T_SIRD_K& kSimd, T_SIRD_EVAL* pEval);

	virtual BOOL CalcDeltaMax(IN T_SIRD_EVAL* pEval, const T_THIS_K& kThis, const int nTimeIdx, OUT double& rDeltaMax);

	virtual BOOL CalcDelta_y(const T_THIS_K& kThis_Seismic, T_SIRD_RSDP_D* pRsdp, OUT double& rDeltaY);

private:
	BOOL CalcDeltaY_fromAnalysis(const T_THIS_K& kThis_Seismic, const T_SIRD_RSDP_D* pRsdp, OUT double& rDeltaY);
	BOOL CalcDeltaY_fromkMphi_Dy(const T_SIRD_RSDP_D* pRsdp, OUT double& rDeltaY);
	BOOL CalcDeltaA_fromkMphi_Dy_forRc(const T_SIRD_RSDP_D* pRsdp, OUT double& rDeltaY);
	BOOL CalcDeltaA_fromkMphi_Dy_forSteel(const T_SIRD_RSDP_D* pRsdp, OUT double& rDeltaY);
	BOOL CalcDeltaY_fromkMphi_Dpy(const T_SIRD_RSDP_D* pRsdp, OUT double& rDeltaY);
};