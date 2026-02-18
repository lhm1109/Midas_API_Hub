#pragma once
// Rating_NR2006_STL_XLOut.h: interface for the CRating_NR2006_XLOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_Rating_NR2006_STL_XLOut_H__)
#define _Rating_NR2006_STL_XLOut_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_XLOut.h"

#include "HeaderPre.h"

class CCRCDataCtrl;
class CAnalysisResult;

struct _T_NR2006_STL_XLOUT
{
	UINT ElemK;
	BOOL bIJ[2]; // 0:I, 1:J
	CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mBend;
	CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mShear;
	CMapEx<T_ALCS_K, T_ALCS_K, BOOL, BOOL> mStiff;

	void Initialize()
	{
		ElemK = 0;
		bIJ[0] = FALSE;
		bIJ[1] = FALSE;
		mBend.RemoveAll();
		mShear.RemoveAll();
		mStiff.RemoveAll();
	}
	_T_NR2006_STL_XLOUT& operator = (const _T_NR2006_STL_XLOUT& rData)
	{
		ElemK = rData.ElemK;
		bIJ[0] = rData.bIJ[0];
		bIJ[1] = rData.bIJ[1];
		T_ALCS_K AlcsK;
		BOOL     bChk;
		CopyMap(rData.mBend , mBend , AlcsK, bChk);
		CopyMap(rData.mShear, mShear, AlcsK, bChk);
		CopyMap(rData.mStiff, mStiff, AlcsK, bChk);

		return *this;
	}
};

class __MY_EXT_CLASS__ CRating_NR2006_STL_XLOut : public CDgn_XLOut
{
public:
	CRating_NR2006_STL_XLOut(void);
	virtual ~CRating_NR2006_STL_XLOut(void);

	void PrintReport(CString szProgramDir, CString szProjectFileDir, CString strPath, int iPrintOpt, CArray<_T_NR2006_STL_XLOUT, _T_NR2006_STL_XLOUT&>& arXlout);

	void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl);

protected:


private:
	int     GetDetailCount();
	CString GetPartNameWithNode(T_ELEM_K ElemK, int iPosiNo);
	int     GetConcurrentSize();

	void SetSheetPage(dgn::lib::IExcel* pExcel, CString strNameCurrent);
	void SetSummarySheetPage(dgn::lib::IExcel* pExcel, const T_ELEM_K ElemK);
	void SetAssessmentSheetPage(dgn::lib::IExcel* pExcel, const T_ELEM_K ElemK, const int& iPosi);

	void GenerateAssementSheet(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_NR2006_STL_XLOUT& XloutD);
	
	void GenerateAssBendingTable(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_NR2006_STL_XLOUT& XloutD, int& stRow);
	void GenerateAssShearTable(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_NR2006_STL_XLOUT& XloutD, int& stRow);
	void GenerateAssTransverseStifferTable(dgn::lib::IExcel* pExcel, const int& iPosi, const _T_NR2006_STL_XLOUT& XloutD, int& stRow);

	void GenerateSummarySheet(dgn::lib::IExcel* pExcel, const _T_NR2006_STL_XLOUT& XloutD);

	void GenerateSumBendingTable(dgn::lib::IExcel* pExcel, const _T_NR2006_STL_XLOUT& XloutD, int& stRow);
	void GenerateSumShearTable(dgn::lib::IExcel* pExcel, const _T_NR2006_STL_XLOUT& XloutD, int& stRow);
	void GenerateSumTransverseStifferTable(dgn::lib::IExcel* pExcel, const _T_NR2006_STL_XLOUT& XloutD, int& stRow, bool bBearing);

	BOOL GenerateDetailRpt(dgn::lib::IExcel* pExcel, CString szProgramDir, CString szProjectFileDir, CString strPath, D_XL_PROGRESS_INTERFACE* pProgDlg);

	CString GetMembType(const int& iElem);
	CString GetRANumber(const int& iAssCat);
	CString GetAssessedCategory(const int& iAssCat);
	CString GetAssessedCategoryColor(const int& iAssCat);

	BOOL IsWagon(int nLiveType);

protected:
	CDBDoc*          m_pDoc;
	CCRCDataCtrl*    m_pDataCtrl;
	CAnalysisResult* m_pDgnResult;
};

#include "HeaderPost.h"

#endif // !defined(_Rating_NR2006_STL_XLOut_H__)
