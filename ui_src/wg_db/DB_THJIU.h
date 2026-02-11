#ifndef __DB_THJIU_DB_H__
#define __DB_THJIU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THJIU
{
public:
	CDB_THJIU()
	{
		m_thji.InitHashTable(HASHSIZETHJI);
	}
	virtual ~CDB_THJIU() {};

public:
	void Add(T_UDRD_KEY Key, T_THJI_UDRD_D& rData)
	{
		m_thji.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_thji.RemoveKey(Key);
	}
	void DelAll()
	{
		m_thji.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_THJI_UDRD_D& rData)
	{
		return m_thji.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_thji.GetCount();
	}
	POSITION GetStart()
	{
		return m_thji.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_THJI_UDRD_D& rData)
	{
		m_thji.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_THJI_UDRD_D, T_THJI_UDRD_D&>m_thji;
};

#endif