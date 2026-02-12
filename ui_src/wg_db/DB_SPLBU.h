//splb
#ifndef __DB_SPLBU_DB_H__
#define __DB_SPLBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_SPLBU
{
public:
	CDB_SPLBU()
	{
		m_SPLB.InitHashTable(HASHSIZESPLB);
	}
	virtual ~CDB_SPLBU() {};

public:
	void Add(T_UDRD_KEY Key, T_SPLB_UDRD_D& rData)
	{
		m_SPLB.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_SPLB.RemoveKey(Key);
	}
	void DelAll()
	{
		m_SPLB.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SPLB_UDRD_D& rData)
	{
		return m_SPLB.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_SPLB.GetCount();
	}
	POSITION GetStart()
	{
		return m_SPLB.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SPLB_UDRD_D& rData)
	{
		m_SPLB.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SPLB_UDRD_D, T_SPLB_UDRD_D&>m_SPLB;
};

#endif