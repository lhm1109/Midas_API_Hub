// RptSteelGirder_AASHTO12.h: interface for the RptSteelGirder_AASHTO12 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RptSteelGirder_KSSC2014_H__)
#define AFX_RptSteelGirder_KSSC2014_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RptSteelGirder_AASHTO12.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRptSteelGirder_KSSC2014 : public CRptSteelGirder_AASHTO12
{
public:
	CRptSteelGirder_KSSC2014();
	virtual ~CRptSteelGirder_KSSC2014();

		BOOL Execute_CSGReport_KSSC2014(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage);

protected:

protected:

};

#include "HeaderPost.h"

#endif // !defined(AFX_RptSteelGirder_KSSC2014_H__)
