// DGN_CSGRptManager_AASHTO12.h: interface for the CDgn_CSGRptManager_AASHTO12 class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_DGN_CSGRptManager_AASHTO12_H__INCLUDED_)
#define AFX_DGN_CSGRptManager_AASHTO12_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_DllManager_Base.h"
//#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_CSG_Struct.h"
#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_CSG_XLStruct.h"

typedef BOOL (CSG_Print_DetailReport_AASHTO)(void*, void*,
																						 void*, void*, size_t, size_t, BOOL &, BOOL &);

class CDgn_CSGRptManager_AASHTO12 : public CDgn_DllManager_Base       
{
public:
	CDgn_CSGRptManager_AASHTO12();
	virtual ~CDgn_CSGRptManager_AASHTO12();
	
protected:
	CSG_Print_DetailReport_AASHTO*     m_pCSGDetail_AASHTO12;
	
public:
	BOOL InitialData(int iNationType);
	
	BOOL Print_DetailReport(int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName,
													CArray<CSG_MEMB_POSD_KSCE_LSD, CSG_MEMB_POSD_KSCE_LSD&>& aInData,   CArray<CSG_MEMB_RES_AASHTO_LRFD_D,  CSG_MEMB_RES_AASHTO_LRFD_D&>& aRptData);

};
#endif // !defined(AFX_DGN_CSGRptManager_AASHTO12_H__INCLUDED_)