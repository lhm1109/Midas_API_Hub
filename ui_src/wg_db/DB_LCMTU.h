#ifndef __DB_LCMTU_DB_H__
#define __DB_LCMTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LCMTU
{
public:
	CDB_LCMTU()
	{
		m_lcmt.InitHashTable(HASHSIZELCMT);
	}
	virtual ~CDB_LCMTU(){};

public:
	void Add(T_UDRD_KEY Key,T_LCMT_UDRD_D& rData)
		{m_lcmt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_lcmt.RemoveKey(Key);}
	void DelAll()
		{m_lcmt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LCMT_UDRD_D& rData)
		{return m_lcmt.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_LCMT_UDRD_D,T_LCMT_UDRD_D&>m_lcmt;
};

#endif