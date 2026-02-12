#include "stdafx.h"
#include "DB_JDJO.h"

CDB_JDJO::CDB_JDJO()
{
	m_JDJO.InitHashTable(HASHSIZEJDJO);
}

CDB_JDJO::~CDB_JDJO()
{
	
}

void CDB_JDJO::Add(T_JDJO_D& rData)
{
	m_JDJO.SetAt(1,rData);
}

BOOL CDB_JDJO::Del()
{
	T_JDJO_D Data;
	BOOL bExist = m_JDJO.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDJO.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
