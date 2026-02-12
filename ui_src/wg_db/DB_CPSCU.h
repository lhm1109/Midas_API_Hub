#ifndef __DB_CPSCU_DB_H__
#define __DB_CPSCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CPSCU
{
public:
	CDB_CPSCU()
	{
		m_cpsc.InitHashTable(HASHSIZECPSC);
	}
	virtual ~CDB_CPSCU(){};

public:
	void Add(T_UDRD_KEY Key,T_CPSC_UDRD_D& rData)
		{m_cpsc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cpsc.RemoveKey(Key);}
	void DelAll()
		{m_cpsc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CPSC_UDRD_D& rData)
		{return m_cpsc.Lookup(Key,rData);}
	int GetCount()
		{return m_cpsc.GetCount();}
	POSITION GetStart()
		{return m_cpsc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CPSC_UDRD_D& rData)
		{m_cpsc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CPSC_UDRD_D,T_CPSC_UDRD_D&>m_cpsc;
};

#endif