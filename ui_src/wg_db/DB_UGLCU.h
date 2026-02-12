#ifndef __DB_UGLCU_DB_H__
#define __DB_UGLCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_UGLCU
{
public:
	CDB_UGLCU()
	{
		m_uglc.InitHashTable(HASHSIZEUGLC);
	}
	virtual ~CDB_UGLCU() {};

public:
	void Add(T_UDRD_KEY Key, T_UGLC_UDRD_D& rData)
	{
		m_uglc.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_uglc.RemoveKey(Key);
	}
	void DelAll()
	{
		m_uglc.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_UGLC_UDRD_D& rData)
	{
		return m_uglc.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_UGLC_UDRD_D, T_UGLC_UDRD_D&>m_uglc;
};

#endif