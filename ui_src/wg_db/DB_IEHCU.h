#ifndef __DB_IEHCU_DB_H__
#define __DB_IEHCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_IEHCU
{
public:
	CDB_IEHCU()
	{
		m_iehc.InitHashTable(HASHSIZEIEHC);
	}
	virtual ~CDB_IEHCU(){};

public:
	void Add(T_UDRD_KEY Key,T_IEHC_UDRD_D& rData)
		{m_iehc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_iehc.RemoveKey(Key);}
	void DelAll()
		{m_iehc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_IEHC_UDRD_D& rData)
		{return m_iehc.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_IEHC_UDRD_D,T_IEHC_UDRD_D&>m_iehc;
};

#endif