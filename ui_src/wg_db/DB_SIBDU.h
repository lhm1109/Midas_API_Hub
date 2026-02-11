#ifndef __DB_SIBDU_DB_H__
#define __DB_SIBDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIBDU
{
public:
	CDB_SIBDU()
	{
		m_sibd.InitHashTable(HASHSIZESIBD);
	}
	virtual ~CDB_SIBDU() {};

public:
	void Add(T_UDRD_KEY Key, T_SIBD_UDRD_D& rData)
	{
		m_sibd.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_sibd.RemoveKey(Key);
	}
	void DelAll()
	{
		m_sibd.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SIBD_UDRD_D& rData)
	{
		return m_sibd.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_sibd.GetCount();
	}
	POSITION GetStart()
	{
		return m_sibd.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SIBD_UDRD_D& rData)
	{
		m_sibd.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SIBD_UDRD_D, T_SIBD_UDRD_D&> m_sibd;
};

#endif