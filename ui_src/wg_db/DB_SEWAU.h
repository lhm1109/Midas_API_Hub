#pragma once

#include <afxtempl.h>

class CDB_SEWAU
{
public:
	CDB_SEWAU()
	{
		m_SEWA.InitHashTable(HASHSIZESEWA);
	}
	virtual ~CDB_SEWAU() {};

public:
	void Add(T_UDRD_KEY Key, T_SEWA_UDRD_D& rData)
	{
		m_SEWA.SetAt(Key, rData);
	}

	BOOL Del(T_UDRD_KEY Key)
	{
		return m_SEWA.RemoveKey(Key);
	}

	void DelAll()
	{
		m_SEWA.RemoveAll();
	}

	BOOL Get(T_UDRD_KEY Key, T_SEWA_UDRD_D& rData)
	{
		return m_SEWA.Lookup(Key, rData);
	}

	int GetCount()
	{
		return m_SEWA.GetCount();
	}

	POSITION GetStart()
	{
		return m_SEWA.GetStartPosition();
	}

	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SEWA_UDRD_D& rData)
	{
		m_SEWA.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SEWA_UDRD_D, T_SEWA_UDRD_D&> m_SEWA;
};
