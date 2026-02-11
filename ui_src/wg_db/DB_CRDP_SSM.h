#ifndef __DB_CRDP_SSM_DB_H__
#define __DB_CRDP_SSM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECT;

class CDB_CRDP_SSM
{
public:
	CDB_CRDP_SSM();
	virtual ~CDB_CRDP_SSM();

public:
	void Add(T_CRDP_SSM_K Key,T_CRDP_SSM_D& rData,CDB_SECT* pSect);
	BOOL Del(T_CRDP_SSM_K Key,CDB_SECT* pSect);
	void AddDesign(T_CRDP_SSM_K Key,T_CRDP_SSM_D& rData,CDB_SECT* pSect);
	BOOL DelDesign(T_CRDP_SSM_K Key,CDB_SECT* pSect);

public:
	// CRDP_SSM
	BOOL Get(T_CRDP_SSM_K Key,T_CRDP_SSM_D& rData)
		{return m_crdp_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_crdp_ssm.GetCount();}
	POSITION GetStart()
		{return m_crdp_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CRDP_SSM_K& rKey,T_CRDP_SSM_D& rData)
		{m_crdp_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CRDP_SSM_K,T_CRDP_SSM_K,T_CRDP_SSM_D,T_CRDP_SSM_D&>m_crdp_ssm;
};

#endif