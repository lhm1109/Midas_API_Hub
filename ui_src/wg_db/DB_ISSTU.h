#pragma once

#include "DB_ST_DT.h"

class CDB_ISSTU
{
public:
	CDB_ISSTU()
	{
		m_isst.InitHashTable(HASHSIZEISST);
	}
	virtual ~CDB_ISSTU() {};

public:
	void Add(T_UDRD_KEY Key, T_ISST_UDRD_D& rData)
	{
		m_isst.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_isst.RemoveKey(Key);
	}
	void DelAll()
	{
		m_isst.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_ISST_UDRD_D& rData)
	{
		return m_isst.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_isst.GetCount();
	}
	POSITION GetStart()
	{
		return m_isst.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_ISST_UDRD_D& rData)
	{
		m_isst.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_ISST_UDRD_D, T_ISST_UDRD_D&>m_isst;
};

