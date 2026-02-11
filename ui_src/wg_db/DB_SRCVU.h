#ifndef __DB_SRCVU_DB_H__
#define __DB_SRCVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SRCVU
{
public:
	CDB_SRCVU()
	{
		m_srcv.InitHashTable(HASHSIZESRCV);
	}
	virtual ~CDB_SRCVU() {};

public:
	void Add(T_UDRD_KEY Key, T_SRCV_UDRD_D& rData)
	{
		m_srcv.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_srcv.RemoveKey(Key);
	}
	void DelAll()
	{
		m_srcv.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SRCV_UDRD_D& rData)
	{
		return m_srcv.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_srcv.GetCount();
	}
	POSITION GetStart()
	{
		return m_srcv.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SRCV_UDRD_D& rData)
	{
		m_srcv.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SRCV_UDRD_D, T_SRCV_UDRD_D&>m_srcv;
};

#endif