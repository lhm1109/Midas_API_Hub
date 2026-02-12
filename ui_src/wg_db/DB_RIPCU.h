#ifndef __DB_RIPCU_DB_H__
#define __DB_RIPCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RIPCU
{
public:
	CDB_RIPCU()
	{
		m_ripc.InitHashTable(HASHSIZERIPC);
	}
	virtual ~CDB_RIPCU(){};

public:
	void Add(T_UDRD_KEY Key,T_RIPC_UDRD_D& rData)
		{m_ripc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ripc.RemoveKey(Key);}
	void DelAll()
		{m_ripc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RIPC_UDRD_D& rData)
		{return m_ripc.Lookup(Key,rData);}
	int GetCount()
		{return m_ripc.GetCount();}
	POSITION GetStart()
		{return m_ripc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RIPC_UDRD_D& rData)
		{m_ripc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RIPC_UDRD_D,T_RIPC_UDRD_D&>m_ripc;
};

#endif