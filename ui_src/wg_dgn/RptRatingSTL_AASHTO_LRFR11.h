// RptRatingSTL_AASHTO_LRFR11.h: interface for the CRptRatingSTL_AASHTO_LRFR11 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RptRatingSTL_AASHTO_LRFR_H__)
#define AFX_RptRatingSTL_AASHTO_LRFR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RptSteelGirder.h"
#include "..\wg_base\Thread.h"
#include "..\wg_db\DBDoc.h"

#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_US_Rating_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_Rating_Struct.h"

#include "CRCDataCtrl.h"
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRptRatingSTL_AASHTO_LRFR11 : public CThread
{
public:
	CRptRatingSTL_AASHTO_LRFR11();
	virtual ~CRptRatingSTL_AASHTO_LRFR11();
	
	void Start(CString sFileName, int iPrintOpt, BOOL bSaveImage) {};
	void Start() {};
	

protected:
	CDBDoc* m_pDoc;
	BOOL m_bStopExecute;
	CCRCDataCtrl* m_pDataCtrl;
	T_RPAS_D m_RpasD;
	
	CCRCForceCtrl* m_pForcCtrl;

public:
	BOOL Execute_RatingReport(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage);

	BOOL GetMvldLoadCaseName(T_MVLD_K MvldK, CString &strName); // CRCDataCtrl∑Œ ¿Ãµø.

protected:
	void Convert_FlexureElemPos(CString strRcaseName, BOOL bPositiveM, T_RCST_BASE &RcstB, RAT_SFLE_BASE &rData);
	void Convert_ShearElemPos(double dVuFactor, CString strRcaseName, T_RCST_BASE &RcstB, RAT_SSHR_BASE &rData);
	void Convert_StressElemPos(CString strRcaseName, BOOL bIsComp, T_RCSS_BASE &RcssB, RAT_SSTR_BASE &rData);
	void Convert_FatigueElemPos(CString strRcaseName, BOOL bIsTop, T_RCFA_BASE &RcfaB, RAT_FATI_BASE &rData);

	void Convert_FlexureElem4RCase(T_RCST_BASE &RcstB, RAT_LRSR_FLEX_RCASE &rData);
	void Convert_ShearElem4RCase(T_RCST_BASE &RcstB, RAT_LRSR_SHEAR_RCASE &rData);
	void Convert_StressElem4RCase(T_RCSS_BASE &RcssB, RAT_LRSR_STRESS_RCASE &rData);
	//void Convert_FatigueElem4RCase(T_RCSS_BASE &RcssB, RAT_LRSR_STRESS_RCASE &rData);

	
};

#include "HeaderPost.h"

#endif // !defined(AFX_RptRatingSTL_AASHTO_LRFR_H__)
