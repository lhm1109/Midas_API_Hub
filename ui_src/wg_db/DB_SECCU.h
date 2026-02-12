#ifndef __DB_SECCU_DB_H__
#define __DB_SECCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECCU
{
public:
	CDB_SECCU()
	{
		m_secc.InitHashTable(HASHSIZESECC);
	}
	virtual ~CDB_SECCU(){};

public:
	void Add(T_UDRD_KEY Key,T_SECC_UDRD_D& rData)
		{m_secc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_secc.RemoveKey(Key);}
	void DelAll()
		{m_secc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SECC_UDRD_D& rData)
		{return m_secc.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SECC_UDRD_D,T_SECC_UDRD_D&>m_secc;
};

#endif