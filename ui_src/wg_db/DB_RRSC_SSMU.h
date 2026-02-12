#ifndef __DB_RRSC_SSMU_DB_H__
#define __DB_RRSC_SSMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RRSC_SSMU
{
public:
	CDB_RRSC_SSMU()
	{
		m_rrscSsm.InitHashTable(HASHSIZERRSCSSM);
	}
	virtual ~CDB_RRSC_SSMU() {};

public:
	void Add(T_UDRD_KEY Key, T_RRSC_SSM_UDRD_D& rData)
	{
		m_rrscSsm.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_rrscSsm.RemoveKey(Key);
	}
	void DelAll()
	{
		m_rrscSsm.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_RRSC_SSM_UDRD_D& rData)
	{
		return m_rrscSsm.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_rrscSsm.GetCount();
	}
	POSITION GetStart()
	{
		return m_rrscSsm.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_RRSC_SSM_UDRD_D& rData)
	{
		m_rrscSsm.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_RRSC_SSM_UDRD_D, T_RRSC_SSM_UDRD_D&> m_rrscSsm;
};

#endif