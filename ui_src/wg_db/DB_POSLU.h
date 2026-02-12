#ifndef __DB_POSLU_DB_H__
#define __DB_POSLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POSLU
{
public:
	CDB_POSLU()
	{
		m_posl.InitHashTable(HASHSIZEPOSL);
	}
	virtual ~CDB_POSLU() {};

public:
	void Add(T_UDRD_KEY Key, T_POSL_UDRD_D& rData)
	{
		m_posl.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_posl.RemoveKey(Key);
	}
	void DelAll()
	{
		m_posl.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_POSL_UDRD_D& rData)
	{
		return m_posl.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_posl.GetCount();
	}
	POSITION GetStart()
	{
		return m_posl.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_POSL_UDRD_D& rData)
	{
		m_posl.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_POSL_UDRD_D, T_POSL_UDRD_D&>m_posl;
};

#endif