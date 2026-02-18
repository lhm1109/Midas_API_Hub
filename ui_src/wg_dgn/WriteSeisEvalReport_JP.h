#pragma once

#include "Dgn_XLOut.h"
#include "..\wg_db\DB_ST_DT_EVAL_SUMMARY.h"

const double _ToPoint = 0.833333333;

struct T_SIGR_D;
struct T_SIGR_LOWER_D;
struct T_SIRD_EVAL_DISP;
struct SDgnExcelPrintOpt;
struct T_EVAL_SUMM_UPPER;
struct T_SIGR_LOWER_COLM_D;
struct T_SISR_EVAL_ROAD3_POS;
struct T_SIBD_EVAL_ANAL_VALUE;
struct T_SIBD_EVAL_RESULT_RC;
struct T_SIBD_EVAL_CALC_MPhi;
struct T_SIBD_EVAL_CALC_PHI_Y;
struct T_SIBD_EVAL_CALC_PHI_A;
struct T_SIBD_EVAL_PHI_Y_ST;
struct T_SIBD_EVAL_PHI_A_ST;
struct T_SIBD_EVAL_CALC_DAMAGE_H29;

class CDgnProgressDlg;
class CBrdgSeisEvalResult_JP;
class dgn::lib::IExcel;
class CDispWholeModel;
class CDispPierModel;

typedef CArray<T_SIPA_K, T_SIPA_K> T_SIPA_K_Array;

class iWriteSeisEvalReportBase
{
public:
	int* m_pParagraph1;
	int* m_pParagraph2;
	T_SIGR_D::eSubType m_subType;
	T_SIFA_H29_D m_tSifaH29;

public:
	iWriteSeisEvalReportBase() 
	{
		m_pParagraph1 = NULL;
		m_pParagraph2 = NULL;
		m_subType = T_SIGR_D::kUnKnown;
		m_tSifaH29.Initialize();
	}
	virtual ~iWriteSeisEvalReportBase() {}

	virtual void Write()=0;
	virtual void AddStep()=0;
};

