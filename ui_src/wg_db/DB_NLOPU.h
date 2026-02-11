#ifndef __DB_NLOPU_DB_H__
#define __DB_NLOPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NLOPU
{
public:
	CDB_NLOPU()
	{
		m_nlop.InitHashTable(HASHSIZENLOP);
	}
	virtual ~CDB_NLOPU(){};

public:
	void Add(T_UDRD_KEY Key,T_NLOP_UDRD_D& rData)
		{m_nlop.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_nlop.RemoveKey(Key);}
	void DelAll()
		{m_nlop.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_NLOP_UDRD_D& rData)
		{return m_nlop.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_NLOP_UDRD_D,T_NLOP_UDRD_D&>m_nlop;
};

#endif