#ifndef __DB_SPSCU_DB_H__
#define __DB_SPSCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_SPSCU
{
public:
	CDB_SPSCU()
	{
		m_SPSC.InitHashTable(HASHSIZESPSC);
	}
	virtual ~CDB_SPSCU() {};

public:
	void Add(T_UDRD_KEY Key, T_SPSC_UDRD_D& rData)
	{
		m_SPSC.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_SPSC.RemoveKey(Key);
	}
	void DelAll()
	{
		m_SPSC.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SPSC_UDRD_D& rData)
	{
		return m_SPSC.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_SPSC.GetCount();
	}
	POSITION GetStart()
	{
		return m_SPSC.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SPSC_UDRD_D& rData)
	{
		m_SPSC.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SPSC_UDRD_D, T_SPSC_UDRD_D&>m_SPSC;
};

#endif
