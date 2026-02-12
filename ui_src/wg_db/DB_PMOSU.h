#ifndef __DB_PMOSU_DB_H__
#define __DB_PMOSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PMOSU
{
public:
	CDB_PMOSU()
	{
		m_pmos.InitHashTable(HASHSIZEPMOS);
	}
	virtual ~CDB_PMOSU() {};

public:
	void Add(T_UDRD_KEY Key, T_PMOS_UDRD_D& rData)
	{
		m_pmos.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_pmos.RemoveKey(Key);
	}
	void DelAll()
	{
		m_pmos.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_PMOS_UDRD_D& rData)
	{
		return m_pmos.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_pmos.GetCount();
	}
	POSITION GetStart()
	{
		return m_pmos.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_PMOS_UDRD_D& rData)
	{
		m_pmos.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_PMOS_UDRD_D, T_PMOS_UDRD_D&> m_pmos;
};

#endif