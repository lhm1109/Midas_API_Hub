// DGN_CSGRptManager_KSSC2014.h: interface for the DGN_CSGRptManager_KSSC2014 class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_DGN_CSGRptManager_KSSC2014_H__INCLUDED_)
#define AFX_DGN_CSGRptManager_KSSC2014_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_CSGRptManager_AASHTO12.h"

typedef BOOL (CSG_Print_DetailReport_KSSC2014)(void*, void*,
																						 void*, void*, size_t, size_t, BOOL &, BOOL &);

class CDgn_CSGRptManager_KSSC2014 : public CDgn_CSGRptManager_AASHTO12       
{
public:
	CDgn_CSGRptManager_KSSC2014();
	virtual ~CDgn_CSGRptManager_KSSC2014();

protected:
	CSG_Print_DetailReport_KSSC2014*     m_pCSGDetail_KSSC2014;

public:
	virtual BOOL InitialData(int iNationType);

 virtual  BOOL Print_DetailReport(int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName,
																																			CArray<CSG_MEMB_POSD_KSCE_LSD, CSG_MEMB_POSD_KSCE_LSD&>& aInData,   CArray<CSG_MEMB_RES_AASHTO_LRFD_D,  CSG_MEMB_RES_AASHTO_LRFD_D&>& aRptData);

};
#endif // !defined(AFX_DGN_CSGRptManager_KSSC2014_H__INCLUDED_)