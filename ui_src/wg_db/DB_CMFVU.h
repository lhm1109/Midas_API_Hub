#ifndef __DB_CMFVU_DB_H__
#define __DB_CMFVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CMFVU
{
public:
	CDB_CMFVU()
	{
		m_cmfv.InitHashTable(HASHSIZECMFV);
	}
	virtual ~CDB_CMFVU() {};

public:
	void Add(T_UDRD_KEY Key, T_CMFV_UDRD_D& rData)
	{
		m_cmfv.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_cmfv.RemoveKey(Key);
	}
	void DelAll()
	{
		m_cmfv.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_CMFV_UDRD_D& rData)
	{
		return m_cmfv.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_cmfv.GetCount();
	}
	POSITION GetStart()
	{
		return m_cmfv.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_CMFV_UDRD_D& rData)
	{
		m_cmfv.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_CMFV_UDRD_D, T_CMFV_UDRD_D&>m_cmfv;
};

#endif