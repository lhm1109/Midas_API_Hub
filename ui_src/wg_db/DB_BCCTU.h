#ifndef __DB_BCCTU_DB_H__
#define __DB_BCCTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BCCTU
{
public:
	CDB_BCCTU()
	{
		m_bcct.InitHashTable(HASHSIZEBCCT);
	}
	virtual ~CDB_BCCTU(){};

public:
	void Add(T_UDRD_KEY Key,T_BCCT_UDRD_D& rData)
		{m_bcct.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_bcct.RemoveKey(Key);}
	void DelAll()
		{m_bcct.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BCCT_UDRD_D& rData)
		{return m_bcct.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BCCT_UDRD_D,T_BCCT_UDRD_D&>m_bcct;
};

#endif