#pragma once

#include <afxtempl.h>

class CDB_HLFWU
{
public:
	CDB_HLFWU()
	{
		m_HLFW.InitHashTable(HASHSIZEHLFW);
	}
	virtual ~CDB_HLFWU() {};

public:
	void Add(T_UDRD_KEY Key, T_HLFW_UDRD_D& rData)
	{
		m_HLFW.SetAt(Key, rData);
	}

	BOOL Del(T_UDRD_KEY Key)
	{
		return m_HLFW.RemoveKey(Key);
	}

	void DelAll()
	{
		m_HLFW.RemoveAll();
	}

	BOOL Get(T_UDRD_KEY Key, T_HLFW_UDRD_D& rData)
	{
		return m_HLFW.Lookup(Key, rData);
	}

	int GetCount()
	{
		return m_HLFW.GetCount();
	}

	POSITION GetStart()
	{
		return m_HLFW.GetStartPosition();
	}

	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_HLFW_UDRD_D& rData)
	{
		m_HLFW.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_HLFW_UDRD_D, T_HLFW_UDRD_D&> m_HLFW;
};
