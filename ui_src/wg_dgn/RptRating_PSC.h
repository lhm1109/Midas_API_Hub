// RptRating_PSC.h: interface for the RptRating_PSC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RptRating_PSC_H__)
#define AFX_RptRating_PSC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RptSteelGirder.h"
#include "..\wg_base\Thread.h"
#include "..\wg_db\DB_ST_DN_RESULT_Rating.h"
 
#include "..\dgnengine\src\CVL_DgnCalc_KR\DgnCalc_Rating_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_KR\DgnExcel_KR_RCSC_XLStruct.h"
//#include "..\dgnengine\src\CVL_DgnReport_KR\DgnExcel_KR_STL_XLStruct.h"
//#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_CSG_XLStruct.h"
//#include "..\dgndb\src\CVL_UmdDataBase\DB_ST_STL.H"
//#include "..\dgndb\src\CVL_UmdDataBase\DB_ST_OPT.H"
//#include "..\dgnengine\src\CVL_DgnCalc_KR\DgnCalc_KR_STL_Struct.h"

class CDBDoc;
class CCRCDataCtrl;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRptRating_PSC : public CThread
{
public:
	CRptRating_PSC();
	virtual ~CRptRating_PSC();
	
	
protected:
	CDBDoc* m_pDoc;
	BOOL m_bStopExecute;
	CCRCDataCtrl* m_pDataCtrl;
	T_RAKR_D m_RakrD;

	CCRCForceCtrl* m_pForcCtrl;
	
public: 
	BOOL Execute_RatingReport(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage);
	void Start(CString sFileName, int iPrintOpt, BOOL bSaveImage) {};
	void Start() {};

protected:

	BOOL ConvertToDLL_RF(ElemPairK ElemK, int nIJ, int nPosiNega, T_RKMC_BASE& BaseD, RF_RES_RF_KSCE_PSC_D& RptD);

};

#include "HeaderPost.h"

#endif // !defined(AFX_RptRating_PSC_H__)
