#ifndef __DB_PRISU_DB_H__
#define __DB_PRISU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PRISU
{
public:
	CDB_PRISU()
	{
		m_pris.InitHashTable(HASHSIZEPRIS);
	}
	virtual ~CDB_PRISU() {};

public:
	void Add(T_UDRD_KEY Key, T_PRIS_UDRD_D& rData)
	{
		m_pris.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_pris.RemoveKey(Key);
	}
	void DelAll()
	{
		m_pris.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_PRIS_UDRD_D& rData)
	{
		return m_pris.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_pris.GetCount();
	}
	POSITION GetStart()
	{
		return m_pris.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_PRIS_UDRD_D& rData)
	{
		m_pris.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_PRIS_UDRD_D, T_PRIS_UDRD_D&> m_pris;
};

#endif