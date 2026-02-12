#ifndef __DB_POSVU_DB_H__
#define __DB_POSVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POSVU
{
public:
	CDB_POSVU()
	{
		m_posv.InitHashTable(HASHSIZEPOSV);
	}
	virtual ~CDB_POSVU() {};

public:
	void Add(T_UDRD_KEY Key, T_POSV_UDRD_D& rData)
	{
		m_posv.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_posv.RemoveKey(Key);
	}
	void DelAll()
	{
		m_posv.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_POSV_UDRD_D& rData)
	{
		return m_posv.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_posv.GetCount();
	}
	POSITION GetStart()
	{
		return m_posv.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_POSV_UDRD_D& rData)
	{
		m_posv.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_POSV_UDRD_D, T_POSV_UDRD_D&>m_posv;
};

#endif