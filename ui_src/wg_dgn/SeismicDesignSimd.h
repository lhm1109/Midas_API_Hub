#pragma once
#include "SeismicDesignDisp.h"

#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

struct T_SIMD_EVAL;
struct T_SIMD_EVAL_DISP;

struct P_SIMD
{
private:
	BOOL bValidValue;
public:
	T_SIMD_K kSimd;
	T_SIPA_K kSipa;
	T_SIGR_K kSigr;

	T_SIMD_D tSimd;
	T_SIPA_D tSipa;
	T_SIGR_D tSigr;

	P_SIMD(const T_SIMD_K& simdKey);
	~P_SIMD() {};

	BOOL IsValidValue() { return bValidValue; };
};

struct P_SIMD_Upper
{
private:
	BOOL bValidValue;
public:
	T_THIS_K kThis;
	T_SIMD_SPLD_D* pSpld;

	T_SIGR_UPPER_K kSigrUpper;
	T_SIGR_UPPER_D tSigrUpper;

	T_SIGR_LOWER_K kSigrLower;
	T_SIGR_LOWER_D tSigrLower;

	T_NODE_K kPierTopNode;
	T_SIGR_LOWER_COLM_D tCOLM;
	ColumnData tLowColmData;

	P_SIMD_Upper(IN P_SIMD* pPSimd, const T_THIS_K& kTHIS, IN T_SIMD_SPLD_D* pSPLD);
	~P_SIMD_Upper() {};

	BOOL IsValidValue() { return bValidValue; };
};

struct P_SIMD_Lower
{
private:
	BOOL bValidValue;
public:
	T_THIS_K kThis;
	T_SIMD_SPLD_D* pSpld;

	T_SIGR_LOWER_K kSigrLower;
	T_SIGR_LOWER_D tSigrLower;

	T_NODE_K kPierTopNode;
	T_SIGR_LOWER_COLM_D tCOLM;
	ColumnData tLowColmData;

	P_SIMD_Lower(IN P_SIMD* pPSimd, const T_THIS_K& kTHIS, IN T_SIMD_SPLD_D* pSPLD);
	~P_SIMD_Lower() {};

	BOOL IsValidValue() { return bValidValue; };
};

//최대응답변위 조사 인터페이스
class iSeismicDesignSimd
{
// protected:
// 	CDBDoc* m_pDoc;

public:
	virtual void AddProgressStep(CDgnProgressDlg* pProgress) = 0;
	virtual BOOL Calculate(CDgnProgressDlg* pProgress) = 0;
};

class CSeismicDesignSimd : public CSeismicDesignDisp
{
protected:
	P_SIMD* m_param;

public:
	CSeismicDesignSimd();
	virtual ~CSeismicDesignSimd() {};

	void AddStep(CDgnProgressDlg* pProgress);
	void AddStep_Calculate(CDgnProgressDlg* pProgress, const T_SIMD_K& kSimd);
	void AddStep_RunEvaluation(CDgnProgressDlg* pProgress, const T_SIMD_K& kSimd, const T_THIS_K& kThis);

	virtual BOOL Calculate(const T_SIMD_K& kSimd, T_SIMD_EVAL* pEval);

	virtual BOOL CalcDeltaA(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA) { return FALSE; }

	/**
	* 교량 전체계 조사시 δmax (교각별의 경우 override 해서 재정의 할 것)
	*/
	virtual BOOL CalcDeltaMax(IN T_SIMD_EVAL* pEval, const T_THIS_K& kThis, const int& nTimeIdx, OUT doubleArray& aDeltaMax);

protected:
	BOOL GetDeltaA(const doubleArray& aDeltaA, OUT double& rDeltaA);
	BOOL GetDeltaMax(const doubleArray& aDeltaMax, OUT double& rDeltaMax);

private:
	void SetSizeArray(OUT T_SIMD_EVAL_DISP* pDisp);
	void SetSizeDeltaA(OUT doubleArray& aValues);
	void SetSizeDeltaMax(OUT doubleArray& aValues);

	void RunEvaluation(IN T_SIMD_EVAL* pEval, OUT T_SIMD_EVAL_DISP* pDisp);

	BOOL CalcDeltaMax_Upper(IN T_SIMD_EVAL* pEval, const T_THIS_K& kThis, const int& nTimeIdx, OUT doubleArray& aDeltaMax);
	BOOL CalcDeltaMax_Lower(IN T_SIMD_EVAL* pEval, const T_THIS_K& kThis, const int& nTimeIdx, OUT doubleArray& aDeltaMax);

	void SetDataEval(OUT T_SIMD_EVAL* pEval);
};