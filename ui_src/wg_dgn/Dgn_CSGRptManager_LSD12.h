// DGN_CSGRptManager_LSD12.h: interface for the CDgn_CSGRptManager_LSD12 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGN_CSGRptManager_LSD12_H__INCLUDED_)
#define AFX_DGN_CSGRptManager_LSD12_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_DllManager_Base.h"
//#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_CSG_Struct.h"
#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_CSG_XLStruct.h"
#include "DgnPscCommon.h"

typedef BOOL (CSG_Print_DetailReport_LSD)(void*, void*, void*, void*, void*, void*, 
																					size_t, size_t, BOOL &, BOOL &);


class CDgn_CSGRptManager_LSD12 : public CDgn_DllManager_Base       
{
public:
	CDgn_CSGRptManager_LSD12();
	virtual ~CDgn_CSGRptManager_LSD12();
	
protected:
	CSG_Print_DetailReport_LSD*         m_pCSGDetail_LSD12;
	
public:
	BOOL InitialData(int iNationType);
	
	BOOL Print_DetailReport(int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName,
													CArray<CSG_INDATA_SPAN, CSG_INDATA_SPAN&>& aSpInData,  CArray<CSG_DRES_SPAN_PRT, CSG_DRES_SPAN_PRT&>& aSpRptData,
													CArray<CSG_MEMB_POSD_KSCE_LSD, CSG_MEMB_POSD_KSCE_LSD&>& aInData,   CArray<CSG_DRES_PRT,  CSG_DRES_PRT&>& aRptData);
//                           CArray<PSC_FYSG_BASE, PSC_FYSG_BASE&>& aFysgSNiP, CArray<PSC_FYCM_BASE, PSC_FYCM_BASE&>& aFycmSNiP,
//                           CArray<PSC_FPSG_BASE, PSC_FPSG_BASE&>& aFpsgSNiP, CArray<PSC_FPMS_BASE, PSC_FPMS_BASE&>& aFpmsSNiP, CArray<PSC_FYPC_D, PSC_FYPC_D&>& aFypcSNiP);
};
#endif // !defined(AFX_DGN_CSGRptManager_LSD12_H__INCLUDED_)