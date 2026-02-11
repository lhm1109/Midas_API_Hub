#include "stdafx.h"
#include "DB_SEMT.h"

CDB_SEMT::CDB_SEMT()
{
	m_semt.InitHashTable(HASHSIZESEMT);
}

CDB_SEMT::~CDB_SEMT()
{

}

void CDB_SEMT::Add(T_SEMT_D& rData)
{
	m_semt.SetAt(1,rData);
}

BOOL CDB_SEMT::Del()
{
	T_SEMT_D Data;
	BOOL bExist=m_semt.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_semt.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

