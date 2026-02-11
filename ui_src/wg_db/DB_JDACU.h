#ifndef __DB_JDACU_DB_H__
#define __DB_JDACU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDACU
{
public:
	CDB_JDACU()
	{
		m_JDAC.InitHashTable(HASHSIZEJDAC);
	}
	virtual ~CDB_JDACU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDAC_UDRD_D& rData)
		{m_JDAC.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDAC.RemoveKey(Key);}
	void DelAll()
		{m_JDAC.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDAC_UDRD_D& rData)
		{return m_JDAC.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDAC_UDRD_D,T_JDAC_UDRD_D&> m_JDAC;
};

#endif