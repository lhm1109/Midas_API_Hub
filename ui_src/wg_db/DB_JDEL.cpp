#include "stdafx.h"
#include "DB_JDEL.h"

CDB_JDEL::CDB_JDEL()
{
	m_JDEL.InitHashTable(HASHSIZEJDEL);
}

CDB_JDEL::~CDB_JDEL()
{
	
}

void CDB_JDEL::Add(T_JDEL_D& rData)
{
	m_JDEL.SetAt(1,rData);
}

BOOL CDB_JDEL::Del()
{
	T_JDEL_D Data;
	BOOL bExist = m_JDEL.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JDEL.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
