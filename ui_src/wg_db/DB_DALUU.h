#ifndef __DB_DALUU_DB_H__
#define __DB_DALUU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DALUU
{
public:
	CDB_DALUU()
	{
		m_dalu.InitHashTable(HASHSIZEDALU);
	}
	virtual ~CDB_DALUU(){};

public:
	void Add(T_UDRD_KEY Key,T_DALU_UDRD_D& rData)
		{m_dalu.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dalu.RemoveKey(Key);}
	void DelAll()
		{m_dalu.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DALU_UDRD_D& rData)
		{return m_dalu.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DALU_UDRD_D,T_DALU_UDRD_D&>m_dalu;
};

#endif