#ifndef __DB_FMAVU_DB_H__
#define __DB_FMAVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FMAVU
{
public:
	CDB_FMAVU()
	{
		m_fmav.InitHashTable(HASHSIZEFMAV);
	}
	virtual ~CDB_FMAVU() {};

public:
	void Add(T_UDRD_KEY Key, T_FMAV_UDRD_D& rData)
	{
		m_fmav.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_fmav.RemoveKey(Key);
	}
	void DelAll()
	{
		m_fmav.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_FMAV_UDRD_D& rData)
	{
		return m_fmav.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_fmav.GetCount();
	}
	POSITION GetStart()
	{
		return m_fmav.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_FMAV_UDRD_D& rData)
	{
		m_fmav.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_FMAV_UDRD_D, T_FMAV_UDRD_D&>m_fmav;
};

#endif