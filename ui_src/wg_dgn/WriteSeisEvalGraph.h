#pragma once

//#include "WriteSeisEvalReport_JP.h"
#include "HeaderPre.h"

namespace mit
{
	namespace frx
	{
		class ChartData;
		class CMSExcel;
	}
}

using namespace std;

struct T_SIBD_EVAL_PHI_Y;
struct T_SIBD_EVAL_PHI_A;
struct T_SIBD_EVAL_PHI_Y_ST;
struct T_SIBD_EVAL_PHI_A_ST;
struct T_SISR_EVAL_LOWER;
struct T_SISR_EVAL_ROAD3;
class CDgnProgressDlg;
class CBrdgSeisEvalResult_JP;
class __MY_EXT_CLASS__ CSeisEvalGraphBase/* : public iWriteSeisEvalReportBase*/
{
public:
	CSeisEvalGraphBase(mit::frx::CMSExcel* pXL, CDgnProgressDlg* pProgress, const CString& SheetName, const UINT& nDesignCode);
	virtual ~CSeisEvalGraphBase() {};

	virtual void Write() {};
	virtual void AddStep() {}
	virtual void SetDefaultMutiParam(mit::frx::ChartData& chart) {};

	// Sheet Management
	void SetCurrentSheetNum(const int& sheetNum);
	int GetCurrentSheetNum();
	void SetPosNextGraph(const POINT& graphPos, const POINT& tablePos);
	void GetPosPreGraph(POINT& graphPos, POINT& tablePos);
	BOOL IsHaveGraph() const { return m_bGraph; }
protected:
	void GetTargetElements(const T_SISR_K& kSigr, const T_SIGR_D::eSubType& nGroupType,
		OUT T_ELEM_K_LIST& aElemKeys, OUT std::map <T_ELEM_K, std::tuple<CString, int>>& mElemInfo,
		const T_SIGR_LOWER_D::eMaterialType& nMatType = T_SIGR_LOWER_D::kUnKnown, const int& nMembType = DB_MEMB_TYPE::D_MBTP_ETC);

	void GetTargetElements(const T_SISR_K& kSigr, const T_SIGR_D::eSubType& nGroupType,
		const T_SIGR_LOWER_D::eMaterialType& nMatType, const int& nMembType,
		OUT T_ELEM_K_LIST& aElemKeys, OUT std::map <T_ELEM_K, std::tuple<CString, int>>& mElemInfo);

	// Add Graph
	void CheckSheetSize();
	BOOL WriteGraph(mit::frx::ChartData& data);
	void AddSheet();
	CString GetCellOrgExp(long Row, long Col);

	// Write Report
	void WriteSeisDir(const int& iSeisDir, int& stRow);
	void WriteSeisType(const int& iType, int& stRow);
	void WriteSeisPos(const int& iPos, int& stRow);
	void WriteReportDir(const int& nExamDir, const int& nType, int& stRow);
	void WriteGraphTitle(int& stRow);
	void SetLegendName(const int& nType, const int& nIndex, CString& strLegend);

protected:
	mit::frx::CMSExcel* m_pXL;
	CString m_BaseSheetName;
	int m_CurrentSheetCnt;
	CString m_NewSheetName;
	CDgnProgressDlg* m_pProgress;
	CBrdgSeisEvalResult_JP* m_pResult;
	CDBDoc* m_pDoc;
	UINT m_nDesignCode;
	POINT m_GraphPos;
	POINT m_TablePos;
	POINT m_TableSize;
	CString m_Xtitle;
	CString m_Ytitle;
	CString m_ChartTitle;
	BOOL m_bGraph;
	enum eType
	{
		kFlexure = 0,
		kFlexure_Phi,
		kShear,
		kALL
	};
};

class CGraphControl
{
	public:
	CGraphControl(CString strPath);
	~CGraphControl();

public:
	void CreateGraphControl(CDgnProgressDlg* pProgress);
	void WriteGraph();
	void AddStepGraph();
	BOOL IsGraph();
	BOOL IsExcel();

	private:
		std::vector<CSeisEvalGraphBase*> m_aGraph;
		UINT m_nDesignCode;
		mit::frx::CMSExcel* m_pXL;
		CString m_strPath;
		CDgnProgressDlg* m_pProgress;
};

#pragma region CGraphCurvatureRC
class CGraphCurvatureRC : public CSeisEvalGraphBase
{
public:
	CGraphCurvatureRC(mit::frx::CMSExcel* pXL, CDgnProgressDlg* pProgress, const CString& SheetName, const UINT& nDesignCode);
	virtual ~CGraphCurvatureRC();

	virtual void Write();
	virtual void AddStep();
	virtual void SetDefaultMutiParam(mit::frx::ChartData& chart);

private:
	BOOL IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SIBD_K>>& mExistSibd);
private:
	std::map<std::tuple<int, int, int, CString>, std::vector< T_SIBD_EVAL_PHI_Y>> m_GraphEval_Phi_Y;
	std::map<std::tuple<int, int, int, CString>, std::vector< T_SIBD_EVAL_PHI_A>> m_GraphEval_Phi_A;
};
#pragma endregion CGraphCurvatureRC

#pragma region CGraphCurvatureST
class CGraphCurvatureST : public CSeisEvalGraphBase
{
public:
	CGraphCurvatureST(mit::frx::CMSExcel* pXL, CDgnProgressDlg* pProgress, const CString& SheetName, const UINT& nDesignCode);
	virtual ~CGraphCurvatureST();

	virtual void Write();
	virtual void AddStep();
	virtual void SetDefaultMutiParam(mit::frx::ChartData& chart);

private:
	BOOL IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SIBD_K>>& mExistSibd);
private:
	std::map<std::tuple<int, int, int, CString>, std::vector< T_SIBD_EVAL_PHI_Y_ST>> m_GraphEval_Phi_Y;
	std::map<std::tuple<int, int, int, CString>, std::vector< T_SIBD_EVAL_PHI_A_ST>> m_GraphEval_Phi_A;
};
#pragma endregion CGraphCurvatureST

#pragma region CGraphShear 
class CGraphShearBase : public CSeisEvalGraphBase
{
public:
	CGraphShearBase(mit::frx::CMSExcel* pXL, CDgnProgressDlg* pProgress, const CString& SheetName, const UINT& nDesignCode);
	virtual ~CGraphShearBase();

protected:
	BOOL IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SISR_K>>& mExistSisr);

protected:
	std::map<std::tuple<int, int, int, CString>, std::vector< T_SISR_EVAL_LOWER>> m_GraphEvalLower;
	std::map<std::tuple<int, int, int, CString>, std::vector< T_SISR_EVAL_ROAD3>> m_GraphEvalRoad3;
};

#pragma region CGraphShearPS
class CGraphShearPS : public CGraphShearBase
{
public:
	CGraphShearPS(mit::frx::CMSExcel* pXL, CDgnProgressDlg* pProgress, const CString& SheetName, const UINT& nDesignCode);
	virtual ~CGraphShearPS();

	virtual void Write();
	virtual void AddStep();
	virtual void SetDefaultMutiParam(mit::frx::ChartData& chart);
};
#pragma endregion CGraphShearPS

#pragma endregion CGraphShear 

#include "HeaderPost.h"