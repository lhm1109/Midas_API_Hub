#pragma once

#include "SeismicDesignBase.h"
#include "..\wg_db\DB_ST_DT_SIET_EVAL.h"

struct T_SIET_D;
struct T_SIPA_D;
struct T_SIGR_LOWER_D;
struct T_SIGR_LOWER_BEAR_D;

class CSeismicDesignSiet : public iSeismicDesignSiet, public CSeismicDesignBase
{
public:
	CSeismicDesignSiet(void);
	virtual ~CSeismicDesignSiet(void);

	virtual void AddProgressStep(CDgnProgressDlg* pProgress);
	virtual BOOL Calculate(CDgnProgressDlg* pProgress);

private:
	void Invoke(CDgnProgressDlg* pProgress, const T_SIET_K& kSiet);
};

struct ConcBear
{
	T_THIS_K kThis;
	double dHmax;
	int nTimeIdx;
	double dTimeCount;
	int nDirShear;

	ConcBear() { Initialize(); }
	ConcBear(const ConcBear& src) { *this = src; }
	ConcBear& operator= (const ConcBear& src)
	{
		if (this != &src) {
			kThis = src.kThis;
			dHmax = src.dHmax;
			nTimeIdx = src.nTimeIdx;
			dTimeCount = src.dTimeCount;
			nDirShear = src.nDirShear;
		}
		return *this;
	}

	void Initialize()
	{
		kThis = 0;
		dHmax = 0;
		nTimeIdx = 0;
		dTimeCount = 0;
		nDirShear = 0;
	}
};

class CSeismicDesignSiet_Bearing : public CSeismicDesignSiet
{
private:
	T_SIET_K m_kSiet;
	T_SIET_D* m_pSiet;
	T_SIPA_D* m_pSipa;
	T_SIGR_LOWER_D* m_pSigrLower;

public:
	CSeismicDesignSiet_Bearing(CDgnProgressDlg* pProgress);
	virtual ~CSeismicDesignSiet_Bearing(void);

	void AddProgressStep(const T_SIET_K& kSiet);
	BOOL Calculate(const T_SIET_K& kSiet);

private:
	void GetBearingForceMax();

	BOOL GetMinMaxForce(const T_THIS_K& kThis, const T_SIET_EVAL_BEAR_MINMAX* pEval, const T_SIET_EVAL_BEAR_GROUP* pGroup, OUT T_SIET_EVAL_NLNK_FORCE* pMax, OUT T_SIET_EVAL_NLNK_FORCE* pMin);

	BOOL GetDispMinMax(const int& nDir, const T_NLNK_K& kNlnk, OUT double& dUmax, OUT double& dUmin);
	
	CString GetBearingType(const T_SIPA_D* pSipa, int nType);

	void GetConcurvityEachBr();

	BOOL GetConcForceEach(const T_THIS_K& kThis, const T_SIET_EVAL_BEAR_GROUP_CONCU* pGroup, OUT T_SIET_EVAL_NLNK_FORCE* pMax, OUT T_SIET_EVAL_NLNK_FORCE* pMin);

	BOOL GetDispConcForceEach(const int& nDir, const T_NLNK_K& kNlnk, const int& nMaxTimeIdx, const int& nMinTimeIdx, OUT double& dUmax, OUT double& dUmin);

	void GetConcurvityLineBr();

	void GetBearingByLine(OUT std::map<int, std::vector<T_SIGR_LOWER_BEAR_D>>& mBearLine);

	void GetConcurvityValue(const std::vector<T_SIGR_LOWER_BEAR_D>& bearList, OUT T_SIET_EVAL_LINE_MINMAX* pLine);

	BOOL GetMinMaxForceAxis(const T_THIS_K& kThis, const T_SIGR_LOWER_BEAR_D& bear, OUT ConcBear& min, OUT ConcBear& max);

	BOOL GetConcValueWithTime(const T_SIGR_LOWER_BEAR_D& bear, const ConcBear& bearTime, OUT T_SIET_EVAL_NLNK_FORCE& rForce);
};

struct ConcNode
{
public:
	enum eForceType
	{
		kAxial,
		kShearY,
		kShearZ,
		kTorsion,
		kMomentY,
		kMomentZ,
		kForceNum,
	};

	T_ELEM_K kElem;
	T_NODE_K kNode;
	int ePosNodeLow;				//최하 요소의 아래 절점 : enum T_SIET_EVAL_MEMB_GROUP::ePosNodeLow
	CString strName;				//그룹이름
	double dTimeCountMin[kForceNum];
	double dTimeCountMax[kForceNum];

