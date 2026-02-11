#ifndef __DB_GLTMU_DB_H__
#define __DB_GLTMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GLTMU
{
public:
	CDB_GLTMU()
	{
		m_gltm.InitHashTable(HASHSIZEGLTM);
	}
	virtual ~CDB_GLTMU(){};

public:
	void Add(T_UDRD_KEY Key,T_GLTM_UDRD_D& rData)
		{m_gltm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gltm.RemoveKey(Key);}
	void DelAll()
		{m_gltm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GLTM_UDRD_D& rData)
		{return m_gltm.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_GLTM_UDRD_D,T_GLTM_UDRD_D&>m_gltm;
};

#endif