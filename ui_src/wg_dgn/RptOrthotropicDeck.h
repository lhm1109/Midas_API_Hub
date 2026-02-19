#if !defined(AFX_RptOrthotropicDeck_H__)
#define AFX_RptOrthotropicDeck_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
// #include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_CSG_XLStruct.h"

class CDBDoc;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRptOrthotropicDeck 
{
public:
	CRptOrthotropicDeck();
	virtual ~CRptOrthotropicDeck();
	
	
protected:
	CDBDoc* m_pDoc;
	T_SODD_D m_SoddD;
//   BOOL m_bStopExecute;
	
	
public:
	void Start(CString sFileName, int iPrintOpt, BOOL bSaveImage);
	
	
	// 엔진에서 출력
//   BOOL IsPrintPosiData();
//   BOOL IsExistPrintCheckOfGlobalData();
//   BOOL CheckPosx_NoneTypeElem(T_POSX_D PosxD);
//   
	CString GetSaveFileDirEx();
	CString GetSaveFileNameEx(BOOL bWithoutExtension = FALSE);
//   static CString GetLoadCombType(int iMaxType);
	
};

#include "HeaderPost.h"

#endif // !defined(AFX_RptOrthotropicDeck_H__)
