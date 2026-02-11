// ServiceAgencyCmd.h: interface for the CServiceAgencyTreemenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVICEAGENCYTREEMENU_H__INCLUDED_)
#define AFX_SERVICEAGENCYTREEMENU_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_base\wg_base_ServiceProvider.h"
class CServiceAgencyTreemenu  : public CServiceAgency
{
public:
	CServiceAgencyTreemenu();
	virtual ~CServiceAgencyTreemenu();

public:
	virtual int DoService(int nServiceNo, void* pData);

private:
	int afxTestMsg();
	int WindPCH2019_ConsiderCrsTor(void* pData);
	int WindPCH2012_ConsiderCrsTor(void* pData);
	int WindPKBC2009_AutoCalcCoeff(void* pData);
	int WindPKBC2016_SelectStructType(void* pData);
	int WindPWindProfile(void* pData, BOOL bKBC2016);
	int WindPCH2001_CalcMagFactor(void* pData);
	int WindPNordalStructure(void* pData);
	int WindPKBC2016_WindDir(void* pData);
	int EarthPStaticProfile(void* pData);
	int EarthPSeismicProfile(void* pData);
	int WindPASCE7_16_SelectStructType(void* pData);
};

#endif // !defined(AFX_SERVICEAGENCYCMD_H__06E93BD3_CBC0_4561_B486_F243DA492178__INCLUDED_)
