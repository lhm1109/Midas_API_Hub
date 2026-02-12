#ifndef __DB_DSFCU_DB_H__
#define __DB_DSFCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DSFCU
{
public:
	CDB_DSFCU()
	{
		m_dsfc.InitHashTable(HASHSIZEDSFC);
	}
	virtual ~CDB_DSFCU() {};

public:
	void Add(T_UDRD_KEY Key, T_DSFC_UDRD_D& rData)
	{
		m_dsfc.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_dsfc.RemoveKey(Key);
	}
	void DelAll()
	{
		m_dsfc.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_DSFC_UDRD_D& rData)
	{
		return m_dsfc.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_DSFC_UDRD_D, T_DSFC_UDRD_D&> m_dsfc;
};

#endif