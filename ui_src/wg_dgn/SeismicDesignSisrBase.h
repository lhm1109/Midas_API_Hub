#pragma once

#include "SeismicDesignBase.h"
#include "..\wg_db\SeismicDesignSisrParam.h"

class CDBDoc;
struct T_SISR_EVAL_LOWER;
struct T_SISR_EVAL_ROAD3;

struct P_INPUT
{
private:
	BOOL bValidValue;
public:
	T_SISR_K kSisr;
	T_SIPA_K kSipa;
	T_SIGR_K kSigr;

	T_SISR_D tSisr;
	T_SIPA_D tSipa;
	T_SIGR_D tSigr;
	T_SIGR_USER_D  tUser;
	T_SIGR_UPPER_D tUpper;
	T_SIGR_LOWER_D tLower;

	P_INPUT(const T_SISR_K& sisrKey);
	~P_INPUT();

	BOOL IsValidValue() { return bValidValue; };
};

class iRoad3Param
{
public:
	double dMd;	//부재 단면에 작용하는 휨 모멘트(종극휨 모멘트)
	double dM0;	//프레스트레스 및 축방향력에 의해 콘크리트의 응력도가 부재 인장연에서 0이 되는 휨 모멘트
	double dN;	//부재 단면에 작용하는 축방향력(사하중 작용시), 단, 프레스트레스력은 포함되지 않는다.
	double dSp;	//전단내력 산출시 전단 스팬비에 의해 콘크리트가 부담하는 전단력의 할증 계수.
	double dSigmace;

protected:
	iRoad3Param() {}
public:
	virtual ~iRoad3Param() {}

	virtual void Init() = 0;
};

class CSeismicDesignSisrBase : public CSeismicDesignBase
{
public:
	CSeismicDesignSisrBase(void);
	virtual ~CSeismicDesignSisrBase(void);

protected:	
	void CalcForce(const T_ELEM_K& kElem, const T_THIS_K_LIST& aThisK, const int nDirShear, OUT T_SISR_EVAL_LOWER& rEval);
	void CalcForce(const T_ELEM_K& kElem, const T_THIS_K_LIST& aThisK, const int nDirShear, OUT T_SISR_EVAL_ROAD3& rEval);

	const BOOL GetSisrParamBase(const T_SISR_PARAM_POS& SisrPos, const int nSeismicForceDirection, seis_jp::eDirShear nDirShear, int nPos, double dShear, T_SISR_PARAM_BASE& rData);
			
	double GetSigmaCk(const T_ELEM_K& kElem, const T_SISR_PARAM_BASE& param, const int& nPos);
	double GetSigmaSy(const T_ELEM_K& kElem);

	virtual double GetCc(int nSeisForcType);
	double CalcCdc(double dadprime);
	double CalcCds(double dadprime);
	double CalcSf(const int& nPos, const int& nSeisDir, const seis_jp::eDirShear& nShearDir, const T_ELEM_K& kElem, const double& dd);

	void MakeSisrUserMap(T_SISR_K SisrK);
	bool GetSisrUserShfo(T_ELEM_K ElemK, int nPos, bool bForcePlus, OUT T_SISR_USER_SHFO_D& rShfoD);

	void MakeSisrLowerElemParamMap(T_SISR_K SisrK);	
	void GetRangeColmGroup(const T_ELEM_K_LIST& aElemK, OUT double& rdBotLevel, OUT double& rdTopLevel);

	bool IsCcUse10(T_ELEM_K ElemK);
	double GetColmHeight(T_ELEM_K ElemK);
	double GetShearSpan(T_ELEM_K ElemK);

	BOOL SetResultRebarCut(const T_SISR_K& kSisr);
	void RunEvalRebarCut(const P_INPUT* pInput, const int& nCutPos, const int& kElem, const CString& strName, const int& nExamShearDir);

private:
	std::vector<T_ELEM_K> m_vSisrCc10;
	std::map<T_ELEM_K, double> m_mSisrColmHeight;
	std::map<T_ELEM_K, double> m_mSisrShearSpan;
	std::map<std::tuple<T_ELEM_K, int, bool>, T_SISR_USER_SHFO_D> m_mSisrUser; /// T_ELEM_K, nPos, bForcePlus
};

