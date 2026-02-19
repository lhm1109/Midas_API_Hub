#pragma once

#include "SeismicDesignSisrBase.h"

struct T_ELEM_D;
struct T_MATD_D;
struct T_SISR_D;
struct T_SIPA_D;
struct T_REIN_SSM_D;
struct T_SISR_EVAL_ROAD3;
struct T_SISR_EVAL_LOWER;
struct T_SISR_EVAL_LOWER_POS;

class CDgnProgressDlg;

class CRoad3Param_H14 : public iRoad3Param
{
public:
	CRoad3Param_H14() : iRoad3Param()
	{
		Init();
	};
	virtual ~CRoad3Param_H14() {};

	virtual void Init()
	{
		dMd = 0.0;
		dM0 = 0.0;
		dN = 0.0;
		dSp = 0.0;
		dSigmace = 0.0;
	}
};

class CSeismicDesignSisr_H14 : public iSeismicDesignSisr, public CSeismicDesignBase
{
public:
	CSeismicDesignSisr_H14(void);
	virtual ~CSeismicDesignSisr_H14(void);

	virtual void AddProgressStep(CDgnProgressDlg* pProgress);
	virtual BOOL Calculate(CDgnProgressDlg* pProgress);
};

class CSeismicDesignSisr_H14_Road3 : public CSeismicDesignSisrBase
{
protected: //계산과정 중 파리미터를 줄이기 위한 임시 멤버 변수들
	T_ELEM_K m_ElemK;
	T_ELEM_D* m_pElem;
	T_MATD_D* m_pMatd;
	T_SISR_D* m_pSisr;
	T_SIPA_D* m_pSipa;

public:
	CSeismicDesignSisr_H14_Road3();
	virtual ~CSeismicDesignSisr_H14_Road3();

	void AddProgressStep(const T_SISR_K& kSisr);
	BOOL Calculate(const T_SISR_K& kSisr);

protected:
	virtual void CalcRatio(const T_SISR_K& SisrK, const T_ELEM_K& kElem, const T_THIS_K_LIST& aThisK, const int nDir,
		const T_SISR_PARAM_POS aSisrParam[2], const T_SISR_PARAM_BASE aTndnParam[2], OUT T_SISR_EVAL_ROAD3& rEval);

private:
	void Init();
	void RunEvaluation(const P_INPUT* pInput, const T_ELEM_K& kElem, const int& nExamShearDir);

	void CalcRoad3Param(const T_SISR_K& SisrK, T_ELEM_K ElemK, int nSeismicForceDirection, int nDir,
						const T_THIS_K_LIST& aThisK, const T_SISR_PARAM_POS aSisrParam[2], 
						OUT CRoad3Param_H14 aParamMax[2], OUT CRoad3Param_H14 aParamMin[2]);
	
	void GetN_fromInitialLoad(const T_ELEM_K& ElemK, OUT double aMax[2], OUT double aMin[2]);
	void GetN_fromLoadCase(const int& nSubsequentType, const T_ELEM_K& ElemK, const T_KEY& kLoadCase, OUT double aMax[2], OUT double aMin[2]);
	void GetN_fromInitElemTable(const T_ELEM_K& ElemK, OUT double aMax[2], OUT double aMin[2]);
	void GetN_fromGridModel(const T_ELEM_K& ElemK, const CArray<T_GILC_K, T_GILC_K>& gilcKeys, OUT double aMax[2], OUT double aMin[2]);

	double CalcTauc(double dSigmack);
protected:
	void GetSp(const T_SISR_K& SisrK, const T_ELEM_K& ElemK, const int& nExamDir,
		OUT iRoad3Param* pMax_I, OUT iRoad3Param* pMax_J, OUT iRoad3Param* pMin_I, OUT iRoad3Param* pMin_J);
	void GetMd(const T_ELEM_K& ElemK, const int& nExamDir, const T_THIS_K_LIST& aThisK,
		OUT iRoad3Param* pMax_I, OUT iRoad3Param* pMax_J, OUT iRoad3Param* pMin_I, OUT iRoad3Param* pMin_J);
	void GetMo(const T_ELEM_K& ElemK, const int& nSeismicForceDir, const int& nShearDir, const T_THIS_K_LIST& aThisK, const T_SISR_PARAM_POS aSisrParam[2],
		OUT iRoad3Param* pMax_I, OUT iRoad3Param* pMax_J, OUT iRoad3Param* pMin_I, OUT iRoad3Param* pMin_J);
	void GetN(const T_ELEM_K& ElemK, const T_THIS_K_LIST& aThisK,
		OUT iRoad3Param* pMax_I, OUT iRoad3Param* pMax_J, OUT iRoad3Param* pMin_I, OUT iRoad3Param* pMin_J);

	double CalcTaumax(double dSigmack);
};

class CSeismicDesignSisr_H14_Road4 : public CSeismicDesignSisrBase
{
protected: //계산과정 중 파리미터를 줄이기 위한 임시 멤버 변수들
	T_ELEM_K m_ElemK;
	T_ELEM_D* m_pElem;
	T_MATD_D* m_pMatd;
	T_SISR_D* m_pSisr;
	T_SIPA_D* m_pSipa;
	T_REIN_SSM_D* m_pReinSsm;

public:
	CSeismicDesignSisr_H14_Road4();
	virtual ~CSeismicDesignSisr_H14_Road4();

	void AddProgressStep(const T_SISR_K& kSisr);
	BOOL Calculate(const T_SISR_K& kSisr);

protected:
	virtual void CalcRatio(const T_SISR_K& SisrK, const T_ELEM_K& kElem, const int nDir, const int nSeisForceType,
		const T_SISR_PARAM_POS aParam[2], OUT T_SISR_EVAL_LOWER& rEval);

	double CalcCe(double dd);
	double CalcCpt(double dpt);
	double CalcCN();
	double CalcTauc(double dSigmack);

private:
	void Init();
	void RunEvaluation(const P_INPUT* pInput, const T_ELEM_K& kElem, const int& nExamShearDir);
};

class CSeismicDesignSisr_H14_Road5 : public CSeismicDesignSisrBase
{
protected: //계산과정 중 파리미터를 줄이기 위한 임시 멤버 변수들
	T_ELEM_K m_ElemK;
	T_ELEM_D* m_pElem;
	T_MATD_D* m_pMatd;
	T_SISR_D* m_pSisr;
	T_SIPA_D* m_pSipa;
	T_REIN_SSM_D* m_pReinSsm;

public:
	CSeismicDesignSisr_H14_Road5();
	virtual ~CSeismicDesignSisr_H14_Road5();

	void AddProgressStep(const T_SISR_K& kSisr);
	BOOL Calculate(const T_SISR_K& kSisr);

protected:
	virtual void CalcRatio(const T_SISR_K& SisrK, const T_ELEM_K& kElem, const int nDir, const int nSeisForceType,
		const T_SISR_PARAM_POS aParam[2], OUT T_SISR_EVAL_LOWER& rEval);

	double CalcCe(double dd);
	double CalcCpt(double dpt);
	double CalcTauc(double dSigmack);

private:
	void Init();
	void RunEvaluation(const P_INPUT* pInput, const T_ELEM_K& kElem, const int& nExamShearDir);
};