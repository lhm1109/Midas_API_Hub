#ifndef __DB_POCTU_DB_H__
#define __DB_POCTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POCTU
{
public:
	CDB_POCTU()
	{
		m_poct.InitHashTable(HASHSIZEPOCT);
	}
	virtual ~CDB_POCTU(){};

public:
	void Add(T_UDRD_KEY Key,T_POCT_UDRD_D& rData)
		{m_poct.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_poct.RemoveKey(Key);}
	void DelAll()
		{m_poct.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_POCT_UDRD_D& rData)
		{return m_poct.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_POCT_UDRD_D,T_POCT_UDRD_D&>m_poct;
};

#endif