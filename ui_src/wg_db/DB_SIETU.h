#ifndef __DB_SIETU_DB_H__
#define __DB_SIETU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIETU
{
public:
	CDB_SIETU()
	{
		m_siet.InitHashTable(HASHSIZESIET);
	}
	virtual ~CDB_SIETU() {};

public:
	void Add(T_UDRD_KEY Key, T_SIET_UDRD_D& rData)
	{
		m_siet.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_siet.RemoveKey(Key);
	}
	void DelAll()
	{
		m_siet.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SIET_UDRD_D& rData)
	{
		return m_siet.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_siet.GetCount();
	}
	POSITION GetStart()
	{
		return m_siet.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SIET_UDRD_D& rData)
	{
		m_siet.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SIET_UDRD_D, T_SIET_UDRD_D&> m_siet;
};

#endif