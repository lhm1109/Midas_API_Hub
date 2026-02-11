#ifndef __DB_POVCU_DB_H__
#define __DB_POVCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POVCU
{
public:
	CDB_POVCU()
	{
		m_povc.InitHashTable(HASHSIZEPOVC);
	}
	virtual ~CDB_POVCU() {};

public:
	void Add(T_UDRD_KEY Key, T_POVC_UDRD_D& rData)
	{
		m_povc.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_povc.RemoveKey(Key);
	}
	void DelAll()
	{
		m_povc.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_POVC_UDRD_D& rData)
	{
		return m_povc.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_povc.GetCount();
	}
	POSITION GetStart()
	{
		return m_povc.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_POVC_UDRD_D& rData)
	{
		m_povc.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_POVC_UDRD_D, T_POVC_UDRD_D&>m_povc;
};

#endif