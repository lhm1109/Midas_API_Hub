#ifndef __DB_RLCOU_DB_H__
#define __DB_RLCOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RLCOU
{
public:
	CDB_RLCOU()
	{
		m_rlco.InitHashTable(HASHSIZERLCO);
	}
	virtual ~CDB_RLCOU(){};

public:
	void Add(T_UDRD_KEY Key,T_RLCO_UDRD_D& rData)
		{m_rlco.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rlco.RemoveKey(Key);}
	void DelAll()
		{m_rlco.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RLCO_UDRD_D& rData)
		{return m_rlco.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RLCO_UDRD_D,T_RLCO_UDRD_D&>m_rlco;
};

#endif