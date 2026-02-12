#ifndef __DB_SEMM_DB_H__
#define __DB_SEMM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MATL;

class CDB_SEMM
{
public:
	CDB_SEMM();
	virtual ~CDB_SEMM();

public:
	void Add(T_SEMM_K Key,T_SEMM_D& rData,CDB_MATL* pMatl);
	BOOL Del(T_SEMM_K Key,CDB_MATL* pMatl);

public:
	BOOL Get(T_SEMM_K Key,T_SEMM_D& rData)
	{return m_semm.Lookup(Key,rData);}
	int GetCount()
	{return m_semm.GetCount();}
	POSITION GetStart()
	{return m_semm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SEMM_K& rKey,T_SEMM_D& rData)
	{m_semm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SEMM_K,T_SEMM_K,T_SEMM_D,T_SEMM_D&>m_semm;
};

#endif