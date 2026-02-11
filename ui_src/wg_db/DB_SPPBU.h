//sppb
#ifndef __DB_SPPBU_DB_H__
#define __DB_SPPBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_SPPBU
{
public:
	CDB_SPPBU()
	{
		m_SPPB.InitHashTable(HASHSIZESPPB);
	}
	virtual ~CDB_SPPBU() {};

public:
	void Add(T_UDRD_KEY Key, T_SPPB_UDRD_D& rData)
	{
		m_SPPB.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_SPPB.RemoveKey(Key);
	}
	void DelAll()
	{
		m_SPPB.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SPPB_UDRD_D& rData)
	{
		return m_SPPB.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_SPPB.GetCount();
	}
	POSITION GetStart()
	{
		return m_SPPB.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SPPB_UDRD_D& rData)
	{
		m_SPPB.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SPPB_UDRD_D, T_SPPB_UDRD_D&>m_SPPB;
};

#endif
