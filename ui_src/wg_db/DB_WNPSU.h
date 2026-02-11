#ifndef __DB_WNPSU_DB_H__
#define __DB_WNPSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_WNPSU
{
public:
	CDB_WNPSU()
	{
		m_wnps.InitHashTable(HASHSIZEWNPS);
	}
	virtual ~CDB_WNPSU() {};

public:
	void Add(T_UDRD_KEY Key, T_WNPS_UDRD_D& rData)
	{
		m_wnps.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_wnps.RemoveKey(Key);
	}
	void DelAll()
	{
		m_wnps.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_WNPS_UDRD_D& rData)
	{
		return m_wnps.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_wnps.GetCount();
	}
	POSITION GetStart()
	{
		return m_wnps.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_WNPS_UDRD_D& rData)
	{
		m_wnps.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_WNPS_UDRD_D, T_WNPS_UDRD_D&>m_wnps;
};

#endif