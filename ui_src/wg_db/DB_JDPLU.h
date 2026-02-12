#ifndef __DB_JDPLU_DB_H__
#define __DB_JDPLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DB_ST_DN_JUD.h"

class CDB_JDPLU
{
public:
	CDB_JDPLU()
	{
		m_JDPL.InitHashTable(HASHSIZEJDPL);
	}
	virtual ~CDB_JDPLU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDPL_UDRD_D& rData)
		{m_JDPL.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDPL.RemoveKey(Key);}
	void DelAll()
		{m_JDPL.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDPL_UDRD_D& rData)
		{return m_JDPL.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDPL_UDRD_D,T_JDPL_UDRD_D&> m_JDPL;
};

#endif