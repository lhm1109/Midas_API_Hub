#ifndef __DB_PDCMU_DB_H__
#define __DB_PDCMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PDCMU
{
public:
	CDB_PDCMU()
	{
		m_PDCM.InitHashTable(HASHSIZEPDCM);
	}
	virtual ~CDB_PDCMU() {};

public:
	void Add(T_UDRD_KEY Key, T_PDCM_UDRD_D& rData)
	{
		m_PDCM.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_PDCM.RemoveKey(Key);
	}
	void DelAll()
	{
		m_PDCM.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_PDCM_UDRD_D& rData)
	{
		return m_PDCM.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_PDCM.GetCount();
	}
	POSITION GetStart()
	{
		return m_PDCM.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_PDCM_UDRD_D& rData)
	{
		m_PDCM.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_PDCM_UDRD_D, T_PDCM_UDRD_D&>m_PDCM;
};

#endif
