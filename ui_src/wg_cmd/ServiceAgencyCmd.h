// ServiceAgencyCmd.h: interface for the CServiceAgencyCmd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVICEAGENCYCMD_H__06E93BD3_CBC0_4561_B486_F243DA492178__INCLUDED_)
#define AFX_SERVICEAGENCYCMD_H__06E93BD3_CBC0_4561_B486_F243DA492178__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_base\wg_base_ServiceProvider.h"
class CServiceAgencyCmd  : public CServiceAgency
{
public:
	CServiceAgencyCmd();
	virtual ~CServiceAgencyCmd();

public:
	virtual int DoService(int nServiceNo, void* pData);

protected:
	int Dlg_DoModal(void* pData);
	int Dlg_Modeless(void* pData);
	int MakeSpectrumData(void* pData);
};

#endif // !defined(AFX_SERVICEAGENCYCMD_H__06E93BD3_CBC0_4561_B486_F243DA492178__INCLUDED_)
