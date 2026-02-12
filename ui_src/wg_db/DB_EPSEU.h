#ifndef __DB_EPSEU_DB_H__
#define __DB_EPSEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EPSEU
{
public:
	CDB_EPSEU()
	{
		m_epse.InitHashTable(HASHSIZEEPSE);
	}
	virtual ~CDB_EPSEU() {};

public:
	void Add(T_UDRD_KEY Key, T_EPSE_UDRD_D& rData)
	{
		m_epse.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_epse.RemoveKey(Key);
	}
	void DelAll()
	{
		m_epse.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_EPSE_UDRD_D& rData)
	{
		return m_epse.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_epse.GetCount();
	}
	POSITION GetStart()
	{
		return m_epse.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_EPSE_UDRD_D& rData)
	{
		m_epse.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_EPSE_UDRD_D, T_EPSE_UDRD_D&>m_epse;
};

#endif