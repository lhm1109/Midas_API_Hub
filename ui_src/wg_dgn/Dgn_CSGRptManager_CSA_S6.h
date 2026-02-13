// DGN_CSGRptManager_CSA_S6.h: interface for the CDgn_CSGRptManager_CSA_S6 class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_DGN_CSGRptManager_CSA_S6_H__INCLUDED_)
#define AFX_DGN_CSGRptManager_CSA_S6_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_DllManager_Base.h"

#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_US\DgnReport_CSG_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_CSG_XLStruct.h"

typedef BOOL (CSG_Print_DetailReport_CSA_S6)(void*, void*, void*, void*, size_t, size_t, BOOL &, BOOL &);

class CDgn_CSGRptManager_CSA_S6 : public CDgn_DllManager_Base
{
public:
	CDgn_CSGRptManager_CSA_S6();
	virtual ~CDgn_CSGRptManager_CSA_S6();

protected:
	CSG_Print_DetailReport_CSA_S6* m_pCSGDetail_CSA_S6;

public:
	BOOL InitialData(int iNationType);

	BOOL Print_DetailReport(int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName,
		CArray<CSG_MEMB_POSD_KSCE_LSD, CSG_MEMB_POSD_KSCE_LSD&>& aInData,   CArray<CSG_RPT_RES_CSA_14,  CSG_RPT_RES_CSA_14&>& aRptData);

};
#endif // !defined(AFX_DGN_CSGRptManager_CSA_S6_H__INCLUDED_)