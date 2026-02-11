#ifndef __DB_JDCRU_DB_H__
#define __DB_JDCRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDCRU
{
public:
	CDB_JDCRU()
	{
		m_JDCR.InitHashTable(HASHSIZEJDCR);
	}
	virtual ~CDB_JDCRU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDCR_UDRD_D& rData)
		{m_JDCR.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDCR.RemoveKey(Key);}
	void DelAll()
		{m_JDCR.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDCR_UDRD_D& rData)
		{return m_JDCR.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDCR_UDRD_D,T_JDCR_UDRD_D&> m_JDCR;
};

#endif