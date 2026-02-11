#ifndef __DB_LCOEU_DB_H__
#define __DB_LCOEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LCOEU
{
public:
	CDB_LCOEU()
	{
		m_lcoe.InitHashTable(HASHSIZELCOE);
	}
	virtual ~CDB_LCOEU(){};

public:
	void Add(T_UDRD_KEY Key,T_LCOE_UDRD_D& rData)
		{m_lcoe.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_lcoe.RemoveKey(Key);}
	void DelAll()
		{m_lcoe.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LCOE_UDRD_D& rData)
		{return m_lcoe.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LCOE_UDRD_D,T_LCOE_UDRD_D&>m_lcoe;
};

#endif