#ifndef __DB_RPSSU_DB_H__
#define __DB_RPSSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RPSSU
{
public:
	CDB_RPSSU()
	{
		m_rpss.InitHashTable(HASHSIZERPSS);
	}
	virtual ~CDB_RPSSU() {};

public:
	void Add(T_UDRD_KEY Key, T_RPSS_UDRD_D& rData)
	{
		m_rpss.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_rpss.RemoveKey(Key);
	}
	void DelAll()
	{
		m_rpss.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_RPSS_UDRD_D& rData)
	{
		return m_rpss.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_RPSS_UDRD_D, T_RPSS_UDRD_D&>m_rpss;
};

#endif