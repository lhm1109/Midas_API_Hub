#ifndef __DB_LDSQU_DB_H__
#define __DB_LDSQU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LDSQU
{
public:
	CDB_LDSQU()
	{
		m_ldsq.InitHashTable(HASHSIZELDSQ);
	}
	virtual ~CDB_LDSQU(){};

public:
	void Add(T_UDRD_KEY Key,T_LDSQ_UDRD_D& rData)
		{m_ldsq.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ldsq.RemoveKey(Key);}
	void DelAll()
		{m_ldsq.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LDSQ_UDRD_D& rData)
		{return m_ldsq.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_LDSQ_UDRD_D,T_LDSQ_UDRD_D&>m_ldsq;
};

#endif