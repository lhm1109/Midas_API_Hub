#pragma once

#include "SeismicDesignSisrBase.h"

class CDgnProgressDlg;
struct T_SISR_EVAL_DIAG;
struct T_SISR_EVAL_DIAG_POS;

class CSeismicDesignSisr_H14_Road3_Diagonal : public CSeismicDesignSisrBase
{
private:
	CDgnProgressDlg* m_pProgress;

public:
	CSeismicDesignSisr_H14_Road3_Diagonal();
	virtual ~CSeismicDesignSisr_H14_Road3_Diagonal();

	void SetProgressDlg(CDgnProgressDlg* pProgress) { m_pProgress = pProgress; };

	static void AddProgressStep(const T_SISR_K& kSisr, CDgnProgressDlg* pProgress);
	BOOL Calculate(const T_SISR_K& kSisr);
		
private:
	void CalcRatio(const T_SISR_K& SisrK, const T_ELEM_K& kElem, const int nDir, 
		const T_SISR_PARAM_POS& ParamI, const T_SISR_PARAM_POS& ParamJ, OUT T_SISR_EVAL_DIAG& rEval);

	double CalcTauc(double dSigmack);
 	void CalcSh(); //	if (변단면이면) //부재 유효높이 변화에 따른 설계 전단력 보정값을 계산
};