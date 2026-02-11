#ifndef __DB_SISR_USERU_DB_H__
#define __DB_SISR_USERU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SISR_USERU
{
public:
	CDB_SISR_USERU()
	{
		m_sisrUser.InitHashTable(HASHSIZESISR_USER);
	}
	virtual ~CDB_SISR_USERU() {};

public:
	void Add(T_UDRD_KEY Key, T_SISR_USER_UDRD_D& rData)
	{
		m_sisrUser.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_sisrUser.RemoveKey(Key);
	}
	void DelAll()
	{
		m_sisrUser.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SISR_USER_UDRD_D& rData)
	{
		return m_sisrUser.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_sisrUser.GetCount();
	}
	POSITION GetStart()
	{
		return m_sisrUser.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SISR_USER_UDRD_D& rData)
	{
		m_sisrUser.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SISR_USER_UDRD_D, T_SISR_USER_UDRD_D&> m_sisrUser;
};

#endif