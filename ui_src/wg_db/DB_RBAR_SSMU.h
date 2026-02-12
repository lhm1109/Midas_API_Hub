#ifndef __DB_RBAR_SSMU_DB_H__
#define __DB_RBAR_SSMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RBAR_SSMU
{
public:
	CDB_RBAR_SSMU()
	{
		m_rbarSsm.InitHashTable(HASHSIZERBARSSM);
	}
	virtual ~CDB_RBAR_SSMU() {};

public:
	void Add(T_UDRD_KEY Key, T_RBAR_SSM_UDRD_D& rData)
	{
		m_rbarSsm.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_rbarSsm.RemoveKey(Key);
	}
	void DelAll()
	{
		m_rbarSsm.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_RBAR_SSM_UDRD_D& rData)
	{
		return m_rbarSsm.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_rbarSsm.GetCount();
	}
	POSITION GetStart()
	{
		return m_rbarSsm.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_RBAR_SSM_UDRD_D& rData)
	{
		m_rbarSsm.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_RBAR_SSM_UDRD_D, T_RBAR_SSM_UDRD_D&> m_rbarSsm;
};

#endif