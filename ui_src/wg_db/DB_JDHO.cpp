#include "stdafx.h"
#include "DB_JDHO.h"

CDB_JDHO::CDB_JDHO()
{
	m_JDHO.InitHashTable(HASHSIZEJDHO);
}

CDB_JDHO::~CDB_JDHO()
{
	
}

void CDB_JDHO::Add(T_JDHO_D& rData)
{
	m_JDHO.SetAt(1,rData);
}

BOOL CDB_JDHO::Del()
{
	T_JDHO_D Data;
	BOOL bExist = m_JDHO.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDHO.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
