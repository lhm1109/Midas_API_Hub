// RptRatingKR_STL_ASD.h: interface for the RptRatingKR_STL_ASD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RptRatingKR_STL_ASD_H__)
#define AFX_RptRatingKR_STL_ASD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RptSteelGirder.h"
#include "..\wg_base\Thread.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DB_ST_DN_RESULT_Rating.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
 
#include "..\dgnengine\src\CVL_DgnCalc_KR\DgnCalc_Rating_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_KR\DgnExcel_KR_STL_XLStruct.h"
//#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_CSG_XLStruct.h"
#include "..\dgndb\src\CVL_UmdDataBase\DB_ST_STL.H"
#include "..\dgndb\src\CVL_UmdDataBase\DB_ST_OPT.H"
#include "..\dgnengine\src\CVL_DgnCalc_KR\DgnCalc_KR_STL_Struct.h"

#include "CRCDataCtrl.h"
#include "HeaderPre.h"

class CDgn_SteelRptManager_KSCE;
class __MY_EXT_CLASS__ CRptRatingKR_STL_ASD : public CThread
{
public:
	CRptRatingKR_STL_ASD();
	virtual ~CRptRatingKR_STL_ASD();
	
	
protected:
	CDBDoc* m_pDoc;
	BOOL m_bStopExecute;
	CCRCDataCtrl* m_pDataCtrl;
	T_RPAS_D m_RpasD;

	CCRCForceCtrl* m_pForcCtrl;
	CDgn_SteelRptManager_KSCE* m_pRptManager;
	
public: 
	BOOL Execute_RatingReport(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage);
	void Start(CString sFileName, int iPrintOpt, BOOL bSaveImage) {};
	void Start() {};

protected:	
	// for LRFD && Section Manager
	BOOL ConvertToDLL_STL_TRANSTFN_D(const T_TRAN_STFN_D& UstlStrn, DGNEXCEL_STL_KR_TRAN_STFN_D& ExlStfnD);
	BOOL ConvertToDLL_STL_TRANSTFN(const T_USTL_TRAN_STFN& UstlOptD, DGNEXCEL_STL_KR_TRAN_STFN& ExlOptD);
	
	BOOL ConvertToDLL_STL_POINT_SSM_D(const T_UMD_POINT_SSM& PointD, DGNEXCEL_STL_KR_POINT_SSM& ExlD);  
	BOOL ConvertToDLL_STL_STRP_SSM_D(const T_UMD_STRP_SSM_D& StrpD, DGNEXCEL_STL_KR_STRP_SSM_D& ExlD);
	BOOL ConvertToDLL_STL_GRPD_SUM_SSM(const T_UMD_GRPD_SUB_SSM& GrpdSubD, DGNEXCEL_STL_KR_GRPD_SUB_SSM& ExlD);
	BOOL ConvertToDLL_STL_PART_SSM(const T_UMD_PART_SSM& PartD, DGNEXCEL_STL_KR_PART_SSM& ExlD);
	BOOL ConvertToDLL_STL_GRPD_SSM_D(const T_UMD_GRPD_SSM_D& GrpdD, DGNEXCEL_STL_KR_GRPD_SSM_D& ExlD);
	BOOL ConvertToDLL_STL_STFN_SUB_TYPE_SSM(const T_UMD_STFN_SUB_TYPE_SSM& StfnSubTypeD, DGNEXCEL_STL_KR_STFN_SUB_TYPE_SSM& ExlD);
	BOOL ConvertToDLL_STL_STFN_SUB_SSM(const T_UMD_STFN_SUB_SSM& StfnSubD, DGNEXCEL_STL_KR_STFN_SUB_SSM& ExlD);
	BOOL ConvertToDLL_STL_STFN_SSM_D(const T_UMD_STFN_SSM_D& StfnD, DGNEXCEL_STL_KR_STFN_SSM_D& ExlD);
	BOOL ConvertToDLL_STL_FATI_SUB_SSM(const T_UMD_FATI_SUB_SSM& FatiSubD, DGNEXCEL_STL_KR_FATI_SUB_SSM& ExlD);
	BOOL ConvertToDLL_STL_FATI_SSM_D(const T_UMD_FATI_SSM_D& FatiD, DGNEXCEL_STL_KR_FATI_SSM_D& ExlD);	

	// 내하력 평가
	BOOL ConvertToDLL_RF(UMD_STL_KR_D &Memb, const int &nPosi, const T_RFKR_BASE& BaseD, RF_RES_RF_KSCE_ASD_D &RptD);
	BOOL ConvertToDLL_Force(T_RFKR_D &Rfkr, RF_RES_RF_KSCE_ASD_D &rRptD, int nIJ, CString& strLcom);
	
	int GetCodeConvert2Engine(const UINT &nCode);

public:
	CStringW GetBridgeLevel(const int &nRank);
	BOOL ConvertToDLL_RptForce(T_RATI_UNITLCOM &Force, RF_RES_KS_FORCE_KSCE_ASD_D &rForce);

};

#include "HeaderPost.h"

#endif // !defined(AFX_RptRatingKR_STL_ASD_H__)
