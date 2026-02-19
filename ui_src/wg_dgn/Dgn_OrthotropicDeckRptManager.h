#if !defined(AFX_Dgn_OrthotropicDeckRptManager_H__INCLUDED_)
#define AFX_Dgn_OrthotropicDeckRptManager_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_DllManager_Base.h"
//#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_CSG_Struct.h"
// #include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
// #include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_CSG_XLStruct.h"
#include "..\dgnengine\src\DGNe_RUS\DgnCalc_RUS_STL_OD_Struct.h"


typedef BOOL (OrthotropicDeck_DetailReport_RUS)(const int, const int, const CStringW &, const CStringW &, const CStringW &, void*,
																								void*, void*, size_t, size_t, BOOL &, BOOL &);

class CDgn_OrthotropicDeckRptManager : public CDgn_DllManager_Base       
{
public:
	CDgn_OrthotropicDeckRptManager();
	virtual ~CDgn_OrthotropicDeckRptManager();
	
protected:
	OrthotropicDeck_DetailReport_RUS*     m_pODDDetail_RUS;
	
public:
	BOOL InitialData(int iNationType);
	
//  BOOL Print_DetailReport(int iDgnCode, CString szSaveFileNameDir, CArray<STL_MEMB_POSD_OD, STL_MEMB_POSD_OD&>& aInData,   CArray<STL_MEMB_RES_SNiP_OD,  STL_MEMB_RES_SNiP_OD&>& aRptData);
	
	BOOL Print_DetailReport(int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szBaseFileDirr, LPCTSTR szSaveFileNameDir, CArray<STL_MEMB_POSD_SNiP_OD, STL_MEMB_POSD_SNiP_OD&>& aInData,   CArray<STL_MEMB_RES_SNiP_OD,  STL_MEMB_RES_SNiP_OD&>& aRptData);

	
};
#endif // !defined(AFX_Dgn_OrthotropicDeckRptManager_H__INCLUDED_)