#ifndef __DB_PPLCU_DB_H__
#define __DB_PPLCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PPLCU
{
public:
	CDB_PPLCU()
	{
		m_pplc.InitHashTable(HASHSIZEPPLC);
	}
	virtual ~CDB_PPLCU(){};

public:
	void Add(T_UDRD_KEY Key,T_PPLC_UDRD_D& rData)
		{m_pplc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pplc.RemoveKey(Key);}
	void DelAll()
		{m_pplc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PPLC_UDRD_D& rData)
		{return m_pplc.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PPLC_UDRD_D,T_PPLC_UDRD_D&>m_pplc;
};

#endif