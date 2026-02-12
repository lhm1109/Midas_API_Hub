#ifndef __DB_RCPGU_DB_H__
#define __DB_RCPGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RCPGU
{
public:
	CDB_RCPGU()
	{
		m_rcpg.InitHashTable(HASHSIZERCPG);
	}
	virtual ~CDB_RCPGU(){};

public:
	void Add(T_UDRD_KEY Key,T_RCPG_UDRD_D& rData)
		{m_rcpg.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rcpg.RemoveKey(Key);}
	void DelAll()
		{m_rcpg.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RCPG_UDRD_D& rData)
		{return m_rcpg.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RCPG_UDRD_D,T_RCPG_UDRD_D&>m_rcpg;
};

#endif