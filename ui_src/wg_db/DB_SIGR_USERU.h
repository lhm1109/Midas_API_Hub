#ifndef __DB_SIGR_USERU_DB_H__
#define __DB_SIGR_USERU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIGR_USERU
{
public:
	CDB_SIGR_USERU()
	{
		m_sigrUser.InitHashTable(HASHSIZESIGR_USER);
	}
	virtual ~CDB_SIGR_USERU() {};

public:
	void Add(T_UDRD_KEY Key, T_SIGR_USER_UDRD_D& rData)
	{
		m_sigrUser.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_sigrUser.RemoveKey(Key);
	}
	void DelAll()
	{
		m_sigrUser.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_SIGR_USER_UDRD_D& rData)
	{
		return m_sigrUser.Lookup(Key, rData);
	}
	int GetCount()
	{
		return m_sigrUser.GetCount();
	}
	POSITION GetStart()
	{
		return m_sigrUser.GetStartPosition();
	}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_SIGR_USER_UDRD_D& rData)
	{
		m_sigrUser.GetNextAssoc(rNextPosition, rKey, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_SIGR_USER_UDRD_D, T_SIGR_USER_UDRD_D&> m_sigrUser;
};

#endif