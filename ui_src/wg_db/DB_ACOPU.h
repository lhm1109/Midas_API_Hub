#ifndef __DB_ACOPU_DB_H__
#define __DB_ACOPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ACOPU
{
public:
	CDB_ACOPU()
	{
		m_acop.InitHashTable(HASHSIZEACOP);
	}
	virtual ~CDB_ACOPU(){};

public:
	void Add(T_UDRD_KEY Key,T_ACOP_UDRD_D& rData)
		{m_acop.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_acop.RemoveKey(Key);}
	void DelAll()
		{m_acop.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ACOP_UDRD_D& rData)
		{return m_acop.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ACOP_UDRD_D,T_ACOP_UDRD_D&>m_acop;
};

#endif