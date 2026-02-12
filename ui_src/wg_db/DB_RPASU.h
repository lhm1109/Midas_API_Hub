#ifndef __DB_RPASU_DB_H__
#define __DB_RPASU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RPASU
{
public:
	CDB_RPASU()
	{
		m_rpas.InitHashTable(HASHSIZERPAS);
	}
	virtual ~CDB_RPASU(){};

public:
	void Add(T_UDRD_KEY Key,T_RPAS_UDRD_D& rData)
		{m_rpas.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rpas.RemoveKey(Key);}
	void DelAll()
		{m_rpas.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RPAS_UDRD_D& rData)
		{return m_rpas.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RPAS_UDRD_D,T_RPAS_UDRD_D&>m_rpas;
};

#endif