#ifndef __DB_RDPOU_DB_H__
#define __DB_RDPOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RDPOU
{
public:
	CDB_RDPOU()
	{
		m_rdpo.InitHashTable(HASHSIZERDPO);
	}
	virtual ~CDB_RDPOU(){};

public:
	void Add(T_UDRD_KEY Key,T_RDPO_UDRD_D& rData)
		{m_rdpo.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rdpo.RemoveKey(Key);}
	void DelAll()
		{m_rdpo.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RDPO_UDRD_D& rData)
		{return m_rdpo.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RDPO_UDRD_D,T_RDPO_UDRD_D&>m_rdpo;
};

#endif