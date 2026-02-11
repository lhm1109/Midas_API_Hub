#pragma once

#include <afxtempl.h>

class CDB_SECOU
{
public:
	CDB_SECOU()
	{
		m_SECO.InitHashTable(HASHSIZESECO);
	}
	virtual ~CDB_SECOU() {};

public:
	void Add(T_UDRD_KEY Key, T_SECO_UDRD_D& rData)
	{
		m_SECO.SetAt(Key, rData);
	}

	BOOL Del(T_UDRD_KEY Key)
	{
		return m_SECO.RemoveKey(Key);
	}

	void DelAll()
	{
		m_SECO.RemoveAll();
	}

	BOOL Get(T_UDRD_KEY Key, T_SECO_UDRD_D& rData)
	{
		return m_SECO.Lookup(Key, rData);
	}

	int GetCount()
	{
		return m_SECO.GetCount();
	}

	POSITION GetStart()
	{
		return m_SECO.GetStartPosition();
	}

	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SECO_UDRD_D& rData)
	{
		m_SECO.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SECO_UDRD_D, T_SECO_UDRD_D&> m_SECO;
};
