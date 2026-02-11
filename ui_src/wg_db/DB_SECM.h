#ifndef __DB_SECM_DB_H__
#define __DB_SECM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MATL;

class CDB_SECM
{
public:
	CDB_SECM();
	virtual ~CDB_SECM();

public:
	void Add(T_SECM_K Key,T_SECM_D& rData,CDB_MATL* pMatl);
	BOOL Del(T_SECM_K Key,CDB_MATL* pMatl);

public:
	BOOL Get(T_SECM_K Key,T_SECM_D& rData)
	{return m_secm.Lookup(Key,rData);}
	int GetCount()
	{return m_secm.GetCount();}
	POSITION GetStart()
	{return m_secm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SECM_K& rKey,T_SECM_D& rData)
	{m_secm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SECM_K,T_SECM_K,T_SECM_D,T_SECM_D&>m_secm;
};

#endif