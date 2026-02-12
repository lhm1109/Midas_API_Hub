#ifndef __DB_EBSVU_DB_H__
#define __DB_EBSVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EBSVU
{
public:
	CDB_EBSVU()
	{
		m_ebsv.InitHashTable(HASHSIZEEBSV);
	}
	virtual ~CDB_EBSVU() {};

public:
	void Add(T_UDRD_KEY Key, T_EBSV_UDRD_D& rData)
	{
		m_ebsv.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_ebsv.RemoveKey(Key);
	}
	void DelAll()
	{
		m_ebsv.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_EBSV_UDRD_D& rData)
	{
		return m_ebsv.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_ebsv.GetCount();
	}
	POSITION GetStart()
	{
		return m_ebsv.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_EBSV_UDRD_D& rData)
	{
		m_ebsv.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_EBSV_UDRD_D, T_EBSV_UDRD_D&>m_ebsv;
};

#endif