#pragma once

#include "DB_ST_DT.h"

class CDB_ARSTU
{
public:
	CDB_ARSTU()
	{
		m_arst.InitHashTable(HASHSIZEARST);
	}
	virtual ~CDB_ARSTU() {};

public:
	void Add(T_UDRD_KEY Key, T_ARST_UDRD_D& rData)
	{
		m_arst.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_arst.RemoveKey(Key);
	}
	void DelAll()
	{
		m_arst.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_ARST_UDRD_D& rData)
	{
		return m_arst.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_ARST_UDRD_D, T_ARST_UDRD_D&>m_arst;
};

