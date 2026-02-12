#ifndef __DB_RCCGU_DB_H__
#define __DB_RCCGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RCCGU
{
public:
	CDB_RCCGU()
	{
		m_rccg.InitHashTable(HASHSIZERCCG);
	}
	virtual ~CDB_RCCGU(){};

public:
	void Add(T_UDRD_KEY Key,T_RCCG_UDRD_D& rData)
		{m_rccg.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rccg.RemoveKey(Key);}
	void DelAll()
		{m_rccg.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RCCG_UDRD_D& rData)
		{return m_rccg.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RCCG_UDRD_D,T_RCCG_UDRD_D&>m_rccg;
};

#endif