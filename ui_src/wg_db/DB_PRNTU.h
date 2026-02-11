#ifndef __DB_PRNTU_DB_H__
#define __DB_PRNTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PRNTU
{
public:
	CDB_PRNTU()
	{
		m_prnt.InitHashTable(HASHSIZEPRNT);
	}
	virtual ~CDB_PRNTU(){};

public:
	void Add(T_UDRD_KEY Key,T_PRNT_UDRD_D& rData)
		{m_prnt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_prnt.RemoveKey(Key);}
	void DelAll()
		{m_prnt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PRNT_UDRD_D& rData)
		{return m_prnt.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PRNT_UDRD_D,T_PRNT_UDRD_D&>m_prnt;
};

#endif