#ifndef __DB_SIPAU_DB_H__
#define __DB_SIPAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIPAU
{
public:
	CDB_SIPAU()
	{
		m_sipa.InitHashTable(HASHSIZESIPA);
	}
	virtual ~CDB_SIPAU() {};

public:
	void Add(T_UDRD_KEY Key, T_SIPA_UDRD_D& rData)
	{
		m_sipa.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_sipa.RemoveKey(Key);
	}
	void DelAll()
	{
		m_sipa.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SIPA_UDRD_D& rData)
	{
		return m_sipa.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_sipa.GetCount();
	}
	POSITION GetStart()
	{
		return m_sipa.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SIPA_UDRD_D& rData)
	{
		m_sipa.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SIPA_UDRD_D, T_SIPA_UDRD_D&> m_sipa;
};

#endif