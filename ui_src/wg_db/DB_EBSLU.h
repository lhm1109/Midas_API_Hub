#ifndef __DB_EBSLU_DB_H__
#define __DB_EBSLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EBSLU
{
public:
	CDB_EBSLU()
	{
		m_ebsl.InitHashTable(HASHSIZEEBSL);
	}
	virtual ~CDB_EBSLU() {};

public:
	void Add(T_UDRD_KEY Key, T_EBSL_UDRD_D& rData)
	{
		m_ebsl.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_ebsl.RemoveKey(Key);
	}
	void DelAll()
	{
		m_ebsl.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_EBSL_UDRD_D& rData)
	{
		return m_ebsl.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_ebsl.GetCount();
	}
	POSITION GetStart()
	{
		return m_ebsl.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_EBSL_UDRD_D& rData)
	{
		m_ebsl.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_EBSL_UDRD_D, T_EBSL_UDRD_D&>m_ebsl;
};

#endif