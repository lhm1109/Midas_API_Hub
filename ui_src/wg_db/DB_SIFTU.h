#ifndef __DB_SIFTU_DB_H__
#define __DB_SIFTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIFTU
{
public:
	CDB_SIFTU()
	{
		m_sift.InitHashTable(HASHSIZESIFT);
	}
	virtual ~CDB_SIFTU() {};

public:
	void Add(T_UDRD_KEY Key, T_SIFT_UDRD_D& rData)
	{
		m_sift.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_sift.RemoveKey(Key);
	}
	void DelAll()
	{
		m_sift.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SIFT_UDRD_D& rData)
	{
		return m_sift.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SIFT_UDRD_D, T_SIFT_UDRD_D&>m_sift;
};

#endif