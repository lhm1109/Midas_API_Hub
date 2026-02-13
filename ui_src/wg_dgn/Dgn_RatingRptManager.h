// CDgn_RatingRptManager.h: interface for the CDgn_RatingRptManager class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_DGN_RatingRptManager_H__INCLUDED_)
#define AFX_DGN_RatingRptManager_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_DllManager_Base.h"
#include "..\dgnengine\src\CVL_DgnCalc_KR\DgnCalc_Rating_Struct.h"
#include "..\dgnengine\src\CVL_DgnCalc_KR\DgnCalc_KR_STL_Struct.h"
#include "..\wg_base\XLStruct.h"

#include "..\dgnengine\src\CVL_DgnReport_KR\DgnExcel_KR_STL_XLStruct.h"
#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_Rating_Struct.h"

#include "..\dgnengine\src\CVL_DgnReport_KR\DgnExcel_KR_RCSC_XLStruct.h"
#include "..\dgnengine\src\CVL_DgnCalc_KR\DgnCalc_KR_RCSC_BeamStruct.h"
#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_CSG_XLStruct.h"
#include "..\dgnengine\src\DgnBase\DgnBase_CodeStruct.h"

typedef BOOL (Rating_RFReport)(void* , void* , void*, void*, void*, void*, size_t , size_t , size_t, size_t, size_t, size_t, BOOL&);
//typedef BOOL (Rating_RFReport_RC_KSCE_USD)(CStringW, CStringW, CStringW, void*,  void * , size_t , BOOL& );
typedef BOOL (Rating_DetailReport)(void* , void* , void* , size_t , size_t , size_t , BOOL&);
typedef BOOL (Rating_DetailReportSTL)(void*, void*, void*, size_t, size_t, size_t, BOOL&);
typedef BOOL (Rating_DetailReportPSC)(void*, void*, void*, size_t, size_t, size_t, BOOL&);
typedef BOOL (Rating_SetPGMInfo)(CStringW, CStringW, void*, size_t, BOOL&);
typedef BOOL (Rating_RFReport_RC_KSCE_USD)(BOOL&, void *, void *, void *,  void *, void *, void *, void *, size_t, size_t, size_t, size_t, size_t, size_t, size_t, BOOL& , BOOL& );  // Rating Report Design Engine DLL
typedef BOOL (Rating_CptDetailReport)(void* , void* , void* , void* , size_t , size_t , size_t , size_t , BOOL&, BOOL&);


class CDgn_RatingRptManager : public CDgn_DllManager_Base       
{
public:
	CDgn_RatingRptManager();
	virtual ~CDgn_RatingRptManager();
	
	void SetDgnCode(int nDgnCode);
protected:
	Rating_RFReport*     m_pRatingRF;
	Rating_DetailReport* m_pRatingDetailRC;
    Rating_DetailReport* m_pRatingDetailSTL;
    Rating_DetailReportSTL* m_pRatingDetailSTLAASHTO;
    Rating_DetailReportPSC* m_pRatingDetailPSCAASHTO;
	Rating_SetPGMInfo*   m_pRatingPgmSet;
	Rating_RFReport_RC_KSCE_USD*  m_pRatingRF_RC;
	Rating_CptDetailReport* m_pRatingCptDetail;

	int m_nDgnCode;
	
public:
	BOOL InitialData(int iNationType);
	
	BOOL Print_RFReport(CString szProgramDir, CString szProjectFileDir, CString szSaveFileName, D_LCOM_INFO& InData,   DGNEXCEL_KR_STLC_OPTION &OptD, 
											CArray<DGNEXCEL_KR_STL_SECT,  DGNEXCEL_KR_STL_SECT&>& aRptData, 
											CArray<RF_RES_RF_KSCE_ASD_D,  RF_RES_RF_KSCE_ASD_D&>& aRFRptData, 
											CArray<CSG_MEMB_POSD_KR_D,  CSG_MEMB_POSD_KR_D&> &aRptCsdInData,
											CArray<S_KR_LSD_RES_D, S_KR_LSD_RES_D&> &arLSDResD);
	BOOL Print_RF_CptDetailReport(CString szProgramDir, CString szProjectFileDir, CString szSaveFileName,
																DGNEXCEL_KR_STLC_OPTION &OptD, 
																CArray<DGNEXCEL_KR_STL_SECT,  DGNEXCEL_KR_STL_SECT&>& aRptData, 
																CArray<CSG_MEMB_POSD_KSCE_LSD,  CSG_MEMB_POSD_KSCE_LSD&> &aRptCsdInData,
																CArray<CSG_DRES_PRT,CSG_DRES_PRT&> &aCptLSDRes,
																CArray<CSG_MEMB_RES_AASHTO_LRFD_D,CSG_MEMB_RES_AASHTO_LRFD_D&> &aCptKSSCRes);

	BOOL Print_DetailReport(CString szProgramDir, CString szProjectFileDir, CString szSaveFileName, D_LCOM_INFO& InData,   DGNEXCEL_KR_STLC_OPTION &OptD, CArray<DGNEXCEL_KR_STL_SECT,  DGNEXCEL_KR_STL_SECT&>& aRptData);
	BOOL Print_PSCRatingAASHTO(CString szProgramDir, CString szProjectFileDir, CString szSaveFileName, RAT_LRAT_RPT &LRatD);
    BOOL Print_STLRatingAASHTO(CString szProgramDir, CString szProjectFileDir, CString szSaveFileName, RAT_LRAT_RPT &LRatD);

	//BOOL Set_CallProgramInfo(CString strBasePath, CString strDataPath, void* pProgress, size_t Tsize, BOOL& bVersionOK);

	BOOL Print_RC_RFReport(BOOL bBema, CString szProgramDir, CString szProjectFileDir, CString szSaveFileName, 
												 BOOL bPrintParm[3], DGNEXCEL_KR_RCSC_OPTION& OptD,  D_LCOM_INFO& LoadD,
												 CArray<DGNEXCEL_KR_BEAM_SECT, DGNEXCEL_KR_BEAM_SECT&>& arRptResD,
												 CArray<RF_RES_RF_KSCE_RC_D, RF_RES_RF_KSCE_RC_D&>& aRptData,
												 CArray<_UMD_RC_BEAM_KCI, _UMD_RC_BEAM_KCI&>& arStrInD,
												 CArray<_UMD_RC_KR_BEAM_RES, _UMD_RC_KR_BEAM_RES&>& arStrResD);
};
#endif // !defined(AFX_DGN_RatingRptManager_H__INCLUDED_)