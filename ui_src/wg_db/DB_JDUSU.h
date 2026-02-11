#ifndef __DB_JDUSU_DB_H__
#define __DB_JDUSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDUSU
{
public:
	CDB_JDUSU()
	{
		m_JDUS.InitHashTable(HASHSIZEJDUS);
	}
	virtual ~CDB_JDUSU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDUS_UDRD_D& rData)
		{m_JDUS.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDUS.RemoveKey(Key);}
	void DelAll()
		{m_JDUS.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDUS_UDRD_D& rData)
		{return m_JDUS.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDUS_UDRD_D,T_JDUS_UDRD_D&> m_JDUS;
};

#endif