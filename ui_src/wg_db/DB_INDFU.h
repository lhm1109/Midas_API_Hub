#pragma once

#include "DB_ST_DT.h"

class CDB_INDFU
{
public:
	CDB_INDFU()
	{
		m_indf.InitHashTable(HASHSIZEINDF);
	}
	virtual ~CDB_INDFU() {};

public:
	void Add(T_UDRD_KEY Key, T_INDF_UDRD_D& rData)
	{
		m_indf.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_indf.RemoveKey(Key);
	}
	void DelAll()
	{
		m_indf.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_INDF_UDRD_D& rData)
	{
		return m_indf.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_INDF_UDRD_D, T_INDF_UDRD_D&>m_indf;
};

