#ifndef __DB_EVCTU_DB_H__
#define __DB_EVCTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EVCTU
{
public:
	CDB_EVCTU()
	{
		m_evct.InitHashTable(HASHSIZEEVCT);
	}
	virtual ~CDB_EVCTU(){};

public:
	void Add(T_UDRD_KEY Key,T_EVCT_UDRD_D& rData)
		{m_evct.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_evct.RemoveKey(Key);}
	void DelAll()
		{m_evct.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_EVCT_UDRD_D& rData)
		{return m_evct.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_EVCT_UDRD_D,T_EVCT_UDRD_D&>m_evct;
};

#endif