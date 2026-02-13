#pragma once
#include "BrdgSeisEvalKISTEC2015_Report.h"

#include "HeaderPre.h"
namespace dgn 
{
	namespace lib 
	{
		class IExcel;
	}
}
class __MY_EXT_CLASS__ CBrdgSeisEvalKALIS2023_Report : public CBrdgSeisEvalKISTEC2015_Report
{
public:
	CBrdgSeisEvalKALIS2023_Report(void);
	virtual ~CBrdgSeisEvalKALIS2023_Report(void);

protected:
	virtual BOOL GenerateEvalPlainConcSheet(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2]);
	virtual BOOL GenerateEffectiveStiffJudgement(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT pResultD[2][2]);
	virtual void GetMPhiGraphFileNames(CString aResName[3]);

private:
	void GeneratePlainConcFlexCapa(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2]);
	void GeneratePlainConcCombCapa(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2]);
	void GeneratePlainConcShearCapa(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2]);
	void GeneratePlainConcCapa(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2]);
};

#include "HeaderPost.h"