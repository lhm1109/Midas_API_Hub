#ifndef __DB_STCTU_DB_H__
#define __DB_STCTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STCTU
{
public:
	CDB_STCTU()
	{
		m_stct.InitHashTable(HASHSIZESTCT);
	}
	virtual ~CDB_STCTU(){};

public:
	void Add(T_UDRD_KEY Key,T_STCT_UDRD_D& rData)
		{m_stct.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stct.RemoveKey(Key);}
	void DelAll()
		{m_stct.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STCT_UDRD_D& rData)
		{return m_stct.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STCT_UDRD_D,T_STCT_UDRD_D&>m_stct;
};

#endif