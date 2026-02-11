#ifndef __DB_MODLU_DB_H__
#define __DB_MODLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MODLU
{
public:
	CDB_MODLU()
	{
		m_modl.InitHashTable(HASHSIZEMODL);
	}
	virtual ~CDB_MODLU(){};

public:
	void Add(T_UDRD_KEY Key,T_MODL_UDRD_D& rData)
		{m_modl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_modl.RemoveKey(Key);}
	void DelAll()
		{m_modl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MODL_UDRD_D& rData)
		{return m_modl.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MODL_UDRD_D,T_MODL_UDRD_D&>m_modl;
};

#endif