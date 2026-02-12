#ifndef __DB_SBRIU_DB_H__
#define __DB_SBRIU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SBRIU
{
public:
	CDB_SBRIU()
	{
		m_sbri.InitHashTable(HASHSIZESBRI);
	}
	virtual ~CDB_SBRIU(){};

public:
	void Add(T_UDRD_KEY Key,T_SBRI_UDRD_D& rData)
		{m_sbri.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sbri.RemoveKey(Key);}
	void DelAll()
		{m_sbri.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SBRI_UDRD_D& rData)
		{return m_sbri.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SBRI_UDRD_D,T_SBRI_UDRD_D&>m_sbri;
};

#endif