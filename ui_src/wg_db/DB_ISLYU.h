#ifndef __DB_ISLYU_DB_H__
#define __DB_ISLYU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ISLYU
{
public:
	CDB_ISLYU()
	{
		m_isly.InitHashTable(HASHSIZEISLY);
	}
	virtual ~CDB_ISLYU() {};

public:
	void Add(T_UDRD_KEY Key, T_ISLY_UDRD_D& rData)
	{
		m_isly.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_isly.RemoveKey(Key);
	}
	void DelAll()
	{
		m_isly.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_ISLY_UDRD_D& rData)
	{
		return m_isly.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_ISLY_UDRD_D, T_ISLY_UDRD_D&>m_isly;
};

#endif