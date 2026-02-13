#pragma once

#include "Dgn_XLOut.h"
#include "WriteSeisEvalReport_JP.h"


class CWriteSeisEvalReportBase;
struct SDgnExcelPrintOpt;

class CDgnProgressDlg;
class CBrdgSeisEvalResult_JP;
class dgn::lib::IExcel;
struct T_SIMD_EVAL;
struct T_SIRD_EVAL;
struct T_SIDP_EVAL;

using namespace seis_jp;
using namespace std;

enum eDispType
{
	kMaxDisp = 0,
	kResidualDisp = 1,
	kDispPlasticRat = 2
};

class CDispWholeModel : public CWriteSeisEvalReportBase
{

public:
	CDispWholeModel(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt, const eDispType& DispType);
	virtual ~CDispWholeModel();

	virtual void Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow) {};
	virtual void WirteTheory(const int& nMaterial, const int iParagraph1, int& iParagraph2, OUT int& stRow) {};
	virtual void GetTheoryRangeName(const int& nMaterial, const BOOL& bConsiderAlpha, OUT CString& RptBlock, OUT int& nSubRow) {};

protected:
	T_SIPA_K GetSimd2SipaKey(const T_SIGR_K& SigrK, const T_SIPA_D::eSeismicForceDirection& enDir, const T_SIPA_D::eSeismicForceType& enType);
	T_SIPA_K GetSird2SipaKey(const T_SIGR_K& SigrK, const T_SIPA_D::eSeismicForceDirection& enDir, const T_SIPA_D::eSeismicForceType& enType);
	double GetDeltaMax(const int& nRspCalcType, const doubleArray& aDeltaMax);
	double GetDeltaA(const int& nDesignCode, const int& nAlwCalcType, const doubleArray& aDeltaA);
};

class CDispPierModel : public CWriteSeisEvalReportBase
{
public:
	CDispPierModel(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt, const eDispType& DispType);
	virtual ~CDispPierModel();

	std::map <std::pair< T_SIGR_LOWER_D::eMaterialType, seis_jp::eAlwCalcType>, CString> m_mMaxDispNEXCOTheory;
	std::map <std::pair< T_SIGR_LOWER_D::eMaterialType, seis_jp::eAlwCalcType>, CString> m_mMaxDispNEXCO;
	std::map <std::pair< T_SIGR_LOWER_D::eMaterialType, seis_jp::eAlwCalcType>, CString> m_mMaxDispJEBCTheory;
	std::map <std::pair< T_SIGR_LOWER_D::eMaterialType, seis_jp::eAlwCalcType>, CString> m_mMaxDispJEBC;

	std::map <std::pair< T_SIGR_LOWER_D::eMaterialType, seis_jp::eAlwCalcType>, CString> m_mResidualDispNEXCOTheory;
	std::map <std::pair< T_SIGR_LOWER_D::eMaterialType, seis_jp::eAlwCalcType>, CString> m_mResidualDispNEXCO;
	std::map <std::pair< T_SIGR_LOWER_D::eMaterialType, seis_jp::eAlwCalcType>, CString> m_mResidualDispJEBCTheory;
	std::map <std::pair< T_SIGR_LOWER_D::eMaterialType, seis_jp::eAlwCalcType>, CString> m_mResidualDispJEBC;

	std::map <std::pair< T_SIGR_LOWER_D::eMaterialType, seis_jp::eAlwCalcType>, CString> m_mDispPlasticRatNEXCOTheory;
	std::map <std::pair< T_SIGR_LOWER_D::eMaterialType, seis_jp::eAlwCalcType>, CString> m_mDispPlasticRatNEXCO;
	std::map <std::pair< T_SIGR_LOWER_D::eMaterialType, seis_jp::eAlwCalcType>, CString> m_mDispPlasticRatJEBCTheory;
	std::map <std::pair< T_SIGR_LOWER_D::eMaterialType, seis_jp::eAlwCalcType>, CString> m_mDispPlasticRatJEBC;

