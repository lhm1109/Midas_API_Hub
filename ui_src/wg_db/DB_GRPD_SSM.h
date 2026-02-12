#ifndef __DB_GRPD_SSM_DB_H__
#define __DB_GRPD_SSM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECT;

class CDB_GRPD_SSM
{
public:
	CDB_GRPD_SSM();
	virtual ~CDB_GRPD_SSM();

public:
	void Add(T_GRPD_SSM_K Key,T_GRPD_SSM_D& rData,CDB_SECT* pSect);
	BOOL Del(T_GRPD_SSM_K Key,CDB_SECT* pSect);

public:
	// grpd_ssm
	BOOL Get(T_GRPD_SSM_K Key,T_GRPD_SSM_D& rData)
		{return m_grpd_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_grpd_ssm.GetCount();}
	POSITION GetStart()
		{return m_grpd_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_GRPD_SSM_K& rKey,T_GRPD_SSM_D& rData)
		{m_grpd_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GRPD_SSM_K,T_GRPD_SSM_K,T_GRPD_SSM_D,T_GRPD_SSM_D&>m_grpd_ssm;
};

#endif