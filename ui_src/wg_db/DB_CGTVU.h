#ifndef __DB_CGTVU_DB_H__
#define __DB_CGTVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGTVU
{
public:
	CDB_CGTVU()
	{
		m_cgtv.InitHashTable(HASHSIZECGTV);
	}
	virtual ~CDB_CGTVU() {};

public:
	void Add(T_UDRD_KEY Key, T_CGTV_UDRD_D& rData)
	{
		m_cgtv.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_cgtv.RemoveKey(Key);
	}
	void DelAll()
	{
		m_cgtv.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_CGTV_UDRD_D& rData)
	{
		return m_cgtv.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_cgtv.GetCount();
	}
	POSITION GetStart()
	{
		return m_cgtv.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_CGTV_UDRD_D& rData)
	{
		m_cgtv.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_CGTV_UDRD_D, T_CGTV_UDRD_D&>m_cgtv;
};

#endif