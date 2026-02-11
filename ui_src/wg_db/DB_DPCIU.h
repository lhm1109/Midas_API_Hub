#pragma once

#include "DB_ST_DT.h"

class CDB_DPCIU
{
public:
	CDB_DPCIU()
	{
		m_mapDpci.InitHashTable(HASHSIZEISGB);
	}
	virtual ~CDB_DPCIU() {};

public:
	void Add(T_UDRD_KEY Key, T_DPCI_UDRD_D& rData)
	{
		m_mapDpci.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_mapDpci.RemoveKey(Key);
	}
	void DelAll()
	{
		m_mapDpci.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_DPCI_UDRD_D& rData)
	{
		return m_mapDpci.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_DPCI_UDRD_D, T_DPCI_UDRD_D&>m_mapDpci;
};