	virtual void Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow) {};
	virtual void WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow) {};
	virtual void GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, const BOOL& bConsiderAlpha, OUT CString& RptBlock, OUT int& nSubRow) {};

protected:
	T_SIPA_K GetSimd2SipaKey(const T_SIGR_K& SigrK, const T_SIPA_D::eSeismicForceDirection& enDir, const T_SIPA_D::eSeismicForceType& enType);
	T_SIPA_K GetSird2SipaKey(const T_SIGR_K& SigrK, const T_SIPA_D::eSeismicForceDirection& enDir, const T_SIPA_D::eSeismicForceType& enType);
	T_SIPA_K GetSidp2SipaKey(const T_SIGR_K& SigrK, const T_SIPA_D::eSeismicForceDirection& enDir, const T_SIPA_D::eSeismicForceType& enType);

	double GetDeltaMax(const int& nRspCalcType, const doubleArray& aDeltaMax);
	double GetDeltaA(const int& nDesignCode, const int& nAlwCalcType, const doubleArray& aDeltaA);
};

#pragma region H24

class CMaxDispWhole : public CDispWholeModel
{
public:
	CMaxDispWhole(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CMaxDispWhole();

	virtual void Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow);
	virtual void WirteTheory(const int& nMaterial, const int iParagraph1, int& iParagraph2, OUT int& stRow);
	virtual void GetTheoryRangeName(const int& nMaterial, const BOOL& bConsiderAlpha, OUT CString& RptBlock, OUT int& nSubRow);

protected:
	void WriteData(const T_SIMD_EVAL& EvalD, int& col, int& stRow);
};

class CResidualDispWhole : public CDispWholeModel
{
public:
	CResidualDispWhole(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CResidualDispWhole();

	virtual void Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow);
	virtual void WirteTheory(const int& nMaterial, const int iParagraph1, int& iParagraph2, OUT int& stRow);
protected:
	void WriteData(const T_SIRD_EVAL& EvalD, int& col, int& stRow);
};

class CMaxDispNEXCO_H24 : public CDispPierModel
{
public:
	CMaxDispNEXCO_H24(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CMaxDispNEXCO_H24();

	virtual void Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow);
	virtual void WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
	virtual void GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, const BOOL& bConsiderAlpha, OUT CString& RptBlock, OUT int& nSubRow);

	virtual void WriteData(const T_SIMD_EVAL& EvalD, int& col, int& stRow);
protected:
	void SetRptBlockBody(const int& nMaterial, const int& nAlwCalcType);
	CString m_strRptBlockBody;
};

class CMaxDispJBEC_H24 : public CDispPierModel
{
public:
	CMaxDispJBEC_H24(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CMaxDispJBEC_H24();

	virtual void Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow);
	virtual void WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
	virtual void GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, const BOOL& bConsiderAlpha, OUT CString& RptBlock, OUT int& nSubRow);

	virtual void WriteData(const T_SIMD_EVAL& EvalD, int& col, int& stRow);
protected:
	void SetRptBlockBody(const int& nMaterial, const int& nAlwCalcType);
	CString m_strRptBlockBody;
};

class CResidualDispNEXCO_H24 : public CDispPierModel
{
public:
	CResidualDispNEXCO_H24(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CResidualDispNEXCO_H24();

	virtual void Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow);
	virtual void WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
	virtual void GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, OUT CString& RptBlock);

	virtual void WriteData(const T_SIRD_EVAL& EvalD, int& col, int& stRow);
protected:
	void SetRptBlockBody(const int& nMaterial, const int& nAlwCalcType);
	CString m_strRptBlockBody;
};

class CResidualDispJBEC_H24 : public CDispPierModel
{
public:
	CResidualDispJBEC_H24(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CResidualDispJBEC_H24();

	virtual void Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow);
	virtual void WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
	virtual void GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, OUT CString& RptBlock);

	virtual void WriteData(const T_SIRD_EVAL& EvalD, int& col, int& stRow);
protected:
	void SetRptBlockBody(const int& nMaterial, const int& nAlwCalcType);
	CString m_strRptBlockBody;
};

