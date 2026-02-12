// ReportCommonFunc.h: interface for the CReportCommonFunc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__REPORTCOMMONFUNC_H__)
#define __REPORTCOMMONFUNC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ReportDefine.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CReportCommonFunc 
{
public:
	CReportCommonFunc();
	virtual ~CReportCommonFunc();

	static void InsertReportHeaderFooter();
	static void GetAllProjStr(CStringArray& aAllProjStr);
	static void ExportData2Word(GEN_IFTAG* pIftag, int nNum=1);
	static void ExportData2Word_ByStyle(GEN_IFTAG* pIftag, int nStyleNum=0);
	static void ChangeStructStr2Char(GEN_IFTAG_STR* pSrc, GEN_IFTAG* pTgt, int nNum=1);
	static void ExportInsertPageBreak();
	static void ExportInsertCatalog(int nLevel);
	static void ExportUpdateCatalog();
	static void ExportInsertPageNumber(int nHeaderFooter, int nAlignment, bool bFirstPage);


	static BOOL GetHeaderFooterStr(CString& strHeader, CString& strFooter);
	static void GetSelectedStr(CArray<UINT,UINT>& aSelected, CString& strRes);
	static void InitAllProjStr();

public:
	static CStringArray m_aAllProjStr; // Header,Footer의 전체 List 문자열(index 맞추기 위해 1부터 시작)
	
};
#include "HeaderPost.h"

#endif // !defined(__REPORTCOMMONFUNC_H__)