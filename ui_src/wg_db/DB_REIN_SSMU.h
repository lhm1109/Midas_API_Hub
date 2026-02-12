#ifndef __DB_REIN_SSMU_DB_H__
#define __DB_REIN_SSMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_REIN_SSMU
{
public:
	CDB_REIN_SSMU()
	{
		m_reinSsm.InitHashTable(HASHSIZEREINSSM);
	}
	virtual ~CDB_REIN_SSMU() {};

public:
	void Add(T_UDRD_KEY Key, T_REIN_SSM_UDRD_D& rData)
	{
		m_reinSsm.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_reinSsm.RemoveKey(Key);
	}
	void DelAll()
	{
		m_reinSsm.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_REIN_SSM_UDRD_D& rData)
	{
		return m_reinSsm.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_reinSsm.GetCount();
	}
	POSITION GetStart()
	{
		return m_reinSsm.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_REIN_SSM_UDRD_D& rData)
	{
		m_reinSsm.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_REIN_SSM_UDRD_D, T_REIN_SSM_UDRD_D&> m_reinSsm;
};

#endif