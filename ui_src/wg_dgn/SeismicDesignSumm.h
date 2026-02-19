#pragma once

#include "SeismicDesignBase.h"
#include "..\wg_db\DB_ST_DT_EVAL_SUMMARY.h"

struct T_SIMD_EVAL;
struct T_SIRD_EVAL;

class CDBDoc;

#define SUMM_EVAL_KEY unsigned int
union SUMM_EVAL_K
{
	SUMM_EVAL_KEY keyMap;
	struct
	{
		int nMinMax : 2;				//SEIS_EVAL_K::eDirection
		int nExamShearDir : 2;			//조사 방향 : seis_jp::eDirShear;
		int nSeismicForceDirection : 2; //지진력 방향 : enum T_SIPA_D::eSeismicForceDirection
		int nMemberType : 26;			//부재 타입 : enum DB_MEMB_TYPE
	}Key;

	SUMM_EVAL_K()
	{
		keyMap = 0;
	}
	SUMM_EVAL_K(const int& nMinMax, const int& nExamShearDir, const int& nSeismicForceDirection, const int& nMemberType)
	{
		Key.nMinMax = nMinMax;
		Key.nExamShearDir = nExamShearDir;
		Key.nSeismicForceDirection = nSeismicForceDirection;
		Key.nMemberType = nMemberType;
	}
	void SetKey(const int& nMinMax, const int& nExamShearDir, const int& nSeismicForceDirection, const int& nMemberType)
	{
		Key.nMinMax = nMinMax;
		Key.nExamShearDir = nExamShearDir;
		Key.nSeismicForceDirection = nSeismicForceDirection;
		Key.nMemberType = nMemberType;
	}

	enum eMinMax
	{
		kMin,
		kMax,
	};
};

class CSeismicDesignSumm : public iSeismicDesignSumm
{
private:
	CDBDoc* m_pDoc;

public:
	CSeismicDesignSumm(void);
	virtual ~CSeismicDesignSumm(void);

	virtual void AddProgressStep(CDgnProgressDlg* pProgress);
	virtual BOOL Calculate(CDgnProgressDlg* pProgress);

private:
	void SummaryOverrallDisp();
	void GetSimdKeyMap(OUT std::vector<T_SIGR_K>& sigrKeys, OUT std::map<T_SIGR_K, std::vector<T_SIMD_K>>& mSimdByGroup);
	void GetSirdKeyMap(OUT std::vector<T_SIGR_K>& sigrKeys, OUT std::map<T_SIGR_K, std::vector<T_SIRD_K>>& mSirdByGroup);
	void GetSummarySimd(const std::vector<T_SIMD_K>& simdKeys, OUT T_EVAL_SUMM_OVERALL& rSummary);
	void GetSimdAvergeDisp(IN T_SIMD_EVAL* pEval, OUT CArray<T_SIMD_SUMM_ITEM, T_SIMD_SUMM_ITEM>& items);
	void GetSummarySird(const std::vector<T_SIRD_K>& sirdKeys, OUT T_EVAL_SUMM_OVERALL& rSummary);
	void GetSirdAvergeDisp(IN T_SIRD_EVAL* pEval, OUT CArray<T_SIRD_SUMM_ITEM, T_SIRD_SUMM_ITEM>& items);

	void SummaryPierDisp();
	void GetSimdLowerKeyMap(OUT std::vector<T_SIGR_K>& sigrKeys, OUT std::map<T_SIGR_K, std::vector<T_SIMD_K>>& mSimdByGroup);
	void GetSirdLowerKeyMap(OUT std::vector<T_SIGR_K>& sigrKeys, OUT std::map<T_SIGR_K, std::vector<T_SIRD_K>>& mSirdByGroup);
	void GetSummarySimdLower(const std::vector<T_SIMD_K>& simdKeys, OUT T_EVAL_SUMM_PIER& rSummary);
	void GetSummarySirdLower(const std::vector<T_SIRD_K>& sirdKeys, OUT T_EVAL_SUMM_PIER& rSummary);

	void SummaryUpperStructure();
	void GetSibdkeysBySigr(/*T_SIBD_D::eSibdType*/const int& nSibdType, OUT std::map<T_SIGR_K, std::vector<T_SIBD_K>>& mSibdKeys);
	void GetSisrUpperKeyMap(std::map<T_SIGR_K, std::vector<T_SISR_K>>& mSisr_UPPER);
	void GetSisrLowerKeyMap(std::map<T_SIGR_K, std::vector<T_SISR_K>>& mSisr_LOWER);
	
	void GetSummarySibdCvsv(const std::vector<T_SIBD_K>& sibdKeys, OUT T_SIBD_CVSV_DATA& rSummary);
	void GetCvsvItemFromSurrender(const T_SIBD_K& kSibd, OUT std::map<SUMM_EVAL_KEY, T_SIBD_CVSV_ITEM>& mCvsvItem);
	void GetCvsvItemFromLimitState(const T_SIBD_K& kSibd, OUT std::map<SUMM_EVAL_KEY, T_SIBD_CVSV_ITEM>& mCvsvItem);

	void GetSummarySibdSrsv(const std::vector<T_SIBD_K>& sibdKeys, OUT T_SIBD_SRSV_DATA& rSummary);
	void GetCvsvItemFromStrength(const T_SIBD_K& kSibd, OUT std::map<SUMM_EVAL_KEY, T_SIBD_SRSV_ITEM>& mCvsvItem);

	void GetSummarySisrUpper(const std::vector<T_SISR_K>& sisrKeys, OUT T_SISR_SUMM_DATA& rSummary);
	void GetSisrItemFromUpper(const T_SISR_K& kSisr, OUT std::map<SUMM_EVAL_KEY, T_SISR_SUMM_ITEM>& mSisrItem);

	void SummaryLowerStructure();
	void GetSummarySibdRebarCutSrsv(const std::vector<T_SIBD_K>& sibdKeys, OUT T_SIBD_SRSV_DATA& rSummary);
	void GetSrsvItemFromRebarCut(T_SIBD_K kSibd, OUT std::map<SUMM_EVAL_KEY, T_SIBD_SRSV_ITEM>& mSrsvItem);
	
	void GetSummarySisrRebarCut(const std::vector<T_SISR_K>& sisrKeys, OUT T_SISR_SUMM_DATA& rSummary);
	void GetSisrItemFromRebarCut(const T_SISR_K& kSisr, OUT std::map<SUMM_EVAL_KEY, T_SISR_SUMM_ITEM>& mSisrItem);

	void GetSummarySisrLower(const std::vector<T_SISR_K>& sisrKeys, OUT T_SISR_SUMM_DATA& rSummary);
	void GetSisrItemFromLower(const T_SISR_K& kSisr, OUT std::map<SUMM_EVAL_KEY, T_SISR_SUMM_ITEM>& mSisrItem);
	void GetSummarySibdSteel(const std::vector<T_SIBD_K>& sibdKeys, OUT CArray<T_SIBD_STEL_ITEM, T_SIBD_STEL_ITEM>& rSummary);
	void GetStelItemFromSurrender(const T_SIBD_K& kSibd, OUT std::map<int, std::map<int, T_SIBD_STEL_ITEM>>& mStelItem);
	void GetStelItemFromLimitState(const T_SIBD_K& kSibd, OUT std::map<int, std::map<int, T_SIBD_STEL_ITEM>>& mStelItem);
};