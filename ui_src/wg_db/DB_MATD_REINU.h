#ifndef __DB_MATD_REINU_DB_H__
#define __DB_MATD_REINU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MATD_REINU
{
public:
	CDB_MATD_REINU()
	{
		m_udrdMap.InitHashTable(HASHSIZEMATDREIN);
	}
	virtual ~CDB_MATD_REINU() {};

public:
	void Add(T_UDRD_KEY Key, T_MATD_REIN_UDRD_D& rData)
	{
		m_udrdMap.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_udrdMap.RemoveKey(Key);
	}
	void DelAll()
	{
		m_udrdMap.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_MATD_REIN_UDRD_D& rData)
	{
		return m_udrdMap.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_udrdMap.GetCount();
	}
	POSITION GetStart()
	{
		return m_udrdMap.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_MATD_REIN_UDRD_D& rData)
	{
		m_udrdMap.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_MATD_REIN_UDRD_D, T_MATD_REIN_UDRD_D&> m_udrdMap;
};

#endif