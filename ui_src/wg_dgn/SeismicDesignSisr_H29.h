#pragma once

#include "SeismicDesignSisr_H14.h"

class CRoad3Param_H29 : public iRoad3Param
{
public:
	double dSd;

	CRoad3Param_H29() : iRoad3Param()
	{
		Init();
	};
	virtual ~CRoad3Param_H29() {};

	virtual void Init()
	{
		dMd = 0.0;
		dM0 = 0.0;
		dN = 0.0;
		dSp = 0.0;
		dSd = 0.0;
	}
};

class CSeismicDesignSisr_H29 : public iSeismicDesignSisr, public CSeismicDesignBase
{
public:
	CSeismicDesignSisr_H29(void);
	virtual ~CSeismicDesignSisr_H29(void);

	virtual void AddProgressStep(CDgnProgressDlg* pProgress);
	virtual BOOL Calculate(CDgnProgressDlg* pProgress);
};

class CSeismicDesignSisr_H29_Road3 : public CSeismicDesignSisr_H14_Road3
{
public:
	CSeismicDesignSisr_H29_Road3();
	virtual ~CSeismicDesignSisr_H29_Road3();

	void AddProgressStep(const T_SISR_K& kSisr);
	BOOL Calculate(const T_SISR_K& kSisr);

protected:
	virtual void CalcRatio(const T_SISR_K& SisrK, const T_ELEM_K& kElem, const T_THIS_K_LIST& aThisK, const int nDir,
		const T_SISR_PARAM_POS aSisrParam[2], const T_SISR_PARAM_BASE aTndnParam[2], OUT T_SISR_EVAL_ROAD3& rEval);

private:
	void CalcRoad3Param(const T_SISR_K& SisrK, T_ELEM_K ElemK, int nSeismicForceDirection, int nDir,
		const T_THIS_K_LIST& aThisK, const T_SISR_PARAM_POS aSisrParam[2],
		OUT CRoad3Param_H29 aParamMax[2], OUT CRoad3Param_H29 aParamMin[2]);
	
	double GetTau_c(const double& dSigmack);
	double GetCe(const double& dH);
	double GetCdc(const T_ELEM_K& kElem, OUT T_SISR_PARAM_BASE& param);
	
protected:
	double CalcTau_r(const T_ELEM_K& kElem, const T_SIPA_K& kSipa, IN T_SISR_PARAM_BASE& param);
	
	virtual double GetCpt(const double& dPt);
	virtual double GetCc(int nSeisForcType);

	void GetSd(const T_ELEM_K& ElemK, const int& nExamDir, const T_THIS_K_LIST& aThisK,
		OUT CRoad3Param_H29 aParamMax[2], OUT CRoad3Param_H29 aParamMin[2]);
};

class CSeismicDesignSisr_H29_Road4 : public CSeismicDesignSisr_H29_Road3
{
public:
	CSeismicDesignSisr_H29_Road4();
	virtual ~CSeismicDesignSisr_H29_Road4();

	void AddProgressStep(const T_SISR_K& kSisr);
	BOOL Calculate(const T_SISR_K& kSisr);

private:
	void RunEvaluation(const P_INPUT* pInput, const T_ELEM_K& kElem, const int& nExamShearDir);

	void CalcRatio(const P_INPUT* pInput, const T_ELEM_K& kElem, const int nDir,
		T_SISR_PARAM_POS aParam[2], OUT T_SISR_EVAL_ROAD3& rEval);

	void GetMo( const T_ELEM_K& ElemK,
				const int& nSeismicForceDir,
				const int& nShearDir,
				const T_THIS_K_LIST& aThisK,
				const T_SISR_PARAM_POS aSisrParam[2],
				OUT iRoad3Param* pMax_I,
				OUT iRoad3Param* pMax_J,
				OUT iRoad3Param* pMin_I,
				OUT iRoad3Param* pMin_J);

protected:
	virtual void CalcRoad3Param(const T_SISR_K& SisrK, T_ELEM_K ElemK, int nSeismicForceDirection, int nDir,
		const T_THIS_K_LIST& aThisK, const T_SISR_PARAM_POS aSisrParam[2], OUT CRoad3Param_H29 aParamMax[2], OUT CRoad3Param_H29 aParamMin[2]);

	virtual double GetCpt(const double& dPt);
};

class CSeismicDesignSisr_H29_Road5 : public CSeismicDesignSisr_H29_Road4
{
public:
	CSeismicDesignSisr_H29_Road5();
	virtual ~CSeismicDesignSisr_H29_Road5();

	void AddProgressStep(const T_SISR_K& kSisr);
	BOOL Calculate(const T_SISR_K& kSisr);

protected:
	virtual void CalcRoad3Param(const T_SISR_K& SisrK, T_ELEM_K ElemK, int nSeismicForceDirection, int nDir,
		const T_THIS_K_LIST& aThisK, const T_SISR_PARAM_POS aSisrParam[2], OUT CRoad3Param_H29 aParamMax[2], OUT CRoad3Param_H29 aParamMin[2]);
};