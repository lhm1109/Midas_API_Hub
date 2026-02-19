#pragma once

class CDBDoc;
struct T_SISR_EVAL_COMPRESS_CALC_CAPACITY;
struct T_SISR_EVAL_COMPRESS_ANAL_CAPACITY;

class CSeismicDesignSisr_H14_Road3_Compress
{
private:
	CDBDoc* m_pDoc;
	T_SISR_K m_kSisr;
	T_ELEM_K m_kElem;

public:
	CSeismicDesignSisr_H14_Road3_Compress(const T_SISR_K& kSisr, const T_ELEM_K& kElem);
	virtual ~CSeismicDesignSisr_H14_Road3_Compress();

	static void Calculate(const T_SISR_K& kSisr, const T_ELEM_K& kElem);

private:
	void Invoke();

	void CalcCapacity(T_SISR_EVAL_COMPRESS_CALC_CAPACITY* pCalc);

	void CalcSeisEvaluation(OUT T_SISR_EVAL_COMPRESS_ANAL_CAPACITY* pAnal);

	void CalcSh(OUT T_SISR_EVAL_COMPRESS_ANAL_CAPACITY* pAnal);
};