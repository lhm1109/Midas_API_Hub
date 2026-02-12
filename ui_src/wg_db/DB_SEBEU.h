#pragma once

#include <afxtempl.h>

class CDB_SEBEU
{
public:
	CDB_SEBEU()
	{
		m_SEBE.InitHashTable(HASHSIZESEBE);
	}
	virtual ~CDB_SEBEU() {};

public:
	void Add(T_UDRD_KEY Key, T_SEBE_UDRD_D& rData)
	{
		m_SEBE.SetAt(Key, rData);
	}

	BOOL Del(T_UDRD_KEY Key)
	{
		return m_SEBE.RemoveKey(Key);
	}

	void DelAll()
	{
		m_SEBE.RemoveAll();
	}

	BOOL Get(T_UDRD_KEY Key, T_SEBE_UDRD_D& rData)
	{
		return m_SEBE.Lookup(Key, rData);
	}

	int GetCount()
	{
		return m_SEBE.GetCount();
	}

	POSITION GetStart()
	{
		return m_SEBE.GetStartPosition();
	}

	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SEBE_UDRD_D& rData)
	{
		m_SEBE.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SEBE_UDRD_D, T_SEBE_UDRD_D&> m_SEBE;
};
