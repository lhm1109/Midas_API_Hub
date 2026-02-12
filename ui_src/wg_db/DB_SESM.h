#ifndef __DB_SESM_DB_H__
#define __DB_SESM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MATL;

class CDB_SESM
{
public:
	CDB_SESM();
	virtual ~CDB_SESM();

public:
	void Add(T_SESM_K Key,T_SESM_D& rData,CDB_MATL* pMatl);
	BOOL Del(T_SESM_K Key,CDB_MATL* pMatl);

public:
	BOOL Get(T_SESM_K Key,T_SESM_D& rData)
	{return m_sesm.Lookup(Key,rData);}
	int GetCount()
	{return m_sesm.GetCount();}
	POSITION GetStart()
	{return m_sesm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SESM_K& rKey,T_SESM_D& rData)
	{m_sesm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SESM_K,T_SESM_K,T_SESM_D,T_SESM_D&>m_sesm;
};

#endif