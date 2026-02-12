#ifndef __DB_JDHOU_DB_H__
#define __DB_JDHOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DB_ST_DN_JUD.h"

class CDB_JDHOU
{
public:
	CDB_JDHOU()
	{
		m_JDHO.InitHashTable(HASHSIZEJDHO);
	}
	virtual ~CDB_JDHOU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDHO_UDRD_D& rData)
		{m_JDHO.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDHO.RemoveKey(Key);}
	void DelAll()
		{m_JDHO.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDHO_UDRD_D& rData)
		{return m_JDHO.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDHO_UDRD_D,T_JDHO_UDRD_D&> m_JDHO;
};

#endif