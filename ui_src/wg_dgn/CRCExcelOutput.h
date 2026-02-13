// CRCExcelOutput.h: interface for the CCRCExcelOutput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRCEXCELOUTPUT_H__FA2409F8_A81E_4583_BC66_58F3A1DA56FB__INCLUDED_)
#define AFX_CRCEXCELOUTPUT_H__FA2409F8_A81E_4583_BC66_58F3A1DA56FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "CRCDataCtrl.h"
#include "DgnKSCE_USD03.h"
#include "PrtKSCE_USD03.h"
#include "PrtKSCE_USD05.h"
#include "PrtKSCE_USD10.h"
#include "..\wg_base\Thread.h"
#include "Dgn_PSCRptManager.h"


#define WM_PSC_XL_REPORT WM_USER+43

struct PSC_BDCW_CR_PROP_R;
struct _FYSG_AASHTO;
struct _FYCM_AASHTO;
struct _BDCY_AASHTO;
struct _CUMS_AASHTO;
struct _CRMS_AASHTO;
struct _BDCW_AASHTO;
struct PSC_BDCW_BASE;

class CCRCDataCtrl;
//class CDgn_PSCRptManager;

class CCRCExcelOutput  : public CThread
{
public:
	CCRCExcelOutput();
	virtual ~CCRCExcelOutput();

protected:
	CCRCDataCtrl m_DataCtrl;
	CDgnKSCE_USD03* m_pDgnKSCE;
	CDgnKSCE_USD05* m_pDgnKSCE05;
	CPrtKSCE_USD03* m_pPrtKSCE;
	CPrtKSCE_USD10* m_pPrtKSCE10;
	//CPrtKSCE_USD05* m_pPrtKSCE05;

	CString m_strChrType;

	CString m_strBasePath;    // _T("*base.dll")이 존재하는 내부 Path.
	CString m_strBaseBook;    // _T("*base.dll")의 이름.
	CString m_strBaseSheet;   // _T("*base.dll")내 Sheet의 이름.

private:
	CDBDoc*		 m_pDoc;
	CMSExcel*	 m_pXL;
	CXLParser* m_pPrs;
	UINT m_iCountSheet;

	int m_iMaxRows;
	int m_iMaxCols;

public:
	BOOL m_bStopExecute;
	void Start();
	BOOL XL_PscReport();
	BOOL XL_PscReport_KSCE10();

	// Report
	void ReportStart();

	BOOL Execute_PSCReport_Rating(ElemPairK ElemKey, int iRptUnit, T_BDCY_AASHTO& BdcyRes, _BDCY_AASHTO& BdcyResD);
	BOOL Set_pExcel(CMSExcel*	 pXL);

protected:

	T_PSCD_D m_PscdD;
	
	BOOL XL_StressAnalysisResult();
	BOOL XL_ConsStageStressDiagram();
	BOOL XL_ServLcomStressDiagram();
	BOOL XL_StressCollectResult();
	
	BOOL XL_UltiLoadDiagram();
	BOOL XL_LcomForceCollect();   //하중별 단면력 집계.
	BOOL XL_FlexStrengthCalc(CString sCopyBookName);   //휨강도 검토.
	BOOL XL_FlexStrengthTable(); //휨강도 Table
	BOOL XL_PosiNegaMomentDiagram(); //
	BOOL XL_ShearStrengthCalc(CString sCopyBookName);  //전단강도 검토.
	BOOL XL_ShearStrengthTable();
	BOOL XL_TorsStrengthCalc(CString sCopyBookName);   //비틀림 강도 검토.
	BOOL XL_ReinforcementCalc(CString sCopyBookName); //인장철근량 계산.

	BOOL XL_FlexStrengthCalc_KSCE10(CString sCopyBookName);
	BOOL XL_FlexStrengthTable_KSCE10();
	BOOL XL_ShearStrengthCalc_KSCE10(CString sCopyBookName);  //전단강도 검토.
	BOOL XL_ShearStrengthTable_KSCE10();

