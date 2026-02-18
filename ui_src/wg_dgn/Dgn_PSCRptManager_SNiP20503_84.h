// Dgn_PSCRptManager_SNiP20503_84.h: interface for the CDgn_PSCRptManager_SNiP20503_84 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Dgn_PSCRptManager_SNiP20503_84_H__320D0EDD_A144_432B_A6E6_1B91E6B8F83C__INCLUDED_)
#define AFX_Dgn_PSCRptManager_SNiP20503_84_H__320D0EDD_A144_432B_A6E6_1B91E6B8F83C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_DllManager_Base.h"
#include "DgnPscCommon.h"

struct PSC_DRES_RPT_SNIP;
struct PSC_FYSG_BASE;
struct PSC_FYCM_BASE;
struct PSC_FPSG_BASE;
struct PSC_FPMS_BASE;
struct PSC_FYPC_D;

typedef BOOL (PSC_Print_Detail_Design_SNiP)(void*, size_t,
																						void*, void*, void*, void*, void*, void*, void*,
																						size_t, size_t, size_t, size_t, size_t, size_t, size_t, BOOL &, BOOL &);

class CDgn_PSCRptManager_SNiP20503_84 : public CDgn_DllManager_Base       
{
public:
	CDgn_PSCRptManager_SNiP20503_84();
	virtual ~CDgn_PSCRptManager_SNiP20503_84();

protected:
	PSC_Print_Detail_Design_SNiP*     m_pDetail;
	
public:
	BOOL InitialData(int iNationType);

	BOOL Print_DetailReport(int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName,
													CArray<PSC_DATA_POSI, PSC_DATA_POSI&>& aInData,   CArray<PSC_DRES_RPT_SNIP, PSC_DRES_RPT_SNIP&>& aRptData,
		                      CArray<PSC_FYSG_BASE, PSC_FYSG_BASE&>& aFysgSNiP, CArray<PSC_FYCM_BASE, PSC_FYCM_BASE&>& aFycmSNiP,
													CArray<PSC_FPSG_BASE, PSC_FPSG_BASE&>& aFpsgSNiP, CArray<PSC_FPMS_BASE, PSC_FPMS_BASE&>& aFpmsSNiP,
													CArray<PSC_FYPC_D, PSC_FYPC_D&>& aFypcSNiP);
};
#endif // !defined(AFX_Dgn_PSCRptManager_SNiP20503_84_H__320D0EDD_A144_432B_A6E6_1B91E6B8F83C__INCLUDED_)