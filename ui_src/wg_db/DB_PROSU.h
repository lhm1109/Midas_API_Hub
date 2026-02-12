#ifndef __DB_PROSU_DB_H__
#define __DB_PROSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PROSU
{
public:
	CDB_PROSU()
	{
		m_pros.InitHashTable(HASHSIZEPROS);
	}
	virtual ~CDB_PROSU() {};

public:
	void Add(T_UDRD_KEY Key, T_PROS_UDRD_D& rData)
	{
		m_pros.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_pros.RemoveKey(Key);
	}
	void DelAll()
	{
		m_pros.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_PROS_UDRD_D& rData)
	{
		return m_pros.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_pros.GetCount();
	}
	POSITION GetStart()
	{
		return m_pros.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_PROS_UDRD_D& rData)
	{
		m_pros.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_PROS_UDRD_D, T_PROS_UDRD_D&> m_pros;
};

#endif