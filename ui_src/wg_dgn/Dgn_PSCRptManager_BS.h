// Dgn_PSCRptManager_BS.h: interface for the CDgn_PSCRptManager_IRS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Dgn_PSCRptManager_BS_H__320D0EDD_A144_432B_A6E6_1B91E6B8F83C__INCLUDED_)
#define AFX_Dgn_PSCRptManager_BS_H__320D0EDD_A144_432B_A6E6_1B91E6B8F83C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DBDoc.h"

#include "../wg_base/MSExcel.h"
//#include "Rating_XLOut.h"
#include "Dgn_DllManager_Base.h"

struct PSC_RPT_BS_D;

typedef BOOL(PSC_Print_Detail_Design_BS)(int, void*, void*, void*, void*, size_t, size_t, size_t, BOOL&);

class CDgn_PSCRptManager_BS : public CDgn_DllManager_Base
{
public:
    CDgn_PSCRptManager_BS();
    virtual ~CDgn_PSCRptManager_BS();

protected:
    PDGNENGINE_RPT_PROG*     m_pDetail;

public:
    BOOL InitialData(int iNationType);

    BOOL Print_DetailReport_BS(int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName, PSC_RPT_BS_D &rData);

private:
    BOOL m_bInitialized;
 
};

#endif // !defined(AFX_Dgn_PSCRptManager_BS_H__320D0EDD_A144_432B_A6E6_1B91E6B8F83C__INCLUDED_)