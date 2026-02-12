#ifndef __DB_MPST_SSMU_DB_H__
#define __DB_MPST_SSMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MPST_SSMU
{
public:
	CDB_MPST_SSMU()
	{
		m_mpstSsm.InitHashTable(HASHSIZEMPSTSSM);
	}
	virtual ~CDB_MPST_SSMU() {};

public:
	void Add(T_UDRD_KEY Key, T_MPST_SSM_UDRD_D& rData)
	{
		m_mpstSsm.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_mpstSsm.RemoveKey(Key);
	}
	void DelAll()
	{
		m_mpstSsm.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_MPST_SSM_UDRD_D& rData)
	{
		return m_mpstSsm.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_MPST_SSM_UDRD_D, T_MPST_SSM_UDRD_D&> m_mpstSsm;
};

#endif