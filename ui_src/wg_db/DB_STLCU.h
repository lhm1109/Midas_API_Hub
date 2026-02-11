#ifndef __DB_STLCU_DB_H__
#define __DB_STLCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STLCU
{
public:
	CDB_STLCU()
	{
		m_stlc.InitHashTable(HASHSIZESTLC);
	}
	virtual ~CDB_STLCU(){};

public:
	void Add(T_UDRD_KEY Key,T_STLC_UDRD_D& rData)
		{m_stlc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stlc.RemoveKey(Key);}
	void DelAll()
		{m_stlc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STLC_UDRD_D& rData)
		{return m_stlc.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STLC_UDRD_D,T_STLC_UDRD_D&>m_stlc;
};

#endif