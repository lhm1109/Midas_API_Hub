#ifndef __DB_CPTVU_DB_H__
#define __DB_CPTVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CPTVU
{
public:
	CDB_CPTVU()
	{
		m_cptv.InitHashTable(HASHSIZECPTV);
	}
	virtual ~CDB_CPTVU() {};

public:
	void Add(T_UDRD_KEY Key, T_CPTV_UDRD_D& rData)
	{
		m_cptv.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_cptv.RemoveKey(Key);
	}
	void DelAll()
	{
		m_cptv.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_CPTV_UDRD_D& rData)
	{
		return m_cptv.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_cptv.GetCount();
	}
	POSITION GetStart()
	{
		return m_cptv.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_CPTV_UDRD_D& rData)
	{
		m_cptv.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_CPTV_UDRD_D, T_CPTV_UDRD_D&>m_cptv;
};

#endif