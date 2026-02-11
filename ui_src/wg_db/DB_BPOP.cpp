#include "stdafx.h"
#include "DB_BPOP.h"

CDB_BPOP::CDB_BPOP()
{
	m_BPOP.InitHashTable(HASHSIZEBPOP);
}

CDB_BPOP::~CDB_BPOP()
{
	
}

void CDB_BPOP::Add(T_BPOP_D& rData)
{
	m_BPOP.SetAt(1,rData);
}

BOOL CDB_BPOP::Del()
{
	T_BPOP_D Data;
	BOOL bExist = m_BPOP.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_BPOP.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
