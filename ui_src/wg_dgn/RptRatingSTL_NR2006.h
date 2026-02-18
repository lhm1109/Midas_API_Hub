// RptRatingSTL_NR2006.h: interface for the CRptRatingSTL_NR2006 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RptRatingSTL_NR2006_H__)
#define AFX_RptRatingSTL_NR2006_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\Thread.h"
#include "..\wg_db\DBDoc.h"

#include "CRCDataCtrl.h"
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRptRatingSTL_NR2006 : public CThread
{
	public:
		CRptRatingSTL_NR2006(int nDgnCode);
		virtual ~CRptRatingSTL_NR2006(void);

		void Start(CString sFileName, int iPrintOpt, BOOL bSaveImage) {};
		void Start() {};

	protected:
		CDBDoc* m_pDoc;
		BOOL           m_bStopExecute;
		CCRCDataCtrl* m_pDataCtrl;
		CCRCForceCtrl* m_pForcCtrl;

	public:
		BOOL Execute_StlRatingReport(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage);

	protected:
		// convert functions	

};

#include "HeaderPost.h"

#endif // !defined(AFX_RptRatingSTL_NR2006_H__)


