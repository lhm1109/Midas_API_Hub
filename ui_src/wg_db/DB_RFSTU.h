#pragma once

#include "DB_ST_DT.h"

class CDB_RFSTU
{
public:
	CDB_RFSTU()
	{
		m_rdpa.InitHashTable(HASHSIZERFST);
	}
	virtual ~CDB_RFSTU() {};

public:
	void Add(T_UDRD_KEY Key, T_RFST_UDRD_D& rData)
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
	BOOL Get(T_UDRD_KEY Key, T_RFST_UDRD_D& rData)
	{
		return m_rdpa.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_RFST_UDRD_D, T_RFST_UDRD_D&>m_rdpa;
};

