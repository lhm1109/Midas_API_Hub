#ifndef __DB_DYNZU_DB_H__
#define __DB_DYNZU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DYNZU
{
public:
	CDB_DYNZU()
	{
		m_dynz.InitHashTable(HASHSIZEDYNZ);
	}
	virtual ~CDB_DYNZU(){};

public:
	void Add(T_UDRD_KEY Key,T_DYNZ_UDRD_D& rData)
		{m_dynz.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dynz.RemoveKey(Key);}
	void DelAll()
		{m_dynz.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DYNZ_UDRD_D& rData)
		{return m_dynz.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_DYNZ_UDRD_D,T_DYNZ_UDRD_D&>m_dynz;
};

class CDB_NZLNU
{
public:
	CDB_NZLNU()
	{
		m_nzln.InitHashTable(HASHSIZEDYNZ);
	}
	virtual ~CDB_NZLNU() {};

public:
	void Add(T_UDRD_KEY Key, T_NZLN_UDRD_D& rData)
	{
		m_nzln.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_nzln.RemoveKey(Key);
	}
	void DelAll()
	{
		m_nzln.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_NZLN_UDRD_D& rData)
	{
		return m_nzln.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_NZLN_UDRD_D, T_NZLN_UDRD_D&>m_nzln;
};

#endif