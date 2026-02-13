#pragma once

#include "SeismicDesignSibd_H14.h"

struct T_SIBD_D;
struct T_SIBD_EVAL_PHI_A;
struct T_SIBD_EVAL_PHI_Y;
struct T_SIBD_EVAL_MOMENT;
struct T_SIBD_EVAL_PHI_A_ST;
struct T_SIBD_EVAL_PHI_Y_ST;
struct T_SIBD_EVAL_REBAR_CUT;
struct T_SIBD_EVAL_CALC_PHI_A;

class CSeismicDesignSibd_H29 : public iSeismicDesignSibd, public CSeismicDesignDisp
{
public:
	CSeismicDesignSibd_H29(void);
	virtual ~CSeismicDesignSibd_H29(void);

	virtual void AddProgressStep(CDgnProgressDlg* pProgress);
	virtual BOOL Calculate(CDgnProgressDlg* pProgress);
};

//내력조사
class CSeismicDesignSibd_H29_Strength : public CSeismicDesignSibd_H14_Strength
{
public:
	CSeismicDesignSibd_H29_Strength(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd);
	virtual ~CSeismicDesignSibd_H29_Strength();

	void AddProgressStep();
	static void Calculate(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd);

protected:
	virtual void Invoke();

	virtual void GetMomentFromMPhi(T_SIBD_EVAL_MOMENT* pEval);
};

//한계상태조사
class CSeismicDesignSibd_H29_LimitState : public CSeismicDesignSibd_H14_LimitState
{
public:
	CSeismicDesignSibd_H29_LimitState(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd);
	virtual ~CSeismicDesignSibd_H29_LimitState();

	void AddProgressStep();
	static void Calculate(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd);

protected:
	virtual void Invoke();

	virtual BOOL GetPhi_a(T_SIBD_D* pSibd, OUT T_SIBD_EVAL_CALC_PHI_A* pDeform);
};

//철근단락부 조사
class CSeismicDesignSibd_H29_RebarReducedPart : public CSeismicDesignSibd_H14
{
public:
	CSeismicDesignSibd_H29_RebarReducedPart(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd);
	virtual ~CSeismicDesignSibd_H29_RebarReducedPart();

	void AddProgressStep();
	static void Calculate(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd);

protected:
	virtual void Invoke();

private:
	void RunEvaluation(const T_SIGR_LOWER_COLM_D& colm, const T_ELEM_K& kElem, const int& nPos, const int& nExamShearDir);

	BOOL ReviewDamaged(const T_SIGR_LOWER_COLM_D& colm, OUT T_SIBD_EVAL_REBAR_CUT* pEval);
};