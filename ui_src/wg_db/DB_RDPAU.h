#pragma once

#include "DB_ST_DT.h"

class CDB_RDPAU
{
public:
	CDB_RDPAU()
	{
		m_rdpa.InitHashTable(HASHSIZERDPA);
	}
	virtual ~CDB_RDPAU() {};

public:
	void Add(T_UDRD_KEY Key, T_RDPA_UDRD_D& rData)
	{
		m_rdpa.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_rdpa.RemoveKey(Key);
	}
	void DelAll()
	{
		m_rdpa.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_RDPA_UDRD_D& rData)
	{
		return m_rdpa.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_RDPA_UDRD_D, T_RDPA_UDRD_D&>m_rdpa;
};

