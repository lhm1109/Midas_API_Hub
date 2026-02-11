
#ifndef __DB_ANOPU_DB_H__
#define __DB_ANOPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ANOPU
{
public:
	CDB_ANOPU()
	{
		m_anop.InitHashTable(HASHSIZEANOP);
	}
	virtual ~CDB_ANOPU(){}

public:
	void Add(T_UDRD_KEY Key,T_ANOP_UDRD_D& rData)
		{m_anop.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_anop.RemoveKey(Key);}
	void DelAll()
		{m_anop.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ANOP_UDRD_D& rData)
		{return m_anop.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ANOP_UDRD_D,T_ANOP_UDRD_D&>m_anop;
};

#endif
