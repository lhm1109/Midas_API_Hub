#pragma once

#include "DB_ST_DT.h"

class CDB_SAFMU
{
public:
	CDB_SAFMU()
	{
		m_safm.InitHashTable(HASHSIZESAFM);
	}
	virtual ~CDB_SAFMU() {};

public:
	void Add(T_UDRD_KEY Key, T_SAFM_UDRD_D& rData)
	{
		m_safm.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_safm.RemoveKey(Key);
	}
	void DelAll()
	{
		m_safm.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SAFM_UDRD_D& rData)
	{
		return m_safm.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SAFM_UDRD_D, T_SAFM_UDRD_D&>m_safm;
};

