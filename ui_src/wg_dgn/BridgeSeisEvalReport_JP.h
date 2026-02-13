#pragma once

#include "HeaderPre.h"

namespace dgn
{
	namespace lib
	{
		class IExcel;
	}
}

struct SDgnExcelPrintOpt;

class CDBDoc;
class CDgnProgressDlg;
class iWriteSeisEvalReportBase;
class CSeisEvalReportBearings;
class CSeisEvalReportPierBottom;
class CSeisEvalReportCurvatureST;
class CSeisEvalReportShearPsResult;
class CSeisEvalReportDispPlastRatio;
class CGraphControl;
class iBridgeSeisEvalReport_JP
{
public:
	iBridgeSeisEvalReport_JP();
	virtual ~iBridgeSeisEvalReport_JP();

protected:
	dgn::lib::IExcel* m_pXL;
	T_UNIT_INDEX m_codeUnit;
	CString m_strBasePath;
	CString m_strBaseBook;

	iWriteSeisEvalReportBase* m_pWriteConditions;
	iWriteSeisEvalReportBase* m_pWriteSummary;
	iWriteSeisEvalReportBase* m_pWriteMaxDisp;
	iWriteSeisEvalReportBase* m_pWriteResidualDisp;
	iWriteSeisEvalReportBase* m_pWriteFlexure;
	iWriteSeisEvalReportBase* m_pWriteShearSus;
	iWriteSeisEvalReportBase* m_pWriteShearSusResult;
	iWriteSeisEvalReportBase* m_pWriteShearSuc;
	iWriteSeisEvalReportBase* m_pWriteShearSucResult;
	CSeisEvalReportDispPlastRatio* m_pWriteDispPlastRatio;
	iWriteSeisEvalReportBase* m_pWriteCurvatureRC;
	CSeisEvalReportCurvatureST* m_pWriteCurvatureST;
	iWriteSeisEvalReportBase* m_pWriteShearPs;
	CSeisEvalReportShearPsResult* m_pWriteShearPsResult;
	iWriteSeisEvalReportBase* m_pWriteRebarReduction;
	CSeisEvalReportPierBottom* m_pWritePierBottom;
	CSeisEvalReportBearings* m_pWriteBearings;

public:
	virtual void CreateInst(CDgnProgressDlg* pProgress, const SDgnExcelPrintOpt* pPrintOpt)=0;
	virtual void AddStep(CDgnProgressDlg* pProgress)=0;
	virtual void Write(CDgnProgressDlg* pProgress)=0;
};

class __MY_EXT_CLASS__ CBridgeSeisEvalReport_JP
{
private:
	dgn::lib::IExcel* m_pXL;
	CDBDoc* m_pDoc;
	CString m_strPath;
	
	//CString m_strBaseSheet;
	CDgnProgressDlg* m_pProgress;
	iBridgeSeisEvalReport_JP* m_pReport;
	CGraphControl* m_pGraphCtrl;

public:
	CBridgeSeisEvalReport_JP(CDgnProgressDlg* pProgress, CString strPath);
	virtual ~CBridgeSeisEvalReport_JP(void);

	void PrintOut(const SDgnExcelPrintOpt* pPrintOpt);
	void PrintOutAll(OUT CArray<CString, CString&>& arOutFileName, OUT CString& strResultMsg);
	
private:
	dgn::lib::IExcel* SetExcel(const int& nDesignCode, OUT CString& strBasePath, OUT CString& strBaseBook);
	void WriteReport(const SDgnExcelPrintOpt* pPrintOpt);
	void WriteGraph();
};

class CBridgeSeisEvalReport_JROAD5_H14 : public iBridgeSeisEvalReport_JP
{
public:
	CBridgeSeisEvalReport_JROAD5_H14(dgn::lib::IExcel* pXL, const CString& strBasePath, const CString& strBaseBook);
	virtual ~CBridgeSeisEvalReport_JROAD5_H14(void);

	virtual void CreateInst(CDgnProgressDlg* pProgress, const SDgnExcelPrintOpt* pPrintOpt);
	virtual void AddStep(CDgnProgressDlg* pProgress);
	virtual void Write(CDgnProgressDlg* pProgress);

private:
	void AddStep_Print_Displaceme();
	void AddStep_Print_UpperStruct();
	void AddStep_Print_LowerStruct();
	void AddStep_Print_UserGroup();

	void Print_Displacement(OUT int& iParagraph1);
	void Print_UpperStruct(OUT int& iParagraph1);
	void Print_LowerStruct(OUT int& iParagraph1);
	void Print_UserGroup(OUT int& iParagraph1);
};

class CBridgeSeisEvalReport_JROAD5_H24 : public CBridgeSeisEvalReport_JROAD5_H14
{
public:
	CBridgeSeisEvalReport_JROAD5_H24(dgn::lib::IExcel* pXL, const CString& strBasePath, const CString& strBaseBook);
	virtual ~CBridgeSeisEvalReport_JROAD5_H24(void);

	virtual void CreateInst(CDgnProgressDlg* pProgress, const SDgnExcelPrintOpt* pPrintOpt);
	virtual void AddStep(CDgnProgressDlg* pProgress);
	virtual void Write(CDgnProgressDlg* pProgress);
};

class CBridgeSeisEvalReport_JROAD5_H29 : public iBridgeSeisEvalReport_JP
{
public:
	CBridgeSeisEvalReport_JROAD5_H29(dgn::lib::IExcel* pXL, const CString& strBasePath, const CString& strBaseBook);
	virtual ~CBridgeSeisEvalReport_JROAD5_H29(void);

	virtual void CreateInst(CDgnProgressDlg* pProgress, const SDgnExcelPrintOpt* pPrintOpt);
	virtual void AddStep(CDgnProgressDlg* pProgress);
	virtual void Write(CDgnProgressDlg* pProgress);

private:
	void AddStep_Print_Displaceme();
	void AddStep_Print_UpperStruct();
	void AddStep_Print_LowerStruct();
	void AddStep_Print_UserGroup();

	void Print_Displacement(OUT int& iParagraph1);
	void Print_UpperStruct(OUT int& iParagraph1);
	void Print_LowerStruct(OUT int& iParagraph1);
	void Print_UserGroup(OUT int& iParagraph1);
};
#include "HeaderPost.h"