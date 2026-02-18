#pragma once

#include "SeismicDesignSisrBase.h"
#include "SeismicDesignSisr_H14.h"

struct T_ELEM_D;
struct T_MATD_D;
struct T_SISR_D;
struct T_SIPA_D;
struct T_REIN_SSM_D;
struct T_SISR_EVAL_ROAD3;
struct T_SISR_EVAL_LOWER;
struct T_SISR_EVAL_LOWER_POS;

class CDgnProgressDlg;

class CSeismicDesignSisr_H24 : public iSeismicDesignSisr, public CSeismicDesignBase
{
public:
	CSeismicDesignSisr_H24(void);
	virtual ~CSeismicDesignSisr_H24(void);

	virtual void AddProgressStep(CDgnProgressDlg* pProgress);
	virtual BOOL Calculate(CDgnProgressDlg* pProgress);
};

class CSeismicDesignSisr_H24_Road3 : public CSeismicDesignSisr_H14_Road3
{
public:
	CSeismicDesignSisr_H24_Road3();
	virtual ~CSeismicDesignSisr_H24_Road3();

	void AddProgressStep(const T_SISR_K& kSisr);
	BOOL Calculate(const T_SISR_K& kSisr);
};

class CSeismicDesignSisr_H24_Road4 : public CSeismicDesignSisr_H14_Road4
{
public:
	CSeismicDesignSisr_H24_Road4();
	virtual ~CSeismicDesignSisr_H24_Road4();

	void AddProgressStep(const T_SISR_K& kSisr);
	BOOL Calculate(const T_SISR_K& kSisr);

protected:
	virtual void CalcRatio(const T_SISR_K& SisrK, const T_ELEM_K& kElem, const int nDir, const int nSeisForceType,
		const T_SISR_PARAM_POS aParam[2], OUT T_SISR_EVAL_LOWER& rEval);
};

class CSeismicDesignSisr_H24_Road5 : public CSeismicDesignSisr_H14_Road5
{
public:
	CSeismicDesignSisr_H24_Road5();
	virtual ~CSeismicDesignSisr_H24_Road5();

	void AddProgressStep(const T_SISR_K& kSisr);
	BOOL Calculate(const T_SISR_K& kSisr);

protected:
	virtual void CalcRatio(const T_SISR_K& SisrK, const T_ELEM_K& kElem, const int nDir, const int nSeisForceType,
		const T_SISR_PARAM_POS aParam[2], OUT T_SISR_EVAL_LOWER& rEval);
};