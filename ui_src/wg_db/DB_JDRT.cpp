#include "stdafx.h"
#include "DB_JDRT.h"

CDB_JDRT::CDB_JDRT()
{
	m_JDRT.InitHashTable(HASHSIZEJDRT);
}

CDB_JDRT::~CDB_JDRT()
{
	
}

void CDB_JDRT::Add(T_JDRT_D& rData)
{
	m_JDRT.SetAt(1,rData);
}

BOOL CDB_JDRT::Del()
{
	T_JDRT_D Data;
	BOOL bExist = m_JDRT.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDRT.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
