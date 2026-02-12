#ifndef __DB_RAKRU_DB_H__
#define __DB_RAKRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RAKRU
{
public:
	CDB_RAKRU()
	{
		m_rakr.InitHashTable(HASHSIZERAKR);
	}
	virtual ~CDB_RAKRU(){};

public:
	void Add(T_UDRD_KEY Key,T_RAKR_UDRD_D& rData)
		{m_rakr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rakr.RemoveKey(Key);}
	void DelAll()
		{m_rakr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RAKR_UDRD_D& rData)
		{return m_rakr.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RAKR_UDRD_D,T_RAKR_UDRD_D&>m_rakr;
};

#endif