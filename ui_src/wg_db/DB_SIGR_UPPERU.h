#ifndef __DB_SIGR_UPPERU_DB_H__
#define __DB_SIGR_UPPERU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIGR_UPPERU
{
public:
	CDB_SIGR_UPPERU()
	{
		m_sigrUpper.InitHashTable(HASHSIZESIGR_UPPER);
	}
	virtual ~CDB_SIGR_UPPERU() {};

public:
	void Add(T_UDRD_KEY Key, T_SIGR_UPPER_UDRD_D& rData)
	{
		m_sigrUpper.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_sigrUpper.RemoveKey(Key);
	}
	void DelAll()
	{
		m_sigrUpper.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SIGR_UPPER_UDRD_D& rData)
	{
		return m_sigrUpper.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_sigrUpper.GetCount();
	}
	POSITION GetStart()
	{
		return m_sigrUpper.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SIGR_UPPER_UDRD_D& rData)
	{
		m_sigrUpper.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SIGR_UPPER_UDRD_D, T_SIGR_UPPER_UDRD_D&> m_sigrUpper;
};

#endif