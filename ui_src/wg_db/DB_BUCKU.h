#ifndef __DB_BUCKU_DB_H__
#define __DB_BUCKU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BUCKU
{
public:
	CDB_BUCKU()
	{
		m_buck.InitHashTable(HASHSIZEBUCK);
	}
	virtual ~CDB_BUCKU(){};

public:
	void Add(T_UDRD_KEY Key,T_BUCK_UDRD_D& rData)
		{m_buck.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_buck.RemoveKey(Key);}
	void DelAll()
		{m_buck.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BUCK_UDRD_D& rData)
		{return m_buck.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BUCK_UDRD_D,T_BUCK_UDRD_D&>m_buck;
};

#endif