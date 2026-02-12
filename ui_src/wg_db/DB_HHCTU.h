#ifndef __DB_HHCTU_DB_H__
#define __DB_HHCTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HHCTU
{
public:
	CDB_HHCTU()
	{
		m_hhct.InitHashTable(HASHSIZEHHCT);
	}
	virtual ~CDB_HHCTU(){};

public:
	void Add(T_UDRD_KEY Key,T_HHCT_UDRD_D& rData)
		{m_hhct.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hhct.RemoveKey(Key);}
	void DelAll()
		{m_hhct.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HHCT_UDRD_D& rData)
		{return m_hhct.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_HHCT_UDRD_D,T_HHCT_UDRD_D&>m_hhct;
};

#endif