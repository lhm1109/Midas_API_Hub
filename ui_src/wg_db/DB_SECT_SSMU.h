#ifndef __DB_SECT_SSMU_DB_H__
#define __DB_SECT_SSMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECT_SSMU
{
public:
	CDB_SECT_SSMU()
	{
		m_rbar.InitHashTable(HASHSIZESECTSSM);
	}
	virtual ~CDB_SECT_SSMU() {};

public:
	void Add(T_UDRD_KEY Key, T_SECT_SSM_UDRD_D& rData)
	{
		m_rbar.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_rbar.RemoveKey(Key);
	}
	void DelAll()
	{
		m_rbar.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SECT_SSM_UDRD_D& rData)
	{
		return m_rbar.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_rbar.GetCount();
	}
	POSITION GetStart()
	{
		return m_rbar.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SECT_SSM_UDRD_D& rData)
	{
		m_rbar.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SECT_SSM_UDRD_D, T_SECT_SSM_UDRD_D&> m_rbar;
};

#endif