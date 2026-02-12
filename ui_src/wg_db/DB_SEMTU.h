#ifndef __DB_SEMTU_DB_H__
#define __DB_SEMTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SEMTU
{
public:
	CDB_SEMTU()
	{
		m_semt.InitHashTable(HASHSIZESEMT);
	}
	virtual ~CDB_SEMTU(){};

public:
	void Add(T_UDRD_KEY Key,T_SEMT_UDRD_D& rData)
		{m_semt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_semt.RemoveKey(Key);}
	void DelAll()
		{m_semt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SEMT_UDRD_D& rData)
		{return m_semt.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SEMT_UDRD_D,T_SEMT_UDRD_D&>m_semt;
};

#endif