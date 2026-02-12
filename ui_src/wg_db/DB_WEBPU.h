#ifndef __DB_WEBPU_DB_H__
#define __DB_WEBPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_WEBPU
{
public:
	CDB_WEBPU()
	{
		m_webp.InitHashTable(HASHSIZEWEBP);
	}
	virtual ~CDB_WEBPU() {};

public:
	void Add(T_UDRD_KEY Key, T_WEBP_UDRD_D& rData)
	{
		m_webp.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_webp.RemoveKey(Key);
	}
	void DelAll()
	{
		m_webp.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_WEBP_UDRD_D& rData)
	{
		return m_webp.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_webp.GetCount();
	}
	POSITION GetStart()
	{
		return m_webp.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_WEBP_UDRD_D& rData)
	{
		m_webp.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_WEBP_UDRD_D, T_WEBP_UDRD_D&> m_webp;
};

#endif
