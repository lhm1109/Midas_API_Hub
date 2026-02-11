#pragma once

#include "DB_ST_DT.h"

class CDB_RRTRU
{
public:
	CDB_RRTRU()
	{
		m_rrtr.InitHashTable(HASHSIZERRTR);
	}
	virtual ~CDB_RRTRU() {};

public:
	void Add(T_UDRD_KEY Key, T_RRTR_UDRD_D& rData)
	{
		m_rrtr.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_rrtr.RemoveKey(Key);
	}
	void DelAll()
	{
		m_rrtr.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_RRTR_UDRD_D& rData)
	{
		return m_rrtr.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_RRTR_UDRD_D, T_RRTR_UDRD_D&> m_rrtr;
};

