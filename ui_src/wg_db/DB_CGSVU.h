#ifndef __DB_CGSVU_DB_H__
#define __DB_CGSVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGSVU
{
public:
	CDB_CGSVU()
	{
		m_cgsv.InitHashTable(HASHSIZECGSV);
	}
	virtual ~CDB_CGSVU() {};

public:
	void Add(T_UDRD_KEY Key, T_CGSV_UDRD_D& rData)
	{
		m_cgsv.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_cgsv.RemoveKey(Key);
	}
	void DelAll()
	{
		m_cgsv.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_CGSV_UDRD_D& rData)
	{
		return m_cgsv.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_cgsv.GetCount();
	}
	POSITION GetStart()
	{
		return m_cgsv.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_CGSV_UDRD_D& rData)
	{
		m_cgsv.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_CGSV_UDRD_D, T_CGSV_UDRD_D&>m_cgsv;
};

#endif