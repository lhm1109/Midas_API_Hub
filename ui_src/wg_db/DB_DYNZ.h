#ifndef __DB_DYNZ_DB_H__
#define __DB_DYNZ_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DYNZ
{
public:
	CDB_DYNZ();
	virtual ~CDB_DYNZ();

public:
	void Add(T_DYNZ_D& rData);
	BOOL Del();

public:
	BOOL Get(T_DYNZ_D& rData)
		{return m_dynz.Lookup(1,rData);}
	int GetCount()
		{return m_dynz.GetCount();}

protected:
	CMap<T_DYNZ_K,T_DYNZ_K,T_DYNZ_D,T_DYNZ_D&>m_dynz;
};

class CDB_NZLN
{
public:
	CDB_NZLN();
	virtual ~CDB_NZLN();

public:
	void Add(T_NZLN_D& rData);
	BOOL Del();

public:
	BOOL Get(T_NZLN_D& rData)
	{
		return m_nzln.Lookup(1, rData);
	}
	int GetCount()
	{
		return m_nzln.GetCount();
	}

protected:
	CMap<T_DYNZ_K, T_DYNZ_K, T_NZLN_D, T_NZLN_D&>m_nzln;
};

#endif