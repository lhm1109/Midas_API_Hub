#ifndef __DB_SIGRU_DB_H__
#define __DB_SIGRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIGRU
{
public:
	CDB_SIGRU()
	{
		m_sigr.InitHashTable(HASHSIZESIGR);
	}
	virtual ~CDB_SIGRU() {};

public:
	void Add(T_UDRD_KEY Key, T_SIGR_UDRD_D& rData)
	{
		m_sigr.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_sigr.RemoveKey(Key);
	}
	void DelAll()
	{
		m_sigr.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SIGR_UDRD_D& rData)
	{
		return m_sigr.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_sigr.GetCount();
	}
	POSITION GetStart()
	{
		return m_sigr.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SIGR_UDRD_D& rData)
	{
		m_sigr.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SIGR_UDRD_D, T_SIGR_UDRD_D&> m_sigr;
};

#endif