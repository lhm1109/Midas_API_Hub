// Rating_RKMC_XLOut.h: interface for the CRating_RKMC_XLOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_Rating_RKMC_XLOut_H__)
#define _Rating_RKMC_XLOut_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../wg_base/MSExcel.h"
//#include "Rating_XLOut.h"
#include "Dgn_XLOut.h"

#include "CRCDataCtrl.h"

#include "..\wg_db\DB_ST_DN_RESULT_PSC.H"
#include "..\dgnengine\src\CVL_DgnCalc_KR\DgnCalc_Rating_Struct.h"


#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRating_RKMC_XLOut : public CDgn_XLOut// CRating_XLOut
{
public:
	CRating_RKMC_XLOut();
	virtual ~CRating_RKMC_XLOut();

	// PSC rating 2005기준
	void PrintReport(CString strPath, int iPrintOpt);

	// PSC rating 2010, 2011 기준
	void Print_RFReport(CString szProgramDir, CString szProjectFileDir, CString szSaveFileName,  int iPrintOpt, CArray<RF_RES_RF_KSCE_PSC_D,  RF_RES_RF_KSCE_PSC_D&>& aRFRptData);

	void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl);
protected:

	CCRCDataCtrl* m_pDataCtrl;

protected:
	//====================================================================================
	//====================================================================================
	// Print Report.
	//====================================================================================
	void Print_RFReport_PSCRating    (CString strPath, int iPrintOpt,  CArray<RF_RES_RF_KSCE_PSC_D,  RF_RES_RF_KSCE_PSC_D&>& aRFRptData);
	void Print_RFReport_PSCRating_LSD(CString strPath, int iPrintOpt,  CArray<RF_RES_RF_KSCE_PSC_D,  RF_RES_RF_KSCE_PSC_D&>& aRFRptData);

	BOOL Print_Detail_PSCRating(dgn::lib::IExcel* pExcel, RF_RES_RF_KSCE_PSC_D& ResData, CString strTitle);
	void SetValue_RF_GROUP_1   (dgn::lib::IExcel* pExcel, CString& RptBlock, RF_RES_RF_KSCE_PSC_D &RptData, CString strTitle, int& stRow);
	void SetValue_RF_GROUP_2   (dgn::lib::IExcel* pExcel, CString& RptBlock, RF_RES_RF_KSCE_PSC_D &RptData, int& stRow);
	void SetValue_RF_GROUP_3   (dgn::lib::IExcel* pExcel, CString& RptBlock, RF_RES_RF_KSCE_PSC_D &RptData, int& stRow);
	void SetValue_RF_GROUP_4   (dgn::lib::IExcel* pExcel, CString& RptBlock, RF_RES_RF_KSCE_PSC_D &RptData, int& stRow);

	BOOL Print_Detail_Force(dgn::lib::IExcel* pExcel, RF_RES_RF_KSCE_PSC_D& ResData, CString strTitle);
	void SetValue_RF_FORCE_COMP(dgn::lib::IExcel* pExcel, CString& RptBlock, RF_RES_RF_KSCE_PSC_D &RptBaseD, RF_RES_KS_FORCE_D &RptData, int& stRow);


private:
	//====================================================================================
	// Print Report.
	//====================================================================================
	void PrintReportExcel(CString strPath, int iPrintOpt, CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<T_RKST_D, T_RKST_D&>& arRKSTData);
	void PrintReportText(CString strPath, CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<T_RKST_D, T_RKST_D&>& arRKSTData);
 
	//====================================================================================
	// Set Page.
	//====================================================================================
	void SetRKMCDetailSheetPage(dgn::lib::IExcel* pExcel);  // 강도설계법
	void SetRKSTDetailSheetPage(dgn::lib::IExcel* pExcel);  // 허용응력설계법

	//====================================================================================
	// Set Rkmc Data. // 강도설계법
	//====================================================================================
	void SetGetRkmcRatingFct(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkmcRatingFctTableHead(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkmcRatingFctTableBody(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkmcKsFct(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkmcKsFctTableHead(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkmcKsFctTableBody(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkmcCapacity(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkmcCapacityTableHead(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkmcCapacityTableBody(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData);

	void PrintGetRkmcRatingFct(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkmcRatingFctTableHead(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkmcRatingFctTableBody(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkmcKsFct(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkmcKsFctTableHead(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkmcKsFctTableBody(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkmcCapacity(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkmcCapacityTableHead(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkmcCapacityTableBody(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);

	//====================================================================================
	// Set Rkst Data. // 허용응력설계법
	//====================================================================================
	void SetGetRkstRatingFct(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkstRatingFctTableHead(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkstRatingFctTableBody(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkstKsFct(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkstKsFctTableHead(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkstKsFctTableBody(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkstCapacity(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkstCapacityTableHead(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData);
	void SetGetRkstCapacityTableBody(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData);

	void PrintGetRkstRatingFct(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkstRatingFctTableHead(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkstRatingFctTableBody(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkstKsFct(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkstKsFctTableHead(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkstKsFctTableBody(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkstCapacity(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkstCapacityTableHead(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);
	void PrintGetRkstCapacityTableBody(dgn::lib::IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData);

	CString SetGetPSCReportBaseFile(int nDgnCode);

	int m_iCode;
	
	// 출력할 수 있는 요소만 List Up
	ArrElemPairKey m_arRkmcKeyList; 
	ArrElemPairKey m_arRkstKeyList;

};

#include "HeaderPost.h"

#endif // !defined(_Rating_RKMC_XLOut_H__)
