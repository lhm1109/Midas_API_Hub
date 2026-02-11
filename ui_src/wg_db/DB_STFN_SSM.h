#ifndef __DB_STFN_SSM_DB_H__
#define __DB_STFN_SSM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECT;

class CDB_STFN_SSM
{
public:
	CDB_STFN_SSM();
	virtual ~CDB_STFN_SSM();

public:
	void Add(T_STFN_SSM_K Key,T_STFN_SSM_D& rData,CDB_SECT* pSect);
	BOOL Del(T_STFN_SSM_K Key,CDB_SECT* pSect);

public:
	// stfn_ssm
	BOOL Get(T_STFN_SSM_K Key,T_STFN_SSM_D& rData)
		{return m_stfn_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_stfn_ssm.GetCount();}
	POSITION GetStart()
		{return m_stfn_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_STFN_SSM_K& rKey,T_STFN_SSM_D& rData)
		{m_stfn_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_STFN_SSM_K,T_STFN_SSM_K,T_STFN_SSM_D,T_STFN_SSM_D&>m_stfn_ssm;
};

#endif