// RptRatingPSC_AASHTO_LRFR19.h: interface for the CRptRatingPSC_AASHTO_LRFR19 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RptRatingPSC_AASHTO_LRFR19_H__)
#define AFX_RptRatingPSC_AASHTO_LRFR19_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "RptSteelGirder.h"
#include "..\wg_base\Thread.h"
#include "..\wg_db\DBDoc.h"

#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_US_Rating_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_Rating_Struct.h"

#include "CRCDataCtrl.h"
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRptRatingPSC_AASHTO_LRFR19 : public CThread
{
public:
    CRptRatingPSC_AASHTO_LRFR19();
    virtual ~CRptRatingPSC_AASHTO_LRFR19();

    void Start(CString sFileName, int iPrintOpt, BOOL bSaveImage) {};
    void Start() {};


protected:
    CDBDoc* m_pDoc;
    BOOL m_bStopExecute;
    CCRCDataCtrl* m_pDataCtrl;

    CCRCForceCtrl* m_pForcCtrl;

public:
    BOOL Execute_RatingReport(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage);

    BOOL GetMvldLoadCaseName(T_MVLD_K MvldK, CString &strName); // CRCDataCtrl∑Œ ¿Ãµø.

protected:
    void Convert_FlexureElemPos(CString strRcaseName, BOOL bPositiveM, T_RTBC_BASE &RcstB, RAT_SFLE_BASE &rData);
    void Convert_ShearElemPos(CString strRcaseName, T_RTSC_BASE &RcstB, RAT_SSHR_BASE &rData);
    void Convert_StressElemPos(CString strRcaseName, BOOL bIsComp, T_RTST_BASE &RcssB, RAT_SSTR_BASE &rData);
    void Convert_StressElemPosPermit(CString strRcaseName, T_RTST_PERMIT_BASE &RcssB, RAT_SSTR_PERMIT_BASE &rData);

    void Convert_FlexureElem4RCase(T_RTBC_BASE &RcstB, RAT_LRSR_FLEX_RCASE &rData);
    void Convert_ShearElem4RCase(T_RTSC_BASE &RcstB, RAT_LRSR_SHEAR_RCASE &rData);
    void Convert_StressElem4RCase(T_RTST_BASE &RcssB, RAT_LRSR_STRESS_RCASE &rData, int nRFCTmin);
    void Convert_StressElem4RCasePermit(T_RTST_PERMIT_BASE &RcssB, RAT_LRSR_STRESS_PERMIT_RCASE &rData);

private:
    void CheckFlexReportData(T_BLRS_D ParamD, RAT_SFLE_D &FlexD);
    void CheckShearReportData(T_BLRS_D ParamD, RAT_SSHR_D &ShearD);
    void CheckSLSReportData(T_BLRS_D ParamD, RAT_SSTR_D &StressD);

};

#include "HeaderPost.h"

#endif // !defined(AFX_RptRatingSTL_AASHTO_LRFR19_H__)
