#ifndef __DB_ESQWU_DB_H__
#define __DB_ESQWU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ESQWU
{
public:
	CDB_ESQWU()
	{
		m_esqw.InitHashTable(HASHSIZEESQW);
	}
	virtual ~CDB_ESQWU(){};

public:
	void Add(T_UDRD_KEY Key,T_ESQW_UDRD_D& rData)
		{m_esqw.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_esqw.RemoveKey(Key);}
	void DelAll()
		{m_esqw.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ESQW_UDRD_D& rData)
		{return m_esqw.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_ESQW_UDRD_D,T_ESQW_UDRD_D&>m_esqw;
};

#endif