#ifndef __DB_RPACU_DB_H__
#define __DB_RPACU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RPACU
{
public:
	CDB_RPACU()
	{
		m_rpac.InitHashTable(HASHSIZERPAC);
	}
	virtual ~CDB_RPACU(){};

public:
	void Add(T_UDRD_KEY Key,T_RPAC_UDRD_D& rData)
	{m_rpac.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_rpac.RemoveKey(Key);}
	void DelAll()
	{m_rpac.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RPAC_UDRD_D& rData)
	{return m_rpac.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_RPAC_UDRD_D,T_RPAC_UDRD_D&>m_rpac;
};

#endif