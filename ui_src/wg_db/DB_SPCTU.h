#ifndef __DB_SPCTU_DB_H__
#define __DB_SPCTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SPCTU
{
public:
	CDB_SPCTU()
	{
		m_spct.InitHashTable(HASHSIZESPCT);
	}
	virtual ~CDB_SPCTU(){};

public:
	void Add(T_UDRD_KEY Key,T_SPCT_UDRD_D& rData)
		{m_spct.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_spct.RemoveKey(Key);}
	void DelAll()
		{m_spct.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SPCT_UDRD_D& rData)
		{return m_spct.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SPCT_UDRD_D,T_SPCT_UDRD_D&>m_spct;
};

#endif