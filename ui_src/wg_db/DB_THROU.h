#ifndef __DB_THROU_DB_H__
#define __DB_THROU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THROU
{
public:
	CDB_THROU()
	{
		m_thro.InitHashTable(HASHSIZETHRO);
	}
	virtual ~CDB_THROU(){};

public:
	void Add(T_UDRD_KEY Key,T_THRO_UDRD_D& rData)
		{m_thro.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_thro.RemoveKey(Key);}
	void DelAll()
		{m_thro.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THRO_UDRD_D& rData)
		{return m_thro.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THRO_UDRD_D,T_THRO_UDRD_D&>m_thro;
};

#endif