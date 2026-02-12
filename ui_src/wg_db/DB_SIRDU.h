#ifndef __DB_SIRDU_DB_H__
#define __DB_SIRDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIRDU
{
public:
	CDB_SIRDU()
	{
		m_sird.InitHashTable(HASHSIZESIRD);
	}
	virtual ~CDB_SIRDU() {};

public:
	void Add(T_UDRD_KEY Key, T_SIRD_UDRD_D& rData)
	{
		m_sird.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_sird.RemoveKey(Key);
	}
	void DelAll()
	{
		m_sird.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SIRD_UDRD_D& rData)
	{
		return m_sird.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_sird.GetCount();
	}
	POSITION GetStart()
	{
		return m_sird.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SIRD_UDRD_D& rData)
	{
		m_sird.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SIRD_UDRD_D, T_SIRD_UDRD_D&> m_sird;
};

#endif