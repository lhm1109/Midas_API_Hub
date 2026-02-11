#ifndef __DB_SISRU_DB_H__
#define __DB_SISRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SISRU
{
public:
	CDB_SISRU()
	{
		m_sisr.InitHashTable(HASHSIZESISR);
	}
	virtual ~CDB_SISRU() {};

public:
	void Add(T_UDRD_KEY Key, T_SISR_UDRD_D& rData)
	{
		m_sisr.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_sisr.RemoveKey(Key);
	}
	void DelAll()
	{
		m_sisr.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SISR_UDRD_D& rData)
	{
		return m_sisr.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_sisr.GetCount();
	}
	POSITION GetStart()
	{
		return m_sisr.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SISR_UDRD_D& rData)
	{
		m_sisr.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SISR_UDRD_D, T_SISR_UDRD_D&> m_sisr;
};

#endif