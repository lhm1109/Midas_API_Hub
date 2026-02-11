#include "stdafx.h"
#include "DB_JDUS.h"

CDB_JDUS::CDB_JDUS()
{
	m_JDUS.InitHashTable(HASHSIZEJDUS);
}

CDB_JDUS::~CDB_JDUS()
{
	
}

void CDB_JDUS::Add(T_JDUS_D& rData)
{
	m_JDUS.SetAt(1,rData);
}

BOOL CDB_JDUS::Del()
{
	T_JDUS_D Data;
	BOOL bExist = m_JDUS.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDUS.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
