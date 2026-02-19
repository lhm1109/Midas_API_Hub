#pragma once

#include "SeismicDesignDisp.h"

struct T_SIDP_D;
struct T_SIPA_D;
struct T_SIDP_EVAL;
struct T_SIMD_EVAL;
struct T_SIMD_EVAL_DISP;
struct T_SIDP_EVAL_BY_CALC;
struct T_SIDP_EVAL_BY_ANAL;
struct T_SIGR_LOWER_COLM_D;
struct ColumnData;

typedef CArray<double, double> doubleArray;
typedef CArray<int, int> intArray;

class CSeismicDesignSidp_H14 : public iSeismicDesignSidp, public CSeismicDesignDisp

{
public:
	CSeismicDesignSidp_H14(void);
	virtual ~CSeismicDesignSidp_H14(void);

	virtual void AddProgressStep(CDgnProgressDlg* pProgress);
	virtual BOOL Calculate(CDgnProgressDlg* pProgress);
};

class CSeismicDesignSidp_H14_Evaluation : public CSeismicDesignSidp_H14
{
private:
	T_SIDP_K m_kSidp;
	T_NODE_K m_kNode;
	T_SIDP_D* m_pSidp;
	T_SIPA_D* m_pSipa;
	T_SIMD_EVAL* m_pEvalSimd;
	ColumnData* m_pLowColmData;

public:
	CSeismicDesignSidp_H14_Evaluation(CDgnProgressDlg* pProgress);
	virtual ~CSeismicDesignSidp_H14_Evaluation();

	static void Execute(IN CDgnProgressDlg* pProgress, const T_SIDP_K& kSidp, const T_NODE_K& kNode, OUT T_SIDP_EVAL* pEval);

private:
	void Invoke(OUT T_SIDP_EVAL* pEval);

	BOOL GetCalcDisplacement(IN T_SIMD_EVAL_DISP* pEvalDisp, OUT T_SIDP_EVAL_BY_CALC* pEval);

	BOOL GetDeltaY(IN T_SIMD_EVAL_DISP* pDispSimd, OUT double& dDeltaY);

	BOOL GetDeltaU(IN T_SIMD_EVAL_DISP* pDisp, OUT double& dDeltaU);
};