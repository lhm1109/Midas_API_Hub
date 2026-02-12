#ifndef __DB_ECUCU_DB_H__
#define __DB_ECUCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ECUCU
{
public:
	CDB_ECUCU()
	{
		m_ecuc.InitHashTable(HASHSIZEECUC);
	}
	virtual ~CDB_ECUCU(){};

public:
	void Add(T_UDRD_KEY Key,T_ECUC_UDRD_D& rData)
		{m_ecuc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ecuc.RemoveKey(Key);}
	void DelAll()
		{m_ecuc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ECUC_UDRD_D& rData)
		{return m_ecuc.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_ECUC_UDRD_D,T_ECUC_UDRD_D&>m_ecuc;
};

#endif