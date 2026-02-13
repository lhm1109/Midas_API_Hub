// Rating_XLOut.h: interface for the CRating_XLOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_Seis_XLOut_H__)
#define _Seis_XLOut_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\..\dgnengine\idesign\DGN_lib\XLOut.h"
#include "..\..\dgnengine\idesign\DGN_lib\XLStruct.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\DBLib.h"

#include "DgnStruct.h"
#include "DgnEngineProgressDlg.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgn_XLOut : public dgn::lib::CXLOut
{
public:
    CDgn_XLOut();
    virtual ~CDgn_XLOut();

protected:
    CDBDoc*	m_pDoc;
    CAttrCtrl* m_pAttrCtrl;

    D_XL_PROGRESS_INTERFACE* m_pProgressInte;
    CString m_strDataPath;

    double m_dFactorAreamm2;

private:
    bool m_bSeisEval; // base 계산서에서 시작하는 column이 첫열이 아닌 경우

protected:
    static CString GetText(LPCTSTR lpszFmt, ...);
    void SetUnitConvertFactor();

    // // Sheet의 Default 특성 Setting.
    void SetDefaultSheetProperty(dgn::lib::IExcel* pExcel);
    void Update_ProgressInft(CString strTitle, CString strMessage, int iPercent); // iPercent : (0∼100)

    void AddBook(dgn::lib::IExcel* pXL, CString strPath, int iPrintOpt);
    CString GetStringPosition(const int &nIndex);
    CString GetStringPositionRating(const int &nIndex);
    CString SetStringEmpty2Hipun(CString &str);
	CString GetLcomType(int iLcomType);
	CString GetStringOKNG(const int& nOK);

    int  CopyRange(dgn::lib::IExcel* pExcel, CString& RptBlock, int stRow);
    int  CopyRangeSubTitle(dgn::lib::IExcel* pExcel, CString& RptBlock, int stRow);
    int  CopyRangeUnImportantTitle(dgn::lib::IExcel* pExcel, CString& RptBlock, int stRow, int nColAdd);
    int  CopyRangeOddTable(dgn::lib::IExcel* pExcel, CString& RptBlock, int stRow);
    int  CopyRangeEvenTable(dgn::lib::IExcel* pExcel, CString& RptBlock, int stRow);

    void OnlyCopyRange(dgn::lib::IExcel* pExcel, CString& RptBlock, int& stRow);

    CString Check_Sign(int nType, double dVal1, double dVal2);
    CString Chk_OK_Sign(int nType, double dVal1, double dVal2);

    BOOL CalcCellID(int stRow, short Row, short Col, short Row_Ref, short Col_Ref, short &rRow_Calc, short &rCol_Calc);
    // strBaseBook  : 복사대상이 되는 BOOK 
    // strSheetName : 복사대상이 되는 sheet
    // strCellname  : 복사대상이 되는 cell
    void PrintCellData(dgn::lib::IExcel* pExcel, CString strCellname, CString strData, short &rRow_Ref, short &rCol_Ref, int& stRow);
    void PrintCellData(dgn::lib::IExcel* pExcel, CString strCellname, double    dData, short &rRow_Ref, short &rCol_Ref, int& stRow);
    void PrintCellDataMark(dgn::lib::IExcel* pExcel, CString strCellname, CString strData, short &rRow_Ref, short &rCol_Ref, int& stRow);
    void PrintCellTrashData(dgn::lib::IExcel* pExcel, CString strCellname, int nColAdd, double    dData, short &rRow_Ref, short &rCol_Ref, int& stRow);

    void PrintCellOddEvenData(dgn::lib::IExcel* pExcel, CString strCellname, int iOddEvenCol, double dData, short &rRow_Ref, short &rCol_Ref, int& stRow);
    void PrintCellOddEvenData(dgn::lib::IExcel* pExcel, CString strCellname, int iOddEvenCol, CString strData, short &rRow_Ref, short &rCol_Ref, int& stRow);


    void PrintCellData(dgn::lib::IExcel* pExcel, CString strBaseBook, CString strSheetName, CString strCellname, CString strData, short &rRow_Ref, short &rCol_Ref, int& stRow);
    void PrintCellData(dgn::lib::IExcel* pExcel, CString strBaseBook, CString strSheetName, CString strCellname, double    dData, short &rRow_Ref, short &rCol_Ref, int& stRow);
    //
    void InsertCellForRoof(dgn::lib::IExcel* pExcel, CString strRefRowCellname, short &rRow_Ref, short &rCol_Ref, int& stRow);
    BOOL CopyCellForRoof(dgn::lib::IExcel* pExcel, CString strRefRowCellname, short &rRow_Ref, short &rCol_Ref, int& stRow);

	CString FormatOutput(int iValue);
	CString FormatOutput(double dValue, int iDecimalPlaces);

	CString GetPartNameWithNode(T_ELEM_K ElemK, int iPosiNo);

	void SetSeisEval(bool bSeisEval) { m_bSeisEval = bSeisEval; }
	bool IsSeisEval() const { return m_bSeisEval; }
};

#include "HeaderPost.h"

#endif // !defined(_Rating_XLOut_H__)