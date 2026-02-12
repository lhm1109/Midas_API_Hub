//spcs
#ifndef __DB_SPCSU_DB_H__
#define __DB_SPCSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_SPCSU
{
public:
	CDB_SPCSU()
	{
		m_SPCS.InitHashTable(HASHSIZESPCS);
	}
	virtual ~CDB_SPCSU() {};

public:
	void Add(T_UDRD_KEY Key, T_SPCS_UDRD_D& rData)
	{
		m_SPCS.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_SPCS.RemoveKey(Key);
	}
	void DelAll()
	{
		m_SPCS.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SPCS_UDRD_D& rData)
	{
		return m_SPCS.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_SPCS.GetCount();
	}
	POSITION GetStart()
	{
		return m_SPCS.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SPCS_UDRD_D& rData)
	{
		m_SPCS.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SPCS_UDRD_D, T_SPCS_UDRD_D&>m_SPCS;
};

#endif