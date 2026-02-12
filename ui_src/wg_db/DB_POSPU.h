#ifndef __DB_POSPU_DB_H__
#define __DB_POSPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POSPU
{
public:
	CDB_POSPU()
	{
		m_posp.InitHashTable(HASHSIZEPOSP);
	}
	virtual ~CDB_POSPU() {};

public:
	void Add(T_UDRD_KEY Key, T_POSP_UDRD_D& rData)
	{
		m_posp.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_posp.RemoveKey(Key);
	}
	void DelAll()
	{
		m_posp.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_POSP_UDRD_D& rData)
	{
		return m_posp.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_posp.GetCount();
	}
	POSITION GetStart()
	{
		return m_posp.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_POSP_UDRD_D& rData)
	{
		m_posp.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_POSP_UDRD_D, T_POSP_UDRD_D&>m_posp;
};

#endif