class CWriteSeisEvalReportBase : public CDgn_XLOut, public iWriteSeisEvalReportBase
{
protected:
	dgn::lib::IExcel* m_pXL;
	int m_nCode_Forc;
	int m_nCode_Leng;
	double m_dLenToM;
	double m_dLenToMM;
	double m_dLenToCM;
	double m_dStrToMPa;
	UINT m_nDesignCode;
	CDgnProgressDlg* m_pProgress;
	CBrdgSeisEvalResult_JP* m_pResult;
	const SDgnExcelPrintOpt* m_pPrintOpt;
	int m_nSeisDir; // 교축 or 교축직각

public:
	CWriteSeisEvalReportBase(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CWriteSeisEvalReportBase();

	virtual void AddStep() {};
	virtual void Write() {};

	static void SetNG_White(dgn::lib::IExcel* pXL, ULONG iRow1, ULONG iCol1, ULONG iRow2, ULONG iCol2);
	static void SetNG_Red(dgn::lib::IExcel* pXL, ULONG iRow1, ULONG iCol1, ULONG iRow2, ULONG iCol2);

protected:
	void GetTargetElements(const T_SISR_K& kSigr, const T_SIGR_D::eSubType& nGroupType, 
		OUT T_ELEM_K_LIST& aElemKeys, OUT std::map <T_ELEM_K, std::tuple<CString, int>>& mElemInfo,
		const T_SIGR_LOWER_D::eMaterialType& nMatType = T_SIGR_LOWER_D::kUnKnown, const int& nMembType = DB_MEMB_TYPE::D_MBTP_ETC);
	
	void GetTargetElements(const T_SISR_K& kSigr, const T_SIGR_D::eSubType& nGroupType, 
		const T_SIGR_LOWER_D::eMaterialType& nMatType, const int& nMembType, 
		OUT T_ELEM_K_LIST& aElemKeys, OUT std::map <T_ELEM_K, std::tuple<CString, int>>& mElemInfo);

	CString GetTextGroup(const T_SIGR_D::eSubType& nGroupType);
	CString GetTextTitle(const T_SIGR_D::eSubType& nGroupType);

	/**
	* nDir : SUMM_EVAL_K::eExamShearDir
	*/
	CString GetTextExamShearDir(const int& nDir);
	void SetValue_Hyphen(const int& nRow, const int& nCol);
	void WriteSeisDir(CString& RptBlock, const int& iSeisDir, int& iParagraph3, int& stRow);
	void WriteSeisType(CString& RptBlock, const int& iType, int& iParagraph4, int& stRow);
	void WriteSeisPos(CString& RptBlock, const int& iPos, int& iParagraph5, int& stRow);
	void WriteReportDir(const int& nSeisDir, const int& nExamDir, const int& nType, CString& strDir);
	void WriteNanHyphenValue(const int& nRow, const int& nCol, const double& value);

	enum eType
	{
		kFlexure=0,
		kFlexure_Phi,
		kShear,
		kALL
	};
};

class CSeisEvalReportConditions : public CWriteSeisEvalReportBase
{
public:
	CSeisEvalReportConditions(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportConditions();

	virtual void AddStep();
	virtual void Write();

protected:
	void AddSheet();
};

class CSeisEvalReportSummary_A3 : public CWriteSeisEvalReportBase
{
private:
	BOOL m_b00_Summ_1_All;
	BOOL m_b00_Summ_1_Pier;
	BOOL m_00_Summ_3_Lower;
	BOOL m_00_Summ_2_Upper_Title;
	BOOL m_00_Summ_3_LowerSteel;

public:
	CSeisEvalReportSummary_A3(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportSummary_A3();

	virtual void AddStep();
	virtual void Write();

private:
	void AddStep_WriteSummaryOverrallDisp(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);
	void AddStep_WriteSummaryPierDisp(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);
	void AddStep_WriteSummaryUpper(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);
	void AddStep_WriteSummaryLower(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);
	BOOL AddStep_WriteSummaryLowerTitle(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);
	void AddStep_WriteSummaryLowerColumn(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);
	void AddStep_WriteSummaryLowerBeam(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);
	void AddStep_WriteSummaryLowerSteel(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);

	void AddSheet();
	void WriteSummaryOverrallDisp(const int& nSeisDir, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph);
	void WriteSummaryPierDisp(const int& nSeisDir, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph);
	void WriteSummaryUpper(const int& nSeisDir, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph);
	void WriteSummaryLower(const int& nSeisDir, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph);
	void WriteSummaryLowerSteel(const int& nSeisDir, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph);

	int WriteUpperTable(const CString& strGroupName, const BOOL& bYZ_Dir, OUT int& stRow);
	void WriteUpperData(const int& nSeisDir, const T_SIGR_D* pSigr, const T_SIGR_UPPER_D* pSigrUpper, const T_EVAL_SUMM_UPPER* pEval, OUT int& stRow, OUT BOOL& bWriteTable, OUT int& iParagraph);
	BOOL CheckUpperColumn(const T_EVAL_SUMM_UPPER* pEval, const int& nSeisDir);
	BOOL CheckUpperColumn(const T_EVAL_SUMM_UPPER* pEval,
		std::vector<T_SIBD_CVSV_ITEM>& CvsvMin, std::vector<T_SIBD_CVSV_ITEM>& CvsvMax,
		std::vector<T_SIBD_SRSV_ITEM>& SrsvMin, std::vector<T_SIBD_SRSV_ITEM>& SrsvMax,
		std::vector<T_SISR_SUMM_ITEM>& SisrMin, std::vector<T_SISR_SUMM_ITEM>& SisrMax, const int& nSeisDir
	);

	int WriteLowerTable(const int& nMembType, const CString& strGroupName, const BOOL& bYZ_Dir, OUT int& stRow);
	int WriteLowerSteelTable(const int& nMembType, const CString& strGroupName, const BOOL& bYZ_Dir, OUT int& stRow);
	void WriteSummaryLowerColumn(const int& nSeisDir, const T_SIGR_D* pSigr, const T_SIGR_LOWER_D* pSigrLower, const T_EVAL_SUMM_LOWER* pEval, OUT int& stRow, OUT int& iParagraph);
	void WriteSummaryLowerBeam(const int& nSeisDir, const T_SIGR_D* pSigr, const T_SIGR_LOWER_D* pSigrLower, const T_EVAL_SUMM_LOWER* pEval, OUT int& stRow, OUT int& iParagraph);
	BOOL CheckLowerColumn(const T_EVAL_SUMM_LOWER* pEval, const int& nSeisDir);
	BOOL CheckLowerColumn(const T_EVAL_SUMM_LOWER* pEval,
		std::vector<T_SIBD_CVSV_ITEM>& CvsMin, std::vector<T_SIBD_CVSV_ITEM>& CvsvMax,
		std::vector<T_SIBD_SRSV_ITEM>& SrsvMin, std::vector<T_SIBD_SRSV_ITEM>& SrsvMax,
		std::vector<T_SISR_SUMM_ITEM>& SisrMin, std::vector<T_SISR_SUMM_ITEM>& SisrMax,
		std::vector<T_SISR_SUMM_ITEM>& SisrRebarCutMin, std::vector<T_SISR_SUMM_ITEM>& SisrRebarCutMax, const int& nSeisDir
	);
	BOOL CheckLowerBeam(const T_EVAL_SUMM_LOWER* pEval, const int& nSeisDir);
	BOOL CheckLowerBeam(const T_EVAL_SUMM_LOWER* pEval,
		std::vector<T_SIBD_CVSV_ITEM>& CvsvMin, std::vector<T_SIBD_CVSV_ITEM>& CvsvMax,
		std::vector<T_SISR_SUMM_ITEM>& SisrMin, std::vector<T_SISR_SUMM_ITEM>& SisrMax, const int& nSeisDir);

	void WriteSummaryLowerSteelColBeam(const int& nSeisDir, const T_SIGR_D* pSigr, const T_SIGR_LOWER_D* pSigrLower, const CArray<T_SIBD_STEL_ITEM, T_SIBD_STEL_ITEM>& StelSum, const DB_MEMB_TYPE& nMembType, OUT int& stRow, OUT int& iParagraph);
	void WriteSummaryLowerSteelDisp(const int& nSeisDir, const T_SIGR_LOWER_D* pSigrLower, const DB_MEMB_TYPE& nMembType, const CArray<T_SIRD_SUMM_ITEM, T_SIRD_SUMM_ITEM>& SirdSum, const int& stRow);
	BOOL CheckLowerSteel(const CArray<T_SIRD_SUMM_ITEM, T_SIRD_SUMM_ITEM>& SirdSum, const CArray<T_SIBD_STEL_ITEM, T_SIBD_STEL_ITEM>& StelSum, const int& nSeisDir);
	void GetCvsvItem(const CArray<T_SIBD_CVSV_ITEM, T_SIBD_CVSV_ITEM>& items, const int nMembType, T_SIGR_LOWER_D::eMaterialType eMatType, const int& nSeisDir, OUT std::vector<T_SIBD_CVSV_ITEM>& rData);
	void GetSrsvItem(const CArray<T_SIBD_SRSV_ITEM, T_SIBD_SRSV_ITEM>& items, const int nMembType, T_SIGR_LOWER_D::eMaterialType eMatType, const int& nSeisDir, OUT std::vector<T_SIBD_SRSV_ITEM>& rData);
	void GetSisrItem(const CArray<T_SISR_SUMM_ITEM, T_SISR_SUMM_ITEM>& items, const int nMembType, T_SIGR_LOWER_D::eMaterialType eMatType, const int& nSeisDir, OUT std::vector<T_SISR_SUMM_ITEM>& rData);

	void WriteCvsvItems(const BOOL& bYZ_Dir, const std::vector<T_SIBD_CVSV_ITEM>& CvsvMin, 
		const std::vector<T_SIBD_CVSV_ITEM>& CvsvMax, const int& stCol, OUT int& stRow, OUT BOOL& bWriteDir);
	void WriteSrsvItems(const BOOL& bYZ_Dir, const std::vector<T_SIBD_SRSV_ITEM>& SrsvMin,
		const std::vector<T_SIBD_SRSV_ITEM>& SrsvMax, const int& stCol, OUT int& stRow, OUT BOOL& bWriteDir);
	void WriteSisrItems(const BOOL& bYZ_Dir, const std::vector<T_SISR_SUMM_ITEM>& SisrMin, 
		const std::vector<T_SISR_SUMM_ITEM>& SisrMax, const int& stCol, OUT int& stRow, OUT BOOL& bWriteDir);

	void WriteCvsvData(int nRow, int nCol, const T_SIBD_CVSV_ITEM& SibdD);
	void WriteCvsvRebarCutData(int nRow, int nCol, const T_SIBD_CVSV_ITEM& SibdD);
};

class CSeisEvalReportSummary_A4 : public CWriteSeisEvalReportBase
{
public:
	CSeisEvalReportSummary_A4(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportSummary_A4();

	virtual void AddStep();
	virtual void Write();

private:
	void AddStep_WriteSummaryOverrallDisp(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);
	void AddStep_WriteSummaryPierDisp(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);
	void AddStep_WriteSummaryUpper(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);
	void AddStep_WriteSummaryLower(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);
	BOOL AddStep_WriteSummaryLowerTitle(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);
	void AddStep_WriteSummaryLowerColumn(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);
	void AddStep_WriteSummaryLowerBeam(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);
	void AddStep_WriteSummaryLowerSteel(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK);

	void GetCvsvItem(const CArray<T_SIBD_CVSV_ITEM, T_SIBD_CVSV_ITEM>& items, const int nMembType, T_SIGR_LOWER_D::eMaterialType eMatType, OUT std::vector<T_SIBD_CVSV_ITEM>& rData);
	void GetSisrItem(const CArray<T_SISR_SUMM_ITEM, T_SISR_SUMM_ITEM>& items, const int nMembType, T_SIGR_LOWER_D::eMaterialType eMatType, OUT std::vector<T_SISR_SUMM_ITEM>& rData);
	void GetStelItem(const CArray<T_SIBD_STEL_ITEM, T_SIBD_STEL_ITEM>& items, const int nMembType, T_SIGR_LOWER_D::eMaterialType eMatType, OUT std::vector<T_SIBD_STEL_ITEM>& rData);
	
	void AddSheet();
	void WriteSummaryOverrallDisp(dgn::lib::IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph);
	void WriteSummaryPierDisp(dgn::lib::IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph);
	void WriteSummaryUpper(dgn::lib::IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph);
	void WriteDataCurvature(dgn::lib::IExcel* pXL, int nRow, int nCol, const int& nDir, const int& nType, const int& kElem, const T_SIBD_CVSV_ITEM& item);
	void WriteDataMoment(dgn::lib::IExcel* pXL, int nRow, int nCol, const int& nDir, const int& nType, const int& kElem, const T_SIBD_SRSV_ITEM& item);
	void WriteDataShear(dgn::lib::IExcel* pXL, int nRow, int nCol, const int& nDir, const int& nType, const int& kElem, const T_SISR_SUMM_ITEM& item);
	void WriteSummaryLower(dgn::lib::IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph);
	BOOL WriteSummaryLowerTitle(dgn::lib::IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph);
	void WriteSummaryLowerColumn(dgn::lib::IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph);
	void WriteCvsvItems(dgn::lib::IExcel* pXL, const std::vector<T_SIBD_CVSV_ITEM>& CvsvMin, const std::vector<T_SIBD_CVSV_ITEM>& CvsvMax, OUT int& stRow);
	void WriteSisrItems(dgn::lib::IExcel* pXL, const std::vector<T_SISR_SUMM_ITEM>& SisrMin, const std::vector<T_SISR_SUMM_ITEM>& SisrMax, OUT int& stRow);
	void WriteSummaryLowerBeam(dgn::lib::IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph);
	void WriteSummaryLowerSteel(dgn::lib::IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph);
};

class CSeisEvalReportMaxDisp : public CWriteSeisEvalReportBase
{
public:
	CSeisEvalReportMaxDisp(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportMaxDisp();

	virtual void AddStep();
	virtual void Write();
private:
	BOOL GetData_MaxDisp(IN CDBDoc* pDoc, OUT CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT CArray<T_SIMD_K, T_SIMD_K>& aSimdK);
	void AddSheet();
	BOOL IsExistData(std::map<std::tuple<int, int, int, int, int>, std::vector<T_SIMD_K>>& mExistSimd);
	int GetExistPos(const T_SIGR_K& SigrK, const T_SIPA_K& SipaK);
private:
	CDispWholeModel* m_pDispWhole;
	CDispPierModel* m_pDispPierNEXCO;
	CDispPierModel* m_pDispPierJBEC;
};

class CSeisEvalReportResidualDisp : public CWriteSeisEvalReportBase
{
public:
	CSeisEvalReportResidualDisp(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportResidualDisp();

	virtual void AddStep();
	virtual void Write();
private:
	void AddSheet();
	BOOL IsExistData(std::map<std::tuple<int, int, int, int, int>, std::vector<T_SIMD_K>>& mExistSimd);
	int GetExistPos(const T_SIGR_K& SigrK, const T_SIPA_K& SipaK);
private:
	CDispWholeModel* m_pDispWhole;
	CDispPierModel* m_pDispPierNEXCO;
	CDispPierModel* m_pDispPierJBEC;
};

class CSeisEvalReportFlexure : public CWriteSeisEvalReportBase
{
private:
	T_SIGR_D::eSubType m_groupType;

public:
	CSeisEvalReportFlexure(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportFlexure();

	virtual void AddStep();
	virtual void Write();
	virtual void WirteTheory(const int& nMomentType, const int& nEarthquakeType, const T_SIGR_D::eSubType& nGroupType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
protected:
	void AddSheet(const T_SIGR_D::eSubType& nGroupType, OUT CString& rSheetName);
	void WriteCommon(const std::vector<T_SISR_K>(&vExistSibd)[2][2][2], const int& nEarthquakeType, const CString& strSheetName);
	void WriteAxisOne(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited);
	void WriteAxisTwo(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited);
	void WriteData(const T_SIBD_EVAL_ANAL_VALUE* analMoment, const T_SIBD_EVAL_RESULT_RC* result, const T_SIBD_EVAL_CALC_MPhi* calcMax, const T_SIBD_EVAL_CALC_MPhi* calcMin, const int& nRow, OUT BOOL& bWrited);
};

#pragma region CSeisEvalReportShear 전단
class CSeisEvalReportShearBase : public CWriteSeisEvalReportBase
{
public:
	CSeisEvalReportShearBase(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportShearBase();

	virtual void WriteAxisOne(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam, OUT int& stRow, OUT BOOL& bWrited) {};
	virtual void WriteAxisTwo(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam, OUT int& stRow, OUT BOOL& bWrited) {};
	virtual void WriteAxisOne(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited) {};
	virtual void WriteAxisTwo(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited) {};
	virtual void WriteCommon(const std::vector<T_SISR_K>& vec, const BOOL& bMax, const int& iMember, int& stRow, BOOL& bWrited);
	virtual void WriteCommon(const std::vector<T_SISR_K>& vec, const int& iMember, int& stRow, BOOL& bWrited);

protected:
	//void AddSheet(const T_SIGR_D::eSubType& nGroupType, OUT CString& rSheetName);
	BOOL IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SISR_K>>& mExistSisr);
	BOOL IsExistData(std::map<std::tuple<int, int, int>, std::vector<T_SISR_K>>& mExistSisr);
};

class CSeisEvalReportShearSus : public CSeisEvalReportShearBase
{
public:
	CSeisEvalReportShearSus(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportShearSus();

	virtual void AddStep();
	virtual void Write();
	virtual void WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nEarthquakeType, const int iParagraph1, int& iParagraph2, OUT int& stRow);

protected:
	void AddSheet(const T_SIGR_D::eSubType& nGroupType, OUT CString& rSheetName);
	//BOOL IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SISR_K>>& mExistSisr);
	//void WriteCommon(const std::vector<T_SISR_K>& vec, const BOOL& bMax, const int& iMember, int& stRow, BOOL& bWrited);
	void WriteAxisOne(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam, OUT int& stRow, OUT BOOL& bWrited);
	void WriteAxisTwo(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam, OUT int& stRow, OUT BOOL& bWrited);
};

class CSeisEvalReportShearSusResult : public CSeisEvalReportShearBase
{
public:
	CSeisEvalReportShearSusResult(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportShearSusResult();

	virtual void AddStep();
	virtual void Write();

private:
	void AddSheet(const T_SIGR_D::eSubType& nGroupType, OUT CString& rSheetName);
	//BOOL IsExistData(std::map<std::tuple<int, int, int>, std::vector<T_SISR_K>>& mExistSisr);
	void WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int iParagraph1, int& iParagraph2, OUT int& stRow);

	//void WriteCommon(const std::vector<T_SISR_K>& vec, const int& iMember, int& stRow, BOOL& bWrited);
	void WriteAxisOne(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited);
	void WriteAxisTwo(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited);
	void WriteData(const T_SISR_EVAL_ROAD3_POS* pPosD, const int& nRow, OUT BOOL& bWrited);
};

class CSeisEvalReportShearSuc : public CSeisEvalReportShearBase
{
public:
	CSeisEvalReportShearSuc(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportShearSuc();

	virtual void AddStep();
	virtual void Write();
	virtual void WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nEarthquakeType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
	virtual void WriteAxisOne(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam, OUT int& stRow, OUT BOOL& bWrited);
	virtual void WriteAxisTwo(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam, OUT int& stRow, OUT BOOL& bWrited);

protected:
	void AddSheet(const T_SIGR_D::eSubType& nGroupType, OUT CString& rSheetName);
	//BOOL IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SISR_K>>& mExistSisr);
	//void WriteCommon(const std::vector<T_SISR_K>& vec, const BOOL& bMax, const int& iMember, int& stRow, BOOL& bWrited);
};

class CSeisEvalReportShearSucResult : public CSeisEvalReportShearBase
{
public:
	CSeisEvalReportShearSucResult(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportShearSucResult();

	virtual void AddStep();
	virtual void Write();

private:
	void AddSheet(const T_SIGR_D::eSubType& nGroupType, OUT CString& rSheetName);
	//BOOL IsExistData(std::map<std::tuple<int, int, int>, std::vector<T_SISR_K>>& mExistSisr);
	void WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
	//void WriteCommon(const std::vector<T_SISR_K>& vec, const int& iMember, int& stRow, BOOL& bWrited);
	void WriteData(const T_SISR_EVAL_ROAD3_POS* pPosD, const int& nRow, OUT BOOL& bWrited);
	void WriteAxisOne(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited);
	void WriteAxisTwo(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited);
};

class CSeisEvalReportShearPs : public CSeisEvalReportShearBase
{
public:
	CSeisEvalReportShearPs(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportShearPs();

	virtual void AddStep();
	virtual void Write();
	virtual void WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nEarthquakeType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
	virtual void WriteCommon(const std::vector<T_SISR_K>& vec, const BOOL& bMax, const int& iMember, int& stRow, BOOL& bWrited);

protected:
	void AddSheet(const T_SIGR_D::eSubType& nGroupType, OUT CString& rSheetName);
	void WriteAxisOne(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited);
	void WriteAxisTwo(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited);

};

class CSeisEvalReportShearPsResult : public CSeisEvalReportShearBase
{
public:
	CSeisEvalReportShearPsResult(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportShearPsResult();

	virtual void AddStep();
	virtual void Write();
	virtual void WriteCommon(const std::vector<T_SISR_K>& vec, const int& iMember, int& stRow, BOOL& bWrited);

private:
	void AddSheet(const T_SIGR_D::eSubType& nGroupType, OUT CString& rSheetName);
	void WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int iParagraph1, int& iParagraph2, OUT int& stRow);

	void WriteAxisOne(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited);
	void WriteAxisTwo(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited);
};
#pragma endregion CSeisEvalReportShear 전단

class CSeisEvalReportDispPlastRatio : public CWriteSeisEvalReportBase
{
public:
	CSeisEvalReportDispPlastRatio(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportDispPlastRatio();

	virtual void AddStep();
	virtual void Write();

private:
	void AddSheet();
	BOOL GetData(OUT CArray<T_SIDP_K, T_SIDP_K>& aSidpK, OUT CArray<T_SIGR_K, T_SIGR_K>& aSigrK);
	void WirteTheory(const int iParagraph1, int& iParagraph2, OUT int& stRow);

	BOOL IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SIDP_K>>& mExistSidp);
	int GetExistPos(const T_SIGR_K& SigrK, const T_SIPA_K& SipaK);
private:
	CDispPierModel* m_pDispPierNEXCO;
	CDispPierModel* m_pDispPierJBEC;
};

class CSeisEvalReportCurvatureRC : public CWriteSeisEvalReportBase
{

public:
	CSeisEvalReportCurvatureRC(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportCurvatureRC();

	virtual void AddStep();
	virtual void Write();
	virtual void WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nSurveyType, const int iParagraph1, int& iParagraph2, OUT int& stRow);

protected:
	void AddSheet(const T_SIGR_D::eSubType& nGroupType, CString& strSheetName);
	void WriteAxisOne(IN const int& nSurveyCondition, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited);
	void WriteAxisTwo(IN const int& nSurveyCondition, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited);
	void WriteData(const T_SIBD_EVAL_ANAL_VALUE* analDeform, const T_SIBD_EVAL_RESULT_RC* result, const T_SIBD_EVAL_CALC_PHI_Y* calcMax, const T_SIBD_EVAL_CALC_PHI_Y* calcMin, const int& nRow, OUT BOOL& bWrited);
	void WriteData(const T_SIBD_EVAL_ANAL_VALUE* analDeform, const T_SIBD_EVAL_RESULT_RC* result, const T_SIBD_EVAL_CALC_PHI_A* calcMax, const T_SIBD_EVAL_CALC_PHI_A* calcMin, const int& nRow, OUT BOOL& bWrited);
	void GetInputValue(OUT int& nFormula, OUT double& dAlpha);
	BOOL IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SIBD_K>>& mExistSibd);
	void WriteCommon(const std::vector<T_SIBD_K>& vec, const int& iMember, const int& iCond, int& stRow, BOOL& bWrited);
};

class CSeisEvalReportCurvatureST : public CWriteSeisEvalReportBase
{
public:
	CSeisEvalReportCurvatureST(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportCurvatureST();

	virtual void AddStep();
	virtual void Write();

private:
	void AddSheet(const T_SIGR_D::eSubType& nGroupType, CString& strSheetName);
	void WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nSurveyType, const int iParagraph1, int& iParagraph2, OUT int& stRow);

	void WriteAxisOne(IN const int& nSurveyCondition, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited);
	void WriteAxisTwo(IN const int& nSurveyCondition, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited);
	void WriteData(const T_SIBD_EVAL_PHI_Y_ST* eval, const int& nRow, OUT BOOL& bWrited);
	void WriteData(const T_SIBD_EVAL_PHI_A_ST* eval, const int& nRow, OUT BOOL& bWrited);
	BOOL IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SIBD_K>>& mExistSibd);
	void WriteCommon(const std::vector<T_SIBD_K>& vec, const int& iMember, const int& iCond, int& stRow, BOOL& bWrited);
};

class CSeisEvalReportRebarReduction : public CWriteSeisEvalReportBase
{
private:
	BOOL m_bWriteHeadSibd;
	BOOL m_bWriteHeadSisr;

public:
	CSeisEvalReportRebarReduction(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportRebarReduction();

	virtual void AddStep();
	virtual void Write();

private:
	void AddStepWriteRebarCutSibd(const T_SIGR_LOWER_K& kSigrLower);
	void AddStepWriteRebarCutSisr(const T_SIGR_LOWER_K& kSigrLower);

	void AddSheet(OUT CString& rSheetName);
	void WirteTheory(const int iParagraph1, int& iParagraph2, OUT int& stRow);

	BOOL WriteRebarCutSibd(const T_SIGR_LOWER_K& kSigrLower, const int& iParagraph1, const int& iParagraph2, 
		OUT int& stRow);
	int WriteRebarCutSibdOne(const std::tuple<T_SIPA_K, T_SIGR_K, int, int, int>& tParam, const T_SIGR_LOWER_COLM_D* pColumn,
		OUT int& stRow, OUT BOOL& bWriteTable, OUT int& iParagraph3);
	int WriteRebarCutSibdTwo(const std::tuple<T_SIPA_K, T_SIGR_K, int, int, int>& tParam, const T_SIGR_LOWER_COLM_D* pColumn,
		OUT int& stRow, OUT BOOL& bWriteTable, OUT int& iParagraph3);
	void WriteRebarCutSibdHead(const int& iParagraph1, const int& iParagraph2, OUT int& iParagraph3, OUT int& stRow);

	BOOL WriteRebarCutSisr(const T_SIGR_LOWER_K& kSigrLower, const int& iParagraph1, const int& iParagraph2, 
		OUT int& stRow);
	int WriteRebarCutSisrOne(const std::tuple<T_SIPA_K, T_SIGR_K, int, int, int>& tParam, const T_SIGR_LOWER_COLM_D* pColumn,
		OUT int& stRow, OUT BOOL& bWriteTable, OUT int& iParagraph3);
	int WriteRebarCutSisrTwo(const std::tuple<T_SIPA_K, T_SIGR_K, int, int, int>& tParam, const T_SIGR_LOWER_COLM_D* pColumn,
		OUT int& stRow, OUT BOOL& bWriteTable, OUT int& iParagraph3);
	void WriteRebarCutSisrHead(const int& iParagraph1, const int& iParagraph2, OUT int& iParagraph3, OUT int& stRow);
};

class CSeisEvalReportPierBottom : public CWriteSeisEvalReportBase
{
public:
	CSeisEvalReportPierBottom(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportPierBottom();

	virtual void AddStep();
	virtual void Write();

private:
	void AddStepMaxMin();
	void AddStepConcurEach();
	void AddStepConcurByFooting();

	void AddSheet();
	void WirteTheory(const int iParagraph1, OUT int& stRow);
	BOOL WriteMaxMin(dgn::lib::IExcel* pXL, const int iParagraph1, int& iParagraph2, int& stRow);
	BOOL WriteConcurEach(dgn::lib::IExcel* pXL, const int iParagraph1, int& iParagraph2, int& stRow);
	BOOL WriteConcurByFooting(dgn::lib::IExcel* pXL, const int iParagraph1, int& iParagraph2, int& stRow);
};

class CSeisEvalReportBearings : public CWriteSeisEvalReportBase
{
public:
	CSeisEvalReportBearings(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportBearings();

	virtual void AddStep();
	virtual void Write();

private:
	void AddStepMaxMin();
	void AddStepConcurEach();
	void AddStepConcurByLine();

	void AddSheet(CString& strSheetName);
	void WirteTheory(const int iParagraph1, OUT int& stRow);
	BOOL WriteMaxMin(dgn::lib::IExcel* pXL, const int iParagraph1, int& iParagraph2, int& stRow, BOOL& bWrite);
	BOOL WriteConcurEach(dgn::lib::IExcel* pXL, const int iParagraph1, int& iParagraph2, int& stRow, BOOL& bWrite);
	BOOL WriteConcurByLine(dgn::lib::IExcel* pXL, const int iParagraph1, int& iParagraph2, int& stRow, BOOL& bWrite);
	CString GetTextBearingType(const T_SIPA_K& kSipa, const int& nType);
};

class CSeisEvalReportConditions_H29 : public CSeisEvalReportConditions
{
public:
	CSeisEvalReportConditions_H29(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportConditions_H29();

	//virtual void AddStep();
	virtual void Write();
};

class CSeisEvalReportFlexure_H29 : public CSeisEvalReportFlexure
{
public:
	CSeisEvalReportFlexure_H29(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportFlexure_H29();

	void WirteTheory(const int& nMomentType, const int& nEarthquakeType, const T_SIGR_D::eSubType& nGroupType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
};

class CSeisEvalReportShearSus_H29 : public CSeisEvalReportShearSus
{
public:
	CSeisEvalReportShearSus_H29(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportShearSus_H29();

	virtual void Write();
	void WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nEarthquakeType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
};

class CSeisEvalReportShearSuc_H29 : public CSeisEvalReportShearSuc
{
public:
	CSeisEvalReportShearSuc_H29(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportShearSuc_H29();

	virtual void Write();
	void WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nEarthquakeType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
	void WriteAxisOne(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam,
		OUT int& stRow, OUT BOOL& bWrited);
	void WriteAxisTwo(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam,
		OUT int& stRow, OUT BOOL& bWrited);
};

class CSeisEvalReportCurvatureRC_H29 : public CSeisEvalReportCurvatureRC
{
public:
	CSeisEvalReportCurvatureRC_H29(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportCurvatureRC_H29();

	void WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nSurveyType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
};

class CSeisEvalReportShearPs_H29 : public CSeisEvalReportShearPs
{
public:
	CSeisEvalReportShearPs_H29(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportShearPs_H29();

	void WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nEarthquakeType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
};

class CSeisEvalReportRebarReduction_H29 : public CWriteSeisEvalReportBase
{
public:
	CSeisEvalReportRebarReduction_H29(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CSeisEvalReportRebarReduction_H29();

	 virtual void AddStep();
	 virtual void Write();
	void WirteTheory(const int iParagraph1, int& iParagraph2, OUT int& stRow);
	void AddSheet(CString& rSheetName);
	void WriteCommon(const std::vector<T_SISR_K>(&vExistSibd)[2], const int& nRow, const CString& strSheetName);
	void WriteAxisOne(std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, int, CString, CString, int, int>& tParam, int& stRow, BOOL& bWrited);
	void WriteAxisTwo(std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, int, CString, CString, int, int>& tParam, int& stRow, BOOL& bWrited);
	void WriteData(const T_SIBD_EVAL_RESULT_RC& result, const T_SIBD_EVAL_CALC_DAMAGE_H29& DamageMax, const T_SIBD_EVAL_CALC_DAMAGE_H29& DamageMin, const int& nRow, BOOL& bWrited);
};