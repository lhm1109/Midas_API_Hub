#pragma once
#include "SeismicDesignDisp.h"

#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

struct T_SIRD_EVAL;
struct T_SIRD_RSDP_D;
struct T_SIRD_EVAL_DISP;

struct P_SIRD
{
private:
	BOOL bValidValue;
public:
	T_SIRD_K kSird;
	T_SIPA_K kSipa;
	T_SIGR_K kSigr;

	T_SIRD_D tSird;
	T_SIPA_D tSipa;
	T_SIGR_D tSigr;

	P_SIRD(const T_SIRD_K& sirdKey);
	~P_SIRD() {};

	BOOL IsValidValue() { return bValidValue; };
};

struct P_SIRD_Lower
{
private:
	BOOL bValidValue;
public:
	T_SIRD_RSDP_D* pRsdp;

	T_SIGR_LOWER_K kSigrLower;
	T_SIGR_LOWER_D tSigrLower;
	 
	T_NODE_K kPierTopNode;
	T_SIGR_LOWER_COLM_D tCOLM;
	ColumnData tLowColmData;

	P_SIRD_Lower(IN P_SIRD* pPSird, IN T_SIRD_RSDP_D* pRSDP);
	~P_SIRD_Lower() {};

	BOOL IsValidValue() { return bValidValue; };
};

//잔류변위 조사 인터페이스
class iSeismicDesignSird
{
public:
	virtual void AddProgressStep(CDgnProgressDlg* pProgress) = 0;
	virtual BOOL Calculate(CDgnProgressDlg* pProgress) = 0;
};

class CSeismicDesignSird : public CSeismicDesignDisp
{
protected:
	P_SIRD* m_param;

public:
	CSeismicDesignSird();
	virtual ~CSeismicDesignSird() {};

	void SetParamSird(P_SIRD* pParam) { m_param = pParam; };

	void AddStep(CDgnProgressDlg* pProgress);
	void AddStep_Calculate(CDgnProgressDlg* pProgress, const T_SIRD_K& kSird);
	void AddStep_RunEvaluation(CDgnProgressDlg* pProgress, const T_SIRD_K& kSird, const T_THIS_K& kThis);

	virtual BOOL Calculate(const T_SIRD_K& kSird, T_SIRD_EVAL* pEval);

protected:
	virtual BOOL CalcDeltaMax(IN T_SIRD_EVAL* pEval, const T_THIS_K& kThis, const int nTimeIdx, OUT double& rDeltaMax) { return FALSE; };

	virtual BOOL CalcDelta_y(const T_THIS_K& kThis, T_SIRD_RSDP_D* pRsdp, OUT double& rDeltaY) { return FALSE; };

private:
	void RunEvaluation(IN T_SIRD_EVAL* pEval, OUT T_SIRD_EVAL_DISP* pDisp);

	BOOL CalcDelta_ra(const T_SIRD_RSDP_D* pRsdp, OUT double& dDelta_r);

	BOOL Get_h(const T_SIGR_LOWER_K& kSigrLower, const CString& strNameCOLM, OUT double& d_h);

	BOOL CalcDelta_r(IN T_SIRD_EVAL* pEval, const int nTimeIdx, const T_SIRD_EVAL_DISP* aParam, OUT T_SIRD_EVAL_DISP* pDisp);

	void SetDataEval(OUT T_SIRD_EVAL* pEval);

	BOOL Get_Cr(const T_SIGR_LOWER_K& kSigrLower, OUT double& d_Cr);

	BOOL Get_r(const T_SIGR_LOWER_K& kSigrLower, OUT double& d_r);
};