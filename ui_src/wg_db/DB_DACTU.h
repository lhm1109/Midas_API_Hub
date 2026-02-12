#ifndef __DB_DACTU_DB_H__
#define __DB_DACTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DACTU
{
public:
	CDB_DACTU()
	{
		m_dact.InitHashTable(HASHSIZEDACT);
	}
	virtual ~CDB_DACTU() {};

public:
	void Add(T_UDRD_KEY Key, T_DACT_UDRD_D& rData)
	{
		m_dact.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_dact.RemoveKey(Key);
	}
	void DelAll()
	{
		m_dact.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_DACT_UDRD_D& rData)
	{
		return m_dact.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_DACT_UDRD_D, T_DACT_UDRD_D&>m_dact;
};

#endif
