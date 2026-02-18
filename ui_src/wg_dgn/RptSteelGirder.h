// RptSteelGirder.h: interface for the RptSteelGirder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RptSteelGirder_H__)
#define AFX_RptSteelGirder_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "..\wg_db\DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_CSG_XLStruct.h"
#include "CRCDataCtrl.h"

class CDBDoc;
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRptSteelGirder //: public CThread, public CXLOut
{
public:
	CRptSteelGirder();
	virtual ~CRptSteelGirder();


protected:
	CDBDoc* m_pDoc;
	T_CPGD_D m_CpgdD;
	BOOL m_bStopExecute;


public:
	void Start(CString sFileName, int iPrintOpt, BOOL bSaveImage);
	
	// 엔진에서 출력
	BOOL IsPrintPosiData();
	BOOL IsExistPrintCheckOfGlobalData();
	BOOL CheckPosx_NoneTypeElem(T_POSX_D PosxD);
	
	CString GetSaveFileDirEx();
	CString GetSaveFileNameEx(BOOL bWithoutExtension /* = FALSE*/);
	static CStringW GetLoadCombType(int iMaxType);

};

#include "HeaderPost.h"

#endif // !defined(AFX_RptSteelGirder_H__)