class CDispPlasticRatNEXCO_H24 : public CDispPierModel
{
public:
	CDispPlasticRatNEXCO_H24(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CDispPlasticRatNEXCO_H24();

	virtual void Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow);
	virtual void WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
	virtual void GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, OUT CString& RptBlock);

	virtual void WriteData(const T_SIDP_EVAL& EvalD, int& col, int& stRow);
protected:
	virtual void SetRptBlockBody(const int& nMaterial, const int& nAlwCalcType);
	CString m_strRptBlockBody;
};

class CDispPlasticRatJBEC_H24 : public CDispPierModel
{
public:
	CDispPlasticRatJBEC_H24(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CDispPlasticRatJBEC_H24();

	virtual void Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow);
	virtual void WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
	virtual void GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, OUT CString& RptBlock);

	virtual void WriteData(const T_SIDP_EVAL& EvalD, int& col, int& stRow);
protected:
	void SetRptBlockBody(const int& nMaterial, const int& nAlwCalcType);
	CString m_strRptBlockBody;
};

#pragma endregion H24

#pragma region H14
class CMaxDispNEXCO_H14 : public CMaxDispNEXCO_H24
{
public:
	CMaxDispNEXCO_H14(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CMaxDispNEXCO_H14();

};

class CMaxDispJBEC_H14 : public CMaxDispJBEC_H24
{
public:
	CMaxDispJBEC_H14(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CMaxDispJBEC_H14();

};

class CResidualDispNEXCO_H14 : public CResidualDispNEXCO_H24
{
public:
	CResidualDispNEXCO_H14(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CResidualDispNEXCO_H14();

};

class CResidualDispJBEC_H14 : public CResidualDispJBEC_H24
{
public:
	CResidualDispJBEC_H14(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CResidualDispJBEC_H14();

};

class CDispPlasticRatNEXCO_H14 : public CDispPlasticRatNEXCO_H24
{
public:
	CDispPlasticRatNEXCO_H14(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CDispPlasticRatNEXCO_H14();

};

class CDispPlasticRatJBEC_H14 : public CDispPlasticRatJBEC_H24
{
public:
	CDispPlasticRatJBEC_H14(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CDispPlasticRatJBEC_H14();

};

#pragma endregion H14

#pragma region H29

class CMaxDispWhole_H29 : public CMaxDispWhole
{
public:
	CMaxDispWhole_H29(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CMaxDispWhole_H29();
	virtual void WirteTheory(const int& nMaterial, const int iParagraph1, int& iParagraph2, OUT int& stRow);
	virtual void GetTheoryRangeName(const int& nMaterial, const BOOL& bConsiderAlpha, OUT CString& RptBlock, OUT int& nSubRow);
};

class CMaxDispNEXCO_H29 : public CMaxDispNEXCO_H24
{
public:
	CMaxDispNEXCO_H29(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CMaxDispNEXCO_H29();
	virtual void WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
	virtual void GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, const BOOL& bConsiderAlpha, OUT CString& RptBlock, OUT int& nSubRow);
};

class CMaxDispJBEC_H29 : public CMaxDispJBEC_H24
{
public:
	CMaxDispJBEC_H29(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CMaxDispJBEC_H29();
	virtual void WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow);
	virtual void GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, const BOOL& bConsiderAlpha, OUT CString& RptBlock, OUT int& nSubRow);
};

class CResidualDispWhole_H29 : public CResidualDispWhole
{
public:
	CResidualDispWhole_H29(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CResidualDispWhole_H29();
};

class CResidualDispNEXCO_H29 : public CResidualDispNEXCO_H24
{
public:
	CResidualDispNEXCO_H29(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CResidualDispNEXCO_H29();
};

class CResidualDispJBEC_H29 : public CResidualDispJBEC_H24
{
public:
	CResidualDispJBEC_H29(dgn::lib::IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt);
	virtual ~CResidualDispJBEC_H29();
};

#pragma endregion H29
