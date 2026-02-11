#include "stdafx.h"
#include "DB_JDCR.h"

CDB_JDCR::CDB_JDCR()
{
	m_JDCR.InitHashTable(HASHSIZEJDCR);
}

CDB_JDCR::~CDB_JDCR()
{
	
}

void CDB_JDCR::Add(T_JDCR_D& rData)
{
	m_JDCR.SetAt(1,rData);
}

BOOL CDB_JDCR::Del()
{
	T_JDCR_D Data;
	BOOL bExist = m_JDCR.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDCR.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
