#ifndef __DB_MVCTU_DB_H__
#define __DB_MVCTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVCTU
{
public:
	CDB_MVCTU()
	{
		m_mvct.InitHashTable(HASHSIZEMVCT);
	}
	virtual ~CDB_MVCTU(){};

public:
	void Add(T_UDRD_KEY Key,T_MVCT_UDRD_D& rData)
		{m_mvct.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvct.RemoveKey(Key);}
	void DelAll()
		{m_mvct.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVCT_UDRD_D& rData)
		{return m_mvct.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVCT_UDRD_D,T_MVCT_UDRD_D&>m_mvct;
};

#endif