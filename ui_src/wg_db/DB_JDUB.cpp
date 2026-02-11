#include "stdafx.h"
#include "DB_JDUB.h"

CDB_JDUB::CDB_JDUB()
{
	m_JDUB.InitHashTable(HASHSIZEJDUB);
}

CDB_JDUB::~CDB_JDUB()
{
	
}

void CDB_JDUB::Add(T_JDUB_D& rData)
{
	m_JDUB.SetAt(1,rData);
}

BOOL CDB_JDUB::Del()
{
	T_JDUB_D Data;
	BOOL bExist = m_JDUB.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDUB.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
