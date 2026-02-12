#pragma once

#include "DB_ST_DT.h"

class CDB_MNDCU
{
public:
	CDB_MNDCU()
	{
		m_mndc.InitHashTable(HASHSIZEMNDC);
	}
	virtual ~CDB_MNDCU() {};

public:
	void Add(T_UDRD_KEY Key, T_MNDC_UDRD_D& rData)
	{
		m_mndc.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_mndc.RemoveKey(Key);
	}
	void DelAll()
	{
		m_mndc.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_MNDC_UDRD_D& rData)
	{
		return m_mndc.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_MNDC_UDRD_D, T_MNDC_UDRD_D&>m_mndc;
};

