#ifndef __DB_BRGTU_DB_H__
#define __DB_BRGTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BRGTU
{
public:
	CDB_BRGTU()
	{
		m_brgt.InitHashTable(HASHSIZEBRGT);
	}
	virtual ~CDB_BRGTU(){};

public:
	void Add(T_UDRD_KEY Key,T_BRGT_UDRD_D& rData)
		{m_brgt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_brgt.RemoveKey(Key);}
	void DelAll()
		{m_brgt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BRGT_UDRD_D& rData)
		{return m_brgt.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BRGT_UDRD_D,T_BRGT_UDRD_D&>m_brgt;
};

#endif