#ifndef __DB_IFCTU_DB_H__
#define __DB_IFCTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_IFCTU
{
public:
	CDB_IFCTU()
	{
		m_ifct.InitHashTable(HASHSIZEIFCT);
	}
	virtual ~CDB_IFCTU(){};

public:
	void Add(T_UDRD_KEY Key,T_IFCT_UDRD_D& rData)
		{m_ifct.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ifct.RemoveKey(Key);}
	void DelAll()
		{m_ifct.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_IFCT_UDRD_D& rData)
		{return m_ifct.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_IFCT_UDRD_D,T_IFCT_UDRD_D&>m_ifct;
};

#endif