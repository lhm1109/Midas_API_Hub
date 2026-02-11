#include "stdafx.h"
#include "DB_ISLY.h"


CDB_ISLY::CDB_ISLY()
{
	m_isly.InitHashTable(HASHSIZEISLY);
}

CDB_ISLY::~CDB_ISLY()
{

}

void CDB_ISLY::Add(T_ISLY_D& rData)
{
	m_isly.SetAt(1, rData);
}

BOOL CDB_ISLY::Del()
{
	T_ISLY_D Data;
	BOOL bExist = m_isly.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_isly.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
