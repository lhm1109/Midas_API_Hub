// RptRatingPSC_BD21_01.h: interface for the CRptRatingPSC_BD21_01 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RptRatingPSC_BD21_01_H__)
#define AFX_RptRatingPSC_BD21_01_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\Thread.h"
#include "..\wg_db\DBDoc.h"

#include "CRCDataCtrl.h"
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRptRatingPSC_BD21_01 : public CThread
{
public:
    CRptRatingPSC_BD21_01(int nDgnCode);
    virtual ~CRptRatingPSC_BD21_01(void);

    void Start(CString sFileName, int iPrintOpt, BOOL bSaveImage) {};
    void Start() {};

protected:
    CDBDoc*        m_pDoc;
    BOOL           m_bStopExecute;
    CCRCDataCtrl*  m_pDataCtrl;
    CCRCForceCtrl* m_pForcCtrl;

public:
    BOOL Execute_RatingReport(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage);
    BOOL Execute_StlRatingReport(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage);
    
protected:
    // convert functions

};

#include "HeaderPost.h"

#endif // !defined(AFX_RptRatingPSC_BD21_01_H__)
