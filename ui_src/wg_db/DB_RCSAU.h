#ifndef __DB_RCSAU_DB_H__
#define __DB_RCSAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RCSAU
{
public:
	CDB_RCSAU()
	{
		m_rcsa.InitHashTable(HASHSIZERCSA);
	}
	virtual ~CDB_RCSAU(){};

public:
	void Add(T_UDRD_KEY Key,T_RCSA_UDRD_D& rData)
		{m_rcsa.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rcsa.RemoveKey(Key);}
	void DelAll()
		{m_rcsa.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RCSA_UDRD_D& rData)
		{return m_rcsa.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RCSA_UDRD_D,T_RCSA_UDRD_D&>m_rcsa;
};

#endif