#ifndef __DB_SIGR_LOWERU_DB_H__
#define __DB_SIGR_LOWERU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIGR_LOWERU
{
public:
	CDB_SIGR_LOWERU()
	{
		m_sigrLower.InitHashTable(HASHSIZESIGR_LOWER);
	}
	virtual ~CDB_SIGR_LOWERU() {};

public:
	void Add(T_UDRD_KEY Key, T_SIGR_LOWER_UDRD_D& rData)
	{
		m_sigrLower.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_sigrLower.RemoveKey(Key);
	}
	void DelAll()
	{
		m_sigrLower.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SIGR_LOWER_UDRD_D& rData)
	{
		return m_sigrLower.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_sigrLower.GetCount();
	}
	POSITION GetStart()
	{
		return m_sigrLower.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SIGR_LOWER_UDRD_D& rData)
	{
		m_sigrLower.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SIGR_LOWER_UDRD_D, T_SIGR_LOWER_UDRD_D&> m_sigrLower;
};

#endif