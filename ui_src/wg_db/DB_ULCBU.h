#ifndef __DB_ULCBU_DB_H__
#define __DB_ULCBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ULCBU
{
public:
	CDB_ULCBU()
	{
		m_ulcb.InitHashTable(HASHSIZEULCB);
	}
	virtual ~CDB_ULCBU(){};

public:
	void Add(T_UDRD_KEY Key,T_ULCB_UDRD_D& rData)
	{
		m_ulcb.SetAt(Key,rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_ulcb.RemoveKey(Key);
	}
	void DelAll()
	{
		m_ulcb.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key,T_ULCB_UDRD_D& rData)
	{
		return m_ulcb.Lookup(Key,rData);
	}
	int GetCount()
	{
		return m_ulcb.GetCount();
	}
	POSITION GetStart()
	{
		return m_ulcb.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ULCB_UDRD_D& rData)
	{
		m_ulcb.GetNextAssoc(rNextPosition,rKey,rData);
	}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_ULCB_UDRD_D,T_ULCB_UDRD_D&> m_ulcb;
};

#endif