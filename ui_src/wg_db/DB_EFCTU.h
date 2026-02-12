#ifndef __DB_EFCTU_DB_H__
#define __DB_EFCTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EFCTU
{
public:
	CDB_EFCTU()
	{
		m_efct.InitHashTable(HASHSIZEEFCT);
	}
	virtual ~CDB_EFCTU(){};

public:
	void Add(T_UDRD_KEY Key,T_EFCT_UDRD_D& rData)
		{m_efct.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_efct.RemoveKey(Key);}
	void DelAll()
		{m_efct.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_EFCT_UDRD_D& rData)
		{return m_efct.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_EFCT_UDRD_D,T_EFCT_UDRD_D&>m_efct;
};

#endif