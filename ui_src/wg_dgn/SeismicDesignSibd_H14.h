#pragma once

#include "SeismicDesignDisp.h"

struct T_SIBD_D;
struct T_SIPA_D;
struct T_SIBD_EVAL_PHI_A;
struct T_SIBD_EVAL_PHI_Y;
struct T_SIBD_EVAL_MOMENT;
struct T_SIBD_EVAL_PHI_A_ST;
struct T_SIBD_EVAL_PHI_Y_ST;
struct T_SIBD_EVAL_REBAR_CUT;
struct T_SIBD_EVAL_CALC_PHI_A;

class CSeismicDesignSibd_H14 : public iSeismicDesignSibd, public CSeismicDesignDisp
{
protected:
	T_SIBD_K m_kSibd;
	T_SIPA_K m_kSipa;
	T_SIGR_K m_kSigr;
	int m_nMaterialType;

public:
	CSeismicDesignSibd_H14(void);
	virtual ~CSeismicDesignSibd_H14(void);

	virtual void AddProgressStep(CDgnProgressDlg* pProgress);
	virtual BOOL Calculate(CDgnProgressDlg* pProgress);

protected:
	virtual void Invoke() {};

	void SetMemberVariable(const T_SIBD_K& kSibd);

protected:
	double MeterToMicron(const double& val);

	/**
	* 휨 조사의 변위방향 옵션에 대한 공통 함수
	* nDir : enum T_SIGR_UPPER_D, T_SIGR_LOWER_COLM_D, T_SIGR_LOWER_BEAM_D, T_SIGR_USER_D::eDirection
	* return : CDgnToolMPhi_SeismicDesign_JP::eCom
	*/
	int GetComponent(const int& nDir);

	BOOL GetInelastHngeResultDist(const int& nExamShearDir, const int& nMinMax, IN T_ELEM_K_LIST& elemKeys, OUT double& dR_yz);
};

//내력조사
class CSeismicDesignSibd_H14_Strength : public CSeismicDesignSibd_H14
{
public:
	CSeismicDesignSibd_H14_Strength(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd);
	virtual ~CSeismicDesignSibd_H14_Strength();

	void AddProgressStep();
	static void Calculate(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd);

protected:
	virtual void Invoke();

	virtual void GetMomentFromMPhi(T_SIBD_EVAL_MOMENT* pEval);

private:
	void RunEvaluation(const T_ELEM_K& kElem, const int& nExamShearDir);

	void GetMomentFromAnalysis(T_SIBD_EVAL_MOMENT* pEval);
};

//항복조사
class CSeismicDesignSibd_H14_Surrender : public CSeismicDesignSibd_H14
{
public:
	CSeismicDesignSibd_H14_Surrender(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd);
	virtual ~CSeismicDesignSibd_H14_Surrender();

	void AddProgressStep();
	static void Calculate(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd);

protected:
	virtual void Invoke();

private:
	void RunEvaluation(const T_ELEM_K& kElem, const int& nExamShearDir);
		
	BOOL GetDeformFromMPhi(T_SIBD_EVAL_PHI_Y* pEval);

	void GetDeformFromAnalysis(OUT T_SIBD_EVAL_PHI_Y* pEval);

	void GetEvalPhiY_ST(IN T_SIBD_EVAL_PHI_Y* pEval, OUT T_SIBD_EVAL_PHI_Y_ST* pEvalST);
};

//한계상태조사
class CSeismicDesignSibd_H14_LimitState : public CSeismicDesignSibd_H14
{
public:
	CSeismicDesignSibd_H14_LimitState(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd);
	virtual ~CSeismicDesignSibd_H14_LimitState();

	void AddProgressStep();
	static void Calculate(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd);

protected:
	virtual void Invoke();

	virtual BOOL GetPhi_a(T_SIBD_D* pSibd, OUT T_SIBD_EVAL_CALC_PHI_A* pDeform);

private:
	void RunEvaluation(const T_ELEM_K& kElem, const int& nExamShearDir);

	BOOL GetDeformFromMPhi(OUT T_SIBD_EVAL_PHI_A* pEval);

	void GetDeformFromAnalysis(OUT T_SIBD_EVAL_PHI_A* pEval);

	void GetEvalPhiA_ST(IN T_SIBD_EVAL_PHI_A* pEval, OUT T_SIBD_EVAL_PHI_A_ST* pEvalST);
};

//철근단락부 조사
class CSeismicDesignSibd_H14_RebarReducedPart : public CSeismicDesignSibd_H14
{
public:
	CSeismicDesignSibd_H14_RebarReducedPart(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd);
	virtual ~CSeismicDesignSibd_H14_RebarReducedPart();

	void AddProgressStep();
	static void Calculate(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd);

protected:
	virtual void Invoke();

private:
	void RunEvaluation(const T_SIGR_LOWER_COLM_D& colm, const T_ELEM_K& kElem, const int& nPos, const int& nExamShearDir);

	void GetMomentFromMPhi(OUT T_SIBD_EVAL_REBAR_CUT* pEval);

	void GetMomentFromAnalysis(const int& nDir, OUT T_SIBD_EVAL_REBAR_CUT* pEval);
};