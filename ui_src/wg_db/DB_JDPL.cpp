#include "stdafx.h"
#include "DB_JDPL.h"

CDB_JDPL::CDB_JDPL()
{
	m_JDPL.InitHashTable(HASHSIZEJDPL);
}

CDB_JDPL::~CDB_JDPL()
{
	
}

void CDB_JDPL::Add(T_JDPL_D& rData)
{
	m_JDPL.SetAt(1,rData);
}

BOOL CDB_JDPL::Del()
{
	T_JDPL_D Data;
	BOOL bExist = m_JDPL.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDPL.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
