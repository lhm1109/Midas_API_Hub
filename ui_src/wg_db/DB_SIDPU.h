#ifndef __DB_SIDPU_DB_H__
#define __DB_SIDPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIDPU
{
public:
	CDB_SIDPU()
	{
		m_sidp.InitHashTable(HASHSIZESIDP);
	}
	virtual ~CDB_SIDPU() {};

public:
	void Add(T_UDRD_KEY Key, T_SIDP_UDRD_D& rData)
	{
		m_sidp.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_sidp.RemoveKey(Key);
	}
	void DelAll()
	{
		m_sidp.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SIDP_UDRD_D& rData)
	{
		return m_sidp.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_sidp.GetCount();
	}
	POSITION GetStart()
	{
		return m_sidp.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SIDP_UDRD_D& rData)
	{
		m_sidp.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SIDP_UDRD_D, T_SIDP_UDRD_D&> m_sidp;
};

#endif