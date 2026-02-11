#ifndef __DB_CSPAU_DB_H__
#define __DB_CSPAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CSPAU
{
public:
	CDB_CSPAU()
	{
		m_cspa.InitHashTable(HASHSIZECSPA);
	}
	virtual ~CDB_CSPAU() {};

public:
	void Add(T_UDRD_KEY Key, T_CSPA_UDRD_D& rData)
	{
		m_cspa.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_cspa.RemoveKey(Key);
	}
	void DelAll()
	{
		m_cspa.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_CSPA_UDRD_D& rData)
	{
		return m_cspa.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_cspa.GetCount();
	}
	POSITION GetStart()
	{
		return m_cspa.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_CSPA_UDRD_D& rData)
	{
		m_cspa.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_CSPA_UDRD_D, T_CSPA_UDRD_D&>m_cspa;
};

#endif