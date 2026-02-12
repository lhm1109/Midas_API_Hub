#pragma once

#include "DB_ST_DT.h"

class CDB_RCTIU
{
public:
	CDB_RCTIU()
	{
		m_rcti.InitHashTable(HASHSIZERCTI);
	}
	virtual ~CDB_RCTIU() {};

public:
	void Add(T_UDRD_KEY Key, T_RCTI_UDRD_D& rData)
	{
		m_rcti.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_rcti.RemoveKey(Key);
	}
	void DelAll()
	{
		m_rcti.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_RCTI_UDRD_D& rData)
	{
		return m_rcti.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_RCTI_UDRD_D, T_RCTI_UDRD_D&>m_rcti;
};

