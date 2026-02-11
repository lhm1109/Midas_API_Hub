#ifndef __DB_EPSTU_DB_H__
#define __DB_EPSTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EPSTU
{
public:
	CDB_EPSTU()
	{
		m_epst.InitHashTable(HASHSIZEEPST);
	}
	virtual ~CDB_EPSTU() {};

public:
	void Add(T_UDRD_KEY Key, T_EPST_UDRD_D& rData)
	{
		m_epst.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_epst.RemoveKey(Key);
	}
	void DelAll()
	{
		m_epst.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_EPST_UDRD_D& rData)
	{
		return m_epst.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_epst.GetCount();
	}
	POSITION GetStart()
	{
		return m_epst.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_EPST_UDRD_D& rData)
	{
		m_epst.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_EPST_UDRD_D, T_EPST_UDRD_D&>m_epst;
};

#endif