	ConcNode() { Initialize(); }
	ConcNode(const ConcNode& src) { *this = src; }
	ConcNode& operator= (const ConcNode& src)
	{
		if (this != &src) {
			kElem = src.kElem;
			kNode = src.kNode;
			ePosNodeLow = src.ePosNodeLow;
			strName = src.strName;
			for (int i = 0; i < kForceNum; ++i) {
				dTimeCountMin[i] = src.dTimeCountMin[i];
			}
			for (int i = 0; i < kForceNum; ++i) {
				dTimeCountMax[i] = src.dTimeCountMax[i];
			}
		}
		return *this;
	}

	void Initialize()
	{
		kElem = 0;
		kNode = 0;
		ePosNodeLow = 0;
		strName = _T("");
		for (int i = 0; i < kForceNum; ++i) {
			dTimeCountMin[i] = 0;
		}
		for (int i = 0; i < kForceNum; ++i) {
			dTimeCountMax[i] = 0;
		}
	}
};

class CSeismicDesignSiet_PierBase : public CSeismicDesignSiet
{
private:
	T_SIET_K m_kSiet;
	T_SIET_D* m_pSiet;
	T_SIPA_D* m_pSipa;
	T_SIGR_LOWER_D* m_pSigrLower;

public:
	CSeismicDesignSiet_PierBase(CDgnProgressDlg* pProgress);
	virtual ~CSeismicDesignSiet_PierBase(void);

	void AddProgressStep(const T_SIET_K& kSiet);
	BOOL Calculate(const T_SIET_K& kSiet);

private:
	void AddProgressStep_FootingLow(T_SIPA_D* pSipa, T_SIGR_LOWER_D* pSigrLower);
	void AddProgressStep_GetConcurvityForce(const T_THIS_K& kThis, ConcNode& master, const CArray<ConcNode, ConcNode>& subNodes);
	void AddProgressStep_GetTimeKrnlForceResult(const T_THIS_K& kThis, 
												const int& nTimeCount, 
												const ConcNode& target, 
												const int& nMinTime, 
												const int& nMaxTime);

	BOOL GetMemberForceMax();

	void GetColumnBotForce(OUT T_SIET_EVAL_MEMB_MINMAX* pEval);

	void GetFootingBotForce(OUT T_SIET_EVAL_MEMB_MINMAX* pEval);

	void GetBotNodeElemKey(const T_ELEM_K_LIST& elemKeys, OUT T_ELEM_K& kElem, OUT int& ePosNodeLow);

	BOOL GetMinMaxForce(const T_THIS_K& kThis, const T_SIET_EVAL_MEMB_GROUP* pGroup, OUT T_SIET_EVAL_BEAM_FORCE* pMax, OUT T_SIET_EVAL_BEAM_FORCE* pMin);


	BOOL GetConcurvityEachMb();

	void GetColumnBotEachMb(OUT T_SIET_EVAL_MEMB_EACH* pEval);

	void GetFootingBotEachMb(OUT T_SIET_EVAL_MEMB_EACH* pEval);

	BOOL GetConcurvityEach(const T_THIS_K& kThis, const T_SIET_EVAL_MEMB_CONCU* pGroup, T_SIET_EVAL_BEAM_FORCE* pMax, T_SIET_EVAL_BEAM_FORCE* pMin);

	BOOL GetConcurvityFootingLow();

	void GetConcMasterNode(OUT CArray<ConcNode, ConcNode>& masters);

	void GetTimeCount(const T_THIS_K& kThis, OUT ConcNode& masterNode);

	void GetConcSubNode(OUT CArray<ConcNode, ConcNode>& subNodes);

	BOOL GetConcurvityForce(const T_THIS_K& kThis, ConcNode& master, const CArray<ConcNode, ConcNode>& subNodes, OUT T_SIET_EVAL_FOOT_CONCU* pForce);

	BOOL GetTimeKrnlForceResult(const T_THIS_K& kThis, const int& nTimeCount, const ConcNode& target, const int& nMinTime, const int& nMaxTime, OUT T_SIET_EVAL_FOOT_GROUP* pMin, OUT T_SIET_EVAL_FOOT_GROUP* pMax);
	BOOL GetNlbmHistoryForce(const ConcNode& target, const int& nMinTime, const int& nMaxTime, OUT T_SIET_EVAL_FOOT_GROUP* pMin, OUT T_SIET_EVAL_FOOT_GROUP* pMax);
};