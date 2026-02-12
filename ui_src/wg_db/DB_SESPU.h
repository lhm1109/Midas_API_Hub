#pragma once

#include <afxtempl.h>

class CDB_SESPU
{
public:
	CDB_SESPU()
	{
		m_SESP.InitHashTable(HASHSIZESESP);
	}
	virtual ~CDB_SESPU() {};

public:
	void Add(T_UDRD_KEY Key, T_SESP_UDRD_D& rData)
	{
		m_SESP.SetAt(Key, rData);
	}

	BOOL Del(T_UDRD_KEY Key)
	{
		return m_SESP.RemoveKey(Key);
	}

	void DelAll()
	{
		m_SESP.RemoveAll();
	}

	BOOL Get(T_UDRD_KEY Key, T_SESP_UDRD_D& rData)
	{
		return m_SESP.Lookup(Key, rData);
	}

	int GetCount()
	{
		return m_SESP.GetCount();
	}

	POSITION GetStart()
	{
		return m_SESP.GetStartPosition();
	}

	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SESP_UDRD_D& rData)
	{
		m_SESP.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SESP_UDRD_D, T_SESP_UDRD_D&> m_SESP;
};
