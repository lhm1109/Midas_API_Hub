#include "stdafx.h"
#include "DB_DYNZ.h"

CDB_DYNZ::CDB_DYNZ()
{
	m_dynz.InitHashTable(HASHSIZEDYNZ);
}

CDB_DYNZ::~CDB_DYNZ()
{

}

void CDB_DYNZ::Add(T_DYNZ_D& rData)
{
	m_dynz.SetAt(1,rData);
}

BOOL CDB_DYNZ::Del()
{
	T_DYNZ_D Data;
	BOOL bExist=m_dynz.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_dynz.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

CDB_NZLN::CDB_NZLN()
{
	m_nzln.InitHashTable(HASHSIZEDYNZ);
}

CDB_NZLN::~CDB_NZLN()
{

}

void CDB_NZLN::Add(T_NZLN_D& rData)
{
	m_nzln.SetAt(1, rData);
}

BOOL CDB_NZLN::Del()
{
	T_NZLN_D Data;
	BOOL bExist = m_nzln.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_nzln.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
