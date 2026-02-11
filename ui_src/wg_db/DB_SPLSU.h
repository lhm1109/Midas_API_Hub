//spls
#ifndef __DB_SPLSU_DB_H__
#define __DB_SPLSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_SPLSU
{
public:
	CDB_SPLSU()
	{
		m_SPLS.InitHashTable(HASHSIZESPLS);
	}
	virtual ~CDB_SPLSU() {};

public:
	void Add(T_UDRD_KEY Key, T_SPLS_UDRD_D& rData)
	{
		m_SPLS.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_SPLS.RemoveKey(Key);
	}
	void DelAll()
	{
		m_SPLS.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SPLS_UDRD_D& rData)
	{
		return m_SPLS.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_SPLS.GetCount();
	}
	POSITION GetStart()
	{
		return m_SPLS.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SPLS_UDRD_D& rData)
	{
		m_SPLS.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SPLS_UDRD_D, T_SPLS_UDRD_D&>m_SPLS;
};

#endif