	BOOL XL_TorsStrengthCalc_KSCE10(CString sCopyBookName);   //비틀림 강도 검토.


	CString GetSaveFileName();
	CString GetSaveFileNameEx(BOOL bWithoutExtension = FALSE);
	CString GetSaveFileDirEx();

	CDgn_PSCRptManager myAashtoPscReport;
	BOOL Execute_PSCReport(int iDgnCode, int iRptUnit, CString sSaveFileName);
	BOOL Print_AASHTO_LRFD12_PSC(ElemPairK ElemKey, T_BDCY_AASHTO& bdcyData, _BDCY_AASHTO& BdcyResD, _CUMS_AASHTO& CumsResD, _BDCW_AASHTO& BdcwResD, _CRMS_AASHTO& CrmtResD,
															 _FYSG_AASHTO& FysgResD_Gr, _FYSG_AASHTO& FysgResD_Sl, _FYCM_AASHTO& FycmResD_Gr, _FYCM_AASHTO& FycmResD_Sl);
 //
	BOOL Execute_PSCReport_EC2_2_05(int iDgnCode, CString sSaveFileName);  // for Eurocode2-2:2005, PSC Design!!
	BOOL Execute_PSCReport_IRC112(int iDgnCode, CString sSaveFileName);  // for EIRc112, PSC Box Girder Design!!
	BOOL Execute_PSCReport_LSD12(int iDgnCode, CString sSaveFileName);     // for KSCE-LSD, PSC Design!!
	BOOL Execute_PSCReport_SNiP20503_84(int iDgnCode, CString sSaveFileName);
	BOOL Execute_PSCReport_AS_5100_5(int nDgnCode, int nAmd, CString sSaveFileName, int iRptSelUnit = 1); // iRptSelUnit = SI Unit
	BOOL Execute_PSCReport_IRS(int iDgnCode, CString sSaveFileName);  // for EIRc112, PSC Box Girder Design!!
    BOOL Execute_PSCReport_BS(int iDgnCode, CString sSaveFileName);  // for Eurocode2-2:2005, PSC Design!!
	BOOL Execute_PSCReport_TMH(int iDgnCode, CString sSaveFileName);  // for Eurocode2-2:2005, PSC Design!!

	void ShouldPrintPosxBarIJ(const T_POSX_D & posx, BOOL &bPrintI, BOOL &bPrintJ);
	void ShouldPrintPosxMomIJ(const T_POSX_D & posx, BOOL &bPrintI, BOOL &bPrintJ);
	void ShouldPrintPosxMomIJ_Negative(const T_POSX_D & posx, BOOL &bPrintI, BOOL &bPrintJ);
	void ShouldPrintPosxShrIJ(const T_POSX_D & posx, BOOL &bPrintI, BOOL &bPrintJ);
	void ShouldPrintPosxTorIJ(const T_POSX_D & posx, BOOL &bPrintI, BOOL &bPrintJ);
	void ShouldPrintPosxFatIJ(const T_POSX_D & posx, BOOL &bPrintI, BOOL &bPrintJ);
	void ShouldPrintPosxCraIJ(const T_POSX_D & posx, BOOL &bPrintI, BOOL &bPrintJ);
	void ShouldPrintPosxIJ(int nFlag, BOOL & bPrintI, BOOL & bPrintJ);

	//
	BOOL IsPrintPosiData();
	BOOL IsExistPrintCheckOfGlobalData();
	//
	BOOL CheckPosx_NoneTypeElem(T_POSX_D PosxD);
	BOOL Convert_PSCBDCWtoAASHTOBDCW(const PSC_BDCW_BASE& dgnbdcw,  _BDCW_PSC_LRFD_BASE& BdcwBase);

private:
    bool IsFirstRow(const int& nRow);
};

#endif // !defined(AFX_CRCEXCELOUTPUT_H__FA2409F8_A81E_4583_BC66_58F3A1DA56FB__INCLUDED_)
