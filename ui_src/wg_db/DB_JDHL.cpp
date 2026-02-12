#include "stdafx.h"
#include "DB_JDHL.h"

CDB_JDHL::CDB_JDHL()
{
	m_JDHL.InitHashTable(HASHSIZEJDHL);
}

CDB_JDHL::~CDB_JDHL()
{
	
}

void CDB_JDHL::Add(T_JDHL_D& rData)
{
	m_JDHL.SetAt(1,rData);
}

BOOL CDB_JDHL::Del()
{
	T_JDHL_D Data;
	BOOL bExist = m_JDHL.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDHL.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
