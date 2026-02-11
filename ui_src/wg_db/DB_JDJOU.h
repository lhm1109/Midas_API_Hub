#ifndef __DB_JDJOU_DB_H__
#define __DB_JDJOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDJOU
{
public:
	CDB_JDJOU()
	{
		m_JDJO.InitHashTable(HASHSIZEJDJO);
	}
	virtual ~CDB_JDJOU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDJO_UDRD_D& rData)
		{m_JDJO.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDJO.RemoveKey(Key);}
	void DelAll()
		{m_JDJO.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDJO_UDRD_D& rData)
		{return m_JDJO.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDJO_UDRD_D,T_JDJO_UDRD_D&> m_JDJO;
};

#endif