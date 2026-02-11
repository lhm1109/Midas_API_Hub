#ifndef __DB_MPGBU_DB_H__
#define __DB_MPGBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MPGBU
{
public:
	CDB_MPGBU()
	{
		m_mpgb.InitHashTable(HASHSIZEMPGB);
	}
	virtual ~CDB_MPGBU(){};

public:
	void Add(T_UDRD_KEY Key,T_MPGB_UDRD_D& rData)
		{m_mpgb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mpgb.RemoveKey(Key);}
	void DelAll()
		{m_mpgb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MPGB_UDRD_D& rData)
		{return m_mpgb.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_MPGB_UDRD_D,T_MPGB_UDRD_D&>m_mpgb;
};

#endif