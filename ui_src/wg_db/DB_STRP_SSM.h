#ifndef __DB_STRP_SSM_DB_H__
#define __DB_STRP_SSM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECT;

class CDB_STRP_SSM
{
public:
	CDB_STRP_SSM();
	virtual ~CDB_STRP_SSM();

public:
	void Add(T_STRP_SSM_K Key,T_STRP_SSM_D& rData,CDB_SECT* pSect);
	BOOL Del(T_STRP_SSM_K Key,CDB_SECT* pSect);

public:
	// strp_ssm
	BOOL Get(T_STRP_SSM_K Key,T_STRP_SSM_D& rData)
		{return m_strp_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_strp_ssm.GetCount();}
	POSITION GetStart()
		{return m_strp_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_STRP_SSM_K& rKey,T_STRP_SSM_D& rData)
		{m_strp_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_STRP_SSM_K,T_STRP_SSM_K,T_STRP_SSM_D,T_STRP_SSM_D&>m_strp_ssm;
};

#endif