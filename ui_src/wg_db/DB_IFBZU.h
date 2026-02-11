#ifndef __DB_IFBZU_DB_H__
#define __DB_IFBZU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_IFBZU
{
public:
	CDB_IFBZU()
	{
		m_ifbz.InitHashTable(HASHSIZEIFBZ);
	}
	virtual ~CDB_IFBZU() {};

public:
	void Add(T_UDRD_KEY Key, T_IFBZ_UDRD_D& rData)
	{
		m_ifbz.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_ifbz.RemoveKey(Key);
	}
	void DelAll()
	{
		m_ifbz.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_IFBZ_UDRD_D& rData)
	{
		return m_ifbz.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_IFBZ_UDRD_D, T_IFBZ_UDRD_D&> m_ifbz;
};

#endif