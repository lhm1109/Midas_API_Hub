#ifndef __DB_NLCTU_DB_H__
#define __DB_NLCTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NLCTU
{
public:
	CDB_NLCTU()
	{
		m_nlct.InitHashTable(HASHSIZENLCT);
	}
	virtual ~CDB_NLCTU(){};

public:
	void Add(T_UDRD_KEY Key,T_NLCT_UDRD_D& rData)
		{m_nlct.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_nlct.RemoveKey(Key);}
	void DelAll()
		{m_nlct.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_NLCT_UDRD_D& rData)
		{return m_nlct.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_NLCT_UDRD_D,T_NLCT_UDRD_D&>m_nlct;
};

#endif