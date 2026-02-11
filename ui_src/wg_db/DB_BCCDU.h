#ifndef __DB_BCCDU_DB_H__
#define __DB_BCCDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BCCDU
{
public:
	CDB_BCCDU()
	{
		m_bccd.InitHashTable(HASHSIZEBCCD);
	}
	virtual ~CDB_BCCDU() {};

public:
	void Add(T_UDRD_KEY Key, T_BCCD_UDRD_D& rData)
	{
		m_bccd.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_bccd.RemoveKey(Key);
	}
	void DelAll()
	{
		m_bccd.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_BCCD_UDRD_D& rData)
	{
		return m_bccd.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_bccd.GetCount();
	}
	POSITION GetStart()
	{
		return m_bccd.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_BCCD_UDRD_D& rData)
	{
		m_bccd.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_BCCD_UDRD_D, T_BCCD_UDRD_D&>m_bccd;
};

#endif