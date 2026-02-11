#ifndef __DB_JDUBU_DB_H__
#define __DB_JDUBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDUBU
{
public:
	CDB_JDUBU()
	{
		m_JDUB.InitHashTable(HASHSIZEJDUB);
	}
	virtual ~CDB_JDUBU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDUB_UDRD_D& rData)
		{m_JDUB.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDUB.RemoveKey(Key);}
	void DelAll()
		{m_JDUB.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDUB_UDRD_D& rData)
		{return m_JDUB.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDUB_UDRD_D,T_JDUB_UDRD_D&> m_JDUB;
};

#endif