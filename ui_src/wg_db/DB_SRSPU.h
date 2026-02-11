#ifndef __DB_SRSPU_DB_H__
#define __DB_SRSPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SRSPU
{
public:
	CDB_SRSPU()
	{
		m_srsp.InitHashTable(HASHSIZESRSP);
	}
	virtual ~CDB_SRSPU() {};

public:
	void Add(T_UDRD_KEY Key, T_SRSP_UDRD_D& rData)
	{
		m_srsp.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_srsp.RemoveKey(Key);
	}
	void DelAll()
	{
		m_srsp.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SRSP_UDRD_D& rData)
	{
		return m_srsp.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_srsp.GetCount();
	}
	POSITION GetStart()
	{
		return m_srsp.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SRSP_UDRD_D& rData)
	{
		m_srsp.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SRSP_UDRD_D, T_SRSP_UDRD_D&>m_srsp;
};

#endif