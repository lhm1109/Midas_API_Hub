#ifndef __DB_SRVPU_DB_H__
#define __DB_SRVPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SRVPU
{
public:
	CDB_SRVPU()
	{
		m_srvp.InitHashTable(HASHSIZESRVP);
	}
	virtual ~CDB_SRVPU() {};

public:
	void Add(T_UDRD_KEY Key, T_SRVP_UDRD_D& rData)
	{
		m_srvp.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_srvp.RemoveKey(Key);
	}
	void DelAll()
	{
		m_srvp.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SRVP_UDRD_D& rData)
	{
		return m_srvp.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_srvp.GetCount();
	}
	POSITION GetStart()
	{
		return m_srvp.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SRVP_UDRD_D& rData)
	{
		m_srvp.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SRVP_UDRD_D, T_SRVP_UDRD_D&>m_srvp;
};

#endif