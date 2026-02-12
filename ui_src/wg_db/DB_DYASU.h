#ifndef __DB_DYASU_DB_H__
#define __DB_DYASU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DYASU
{
public:
	CDB_DYASU()
	{
		m_dyas.InitHashTable(HASHSIZEDYAS);
	}
	virtual ~CDB_DYASU(){};

public:
	void Add(T_UDRD_KEY Key,T_DYAS_UDRD_D& rData)
		{m_dyas.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dyas.RemoveKey(Key);}
	void DelAll()
		{m_dyas.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DYAS_UDRD_D& rData)
		{return m_dyas.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_DYAS_UDRD_D,T_DYAS_UDRD_D&>m_dyas;
};

#endif