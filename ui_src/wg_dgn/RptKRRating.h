// RptKRRating.h: interface for the RptKRRating class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RptKRRating_H__)
#define AFX_RptKRRating_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\dgnengine\src\CVL_DgnCalc_KR\DgnCalc_Rating_Struct.h"

class CDBDoc;
class CCurUnitSaver;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CRptKRRating //: public CThread, public CXLOut
{
public:
	CRptKRRating();
	virtual ~CRptKRRating();


protected:
	CDBDoc* m_pDoc;
	BOOL m_bStopExecute;

	CCurUnitSaver m_UnitSaver;

public:
	void Start(CString sFileName, int iPrintOpt, BOOL bSaveImage);
	
	void SetCodeUnit(int nDgnCode);

	// 엔진에서 출력
	BOOL IsPrintPosiData();
	BOOL IsExistPrintCheckOfGlobalData();
	BOOL CheckPosx_NoneTypeElem(T_POSX_D PosxD);
	
	CString GetSaveFileDirEx();
	CString GetSaveFileNameEx(BOOL bWithoutExtension /* = FALSE*/);
	static CString GetLoadCombType(int iMaxType);

};

#include "HeaderPost.h"

#endif // !defined(AFX_RptKRRating_H__)
