#include "stdafx.h"
#include "DB_ISGB.h"


CDB_ISGB::CDB_ISGB()
{
	m_Isolation.InitHashTable(HASHSIZEISGB);
}

CDB_ISGB::~CDB_ISGB()
{

}

void CDB_ISGB::Add(T_ISGB_D& rData)
{
	m_Isolation.SetAt(1, rData);
}

BOOL CDB_ISGB::Del()
{
	T_ISGB_D Data;
	BOOL bExist = m_Isolation.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_Isolation.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
