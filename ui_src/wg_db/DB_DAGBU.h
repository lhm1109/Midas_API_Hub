#ifndef __DB_DAGBU_DB_H__
#define __DB_DAGBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DAGBU
{
public:
	CDB_DAGBU()
	{
		m_damping.InitHashTable(HASHSIZEDAGB);
	}
	virtual ~CDB_DAGBU(){};

public:
	void Add(T_UDRD_KEY Key,T_DAGB_UDRD_D& rData)
	{m_damping.SetAt(Key, rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_damping.RemoveKey(Key);}
	void DelAll()
	{m_damping.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DAGB_UDRD_D& rData)
		{return m_damping.Lookup(Key,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DAGB_UDRD_D,T_DAGB_UDRD_D&>m_damping;
};

#endif