#pragma once

#include "DB_ST_DT.h"

class CDB_DSSTU
{
public:
	CDB_DSSTU()
	{
		m_dsst.InitHashTable(HASHSIZEDSST);
	}
	virtual ~CDB_DSSTU() {};

public:
	void Add(T_UDRD_KEY Key, T_DSST_UDRD_D& rData)
	{
		m_dsst.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_dsst.RemoveKey(Key);
	}
	void DelAll()
	{
		m_dsst.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_DSST_UDRD_D& rData)
	{
		return m_dsst.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_dsst.GetCount();
	}
	POSITION GetStart()
	{
		return m_dsst.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_DSST_UDRD_D& rData)
	{
		m_dsst.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_DSST_UDRD_D, T_DSST_UDRD_D&>m_dsst;
};

