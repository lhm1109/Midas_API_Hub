#ifndef __DB_STBKU_DB_H__
#define __DB_STBKU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STBKU
{
public:
	CDB_STBKU()
	{
		m_stbk.InitHashTable(HASHSIZESTBK);
	}
	virtual ~CDB_STBKU(){};

public:
	void Add(T_UDRD_KEY Key,T_STBK_UDRD_D& rData)
	{
		m_stbk.SetAt(Key,rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_stbk.RemoveKey(Key);
	}
	void DelAll()
	{
		m_stbk.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key,T_STBK_UDRD_D& rData)
	{
		return m_stbk.Lookup(Key,rData);
	}
	int GetCount()
	{
		return m_stbk.GetCount();
	}
	POSITION GetStart()
	{
		return m_stbk.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STBK_UDRD_D& rData)
	{
		m_stbk.GetNextAssoc(rNextPosition,rKey,rData);
	}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_STBK_UDRD_D,T_STBK_UDRD_D&> m_stbk;
};

#endif