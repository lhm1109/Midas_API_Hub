#ifndef __DB_DYFGU_DB_H__
#define __DB_DYFGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DYFGU
{
public:
	CDB_DYFGU()
	{
		m_dyfg.InitHashTable(HASHSIZEDYFG);
	}
	virtual ~CDB_DYFGU(){};

public:
	void Add(T_UDRD_KEY Key,T_DYFG_UDRD_D& rData)
		{m_dyfg.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dyfg.RemoveKey(Key);}
	void DelAll()
		{m_dyfg.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DYFG_UDRD_D& rData)
		{return m_dyfg.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_DYFG_UDRD_D,T_DYFG_UDRD_D&>m_dyfg;
};

#endif