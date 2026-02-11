#ifndef __DB_BPOPU_DB_H__
#define __DB_BPOPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_BPOPU
{
public:
	CDB_BPOPU()
	{
		m_BPOP.InitHashTable(HASHSIZEBPOP);
	}
	virtual ~CDB_BPOPU(){};

public:
	void Add(T_UDRD_KEY Key,T_BPOP_UDRD_D& rData)
		{m_BPOP.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_BPOP.RemoveKey(Key);}
	void DelAll()
		{m_BPOP.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BPOP_UDRD_D& rData)
		{return m_BPOP.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BPOP_UDRD_D,T_BPOP_UDRD_D&> m_BPOP;
};

#endif