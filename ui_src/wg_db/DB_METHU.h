#ifndef __DB_METHU_DB_H__
#define __DB_METHU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_METHU
{
public:
	CDB_METHU()
	{
		m_meth.InitHashTable(HASHSIZEMETH);
	}
	virtual ~CDB_METHU(){};

public:
	void Add(T_UDRD_KEY Key,T_METH_UDRD_D& rData)
		{m_meth.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_meth.RemoveKey(Key);}
	void DelAll()
		{m_meth.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_METH_UDRD_D& rData)
		{return m_meth.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_METH_UDRD_D,T_METH_UDRD_D&>m_meth;
};

#endif