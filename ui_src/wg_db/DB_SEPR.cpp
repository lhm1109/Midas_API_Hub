#include "stdafx.h"
#include "DB_SEPR.h"

CDB_SEPR::CDB_SEPR()
{
	m_sepr.InitHashTable(HASHSIZESEPR);
}

CDB_SEPR::~CDB_SEPR()
{

}

void CDB_SEPR::Add(T_SEPR_D& rData)
{
	m_sepr.SetAt(1,rData);
}

BOOL CDB_SEPR::Del()
{
	T_SEPR_D Data;
	BOOL bExist=m_sepr.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_sepr.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

