// Report_KSCE_USD03.h: interface for the Report_KSCE_USD03 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REPORT_KSCE_USD03_H__AE4468AD_DAD4_464E_9C44_026FC2B64148__INCLUDED_)
#define AFX_REPORT_KSCE_USD03_H__AE4468AD_DAD4_464E_9C44_026FC2B64148__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\wg_xl\ReportBase.h"

#include "HeaderPre.h"

class CDBDoc;

class __MY_EXT_CLASS__ CReport_KSCE_USD03 : public CReportBase  
{
public:
	CReport_KSCE_USD03();
	virtual ~CReport_KSCE_USD03();

	virtual CString GetFilePathName();

protected:
	virtual BOOL MakeData();
	virtual BOOL MakeDefaultStyle();

	BOOL MakeDataFromDB();
	
	void InitializeData();

	BOOL MakeCoverData();

	BOOL MakeWorkData();
	BOOL MakeGraphData();

protected:
	CDBDoc* m_pDoc;
// 현 보고서에만 사용되는 스타일 추가
protected:
	T_CSS_K m_TextPadding1CssK;     //들여쓰기 1칸을 나타내는 스타일
	T_CSS_K m_TextPadding2CssK;     //들여쓰기 2칸을 나타내는 스타일
	T_CSS_K m_TextValueCssK;        //Text의 내용중 변수값에 해당되는 부분을 나타내는 스타일
	T_CSS_K m_TextPicCaptionCssK;   // 그림의 캡션을 나타내는 스타일
	T_CSS_K m_TDCenterCssK;         // 가운데 정렬된 셀을 나타내는 스타일
};

#include "HeaderPost.h"

#endif // !defined(AFX_REPORT_KSCE_USD03_H__AE4468AD_DAD4_464E_9C44_026FC2B64148__INCLUDED_)
