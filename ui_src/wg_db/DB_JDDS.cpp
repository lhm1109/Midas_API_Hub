#include "stdafx.h"
#include "DB_JDDS.h"

CDB_JDDS::CDB_JDDS()
{
	m_JDDS.InitHashTable(HASHSIZEJDDS);
}

CDB_JDDS::~CDB_JDDS()
{
	
}

void CDB_JDDS::Add(T_JDDS_D& rData)
{
	m_JDDS.SetAt(1,rData);
}

BOOL CDB_JDDS::Del()
{
	T_JDDS_D Data;
	BOOL bExist = m_JDDS.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDDS.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
