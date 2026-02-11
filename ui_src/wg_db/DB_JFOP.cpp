#include "stdafx.h"
#include "DB_JFOP.h"

CDB_JFOP::CDB_JFOP()
{
	m_JFOP.InitHashTable(HASHSIZEJFOP);
}

CDB_JFOP::~CDB_JFOP()
{
	
}

void CDB_JFOP::Add(T_JFOP_D& rData)
{
	m_JFOP.SetAt(1,rData);
}

BOOL CDB_JFOP::Del()
{
	T_JFOP_D Data;
	BOOL bExist = m_JFOP.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret = m_JFOP.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
