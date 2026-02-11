#ifndef __DB_PANAU_DB_H__
#define __DB_PANAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PANAU
{
public:
	CDB_PANAU()
	{
		m_pana.InitHashTable(HASHSIZEPANA);
	}
	virtual ~CDB_PANAU() {};

public:
	void Add(T_UDRD_KEY Key, T_PANA_UDRD_D& rData)
	{
		m_pana.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_pana.RemoveKey(Key);
	}
	void DelAll()
	{
		m_pana.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_PANA_UDRD_D& rData)
	{
		return m_pana.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_pana.GetCount();
	}
	POSITION GetStart()
	{
		return m_pana.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_PANA_UDRD_D& rData)
	{
		m_pana.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_PANA_UDRD_D, T_PANA_UDRD_D&>m_pana;
};

#endif