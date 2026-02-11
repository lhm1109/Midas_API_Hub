#ifndef __DB_LCRSU_DB_H__
#define __DB_LCRSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LCRSU
{
public:
	CDB_LCRSU()
	{
		m_lcrs.InitHashTable(HASHSIZELCRS);
	}
	virtual ~CDB_LCRSU(){};

public:
	void Add(T_UDRD_KEY Key,T_LCRS_UDRD_D& rData)
		{m_lcrs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_lcrs.RemoveKey(Key);}
	void DelAll()
		{m_lcrs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LCRS_UDRD_D& rData)
		{return m_lcrs.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LCRS_UDRD_D,T_LCRS_UDRD_D&>m_lcrs;
};

#endif