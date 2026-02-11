#ifndef __DB_EXLDU_DB_H__
#define __DB_EXLDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EXLDU
{
public:
	CDB_EXLDU()
	{
		m_exld.InitHashTable(HASHSIZEEXLD);
	}
	virtual ~CDB_EXLDU(){};

public:
	void Add(T_UDRD_KEY Key,T_EXLD_UDRD_D& rData)
		{m_exld.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_exld.RemoveKey(Key);}
	void DelAll()
		{m_exld.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_EXLD_UDRD_D& rData)
		{return m_exld.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_EXLD_UDRD_D,T_EXLD_UDRD_D&>m_exld;
};

#endif