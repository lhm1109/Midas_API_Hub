#ifndef __DB_ISGBU_DB_H__
#define __DB_ISGBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ISGBU
{
public:
	CDB_ISGBU()
	{
		m_Isolation.InitHashTable(HASHSIZEISGB);
	}
	virtual ~CDB_ISGBU(){};

public:
	void Add(T_UDRD_KEY Key,T_ISGB_UDRD_D& rData)
	{m_Isolation.SetAt(Key, rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_Isolation.RemoveKey(Key);}
	void DelAll()
	{m_Isolation.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ISGB_UDRD_D& rData)
		{return m_Isolation.Lookup(Key,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ISGB_UDRD_D,T_ISGB_UDRD_D&>m_Isolation;
};

#endif