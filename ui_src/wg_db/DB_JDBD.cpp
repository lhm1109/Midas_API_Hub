#include "stdafx.h"
#include "DB_JDBD.h"

CDB_JDBD::CDB_JDBD()
{
	m_JDBD.InitHashTable(HASHSIZEJDBD);
}

CDB_JDBD::~CDB_JDBD()
{
	
}

void CDB_JDBD::Add(T_JDBD_D& rData)
{
	m_JDBD.SetAt(1,rData);
}

BOOL CDB_JDBD::Del()
{
	T_JDBD_D Data;
	BOOL bExist = m_JDBD.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDBD.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
